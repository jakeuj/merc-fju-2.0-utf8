#include "edit.h"

void    show_room_shape                 ( ROOM_INFO *);
void    show_edit_room                  ( int  );
void    show_room_sector                ( ROOM_INFO * );
void    show_room_flag                  ( ROOM_INFO * );
void    set_room_default                ( ROOM_INFO * );
void    set_exit_default                ( EXIT_INFO * );
void    set_desc_default                ( ROOM_DESC * );
void    show_select_room_sector         ( int );
void    show_select_room_flag           ( int );
void    show_edit_exit                  ( void );
void    do_edit_exit                    ( EXIT_INFO * , int );
void    show_exit_to_room               ( ROOM_INFO * );
void    show_exit_bar                   ( int control );
void    show_exit_shape                 ( void );
void    print_exit_shape                ( int );
void    print_exit_bar                  ( int );
void    print_exit_lock_title           ( int );
void    print_exit_lock                 ( EXIT_INFO * );
void    show_desc_shape                 ( void );
void    show_desc_to_room               ( ROOM_INFO * );
void    show_desc_bar                   ( int );
void    do_edit_desc                    ( ROOM_DESC * , int );
void    print_desc_shape                ( int );
void    print_desc_bar                  ( int );
void    do_save_room                    ( ROOM_INFO * , FILE * );
int     load_room                       ( ROOM_INFO * );

static struct  const_string   sector_name [] =
{

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
  { ""           ,                0 }
};

static struct  const_string   room_flag_name [] =
{
  { "ROOM_DARK",                     1 },
  { "ROOM_NO_MOB",                   2 },
  { "ROOM_INDOOR",                   4 },
  { "ROOM_PRIVATE",                  8 },
  { "ROOM_SAFE",                    16 },
  { "ROOM_FOREVER_LIGHT",           32 },
  { "ROOM_NO_RECALL",               64 },
  { "ROOM_DEPOSIT_MONEY",          128 },
  { "ROOM_STOREROOM",              256 },
  { "ROOM_NOFIGHT",                512 },
  { "ROOM_NOQUIT",                1024 },
  { ""           ,                   0 }
};

static struct  const_string   direction_name [] =
{
  { "NORTH",                      0 },
  { "EAST",                       1 },
  { "SOUTH",                      2 },
  { "WEST",                       3 },
  { "UP",                         4 },
  { "DOWN",                       5 },
  { "",                           0 }
};

static struct  const_string    lock_status [] =
{

  { "EX_ISDOOR",                   0 },
  { "EX_CLOSED",                   1 },
  { "EX_LOCKED",                   2 },
  { "EX_PICKPROOF" ,               3 },
  { "" ,                           0 }
};

void show_room_shape( ROOM_INFO * pRoom )
{
  print_string( ROOM_X , ROOM_Y    , ROOM_COL ,"┌─────────────────────────────────────┐" );
  print_string( ROOM_X , ROOM_Y + 1, ROOM_COL ,"│檔案名稱:                                                                 │" );
  print_string( ROOM_X , ROOM_Y + 2, ROOM_COL ,"├─────────────────────────────────────┤" );
  print_string( ROOM_X , ROOM_Y + 3, ROOM_COL ,"│房間號碼: [     ]  房間名稱                                               │" );
  print_string( ROOM_X , ROOM_Y + 4, ROOM_COL ,"├─────────────────────────────────────┤" );
  print_string( ROOM_X , ROOM_Y + 5, ROOM_COL ,"│描述                                                                      │" );
  print_string( ROOM_X , ROOM_Y + 6, ROOM_COL ,"│                                                                          │" );
  print_string( ROOM_X , ROOM_Y + 7, ROOM_COL ,"│                                                                          │" );
  print_string( ROOM_X , ROOM_Y + 8, ROOM_COL ,"│                                                                          │" );
  print_string( ROOM_X , ROOM_Y + 9, ROOM_COL ,"│                                                                          │" );
  print_string( ROOM_X , ROOM_Y +10, ROOM_COL ,"│                                                                          │" );
  print_string( ROOM_X , ROOM_Y +11, ROOM_COL ,"│                                                                          │" );
  print_string( ROOM_X , ROOM_Y +12, ROOM_COL ,"│                                                                          │" );
  print_string( ROOM_X , ROOM_Y +13, ROOM_COL ,"├─────────────────────────────────────┤" );
  print_string( ROOM_X , ROOM_Y +14, ROOM_COL ,"│房間地形 [ ] 室內  [ ] 城市  [ ] 原野  [ ] 森林  [ ] 山丘  [ ] 山區       │" );
  print_string( ROOM_X , ROOM_Y +15, ROOM_COL ,"│         [ ] 水域不可游泳    [ ] 水域可以游泳    [ ] 天空  [ ] 沙漠       │" );
  print_string( ROOM_X , ROOM_Y +16, ROOM_COL ,"├─────────────────────────────────────┤" );
  print_string( ROOM_X , ROOM_Y +17, ROOM_COL ,"│房間旗標 [ ] 黑暗 [ ] 沒有MOB [ ] 室內 [ ] 私人 [ ] 安全 [ ] 不能傳送     │" );
  print_string( ROOM_X , ROOM_Y +18, ROOM_COL ,"│         [ ] 永明 [ ] 鏢局    [ ] 錢莊                                    │" );
  print_string( ROOM_X , ROOM_Y +19, ROOM_COL ,"├─────────────────────────────────────┤" );
  print_string( ROOM_X , ROOM_Y +20, ROOM_COL ,"│< 編輯出口> < 編輯額外描述 > < 存檔 > < 放棄 >                            │" );
  print_string( ROOM_X , ROOM_Y +21, ROOM_COL ,"└─────────────────────────────────────┘" );

  /* 列印檔名 */
  print_string( ROOM_X + 15 , ROOM_Y + 1 , ROOM_COL
    , pRoom->pFile->filename );

  /* 列印檔案的情形 */
  if ( pRoom->valid == 0 )
    print_string( ROOM_X + 60 , ROOM_Y + 1 , ROOM_COL , "舊檔案" );

  else if ( pRoom->valid == 1 )
    print_string( ROOM_X + 60 , ROOM_Y + 1 , ROOM_COL , "新檔案" );

  else if ( pRoom->valid == 2 )
    print_string( ROOM_X + 60 , ROOM_Y + 1 , ROOM_COL , "不正確的舊檔案" );

  return;
}

