現版 1-30 級導覽草案（P3-T2）

目的
- 提供現版可直接執行的 1-30 級路線，不依賴外部網站。
- 導引以「主城承接 + 指令先行」為核心，避免新手在混合等級區迷路。

共通起手指令（每級段都要會）
- `area`：先看目前可去區域
- `find <NPC>`：找訓練與轉職 NPC
- `train <trainer> <str|int|wis|dex|con>`：消耗訓練點升屬性
- `rebirth <target>`：完成轉職
- `recall set`：設定回城點

1-10 級：學宮起手與基礎試煉
- 推薦區域：`academy`、`drillground`（低段）
- 進出方式：
  - 新手起點 `500`
  - `north` 進 `academy`（教學）
  - 學宮內可銜接 `drillground` 試煉線
  - 若迷路可直接 `new` 回訓練區
- 本段目標：
  - 完成基本指令熟悉（移動/戰鬥/help）
  - 練到可離校進主城

10-20 級：長安承接與第一次轉職
- 推薦區域：`changan`（主城周邊）+ `drillground`（補練）
- 進出方式：
  - 從 `academy` 回 `500`，`down` 到 `10001`（長安）
  - 在 `10001` 先執行：
    1) `area`
    2) `find trainer` / `find shaman` / `find 小將`
    3) `train ...`
    4) `rebirth ...`（十級前後）
    5) `recall set`
- 本段目標：
  - 完成第一次轉職
  - 穩定把回城點設在常用主城

20-30 級：雙城承接（長安 ↔ 許昌）
- 推薦區域：`xuchang` + `changan` 指定節點
- 進出方式：
  - 先到長安車站（10086）
  - 用 `bus` 往返 `長安春水站 <-> 許昌驛站`
  - 在兩城之間挑選可負擔怪物區練功
- 本段目標：
  - 建立固定補給/訓練/練功循環
  - 確保每次外出前完成 `recall set`

風險與提醒
- `drillground` 為「新手試煉 + 對戰」混合區，不是純安全區。
- `changan`/`xuchang` 皆為混合等級城市，請先 `area` + `find` 再決定練點。

建議掛載位置
- 主入口：`help newbies`（保留）
- 詳細版：`help getstart` + 本草案轉寫為後續 help 章節
