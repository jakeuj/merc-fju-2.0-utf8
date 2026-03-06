# Merc 區域建立檢查清單

## 0. 前置規劃
- 決定 *area slug*（`area/<slug>`），建議使用英文字母小寫與底線，例如 `stormwind`、`orgrimmar`。
- 規劃 VNUM 區段（房間、怪物、物品、重置、商店共用同一段）。Stormwind 以 100xx、Orgrimmar 以 101xx 為例，確保不與 `area/index` 既有範圍衝突。
- 指定 Serial（區域流水號）與 Capital（起始房間 VNUM），可沿用 100、101 這類百位數讓資料表排序。
- 蒐集區域簡述、交通、怪物等級等敘述，用於 `index` 的 Description。

## 1. 建立目錄與登錄
| 項目 | 動作 |
| --- | --- |
| `area/<slug>/` | 建立 `index`, `mob/`, `obj/`, `roo/`, `res/`, `shp/` 資料夾；如需要 `mineral/`、`reset/` 也在此建立。 |
| `area/directory.lst` | 追加 `<slug>` 新行，確保伺服器開機會載入此區。保持字母排序並與既有內容相同縮排。 |

## 2. 檔案填寫步驟
1. `index`：填入 Echo、Editor、Name、Fog、Upper/Lower、Serial、Capital 以及多段 Description（以 `~` 結尾）。
2. `mob/*.mob`：一個 VNUM 一檔。欄位順序與 `document/mob.txt` 一致，必要旗標（`Sentinel`、`StayArea` 等）逐行寫出。
3. `obj/*.obj`：道具資料遵守 `document/obj.txt`。使用 `Values` 陣列定義屬性，自行決定 `WearFlags`、`ExtraFlags`。
4. `roo/*.roo`：每個房間獨立檔案，欄位參照 `document/room.txt`（Name、Description、SectorType、出入口 `#Exit` 區塊等）。
5. `res/*.res`：可集中於單一檔案，如 `stormwind.res`。指令語法詳見 `document/reset.txt`。
6. `shp/*.shp`：需要商店時設定 `Type`, `Keeper`, `Object`, `OpenHour/CloseHour`, `Sellprofit/Buyprofit`，語法參考 `document/shop.txt`。

## 3. 寫作指引
- 全部檔案維持 UTF-8，避免含 BOM。若複製舊資料，先跑 `python3 scripts/check-data.py` 確認編碼。
- 中文敘述以全形標點撰寫，英文明確保大小寫一致。對話與 emote 以 `Process` 區塊撰寫。
- `mob` 與 `obj` 的 `AutoSetValue`, `Effect`, `Process` 等欄位依需求加減。
- `roo` 出入口需互相對應，並確保 `ExitVnum` 指向存在的房間；若有門鎖機制，記得在 `res` 內加入對應 `D` 指令。
- 商店 NPC 需在 `res` 內以 `M` 指令刷新，並以 `G` 將販售品綁定到 keeper。

## 4. 驗證與測試
- 執行 `python3 scripts/check-data.py`：快速檢查 UTF-8 與必要標記。
- 以 `git status` 確認只有目標區域的檔案變更；必要時 `git diff` 檢查 VNUM 與旗標。
- 進入遊戲後可用 `reload area <slug>` 或重新啟動 `./startup` 載入，並使用 `goto <vnum>`、`stat obj <vnum>` 等指令逐一巡查。
- 若資料量大，建議在 `res` 檔案頂端加入註解分段，方便維護者巡覽。

## 5. 延伸參考
- `document/mob.txt`, `document/obj.txt`, `document/room.txt`, `document/reset.txt`, `document/shop.txt`
- `references/wow-area-example.md`：Stormwind 與 Orgrimmar 範例清單，快速檢視 commit 8df189ef 的寫法。