/* 顯示房間地形 */
void show_edit_room( int control )
{

  print_string( ROOM_X + 2 , ROOM_Y + 3
    ,( ( control == 1 ) ? ROOM_TAG : ROOM_COL ) , "房間號碼"     );

  print_string( ROOM_X + 21 , ROOM_Y + 3
    ,( ( control == 2 ) ? ROOM_TAG : ROOM_COL ) , "房間名稱"     );

  print_string( ROOM_X + 2 , ROOM_Y + 5
    ,( ( control == 3 ) ? ROOM_TAG : ROOM_COL ) , "描述"         );

  print_string( ROOM_X + 2 , ROOM_Y + 14
    ,( ( control == 4 ) ? ROOM_TAG : ROOM_COL ) , "房間地形"     );

  print_string( ROOM_X + 2 , ROOM_Y + 17
    ,( ( control == 5 ) ? ROOM_TAG : ROOM_COL ) , "房間旗標"     );

  print_string( ROOM_X + 4 , ROOM_Y + 20
    ,( ( control == 6 ) ? ROOM_TAG : ROOM_COL ) , "編輯出口"     );

  print_string( ROOM_X + 16, ROOM_Y + 20
    ,( ( control == 7 ) ? ROOM_TAG : ROOM_COL ) , "編輯額外描述" );

  print_string( ROOM_X + 33 , ROOM_Y + 20
    ,( ( control == 8 ) ? ROOM_TAG : ROOM_COL ) , "存檔"         );

  print_string( ROOM_X + 42 , ROOM_Y + 20
    ,( ( control == 9 ) ? ROOM_TAG : ROOM_COL ) , "放棄"         );

  return;

}

void show_room_sector( ROOM_INFO * pRoom )
{

  print_string( ROOM_X + 12 , ROOM_Y + 14 , ROOM_COL
    ,( ( pRoom->room_sector == 0 ) ? "*" : " " ) );

  print_string( ROOM_X + 22 , ROOM_Y + 14 , ROOM_COL
    ,( ( pRoom->room_sector == 1 ) ? "*" : " " ) );

  print_string( ROOM_X + 32 , ROOM_Y + 14 , ROOM_COL
    ,( ( pRoom->room_sector == 2 ) ? "*" : " " ) );

  print_string( ROOM_X + 42 , ROOM_Y + 14 , ROOM_COL
    ,( ( pRoom->room_sector == 3 ) ? "*" : " " ) );

  print_string( ROOM_X + 52 , ROOM_Y + 14 , ROOM_COL
    ,( ( pRoom->room_sector == 4 ) ? "*" : " " ) );

  print_string( ROOM_X + 62 , ROOM_Y + 14 , ROOM_COL
    ,( ( pRoom->room_sector == 5 ) ? "*" : " " ) );

  print_string( ROOM_X + 12 , ROOM_Y + 15 , ROOM_COL
    ,( ( pRoom->room_sector == 6 ) ? "*" : " " ) );

  print_string( ROOM_X + 32 , ROOM_Y + 15 , ROOM_COL
    ,( ( pRoom->room_sector == 7 ) ? "*" : " " ) );

  print_string( ROOM_X + 52 , ROOM_Y + 15 , ROOM_COL
    ,( ( pRoom->room_sector == 8 ) ? "*" : " " ) );

  print_string( ROOM_X + 62 , ROOM_Y + 15 , ROOM_COL
    ,( ( pRoom->room_sector == 9 ) ? "*" : " " ) );

  return;
}

void show_select_room_sector( int selection )
{

  print_string( ROOM_X + 15 , ROOM_Y + 14
    ,( ( selection == 0 ) ? ROOM_TAG : ROOM_COL ) , "室內");

  print_string( ROOM_X + 25 , ROOM_Y + 14
    ,( ( selection == 1 ) ? ROOM_TAG : ROOM_COL ) , "城市" );

  print_string( ROOM_X + 35 , ROOM_Y + 14
    ,( ( selection == 2 ) ? ROOM_TAG : ROOM_COL ) , "原野" );

  print_string( ROOM_X + 45 , ROOM_Y + 14
    ,( ( selection == 3 ) ? ROOM_TAG : ROOM_COL ) , "森林" );

  print_string( ROOM_X + 55 , ROOM_Y + 14
    ,( ( selection == 4 ) ? ROOM_TAG : ROOM_COL ) , "山丘" );

  print_string( ROOM_X + 65 , ROOM_Y + 14
    ,( ( selection == 5 ) ? ROOM_TAG : ROOM_COL ) , "山區" );

  print_string( ROOM_X + 15 , ROOM_Y + 15
    ,( ( selection == 6 ) ? ROOM_TAG : ROOM_COL ) , "水域不可游泳" );

  print_string( ROOM_X + 35 , ROOM_Y + 15
    ,( ( selection == 7 ) ? ROOM_TAG : ROOM_COL ) , "水域可以游泳" );

  print_string( ROOM_X + 55 , ROOM_Y + 15
    ,( ( selection == 8 ) ? ROOM_TAG : ROOM_COL ) , "天空" );

  print_string( ROOM_X + 65 , ROOM_Y + 15
    ,( ( selection == 9 ) ? ROOM_TAG : ROOM_COL ) , "沙漠" );

  return;
}

