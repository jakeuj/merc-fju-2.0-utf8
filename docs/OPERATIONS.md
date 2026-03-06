# Merc-FJU Operations Guide

本文件彙整日常啟動、部署與清理流程，讓 README 保持精簡。若需要更進階的建置流程，
可搭配 [`docs/BUILD.md`](BUILD.md)、[`docs/DATA_LAYOUT.md`](DATA_LAYOUT.md) 與
`document/README` 一起閱讀。

## Docker/容器流程

### 拉取與啟動
```bash
docker pull jakeuj/merc-fju-2.0-utf8:latest
docker run --name merc -d -p 3838:3838 -p 1234:1234 -p 8888:8888 jakeuj/merc-fju-2.0-utf8:latest
```

或在 repo 內使用 Makefile：

```
make docker-build   # 使用 docker/Dockerfile 建置映像
make docker-run     # 以 13838/11234/18888 → 3838/1234/8888 的靜態映射啟動
make docker-shell   # 進入容器偵錯
```

容器 entrypoint 會依序執行 `scripts/bootstrap.sh` → `make clean && make`
→ `src/startup merc.ini`。要進入容器可執行 `docker exec -it merc /bin/bash`。

### 映像清潔與重製
- `docker/Dockerfile` 在 `COPY . /app` 後立刻呼叫 `scripts/clean-runtime.sh`，會刪除
  `player/ mail/ log/ debug/ vote/` 等 runtime 目錄並重建空白結構，
  也會將 `data/immlist`、`etc/database`、`etc/address`、`etc/stock` 等檔案還原為模板。
- `.dockerignore` 排除 `.git/`、runtime、IDE 檔案，避免把開發者的玩家資料放進映像。
- 若要離線檢查某個 staging 目錄是否乾淨，可跑
  `bash scripts/clean-runtime.sh /path/to/stage`（請勿對正式環境目錄執行）。

### 資料持久化掛載
伺服器在 `/app/player`、`/app/mail`、`/app/board`、`/app/vote`、`/app/log`、`/app/debug`、
`/app/etc` 等路徑寫入資料，下例示範將它們掛到主機目錄：

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
  -v /srv/merc/data-server:/app/data/server \
  -v /srv/merc/immlist:/app/data/immlist \
  jakeuj/merc-fju-2.0-utf8:latest
```

若使用 GCP Persistent Disk，直接把 `/srv/merc` 指向該磁碟即可達到容器無狀態、資料持久化。

## 本機（macOS / Linux）流程

### 工具與編譯
1. 安裝工具鏈  
   - **macOS**：`xcode-select --install`；再 `brew install csh` 以提供啟動腳本需要的 C shell。  
   - **Linux**：`sudo apt install build-essential csh libxcrypt-compat`（或對應套件）。
2. 在專案根目錄跑 `scripts/bootstrap.sh`，建立 `log/ player/ mail/ debug/ vote/` 等可寫目錄。
3. 進入 `src/`：`make clean && make`。
4. 更新 `src/merc.ini`（或 `etc/merc.ini`），把 `HOME DIRECTORY` 設成實際路徑，並確認 `MUD PORT` 未被佔用。

### 啟動選項
- **傳統 csh 迴圈**  
  ```bash
  cd src
  ./startup merc.ini &
  ```
  會在 `log/1000.log` 起自動輪替，偵測到 `shutdown.txt` 時結束。

- **輕量啟動腳本**  
  ```bash
  ./start-merc.sh start
  ./start-merc.sh stop
  ./start-merc.sh restart
  ./start-merc.sh status
  ```
  `start-merc.sh` 會：
  1. 再次呼叫 `scripts/bootstrap.sh`（保險起見）。
  2. 如找不到 `src/merc` 會自動 `cd src && make`。
  3. 啟動前移除殘留 `src/shutdown.txt`，避免進程瞬間退出。
  4. 將輸出寫入 `log/manual-start-YYYYmmdd-HHMMSS.log`，並把 PID 存在 `log/merc.pid`。
  5. `stop` 會讀取 PID（或 fallback `pgrep`）並送出 `kill`，`status` 則回報 RUNNING / STOPPED。

macOS 若遇到 `./startup` 被 `setpriority: Permission denied` 阻擋，可使用 `./start-merc.sh`
或直接執行 `./src/merc src/merc.ini`。

## 常用支援腳本
- `scripts/bootstrap.sh` – 建立/修復 `log/ player/ mail/ debug/ vote/` 等目錄。
- `scripts/check-data.py` – 確認 `area/ skill/ angel/` 等資料仍為合法 UTF-8 並包含必要標記。
- `scripts/clean-runtime.sh` – 清空 runtime 目錄並還原必要模板；用於發佈映像或重置 staging。

更多資料結構請見 [`docs/DATA_LAYOUT.md`](DATA_LAYOUT.md)，GCP 部署建議請見
[`docs/gcp-deployment-plan.md`](gcp-deployment-plan.md)。
