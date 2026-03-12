---
name: mud-source-explainer
description: Hands-on guide for the 三國歪傳之降龍伏虎 (Merc-FJU) MUD source tree in the current workspace. Use when compiling or debugging src/merc, editing merc.ini, starting the server from Linux/macOS/WSL/Windows wrappers, updating area/angel/skill data, or answering repo-structure questions about this game.
---

# Sango Jianglong Fuhu Source

使用本技能處理目前工作區內的 Merc-FJU 2.0 UTF-8 原始碼樹，例如查詢目錄結構、編譯/啟動伺服器、調整 `merc.ini`、或編輯 area/angel/skill 等資料。以下說明依台灣慣用繁體中文撰寫，遇到英文術語時保留英文並補充意義。

## Quick Start Workflow（快速啟動流程）
1. 先確認目前 shell 位於專案根目錄；不要假設固定路徑，優先以目前工作區為準。
2. 先讀 [references/overview.md](references/overview.md) 取得建置重點，再搭配 [docs/DATA_LAYOUT.md](docs/DATA_LAYOUT.md) 確認資料夾責任與 runtime 行為。
3. 依目標環境編修 `src/merc.ini` 或 `etc/merc.ini` 模板；若 repo 路徑變更或切到 WSL，優先重新生成 `src/merc.ini`，確認 `HOME DIRECTORY`、port 清單與政策欄位正確。
4. 進入 `src/` 執行 `make clean && make`，得到 Linux/macOS/WSL 對應平台的 `merc` 後再啟動。
5. 啟動時預設優先使用 `./start-merc.sh start`；若人在 Windows shell，優先使用 `.\start-merc.cmd start` 或 `.\start-merc.ps1 start` 轉進 WSL，再由 wrapper 呼叫 `./start-merc.sh`。
6. 觀察 `log/manual-start-*.log` 或 `log/####.log` 是否持續滾動，並確認 `src/shutdown.txt` 沒有殘留。
7. 編輯 area/help/social 時遵循 `document/*.txt` 模板並維持 UTF-8，必要時套用 `scripts/check-data.py`、`convert_big5_to_utf8.py` 驗證資料。
8. 若 `start-merc.sh` 已成功進入載入流程卻仍退出，先把問題分類成「遊戲資料層錯誤」而不是「啟動器/WSL 錯誤」，並從最新 log 抓第一個 `BUG:` 或 parse error。

## Repository Layout Essentials（目錄速覽）
- `src/`: C 程式碼、`startup`、`Makefile.*`、`merc.ini`。建置與 IPC 相關調整都在這裡。
- `area/`, `angel/`, `command/`, `skill/`, `social/`: 世界資料（NPC、技能、社交動作、指令描述）。
- `include/`, `data/`: 共享表格與全域設定，`data/` 部分檔案可在 runtime 被管理者覆寫。
- `document/`, `doc/`: 分別為更新後的中文指南與原始 Merc/Diku 英文說明。
- `greeting/`, `help/`, `joke/`: 玩家登入畫面與線上說明文；`help/*.hlp` 受 `merc.ini` 的 `Help Extension` 控制。
- Runtime 產出集中在 `player/`, `mail/`, `log/`, `debug/`, `vote/`, `board/`, `etc/`（部分），清理時依 `docs/RUNTIME_RESET.md` 腳本或手冊操作。

## Building and Running（建置與執行）
- Linux 預設：`cd src && make clean && make`。BSD 需先以 `cp Makefile.bsd Makefile` 切換。
- 編譯需求：gcc ≥ 2.7、`make`、`crypt`、POSIX shell 工具。若缺 libcrypt，於 `Makefile` 補上 `-lcrypt`。
- `startup` 是 `csh` 無限迴圈，會設定 ulimit、輪換 `log/*.log`，除非偵測到 `shutdown.txt` 才停止。
- 專案另提供 `start-merc.sh` 作為較安全的單次入口：會先跑 `scripts/bootstrap.sh`、必要時自動 `make`、清掉殘留 `src/shutdown.txt`，並把 PID 寫到 `log/merc.pid`。
- 在 Windows 11 + WSL 情境下，不要直接在 PowerShell 執行 Linux ELF `src/merc`；改用 `start-merc.cmd` 或 `start-merc.ps1`，讓它自動把 Windows 路徑轉成 `/mnt/<drive>/...` 後呼叫 `./start-merc.sh`。
- 啟動前確認 `player/??`, `log/`, `mail/` 等目錄擁有寫入權限，否則伺服器會在初始化時退出。
- 若 repo 位於 `/mnt/c`、`/mnt/d`、`/mnt/h` 等 WSL 掛載磁碟，`chmod` 可能失敗但目錄仍可寫；若同時不可寫，先修正掛載權限或目錄 owner，再重試啟動。