void show_room_flag( ROOM_INFO * pRoom )
{

  print_string( ROOM_X + 12 , ROOM_Y + 17 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_DARK ) )          ? "*" : " " ) );

  print_string( ROOM_X + 21 , ROOM_Y + 17 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_NO_MOB ) )        ? "*" : " " ) );

  print_string( ROOM_X + 33 , ROOM_Y + 17 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_INDOOR  ) )       ? "*" : " " ) );

  print_string( ROOM_X + 42 , ROOM_Y + 17 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_PRIVATE ) )       ? "*" : " " ) );

  print_string( ROOM_X + 51 , ROOM_Y + 17 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_SAFE ) )          ? "*" : " " ) );

  print_string( ROOM_X + 60 , ROOM_Y + 17 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_FOREVER_LIGHT ) ) ? "*" : " " ) );

  print_string( ROOM_X + 12 , ROOM_Y + 18 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_NO_RECALL ) )     ? "*" : " " ) );

  print_string( ROOM_X + 21 , ROOM_Y + 18 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_DEPOSIT_MONEY ) ) ? "*" : " " ) );

  print_string( ROOM_X + 33 , ROOM_Y + 18 , ROOM_COL
    , ( ( IS_SET( pRoom->room_flag , ROOM_STOREROOM ) )     ? "*" : " " ) );

  return;
}

void show_select_room_flag( int selection )
{

  print_string( ROOM_X + 15 , ROOM_Y + 17
    ,( ( selection == 0 ) ? ROOM_TAG : ROOM_COL ) , "黑暗" );

  print_string( ROOM_X + 24 , ROOM_Y + 17
    ,( ( selection == 1 ) ? ROOM_TAG : ROOM_COL ) , "沒有MOB" );

  print_string( ROOM_X + 36 , ROOM_Y + 17
    ,( ( selection == 2 ) ? ROOM_TAG : ROOM_COL ) , "室內" );

  print_string( ROOM_X + 45 , ROOM_Y + 17
    ,( ( selection == 3 ) ? ROOM_TAG : ROOM_COL ) , "私人" );

  print_string( ROOM_X + 54 , ROOM_Y + 17
    ,( ( selection == 4 ) ? ROOM_TAG : ROOM_COL ) , "安全" );

  print_string( ROOM_X + 63 , ROOM_Y + 17
    ,( ( selection == 5 ) ? ROOM_TAG : ROOM_COL ) , "不能傳送" );

  print_string( ROOM_X + 15 , ROOM_Y + 18
    ,( ( selection == 6 ) ? ROOM_TAG : ROOM_COL ) , "永明" );

  print_string( ROOM_X + 24 , ROOM_Y + 18
    ,( ( selection == 7 ) ? ROOM_TAG : ROOM_COL ) , "鏢局" );

  print_string( ROOM_X + 36 , ROOM_Y + 18
    ,( ( selection == 8 ) ? ROOM_TAG : ROOM_COL ) , "錢莊" );

  return;
}


void set_room_default( ROOM_INFO * pRoom )
{

  int   loop;

  pRoom->vnum        = 0;
  pRoom->room_sector = 0;
  pRoom->room_flag   = 0;

  for ( loop = 0; loop < 50   ; loop++ ) pRoom->name[ loop ]  = '\x0';
  for ( loop = 0; loop < 1000 ; loop++ ) pRoom->description [ loop ] = '\x0';

  for ( loop = 0; loop < 6; loop++ ) set_exit_default( pRoom->exit[loop] );
  for ( loop = 0; loop < 5; loop++ ) set_desc_default( pRoom->desc[loop] );

  return;
}

void set_exit_default( EXIT_INFO * pExit )
{
  int   loop;

  pExit->room   = 0;
  pExit->key    = 0;
  pExit->lock   = 0;

  for ( loop = 0; loop < 80; loop++ ) pExit->keyword[loop]     = '\x0';
  for ( loop = 0; loop < 80; loop++ ) pExit->description[loop] = '\x0';

  return;

}

void set_desc_default( ROOM_DESC * pDesc )
{
  int   loop;

  for ( loop = 0; loop < 50  ; loop++ ) pDesc->keyword[loop ]    = '\x0';
  for ( loop = 0; loop < 1000; loop++ ) pDesc->description[loop] = '\x0';
  return;
}

