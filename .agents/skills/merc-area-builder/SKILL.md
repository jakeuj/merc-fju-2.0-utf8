---
name: merc-area-builder
description: 建立、擴充或正式替換 Merc-FJU 區域資料的完整工作流程：依 README 規劃區域目錄與 index/mineral/mob/obj/res/roo/shp 結構、撰寫或搬移各類資料檔、更新 area/directory.lst、同步調整 recall/新手區/對戰區等系統設定與玩家提示文字，必要時再調整技能 Chance/Value，並以專案內工具與遊戲 reload 驗證。適用於新增正式區域、移除釋出區，或大幅修改任何 MUD 區域時。
---

# Merc Area Builder

此技能協助你在目前的 Merc-FJU 專案工作區內建構、擴充或正式替換區域資料。步驟採繁體中文說明，英文技術術語保留原文。

## 快速開始（6 步驟）
1. **規劃**：決定區域 slug（`area/<slug>`）、VNUM 區段、`Serial`、`Capital`、故事描述。先確認 `area/` 內沒有同名目錄，也不與既用 VNUM 衝突。
2. **建立骨架**：依 `document/README` 建立完整區域結構。每個區域至少要有 `index`，通常還要建立 `mineral/`、`mob/`、`obj/`、`res/`、`roo/`、`shp/`。即使暫時沒有礦物或商店，仍優先保留目錄結構，避免後續載入或維護不一致。
3. **更新載入清單**：新增正式區時，把 `<slug>` 寫入 `area/directory.lst`。若是正式世界替換，需改成只保留 `limbo` 與新正式區，並移除被淘汰的釋出/測試區。
4. **填寫或搬移資料檔**：依下方「資料檔案指南」撰寫 index/mob/obj/roo/res/shp，或以既有三國區做模板整批搬移到新 VNUM。長篇描述結尾記得 `~`，搬移後要清除舊城名與舊勢力名詞。
5. **同步系統設定**：若區域承接正式主城、新手區或戰鬥區功能，需一併更新 `src/merc.ini`、`etc/merc.ini`、`src/variable.c`、`src/job.c`、`data/bounty.txt`、`data/bus.txt`、`data/ship.txt` 與玩家可見 help/提示字串。若此區域還開放新的技能，再同步檢查 `Chance` / `Value`。
6. **驗證**：執行 `python3 scripts/check-data.py`（整體 UTF-8 與基本標記），視需要用搜尋工具再次檢查出入口、裝備、商店、reset 與玩家提示是否仍引用舊區，最後啟動 `./startup` 或在遊戲中 `reload area <slug>`，以 `goto <vnum>`、`stat mob <vnum>` 巡視並查看 `log/` 是否有 parse 錯誤。

詳細 checklist 與 commit 範例請閱讀 `references/area-build-checklist.md` 與 `references/wow-area-example.md`。

## README 專案慣例
- README 明確說明，為了讓遊戲可正常運作，發行包釋放了數個示範區域；正式營運後，除 `limbo` 外，其餘釋出區域應依 `COPYRIGHT` 規範評估是否移除。規劃新內容時，不要假設示範區域會永久存在。
- 區域開發的起點是參考既有區域檔案。現況已切成正式區群 `academy`、`changan`、`xuchang`、`drillground`；新工作優先比對這些正式區與 `limbo`，不要再依賴已移除的釋出區。
- 每次新增區域時，優先確認目錄命名、檔案編碼、分隔符與既有檔案一致，不要混入其他 MUD 變體的格式。

## 正式替換工作流
- 若任務目標是「正式上線版本」，預設不是單純新增區域，而是檢查哪些舊區要從 `area/` 與 `area/directory.lst` 全面退場。
- 正式替換時，至少同時檢查 4 類耦合：`directory.lst` 載入順序、系統固定房號（如 `Room Recall` / `Room School`）、工作函式房號（如 `job_goto_pk_area`）、以及玩家可見文字（help、懸賞、驛站、空運、提示訊息）。
- 若新區是由舊區複製而來，除了 VNUM 外，也要清除 `Name`、`Description`、NPC keyword、商店名、交通目的地、懸賞訊息中殘留的舊城名稱。
- 當正式區完全承接功能後，應把舊區實體目錄移除，而不是只從 `directory.lst` 停用。

