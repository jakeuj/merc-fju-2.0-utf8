---
name: merc-area-builder
description: 建立 Merc-FJU (三國歪傳之降龍伏虎) 新區域的完整工作流程：規劃 VNUM/Serial、建立 area slug 目錄、撰寫 index/mob/obj/roo/res/shp 檔案、更新 area/directory.lst、並以 scripts/check-data.py 與遊戲 reload 驗證。適用於新增或大幅擴充任何 MUD 區域時。
---

# Merc Area Builder

此技能協助你在 `/Users/jakeuj/auggie/mud2` 內建構新的 Merc 區域資料。步驟採繁體中文說明，英文技術術語保留原文。

## 快速開始（6 步驟）
1. **規劃**：決定區域 slug（`area/<slug>`）、VNUM 區段、`Serial`、`Capital`、故事描述。先確認 `area/` 內沒有同名目錄，也不與既用 VNUM 衝突。
2. **建立骨架**：在 `area/` 底下建立 `<slug>/index`、`mineral/`、`mob/`、`obj/`、`roo/`、`res/`、`shp/`（沒有礦物掉落也建空目錄，方便後續擴充）。區域數量多時可先複製 `stormwind` 或 `orgrimmar` 結構再批次取代。
3. **更新載入清單**：將 `<slug>` 追加到 `area/directory.lst`（依字母排序或就近放在同系列後方），確保伺服器啟動時會讀到新區域。
4. **填寫資料檔**：依下方「資料檔案指南」撰寫 index/mob/obj/roo/res/shp，並保持 UTF-8。長篇描述結尾記得 `~`。
5. **驗證**：執行 `python3 scripts/check-data.py`（整體 UTF-8 與基本標記），視需要 `rg vnum` 或 `git diff` 再次檢查出入口、裝備、商店設定。
6. **載入測試**：啟動 `./startup` 或在遊戲中 `reload area <slug>`，以 `goto <vnum>`、`stat mob <vnum>` 巡視；觀察 `log/` 是否有 parse 錯誤。

詳細 checklist 與 commit 範例請閱讀 `references/area-build-checklist.md` 與 `references/wow-area-example.md`。

## 資料檔案指南
- **index**：欄位順序與 commit `stormwind/index`、`orgrimmar/index` 相同。`Echo` = `WILL_ECHO`，`Fog` 可填白天/夜間兩筆，`Serial` 建議採 3 位數流水號，`Capital` 指向起始房間。`Description` 可分段敘述地理、交通、怪物等級，每段之間以空行+`~` 結束。
- **mob/*.mob**：一檔一 VNUM，欄位定義請查 `document/mob.txt`。常用旗標：`Sentinel`, `StayArea`, `AutoSetValue`, `Effect`。劇情對話與戰鬥邏輯寫在 `Process` 區塊，可比照 commit 8df189e 的 `fight_prog`、`rand_prog` 流程（先排除 NPC/Immortal，再依 Faction 設敵）。記得設定 `Level`, `Class`, `Alignment` 與基本屬性，未定值可填 `-1` 讓系統自算。
- **obj/*.obj**：欄位依 `document/obj.txt`。遵循 commit 範例：武器設定 `Type`, `WearFlags`, `Value0-5`，道具/食物/藥水以對應表填寫。若需商店販售，請確保 `res` 內以 `G` 或 `O` 將物品與 NPC 綁定。
- **roo/*.roo**：每個房間獨立檔案，欄位詳見 `document/room.txt`。`SectorType` 使用常數（如 `SECT_CITY`、`SECT_INSIDE`），房間描述可多行。每個 `#Exit` 塊需包含 `Direction`, `ExitVnum`, `ExitKeyword`, `ExitDesc`, `ExitKey`，並對應相鄰房間。
- **res/*.res**：可集中為一檔，例如 `stormwind.res`。語法詳見 `document/reset.txt`。建議以註解區分「守衛配置」、「商店」、「王宮」等主題：使用 `M` 刷怪、`E` 裝備、`G` 給物品、`D` 控制門、`O` 放置場景物件。
- **shp/*.shp**：每個商店一檔，`Type` 通常 `SHOP_STORE`，`Keeper` 為 NPC VNUM，商品種類以 `Object` 列舉 item type，`Sellprofit` / `Buyprofit` 控制價格。參照 `document/shop.txt` 與 commit 中 10007/10105 範例。
- **mineral/**：如需礦脈/採集物，沿用 README 所述格式：每種資源一檔並放入 `mineral/`，再在 `res` 內加上對應的刷新。即使暫時沒有礦物，也建議保留目錄以符合資料夾結構。

## 規劃建議
- **VNUM 與 Serial**：保持連號方便查詢。可將陣營或地區對應不同百位（例：100xx = 聯盟、101xx = 部落），並在 `notes` 裡記錄已用範圍。
- **命名**：檔名與 VNUM 相同（`10001.mob`、`10001.roo`）；若採單一 `res` 檔，使用 `<slug>.res`。
- **語言風格**：描述以台灣繁體中文書寫，必要英文字以括號標註，如 `一名奧格瑞瑪守衛(orgrimmar guard)`。
- **Process 腳本**：對話可使用 `say`、`emote`；複雜行為可調用 `mpsetenemy`, `rand(n)` 等內建 MUDProg 指令。保持條件簡潔避免無線迴圈。

## 驗證與除錯
1. `python3 scripts/check-data.py`：確保 UTF-8 與必要標記；若僅檢查 `area/slug`，可臨時修改腳本 TARGET 列表或先備份後還原。
2. `rg` / `git diff`：快速確認 VNUM 是否互相對應，例如 `rg 10010 area/<slug>/res` 查找門鎖設定。
3. 遊戲內測試：`reload area <slug>` 後觀察 `log/<pid>.log`，若出現 `db_read_area`、`load_resets` 錯誤，依行數回頭修正。
4. 匯出資料：若需交付範例，可壓縮整個 `area/<slug>` 目錄與 `area/directory.lst` diff，一併附上此技能說明。

## 參考資料
- `references/area-build-checklist.md`：逐項核對模板，含建議指令與常見陷阱。
- `references/wow-area-example.md`：commit 8df189ef9153e463e435d817893967e033f3a976 的 Stormwind/Orgrimmar 實例。
- 原始文件：`document/mob.txt`, `document/obj.txt`, `document/room.txt`, `document/reset.txt`, `document/shop.txt`。