void do_edit_room( void )
{
  FILE_INFO *   pFile;
  int           control = 1;
  int           execute = 0;
  int           control_key;

  /* 清除螢幕以及消除游標 */
  clear_screen();
  set_cursor_type( NO_CURSOR );

  /* 開啟檔案 */
  pFile = get_file_name( ".roo" );

  /* 檔案無法讀取 */
  if ( pFile->information < 0 ) return;

  /* 設定房間的預設值 */
  set_room_default( pRoom );
  pRoom->valid        = pFile->information;
  pRoom->pFile        = pFile;

  pRoom->pVnum->save_x_cursor   = 0;
  pRoom->pVnum->save_y_cursor   = 0;
  pRoom->pName->save_x_cursor   = 0;
  pRoom->pName->save_y_cursor   = 0;
  pRoom->pDesc->save_x_cursor   = 0;
  pRoom->pDesc->save_y_cursor   = 0;

  /* 載入房間檔案 */
  if ( pFile->information == 0 )
  {
    if ( !load_room( pRoom ) )
    {
      pRoom->valid = 2;
      set_room_default( pRoom );
    }
  }

  /* 顯示編輯房間的畫面 */
  show_room_shape( pRoom );

  /* 顯示房間地形 */
  show_room_sector( pRoom );
  show_room_flag( pRoom );

  /* 列印房間的變數 */
  print_long_string( 1 , pRoom->pName );
  print_long_string( 1 , pRoom->pDesc );
  print_number( 5 , pRoom->vnum , pRoom->pVnum );

  do
  {

    /* 顯示控制光棒 */
    show_edit_room( control );

    /* 從鍵盤得取控制鍵 */
    control_key = get_control_key();

    if ( control_key == RETURN_TAB    ) control++;
    if ( control_key == RETURN_UP     ) control--;
    if ( control_key == RETURN_DOWN   ) control++;
    if ( control_key == RETURN_RETURN ) execute = control;
    if ( control_key == RETURN_SPACE  ) execute = control;
    if ( control_key == RETURN_HOME   ) control = 1;
    if ( control_key == RETURN_END    ) control = 9;

    /* 檢查是否超過範圍 */
    check_boundary( 1 , 9 , &control );

    switch ( execute )
    {

      case 1 :

        /* 設定房間號碼字串起啟值 */
        pRoom->pVnum->save_x_cursor   = 0;
        pRoom->pVnum->save_y_cursor   = 0;
        clear_string( pRoom->pVnum );
        number_to_string( pRoom->vnum , pRoom->pVnum->address );

        get_string( pRoom->pVnum );
        pRoom->vnum = atoi( pRoom->pVnum->address );
        if ( pRoom->vnum < 0 ) pRoom->vnum = 0;
        print_number( 5 , pRoom->vnum , pRoom->pVnum );

        execute = 0;
        break;

      case 2 :

        get_string( pRoom->pName );
        execute = 0;
        break;

      case 3 :

        get_string( pRoom->pDesc );
        execute = 0;
        break;

      case 4 :

        {
          int touch;

          do
          {
            /* 顯示房間地形 */

            show_room_sector( pRoom );
            show_select_room_sector( pRoom->room_sector );

            touch = get_control_key();

            if ( touch == RETURN_RIGHT  ) pRoom->room_sector++;
            if ( touch == RETURN_LEFT   ) pRoom->room_sector--;
            if ( touch == RETURN_HOME   ) pRoom->room_sector = 0;
            if ( touch == RETURN_END    ) pRoom->room_sector = 9;

            check_boundary( 0 , 9 , &(pRoom->room_sector ) );

          } while ( touch != RETURN_RETURN
                 && touch != RETURN_TAB
                 && touch != RETURN_SPACE );
        }

        show_select_room_sector( 10 );
        execute = 0;
        break;

      case 5 :

        {
          int touch;
          int control = 0;

          do
          {

            show_room_flag( pRoom );
            show_select_room_flag( control );
            touch = get_control_key();

            if ( touch == RETURN_RIGHT  ) control++;
            if ( touch == RETURN_LEFT   ) control--;
            if ( touch == RETURN_HOME   ) control = 0;
            if ( touch == RETURN_END    ) control = 8;

            if ( touch == RETURN_SPACE )
              pRoom->room_flag ^= room_flag_name[control].value;

            check_boundary( 0 , 8 , &control );

          } while ( touch != RETURN_RETURN  && touch != RETURN_TAB );

        }

        show_select_room_flag( 9 );
        execute = 0;
        break;

      case 6 :

        {

          int   control = 1;
          int   execute = 0;
          int   touch;

          clear_screen();
          show_exit_shape();

          do
          {

            show_exit_to_room( pRoom );
            show_exit_bar( control );

            touch = get_control_key();

            if ( touch == RETURN_RIGHT  ) control++;
            if ( touch == RETURN_LEFT   ) control--;
            if ( touch == RETURN_TAB    ) control++;
            if ( touch == RETURN_UP     ) control--;
            if ( touch == RETURN_DOWN   ) control++;
            if ( touch == RETURN_HOME   ) control = 1;
            if ( touch == RETURN_END    ) control = 7;
            if ( touch == RETURN_RETURN ) execute = control;

            check_boundary( 1 , 7 , &control );

            if ( execute != 7 && execute != 0 )
            {
              show_exit_bar( 8 );
              do_edit_exit( pRoom->exit[execute -1 ] , execute -1  );
              execute = 0;
            }

          } while ( execute != 7 );

        }

        /* 顯示房間地形 */
        clear_screen();
        show_room_shape( pRoom );
        show_room_sector( pRoom );
        show_room_flag( pRoom );

        /* 列印房間的變數 */
        print_long_string( 1 , pRoom->pName );
        print_long_string( 1 , pRoom->pDesc );
        print_number( 5 , pRoom->vnum , pRoom->pVnum );

        execute = 0;
        break;

      case 7 :

        {

          int   control = 1;
          int   execute = 0;
          int   touch;

          clear_screen();
          show_desc_shape();

          do
          {

            show_desc_to_room( pRoom );
            show_desc_bar( control );

            touch = get_control_key();

            if ( touch == RETURN_RIGHT  ) control++;
            if ( touch == RETURN_LEFT   ) control--;
            if ( touch == RETURN_TAB    ) control++;
            if ( touch == RETURN_UP     ) control--;
            if ( touch == RETURN_DOWN   ) control++;
            if ( touch == RETURN_HOME   ) control = 1;
            if ( touch == RETURN_END    ) control = 6;
            if ( touch == RETURN_RETURN ) execute = control;

            check_boundary( 1 , 6 , &control );

            if ( execute != 6 && execute != 0 )
            {
              show_desc_bar( 7 );
              do_edit_desc( pRoom->desc[execute -1 ] , execute -1  );
              execute = 0;
            }

          } while ( execute != 6 );

        }

        /* 顯示房間地形 */
        clear_screen();
        show_room_shape( pRoom );
        show_room_sector( pRoom );
        show_room_flag( pRoom );

        /* 列印房間的變數 */
        print_long_string( 1 , pRoom->pName );
        print_long_string( 1 , pRoom->pDesc );
        print_number( 5 , pRoom->vnum , pRoom->pVnum );

        execute = 0;
        break;

      case 8 :
        fclose( pFile->filehandler );
        pFile->filehandler = fopen ( pFile->filename ,"w+" );

        do_save_room( pRoom , pFile->filehandler );
        fclose( pFile->filehandler );
        return;

      case 9 :

        if ( pFile->information == 1 ) delete_null_file( pFile );
        else fclose( pFile->filehandler );

    }

  } while ( execute != 9 );

  return;
}

