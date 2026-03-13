現版新手進階文件草案（P4-T2）
主題：屬性與訓練 / 轉職 / 交通與 recall

定位
- 這份文件給已離開學宮、在長安開始成長的新手。
- 範例全部使用現版可見 NPC 與指令：trainer / shaman / 小將 / 車站。

一、屬性與訓練（train）
1) 先找訓練 NPC
- 指令：`find trainer`
- 目的：先定位訓練師，避免在城內盲走。

2) 基本訓練格式
- `train <trainer> <str|int|wis|dex|con>`
- 範例：`train trainer con`

3) 新手建議
- 每次升級先把訓練點花掉，再決定是否外出練功。
- 若不確定加點，先補生存向屬性（如 con/dex），再補輸出向。

二、轉職（rebirth）
1) 轉職時機
- 十級前後優先完成第一次轉職。

2) 主城 NPC 範例
- 道士線：`find shaman` 後 `rebirth shaman`
- 武官線：`find 小將` 後 `rebirth 小將`

3) 執行順序建議
- `find` -> `rebirth` -> `score` / `learn` 檢查變化

三、交通（bus）與路線承接
1) 車站起手
- 先到長安車站（10086）
- 指令：`bus`

2) 20-30 級承接
- 使用 bus 往返：`長安春水站 <-> 許昌驛站`
- 用於建立「主城補給 -> 外出練功 -> 回城整備」循環

四、recall 與回城節奏
1) 出門前先設定
- 指令：`recall set`
- 目的：把回城點設在常用主城，迷路或危急時可快速回補。

2) 建議節奏
- 每次換主要練功據點時，重新確認 recall 設定是否仍符合目前路線。

五、一輪可直接照做的流程（長安版）
1. `area`
2. `find trainer` -> `train trainer <屬性>`
3. `find shaman` 或 `find 小將` -> `rebirth <target>`
4. `recall set`
5. `bus`（需要跨城時）

延伸閱讀
- `help newbies`
- `help getstart`
- `help newbiedoc`
- `help trainattr`
- `help classrebirth`
- `help travel`
