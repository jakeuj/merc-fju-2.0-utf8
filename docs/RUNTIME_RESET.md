# Runtime Reset Checklist

本表列出 Merc-FJU 執行期間會被修改、但在部署／打包時需特別處理的路徑。原始參考值以 `main` 分支的乾淨版本為準，develop 若因本機測試寫入資料，請依下述指示還原。

| 路徑 | 類型 | main 分支預設狀態 | 用途／程式假設 | Reset 動作 |
|------|------|-------------------|----------------|-------------|
| `player/<letter>/` | 目錄 | 存在但為空，只含 52 個字母桶 | `ini.c` 的 `ADJUST()` 會依角色第一個字元寫入 `player/X/<name>/data`，若桶不存在就會 `create_dir` 失敗 | **保留目錄結構**，但清除各桶內容。`scripts/clean-runtime.sh` 會自動重建 a–z/A–Z；若手動清空 Persistent Disk 記得重新建立 |
| `mail/`, `log/`, `debug/`, `vote/` | 目錄 | 存在但為空 | runtime 投遞信件、日誌與 debug dump 需要這些可寫目錄 | 目錄需存在並賦予 `mud:mud` 權限；內容可全刪 |
| `board/imm/list`, `board/loyang/list` | 檔案 | `End` | 留言板程式在啟動時讀取清單，缺少 `End` sentinel 會被視為壞檔 | 重寫為 `End\n` |
| `etc/address` | 檔案 | 空檔 (blob `e69de29...`) | `address` 指令記錄歷史連線/白名單；若帶有舊 IP 會造成誤判 | **清空**；部署後再由管理者重新寫入需要允許的 IP |
| `etc/database` | 檔案 | 空檔 | 記錄「建立帳號」驗證資訊，含帳號、IP、建立時間 | **清空**，避免把舊帳號/時間帶到新環境 |
| `data/immlist` | 檔案 | 空檔 | 儲存 GM 清單，`load_immlist()` 啟動時直接讀本檔 | **清空**；不要留著本地測試的神族名單 |
| `etc/stock` | 檔案 | 5 家股票，每家價格 10000 | 行情系統會讀固定 5 筆初始資料，若檔案不在或格式錯誤，指令會壞掉 | **還原預設內容**（README/`etc/stock` 現有 5 行），不要刪除 |
| `etc/hero` | 檔案 | `End` | 儲存英雄榜，讀寫時需要 sentinel | 覆寫為 `End\n` |
| `etc/net.log`, `etc/player.new` | 檔案 | 空檔 | 網路紀錄／新玩家通知 | 清空即可 |
| `debug/bugs`, `debug/error`, `debug/fail*` | 檔案 | 空檔 | runtime 產生，若殘留舊紀錄會混淆除錯 | 清空即可 |
| `data/server` | 檔案 | 內含 127.0.0.1/192.168.65.x 及 `#END` | 登入時判斷工作站白名單；此檔**不是** runtime 產出 | 如需更新白名單請手動編輯，但 reset 時請保留模板 |

## Reset 流程建議

1. 針對 staging 或 Persistent Disk，執行 `bash scripts/clean-runtime.sh <目錄>`，自動處理上述目錄/檔案（含 player 字母桶、`etc/stock` 模板、`board/*/list` sentinel）。
2. 若是在遠端環境手動清空 `player/`，請再次確認 `player/a`…`player/Z` 存在；缺少任一桶都會在建立角色時噴 `create_dir: 無法建立目錄 <Name>`。
3. 需自訂白名單或神族名單時，請在部署後再透過遊戲指令或手動編輯填入；不要把開發環境的 `etc/address`、`data/immlist` 直接帶進正式環境。
4. 文件中提到的檔案皆可參照 `main` 分支 (`git show main:<path>`) 取得乾淨模板，這也是自動化時最可靠的來源。
