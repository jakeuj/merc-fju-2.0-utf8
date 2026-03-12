# Merc Local Ops Cheatsheet

## Environment Split
- Linux/macOS/WSL shell: run `./start-merc.sh start`
- Windows PowerShell: run `.\start-merc.ps1 start`
- Windows cmd / quick launcher: run `.\start-merc.cmd start`
- Do not run `src/merc` directly from PowerShell or cmd; it is a Linux ELF binary

## Standard Startup Paths

### Linux / macOS / WSL
```bash
./start-merc.sh start
./start-merc.sh status
./start-merc.sh stop
```

### Windows -> WSL wrapper
```powershell
.\start-merc.ps1 start
.\start-merc.ps1 status
.\start-merc.ps1 stop
```

```cmd
.\start-merc.cmd start
```

- `start-merc.ps1` converts a Windows repo path like `H:\repos\merc-fju-2.0-utf8`
  into `/mnt/h/repos/merc-fju-2.0-utf8`, then calls `./start-merc.sh` inside WSL.
- If multiple distros exist, use `.\start-merc.ps1 start -Distro Ubuntu-24.04`.

## Rebuild and Re-render merc.ini

### Re-render config after path changes
```bash
MERC_FORCE_RENDER_INI=1 \
MERC_HOME_VALUE=/mnt/h/repos/merc-fju-2.0-utf8 \
./scripts/bootstrap.sh .
```

### Rebuild binary
```bash
cd src
make clean && make
cd ..
```

## Common Failures

### `log is not writable`
Meaning:
- `scripts/bootstrap.sh` tried to create or chmod a runtime directory, but the current WSL user cannot write there.

Check:
```bash
ls -ld log player mail debug vote
test -w log && echo writable || echo not-writable
whoami
```

Repair:
```bash
sudo chown -R "$(whoami)":"$(whoami)" log player mail debug vote
chmod -R u+rwX log player mail debug vote
```

Notes:
- On `/mnt/<drive>` mounts, `chmod` can fail even when the folder is already writable.
- Treat it as a real blocker only when `test -w <dir>` also fails.

### Immediate exit after launch
Meaning:
- `src/shutdown.txt` may still exist, or the process fails during early boot.

Check:
```bash
ls src/shutdown.txt
ls -t log | head
```

Repair:
```bash
rm -f src/shutdown.txt
./start-merc.sh start
```

### `merc.ini` points to the wrong path
Meaning:
- `HOME DIRECTORY` still references an old host path, often after moving from macOS to WSL.

Check:
```bash
grep -n "HOME DIRECTORY" src/merc.ini
pwd
```

Repair:
```bash
MERC_FORCE_RENDER_INI=1 \
MERC_HOME_VALUE="$(pwd)" \
./scripts/bootstrap.sh .
```

### Wrapper works but server still fails
Meaning:
- Windows -> WSL forwarding succeeded; the failure is inside WSL, usually build/config/permission related.

Check:
```powershell
.\start-merc.ps1 status
```

Then continue inside WSL:
```bash
./start-merc.sh status
ls -t log | head
```

## Verification
- Process status: `./start-merc.sh status`
- Listening ports: `ss -ltn | grep -E ':(3838|1234|8888)[[:space:]]'`
- Recent logs: `ls -t log | head`
- Live logs: `tail -f log/manual-start-YYYYmmdd-HHMMSS.log`
