# Merc-FJU Build & Run Guide

## Docker (recommended)
1. `make docker-build` – builds `merc-fju` using `docker/Dockerfile` (Ubuntu 24.04, gcc, csh, libxcrypt).
2. `make docker-run` – launches the image, binds the repo into `/app`, and maps host ports 13838→3838, 11234→1234, 18888→8888.
3. Inside the container the entrypoint runs `scripts/bootstrap.sh`, `make clean && make`, and finally `src/startup merc.ini`.
4. Attach via `docker exec -it merc-fju /bin/bash` for debugging or use `make docker-shell`.

### Image hygiene
- `docker/Dockerfile` calls `scripts/clean-runtime.sh` right after `COPY . /app`, which wipes `player/ mail/ log/ debug/ vote/` and truncates `data/immlist`, `etc/database`, `etc/address`, `etc/stock`, `board/*/list`, and debug logs so no local player data sneaks into the release image.
- `.dockerignore` excludes `.git/`, runtime folders, IDE junk, etc., shrinking the context that reaches Docker.
- To inspect or pre-clean a staging directory outside of Docker, run `bash scripts/clean-runtime.sh /absolute/path/to/stage` (never point it at a live volume you still need).
- Even though `player/` is runtime-only, the game assumes the alphabetical buckets (`player/a` … `player/Z`, bucket/name/data) already exist; `clean-runtime.sh` recreates them, so if you manually wipe a Persistent Disk make sure to re-run the script or re-create those directories before starting the server.
- Some mutable files must be restored to their defaults instead of removed—`etc/stock`, `etc/address`, `etc/database`, `data/immlist`, `board/*/list`—because the server reads them during boot. Keep template copies in the repo and copy them over when seeding a new volume.

### docker compose
```
docker compose up --build merc
docker compose run merc make clean && make
```
The compose file binds the entire repo, so runtime files (`log/`, `player/`, etc.) stay on the host; it keeps the same host-port mapping (13838/11234/18888).

## Host-only build (fallback)
> 適用於 macOS（含 Apple Silicon）與一般 Linux 主機，不需要 Docker。

1. 安裝工具鏈  
   - macOS：`xcode-select --install` 取得 clang/make，`brew install csh` 以提供啟動腳本所需的 C shell。  
   - Linux：`sudo apt install build-essential csh libxcrypt-compat`（或等價套件）。若發生 `crypt` 連結錯誤，可另外安裝 `libxcrypt-dev`。
2. （第一次在新路徑執行時）跑 `scripts/bootstrap.sh`，確保 `log/ player/ mail/ debug/ vote/` 等 runtime 目錄存在並具寫入權限。
3. 進入 `src/`：`make clean && make`。macOS 會自動使用 clang，無需額外 flags。
4. 編輯 `src/merc.ini` 或 `etc/merc.ini`，將 `HOME DIRECTORY` 改成實際專案路徑，並確認 `MUD PORT` 未被其他服務占用。
5. 啟動方式與舊版一致：`cd src && ./startup merc.ini &`。該 csh 迴圈會自動輪替 `log/*.log` 並在偵測 `shutdown.txt` 時關閉。
6. 本機測試可直接 `nc localhost 3838`（或系統內建 `telnet`）登入，所有玩家/日誌資料都寫回主機目錄。

## Maintenance scripts
- `scripts/check-data.py` – verifies UTF-8 encoding + structural markers across `area/`, `skill/`, `angel/`.
- `scripts/bootstrap.sh` – re-creates writable runtime directories (`log/`, `player/`, `mail/`, `debug/`, `vote/`).