void show_exit_shape( void )
{

  print_string( EXIT_X , EXIT_Y    , EXIT_COL ,"┌─────────────────────────────────────┐ " );
  print_string( EXIT_X , EXIT_Y + 1, EXIT_COL ,"│出口 北[     ] 東[     ] 南[     ] 西[     ] 上[     ] 下[     ] < 結束 > │ " );
  print_string( EXIT_X , EXIT_Y + 2, EXIT_COL ,"└─────────────────────────────────────┘ " );
  print_string( EXIT_X , EXIT_Y + 3, EXIT_COL ,"                                                                               " );
  print_string( EXIT_X , EXIT_Y + 4, EXIT_COL ,"                                                                               " );
  print_string( EXIT_X , EXIT_Y + 5, EXIT_COL ,"┌─────────────────────────────────────┐ " );
  print_string( EXIT_X , EXIT_Y + 6, EXIT_COL ,"│出口 [  ] 通往房間 [     ]                                                │ " );
  print_string( EXIT_X , EXIT_Y + 7, EXIT_COL ,"├─────────────────────────────────────┤ " );
  print_string( EXIT_X , EXIT_Y + 8, EXIT_COL ,"│門的狀態 [ ] 沒有門 [ ] 門是關上的 [ ] 鎖上 [ ] 鎖上且無法撬開的          │ " );
  print_string( EXIT_X , EXIT_Y + 9, EXIT_COL ,"│鑰匙號碼 [     ]                                                          │ " );
  print_string( EXIT_X , EXIT_Y +10, EXIT_COL ,"├─────────────────────────────────────┤ " );
  print_string( EXIT_X , EXIT_Y +11, EXIT_COL ,"│關鍵字                                                                    │ " );
  print_string( EXIT_X , EXIT_Y +12, EXIT_COL ,"├─────────────────────────────────────┤ " );
  print_string( EXIT_X , EXIT_Y +13, EXIT_COL ,"│描述                                                                      │ " );
  print_string( EXIT_X , EXIT_Y +14, EXIT_COL ,"├─────────────────────────────────────┤ " );
  print_string( EXIT_X , EXIT_Y +15, EXIT_COL ,"│< 結束 >                                                                  │ " );
  print_string( EXIT_X , EXIT_Y +16, EXIT_COL ,"└─────────────────────────────────────┘ " );

  return;
}

void print_exit_shape( int direction )
{

  char * const dir_name [] = { "北" ,"東" , "南" , "西" , "上" , "下" };

  print_string( EXIT_X , EXIT_Y + 5, EXIT_COL ,"┌─────────────────────────────────────┐ " );
  print_string( EXIT_X , EXIT_Y + 6, EXIT_COL ,"│出口 [  ] 通往房間 [     ]                                                │ " );
  print_string( EXIT_X , EXIT_Y + 7, EXIT_COL ,"├─────────────────────────────────────┤ " );
  print_string( EXIT_X , EXIT_Y + 8, EXIT_COL ,"│門的狀態 [ ] 沒有門 [ ] 門是關上的 [ ] 鎖上 [ ] 鎖上且無法撬開的          │ " );
  print_string( EXIT_X , EXIT_Y + 9, EXIT_COL ,"│鑰匙號碼 [     ]                                                          │ " );
  print_string( EXIT_X , EXIT_Y +10, EXIT_COL ,"├─────────────────────────────────────┤ " );
  print_string( EXIT_X , EXIT_Y +11, EXIT_COL ,"│關鍵字                                                                    │ " );
  print_string( EXIT_X , EXIT_Y +12, EXIT_COL ,"├─────────────────────────────────────┤ " );
  print_string( EXIT_X , EXIT_Y +13, EXIT_COL ,"│描述                                                                      │ " );
  print_string( EXIT_X , EXIT_Y +14, EXIT_COL ,"├─────────────────────────────────────┤ " );
  print_string( EXIT_X , EXIT_Y +15, EXIT_COL ,"│< 結束 >                                                                  │ " );
  print_string( EXIT_X , EXIT_Y +16, EXIT_COL ,"└─────────────────────────────────────┘ " );

  if ( direction >= 0 && direction < 6 )
    print_string( EXIT_X + 8  , EXIT_Y + 6, EXIT_COL
      , dir_name[direction] );

  return;
}

void show_exit_to_room( ROOM_INFO * pRoom )
{
  int   i;
  int   j;
  char  temp[10];

  for ( i = 0; i < 6 ; i++ )
  {

     for ( j = 0; j < 10 ; j++ ) temp[j] = '\x0';

     if ( pRoom->exit[i]->room > 0 )
     {
       number_to_string( pRoom->exit[i]->room  , temp );
       align_word( temp , 5 );
     }

     else
     {
       strcpy( temp , " 從缺" );
     }

     print_string( EXIT_X + 10 + ( i * 10 ) , EXIT_Y + 1
       , EXIT_COL , temp );
  }
}

void show_exit_bar( int control )
{
  print_string( EXIT_X +  7 , EXIT_Y + 1
    ,( ( control == 1 ) ? EXIT_TAG : EXIT_COL ) , "北" );

  print_string( EXIT_X + 17 , EXIT_Y + 1
    ,( ( control == 2 ) ? EXIT_TAG : EXIT_COL ) , "東" );

  print_string( EXIT_X + 27 , EXIT_Y + 1
    ,( ( control == 3 ) ? EXIT_TAG : EXIT_COL ) , "南" );

  print_string( EXIT_X + 37 , EXIT_Y + 1
    ,( ( control == 4 ) ? EXIT_TAG : EXIT_COL ) , "西" );

  print_string( EXIT_X + 47 , EXIT_Y + 1
    ,( ( control == 5 ) ? EXIT_TAG : EXIT_COL ) , "上" );

  print_string( EXIT_X + 57 , EXIT_Y + 1
    ,( ( control == 6 ) ? EXIT_TAG : EXIT_COL ) , "下" );

  print_string( EXIT_X + 69 , EXIT_Y + 1
    ,( ( control == 7 ) ? EXIT_TAG : EXIT_COL ) , "結束" );

  return;
}

