#!/usr/bin/env bash
set -euo pipefail

ROOT=${1:-$(cd "$(dirname "$0")/.." && pwd)}
RUNTIME_DIRS=(log debug player mail vote)

for dir in "${RUNTIME_DIRS[@]}"; do
  mkdir -p "${ROOT}/${dir}"
  chmod 775 "${ROOT}/${dir}"
done

# Ensure merc.ini exists where startup expects it.
if [[ ! -f "${ROOT}/src/merc.ini" ]]; then
  echo "warning: ${ROOT}/src/merc.ini is missing" >&2
fi
