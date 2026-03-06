# 資料檔案結構說明

本文整合 `document/README` 的舊版說明與目前 UTF-8 版專案狀態，協助快速理解各資料夾的用途、是否會在 runtime 產生新檔，以及相依檔案。需要 reset 的具體指派請搭配 [`docs/RUNTIME_RESET.md`](RUNTIME_RESET.md)。

## 頂層目錄地圖

| 目錄 | 類型 | Runtime？ | 說明 |
|------|------|-----------|------|
| `src/` | 原始碼 | ✗ | C 程式碼、Makefile、`startup`、產生後的 `merc.ini`（active config）。Docker build 從這裡產生 `merc`。 |
| `area/` | 世界資料 | △ | 區域定義（`index`、`mob/`、`obj/`、`roo/`、`res/`、`mineral/`、`shp/`），大多數情況視同靜態資產。 |
| `angel/` | 世界資料 | ✗ | 守護神設定（NPC guardian）。 |
| `command/` | 世界資料 | ✗ | 指令描述（`.ins`）與 `command.lst`。詳見下節。 |
| `skill/` | 世界資料 | ✗ | 技能資料檔案。 |
| `social/` | 世界資料 | ✗ | 社交指令描述。 |
| `data/` | 世界資料 | △ | 全域表格（股票、白名單、節慶、immlist 等）；部分檔案在 runtime 會更新，需視情況 reset。 |
| `document/` | 文件 | ✗ | UTF-8 版參考資料（本 README、部署文件等）。 |
| `doc/` | 文件 | ✗ | 原始 Merc/Diku 英文手冊。 |
| `include/` | 資料表 | ✗ | 角色職業、地形、區域旗標等參考資料（`.h`/`.txt`）。 |
| `greeting/` | 文本 | ✗ | 登入畫面 (ASCII art、歡迎詞)。 |
| `help/` | 文本 | ✗ | 線上求助檔案 (`*.hlp`)。 |
| `joke/` | 文本 | ✗ | 笑話集，供 `joke` 指令使用。 |
| `board/` | 半動態資料 | △ | 留言版設定 (`*.lst`) 與內容 (`*.data`/`list`)；`board/*/list` 需保留 `End` sentinel。 |
| `etc/` | 半動態資料 | △ | 雜項設定（登入白名單、股票、MOTD、hero 榜…），下節列出細項。 |
| `player/` | Runtime | ✓ | 玩家檔案。採 `player/<小寫首字>/<角色>/data` 結構；bucket 可由 runtime 自動補建。詳見〈player〉。 |
| `mail/` | Runtime | ✓ | 玩家信件。 |
| `log/` | Runtime | ✓ | 系統日誌 (`startup`、遊戲 log、rollover)。 |
| `debug/` | Runtime | ✓ | 錯誤回報、anti-dupe、疑似拷貝裝備等。詳見下節。 |
| `vote/` | Runtime | ✓ | 投票資料（`poll/`、`ballot/`）。 |
| `data/server` | 配置 | △ | 工作站白名單；雖可變動但屬設定檔，不會自動重建。 |
| `document/README` | 文件 | ✗ | 原始中文說明，保留歷史流程，本文即以其為基礎擴寫。 |
| `edit/` | 歷史遺跡 | ✗ | DOS 編輯程式，現已不使用。 |
| `scripts/` | 工具 | ✗ | `clean-runtime.sh`、`bootstrap.sh`、`render-merc-ini.sh` 等建置腳本。 |
| `docker/` | 工具 | ✗ | Dockerfile 與 entrypoint。 |
| `docs/` | 文件 | ✗ | 現代化後新增的 Markdown 文件。 |
| `runtime-test/` | 測試 | ✓ | 本地 runtime fixture，供整合測試與 CI。 |
| `convert_big5_to_utf8.py`, `scripts/check-data.py` | 工具 | ✗ | 轉碼與資料驗證腳本。 |

> 依 `document/README` 列出的目錄基準，若要快速了解原始專案的預期結構，建議搭配本文與舊版 README 一起閱讀。

