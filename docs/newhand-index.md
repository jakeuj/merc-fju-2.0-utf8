Newhand Reference Index
Summary
這份文件是從 `H:\repos\.agents\skills\sango-mud-website-reference\references\newhand-index.md`
鏡像進 repo 的版本，目的不是取代外部 skills，而是讓看不到 `H:\repos\.agents\skills`
的 Codex Web / AI coding agent 也能取得新手區、教學流程與玩家體驗的核心參考。

使用原則：

- 若任務牽涉新手流程、出生地、`Room School`、`#Keyword` 特殊互動、1-30 級引導或主城承接，先讀本檔。
- 本檔是索引與摘要，不是完整資料庫；需要細節時再開 `H:\repos\3yWebsite\newhand\...` 對應頁面。
- 若本檔與遊戲現況衝突，以現行 repo 實作為準，再把差異記回 `docs/plan-002.txt`。

Core References

- `H:\repos\3yWebsite\newhand\commands\index.html`
  - 指令總覽，適合確認玩家能用的正式指令名稱與分類。
- `H:\repos\3yWebsite\newhand\newbies\index.html`
  - 系統式新手說明，適合確認 `learn`、`enable`、`area`、`find`、`train`、`rebirth`、`recall` 的官方引導語氣。
- `H:\repos\3yWebsite\newhand\players\newplayer\index.html`
  - 玩家撰寫的新手文章入口，適合確認玩家實際感受到的路線與世界認知。
- `H:\repos\3yWebsite\newhand\rule\index.html`
  - 玩家規則與可接受行為，適合比對 alias、自動化與公共頻道規範。

High-Value New Player Articles

1. 路線與新手區主流程

- `H:\repos\3yWebsite\newhand\players\newplayer\9903151.html`
  - 舊版新手上路主線文章。
  - 重要價值：
    - 新手區不是只有入口房，而是有完整練功路線
    - `look hole -> bore hole` 是正式教學的一部分
    - 離開新手區後會進主城，再展開正式冒險

2. 創角與前期生存

- `H:\repos\3yWebsite\newhand\players\newplayer\0104101.html`
  - 創角流程、出生地選單、起手技能、前期生活指令。
  - 重要價值：
    - 可用來比對現版出生地選單是否合理
    - 可用來補強 `item`、`equipment`、`score`、`deposit`、`withdraw`、`rebirth` 等前期提示

3. 10-100 級升級動線

- `H:\repos\3yWebsite\newhand\players\newplayer\0104121.html`
  - 依等級帶列出主城與區域切換的升級建議。
  - 重要價值：
    - 可用來判斷現版 10-30 級是否出現承接斷層
    - 可用來規劃長安之後的下一步區域導引

4. 新手生存與屬性觀念

- `H:\repos\3yWebsite\newhand\players\newplayer\9907151.htm`
  - `enable`、`flee`、`area`、`find`、`recall`、裝備、拍賣與屬性建議。
  - 重要價值：
    - 反映玩家真正重視的生存提示
    - 適合轉寫成現版新手 help / 房間提示 / 看板文案

5. 類別示例：道士

- `H:\repos\3yWebsite\newhand\players\newplayer\9904101.html`
  - 道士屬性、訓練、技能與升級地點建議。
  - 重要價值：
    - 可當作類別導向新手文件的模板

6. alias 與操作便利性

- `H:\repos\3yWebsite\newhand\players\newplayer\9905191.html`
  - alias 使用經驗。
  - 重要價值：
    - 適合補為操作便利文件，而不是房間教學內容

7. realm / 國家系統新手承接

- `H:\repos\3yWebsite\newhand\players\newplayer\0104231.html`
  - 新手加入國家的動機、時機與基本指令。
  - 重要價值：
    - 適合規劃 15 級後或離開新手保護期後的社群導引

Quick Routing

- 如果任務是房間文案、隱藏指令、時空裂縫、教學動線：
  - 先看 `9903151.html`
- 如果任務是出生地、創角流程、起手技能、前期資源：
  - 先看 `0104101.html`
- 如果任務是 10-30 級主城承接與升級動線：
  - 先看 `0104121.html` 與 `9907151.htm`
- 如果任務是官方 help 或指令說明文字：
  - 先看 `commands/index.html` 與 `newbies/index.html`
- 如果任務是國家 / realm 引導：
  - 先看 `0104231.html`

What AI Should Extract

- 舊版新手區實際在教什麼，而不是只看房名
- 玩家離開學宮/新手區之後的第一個合理落點
- 哪些 `#Keyword` 原本預期對應真實特殊動作
- 哪些知識應該從房間教學延續到主城 help / NPC 提示
- 目前現版缺的是房間本體，還是承接與導引

Practical Conclusion

目前 repo 的判斷基準應是：

- `academy` / `drillground` 的骨架大致已承接舊版
- 真缺口主要在：
  - 新手練功區與 PvP 校場的定位
  - 離開教學區後的主城承接
  - 1-30 級導引
  - 把舊網站中的玩家知識轉成現版遊戲內可見說明
