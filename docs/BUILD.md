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

### docker compose
```
docker compose up --build merc
docker compose run merc make clean && make
```
The compose file binds the entire repo, so runtime files (`log/`, `player/`, etc.) stay on the host; it keeps the same host-port mapping (13838/11234/18888).

## Host-only build (fallback)
1. Install dependencies: `gcc`, `make`, `csh`, `libxcrypt-compat` (or `libxcrypt-dev`), `libncurses-dev`.
2. Run `scripts/bootstrap.sh` once to create writable directories.
3. Compile from `src/`: `make clean && make`.
4. Launch via `cd src && ./startup merc.ini` (requires `csh`).

## Maintenance scripts
- `scripts/check-data.py` – verifies UTF-8 encoding + structural markers across `area/`, `skill/`, `angel/`.
- `scripts/bootstrap.sh` – re-creates writable runtime directories (`log/`, `player/`, `mail/`, `debug/`, `vote/`).
