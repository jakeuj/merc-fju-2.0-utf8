---
name: mud-command-handbook
description: Provide accurate game command lookup and response templates for MUD/MMORPG administration and player operations. Use when the user asks how to execute in-game commands, GM/admin commands, attribute/stat changes, item/currency edits, teleport/spawn actions, permission checks, rollback/safety procedures, or syntax examples such as setting CON to 100.
---

# Game Command Handbook

## Overview

Provide concise, executable command guidance for game players and administrators.
Return command syntax, required permissions, and a safe execution order.

## Workflow

1. Identify context.
- Confirm game/server type, command engine, and role (`player`, `GM`, `admin`, `console`, `DBA`).
- If unknown, provide a "common variants" answer with clearly labeled alternatives.

2. Identify target and scope.
- Confirm who/what to modify: self, other player, NPC, online/offline character, temporary/permanent.
- Confirm scope: single attribute, full reset, batch update.

3. Provide executable command block.
- Provide the shortest valid command first.
- Add 1-3 common variants only when syntax differs across engines.
- Include expected success output and quick verification command.

4. Add safety guardrails.
- Remind about backup/audit for destructive actions.
- Prefer temporary test change on staging/admin test character when possible.
- Never suggest bypassing auth or exploiting security.

## Response Format

Use this structure:

1. `Assumption`: engine/permission assumptions.
2. `Command`: primary command.
3. `Variants`: optional alternatives for other command systems.
4. `Verify`: command to confirm change.
5. `Risk Note`: backup/logging reminder if needed.

## Example Query Handling

Example user request:
- "我是遊戲管理者，如何把自己的 con 屬性提升到 100？"

Default answer pattern:

- `Assumption`: You are a GM with character-edit permission.
- `Command`:
  - `set <your_name> con 100`
- `Variants`:
  - `setattr <your_name> con 100`
  - `mset <your_name> con 100`
  - `@set <your_name>/con=100`
- `Verify`:
  - `stat <your_name>`
  - `score`
- `Risk Note`:
  - If your server persists to DB, save/reload once and re-check to ensure it is not overwritten by template sync.

## References

Load [references/command-patterns.md](references/command-patterns.md) when the user asks for:
- command syntax variants by engine,
- GM/admin operations (stats, item, money, teleport, punish, restore),
- safe rollback/checklist steps.
