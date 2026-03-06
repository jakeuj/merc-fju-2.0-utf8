# 輔大三國歪傳之降龍伏虎 (Merc-FJU 2.0 UTF-8)

> 本專案為 **Merc 2.2** 修改版——輔仁大學「三國歪傳之降龍伏虎」MUD 遊戲伺服器，
> 已將原始 Big5 編碼全面轉換為 **UTF-8**，並補上 Docker 化的現代工具鏈，
> 讓遊戲在新款 Linux／macOS 可直接建置。傳統部署細節仍可參考
> `document/README`；本檔只整理 Merc-FJU 2.0 UTF-8 版的更新與快速上手流程。

## 試玩連線

目前有架設公開測試站，歡迎直接連入體驗：

```
telnet mud.jakeuj.com 3838
```

> 若您使用 macOS，可在終端機直接執行上述指令；
> Windows 使用者可安裝 [PuTTY](https://www.putty.org/) 或其他 Telnet 客戶端，
> 主機填 `mud.jakeuj.com`，連接埠填 `3838`，連線類型選 `Telnet`。
> 請確認客戶端的字元編碼設定為 **UTF-8**，否則中文顯示會出現亂碼。

## 快速開始

- **Docker**：先 `docker pull jakeuj/merc-fju-2.0-utf8:latest`，再用
  `make docker-run` 或 `docker run` 映射 3838/1234/8888，即可啟動會自動
  `make clean && make` 並執行 `src/startup merc.ini` 的容器。持久化、清潔、
  compose 範例請見 [docs/OPERATIONS.md](docs/OPERATIONS.md#Docker/容器流程)。
- **本機 / 裸機**：安裝 gcc/make/csh → `scripts/bootstrap.sh` →
  `cd src && make clean && make`，再以 `./start-merc.sh start|stop|restart|status`
  管理；該腳本會清理 `shutdown.txt`、記錄 PID 與 log。詳細步驟與 macOS 提示請見
  [docs/OPERATIONS.md](docs/OPERATIONS.md#本機macOS--linux流程)。
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

> 想了解 `command/`、`etc/`、`debug/` 等目錄底下檔案的作用，可參考 [`docs/DATA_LAYOUT.md`](docs/DATA_LAYOUT.md)。

更完整的檔案說明、傳統工具需求與授權條款，請參考 `document/README` 以及
`document/COPYRIGHT`。以下段落僅摘要 Merc-FJU 2.0 UTF-8 版新增或調整的重點。

## 現代化重點

- **UTF-8 化**：所有遊戲內容、介面文字與資料表皆完成 Big5→UTF-8 轉換，並修正
  `fread_string`、`merc.ini` 解析器等舊版無法處理多位元字元的 bug。
- **Docker 工具鏈**：`docker/Dockerfile` 安裝 `build-essential`、`csh`、`libxcrypt-compat`
  等依賴，確保舊程式可在新 Linux 核心上編譯與執行。
- **scripts/**：`bootstrap.sh` 負責初始化可寫目錄；`check-data.py` 可驗證
  `area/ skill/ angel/` 等資料是否仍為合法 UTF-8 與需有欄位。
- **etc/merc.ini**：已改以 `/app` 為 HOME DIRECTORY，並新增多組 `MUD PORT`（預設 3838、1234、8888）。
  若需要舊版參數對照，可比對 `document/README` 或 `docs/merc.ini.snapshot`。
- **data/server**：可設定免除多重登入／DNS 查詢的工作站白名單；新版 Docker host
  會在此列出（例如 `192.168.65.x`）。

## 開發與維運文件

- `docs/OPERATIONS.md`：集中說明 Docker 部署、資料掛載、start-merc 啟動與常用腳本。
- `docs/BUILD.md`：列出 Docker / docker compose 指令、主機直編、維運腳本與 CI 建議。
- `document/README`：完整傳統配備、資料結構與內容建置教學，供延伸閱讀或比對。
- `document/COPYRIGHT`、`doc/license.*`：沿用 Merc / Diku 與三國歪傳製作群的授權條款，
  仍須保留原作者資訊並不得商業使用。

## 原始製作群

| 姓名 | 學校／所系 | Email |
|------|-----------|-------|
| 蘇家興 | 輔仁大學化學研究所 86 期 | paul@mud.ch.fju.edu.tw |
| 周昀瑾 | 輔仁大學生物研究所 86 期 | lc@mud.ch.fju.edu.tw |
| 黃欣偉 | 輔仁大學化學研究所 85 期 | robinl@mud.ch.fju.edu.tw |
| 高智亮 | 師範大學化學研究所 85 期 | lumo@mud.ch.fju.edu.tw |
| 徐國財 | 輔仁大學化學研究所 84 期 | ene@mud.ch.fju.edu.tw |

## 翻新測試人

| 姓名 | 學校／所系 | Email |
|------|-----------|-------|
| 朱立恆 | 輔仁大學資管學系 95 期 | 495742481@m365.fju.edu.tw |

> 感謝原團隊與社群貢獻；若需完整歷史說明、原始說明書或轉檔腳本，
> 請查閱 `document/` 目錄（保留所有舊版 README／授權）以及 `convert_big5_to_utf8.py`。
