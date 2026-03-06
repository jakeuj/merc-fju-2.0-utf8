# merc.ini 檔案行為表

以下整理的是 `merc.ini` 內「檔案路徑設定」在目前程式中的實際行為。

- 這裡的「會 error」是指：**開機時**可能因 `LOG_ERR`，或在 `boot_db()` 期間因 `LOG_DEBUG` 觸發關機。
- 所有 `/etc/...`、`/data/...`、`/debug/...` 路徑都會先加上 `HOME DIRECTORY`（見 `adjust_filename()`）。
- `f_open("r")` 內部會先 `stat()` 再 `mmap()`；因此對這套程式來說，**0 bytes 檔案通常等同於不能正常讀取**。
- `#END` 只對 `SITE FILE`、`STATION FILE`、`XNAME FILE` 這類列表型檔案有意義；其他檔案多半使用各自的 `End` / `#Section` 結構，或根本不是讀取型檔案。

| 設定鍵 | 不存在 / 0 bytes 會在開機直接 error | 沒有 `#END` 會直接 error | 目前行為 / 備註 |
| --- | --- | --- | --- |
| `SYMBOL FILE` | **會** | 不使用 `#END` | `load_symbol()` 仍是硬需求；缺檔或 0 bytes 會失敗。即使檔案存在，內容格式錯誤也可能在 boot 期關機。 |
| `SITE FILE` / `STATION FILE` / `XNAME FILE` | **不會** | **不會直接** | 目前已改成自動補建最小合法內容 `#END\n`。若檔案存在但內容格式錯，仍可能在 boot 期出錯；`XNAME FILE` 的錯誤旗標尤其會觸發 `LOG_DEBUG`。 |
| `WELCOME FILE` / `WELCOME IMMORTAL` | **不會** | 不使用 `#END` | 目前已改成自動補建一個換行。用途是畫面文字；缺檔或 0 bytes 不再是 fatal。 |
| `Hero File` / `Database File` / `STOCK FILE` / `QUEST FILE` / `QUESTION FILE` / `IMMLIST FILE` / `SALE FILE` / `SITUS FILE` / `BUS FILE` / `Donate File` / `Gift File` / `Date File` / `Bounty File` / `Event File` / `SHIP FILE` / `Promotion File` / `CLUB FILE` / `INTERNAL FILE` | **通常不會** | 不使用 `#END` | 這些 loader 大多是 `if ( (pFile = f_open(..., "r")) )`：缺檔或 0 bytes 會直接略過，不會因為「讀不到」立刻 fatal；但若檔案存在且內容格式損壞，仍可能在 boot 期因 parser 錯誤而關機。 |
| `IDEAS FILE` / `TYPO FILE` / `NEWPLAYER FILE` | **不會** | 不使用 `#END` | 不是開機必讀，主要在玩家操作時以 `FOPEN(..., "a")` 追加寫入；檔案不存在通常會在第一次寫入時建立。 |
| `CHECK FILE` / `WIZARD FILE` | **不會** | 不使用 `#END` | 非開機必讀；分別在檢查重複物品、神族操作紀錄時以 `a` 模式寫入。 |
| `ADDRESS FILE` | **不會** | 不使用 `#END` | 不是 boot 必讀；主要在神族執行 `address !save` 時以 `FOPEN(..., "w")` 重寫。 |
| `PURGE FILE` / `ERROR FILE` / `BUGS FILE` / `FAILLOAD FILE` / `FAILEXIT FILE` / `FAILPASS FILE` / `FAILENABLE FILE` / `WIZFLAGS FILE` / `BAD FILE` / `Bad Object File` / `Suspect File` / `XNAME LOG` / `CHAT LOG` / `Suicide Log` | **不會** | 不使用 `#END` | 這些都是 log / debug 類檔案，主要以 `FOPEN(..., "a")` 追加寫入；檔案不存在通常會在首次寫入時建立。 |

## 目錄設定：不存在是否會出錯

