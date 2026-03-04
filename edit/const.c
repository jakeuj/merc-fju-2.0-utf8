#include "edit.h"

const   struct  const_string    const_table [] =
{

  /* 方向 */

  { "NORTH",                      0 },
  { "EAST",                       1 },
  { "SOUTH",                      2 },
  { "WEST",                       3 },
  { "UP",                         4 },
  { "DOWN",                       5 },

  /* 區域旗標 */

  { "NO_ECHO",                    0 },
  { "WILL_ECHO",                  1 },

  /* 房間旗標 */

  { "ROOM_DARK",                  1 },
  { "ROOM_NO_MOB",                2 },
  { "ROOM_INDOOR",                4 },
  { "ROOM_PRIVATE",               8 },
  { "ROOM_SAFE",                 16 },
  { "ROOM_FOREVER_LIGHT",        32 },
  { "ROOM_NO_RECALL",            64 },
  { "ROOM_DEPOSIT_MONEY",       128 },
  { "ROOM_STOREROOM",           256 },
  { "ROOM_NOFIGHT",             512 },
  { "ROOM_NOQUIT",             1024 },

  /* 地形旗標 */

  { "SECT_INSIDE",                0 },
  { "SECT_CITY",                  1 },
  { "SECT_FIELD",                 2 },
  { "SECT_FOREST",                3 },
  { "SECT_HILLS",                 4 },
  { "SECT_MOUNTAIN",              5 },
  { "SECT_WATER_SWIM",            6 },
  { "SECT_WATER_NOSWIM",          7 },
  { "SECT_AIR",                   8 },
  { "SECT_DESERT",                9 },

  /* 物品種類 */

  { "ITEM_LIGHT",                 1 },
  { "ITEM_SCROLL",                2 },
  { "ITEM_WAND",                  3 },
  { "ITEM_STAFF",                 4 },
  { "ITEM_WEAPON",                5 },
  { "ITEM_TREASURE",              6 },
  { "ITEM_ARMOR",                 7 },
  { "ITEM_POTION",                8 },
  { "ITEM_FURNITURE",             9 },
  { "ITEM_TRASH",                10 },
  { "ITEM_CONTAINER",            11 },
  { "ITEM_DRINK_CON",            12 },
  { "ITEM_KEY",                  13 },
  { "ITEM_FOOD",                 14 },
  { "ITEM_MONEY",                15 },
  { "ITEM_BOAT",                 16 },
  { "ITEM_CORPSE_NPC",           17 },
  { "ITEM_CORPSE_PC",            18 },
  { "ITEM_FOUNTAIN",             19 },
  { "ITEM_PILL",                 20 },

  /* 物品的屬性 */

  { "ITEM_GLOW",                  1 },
  { "ITEM_HUM",                   2 },
  { "ITEM_DARK",                  4 },
  { "ITEM_LOCK",                  8 },
  { "ITEM_EVIL",                 16 },
  { "ITEM_INVIS",                32 },
  { "ITEM_MAGIC",                64 },
  { "ITEM_NODROP",              128 },
  { "ITEM_BLESS",               256 },
  { "ITEM_ANTI_GOOD",           512 },
  { "ITEM_ANTI_EVIL",          1024 },
  { "ITEM_ANTI_NEUTRAL",       2048 },
  { "ITEM_NOREMOVE",           4096 },
  { "ITEM_INVENTORY",          8192 },

  /* 物品的穿戴旗標 */

  { "ITEM_NOT_TAKE",              0 },
  { "ITEM_TAKE",                  1 },
  { "ITEM_WEAR_FINGER",           2 },
  { "ITEM_WEAR_NECK",             4 },
  { "ITEM_WEAR_BODY",             8 },
  { "ITEM_WEAR_HEAD",            16 },
  { "ITEM_WEAR_LEGS",            32 },
  { "ITEM_WEAR_FEET",            64 },
  { "ITEM_WEAR_HANDS",          128 },
  { "ITEM_WEAR_ARMS",           256 },
  { "ITEM_WEAR_SHIELD",         512 },
  { "ITEM_WEAR_ABOUT",         1024 },
  { "ITEM_WEAR_WAIST",         2048 },
  { "ITEM_WEAR_WRIST",         4096 },
  { "ITEM_WIELD",              8192 },
  { "ITEM_HOLD",              16384 },

  /* 影響屬性的項目 */

  { "APPLY_NONE",                 0 },
  { "APPLY_STR",                  1 },
  { "APPLY_DEX",                  2 },
  { "APPLY_INT",                  3 },
  { "APPLY_WIS",                  4 },
  { "APPLY_CON",                  5 },
  { "APPLY_SEX",                  6 },
  { "APPLY_CLASS",                7 },
  { "APPLY_LEVEL",                8 },
  { "APPLY_AGE",                  9 },
  { "APPLY_HEIGHT",              10 },
  { "APPLY_WEIGHT",              11 },
  { "APPLY_MANA",                12 },
  { "APPLY_HIT",                 13 },
  { "APPLY_MOVE",                14 },
  { "APPLY_GOLD",                15 },
  { "APPLY_EXP",                 16 },
  { "APPLY_AC",                  17 },
  { "APPLY_HITROLL",             18 },
  { "APPLY_DAMROLL",             19 },
  { "APPLY_SAVING_PARA",         20 },
  { "APPLY_SAVING_ROD",          21 },
  { "APPLY_SAVING_PETRI",        22 },
  { "APPLY_SAVING_BREATH",       23 },
  { "APPLY_SAVING_SPELL",        24 },

  /* 最後的標記 */

  { ""                           , 0 }
};

const   struct  item_struct     item_type[ MAX_ITEM ] =
{

  { "燈籠"       , "ITEM_LIGHT"      , 1 },
  { "捲軸"       , "ITEM_SCROLL"     , 1 },
  { "法杖1"      , "ITEM_WAND"       , 1 },
  { "法杖2"      , "ITEM_STAFF"      , 1 },
  { "武器"       , "ITEM_WEAPON"     , 1 },
  { "寶物"       , "ITEM_TREASURE"   , 1 },
  { "護甲"       , "ITEM_ARMOR"      , 1 },
  { "藥水"       , "ITEM_POTION"     , 1 },
  { "家具"       , "ITEM_FURNITURE"  , 1 },
  { "垃圾"       , "ITEM_TRASH"      , 0 },
  { "容器"       , "ITEM_CONTAINER"  , 1 },
  { "水袋"       , "ITEM_DRINK_CON"  , 1 },
  { "鑰匙"       , "ITEM_KEY"        , 1 },
  { "食物"       , "ITEM_FOOD"       , 1 },
  { "錢"         , "ITEM_MONEY"      , 0 },
  { "船"         , "ITEM_BOAT"       , 0 },
  { "非玩家屍體" , "ITEM_CORPSE_NPC" , 0 },
  { "玩家屍體"   , "ITEM_CORPSE_PC"  , 0 },
  { "泉水"       , "ITEM_FOUNTAIN"   , 0 },
  { "藥丸"       , "ITEM_PILL"       , 1 }
};

