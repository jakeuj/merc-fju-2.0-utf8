# Merc-FJU Build & Run Guide

## Docker (recommended)
1. `make docker-build` – builds `merc-fju` using `docker/Dockerfile` (Ubuntu 24.04, gcc, csh, libxcrypt).
2. `make docker-run` – launches the image, binds the repo into `/app`, and maps host ports 13838→3838, 11234→1234, 18888→8888.
3. `docker/Dockerfile` renders `src/merc.ini` from `etc/merc.ini` during image build, always using `HOME DIRECTORY=/app`.
4. Inside the container the entrypoint runs `scripts/bootstrap.sh`, `make clean && make`, and finally `src/startup merc.ini`.
5. Attach via `docker exec -it merc-fju /bin/bash` for debugging or use `make docker-shell`.

### Image hygiene
- `docker/Dockerfile` calls `scripts/clean-runtime.sh` right after `COPY . /app`, which wipes `player/ mail/ log/ debug/ vote/` and truncates `data/immlist`, `etc/database`, `etc/address`, `etc/stock`, `board/*/list`, and debug logs so no local player data sneaks into the release image.
- `.dockerignore` excludes `.git/`, runtime folders, IDE junk, etc., shrinking the context that reaches Docker.
- To inspect or pre-clean a staging directory outside of Docker, run `bash scripts/clean-runtime.sh /absolute/path/to/stage` (never point it at a live volume you still need).
- Even though `player/` is runtime-only, the game stores player data as `player/<lowercase-first-letter>/<name>/data`. The server now auto-creates missing buckets/player directories during save; `clean-runtime.sh` still pre-creates/cleans `player/` so staging and image builds start from a tidy runtime tree.
- Some mutable files must be restored to their defaults instead of removed—`etc/stock`, `etc/address`, `etc/database`, `data/immlist`, `board/*/list`—because the server reads them during boot. Keep template copies in the repo and copy them over when seeding a new volume.

### docker compose
```
docker compose up --build merc
docker compose run merc make clean && make
```
The compose file binds the entire repo, so runtime files (`log/`, `player/`, etc.) stay on the host; it keeps the same host-port mapping (13838/11234/18888).

## Host-only build (fallback)
> 適用於 macOS（含 Apple Silicon）與一般 Linux 主機，不需要 Docker。

1. Install dependencies/toolchain:
   - macOS: `xcode-select --install` and `brew install csh`.
   - Linux: `gcc`, `make`, `csh`, `libxcrypt-compat` (or `libxcrypt-dev`), `libncurses-dev`.
2. Run `make bootstrap` (or `scripts/bootstrap.sh`) once to create writable runtime directories and generate `src/merc.ini` from `etc/merc.ini` using your current repo absolute path.
3. If the repo path changes, re-render with `MERC_FORCE_RENDER_INI=1 make bootstrap` or `make render-merc-ini`.
4. Compile from `src/`: `make clean && make`.
5. Launch via `cd src && ./startup merc.ini` (requires `csh`).
6. Local testing can use `nc localhost 3838` (or `telnet`) and runtime data will stay on the host filesystem.

## Maintenance scripts
- `scripts/check-data.py` – verifies UTF-8 encoding + structural markers across `area/`, `skill/`, `angel/`.
- `scripts/bootstrap.sh` – re-creates writable runtime directories (`log/`, `player/`, `mail/`, `debug/`, `vote/`) and backfills `src/merc.ini` when missing.
- `scripts/render-merc-ini.sh` – renders `src/merc.ini` from `etc/merc.ini`, overriding `HOME DIRECTORY` with the provided path, `MERC_HOME_VALUE`, or the repo root.
