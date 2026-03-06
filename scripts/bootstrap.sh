#!/usr/bin/env bash
set -euo pipefail

ROOT=${1:-$(cd "$(dirname "$0")/.." && pwd)}
RUNTIME_DIRS=(log debug player mail vote)
RENDER_SCRIPT="${ROOT}/scripts/render-merc-ini.sh"
NEED_RENDER=0

for dir in "${RUNTIME_DIRS[@]}"; do
  mkdir -p "${ROOT}/${dir}"
  chmod 775 "${ROOT}/${dir}"
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
