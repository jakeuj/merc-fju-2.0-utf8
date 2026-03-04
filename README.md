# 輔大三國歪傳之降龍伏虎 (Merc-FJU 2.0 UTF-8)

> 本專案為 **Merc 2.2** 修改版——輔仁大學「三國歪傳之降龍伏虎」MUD 遊戲伺服器，
> 已將原始 Big5 編碼全面轉換為 **UTF-8**，並補上 Docker 化的現代工具鏈，
> 讓遊戲在新款 Linux／macOS 可直接建置。傳統部署細節仍可參考
> `document/README`；本檔只整理 Merc-FJU 2.0 UTF-8 版的更新與快速上手流程。

## 快速開始

最省事的方式是使用提供的 Ubuntu 24.04 Docker 容器：

```bash
cd /Users/jakeuj/auggie/mud2
make docker-build          # 建立 merc-fju 基底映像
make docker-run            # 以 -p 13838/11234/18888 映射並啟動
```

容器入口點會自動：

1. 以非 root 身份執行 `scripts/bootstrap.sh` 建立 `log/ player/ mail/` 等可寫目錄。
2. 在 `/app/src` 執行 `make clean && make`，將 `merc` 與 `.o` 全數重新建置並回報編譯警告。
3. 啟動 `./startup merc.ini`，使用與舊版相同的 csh 循環與日誌輪替。

Docker 內部的 `HOME DIRECTORY` 固定設為 `/app`，因此本地程式碼與遊戲資料
（`area/ skill/ angel/ board/ player/` 等）都會透過 bind mount 保留在主機目錄。
若要 shell 進容器，可使用 `make docker-shell` 或 `docker exec -it merc-fju /bin/bash`。

> 若無 Docker，或需要遵循舊式手動編譯方式，請直接閱讀
> `document/README`（保留原始說明與硬體需求），並依照其中「新手上路」章節流程操作。

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

## 手動編譯（可選）

若仍想於主機直接建置，可遵循 `document/README` 的舊流程，再補上以下調整：

```bash
# Linux / macOS
cd src
make clean && make
```

確保系統已安裝 `gcc`、`make`、`csh`、`libxcrypt-compat`（Ubuntu）或等價套件，
並在 `etc/merc.ini` 中調整目錄與埠號：

```ini
NAME            <你的遊戲名稱>
MUD PORT        <連線埠號>
HOME DIRECTORY  <遊戲實際路徑>
```

啟動方式與舊版相同（`./startup` 會在 `log/` 內滾動紀錄，並於 `shutdown.txt` 出現時終止）：

```bash
cd src
./startup &
```

## 開發與維運文件

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