void print_exit_bar( int control )
{
  print_string( EXIT_X +  12 , EXIT_Y + 6
    ,( ( control == 1 ) ? EXIT_TAG : EXIT_COL ) , "通往房間" );

  print_string( EXIT_X +  2 , EXIT_Y + 8
    ,( ( control == 2 ) ? EXIT_TAG : EXIT_COL ) , "門的狀態" );

  print_string( EXIT_X +  2 , EXIT_Y + 9
    ,( ( control == 3 ) ? EXIT_TAG : EXIT_COL ) , "鑰匙號碼" );

  print_string( EXIT_X +  2 , EXIT_Y + 11
    ,( ( control == 4 ) ? EXIT_TAG : EXIT_COL ) , "關鍵字"   );

  print_string( EXIT_X +  2 , EXIT_Y + 13
    ,( ( control == 5 ) ? EXIT_TAG : EXIT_COL ) , "描述"     );

  print_string( EXIT_X +  4 , EXIT_Y + 15
    ,( ( control == 6 ) ? EXIT_TAG : EXIT_COL ) , "結束"     );

  return;
}


void    print_exit_lock_title( int control )
{

  print_string( EXIT_X + 15 , EXIT_Y + 8
    ,( ( control == 0 ) ? EXIT_TAG : EXIT_COL )
    , "沒有門"     );

  print_string( EXIT_X + 26 , EXIT_Y + 8
    ,( ( control == 1 ) ? EXIT_TAG : EXIT_COL )
    , "門是關上的" );

  print_string( EXIT_X + 41 , EXIT_Y + 8
    ,( ( control == 2 ) ? EXIT_TAG : EXIT_COL )
    , "鎖上"       );

  print_string( EXIT_X + 50 , EXIT_Y + 8
    ,( ( control == 3 ) ? EXIT_TAG : EXIT_COL )
    , "鎖上且無法撬開的" );

  return;
}

void    print_exit_lock( EXIT_INFO * pExit )
{

  print_string( EXIT_X + 12 , EXIT_Y + 8 , EXIT_COL
    , ( ( pExit->lock == 0 ) ? "*" : " " ) );

  print_string( EXIT_X + 23 , EXIT_Y + 8 , EXIT_COL
    , ( ( pExit->lock == 1 ) ? "*" : " " ) );

  print_string( EXIT_X + 38 , EXIT_Y + 8 , EXIT_COL
    , ( ( pExit->lock == 2 ) ? "*" : " " ) );

  print_string( EXIT_X + 47 , EXIT_Y + 8 , EXIT_COL
    , ( ( pExit->lock == 3 ) ? "*" : " " ) );

  return;
}

void do_edit_exit( EXIT_INFO * pExit , int direction )
{

  int           control = 1;
  int           execute = 0;
  int           touch;

  /* 設定房間號碼字串起啟值 */
  pExit->pRoom->save_x_cursor   = 0;
  pExit->pRoom->save_y_cursor   = 0;

  /* 設定房間鑰匙號碼字串起啟值 */
  pExit->pKey->save_x_cursor   = 0;
  pExit->pKey->save_y_cursor   = 0;

  /* 設定房間出口關鍵字字串起啟值 */
  pExit->pKeyword->save_x_cursor   = 0;
  pExit->pKeyword->save_y_cursor   = 0;

  /* 設定房間出口描述字字串起啟值 */
  pExit->pDesc->save_x_cursor   = 0;
  pExit->pDesc->save_y_cursor   = 0;

  print_exit_shape( direction );
  print_exit_lock( pExit );

  print_number( 5 , pExit->room , pExit->pRoom );
  print_number( 5 , pExit->key  , pExit->pKey  );
  print_long_string( 1 , pExit->pKeyword );
  print_long_string( 1 , pExit->pDesc    );

  do
  {

    print_exit_bar( control );
    touch = get_control_key();

    if ( touch == RETURN_RIGHT  ) control++;
    if ( touch == RETURN_LEFT   ) control--;
    if ( touch == RETURN_TAB    ) control++;
    if ( touch == RETURN_UP     ) control--;
    if ( touch == RETURN_DOWN   ) control++;
    if ( touch == RETURN_HOME   ) control = 1;
    if ( touch == RETURN_END    ) control = 6;
    if ( touch == RETURN_RETURN ) execute = control;

    check_boundary( 1 , 6 , &control );

    switch ( execute )
    {
      case 1 :

        pExit->pRoom->save_x_cursor   = 0;
        pExit->pRoom->save_y_cursor   = 0;
        clear_string( pExit->pRoom );
        number_to_string( pExit->room , pExit->pRoom->address );

        get_string( pExit->pRoom );
        pExit->room = atoi( pExit->pRoom->address );
        if ( pExit->room < 0 ) pExit->room = 0;

        number_to_string( pExit->room , pExit->pRoom->address );
        align_word( pExit->pRoom->address , 5 );
        print_string( EXIT_X + 22 , EXIT_Y + 6 , EXIT_COL
          , pExit->pRoom->address );

        execute = 0;
        break;

      case 2 :

        {
          int touch;

          do
          {

            print_exit_lock_title( pExit->lock );
            print_exit_lock( pExit );

            touch = get_control_key();

            if ( touch == RETURN_RIGHT  ) pExit->lock++;
            if ( touch == RETURN_LEFT   ) pExit->lock--;
            if ( touch == RETURN_TAB    ) pExit->lock++;
            if ( touch == RETURN_HOME   ) pExit->lock = 0;
            if ( touch == RETURN_END    ) pExit->lock = 3;

            check_boundary( 0 , 3 , &pExit->lock );

          } while ( touch != RETURN_RETURN );
        }

        if ( pExit->lock == 0 || pExit->lock == 1 )
          print_number( 5 , ( pExit->key = 0 ) , pExit->pKey  );

        print_exit_lock_title( 4 );
        execute = 0;
        break;

      case 3 :

        if ( pExit->lock == 2 || pExit->lock == 3 )
        {

          pExit->pKey->save_x_cursor   = 0;
          pExit->pKey->save_y_cursor   = 0;
          clear_string( pExit->pKey );
          number_to_string( pExit->key , pExit->pKey->address );

          get_string( pExit->pKey );
          pExit->key = atoi( pExit->pKey->address );
          if ( pExit->key < 0 ) pExit->key = 0;

          number_to_string( pExit->key , pExit->pKey->address );
          align_word( pExit->pKey->address , 5 );
          print_string( EXIT_X + 12 , EXIT_Y + 9 , EXIT_COL
            , pExit->pKey->address );

        }

        execute = 0;
        break;

      case 4 :

        get_string( pExit->pKeyword );
        execute = 0;
        break;

      case 5 :

        get_string( pExit->pDesc );
        execute = 0;
        break;
    }

  } while ( execute != 6 );

  print_exit_shape( 7 );
  return;
}

