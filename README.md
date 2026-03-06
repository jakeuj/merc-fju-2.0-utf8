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

預先建置的映像已發佈於 Docker Hub，可直接拉取使用：

[![Docker Hub](https://img.shields.io/docker/pulls/jakeuj/merc-fju-2.0-utf8?logo=docker)](https://hub.docker.com/repository/docker/jakeuj/merc-fju-2.0-utf8)

```bash
docker pull jakeuj/merc-fju-2.0-utf8:latest
```

或從原始碼自行建置（需先 clone 本專案）：

```bash
cd /Users/jakeuj/auggie/mud2
make docker-build          # 建立 merc-fju 基底映像
make docker-run            # 以 -p 13838/11234/18888 映射並啟動
```

容器入口點會自動：

1. 以非 root 身份執行 `scripts/bootstrap.sh` 建立 `log/ player/ mail/` 等可寫目錄，並在缺少 `src/merc.ini` 時自動由 `etc/merc.ini` 模板生成。
2. 在 `/app/src` 執行 `make clean && make`，將 `merc` 與 `.o` 全數重新建置並回報編譯警告。
3. 啟動 `./startup merc.ini`，使用與舊版相同的 csh 循環與日誌輪替。

Docker 內部生成的 `src/merc.ini` 會把 `HOME DIRECTORY` 固定設為 `/app`，因此本地程式碼與遊戲資料
（`area/ skill/ angel/ board/ player/` 等）都會透過 bind mount 保留在主機目錄。
若要 shell 進容器，可使用 `make docker-shell` 或 `docker exec -it merc-fju /bin/bash`。

### 映像乾淨化與釋出建議

- `docker/Dockerfile` 在 `COPY . /app` 之後會執行 `scripts/clean-runtime.sh`，自動清空 `player/ mail/ log/ debug/ vote/` 等 runtime 目錄，並重設 `data/immlist`、`etc/database`、`etc/address`、`etc/stock` 等敏感檔案為 GitHub 上的乾淨值，最後移除 `.git`。即使本地開發時留下玩家檔或免洗帳號，建置出的映像也不會帶入這些資料。
- 若需要手動檢查某個 staging 目錄是否乾淨，可執行 `bash scripts/clean-runtime.sh <絕對路徑>`；請避免直接對正在服務的實際資料夾執行（會刪掉所有玩家資料）。
- `.dockerignore` 會排除 `.git/`、`player/` 等目錄，減少 build context 並避免 runtime 檔案意外被複製到映像中。
- 雖然 `player/` 在執行時會新增/刪除玩家檔，但映像內仍會隨附 52 個字母分桶（`player/a` … `player/Z`，每一桶底下才是 `<角色名>/data`），因為伺服器存檔時會直接假設這些目錄存在；若手動清空 Persistent Disk，請務必再次執行 `scripts/clean-runtime.sh` 或自行重建這些子目錄，否則會出現 `create_dir: 無法建立目錄 <Name>` 錯誤。
- 有些「會被更新但不能刪除」的檔案需要還原為預設內容，例如 `etc/stock`（預設五家股票）、`etc/address`、`etc/database`、`data/immlist`；上述檔案應在 build 或部署流程中用模板覆寫，而不是直接移除，否則伺服器啟動後會缺少必要設定。
- 更詳細的 reset 清單與每個檔案用途，請參考 [`docs/RUNTIME_RESET.md`](docs/RUNTIME_RESET.md)。
- 發佈到 GCP Artifact Registry 或 Docker Hub 之前，建議流程：`git status` 確認程式碼已 commit → `docker build -t merc-fju:release -f docker/Dockerfile .` → `docker tag`/`docker push`。如此生成的映像即可在 GCP VM 上直接掛載乾淨 volume 即時啟用。

### 本機（macOS / Linux）直接建置

若開發環境已具備 gcc/clang 與 make，也可以像傳統 Merc 一樣在主機上原生執行。針對
macOS（Apple Silicon）與大部分 Linux 發行版，可依下列步驟操作：

1. 安裝工具鏈  
   - macOS：執行 `xcode-select --install` 取得 clang 與 make，然後 `brew install csh`
     以提供 `./startup` 依賴的 C shell。  
   - Linux：`sudo apt install build-essential csh libxcrypt-compat`（或對應套件）。
2. 回到專案根目錄，先跑 `scripts/bootstrap.sh` 建立 `log/ player/ mail/` 等可寫目錄。
3. 進入 `src/`：`make clean && make`。
4. 編輯 `src/merc.ini` 或 `etc/merc.ini`，確保 `HOME DIRECTORY` 指向實際路徑（例如
   `/Users/jakeuj/auggie/mud2`），並確認 `MUD PORT` 不與現有服務衝突。
5. 啟動伺服器：

   ```bash
   cd src
   ./startup &
   ```

6. 本機測試可直接 `nc localhost 3838`（或任何 telnet 客戶端）登入，所有 runtime
   資料會寫回主機目錄而非容器。

更多細節與常見維運指令，請參考 [`docs/BUILD.md`](docs/BUILD.md) 的
「Host-only build」章節與 `document/README` 的傳統說明。
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
| `etc/` | 雜項設定與 `merc.ini` 模板 |
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

### Docker 部署的持久化掛載

官方 Docker 映像會在 `/app` 下寫入數個路徑，這些路徑已於 `docker/Dockerfile` 內宣告為 `VOLUME`，確保資料會被掛到具備持久性的 volume。若要把容器實際資料存到本機磁碟（或 GCP Persistent Disk），請：

> 注意：`/app/data/server` 與 `/app/data/immlist` 是**檔案**而非目錄，不屬於 `VOLUME` 宣告路徑。若要持久化這兩者，請使用檔案對檔案的 bind mount。
> 若 host 端檔案不存在且使用 `-v`，Docker 可能自動建立目錄，導致容器端型別錯誤；建議先 `touch` 檔案並使用 `--mount type=bind`。

1. 先建立對應目錄並複製預設內容（避免第一次掛載時是空的）：

   ```bash
   mkdir -p /srv/merc/{player,mail,board,vote,log,debug,etc,data}
   rsync -a board/ /srv/merc/board/
   rsync -a etc/ /srv/merc/etc/
   cp data/server /srv/merc/data/server
   cp data/immlist /srv/merc/data/immlist
   ```

2. 以 `docker run`（或 compose）掛載這些目錄：

   ```bash
   docker run --name merc -d --restart unless-stopped \
     -p 3838:3838 -p 1234:1234 -p 8888:8888 \
     -v /srv/merc/player:/app/player \
     -v /srv/merc/mail:/app/mail \
     -v /srv/merc/board:/app/board \
     -v /srv/merc/vote:/app/vote \
     -v /srv/merc/log:/app/log \
     -v /srv/merc/debug:/app/debug \
     -v /srv/merc/etc:/app/etc \
     --mount type=bind,src=/srv/merc/data/server,dst=/app/data/server \
     --mount type=bind,src=/srv/merc/data/immlist,dst=/app/data/immlist \
     -e MERC_HOME=/app \
     jakeuj/merc-fju-2.0-utf8:latest
   ```

3. 在 GCP 上只要把 `/srv/merc` 指向 Persistent Disk（或 Cloud Storage FUSE）即可複製相同做法，達到玩家檔案、信件、留言板與 `etc/` 內其他設定的持久化。
## 現代化重點

- **UTF-8 化**：所有遊戲內容、介面文字與資料表皆完成 Big5→UTF-8 轉換，並修正
  `fread_string`、`merc.ini` 解析器等舊版無法處理多位元字元的 bug。
- **Docker 工具鏈**：`docker/Dockerfile` 安裝 `build-essential`、`csh`、`libxcrypt-compat`
  等依賴，確保舊程式可在新 Linux 核心上編譯與執行。
- **scripts/**：`bootstrap.sh` 負責初始化可寫目錄，`render-merc-ini.sh` 會由 `etc/merc.ini` 模板生成 `src/merc.ini`；`check-data.py` 可驗證 `area/ skill/ angel/` 等資料是否仍為合法 UTF-8 與需有欄位。
- **etc/merc.ini**：現在是 git 追蹤的模板檔；Docker build 會固定生成 `HOME DIRECTORY=/app` 的 `src/merc.ini`，本機 host-only 開發則用當前 repo 絕對路徑生成。若需要舊版參數對照，可比對 `document/README` 或 `docs/merc.ini.snapshot`。
- **data/server**：可設定免除多重登入／DNS 查詢的工作站白名單；新版 Docker host
  會在此列出（例如 `192.168.65.x`）。

## 手動編譯（可選）

若仍想於主機直接建置，可遵循 `document/README` 的舊流程，再補上以下調整：

```bash
# Linux / macOS
cd src
make clean && make
```

確保系統已安裝 `gcc`、`make`、`csh`、`libxcrypt-compat`（Ubuntu）或等價套件；若要調整遊戲名稱、port 等固定設定，請先編輯 `etc/merc.ini` 模板，然後再生成本機使用的 `src/merc.ini`：

```bash
make render-merc-ini
# 或 make bootstrap
```

若 repo 路徑變更，可用 `MERC_FORCE_RENDER_INI=1 make bootstrap` 重新生成；若想覆寫目標路徑，也可執行 `MERC_HOME_VALUE=/your/path make render-merc-ini`。

啟動方式與舊版相同（`./startup` 會在 `log/` 內滾動紀錄，並於 `shutdown.txt` 出現時終止）：

```bash
cd src
./startup &
```

或在專案根目錄以 `./start-merc.sh` 一鍵啟動，腳本會：
1. 自動執行 `scripts/bootstrap.sh`（若存在）以建立 `log/`、`player/` 等目錄。
2. 檢查 `src/merc` / `src/merc.ini` 是否可用，必要時拒絕啟動並提示補救。
3. 移除殘留的 `src/shutdown.txt`，以免程式啟動瞬間自我關閉。
4. 若偵測不到 `src/merc` 會自動執行 `cd src && make` 嘗試編譯。
5. 將輸出寫入 `log/manual-start-YYYYmmdd-HHMMSS.log`，並在 `log/merc.pid` 保存 PID。

> macOS/本機除錯：若 `./startup` 因 `setpriority: Permission denied.` 等訊息被系統阻擋，可使用 `./start-merc.sh` 或直接在專案根目錄執行 `./src/merc src/merc.ini`。  
> 每次強制關閉後請刪除 `src/shutdown.txt`（或在遊戲內輸入 `shutdown`）再重新啟動，否則主程式會立刻偵測到舊的關機旗標而結束。
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