下列章節會針對「需要單檔解釋」的目錄更深入說明。

## `player/` 玩家資料

- 結構：`player/<letter>/<name>/data`。`<letter>` 取決於角色名稱第一個字的**小寫**版本。`data` 檔案內含角色屬性、經驗、物品等資訊。
- 來源：`save.c` 的 `file_name()` 直接組合 `player_dir` + 小寫首字母 + `/` + `<name>`；`create_dir()` 會在存檔時自動補建缺少的 bucket 與玩家目錄。
- Reset 建議：保留 `player/` 根目錄即可；`scripts/clean-runtime.sh` 會主動清空並重建常用 bucket，方便重置測試/打包環境，但 runtime 不再依賴 bucket 必須預先存在。

## `area/` 與世界資料

- 目錄結構（每個區域下）：`index`（必備基本資料）、`mob/`、`obj/`、`roo/`、`res/`、`mineral/`、`shp/`。原始 `document/README` 即以此格式說明。
- 在 UTF-8 版中，這些檔案都視為靜態內容（git 追蹤）。如要新增/調整區域，沿用相同目錄結構即可。

## `angel/`

- 內容：守護神（天使）設定，用於 `load_angel()` 載入 NPC 守護者。
- 重設需求：純靜態資料，一般不需變動；若要擴充，可複製既有檔案作為模板。

## `data/`

- 常見檔案：`data/server`（登入白名單）、`data/immlist`（GM 列表）、`data/bounty.txt`、`data/event.txt`、`data/sale.txt`、`data/welcome`, `data/welcome.imm`。
- 其中 `data/server`、`data/immlist` 會在 runtime 被管理者修改；依情境決定是清空還是保留模板。`docs/RUNTIME_RESET.md` 有更細的 reset 指引。

## `document/` vs `doc/`

- `document/`：翻新後的中文參考資料與授權（UTF-8）。
- `doc/`：保留原 Merc/Diku 英文手冊、授權、舊說明，主要用來查歷史或比對資料格式。

## `command/` 指令定義

## `command/` 指令定義

- `command/command.lst`：啟動時由 `load_commands()` 逐行讀入，決定指令載入順序。每一行對應下面某個 `.ins` 檔案的 `Name` 欄位。
- `command/<letter>/*.ins`：以英文字首分桶（`a/` 放 `address.ins`、`b/` 放 `buy.ins`…），每個 `.ins` 內含指令的中/英名稱、等級、對應的 `do_xxx` 函式、help 內容等欄位。開發新指令時需新增 `.ins` 並在 `command.lst` 中掛上對應名稱。

> 小技巧：`command/` 只會在 build 階段被讀入轉成 `command.o`，因此這些檔案不會在 runtime 被修改；若只要描述資料夾層級即可，不需要追蹤單檔差異。

## `etc/` 雜項設定檔

| 檔案 | `merc.ini` 參數 | 用途 | main 分支預設值 | Reset 建議 |
|------|-----------------|------|------------------|------------|
| `etc/address` | `ADDRESS FILE` | 記錄登入位址、白名單；`address` 指令與登入檢查會讀寫此檔 | 空檔 | 清空後由正式環境重新產生；不要把開發環境的 IP 留下 |
| `etc/check.txt` | `CHECK FILE` | `check` 指令 (anti-dupe) 會把重複序號裝備寫入此檔 | 空檔 | 清空即可 |
| `etc/club.txt` | `CLUB FILE` | 幫派/俱樂部設定，由 `load_club()/save_club()` 操作 | 依 repo 內容（若無則空） | 若需要重置，可還原 main 版或透過遊戲指令重新建立 |
| `etc/database` | `DATABASE FILE` | 記錄註冊/建立帳號時的資訊（帳號、IP、時間戳） | 空檔 | 清空；避免把舊玩家紀錄帶進新環境 |
| `etc/donate` | `DONATE FILE` | `donate` 指令用的設定（可捐金額、福利、等級門檻） | 文字檔（如 `Money 30000`…） | 若要改預設，修改此檔即可；reset 時保留模板 |
| `etc/hero` | `HERO FILE` | 英雄榜資料；啟動時需遇到 `End` sentinel | `End` | 覆寫為 `End\n` |
| `etc/ideas`, `etc/typos`, `etc/wizard.log` | 由對應指令寫入 | 玩家回報/巫師日誌 | 空檔 | 清空即可 |
| `etc/motd.txt` | MOTD | 登入畫面；純文字 | 見 repo 現有內容 | 依需求編輯 |
| `etc/net.log` | `NETLOG FILE` | 網路狀態紀錄，由核心自動 append | 空檔 | 清空即可 |
| `etc/player.new` | `NEW PLAYER FILE` | 紀錄新角色登入資訊 | 空檔 | 清空即可 |
| `etc/purge.dat` | `PURGE FILE` | 角色清除設定／排程 | 見 repo 現有內容 | 依需求編輯 |
| `etc/stock` | `STOCK FILE` | 股票系統初始五家公司 | 5 行預設值 (全部價格 10000) | **還原為預設內容**，不可刪除 |
| `etc/motd.txt`, `etc/donate`, `etc/club.txt`, `etc/purge.dat` | - | 純設定檔，build 時不會重寫 | 依 repo | Reset 時若要回到乾淨狀態，可 `git checkout main -- <檔案>` |

