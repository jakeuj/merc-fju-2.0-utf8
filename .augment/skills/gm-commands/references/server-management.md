# 伺服器管理指令

## 廣播與通訊

### echo — 向當前房間廣播訊息（L_ANG）
```
echo <訊息>
```
將訊息發送到執行者所在的房間內所有人。

### aecho — 向全伺服器廣播（L_ANG）
```
aecho <訊息>
```
向所有在線玩家廣播訊息（系統公告）。

### recho — 向指定房間廣播（L_ANG）
```
recho <房間vnum> <訊息>
```
向特定房間內的所有人發送訊息。

### immtalk — 神族頻道（L_ANG）
```
immtalk <訊息>
```
或使用簡短語法 `: <訊息>`（colon 快捷方式）。
只有神族（immortal）等級的管理員可見。

---

## 存取控制

### ban — 封鎖 IP（L_SUP）
```
ban <IP位址> [all|newbie|mortal]
```
封鎖特定 IP 的連線。`all` 封鎖所有人，`newbie` 封鎖新帳號建立，`mortal` 封鎖一般玩家。

### allow — 解除封鎖（L_SUP）
```
allow <IP位址>
```
解除對指定 IP 的封鎖。

### disconnect — 強制斷線（L_SUP）
```
disconnect <連線編號>
```
強制切斷指定連線（可從 `users` 指令取得連線編號）。

### wizlock — 鎖定伺服器不接受新玩家（L_SUP）
```
wizlock
```
切換鎖定狀態，鎖定後只有神族等級可以登入。

---

## 監控與診斷

### users — 查看連線中的玩家（L_ANG）
```
users
```
列出目前所有連線，顯示連線編號、IP、玩家名稱、狀態等。

### sysinfo — 系統資訊（L_ANG）
```
sysinfo
```
顯示伺服器的系統資訊，如記憶體使用、執行時間等。

### data — 查看遊戲資料統計（L_GOD）
```
data
```
顯示遊戲資料的記憶體統計（mob/obj/room 的數量與佔用）。

### debug — 除錯模式切換（L_ANG）
```
debug
```
開啟或關閉伺服器的除錯輸出模式。

### status — 查看系統狀態（L_GOD）
```
status
```
顯示伺服器的詳細執行狀態。

---

## 伺服器控制

### reboot — 重新啟動伺服器（L_SUP）
```
reboot
```
安全重啟伺服器，會先儲存所有玩家資料。

### shutdown — 關閉伺服器（L_SUP）
```
shutdown
```
完全關閉伺服器。

### cleanup — 清理資料（L_SUP）
```
cleanup
```
執行記憶體與資料的清理作業。

---

## 檔案系統操作（L_ANG）

| 指令 | 說明 |
|------|------|
| `ls` / `directory` | 列出目錄內容 |
| `cd` | 切換工作目錄 |
| `cat` | 顯示檔案內容 |
| `cp` | 複製檔案 |
| `md` | 建立目錄 |
| `rd` | 刪除目錄 |
| `rm` | 刪除檔案 |
| `touch` | 建立空白檔案 |
| `pwd` | 顯示當前目錄 |
| `grep` | 搜尋檔案內容 |
| `filename` | 顯示相關路徑 |
| `filestat` | 顯示檔案狀態 |
| `fcntl` | 檔案控制操作 |

---

## 其他輔助指令

### wizhelp — 列出所有神族指令（L_ANG）
```
wizhelp
wizhelp <指令名稱>
```
不加參數列出所有可用的神族指令；加指令名稱顯示該指令說明。

### holylight — 切換神聖視野（L_ANG）
```
holylight
```
開啟後可以看見隱形生物與物品，並在黑暗中仍能正常視物。

### invis — 切換隱形狀態（L_ANG）
```
invis
invis <等級>
```
切換自身隱形，可指定僅低於某等級的人看不見你。

### nodeath — 不死模式（L_SUP）
```
nodeath
```
切換不死狀態，開啟後不會因傷害死亡。

### bamfin / bamfout — 設定傳送訊息（L_ANG）
```
bamfin <傳入訊息>
bamfout <傳出訊息>
```
設定自己使用 goto/transfer 時顯示的動態訊息。

### address — 顯示玩家 IP（L_GOD）
```
address <玩家名稱>
```
查詢玩家的連線 IP 位址。

### xnames — 管理禁用名稱（L_GOD）
```
xnames
```
管理被禁止使用的角色名稱清單。

