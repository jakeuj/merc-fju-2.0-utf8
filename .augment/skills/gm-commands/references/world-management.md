# 世界管理指令

## 怪物（MOB）管理

### mload — 載入怪物（L_DEI）
```
mload <vnum>
```
在當前房間生成一隻指定 vnum 的怪物。
vnum 可透過 `mfind` 查詢。

### miset — 設定怪物索引資料（L_SUP）
```
miset <vnum> <項目> <數值>
```
修改怪物的**原始定義**（影響此後所有新生成的同類怪物）。

### purge — 清除房間（L_DEI）
```
purge                → 清除房間內所有怪物與物品（不留屍體）
purge <怪物名稱>     → 僅清除指定怪物（不留屍體）
```
與 `slay` 的差別：purge 不留屍體，slay 會留屍體。

### mpstat — 查看怪物的 MOB Program（L_DEI）
```
mpstat <怪物名稱>
```
顯示怪物身上所有 MOBprog 的觸發條件與程式內容。

---

## 物品（OBJ）管理

### oload — 載入物品（L_DEI）
```
oload <vnum>
```
在當前房間地上生成一個指定 vnum 的物品。

### oset — 修改物品屬性（L_DEI）
```
oset <物品名稱> <項目> <數值>
oset <物品名稱> <字串欄位> <文字>
```

**數值型項目：**

| 項目 | 說明 |
|------|------|
| `value0` ~ `value3` | 物品類型相關數值（依物品種類不同意義不同） |
| `weight` | 重量 |
| `cost` | 價格 |
| `level` | 物品等級限制 |
| `timer` | 計時消失倒數 |
| `flags` | 物品旗標（見下方） |
| `wear` | 穿戴位置 |
| `armor` | 護甲值 |
| `hitroll` | 命中加成 |
| `damroll` | 傷害加成 |

**字串型項目：** `name`、`short`、`long`、`description`

**旗標（`oset <物品> flags <旗標>`）：**
`glow`、`hum`、`dark`、`evil`、`invis`、`magic`、`nodrop`、`bless`、`antigood`、`antievil`、`antineutral`、`noremove`、`nosave`、`contraband`

**範例：**
```
oload 1234                   → 在地上放一個 vnum=1234 的物品
oset sword level 50          → 將手上的 sword 等級設為 50
oset sword flags nodrop      → 切換 sword 的不可丟棄旗標
```

### ostat — 查看物品詳細屬性（L_ANG）
```
ostat <物品名稱>
```

### ofind — 依名稱搜尋物品索引（L_ANG）
```
ofind <關鍵字>
```

### owhere — 找出物品目前位置（L_DEI）
```
owhere <物品名稱>
```
列出遊戲世界中所有同名物品的所在位置。

### olevel — 列出指定等級範圍的物品（L_ANG）
```
olevel <最低等級> <最高等級>
```

### olist — 列出物品索引（L_ANG）
```
olist
olist <關鍵字>
```

### otype — 按類型列出物品（L_ANG）
```
otype <類型>
```
類型可選：`weapon`、`armor`、`potion`、`scroll`、`wand`、`staff`、`food`、`container`、`drink_con`、`money`、`key`、`light`、`treasure`、`boat`、`corpse_npc`、`fountain`、`pill`、`magicstone` 等。

### oaf — 依 apply 欄位搜尋物品（L_ANG）
```
oaf <apply 欄位>
```
可選：`str`、`int`、`wis`、`dex`、`con`、`sex`、`class`、`level`、`age`、`height`、`weight`、`mana`、`hp`、`move`、`gold`、`exp`、`ac`、`hitroll`、`damroll`、`saving_para` 等。
範例：`oaf con` → 列出所有增加體質的物品。

---

## 房間（ROOM）管理

### rstat — 查看房間資料（L_ANG）
```
rstat
rstat <房間 vnum>
```
顯示當前（或指定）房間的 vnum、旗標、區域、出口等完整資訊。

### rset — 修改房間屬性（L_DEI）
```
rset <房間 vnum> <項目> <數值>
```
可修改房間的各種旗標與屬性。

### sector — 修改房間地形（L_ANG）
```
sector <地形類型>
```

---

## 技能（SKILL）管理

### lfind — 搜尋技能（L_DEI）
```
lfind <關鍵字>
```

### llookup — 查看技能詳細資料（L_ANG）
```
llookup <技能名稱>
```

### lset — 設定玩家技能熟練度（L_DEI）
```
lset <玩家> <技能名稱> <熟練度>
```
熟練度 0 = 移除技能，1~100 = 設定熟練百分比。

### sset — 修改技能索引定義（L_GOD）
```
sset <技能名稱> <項目> <數值>
```
修改技能的基礎定義（全域生效，謹慎使用）。

