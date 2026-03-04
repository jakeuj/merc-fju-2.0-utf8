#!/usr/bin/env bash
set -euo pipefail

ROOT=${MERC_HOME:-/app}

if [[ -x "${ROOT}/scripts/bootstrap.sh" ]]; then
  "${ROOT}/scripts/bootstrap.sh" "${ROOT}"
fi

cd "${ROOT}/src"
make clean
make

if [[ $# -gt 0 ]]; then
  exec "$@"
fi

exec ./startup merc.ini
