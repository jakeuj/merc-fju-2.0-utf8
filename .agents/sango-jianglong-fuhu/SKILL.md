---
name: sango-jianglong-fuhu
description: 提供 https://github.com/jakeuj/3yWebsite 鏡像的三國歪傳之降龍伏虎（Sango 3838）官網資訊：新聞、規則、技能、國家、地圖、下載與授權。任何需要參考此網站內容回答玩家或歷史問題時使用。
---

# 三國歪傳之降龍伏虎資料指南

## Overview
- 本技能協助你使用 `https://github.com/jakeuj/3yWebsite` 的網站鏡像快速定位三國歪傳之降龍伏虎（Sango 3838）官網內容。
- 鏡像主要涵蓋 1999–2002 的新聞、玩家手冊、技能資料、國家系統、地圖、下載檔與授權聲明，全部為繁體中文 HTML，仍沿用 frame 架構。

## Quick Start
1. **鎖定需求**：釐清使用者要找的是最新公告、玩法教學、技能數據、國家資訊、地圖交通、下載/授權，或外部連結。必要時確認時間點，因內容停在 2002-02-01 周邊。
2. **選擇區塊**：對照《references/section-cheatsheet.md》的表格，找出對應目錄（例如 `news/`, `newhand/`, `skill/`, `realm/`, `map/`, `download/`, `announce/`, `link/`）。
3. **定位檔案**：
   - 先到 `https://github.com/jakeuj/3yWebsite` 對照目錄；若手邊有 repo 副本，再用 `rg -n "關鍵字" <repo-path> -g '*.html'` 尋找中文/英文技能名、help 指令、國家名稱。
   - 直接 `sed -n 'a,bp' <file>` 或 `bat --style=plain` 擷取段落，避免整頁載入。
4. **摘要並標註日期/路徑**：說明原文語境（例如公告日期、表格標頭），保留原用詞，若有 help 名稱以反引號包住（例：`help skills`）。
5. **回應限制**：若請求需要 phorum 討論區內容，說明鏡像只有連結沒有備份，改引用網站其他來源或提出限制。

## Directory Guide
- `body.html` 與 `topmenu.html` 只是 frame，直接進入子頁面可避免額外框架。
- `news/index.html` 是新聞彙整表，點進去的 `YYYYMMDDN.html` 為全文，通常含更新描述與實裝時間。
- `newhand/` 下分 `rule`, `newbies`, `commands`, `players`, `other` 等多層段落，皆有「閱讀導覽」方便跳轉；用 `#anchor` 搜索同頁段落。
- `skill/index.html` 提供分類與檔名，查詢特定技能時先記錄中文名與對應 HTML 檔。
- `realm/*` 分別對應 help 文件 (`doc`)、現有國家列表 (`list`)、國家廣告 (`ads`)。
- `map/` 內文為 ASCII 圖；複製時保持空白與字距。
- `download/index.html` 既有 tarball 連結也有 00–07 號撰寫教學，可回答程式/區域編寫問題。
- `announce/index.html` 與 `config/index.html`、`imm/index.html` 提供授權、故事背景與開發者資訊。
- `link/index.html` 包含其他 MUD 的 telnet/BBS/Web；`phorum.html` 僅是外站連結（鏡像未抓到內容）。

## Workflow Patterns
### A. 最新公告 / 系統調整
1. 開 `news/index.html` 找到日期列（使用瀏覽器或 `rg -n "2002/01" news/index.html`）。
2. 進入對應 `news/YYYYMMDDN.html` 取得詳細內容。描述時明確標示日期與主題（例：「2002/02/01 隱藏術」）。
3. 若要交代策源，補充 `config/index.html` 或 `imm/index.html` 的開發者背景。

### B. 規則與入門
1. `newhand/rule/index.html`：玩家規則 10 條，2002.01 版。回答違規、Robot、多重連線等議題時引用條次。
2. `newhand/newbies/index.html`：10 個章節（經驗、技能、學習、致能、戰鬥、職業、轉職、升屬性、練功、屬性分配）。為新手流程提供 step-by-step 指引。
3. `newhand/commands/index.html`：按主題整理方向/狀態/團隊/戰鬥/技能/交易/物件/溝通/交通/組態/查詢指令；回答指令用途時引用表格。
4. `newhand/other/index.html`：藥水、藥物、捲軸、免死金牌、極限、死亡、通緝、賞金、版權 `help` 內容，適合回答道具或系統上限。

### C. 技能、職業與教學
1. `skill/index.html` 決定分類 → 開對應 HTML（如 `skill/fire.html`, `skill/thief.html`, `skill/step.html`, `skill/learnlv.html`）。
2. 若需要職業心得/玩法，進入 `newhand/players/<class>/index.html` 或 `newhand/players/other`。
3. 對「新技能開放」類問題，確認 `news/` 相關公告並對照 `skill/` 描述。

### D. 國家、地圖、交通
1. 國家功能/指令：`realm/doc/0*.html` 依 help 名稱拆分；列表：`realm/list/index.html`（標註資料日期 2002/01/25）；廣告：`realm/ads/*.html`。
2. 地理：`map/index.html` 的總圖與區域清單、各城市頁面 ASCII 地圖、`map/bus.html` 的 22 站票價表。
3. 需要地名位置時可引用 ASCII 圖，並提醒保持等寬字體。

### E. 下載、授權與開發
1. `download/index.html`：tarball 連結（`merc-fju.tar.gz`, `fjumud.tar.gz`, `merc-fju-2.0.tar.gz`）與撰寫手冊 `00.html`–`07.html`（怪物、房間、物品、重置、標題、商店、程式）。
2. `announce/index.html`：14 條 Merc/Diku 授權與製作群規範；回答再散佈或募款問題時引用對應條次。
3. `config/index.html`：故事背景與程式修改者名單；`imm/index.html` 依等級列出顧問與大神個人頁。

### F. 外部社群
- `link/index.html`：若被要求提供其他 MUD 或相關站台，從此頁擷取 telnet/BBS/Web 三元資訊，注意 `{D}/{L}/{T}` 表示程式架構。
- `phorum.html`：僅殘留外部論壇 URL，說明鏡像無備份內容，可改引用 `news/` 或建議使用現代平台。

## Referencing & Response Tips
- 在回答中標註來源檔（例：`news/200201251.html`）與內文日期；若內容自表格，說明欄位名稱。
- ASCII 圖或大型表格可擷取核心片段，必要時提及「來源為 map/index.html」並提醒保持等寬呈現。
- 中文內文請保留原名詞；英文 help/指令名稱使用反引號或括號清楚表示。
- 若資訊缺失（例：線上論壇內容），明確說明鏡像限制並提供可行替代（如 `news/` 或 `link/` 中的 BBS 位址）。

## Resources
- [`references/section-cheatsheet.md`](references/section-cheatsheet.md)：主選單與目錄速查表、搜尋指令，以及時間/語系備註。
