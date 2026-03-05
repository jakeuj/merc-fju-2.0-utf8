#!/usr/bin/env bash
set -euo pipefail

ROOT=${MERC_HOME:-/app}

if [[ -x "${ROOT}/scripts/bootstrap.sh" ]]; then
  "${ROOT}/scripts/bootstrap.sh" "${ROOT}"
fi

cd "${ROOT}/src"

NEED_BUILD=0
if [[ ! -x "${ROOT}/src/merc" ]]; then
  NEED_BUILD=1
fi

if [[ "${MERC_FORCE_BUILD:-0}" == "1" ]]; then
  NEED_BUILD=1
fi

if [[ "${NEED_BUILD}" == "1" ]]; then
  make clean
  make
fi

if [[ $# -gt 0 ]]; then
  exec "$@"
fi

exec ./startup merc.ini
