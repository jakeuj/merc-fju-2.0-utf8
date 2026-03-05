# Command Patterns

## Attribute and Stat Changes

Use one of these based on server implementation:
- `set <target> <stat> <value>`
- `setattr <target> <stat> <value>`
- `mset <target> <stat> <value>`
- `@set <target>/<stat>=<value>`

Common stat keys:
- `str`, `dex`, `con`, `int`, `wis`, `cha`

Example:
- `set jake con 100`

Verification:
- `stat jake`
- `score jake`

## GM/Admin Common Operations

- Spawn item:
  - `oload <vnum>`
  - `spawn item <id> [count]`
- Give item/currency:
  - `give <target> <item>`
  - `set <target> gold <amount>`
- Teleport:
  - `goto <room_id>`
  - `transfer <target> <room_id>`
- Character moderation:
  - `silence <target> <duration>`
  - `freeze <target> <duration>`
  - `ban <target>`

## Safety Checklist

Before high-impact changes:
1. Confirm permission level and target identity.
2. Snapshot/backup data if persistent storage is involved.
3. Execute on test target first when possible.
4. Verify with readback command.
5. Log action and actor for audit.

Rollback patterns:
- Reapply old value (preferred): `set <target> <stat> <old_value>`
- Restore from backup snapshot.