## Configuration Guidance (`src/merc.ini`)
- `MUD PORT`: 每行 >1024 代表一個監聽 socket，維運多埠時保持排序與註解。
- 伺服器識別：`NAME`, `HOME DIRECTORY`, `URL`, `EMAIL` 需依部署更新。
- IPC 與 Idle：`IPC KEY`, `IPC Block`, `Idle Reset`, `Idle Check` 控制共享記憶體與自動踢人行為。
- Access Policy：`Strict Password`, `Strict Email`, `Multi login`, `Check Server`, `FQDN Limit` 決定登入限制。
- Gameplay Toggle：`Player Angel`, `Angel Level/Times`, `Attack Value`, `Skill Value`, `Group Exp Rate` 等會改變遊戲平衡，變動前留文件。
- 目錄鍵值：`HELP DIRECTORY`, `SOCIAL DIRECTORY`, `QM Directory`, `GREET DIRECTORY` 等皆相對於 `HOME DIRECTORY`，與實際 repo 保持一致。

## 資料佈局重點（整合 [docs/DATA_LAYOUT.md](docs/DATA_LAYOUT.md)）
- 靜態 vs Runtime：`src/`, `area/`, `angel/`, `command/`, `skill/`, `social/`, `document/`, `doc/`, `scripts/`, `docker/` 為靜態；`player/`, `mail/`, `log/`, `debug/`, `vote/`, `runtime-test/` 會在 runtime 生成；`data/`, `board/`, `etc/` 屬半動態，需看具體檔案是否被系統覆寫。
- `player/`: 採 52 個字母桶（`player/a`…`player/Z`），內含 `<角色名>/data`。`ini.c` 的 `adjust_filename()` 不會自動建桶，reset 時可跑 `scripts/clean-runtime.sh` 重建。
- `area/`: 每區域含 `index`, `mob/`, `obj/`, `roo/`, `res/`, `mineral/`, `shp/`。UTF-8 版視為靜態，依 `document/*.txt` 模板撰寫即可。
- `area/` 的 VNUM 區段必須全域唯一；同時載入的兩個區域若重複宣告同一個 room/mob/obj VNUM，伺服器會在載入期間中止。
- `command/`: `command/command.lst` 決定載入順序，條目名稱須與 `command/<letter>/*.ins` 的 `Name` 欄位一致；新增指令時新增 `.ins` 並更新 `command.lst`。
- `etc/`: 對應 `merc.ini` 的檔案鍵值（例如 `ADDRESS FILE`, `HERO FILE`, `STOCK FILE`）。`etc/stock` 須保留五家公司預設值；`etc/hero` 至少包含 `End` sentinel；`etc/motd.txt`, `etc/donate`, `etc/club.txt`, `etc/purge.dat` 建議以 Git 版為模板後再覆寫。
- `data/`: 包含 `server`（登入白名單）、`immlist`, `bounty.txt`, `event.txt`, `sale.txt`, `welcome`, `welcome.imm` 等；其中 `server`、`immlist` 會在 runtime 被管理者修改，需要依部署情境決定是否 reset。
- `debug/`: 記錄 `bug`, `error`, `fail*`, `chat.log` 等檔案，啟動前確保檔案存在且可寫，reset 時可直接清空。
- 其他資料夾：`greeting/`（ASCII/ANSI 畫面）、`help/`（`*.hlp`）、`social/`, `skill/`, `joke/`, `vote/` 等，除非刻意變更內容，平時視為靜態檔案。

