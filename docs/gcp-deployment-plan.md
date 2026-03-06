# GCP Compute Engine Deployment Plan for Merc-FJU MUD

## Summary
Create a reproducible pipeline that builds the existing Ubuntu 24.04 Docker image, stores it in Artifact Registry, provisions a Compute Engine VM with attached Persistent Disk for mutable game data, and configures systemd start-up, networking, logging, and backups so the MUD server runs reliably with state durability. Production target is GCP project `jk-mud-0304` in region `asia-east1` (current VM `merc-mud-1` lives in zone `asia-east1-b`).

---

## 1. Container Build & Release Flow
1. **Repo prep**  
   - Keep `/Users/jakeuj/auggie/mud2` as canonical source.  
   - `.dockerignore` excludes `player/ mail/ log/ debug/` etc. to shrink build context.  
   - `scripts/clean-runtime.sh` now runs inside `docker/Dockerfile`, scrubbing runtime folders (`player/ mail/ log/ debug/ vote/`) plus skip-worktree files (`data/immlist`, `etc/database`, `etc/address`, `etc/stock`, `board/*/list`, debug logs) so release images never include developer data.  
   - `player/` 必須保留所有字母分桶（`/player/a` … `/player/Z`，每個桶底下才會生成 `<角色名>/data`），因此清空 PD 後記得重跑 `scripts/clean-runtime.sh` 或以 `python`/bash 重新建立這 52 個子目錄；缺少它們會導致 `create_dir` 失敗。  
   - 檔案如 `etc/stock`、`etc/address`、`etc/database`、`data/immlist` 是在 runtime 更新但仍需要起始預設值的 Template，部署腳本應該把 repo 內的版本複製到 PD，而不是將它們刪除。  
2. **Artifact Registry**  
   - Create `LOCATION-docker.pkg.dev/PROJECT/merc-fju/merc-fju` repo.  
   - Authenticate local Docker (`gcloud auth configure-docker LOCATION-docker.pkg.dev`).  
3. **Build/Test**  
   - Run `docker build -t merc-fju:local -f docker/Dockerfile .`.  
   - Optional: `docker run --rm merc-fju:local ./src/merc --version` or smoke test via `docker-compose`.  
4. **Tag & Push**  
   - `docker tag merc-fju:local LOCATION-docker.pkg.dev/PROJECT/merc-fju/merc-fju:YYYYMMDD-N`.  
   - `docker push ...`.  
   - Mirror to Docker Hub for non-GCP consumers: `docker tag merc-fju:local jakeuj/merc-fju-2.0-utf8:YYYYMMDD-N` then `docker push jakeuj/merc-fju-2.0-utf8:YYYYMMDD-N`. Pulling this image on prod avoids rebuilds on the VM.  
5. **Release records**  
   - Maintain CHANGELOG entry per image tag (commit hash, build date, Merc data snapshot).  

## 2. Infrastructure (Compute Engine)
1. **Network & IAM**  
   - Ensure VPC/subnet with egress path to Internet; reserve static external IP for telnet clients.  
   - Create service account `merc-fju-vm` with roles: `Artifact Registry Reader`, `Logging Writer`, `Monitoring Metric Writer`.  
2. **Firewall**  
   - Create ingress rule allowing TCP `3838,1234,8888` (game ports) and `22` for SSH from admin CIDRs only.  
3. **Persistent Disk layout**  
   - 50 GB balanced PD (adjustable) mounted at `/srv/merc-data`.  
   - Directories on PD: `player/ mail/ board/ vote/ log/ debug/ etc/`; also persist whitelist-related files `etc/address`, `data/server`, and `data/immlist` so IP and immortal ACLs survive rebuilds.  
   - Docker image now declares the directory paths as `VOLUME`s (see `docker/Dockerfile`), so the PD must be bind-mounted into each of them at runtime or data will be ephemeral. Keep `/app/etc/address`, `/app/data/server`, and `/app/data/immlist` as file bind-mounts instead of anonymous volumes.  
   - `scripts/bootstrap.sh` creates runtime folders and can backfill `src/merc.ini` from the tracked `etc/merc.ini` template. Seed PD the first time by copying repo defaults (`cp -r area etc data/server data/immlist ...`) before starting the container. Keep runtime `HOME DIRECTORY` at `/app`; the Persistent Disk is only the host-side bind-mount source.
4. **VM specs**  
   - `e2-small` (2 vCPU/2 GB) baseline; allow vertical scaling via instance template.  
   - Boot disk: 20 GB Ubuntu 24.04 LTS.  
   - Metadata startup script installs Docker & docker compose plugin, mounts PD, pulls image, writes systemd unit.  

## 3. Runtime Configuration
1. **Mount & permissions**  
   - Format PD (`mkfs.ext4`), mount via `/etc/fstab`: `/dev/disk/by-id/google-merc-data /srv/merc-data ext4 defaults,nofail 0 2`.  
   - `chown mud:mud /srv/merc-data`.  
