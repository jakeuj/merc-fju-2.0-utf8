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

ensure_binary() {
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
}

do_start() {
  ensure_binary
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
  echo "[start-merc] stop via ./start-merc.sh stop 或 kill ${pid}"
}

running_pid() {
  if [[ -f "$PIDFILE" ]]; then
    cat "$PIDFILE"
  fi
  return 0
}

is_running() {
  local pid="$1"
  [[ -n "$pid" ]] && ps -p "$pid" >/dev/null 2>&1
}

do_stop() {
  local pid
  pid="$(running_pid)"
  if [[ -z "$pid" ]]; then
    echo "[start-merc] 沒有找到 PID 檔案，將嘗試使用 pgrep"
    pid="$(pgrep -f "$BIN")" || true
    if [[ -z "$pid" ]]; then
      echo "[start-merc] 伺服器似乎沒有在執行"
      return
    fi
  fi

  if is_running "$pid"; then
    echo "[start-merc] 正在停止 PID=${pid}"
    kill "$pid" 2>/dev/null || true
    sleep 1
    if is_running "$pid"; then
      echo "[start-merc] PID ${pid} 尚未結束，可再執行 kill ${pid} 或使用 kill -9" >&2
    else
      echo "[start-merc] 伺服器已停止"
    fi
  else
    echo "[start-merc] PID ${pid} 未在執行"
  fi

  rm -f "$PIDFILE"
}

do_status() {
  local pid
  pid="$(running_pid)"
  if [[ -n "$pid" ]]; then
    if is_running "$pid"; then
      echo "[start-merc] RUNNING - PID ${pid}"
      return
    fi
  fi

  pid="$(pgrep -f "$BIN")" || true
  if [[ -n "$pid" ]]; then
    echo "[start-merc] RUNNING - PID ${pid} (pgrep)"
  else
    echo "[start-merc] STOPPED"
  fi
}

ACTION="${1:-start}"

case "$ACTION" in
  start) do_start ;;
  stop) do_stop ;;
  restart)
    do_stop
    do_start
    ;;
  status) do_status ;;
  *)
    echo "用法: $0 [start|stop|restart|status]" >&2
    exit 1
    ;;
esac
