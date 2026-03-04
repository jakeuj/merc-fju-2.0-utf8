#!/usr/bin/env python3
"""
Lightweight content linter for 三國歪傳之降龍伏虎 data files.
Ensures files decode as UTF-8 and contain minimal structural markers.
"""

from __future__ import annotations

import pathlib
import sys
from typing import Iterable

ROOT = pathlib.Path(__file__).resolve().parents[1]

TARGETS = ("area", "skill", "angel")
# Files listed here are known to contain legacy encodings that need manual work.
KNOWN_LEGACY = {
    "area/limbo/obj/124.obj",
    "area/new/roo/458.roo",
    "skill/t/tiger_axe.ski",
    "skill/s/sky_blade.ski",
    "skill/h/hashin.ski",
    "skill/d/dragonfist.ski",
    "skill/d/dream_blade.ski",
    "skill/e/eten.ski",
}


def iter_files(folder: pathlib.Path) -> Iterable[pathlib.Path]:
    for path in folder.rglob("*"):
        if path.is_file():
            yield path


def markers_for(path: pathlib.Path) -> tuple[str, ...]:
    if path.suffix.lower() == ".ski":
        return ("Name", "End")
    if path.suffix.lower() == ".ang":
        return ("End",)
    return ()


def lint_file(path: pathlib.Path, errors: list[str]) -> None:
    rel = str(path.relative_to(ROOT))
    if rel in KNOWN_LEGACY:
        return

    try:
        text = path.read_text(encoding="utf-8")
    except UnicodeDecodeError as exc:
        errors.append(f"{path}: UTF-8 decode error ({exc})")
        return

    if not text.strip():
        errors.append(f"{path}: file is empty")
        return

    markers = markers_for(path)
    missing = [marker for marker in markers if marker not in text]
    if missing:
        errors.append(f"{path}: missing markers {', '.join(missing)}")


def main() -> int:
    errors: list[str] = []
    for dirname in TARGETS:
        folder = ROOT / dirname
        if not folder.exists():
            errors.append(f"{folder}: directory missing")
            continue
        for path in iter_files(folder):
            lint_file(path, errors)

    if errors:
        print("Data check found issues:", file=sys.stderr)
        for msg in errors:
            print(f"  - {msg}", file=sys.stderr)
        return 1

    print("All checked data files decoded as UTF-8 and contained required markers.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