2. **Data layout**  
   - On first boot, copy repo subdirs that must persist (`player/ mail/ board/ vote/ log/ debug/ etc/`) and the `data/server` whitelist file plus `data/immlist` (immortal list) onto PD; keep codebase read-only under `/opt/merc`.  
   - Do not repoint `HOME DIRECTORY` to the host PD path. Production should keep the generated `src/merc.ini` at `HOME DIRECTORY=/app`; adjust only the bind mounts and any gameplay flags such as `Check Server`.
3. **Container runtime**  
   - Image ships with `VOLUME ["/app/player", "/app/mail", "/app/board", "/app/vote", "/app/log", "/app/debug", "/app/etc"]`; mount each one to its PD-backed path before startup, and add `-v /srv/merc-data/data/server:/app/data/server -v /srv/merc-data/data/immlist:/app/data/immlist -v /srv/merc-data/etc/address:/app/etc/address` to persist whitelist artifacts.  
   - Entrypoint `/app/docker/entrypoint.sh` respects `MERC_HOME` (default `/app`), runs `scripts/bootstrap.sh`, backfills `src/merc.ini` when missing (or when `MERC_FORCE_RENDER_INI=1`), and conditionally rebuilds `src/merc` when missing or when `MERC_FORCE_BUILD=1`. Set `MERC_HOME=/app` in production unless you deliberately relocate the tree.
   - `docker run --name merc -d --restart unless-stopped \`  
     `-p 3838:3838 -p 1234:1234 -p 8888:8888 \`  
     `-v /srv/merc-data/player:/app/player ...` (repeat for each dir)  
     `-e MERC_HOME=/app \`  
     `LOCATION-docker.pkg.dev/...:TAG`.  
   - Alternatively, author a `docker-compose.yml` referencing the Artifact Registry image and PD mounts; install `docker compose` plugin and run via systemd.  
4. **systemd unit**  
   - File `/etc/systemd/system/merc.service` runs compose up (or docker run) after network-online.target & PD mount; enables automatic restart.  

## 4. Observability & Backups
1. **Cloud Logging**  
   - Stream container stdout/stderr; optionally tee `/srv/merc-data/log` into Cloud Logging via Ops Agent (install and configure).  
2. **Monitoring**  
   - Expose custom metrics (CPU, memory, open sockets) using Cloud Monitoring agent.  
   - Schedule uptime check for TCP 3838 from multiple regions; page if down >1 min.  
3. **Backups**  
   - Nightly snapshot of PD via Cloud Scheduler + Cloud Functions/Run job (`gcloud compute disks snapshot`). Retain 7 daily + 4 weekly copies.  
   - Secondary: `gsutil rsync -r /srv/merc-data gs://PROJECT-merc-backup/YYYY-MM-DD` triggered post-snapshot.  
4. **Secrets**  
   - Store admin credentials/contact emails in Secret Manager; mount via env vars or entrypoint as needed.  

## 5. Deployment / Rollout Procedure
1. **Initial bring-up**  
   - Push image; create PD; provision VM `merc-mud-1` in `asia-east1-b` with startup script referencing Artifact Registry tag.  
   - SSH in (`gcloud compute ssh merc-mud-1 --zone=asia-east1-b`), verify `docker ps`, inspect `log/*.log`, ensure telnet connectivity from trusted client IPs (currently 211.20.19.206) after confirming the address whitelist.  
2. **Blue/Green updates**  
   - Use Instance Template + Managed Instance Group of size 1 for controlled rollouts:  
     - Create new template pinned to new image tag, update MIG; once healthy, delete old instance.  
   - Alternatively, stop container, pull new tag, start container; ensure `player/` etc. remain mounted.  
3. **Disaster recovery drill**  
   - Document steps to recreate VM from latest snapshot + Artifact Registry image.  
   - Test restoration quarterly.  

## 6. Testing & Validation
- **Container smoke test**: `docker run --rm merc-fju:TAG telnet localhost 3838` inside container to ensure ports open.  
- **Whitelist test**: on the VM, confirm `/srv/merc-data/etc/address` and `/srv/merc-data/data/server` include the current admin IPs, then telnet from the same IP and ensure the login banner proceeds past “檢查連線位址﹐請稍待片刻...”.  
- **Functional**: connect from external host, run basic commands, verify persistence by relogging.  
- **Failover**: reboot VM; confirm systemd restarts container and PD auto-mounts.  
- **Backup restore test**: mount snapshot to temp VM, verify `player/` contents readable.  
- **Security**: port scan from outside VPC, confirm only intended ports open.  

## 7. Assumptions & Defaults
- Deployment uses single Compute Engine VM (no auto-scaling).  
- Telnet ports remain 3838/1234/8888; TLS termination handled elsewhere if needed.  
- 50 GB PD sufficient; adjust per player growth.  
- Docker image built from current `docker/Dockerfile` without modifications.  
- Ops team comfortable managing systemd + Docker on VM.
