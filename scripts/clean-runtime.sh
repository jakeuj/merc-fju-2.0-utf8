#!/usr/bin/env bash
set -euo pipefail

ROOT=${1:-$(cd "$(dirname "$0")/.." && pwd)}
PLAYER_BUCKETS=( {a..z} )

clean_dir() {
  local rel="$1"
  local abs="${ROOT}/${rel}"
  if [[ -d "$abs" ]]; then
    find "$abs" -mindepth 1 -delete
  else
    mkdir -p "$abs"
  fi
  chmod 775 "$abs"
}

truncate_file() {
  local rel="$1"
  local abs="${ROOT}/${rel}"
  mkdir -p "$(dirname "$abs")"
  : > "$abs"
}

write_literal() {
  local rel="$1"
  local abs="${ROOT}/${rel}"
  mkdir -p "$(dirname "$abs")"
  cat >"$abs"
}

# Directories generated at runtime that must stay empty in release artifacts.
RUNTIME_DIRS=(
  mail
  log
  debug
  vote
)

clean_player_buckets() {
  local base="${ROOT}/player"
  mkdir -p "$base"
  chmod 775 "$base"

  for bucket in "${PLAYER_BUCKETS[@]}"; do
    local bucket_path="${base}/${bucket}"
    mkdir -p "$bucket_path"
    chmod 775 "$bucket_path"
    find "$bucket_path" -mindepth 1 -delete
  done
}

clean_player_buckets

for dir in "${RUNTIME_DIRS[@]}"; do
  clean_dir "$dir"
done

# Files that should be blanked before packaging.
TRUNCATE_FILES=(
  "etc/address"
  "etc/database"
  "etc/net.log"
  "etc/player.new"
  "debug/bugs"
  "debug/error"
  "debug/failenable"
  "debug/failexit"
)

for file in "${TRUNCATE_FILES[@]}"; do
  truncate_file "$file"
done

# Files that require specific sentinels/default values.
write_literal "board/imm/list" <<'EOF'
End
EOF

write_literal "board/loyang/list" <<'EOF'
End
EOF

write_literal "etc/hero" <<'EOF'
End
EOF

write_literal "data/immlist" <<'EOF'
End
EOF

write_literal "etc/stock" <<'EOF'
Stock   1       阿丁棺材店              10000   0       0
Stock   2       人肉包子店              10000   0       0
Stock   3       萬花樓娛樂公司          10000   0       0
Stock   4       豬哥賭博公會            10000   0       0
Stock   5       不好吃免錢餐飲          10000   0       0
EOF

echo "Runtime data under ${ROOT} sanitized."