| 設定鍵 | 目錄不存在會在開機直接出錯 | 目前行為 / 備註 |
| --- | --- | --- |
| `Player Area` | **會** | 這是玩家區域資料目錄，不是玩家存檔目錄。boot 讀區域時若目錄不存在，會記 `玩家區域目錄 ... 無法存取`，屬於 `LOG_ERR`。 |
| `BOARD DIRECTORY` | **會** | `open_board_directory()` 會在 boot 期讀 `board.lst` 與各版面子目錄；根目錄不存在、子目錄不可存取、或缺 index/list 都會 `LOG_ERR`。 |
| `GREET DIRECTORY` | **會** | `load_greeting()` 在 boot 期直接讀取。目錄不存在會 `LOG_ERR`，而且至少必須載入一個 greeting 檔。 |
| `VOTE DIRECTORY` | **會** | `load_vote()` 缺目錄時記 `LOG_DEBUG`，但它發生在 `boot_db()` 期間；此時 `LOG_DEBUG` 也會導致關機。 |
| `JOKE DIRECTORY` | **會** | `load_joke()` 缺目錄時也是 `LOG_DEBUG`；因為在 boot 期執行，實務上仍視為開機失敗。 |
| `Note Directory` | **會** | `load_mail()` 缺目錄時直接 `LOG_ERR`。 |
| `SKILL DIRECTORY` | **會** | `load_skill()` 會先讀技能索引檔；目錄不存在時索引檔無法開啟，最後記 `Load_skill：沒有技能目錄列表 ...`。 |
| `INSTRUMENT DIRECTORY` | **會** | `load_instrument()` 會先讀指令索引檔；目錄不存在時會記 `Load_instrument：沒有命令目錄列表 ...`。 |
| `SECTOR DIRECTORY` | **會** | `load_sector()` 缺目錄時會 `LOG_ERR`；若因此沒有載到預設地形 `DefaultSector`，也會再報錯。 |
| `PLAYER DIRECTORY` | **通常不會** | 這是玩家存檔目錄，不是 boot 必讀。程式不會在開機先檢查它，但之後建立角色目錄、存檔、神族操作玩家檔時可能失敗。 |
| `MOBPROGS DIRECTORY` | **目前看起來不會** | 目前 repo 內看得到 ini 讀取與 `adjust_filename()` 處理，但暫時沒找到後續實際使用 `MOBProgs_dir` 的路徑。 |

## 補充說明

1. `SITE FILE` / `STATION FILE` / `XNAME FILE` 即使沒有 `#END`，只要讀到 EOF 也會停；所以「缺 `#END`」**不是必然 fatal**。但仍建議保留 `#END` 作為最小合法結尾。
2. `SYMBOL FILE` 是目前這份表裡唯一仍然需要你**事先準備好正確內容**的檔案。
3. log 類檔案雖然多半可自動建立，**父目錄仍然要存在**；例如 `/debug/...` 若整個目錄不存在，`fopen("a")` 還是會失敗。
4. 本次程式碼變更另外也把 `Motd File` 改成自動補建換行；它沒出現在上面這段 `merc.ini` 範圍內，但行為和 `WELCOME FILE` 類似。
5. `Player Area` 和 `PLAYER DIRECTORY` 雖然預設都常指向 `/player`，但兩者用途不同：前者是玩家區域資料，後者是玩家存檔目錄。
6. `BOARD DIRECTORY`、`SKILL DIRECTORY`、`INSTRUMENT DIRECTORY`、`Player Area` 這類設定，即使根目錄存在，若缺少對應的索引/列表檔，也一樣可能在 boot 期報錯。

## 主要依據

- `src/comm.c`：`read_ini()` → `default_file()` → `adjust_filename()` → `boot_db()`
- `src/db.c`：boot 時實際呼叫哪些 `load_*()`
- `src/load.c`：各資料檔的開檔方式與 parser 行為
- `src/fcntl.c`：`f_open()` 對 `stat()` / `mmap()` 的限制
- `src/buffer.c` / `src/check.c` / `src/socket.c`：各 log / runtime 檔案的 `FOPEN("a"/"w")`