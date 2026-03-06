# 跨界新世界地圖替換計畫

> 基於 `/Users/jakeuj/auggie/3yWebsite/map/index.html` 的 ASCII 地圖與 `newhand/newbies/index.html` 的新手指南內容，重新建構官方世界觀並逐步取代既有六大區域。

## 1. 現況盤點

| 區域 | 房間數 | 怪物 | 物品 | Reset 檔案 | 商店 | 主要職能 |
| --- | --- | --- | --- | --- | --- | --- |
| `loyang` | 238 | 106 | 113 | 1 | 11 | Recall、捷運起點、技能師傅、任務樞紐 |
| `beiping` | 63 | 19 | 27 | 1 | 1 | 北境練功、屬性師傅 |
| `new` | 28 | 7 | 2 | 1 | 1 | 新手教學、引導任務 |
| `newfight` | 98 | 43 | 1 | 2 | 0 | 新手練功、保護機制 |
| `pk_area` | 35 | 0 | 0 | 0 | 0 | PK 場、活動場地 |
| `free_fight` | 40 | 0 | 0 | 0 | 0 | 競技場、練習場 |

> 統計指令：`python3 scripts/tools/count_area_content.py`（見下方腳本段）。

### 1.1 影響資料節點
- `area/directory.lst`：新增 slug 需插入於 *新建區域* 區段，舊區移除前保持載入。
- `data/bus.txt`：永靖樞城為新公車起點，需追加白狼邊境與跨界城邦站點。
- `help/*.hlp`：`help/area`、`help/newbie`、`help/bus` 需更新敘述；新增 `help/new_world.hlp`、`help/quest_<slug>.hlp`。
- `scripts/check-data.py`：替換前確認新 slug 被列入檢查；可透過 CLI 參數限制範圍。
- `greeting/`、`help/3838.hlp`：公告新世界開放時程與暫時共存策略。

## 2. 等級－區域－功能矩陣

| 等級帶 | 對應區域 | 主要內容 | 技能／任務 | 裝備掉落 | 備註 |
| --- | --- | --- | --- | --- | --- |
| 1–5 | 雲梯書院 (`academy`) | 入門教學、互動課程 | 文官/武官/道士導師、逃跑/步法練習 | 新手布衣、初級法器 | 透過 job/new 直接進入 |
| 5–20 | 汜水演武林 (`rookie_field`) | 練功三支線、採集、劇情事件 | 山賊討伐、藥草採集、幻境塔演算法 | 初級武器、藥草 | 完成後解除安全保護 |
| 10–70 | 永靖樞城 (`yongjing`) | 主城、商店、任務公告、技能師傅 | 六名高級老師、外交任務、屬性訓練 | 各階裝備、技能書 | Recall、公交起點 |
| 20–60 (70) | 白狼邊境 (`bailang`) | 野外拓荒、副本入口、採集 | 商隊護送、秘境探索 | 稀有素材、精英掉落 | 含三個副本入口 |
| 50–100 | 裂界戰境 (`coliseum`) | PvE 層級挑戰、輪替 Boss | 每週挑戰、排行榜 | 高等裝備、徽章 | 需外交任務解鎖 |
| 30–100 | 星火決戰所 (`arena_prime`) | PvP 競技、觀戰賭場 | 排位賽、戰功兌換 | 榮譽貨幣、外觀 | 跟 `pk_area` / `free_fight` 對應 |
| 60+ | Stormwind / Orgrimmar | 跨界城邦、陣營任務 | 外交／軍務線 | 跨界製程 | 保留現存房間，新增傳送劇情 |

## 3. 模組規格與 VNUM 區段

