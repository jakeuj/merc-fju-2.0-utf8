---
name: merc-local-ops
description: 操作 Merc-FJU 本機啟動、WSL 啟動、Windows wrapper、runtime 目錄權限修復與常見維運排錯的專用技能。當使用者要在 Linux/macOS/WSL/Windows 上啟動伺服器、詢問 start-merc.sh 或 start-merc.ps1 / start-merc.cmd 用法、修正 merc.ini 路徑、排查 log is not writable / shutdown.txt / 無法 listen port 等本機環境問題時使用。
---

# Merc Local Ops

使用本技能處理 Merc-FJU 在本機與 WSL 的啟動、停止、狀態檢查、權限修復與環境排錯。優先沿用 repo 既有入口與文件，不要重新發明另一套啟動流程。

## Quick Start
1. 先辨識使用者目前所在環境：Linux/macOS shell、WSL shell、PowerShell、cmd、還是 Docker。
2. 優先使用 repo 既有入口：
- Linux/macOS/WSL 內用 `./start-merc.sh <action>`。
- Windows shell 用 `.\start-merc.cmd <action>` 或 `.\start-merc.ps1 <action>`，讓 wrapper 轉進 WSL。
3. 若啟動失敗，先檢查 3 件事：
- `src/merc.ini` 的 `HOME DIRECTORY` 是否指向當前環境的真實路徑。
- `log/`, `player/`, `mail/`, `debug/`, `vote/` 是否可寫。
- `src/shutdown.txt` 是否殘留。
4. 若 `scripts/bootstrap.sh` 已經通過但伺服器仍很快退出，立刻檢查 `log/manual-start-*.log`，不要再把焦點停留在 wrapper 或 WSL 本身。
5. 需要具體指令、檢查順序或修復步驟時，再讀 [references/local-ops-cheatsheet.md](references/local-ops-cheatsheet.md)。

## Workflow Decision Tree
1. 若使用者是要「快速啟動」：
- 直接給對應 shell 的最短指令，並說明它實際會走到哪支 launcher。
2. 若使用者是要「修復啟動失敗」：
- 先判斷是 build 問題、路徑問題、權限問題、還是舊的 `shutdown.txt`。
- 若 `bootstrap` 與 `merc.ini` 都正常，下一層優先懷疑 area/mob/obj/reset 資料錯誤，例如 VNUM 重複、引用不存在房間、或格式損壞。
- 依問題類型提供最小修復步驟，不要一次丟整套長清單。
3. 若使用者是要「Windows 啟動」：
- 明確提醒 `src/merc` 是 Linux ELF，不要叫使用者直接在 PowerShell 執行它。
- 優先導向 `start-merc.cmd` / `start-merc.ps1`。
4. 若使用者是要「WSL 維運」：
- 先把 Windows 路徑換成 `/mnt/<drive>/...` 形式，再檢查 `merc.ini` 與 runtime 目錄權限。
5. 若使用者是要「修改 skill / docs / launcher」：
- 以 repo 既有 `start-merc.sh` 為單一真相來源，Windows 端只做 wrapper，不要複製邏輯。

## Response Rules
- 先給可直接執行的命令，再補 1 句原因或注意事項。
- 明確區分「在 WSL 內執行」與「在 Windows shell 執行」。
- 若路徑是相依於當前 workspace，避免寫死舊的 macOS 路徑。
- 講權限問題時，優先指出哪個目錄不可寫，以及這會在 `scripts/bootstrap.sh` 哪一步失敗。
- 若 repo 位於 `/mnt/c`、`/mnt/d`、`/mnt/h` 這類 drvfs 掛載點，預設要想到 `chmod` 可能失敗但目錄仍可能可寫；只有在實際不可寫時才引導修權限。
- 若只有 `player/` 在 WSL 掛載磁碟上異常不可寫，而且 `sudo` 不方便使用，可先備份現有玩家資料，再重建 `player/` 本體與 52 個 bucket 目錄，最後把玩家子目錄搬回去。
- 若需要重新生成 `src/merc.ini`，提醒使用 `MERC_FORCE_RENDER_INI=1` 與正確的 `MERC_HOME_VALUE`。
- 若 wrapper 已成功進入 WSL，後續錯誤多半不是 wrapper 本身，而是 WSL 內的權限、編譯或設定問題。
- 若 log 已經明確顯示資料載入錯誤，回答時要把問題切換成遊戲資料/世界設定修復，不要繼續描述成「啟動器壞掉」。

## What To Check Before Editing Code
- 先讀 `README.md` 的「本機（macOS / Linux / WSL）直接建置」與「在 WSL 中執行」段落。
- 先確認現有 launcher：`start-merc.sh`、`start-merc.ps1`、`start-merc.cmd`、`scripts/bootstrap.sh`。
- 若任務是修啟動，不要先改 skill；優先重現錯誤並確認是文件缺漏還是腳本缺陷。

## References
- [references/local-ops-cheatsheet.md](references/local-ops-cheatsheet.md)：Linux/macOS/WSL/Windows 的啟動指令、常見錯誤與修復順序。
