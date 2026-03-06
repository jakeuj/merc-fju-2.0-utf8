# Merc-FJU (三國歪傳之降龍伏虎) Quick Reference

## Paths
- Repo root: `/Users/jakeuj/auggie/mud2`
- Executable target: `src/merc`
- Primary config: `src/merc.ini` (copy alongside runtime or to `etc/merc.ini` as needed)

## Directory Map
| Path | Purpose |
| --- | --- |
| `src/` | C sources, Makefiles, `startup` launcher, default `merc.ini` |
| `area/` | World areas (rooms, mobs, resets, shops) |
| `angel/` | Guardian NPC definitions |
| `command/`, `skill/`, `social/` | Command, skill, and social text definitions |
| `include/` | Shared headers for job/class/terrain data |
| `etc/` | Runtime configs like `merc.ini` when deployed |
| `data/` | System data tables |
| `document/`, `doc/` | Localized + upstream docs and licenses |
| `greeting/`, `help/`, `joke/` | Player-facing text |
| `board/`, `mail/`, `log/`, `debug/`, `player/`, `vote/` | Generated data at runtime |

## Build Steps
```bash
cd /Users/jakeuj/auggie/mud2/src
make clean && make           # Linux default Makefile
# For BSD: cp Makefile.bsd Makefile before make
```
- Requires `gcc`, `make`, and `crypt` library (standard on Linux/macOS via libcrypto/libcrypt).
- Output `merc` binary stays in `src/`.

## Configuration
Edit `src/merc.ini` (or deployed copy in `etc/merc.ini`). Key fields:
- `MUD PORT` (multiple entries allowed, each >1024)
- `NAME` shown to players (UTF-8 supported)
- `HOME DIRECTORY` absolute path to repo root
- `IPC KEY`, `IPC Block`, `MAXDESC`, idle limits
- `HELP/SOCIAL/ANGEL/AREA DIRECTORY` paths are relative to `HOME DIRECTORY`
- Security toggles (`Strict Password`, `Strict Email`, `Multi login`, etc.)
- Runtime defaults for angels, logging, backup flags

Copy updated config next to the executable before launching.

## Launching & Logs
```bash
cd /Users/jakeuj/auggie/mud2/src
./startup &   # csh script; keeps merc running and logs to ../log/<n>.log
```
- `startup` enforces stack limits, appends output to `log/###.log`, watches for `shutdown.txt` flag.
- `shutdown.txt` lets you stop the loop gracefully.

## Data Editing Tips
- Area files use classic Merc formats; follow templates in `document/*.txt`.
- `document/mob.txt`, `obj.txt`, `room.txt`, `reset.txt`, `shop.txt` describe field ordering.
- Keep UTF-8 encoding when editing; original repo converted from Big5 via `convert_big5_to_utf8.py`.
- Use `help/` and `greeting/` directories for player messaging updates.

## Licensing Notes
- Upstream Diku/Merc terms in `doc/license.*`; local COPYRIGHT at `document/COPYRIGHT`.
- Redistribution allowed for non-commercial use with credits intact.