| 模組 | slug | VNUM 範圍 | Serial | Capital | 目標規模 | 備註 |
| --- | --- | --- | --- | --- | --- | --- |
| 雲梯書院 | `academy` | 11001–11100 | 110 | 11001 | 40 rooms / 12 mobs / 3 shops | 新手劇情、NewHand |
| 汜水演武林 | `rookie_field` | 11101–11280 | 111 | 11101 | 80 rooms / 30 mobs / 2 迷宮 | 新手練功、NewHand |
| 永靖樞城 | `yongjing` | 11281–11520 | 150 | 11281 | 220 rooms / 70 mobs / 8 shops | 主城、Recall |
| 白狼邊境 | `bailang` | 11521–11680 | 190 | 11521 | 90 rooms / 25 mobs / 1 商隊 | 北境拓荒、副本入口 |
| 星火決戰所 | `arena_prime` | 11701–11780 | 210 | 11701 | 35 rooms / 動態 NPC | PK 場、觀眾席 |
| 裂界戰境 | `coliseum` | 11801–11880 | 211 | 11801 | 40 rooms / Boss 周期 | PvE 競技、副本 |

## 4. ASCII 世界草圖

```
             白狼邊境───╮────暴風外務館 (Stormwind)
                │        │
雲夢渡口──永靖樞城──星火決戰所──裂界戰境──奧格軍務處 (Orgrimmar)
    │            │
雲梯書院──汜水演武林
```

## 5. 任務與技能師傅配置
- **雲梯書院**：新手導師（文官/武官/道士）、逃跑教官、Enable 教練。
- **汜水演武林**：山賊首領（攻擊技能）、藥草學者（補給技能）、幻境守護者（步法/閃避）。
- **永靖樞城**：六名高級技能師傅（武技、法術、步法、逃跑、輔助、工藝），加上外交引導 NPC。
- **白狼邊境**：商隊隊長（護送任務）、秘境研究員（副本鑰匙）、信標工程師（Recall 擴充）。
- **星火決戰所**：報名官、賭場主持、戰況播報員。
- **裂界戰境**：活動協調者、輪替 Boss 向導、獎勵官員。

## 6. Reset／商店策略
- 以 `res/<slug>.res` 分段註解：守衛配置、商店、任務 NPC、特殊事件。
- 商店檔 `shp/`：
  - `academy`: 書院補給商、教材商。
  - `yongjing`: 武器、防具、藥品、技能書、外交兌換。
  - `bailang`: 商隊補給、素材收購。
  - `arena_prime`: 榮譽商人、下注 NPC。
  - `coliseum`: 徽章兌換、修裝服務。

## 7. Bus / 傳送整合建議
- `data/bus.txt` 追加：
  - 「永靖樞城總站」(11290)、
  - 「白狼邊境哨站」(11521)、
  - 「雲梯書院學區」(11001) ─ 表示內部穿梭馬車。
- 永靖樞城中央廣場設 `Job`：星火決戰所、裂界戰境、Stormwind/Orgrimmar 傳送。

## 8. QA 與漸進式下線
1. 建立新區域 → `python3 scripts/check-data.py --area <slug>`。
2. 在測試伺服器 `reload area <slug>`、`goto <Capital>` 巡房。
3. 完成所有模組後才從 `area/directory.lst` 移除舊區；舊資料移到 `area/archive/` 分支。
4. 更新 `help/area`、`help/newbie`、`help/bus`、`help/3838.hlp`，公告遷移時程。
5. 最終以 `git diff --stat area docs help data` 確認變動集中。

## 9. 補充腳本
```bash
python3 - <<'PY'
import os, json
areas = ['loyang','beiping','new','newfight','pk_area','free_fight']
base = 'area'
info = {}
for area in areas:
    path = os.path.join(base, area)
    def count(sub):
        target = os.path.join(path, sub)
        return sum(1 for f in os.scandir(target) if f.is_file()) if os.path.exists(target) else 0
    info[area] = {
        'rooms': count('roo'),
        'mobs': count('mob'),
        'objs': count('obj'),
        'res': count('res'),
        'shops': count('shp')
    }
print(json.dumps(info, ensure_ascii=False, indent=2))
PY
```

## 10. 未完成項目 / 待辦
- 依規劃填充 `mob/`, `obj/`, `res/`, `shp/` 內容並撰寫房間描述、互通出口。
- 更新 `data/bus.txt` 與 `help` 檔案，並設計 Stormwind/Orgrimmar 任務線。
- 建立副本入口 ASCII 圖（芒盪山洞、地下水系統、秘境樹海）。
- 將 `scripts/check-data.py` 改為可接受 `--slug` 參數，以縮短迭代時間。