> 注意：`etc/merc.ini` 與 `etc/minimal.ini` 是設定模板；實際啟動時讀的是 `src/merc.ini`。`src/merc.ini` 應由 `scripts/render-merc-ini.sh` 生成，而不是直接進版控；production 容器內固定使用 `/app`，host-only 開發則生成本機絕對路徑。

## `debug/` 記錄檔

| 檔案 | `merc.ini` 參數 | 說明 |
|------|-----------------|------|
| `debug/bugs` | `BUGS FILE` | `bug` 指令與 `mudlog(LOG_DEBUG, …)` 會寫入；用於追蹤玩家回報
| `debug/error` | `ERROR FILE` | 致命錯誤或 `mudlog(LOG_ERROR, …)` 紀錄
| `debug/failenable`, `debug/failexit`, `debug/failload`, `debug/failpass` | `FAILENABLE FILE` 等 | 各種 fail-safe 的輸出（房間出口修復、載入失敗…）
| `debug/chat.log`, `debug/xnames.log`, `debug/suicide.log` | 對應指令／系統事件 | 依檔名直覺即可判讀
| `debug/badfile`, `debug/badobject`, `debug/suspect` | 驗證資料檔與可疑玩家的 dump | 如果需要留作證據，可在 reset 前備份；否則可以清空 |

這些檔案在 runtime 會自動 append，因此僅需確保檔案存在並授權 `mud` 可寫；reset 時可直接清空。

## 其他內容資料夾

- `greeting/`：登入畫面、情境文字；ASCII/ANSI 檔案以 `greeting/ansi.*` 命名。
- `help/`：`*.hlp` help 與 `help/immortal/` 等子目錄，對應遊戲內 `help` 指令。
- `skill/`：技能資料與權重設定；由 `load_skill_table()` 讀取。
- `social/`：社交指令集合（玩家 emote），類似 command 但較輕量。
- `joke/`：笑話集，供 `joke` 指令抽取。
- `vote/`：投票系統檔案（議題、票匭），為 runtime 內容但結構固定。
- `mail/`：玩家郵件；所有檔案為 runtime 內容。
- `log/`：`startup` 以及遊戲迴圈產生的日誌。預設會滾動產生 `1000.log`、`1001.log` …。

## 其他目錄備註

- `mail/`：玩家信件，完全 runtime 產物。Reset 時保留目錄、清空內容。
- `board/`：`board/*.lst`/`*.data` 為留言板結構，`board/imm/list` 與 `board/loyang/list` 需要保留 `End` sentinel；其他資料表可依需求保留或清空。
- `data/server`：登入白名單，並非 runtime 產物；reset 後通常需要修改成實際 IP 清單。

如需更細部的 reset 流程，請搭配 [`docs/RUNTIME_RESET.md`](RUNTIME_RESET.md)。