void show_desc_shape( void )
{

  print_string( DESC_X , DESC_Y    , DESC_COL ,"┌─────────────────────────────────────┐" );
  print_string( DESC_X , DESC_Y + 1, DESC_COL ,"│ [ ] 描述一  [ ] 描述二  [ ] 描述三  [ ] 描述四  [ ] 描述五  < 結束 >     │" );
  print_string( DESC_X , DESC_Y + 2, DESC_COL ,"└─────────────────────────────────────┘" );
  print_string( DESC_X , DESC_Y + 3, DESC_COL ,"                                                                              " );
  print_string( DESC_X , DESC_Y + 4, DESC_COL ,"┌─────────────────────────────────────┐" );
  print_string( DESC_X , DESC_Y + 5, DESC_COL ,"│關鍵字                                                                    │" );
  print_string( DESC_X , DESC_Y + 6, DESC_COL ,"├─────────────────────────────────────┤" );
  print_string( DESC_X , DESC_Y + 7, DESC_COL ,"│描述                                                                      │" );
  print_string( DESC_X , DESC_Y + 8, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y + 9, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +10, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +11, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +12, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +13, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +14, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +15, DESC_COL ,"├─────────────────────────────────────┤" );
  print_string( DESC_X , DESC_Y +16, DESC_COL ,"│< 結束 >                                                                  │" );
  print_string( DESC_X , DESC_Y +17, DESC_COL ,"└─────────────────────────────────────┘" );

  return;
}

void show_desc_to_room( ROOM_INFO * pRoom )
{

  int i;

  for ( i = 0; i < 5 ; i++ )
    print_string( ( i * 12 ) + 5 , DESC_Y + 1 , DESC_COL
      , ( ( pRoom->desc[i]->keyword[0] != '\x0' ) ? "*" : " " ) );

  return;
}

void print_desc_shape( int serial )
{

  char * const desc_name [] = { "描述一" , "描述二"
    , "描述三" , "描述四" , "描述五" };

  print_string( DESC_X , DESC_Y + 4, DESC_COL ,"┌─────────────────────────────────────┐" );
  print_string( DESC_X , DESC_Y + 5, DESC_COL ,"│關鍵字                                                                    │" );
  print_string( DESC_X , DESC_Y + 6, DESC_COL ,"├─────────────────────────────────────┤" );
  print_string( DESC_X , DESC_Y + 7, DESC_COL ,"│描述                                                                      │" );
  print_string( DESC_X , DESC_Y + 8, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y + 9, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +10, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +11, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +12, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +13, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +14, DESC_COL ,"│                                                                          │" );
  print_string( DESC_X , DESC_Y +15, DESC_COL ,"├─────────────────────────────────────┤" );
  print_string( DESC_X , DESC_Y +16, DESC_COL ,"│< 結束 >                                                                  │" );
  print_string( DESC_X , DESC_Y +17, DESC_COL ,"└─────────────────────────────────────┘" );

  if ( serial >= 0 && serial < 5 )
    print_string( DESC_X + 65  , DESC_Y + 5 , DESC_COL
      , desc_name[serial] );

  return;
}
void show_desc_bar( int control )
{
  print_string( DESC_X +  7 , DESC_Y + 1
    ,( ( control == 1 ) ? DESC_TAG : DESC_COL ) , "描述一" );

  print_string( DESC_X + 19 , DESC_Y + 1
    ,( ( control == 2 ) ? DESC_TAG : DESC_COL ) , "描述二" );

  print_string( DESC_X + 31 , DESC_Y + 1
    ,( ( control == 3 ) ? DESC_TAG : DESC_COL ) , "描述三" );

  print_string( DESC_X + 43 , DESC_Y + 1
    ,( ( control == 4 ) ? DESC_TAG : DESC_COL ) , "描述四" );

  print_string( DESC_X + 55 , DESC_Y + 1
    ,( ( control == 5 ) ? DESC_TAG : DESC_COL ) , "描述五" );

  print_string( DESC_X + 65 , DESC_Y + 1
    ,( ( control == 6 ) ? DESC_TAG : DESC_COL ) , "結束" );

  return;
}

void print_desc_bar( int control )
{
  print_string( DESC_X +  2 , DESC_Y + 5
    ,( ( control == 1 ) ? DESC_TAG : DESC_COL ) , "關鍵字" );

  print_string( DESC_X +  2 , DESC_Y + 7
    ,( ( control == 2 ) ? DESC_TAG : DESC_COL ) , "描述" );

  print_string( DESC_X +  4 , DESC_Y + 16
    ,( ( control == 3 ) ? DESC_TAG : DESC_COL ) , "結束"     );

  return;
}

