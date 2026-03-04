# Merc-FJU Data Baseline

- Core content directories confirmed on 2026-03-04: `area/`, `angel/`, `command/`, `skill/`, `social/`, `board/`, `help/`, `document/`.
- UTF-8 verification: `file area/limbo/roo/1.roo` reports `Unicode text, UTF-8 text`.
- Runtime-generated directories (`log/`, `debug/`, `player/`, `mail/`, `vote/`) are intentionally ignored via `.gitignore`.
- `src/merc.ini` is the active configuration; see `docs/merc.ini.snapshot` for the pre-modernization copy.

Keep this file updated if authoritative content changes so we always know what must stay untouched.
