#!/usr/bin/env bash
set -euo pipefail

ROOT=${1:-$(cd "$(dirname "$0")/.." && pwd)}
RUNTIME_DIRS=(log debug player mail vote)
PLAYER_BUCKETS=( {a..z} {A..Z} )
RENDER_SCRIPT="${ROOT}/scripts/render-merc-ini.sh"
NEED_RENDER=0

ensure_writable_dir() {
  local runtime_path="$1"
  mkdir -p "${runtime_path}"

  # Some bind-mounted filesystems (notably WSL drvfs) can reject chmod even
  # when the directory is already writable for the current user.
  if ! chmod 775 "${runtime_path}" 2>/dev/null; then
    if [[ ! -w "${runtime_path}" ]]; then
      echo "error: ${runtime_path} is not writable and chmod failed" >&2
      exit 1
    fi
    echo "warning: chmod 775 failed on ${runtime_path}; continuing with existing permissions" >&2
  fi
}

for dir in "${RUNTIME_DIRS[@]}"; do
  ensure_writable_dir "${ROOT}/${dir}"
done

# Merc stores characters under player/<bucket>/<name>/data, and adjust_filename()
# expects the 52 bucket directories to already exist.
for bucket in "${PLAYER_BUCKETS[@]}"; do
  ensure_writable_dir "${ROOT}/player/${bucket}"
done

# Ensure merc.ini exists where startup expects it.
if [[ ! -f "${ROOT}/src/merc.ini" ]]; then
  NEED_RENDER=1
fi

if [[ "${MERC_FORCE_RENDER_INI:-0}" == "1" ]]; then
  NEED_RENDER=1
fi

if [[ "${NEED_RENDER}" == "1" ]]; then
  if [[ ! -x "${RENDER_SCRIPT}" ]]; then
    echo "warning: ${RENDER_SCRIPT} is missing or not executable" >&2
  else
    "${RENDER_SCRIPT}" "${ROOT}" "${MERC_HOME_VALUE:-${ROOT}}"
  fi
fi

if [[ ! -f "${ROOT}/src/merc.ini" ]]; then
  echo "warning: ${ROOT}/src/merc.ini is missing" >&2
fi
