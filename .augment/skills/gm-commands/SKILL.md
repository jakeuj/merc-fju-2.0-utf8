---
name: gm-commands
description: Merc-FJU 2.0 MUD 遊戲的 GM（管理者）指令完整參考。當使用者詢問如何管理玩家、修改角色屬性、載入怪物/物品、傳送玩家、查詢遊戲狀態、或任何需要神族（immortal）帳號才能執行的遊戲管理操作時，請使用此 skill。涵蓋 mset、oset、rset、advance、restore、transfer、goto、mload、oload、slay、purge、freeze、ban 等所有管理指令的語法、等級需求與參數說明。
---

# Merc-FJU 2.0 GM 指令手冊

本遊戲是基於 Merc 2.2 引擎的三國題材 MUD，以下是所有管理者可用指令的完整參考。

## 等級體系

等級總上限 `MAX_LEVEL = 120`，神族等級由低到高：

| 代號 | 等級數值 | 說明 |
|------|---------|------|
| `L_HER` | 116 | 英雄（Hero），剛踏入神族的最低門檻 |
| `L_ANG` | 117 | 天使（Angel），一般管理功能 |
| `L_DEI` | 118 | 神明（Dei），核心修改權限 |
| `L_SUP` | 119 | 至尊（Supreme），高危操作 |
| `L_GOD` | 120 | 造物主（God），最高權限 |

`LEVEL_IMMORTAL = 101`：玩家升到 101 級後才進入不死（immortal）範圍。

---

## 快速任務索引

| 我想要… | 使用指令 | 所需等級 |
|---------|---------|---------|
| 修改玩家屬性（力量/體質等） | `mset` | L_DEI (118) |
| 修改玩家等級 | `advance` | L_GOD (120) |
| 查看玩家詳細資料 | `mstat` | L_ANG (117) |
| 回復玩家 HP/MP/移動力 | `restore` | L_DEI (118) |
| 傳送玩家到自己位置 | `transfer` | L_DEI (118) |
| 傳送自己到特定房間 | `goto` | L_ANG (117) |
| 載入怪物 | `mload` | L_DEI (118) |
| 載入物品 | `oload` | L_DEI (118) |
| 殺死玩家/怪物 | `slay` | L_DEI (118) |
| 清除房間所有怪物/物品 | `purge` | L_DEI (118) |
| 封禁帳號 | `ban` | L_SUP (119) |
| 凍結玩家指令 | `freeze` | L_SUP (119) |
| 查看線上玩家詳情 | `users` | L_ANG (117) |

---

## 核心指令詳解

### mset — 修改角色屬性（L_DEI）

```
mset <對象> <項目> <數值>
mset <對象> <字串欄位> <文字>
```

**數值型項目：**

| 項目 | 說明 | 備注 |
|------|------|------|
| `str` | 力量 | 0 ~ MaxStr |
| `int` | 智力 | 0 ~ MaxInt |
| `wis` | 學識 | 0 ~ MaxWis |
| `dex` | 敏捷 | 0 ~ MaxDex |
| `con` | 體質（體格） | 0 ~ MaxCon，內部匹配關鍵字為 `constitutin` |
| `sex` | 性別 | 0=中性 1=男 2=女 |
| `level` | 等級 | 0 ~ MAX_LEVEL(120) |
| `gold` | 金幣 | |
| `hp` | 生命力上限 | |
| `mana` | 法力上限 | |
| `move` | 移動力上限 | |
| `practice` | 練習點數 | |
| `align` | 陣營值 | -1000 ~ 1000 |
| `bank` | 存款 | |
| `firman` | 護符數量 | |
| `thirst` | 口渴程度 | |
| `drunk` | 酒醉程度 | |
| `full` | 飽食程度 | |

**字串型項目：** `name`、`short`、`long`、`description`、`title`

**旗標型項目（`mset <對象> flags <旗標>`）：**
`sentinel`、`scavenger`、`aggressive`、`stayarea`、`wimpy`、`train`、`rebirth`、`fight`、`ask`、`noreborn`、`nokill`、`nosummon`、`enroll`、`good`、`evil`、`speak`

**範例：**
```
mset jake con 25       → 將 jake 的體質設為 25
mset jake hp 5000      → 將 jake 的最大 HP 設為 5000
mset jake title 大俠   → 將 jake 的稱號改為「大俠」
mset jake align 1000   → 將 jake 設為最善良陣營
```

---

### advance — 升降玩家等級（L_GOD）

```
advance <玩家名字> <等級>
```
等級上限為 100（玩家可達等級），不能超過 100。

---

### restore — 回復玩家狀態（L_DEI）

```
restore              → 回復自己
restore <玩家>       → 回復指定玩家的 HP/MP/MV 至最大值
restore !<玩家>      → 僅移除玩家身上所有法術效應
```

---

### transfer — 傳送玩家（L_DEI）

```
transfer <玩家>      → 召喚指定玩家到你所在位置
transfer all         → 召喚全體玩家
```

### goto — 傳送自己（L_ANG）

```
goto <房間號碼>      → 傳送自己到指定 vnum 房間
goto @<玩家名稱>     → 傳送自己到該玩家所在位置
```

---

詳細分類指令請參考：
- `references/player-management.md` — 玩家管理（查看、封禁、懲罰類）
- `references/world-management.md` — 世界管理（載入、清除、物品、房間類）
- `references/server-management.md` — 伺服器管理（重開、廣播、監控類）

