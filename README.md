# 輔大三國歪傳之降龍伏虎 (Merc-FJU 2.0 UTF-8)

> 本專案為 **Merc 2.2** 修改版——輔仁大學「三國歪傳之降龍伏虎」MUD 遊戲伺服器，
> 已將原始 Big5 編碼全面轉換為 **UTF-8**，方便在現代 Linux / macOS 環境下編譯與使用。

## 目錄結構

| 目錄 | 說明 |
|------|------|
| `src/` | C 語言原始程式碼 |
| `area/` | 遊戲區域資料 |
| `angel/` | 守護神設定資料 |
| `command/` | 指令資料 |
| `data/` | 系統資料 |
| `document/` | 本版參考手冊 |
| `doc/` | 原始 Merc 參考文件 |
| `include/` | 職業、地形等設定 |
| `etc/` | 雜項設定（含 `merc.ini`） |
| `greeting/` | 進站畫面 |
| `help/` | 線上求助檔案 |
| `skill/` | 技能資料檔案 |
| `social/` | 社交指令 |
| `board/` | 版面設定與資料 |
| `joke/` | 笑話集 |
| `player/` | 玩家存檔（執行時產生） |
| `mail/` | 玩家信件（執行時產生） |
| `log/` | 記錄檔（執行時產生） |
| `debug/` | 錯誤回報（執行時產生） |
| `vote/` | 投票資料 |

## 系統需求

- **OS**：Linux（核心 2.0.30 以上）或 FreeBSD
- **記憶體**：建議 32 MB 以上（執行需約 12 MB）
- **工具**：`gcc`（≥ 2.7.2.2）、`make`、`tar`、`zip`
- **函式庫**：`crypt` library（一般 Linux 內附）
- `/proc` 虛擬檔案系統支援（不需 root 權限）

## 編譯

```bash
# 一般 Linux
cd src
make clean && make

# FreeBSD
cp Makefile.bsd Makefile
cd src
make clean && make
```

編譯完成後，`src/` 目錄下會產生可執行檔 `merc`。

## 設定

編輯 `etc/merc.ini`，至少設定以下三項：

```ini
NAME            <你的遊戲名稱>
MUD PORT        <連線埠號>
HOME DIRECTORY  <遊戲實際路徑>
```

## 啟動

```bash
cd src
./startup &
```

第一個連線的玩家將成為超級管理者（Implementor）。

若遇問題，請查閱 `log/` 或 `debug/` 目錄中的錯誤訊息。

## 版權

本版本基於 **Merc 2.2**，版權需遵守 `doc/license.doc` 及 `doc/license.txt` 的 Diku/Merc 授權條款。

本地修改版（三國歪傳製作群）版權說明請見 [document/COPYRIGHT](document/COPYRIGHT)，重點摘要：

- 限制範圍目錄：`src/`、`area/`、`angel/`、`data/`、`greeting/`、`social/`、`document/`、`etc/`、`help/`、`edit/`
- 可自由修改與再發行，**不得涉及商業行為**
- 公開架設須保留 Diku、Merc 及「三國歪傳之降龍伏虎」字樣
- 公開架設區域不得含有釋放版本區域（limbo 除外）
- 詳細規定請閱讀 [document/COPYRIGHT](document/COPYRIGHT)

## 原始製作群

| 姓名 | 學校／所系 | Email |
|------|-----------|-------|
| 蘇家興 | 輔仁大學化學研究所 86 期 | paul@mud.ch.fju.edu.tw |
| 周昀瑾 | 輔仁大學生物研究所 86 期 | lc@mud.ch.fju.edu.tw |
| 黃欣偉 | 輔仁大學化學研究所 85 期 | robinl@mud.ch.fju.edu.tw |
| 高智亮 | 師範大學化學研究所 85 期 | lumo@mud.ch.fju.edu.tw |
| 徐國財 | 輔仁大學化學研究所 84 期 | ene@mud.ch.fju.edu.tw |

## UTF-8 轉換說明

本 fork 使用 [convert_big5_to_utf8.py](convert_big5_to_utf8.py) 將原始 Big5 編碼的 2868 個檔案批次轉換為 UTF-8，以利現代工具鏈與版本控制使用。