void do_edit_desc( ROOM_DESC * pDesc , int serial )
{

  int           execute = 0;
  int           control = 1;
  int           touch;

  print_desc_shape( serial );

  /* 設定房間關鍵字字串起啟值 */
  pDesc->pKeyword->save_x_cursor   = 0;
  pDesc->pKeyword->save_y_cursor   = 0;

  /* 設定房間關鍵字字串起啟值 */
  pDesc->pDesc->save_x_cursor   = 0;
  pDesc->pDesc->save_y_cursor   = 0;

  print_long_string( 1 , pDesc->pKeyword     );
  print_long_string( 1 , pDesc->pDesc        );

  do
  {

    print_desc_bar( control );
    touch = get_control_key();

    if ( touch == RETURN_RIGHT  ) control++;
    if ( touch == RETURN_LEFT   ) control--;
    if ( touch == RETURN_TAB    ) control++;
    if ( touch == RETURN_UP     ) control--;
    if ( touch == RETURN_DOWN   ) control++;
    if ( touch == RETURN_HOME   ) control = 1;
    if ( touch == RETURN_END    ) control = 3;
    if ( touch == RETURN_RETURN ) execute = control;

    check_boundary( 1 , 3 , &control );

    switch ( execute )
    {
      case 1 :

        get_string( pDesc->pKeyword );
        execute = 0;
        break;

      case 2 :

        get_string( pDesc->pDesc );
        execute = 0;
        break;
    }

  } while ( execute != 3 );

  print_desc_shape( 7 );
  return;
}

void do_save_room( ROOM_INFO * pRoom , FILE * fp )
{
  int  loop;
  int  touch;

  fprintf( fp , "Vnum            %d\n" , pRoom->vnum      );
  fprintf( fp , "Name            %s~\n" , pRoom->name     );

  fprintf( fp , "Description\n"                           );
  write_string( fp , pRoom->pDesc );

  fprintf( fp , "SectorType      "                        );
  write_select( fp , pRoom->room_sector , sector_name     );

  fprintf( fp , "RoomFlag        "                        );
  write_bit( fp , pRoom->room_flag , room_flag_name   );

  for ( touch = 0 , loop = 0 ; loop < 6 ; loop++ )
  {
    if ( pRoom->exit[loop]->room > 0 )
    {
       if ( touch == 0 ) fprintf( fp , "\n"                           );
       fprintf( fp , "#Exit\n"                                        );

       fprintf( fp , "Direction       "                               );
       write_select( fp , loop , direction_name                       );

       fprintf( fp , "ExitVnum        %d\n" , pRoom->exit[loop]->room );

       fprintf( fp , "ExitDesc        "                               );
       write_string( fp , pRoom->exit[loop]->pDesc                    );

       fprintf( fp , "ExitKeyword     "                               );
       write_string( fp , pRoom->exit[loop]->pKeyword                 );

       fprintf( fp , "ExitKey         %d\n" , pRoom->exit[loop]->key  );

       fprintf( fp , "Lock            "                               );
       write_select( fp , pRoom->exit[loop]->lock , lock_status       );

       fprintf( fp , "End\n\n"                                        );
       touch = 1;
    }

  }

  for ( loop = 0 ; loop < 5 ; loop++ )
  {
    if ( pRoom->desc[loop]->keyword[0] != '\x0' )
    {
       if ( touch == 0 ) fprintf( fp , "\n"                           );
       fprintf( fp , "#Keyword\n"                                     );

       fprintf( fp , "Keyword         "                               );
       write_string( fp , pRoom->desc[loop]->pKeyword                 );

       fprintf( fp , "Description\n"                                  );
       write_string( fp , pRoom->desc[loop]->pDesc                    );

       fprintf( fp , "End\n\n"     );
       touch = 1;

    }
  }

  return;

}

int load_room( ROOM_INFO * pRoom )
{

  char   word[ MAX_WORD_LENGTH ];
  char * pWord;
  int    Match;

  pWord = word ;

  for ( ; ; )
  {
    fread_word( pRoom->pFile , pWord );
    if ( pRoom->pFile->status == 1 ) return FALSE;
    Match = FALSE;

    switch( pWord[0] )
    {
      case '*' :

        fread_to_eol( pRoom->pFile );
        Match = TRUE;
        break;

      case '#' :

        if ( !strcmp( pWord , "#ROOM" ) )
        {
          Match = TRUE;
          break;
        }

       break;

     case 'D' :

       if ( !strcmp( pWord , "Description" ) )
       {
         fread_to_eol( pRoom->pFile );
         fread_string( pRoom->pFile , pRoom->pDesc );
         Match = TRUE;
         break;
       }

       break;

     case 'E' :

       if ( !strcmp( pWord , "End" ) )
       {
         if ( fread_if_eof( pRoom->pFile ) ) return TRUE;
         else                                return FALSE;
       }

       break;

     case 'N' :

       if ( !strcmp( pWord , "Name" ) )
       {
         fread_string( pRoom->pFile , pRoom->pName );
         if ( pRoom->pFile->status == 1 ) return FALSE;
         Match = TRUE;
         break;
       }

       break;

     case 'S' :

       if ( !strcmp( pWord , "SectorType" ) )
       {
          pRoom->room_sector  = fread_number( pRoom->pFile );
          if ( pRoom->pFile->status == 1 ) return FALSE;
          if ( pRoom->room_sector < 0 || pRoom->room_sector > 9 )
            return FALSE;

          Match = TRUE;
          break;
        }

        break;

     case 'V' :

       if ( !strcmp( pWord , "Vnum" ) )
       {
          pRoom->vnum   = fread_number( pRoom->pFile );
          if ( pRoom->pFile->status == 1 ) return FALSE;
          if ( pRoom->vnum   < 0 )         return FALSE;

          Match = TRUE;
          break;

       }

       break;

    }

    if ( !Match ) return FALSE;
  }
}