## Runtime 清理與重建
- 若需清理整個 runtime footprint，優先閱讀 [docs/RUNTIME_RESET.md](docs/RUNTIME_RESET.md) 並透過 `scripts/clean-runtime.sh` 自動處理 `player/`, `mail/`, `log/`, `debug/`, `vote/`, `board/` 等目錄。
- Reset `etc/`、`data/` 時，務必依表格判斷哪些檔案以空檔起始（如 `etc/address`, `etc/database`）、哪些需保留 sentinel（`etc/hero`, `board/*/list`）。
- 在 GCP 或本地 Persistent Disk 重新掛載後，記得再跑一次桶目錄建立腳本，否則 `merc` 啟動時會在 `adjust_filename()` 階段失敗。

## Git / Commit Hygiene（提交衛生）
- 提交功能或內容變更前，先把 `git status --short` 分成「靜態資料」與「runtime 產物」兩類來看，不要把執行過程中自然變動的檔案一起提交。
- `etc/`, `data/`, `board/` 都是半動態區；同一路徑下可能同時存在應提交的設定檔與不應提交的 runtime 檔，必須逐檔判斷。
- 常見應排除的 runtime 變更包含 `etc/wizard.log`、管理期間被刷新但未刻意編修的 `etc/hero`、以及各種 log / cache / player state 檔案。
- 若這次任務確實有修改 `merc.ini`、`bounty.txt`、`bus.txt`、`ship.txt`、`help/*.hlp` 等靜態或部署設定，應只 stage 這些目標檔；不要因為它們和 runtime 檔同在 `etc/` 或 `data/` 就一起提交。
- 提交前可先用 `git restore --staged <path>` 把誤加入 index 的 runtime 檔移出，再做一次 `git status --short` 確認 commit 範圍。
- 遇到大型世界替換或 reload 測試後，特別要重查 `etc/hero`、`etc/wizard.log`、`log/`, `debug/` 是否被執行流程動到；這些通常屬本機/伺服器狀態，不屬於功能交付內容。

## Content Editing（內容編輯）
- `document/mob.txt`, `obj.txt`, `room.txt`, `reset.txt`, `shop.txt` 提供標準欄位解釋；改區域時先補完 `index` 再更新 `mob/obj/roo/res` 等。
- `angel/` 內的守護神設定會被 `src/angel.c` 解析，ID 需與程式碼中的枚舉一致。
- `help/` 與 `social/` 檔名需搭配 `merc.ini` 的副檔名設定（預設 `.hlp`、`.soc`）。
- 新增資料後可在遊戲內使用 `reload area <file>`、`reload social` 等指令（若權限允許）來熱載入；否則重啟 `startup`。

## Troubleshooting & Maintenance（偵錯維護）
- Build 失敗時：`make clean` 後重新編譯，確認 `include/` 內 header 未缺；若是 BSD/Clang，檢查 `Makefile` flag。
- Runtime 問題：先看 `log/<數字>.log` 與 `debug/error`，再確認 `shutdown.txt` 是否被誤寫。
- Windows/WSL 問題：若 `start-merc.ps1` 已成功進入 WSL 但 `scripts/bootstrap.sh` 報 `log is not writable`，優先處理 `/mnt/<drive>/...` 目錄權限，而不是修改 launcher 邏輯。
- 若 `scripts/bootstrap.sh` 已成功而 `merc` 仍退出，立即查看 `log/manual-start-*.log`；常見第二層原因是 `area/` 內重複 VNUM 或壞掉的引用，而不是 wrapper 或 shell 問題。
- 若只有 `player/` 在 `/mnt/<drive>` 上不可寫，且 repo 根目錄本身可寫，可先備份實際玩家資料，重建 `player/` 與 bucket 目錄後再恢復玩家子目錄。
- 玩家資料卡住：檢查 `merc.ini` 中的 `File Quota`, `Hold day` 限制，必要時清理 `player/<letter>/<name>/data`。
- 法律需求：遵守 Merc/Diku 授權，禁止商業化並保留原始 CREDIT。

## References（延伸文件）
- [references/overview.md](references/overview.md)：建置總覽、啟動檢查清單。
- [docs/DATA_LAYOUT.md](docs/DATA_LAYOUT.md)：目錄用途、runtime 產物、reset 範例。
- [docs/RUNTIME_RESET.md](docs/RUNTIME_RESET.md)：針對 runtime 檔案的清理腳本與人工流程。
