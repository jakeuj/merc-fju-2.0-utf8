# 新世界地圖建置計畫指引

## 任務目標
- 依《跨界新世界地圖替換計畫》完成六大模組（雲梯書院、汜水演武林、永靖樞城、白狼邊境、星火決戰所、裂界戰境）並逐步取代 `loyang`、`beiping`、`new`、`newfight`、`pk_area`、`free_fight`。
- 保留 Stormwind/Orgrimmar 做為高等異界據點，與永靖樞城透過外交任務串連。
- 更新交通（bus/傳送）、help 文件與任務流程，確保 1–100 級玩家都有對應地圖與技能師傅。

## 目前進度（2026-03-06）
- ✅ 產出高層計畫：`docs/world-replacement.md` 整理 legacy 統計、VNUM 分配、ASCII 動線與 QA 流程。
- ✅ 完成六個新 slug 骨架（`area/<slug>/index`、子目錄與首間房間），並設定 Serial/Capital。
- ✅ 新增 bus 站點與房間：永靖驛站(11290)、雲梯內環(11010)、白狼驛站(11530)；`data/bus.txt` 已加入新站。
- ✅ 更新 Stormwind/Orgrimmar 描述，說明外交門戶。
- ✅ 新增幫助檔 `help/new_world.hlp`、`help/quest_academy.hlp`、`help/quest_yongjing.hlp`。
- ⏳ `mob/obj/res/shp/mineral` 僅包含 README placeholder，尚未填寫具體重置與 NPC。
- ⏳ 舊六區仍載入並提供功能；尚未建立 archive 或刪除流程。

## 下一步 TODO
1. **雲梯書院 (`academy`)**
   - 撰寫 40 間房間、12 個 NPC、3 間商店與訓練/enable 流程。
   - `quest_academy` 中提到的技能需在 `skill/` 或 NPC prog 實作。
2. **汜水演武林 (`rookie_field`)**
   - 規劃山賊營、藥草谷、幻境塔三條路線的房間/Reset；加入逃跑/步法訓練事件。
3. **永靖樞城 (`yongjing`)**
   - 完整主城地圖（目標 220 rooms）、八間商店、六位技能師、公交/傳送 job。
   - 更新 `data/bus.txt` job keyword (`job bus`?) 及 `help/bus` 說明。
4. **白狼邊境 (`bailang`)**
   - 設定商隊路線、副本入口（芒盪山洞、地下水系統、秘境樹海）；配置 25 mobs。
5. **星火決戰所 & 裂界戰境**
   - 設計對戰控制腳本、裁判 NPC、觀眾席；為 PvE 設置層級與獎勵物品。
6. **交通 / 文檔**
   - 更新 `help/bus.hlp`、`help/area`、`greeting/*.txt`，宣告雙版本共存。
   - 考慮為 `scripts/check-data.py` 新增 `--slug` 參數以縮短測試迴圈。
7. **舊區 Sunset**
   - 當所有新區完成時，把舊檔案移入 `area/archive/` 或保留 git tag，再從 `area/directory.lst` 移除。

## 給下一位 AI 的作業提示
```
你正在 /Users/jakeuj/auggie/mud2，延續三國 x WoW 跨界地圖重建。先閱讀 docs/world-replacement.md 與 docs/world-plan-progress.md，確認哪個 slug 的 TODO 最迫切。編輯任何 area/<slug> 子檔案時遵循 merc-area-builder 指南：保持 UTF-8、VNUM 對應、Reset/商店語法正確，並於完成後執行 python3 scripts/check-data.py。若新增交通或任務，記得同步 help/ 與 data/bus.txt。完成一項模組後更新本檔案的進度與下一步。
```
