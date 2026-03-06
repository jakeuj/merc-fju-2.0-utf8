# 三國歪傳資料速查

鏡像路徑：`/Users/jakeuj/auggie/3yWebsite`。HTML 為繁體中文 UTF-8，原網站採用多層 frame，請直接打開目標頁面以免迷航。

## 主選單對應

| 節點 | 主要路徑 | 摘要 | 特別注意 |
| --- | --- | --- | --- |
| 系統公告 | `news/`, `intro/`, `config/`, `imm/` | `news/index.html`列出 1999–2002 更新，逐日 HTML 以 `YYYYMMDDN.html` 命名。`intro/*.html` 解釋 MUD 概念、用語、Imm 角色。`config/index.html` 故事背景與開發者名單。`imm/*.html` 為大神介紹。 | 引述公告時寫明日期；若需要作者，於內文尋找 `font` 或 `<p>` 標籤。 |
| 新手上路 | `newhand/` | `rule/index`=玩家規則（2002.01 版）、`newbies/index`=十節入門手冊、`commands/index`=指令分類、`players/**/*.html`=各職業心得、`other/index`=消耗品/通緝等資料。 | 檔案高度結構化，可用 `rg -n "關鍵字" newhand -g '*.html'` 搜尋中文或英文字串。 |
| 技能資料 | `skill/` | `skill/index` 分類總表；各武器/法術/職業/步法/技能熟練度皆獨立 HTML（例如 `skill/sword.html`）。 | 先從索引抓到檔名再打開細節。 |
| 國家系統 | `realm/doc`, `realm/list`, `realm/ads` | `doc/index` 連到六份 `HELP` 文件，`list/index` 列 34 國（含英文名、狀態、國王），`ads` 收錄國家招募文。 | `list/index` 頂部標記資料日期 (2002/01/25)。 |
| 地圖/交通 | `map/` | `map/index` 為 ASCII 世界地圖與區域列表；各城鎮有單頁 ASCII 內圖；`bus.html` 為 22 站票價表。 | ASCII 內容等寬，複製時保留空白。 |
| 下載 & 開發 | `download/` | `index` 連到釋出版程式 (`merc-fju.tar.gz`, FreeBSD 版、2.0 版) 與 00–07 號撰寫教學（怪物/房間/物品/重置/商店/程式）。 | 若需說明區域製作，引用相對應的教學檔。 |
| 連結 | `link/index.html` | 全台各 MUD 的 telnet/BBS/Web 清單與協定。 | 條目含 `{D}` `{L}` 表示 Diku / LP 等架構；若需要特定遊戲資訊，可直接搜尋關鍵字。 |
| 版權宣告 | `announce/index.html` | 14 條 Merc/Diku 授權與製作群規範。 | 回應授權/再散布問題時務必對照條款號。 |
| 討論區 | `phorum.html` | Frame 指向 `http://kallamity.cv.nctu.edu.tw/...`，鏡像未含內容。 | 說明連結已失效並建議使用本地新聞或 BBS 備註。 |

## 快速搜尋技巧

- `rg -n "keyword" /Users/jakeuj/auggie/3yWebsite -g '*.html'`：搜尋中文/英文技能名稱、國家名等。
- `rg -n "2002/01" news/index.html`：在新聞索引定位某月份。
- `sed -n 'start,endp' <file>` 或 `bat --style=plain`：擷取段落避免載入整頁。
- `open`/`xdg-open` 會啟動 GUI 瀏覽器；多數情況使用文字工具較有效率。

## 時間與語系提示

- 新聞與規則最後更新約 2002-02-01；回答時標註「資料日期」。
- 中文內文採繁體，保留原用詞（例：「武將」、「伶人」）。若需要英文別名，可從同頁括號或表格取得。
- 指令/Help 名稱以小寫英文顯示，回答時建議格式為 `help skillname`。 
