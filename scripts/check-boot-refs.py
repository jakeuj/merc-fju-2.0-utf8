#!/usr/bin/env python3
"""
Validate boot-time cross references that are not covered by the UTF-8/content linter.

Checks these files:
  - data/gift.txt
  - data/sale.txt
  - data/ship.txt
  - data/bus.txt
  - board/*/index
"""

from __future__ import annotations

import pathlib
import re
import sys
from collections import Counter

ROOT = pathlib.Path(__file__).resolve().parents[1]
AREA_DIR = ROOT / "area"

VNUM_PATTERN = re.compile(r"^Vnum\s+(\d+)\s*$")
EXIT_VNUM_PATTERN = re.compile(r"^ExitVnum\s+(\d+)\s*$")


def load_area_vnums() -> tuple[set[int], set[int], set[int], list[tuple[pathlib.Path, int, int]], list[str]]:
    rooms: set[int] = set()
    mobs: set[int] = set()
    objs: set[int] = set()
    room_exits: list[tuple[pathlib.Path, int, int]] = []
    errors: list[str] = []

    for path in AREA_DIR.rglob("*"):
        if not path.is_file():
            continue

        target: set[int] | None = None
        suffix = path.suffix.lower()
        if suffix == ".roo":
            target = rooms
        elif suffix == ".mob":
            target = mobs
        elif suffix == ".obj":
            target = objs
        else:
            continue

        try:
            text = path.read_text(encoding="utf-8")
        except UnicodeDecodeError as exc:
            raise RuntimeError(f"{path}: UTF-8 decode error ({exc})") from exc

        match = VNUM_PATTERN.search(text.splitlines()[0] if text else "")
        if not match:
            raise RuntimeError(f"{path}: missing leading Vnum line")
        vnum = int(match.group(1))
        rel = path.relative_to(ROOT)
        if vnum in target:
            errors.append(f"{rel}: duplicate Vnum {vnum}")
        target.add(vnum)

        if suffix == ".roo":
            for lineno, line in enumerate(text.splitlines(), start=1):
                exit_match = EXIT_VNUM_PATTERN.match(line.strip())
                if exit_match:
                    room_exits.append((rel, lineno, int(exit_match.group(1))))

    return rooms, mobs, objs, room_exits, errors


def read_lines(path: pathlib.Path) -> list[str]:
    return path.read_text(encoding="utf-8").splitlines()


def parse_key_values(lines: list[str]) -> list[tuple[int, str, str]]:
    entries: list[tuple[int, str, str]] = []
    for lineno, line in enumerate(lines, start=1):
        stripped = line.strip()
        if not stripped or stripped.startswith("*"):
            continue
        parts = stripped.split(None, 1)
        if len(parts) == 2:
            entries.append((lineno, parts[0], parts[1].strip()))
    return entries


def check_gift(lines: list[str], mobs: set[int], objs: set[int], errors: list[str]) -> None:
    for lineno, key, value in parse_key_values(lines):
        if key == "Sender":
            vnum = int(value)
            if vnum not in mobs:
                errors.append(f"data/gift.txt:{lineno}: Sender mob {vnum} does not exist")
        elif key == "Gift":
            vnum = int(value)
            if vnum not in objs:
                errors.append(f"data/gift.txt:{lineno}: Gift object {vnum} does not exist")


def check_sale(lines: list[str], objs: set[int], errors: list[str]) -> None:
    for lineno, key, value in parse_key_values(lines):
        if key != "Object":
            continue
        vnum = int(value)
        if vnum not in objs:
            errors.append(f"data/sale.txt:{lineno}: sale object {vnum} does not exist")


def check_ship(lines: list[str], rooms: set[int], errors: list[str]) -> None:
    seen_cabins: Counter[int] = Counter()
    for lineno, key, value in parse_key_values(lines):
        if key in {"Starting", "Destination"}:
            vnum = int(value)
            if vnum not in rooms:
                errors.append(f"data/ship.txt:{lineno}: {key} room {vnum} does not exist")
        elif key == "Cabin":
            vnum = int(value)
            seen_cabins[vnum] += 1
            if vnum in rooms:
                errors.append(f"data/ship.txt:{lineno}: Cabin room {vnum} already exists in area data")
    for vnum, count in seen_cabins.items():
        if count > 1:
            errors.append(f"data/ship.txt: Cabin room {vnum} is duplicated {count} times")


def check_bus(lines: list[str], rooms: set[int], errors: list[str]) -> None:
    platform_counts: Counter[int] = Counter()
    loge_counts: Counter[int] = Counter()

    for lineno, line in enumerate(lines, start=1):
        stripped = line.strip()
        if not stripped.startswith("Bus"):
            continue

        match = re.match(r"^Bus\s+'[^']+'\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s*$", stripped)
        if not match:
            errors.append(f"data/bus.txt:{lineno}: malformed Bus entry")
            continue

        station, platform, loge, _cost = map(int, match.groups())
        if station not in rooms:
            errors.append(f"data/bus.txt:{lineno}: station room {station} does not exist")
        if platform in rooms:
            errors.append(f"data/bus.txt:{lineno}: platform room {platform} already exists in area data")
        if loge in rooms:
            errors.append(f"data/bus.txt:{lineno}: loge room {loge} already exists in area data")
        platform_counts[platform] += 1
        loge_counts[loge] += 1

    for vnum, count in platform_counts.items():
        if count > 1:
            errors.append(f"data/bus.txt: platform room {vnum} is duplicated {count} times")
    for vnum, count in loge_counts.items():
        if count > 1:
            errors.append(f"data/bus.txt: loge room {vnum} is duplicated {count} times")


def check_board(rooms: set[int], errors: list[str]) -> None:
    slot_counts: Counter[int] = Counter()
    for path in (ROOT / "board").glob("*/index"):
        for lineno, key, value in parse_key_values(read_lines(path)):
            if key == "Location":
                vnum = int(value)
                if vnum not in rooms:
                    errors.append(f"{path.relative_to(ROOT)}:{lineno}: board location room {vnum} does not exist")
            elif key == "Slot":
                slot_counts[int(value)] += 1

    for slot, count in slot_counts.items():
        if count > 1:
            errors.append(f"board/*/index: board slot {slot} is duplicated {count} times")


def check_room_exits(
    rooms: set[int],
    room_exits: list[tuple[pathlib.Path, int, int]],
    errors: list[str],
) -> None:
    for path, lineno, exit_vnum in room_exits:
        if exit_vnum not in rooms:
            errors.append(f"{path}:{lineno}: ExitVnum {exit_vnum} does not exist")


def main() -> int:
    try:
        rooms, mobs, objs, room_exits, area_errors = load_area_vnums()
    except RuntimeError as exc:
        print(f"Boot reference check failed: {exc}", file=sys.stderr)
        return 1

    errors: list[str] = list(area_errors)
    check_room_exits(rooms, room_exits, errors)
    check_gift(read_lines(ROOT / "data" / "gift.txt"), mobs, objs, errors)
    check_sale(read_lines(ROOT / "data" / "sale.txt"), objs, errors)
    check_ship(read_lines(ROOT / "data" / "ship.txt"), rooms, errors)
    check_bus(read_lines(ROOT / "data" / "bus.txt"), rooms, errors)
    check_board(rooms, errors)

    if errors:
        print("Boot reference check found issues:", file=sys.stderr)
        for error in errors:
            print(f"  - {error}", file=sys.stderr)
        return 1

    print("Boot reference check passed for gift/sale/ship/bus/board data.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
