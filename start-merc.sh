#!/usr/bin/env bash
# Quick launcher for 三國歪傳之降龍伏虎 (Merc-FJU) on local/dev hosts.

set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
SRCDIR="$ROOT/src"
BIN="$SRCDIR/merc"
INI="$SRCDIR/merc.ini"
SHUTDOWN_FLAG="$SRCDIR/shutdown.txt"
LOGDIR="$ROOT/log"
PIDFILE="$LOGDIR/merc.pid"
BOOTSTRAP="$ROOT/scripts/bootstrap.sh"

echo "[start-merc] working dir: $ROOT"

if [[ -x "$BOOTSTRAP" ]]; then
  echo "[start-merc] ensuring runtime dirs via scripts/bootstrap.sh"
  "$BOOTSTRAP" "$ROOT"
fi

if [[ ! -x "$BIN" ]]; then
  echo "[start-merc] 找不到 ${BIN}，嘗試自動編譯 (cd src && make)"
  (cd "$SRCDIR" && make)
fi

if [[ ! -x "$BIN" ]]; then
  echo "[start-merc] error: 編譯後仍找不到 ${BIN}，請手動檢查 make 是否成功" >&2
  exit 1
fi

if [[ ! -f "$INI" ]]; then
  echo "[start-merc] error: 找不到設定檔 $INI" >&2
  exit 1
fi

mkdir -p "$LOGDIR"

if [[ -f "$SHUTDOWN_FLAG" ]]; then
  echo "[start-merc] 偵測到舊的 shutdown.txt，準備移除以允許重新啟動"
  rm -f "$SHUTDOWN_FLAG"
fi

timestamp="$(date +%Y%m%d-%H%M%S)"
LOGFILE="$LOGDIR/manual-start-$timestamp.log"

echo "[start-merc] launching ${BIN}, log -> $LOGFILE"
"$BIN" "$INI" >>"$LOGFILE" 2>&1 &
pid=$!

echo "$pid" > "$PIDFILE"
echo "[start-merc] merc pid=${pid} (記錄於 $PIDFILE)"
echo "[start-merc] tail -f \"$LOGFILE\" to watch server boot"
echo "[start-merc] stop via in-game shutdown or run: kill ${pid}"
