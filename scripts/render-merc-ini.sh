#!/usr/bin/env bash
set -euo pipefail

ROOT=${1:-$(cd "$(dirname "$0")/.." && pwd)}
HOME_VALUE=${2:-${MERC_HOME_VALUE:-$ROOT}}
TEMPLATE_PATH="${ROOT}/etc/merc.ini"
OUTPUT_PATH="${ROOT}/src/merc.ini"
TMP_PATH="${OUTPUT_PATH}.tmp"

if [[ ! -f "${TEMPLATE_PATH}" ]]; then
  echo "error: merc.ini template not found at ${TEMPLATE_PATH}" >&2
  exit 1
fi

mkdir -p "$(dirname "${OUTPUT_PATH}")"

awk -v home_dir="${HOME_VALUE}" '
  BEGIN { replaced = 0 }
  /^[[:space:]]*HOME DIRECTORY[[:space:]]*=/ {
    printf "HOME DIRECTORY  \t=\t%s\n", home_dir
    replaced = 1
    next
  }
  { print }
  END {
    if (!replaced) {
      print "error: HOME DIRECTORY not found in template" > "/dev/stderr"
      exit 1
    }
  }
' "${TEMPLATE_PATH}" > "${TMP_PATH}"

mv "${TMP_PATH}" "${OUTPUT_PATH}"
chmod 664 "${OUTPUT_PATH}"

echo "Rendered ${OUTPUT_PATH} with HOME DIRECTORY=${HOME_VALUE}"