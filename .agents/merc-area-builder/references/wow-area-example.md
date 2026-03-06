# 範例：Stormwind / Orgrimmar（commit 8df189ef9153e463e435d817893967e033f3a976）

這份摘要整理了該 commit 新增的兩個大型城鎮區域，可作為建立新區域時的參考模板。

## 共通設計要點
- slug 均寫入 `area/directory.lst` 底部：`stormwind`, `orgrimmar`。
- 每個區域目錄都包含 `index`, `mob/`, `obj/`, `roo/`, `res/`, `shp/` 六個核心資料夾。
- `index` 連續兩段 `Fog`（白天/夜晚），`Upper` = `LEVEL_PC`、`Lower` = `1`，並以 `Serial` 100 / 101 區分勢力。
- `Capital` 指向主要城市入口房間（Stormwind 10001、Orgrimmar 10101），方便 `goto` 及 `recall`。
- `mob/*.mob` 擁有 `Process` 區塊處理 `fight_prog`、`rand_prog`，先排除 `isnpc`、`isimmort` 再以 `mpsetenemy` 標記陣營。
- `res/*.res` 以註解分段描述守衛、商人、任務 NPC，並以 `G`（give）或 `E`（equip）指示預設裝備。

## Stormwind
- VNUM 10001–10040 房間（`roo/10001.roo` … `10040.roo`），`ls area/stormwind/roo | wc -l` 顯示 40 個房間。
- 20 個 `mob` 檔（10001–10020）涵蓋守衛、國王、飛行大師與市民；NPC 對話使用 `say`、`emote`。
- 18 個 `obj`（10001–10018）對應武器、防具、補給品，並在 `res/stormwind.res` 透過 `G` 綁定商店存貨。
- `shp/` 包含 4 間商店（武器、防具、藥水、書店）：`10007.shp`, `10008.shp`, `10009.shp`, `10018.shp`。
- `res/stormwind.res` 擷取語法如下（依序運用 `M`, `E`, `G`, `D`, `O`）：
  - 先刷新守衛並裝備指定 `obj`。
  - 王宮段落用 `E` 給 key item (`王宮令牌`)。
  - `O` 指令放置互動物件（如噴水池補水）。

## Orgrimmar
- VNUM 10101–10140 房間（40 檔 `roo`）。
- 21 個 `mob`（10101–10121）涵蓋守衛、薩滿、巨魔、術士與飛行大師；高危守衛設定 `Alignment -200`、`Effect 'sanctuary'`。
- 18 個 `obj`（10101–10118），多為部落風格武器、防具、藥水。
- `shp/` 內 4 間商店：`10105.shp`, `10106.shp`, `10107.shp`, `10120.shp`，販售武器、防具、藥品、圖騰。
- `res/orgrimmar.res` 做法與 Stormwind 相同：大量 `M` 條目控制同一房間 NPC 上限、`G` 交付商品、`D` 控制城門。

## 如何使用本參考
1. 依新區域調整 `Serial` 與 VNUM 區段，但沿用此 commit 的欄位順序與檔案拆分方式。
2. 需要城市級規模時，可直接複製 Stormwind/Orgrimmar 的資料夾做為骨架，再以 `rg` 取代字串。
3. `Process` 區塊可照範例判斷陣營 (`isenemy`, `mpsetenemy`)，避免敵對陣營誤觸。
4. `res` 註解示範如何照功能群組分段（守衛/商人/飛行大師），新增區域時優先維持這個章節式寫法以利維護。