## 資料檔案指南
- **index**：欄位順序與 commit `stormwind/index`、`orgrimmar/index` 相同。`Echo` = `WILL_ECHO`，`Fog` 可填白天/夜間兩筆，`Serial` 建議採 3 位數流水號，`Capital` 指向起始房間。`Description` 可分段敘述地理、交通、怪物等級，每段之間以空行+`~` 結束。
- **mob/*.mob**：一檔一 VNUM，欄位定義請查 `document/mob.txt`。常用旗標：`Sentinel`, `StayArea`, `AutoSetValue`, `Effect`。劇情對話與戰鬥邏輯寫在 `Process` 區塊，可比照 commit 8df189e 的 `fight_prog`、`rand_prog` 流程（先排除 NPC/Immortal，再依 Faction 設敵）。記得設定 `Level`, `Class`, `Alignment` 與基本屬性，未定值可填 `-1` 讓系統自算。
- **obj/*.obj**：欄位依 `document/obj.txt`。遵循 commit 範例：武器設定 `Type`, `WearFlags`, `Value0-5`，道具/食物/藥水以對應表填寫。若需商店販售，請確保 `res` 內以 `G` 或 `O` 將物品與 NPC 綁定。
- **roo/*.roo**：每個房間獨立檔案，欄位詳見 `document/room.txt`。`SectorType` 使用常數（如 `SECT_CITY`、`SECT_INSIDE`），房間描述可多行。每個 `#Exit` 塊需包含 `Direction`, `ExitVnum`, `ExitKeyword`, `ExitDesc`, `ExitKey`，並對應相鄰房間。
- **res/*.res**：可集中為一檔，例如 `stormwind.res`。語法詳見 `document/reset.txt`。建議以註解區分「守衛配置」、「商店」、「王宮」等主題：使用 `M` 刷怪、`E` 裝備、`G` 給物品、`D` 控制門、`O` 放置場景物件。
- **shp/*.shp**：每個商店一檔，`Type` 通常 `SHOP_STORE`，`Keeper` 為 NPC VNUM，商品種類以 `Object` 列舉 item type，`Sellprofit` / `Buyprofit` 控制價格。參照 `document/shop.txt` 與 commit 中 10007/10105 範例。
- **mineral/**：如需礦脈/採集物，沿用 README 所述格式：每種資源一檔並放入 `mineral/`，再在 `res` 內加上對應的刷新。即使暫時沒有礦物，也建議保留目錄以符合資料夾結構。

## 規劃建議
- **VNUM 與 Serial**：保持連號方便查詢。正式世界目前可視為 `100xx = changan`、`101xx = xuchang`、`102xx = academy`、`103xx = drillground`；新正式區建議另開新的百位段，避免再占用已承接的正式區。
- **命名**：檔名與 VNUM 相同（`10001.mob`、`10001.roo`）；若採單一 `res` 檔，使用 `<slug>.res`。
- **語言風格**：描述以台灣繁體中文書寫，必要英文字以括號標註。正式版優先使用三國語境與城名，不要留下 `Stormwind`、`Orgrimmar`、`Alliance`、`Horde` 等已淘汰設定。
- **Process 腳本**：對話可使用 `say`、`emote`；複雜行為可調用 `mpsetenemy`, `rand(n)` 等內建 MUDProg 指令。保持條件簡潔避免無線迴圈。
- **技能聯動**：若區域設計涉及可習得、可掉落或可由 NPC 使用的新技能，除了區域資料外，還要主動檢查技能資料檔中的 `Chance` / `Value` 是否與區域等級帶和傷害預期相符。
- **交通與服務聯動**：只要新區承接主城、新手區或戰鬥區，就預設要檢查 `bus.txt`、`ship.txt`、懸賞房號、`RoomRecall/RoomSchool` 與 `job_goto_pk_area` 這類固定入口。

## 驗證與除錯
1. `python3 scripts/check-data.py`：確保 UTF-8 與必要標記；若僅檢查 `area/slug`，可臨時修改腳本 TARGET 列表或先備份後還原。
2. `rg` / `git diff`：快速確認 VNUM 是否互相對應，例如 `rg 10010 area/<slug>/res` 查找門鎖設定；正式替換時，再用搜尋工具掃 `help/`, `data/`, `src/` 裡是否仍殘留舊城名與舊房號。
3. 系統固定房號檢查：確認 `Room Recall`、`Room School`、懸賞 `Room`、驛站 `Bus`、空運 `Starting/Destination`、以及 `job_goto_pk_area` 這類房號都已指向新正式區。
4. 遊戲內測試：`reload area <slug>` 後觀察 `log/<pid>.log`，若出現 `db_read_area`、`load_resets` 錯誤，依行數回頭修正；若有主城/新手區替換，另外測 `recall`、`new`、對戰傳送與交通工具。
5. 匯出資料：若需交付範例，可附上新 `area/<slug>` 目錄、`area/directory.lst`、以及相關 `src/` / `data/` / `help/` diff，一併附上此技能說明。

## 參考資料
- `references/area-build-checklist.md`：逐項核對模板，含建議指令與常見陷阱。
- `references/wow-area-example.md`：commit 8df189ef9153e463e435d817893967e033f3a976 的 Stormwind/Orgrimmar 實例。
- 原始文件：`document/mob.txt`, `document/obj.txt`, `document/room.txt`, `document/reset.txt`, `document/shop.txt`。
