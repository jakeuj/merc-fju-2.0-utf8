# Changelog

## 2026-03-04
- Documented the legacy data baseline (`docs/BASELINE.md`) and captured `src/merc.ini` as `docs/merc.ini.snapshot`.
- Added Docker-based toolchain (`docker/Dockerfile`, `docker/entrypoint.sh`, `docker-compose.yml`) plus Makefile helpers for container builds.
- Introduced bootstrap and data-check scripts under `scripts/`.
- Updated `.gitignore` to ignore runtime artifacts and added developer docs (`docs/BUILD.md`).
- Modernized the C tree: renamed struct members `restrictions`, fixed the `:~` token literal, enabled `_XOPEN_SOURCE`, and linked `-lcrypt`.
- Added runtime-optional System V IPC handling so shared-memory features log-and-skip when the kernel refuses allocation, instead of crashing.
