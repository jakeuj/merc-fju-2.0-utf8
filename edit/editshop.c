#include "edit.h"

#define SHOP_X          5
#define SHOP_Y          3
#define SHOP_FRONT_COL  LIGHTGREEN
#define SHOP_BACK_COL   BLACK
#define SHOP_TAG_COL    RED
#define SHOP_COL        ( SHOP_FRONT_COL + ( SHOP_BACK_COL << 4 ) )
#define SHOP_TAG        ( SHOP_FRONT_COL + ( SHOP_TAG_COL  << 4 ) )

void    show_edit_shop_shape    ( SHOP_INFO * );
void    show_edit_shop          ( int );
void    show_shop_object        ( SHOP_INFO * );
void    show_shop_object_title  ( int );
void    do_save_shop            ( SHOP_INFO * , FILE * );
void    set_shop_default        ( SHOP_INFO * );
int     load_shop               ( SHOP_INFO * );

/* 顯示商店的起始畫面 */
void show_edit_shop_shape( SHOP_INFO * pShop )
{

  char show_number[ 10 ];

  print_string( SHOP_X , SHOP_Y    , SHOP_COL ,"┌──────────────────────────────────┐" );
  print_string( SHOP_X , SHOP_Y + 1, SHOP_COL ,"│檔案名稱 :                                                          │" );
  print_string( SHOP_X , SHOP_Y + 2, SHOP_COL ,"├──────────────────────────────────┤" );
  print_string( SHOP_X , SHOP_Y + 3, SHOP_COL ,"│商店主人 : [     ]                                                  │" );
  print_string( SHOP_X , SHOP_Y + 4, SHOP_COL ,"├──────────────────────────────────┤" );
  print_string( SHOP_X , SHOP_Y + 5, SHOP_COL ,"│販賣物品種類 [ ] 燈籠 [ ] 捲軸 [ ] 法杖1 [ ] 法杖2 [ ] 武器 [ ] 寶物│" );
  print_string( SHOP_X , SHOP_Y + 6, SHOP_COL ,"├──────────────────────────────────┤" );
  print_string( SHOP_X , SHOP_Y + 7, SHOP_COL ,"│[ ] 護甲 [ ] 藥水 [ ] 家具 [ ] 垃圾 [ ] 容器 [ ] 水袋 [ ] 鑰匙      │" );
  print_string( SHOP_X , SHOP_Y + 8, SHOP_COL ,"├──────────────────────────────────┤" );
  print_string( SHOP_X , SHOP_Y + 9, SHOP_COL ,"│[ ] 食物 [ ] 錢 [ ] 船 [ ] 非玩家屍體 [ ] 玩家屍體 [ ] 泉水 [ ] 藥丸│" );
  print_string( SHOP_X , SHOP_Y +10, SHOP_COL ,"├──────────────────────────────────┤" );
  print_string( SHOP_X , SHOP_Y +11, SHOP_COL ,"│開店時間 [  ]    關店時間 [  ]    買入利益 [   ]    賣出利益 [   ]  │" );
  print_string( SHOP_X , SHOP_Y +12, SHOP_COL ,"├──────────────────────────────────┤" );
  print_string( SHOP_X , SHOP_Y +13, SHOP_COL ,"│<存檔>  <放棄>                                                      │" );
  print_string( SHOP_X , SHOP_Y +14, SHOP_COL ,"└──────────────────────────────────┘" );

  /* 列印檔名 */
  print_string( SHOP_X + 15 , SHOP_Y + 1 , SHOP_COL
    , pShop->pFile->filename );

  /* 列印檔案的情形 */
  if ( pShop->valid == 0 )
    print_string( SHOP_X + 40 , SHOP_Y + 1 , SHOP_COL , "舊檔案" );

  else if ( pShop->valid == 1 )
    print_string( SHOP_X + 40 , SHOP_Y + 1 , SHOP_COL , "新檔案" );

  else if ( pShop->valid == 2 )
    print_string( SHOP_X + 40 , SHOP_Y + 1 , SHOP_COL , "不正確的舊檔案" );

  /* 轉換商店主人變數成字串, 並且向右對齊 5 */
  number_to_string( pShop->keeper , show_number );
  align_word( show_number , 5 );
  print_string( SHOP_X + 14 , SHOP_Y + 3 , SHOP_COL , show_number );

  /* 轉換商店開店時間變數成字串, 並且向右對齊 2 */
  number_to_string( pShop->openhour , show_number );
  align_word( show_number , 2 );
  print_string( SHOP_X + 12 , SHOP_Y +11 , SHOP_COL , show_number );

  /* 轉換商店關店時間變數成字串, 並且向右對齊 2 */
  number_to_string( pShop->closehour , show_number );
  align_word( show_number , 2 );
  print_string( SHOP_X + 29 , SHOP_Y +11 , SHOP_COL , show_number );

  /* 轉換商店買入利益變數成字串, 並且向右對齊 3 */
  number_to_string( pShop->buyprofit , show_number );
  align_word( show_number , 3 );
  print_string( SHOP_X + 46 , SHOP_Y +11 , SHOP_COL , show_number );

  /* 轉換商店售出利益變數成字串, 並且向右對齊 3 */
  number_to_string( pShop->sellprofit , show_number );
  align_word( show_number , 3 );
  print_string( SHOP_X + 64 , SHOP_Y +11 , SHOP_COL , show_number );

  /* 顯示販賣物品種類 */
  show_shop_object( pShop );

  return;
}

/* 顯示編輯時的控制光棒 */
void show_edit_shop( int control )
{
  print_string( SHOP_X + 2 , SHOP_Y + 3
    ,( ( control == 1 ) ? SHOP_TAG : SHOP_COL ) , "商店主人"     );

  print_string( SHOP_X + 2 , SHOP_Y + 5
    ,( ( control == 2 ) ? SHOP_TAG : SHOP_COL ) , "販賣物品種類" );

  print_string( SHOP_X + 2 , SHOP_Y + 11
    ,( ( control == 3 ) ? SHOP_TAG : SHOP_COL ) , "開店時間"     );

  print_string( SHOP_X + 19 , SHOP_Y + 11
    ,( ( control == 4 ) ? SHOP_TAG : SHOP_COL ) , "關店時間"     );

  print_string( SHOP_X + 36 , SHOP_Y + 11
    ,( ( control == 5 ) ? SHOP_TAG : SHOP_COL ) , "買入利益"     );

  print_string( SHOP_X + 54 , SHOP_Y + 11
    ,( ( control == 6 ) ? SHOP_TAG : SHOP_COL ) , "賣出利益"     );

  print_string( SHOP_X + 3 , SHOP_Y + 13
    ,( ( control == 7 ) ? SHOP_TAG : SHOP_COL ) , "存檔"         );

  print_string( SHOP_X + 11 , SHOP_Y + 13
    ,( ( control == 8 ) ? SHOP_TAG : SHOP_COL ) , "放棄"         );

  return;
}

/* 顯示商店可以販賣物品的種類 */
void show_shop_object( SHOP_INFO * pShop )
{

  print_string( SHOP_X + 16 , SHOP_Y + 5 , SHOP_COL
    , ( ( pShop->object[ 0 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 25 , SHOP_Y + 5 , SHOP_COL
    , ( ( pShop->object[ 1 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 34 , SHOP_Y + 5 , SHOP_COL
    , ( ( pShop->object[ 2 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 44 , SHOP_Y + 5 , SHOP_COL
    , ( ( pShop->object[ 3 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 54 , SHOP_Y + 5 , SHOP_COL
    , ( ( pShop->object[ 4 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 63 , SHOP_Y + 5 , SHOP_COL
    , ( ( pShop->object[ 5 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X +  3 , SHOP_Y + 7 , SHOP_COL
    , ( ( pShop->object[ 6 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 12 , SHOP_Y + 7 , SHOP_COL
    , ( ( pShop->object[ 7 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 21 , SHOP_Y + 7 , SHOP_COL
    , ( ( pShop->object[ 8 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 30 , SHOP_Y + 7 , SHOP_COL
    , ( ( pShop->object[ 9 ] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 39 , SHOP_Y + 7 , SHOP_COL
    , ( ( pShop->object[ 10] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 48 , SHOP_Y + 7 , SHOP_COL
    , ( ( pShop->object[ 11] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 57 , SHOP_Y + 7 , SHOP_COL
    , ( ( pShop->object[ 12] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X +  3 , SHOP_Y + 9 , SHOP_COL
    , ( ( pShop->object[ 13] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 12 , SHOP_Y + 9 , SHOP_COL
    , ( ( pShop->object[ 14] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 19 , SHOP_Y + 9 , SHOP_COL
    , ( ( pShop->object[ 15] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 26 , SHOP_Y + 9 , SHOP_COL
    , ( ( pShop->object[ 16] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 41 , SHOP_Y + 9 , SHOP_COL
    , ( ( pShop->object[ 17] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 54 , SHOP_Y + 9 , SHOP_COL
    , ( ( pShop->object[ 18] == 0 ) ? " " : "*" ) );

  print_string( SHOP_X + 63 , SHOP_Y + 9 , SHOP_COL
    , ( ( pShop->object[ 19] == 0 ) ? " " : "*" ) );

  return;
}

/* 顯示商店可以販賣物品種類的名字 */
void show_shop_object_title( int selection )
{
  print_string( SHOP_X + 19 , SHOP_Y + 5
    ,( ( selection == 0 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 0 ].item_name );

  print_string( SHOP_X + 28 , SHOP_Y + 5
    ,( ( selection == 1 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 1 ].item_name );

  print_string( SHOP_X + 37 , SHOP_Y + 5
    ,( ( selection == 2 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 2 ].item_name );

  print_string( SHOP_X + 47 , SHOP_Y + 5
    ,( ( selection == 3 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 3 ].item_name );

  print_string( SHOP_X + 57 , SHOP_Y + 5
    ,( ( selection == 4 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 4 ].item_name );

  print_string( SHOP_X + 66 , SHOP_Y + 5
    ,( ( selection == 5 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 5 ].item_name );

  print_string( SHOP_X + 6 , SHOP_Y + 7
    ,( ( selection == 6 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 6 ].item_name );

  print_string( SHOP_X + 15 , SHOP_Y + 7
    ,( ( selection == 7 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 7 ].item_name );

  print_string( SHOP_X + 24 , SHOP_Y + 7
    ,( ( selection == 8 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 8 ].item_name );

  print_string( SHOP_X + 33 , SHOP_Y + 7
    ,( ( selection == 9 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 9 ].item_name );

  print_string( SHOP_X + 42 , SHOP_Y + 7
    ,( ( selection == 10 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 10 ].item_name );

  print_string( SHOP_X + 51 , SHOP_Y + 7
    ,( ( selection == 11 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 11 ].item_name );

  print_string( SHOP_X + 60 , SHOP_Y + 7
    ,( ( selection == 12 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 12 ].item_name );

  print_string( SHOP_X + 6 , SHOP_Y + 9
    ,( ( selection == 13 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 13 ].item_name );

  print_string( SHOP_X + 15 , SHOP_Y + 9
    ,( ( selection == 14 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 14 ].item_name );

  print_string( SHOP_X + 22 , SHOP_Y + 9
    ,( ( selection == 15 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 15 ].item_name );

  print_string( SHOP_X + 29 , SHOP_Y + 9
    ,( ( selection == 16 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 16 ].item_name );

  print_string( SHOP_X + 44 , SHOP_Y + 9
    ,( ( selection == 17 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 17 ].item_name );

  print_string( SHOP_X + 57 , SHOP_Y + 9
    ,( ( selection == 18 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 18 ].item_name );

  print_string( SHOP_X + 66 , SHOP_Y + 9
    ,( ( selection == 19 ) ? SHOP_TAG : SHOP_COL )
    , item_type[ 19 ].item_name );

  return;
}

void do_edit_shop ( void )
{
  FILE_INFO   * pFile;
  SHOP_INFO     aShop;
  SHOP_INFO   * pShop;
  STRING_INFO   aString;
  STRING_INFO * pString;
  int           control_key;
  int           control = 1;
  int           execute = 0;
  int           loop;
  int           selection = 0;
  char          temp[10];

  /* 清除螢幕以及消除游標 */
  clear_screen();
  set_cursor_type( NO_CURSOR );

  /* 開啟檔案 */
  pFile = get_file_name( ".sho" );

  /* 檔案無法讀取 */
  if ( pFile->information < 0 ) return;

  /* 設定商店的起使值 */
  pShop               = &aShop;
  pShop->valid        = pFile->information;
  pShop->pFile        = pFile;
  set_shop_default( pShop );

  /* 設定公用輸入字串的起始值 */
  pString                  = &aString;
  pString->y_len           = 1;
  pString->front_color     = BLUE;
  pString->back_color      = LIGHTGRAY;
  pString->org_front_color = SHOP_COL;
  pString->org_back_color  = BLACK;
  pString->smash           = SMASH_NO_EXECUTE;
  pString->number          = NUMBER_ONLY;
  pString->address         = temp;

  /* 載入商店檔案 */
  if ( pFile->information == 0 )
  {
    /* 如果有錯誤 */
    if ( !load_shop( pShop ) )
    {
      pShop->valid = 2;
      set_shop_default( pShop );
    }
  }

  /* 顯示編輯商店的畫面 */
  show_edit_shop_shape( pShop );

  do
  {
    /* 顯示控制光棒 */
    show_edit_shop( control );

    /* 從鍵盤得取控制鍵 */
    control_key = get_control_key();

    if ( control_key == RETURN_TAB    ) control++;
    if ( control_key == RETURN_UP     ) control--;
    if ( control_key == RETURN_DOWN   ) control++;
    if ( control_key == RETURN_RETURN ) execute = control;
    if ( control_key == RETURN_SPACE  ) execute = control;
    if ( control_key == RETURN_HOME   ) control = 1;
    if ( control_key == RETURN_END    ) control = 8;

    check_boundary( 1 , 8 , &control );

    switch ( execute )
    {
      /* 編輯商店主人 */
      case 1 :

        pString->x_pos           = SHOP_X + 14;
        pString->y_pos           = SHOP_Y +  3;
        pString->x_len           = 5;
        pString->y_len           = 1;
        pString->save_x_cursor   = 0;
        pString->save_y_cursor   = 0;
        for ( loop = 0 ; loop < 6; loop++ ) *(pString->address+loop) = '\x0';
        number_to_string( pShop->keeper , pString->address );

        get_string( pString );
        pShop->keeper = atoi(pString->address );
        if ( pShop->keeper < 0 ) pShop->keeper = 0;

        number_to_string( pShop->keeper , pString->address );
        align_word( pString->address , 5 );
        print_string( SHOP_X + 14 , SHOP_Y + 3 , SHOP_COL , pString->address );

        execute = 0;
        break;

      /* 編輯販賣物品的種類 */
      case 2 :

        {
          show_shop_object( pShop );
          show_shop_object_title( selection );

          do
          {
            control_key = get_control_key();

            if ( control_key == RETURN_LEFT  ) selection--;
            if ( control_key == RETURN_RIGHT ) selection++;
            if ( control_key == RETURN_HOME  ) selection = 0;
            if ( control_key == RETURN_END   ) selection =19;


            /* 若是 PGDN 或是  PGUP 則清除所有的選項 */
            if ( control_key == RETURN_PGDN || control_key == RETURN_PGUP )
            {
              for ( loop = 0; loop < MAX_ITEM ; loop++ )
                pShop->object[ loop ] = '\x0';
              pShop->object_count = 0;

            }

            /* 空白鍵則代表選擇這種物品是否可以販賣 */
            if ( control_key == RETURN_SPACE
                 || control_key == RETURN_UP
                 || control_key == RETURN_DOWN )
            {
              /* 若是已經圈選, 則反向清除 */
              if ( pShop->object[selection] == 1 )
              {
                pShop->object[selection] = 0;
                pShop->object_count--;
              }

              /* 若沒圈選, 則設定它 */
              else
              {
                /* 若圈選的數目多過 MAX_TRADE 則不處理 */
                if ( pShop->object_count < MAX_TRADE )
                {
                  /* 若這種物品不可以販賣, 也不處理它 */
                  if ( item_type[selection].sell == 1 )
                  {
                    pShop->object[selection] = 1;
                    pShop->object_count++;
                  }
                }
                else
                {
                   warning_beep();
                }
              }
            }

            /* 看看選擇項目是否超出了範圍 */
            check_boundary( 0 , MAX_ITEM - 1 , &selection );

            /* 顯示是否物品可以被販賣 */
            show_shop_object( pShop );

            /* 顯示全部可以販賣物品種類的名稱 */
            show_shop_object_title( selection );

          } while ( control_key != RETURN_RETURN
              && control_key != RETURN_TAB );

         /* 清除販賣物品的選擇光棒 */
         show_shop_object_title( MAX_ITEM );

         execute = 0;
         break;

        }

      /* 設定商店開門的時間 範圍 0 - 24 */
      case 3 :

        pString->x_pos           = SHOP_X + 12;
        pString->y_pos           = SHOP_Y + 11;
        pString->x_len           = 2;
        pString->y_len           = 1;
        pString->save_x_cursor   = 0;
        pString->save_y_cursor   = 0;
        for ( loop = 0 ; loop < 3; loop++ ) *(pString->address+loop) = '\x0';
        number_to_string( pShop->openhour , pString->address );

        get_string( pString );
        pShop->openhour = atoi(pString->address );

        /* 檢查時間是否超出範圍 */
        if ( pShop->openhour < 0  ) pShop->openhour = 0;
        if ( pShop->openhour > 24 ) pShop->openhour = 0;

        number_to_string( pShop->openhour , pString->address );
        align_word( pString->address , 2 );
        print_string( SHOP_X + 12 , SHOP_Y + 11, SHOP_COL , pString->address );

        execute = 0;
        break;

      /* 設定商店關門的時間 範圍 0 - 24 */
      case 4 :

        pString->x_pos           = SHOP_X + 29;
        pString->y_pos           = SHOP_Y + 11;
        pString->x_len           = 2;
        pString->y_len           = 1;
        pString->save_x_cursor   = 0;
        pString->save_y_cursor   = 0;
        for ( loop = 0 ; loop < 3; loop++ ) *(pString->address+loop) = '\x0';
        number_to_string( pShop->closehour , pString->address );

        get_string( pString );
        pShop->closehour = atoi(pString->address );

        /* 檢查時間是否超出範圍 */
        if ( pShop->closehour <  0 ) pShop->closehour = 0;
        if ( pShop->closehour > 24 ) pShop->closehour = 0;

        number_to_string( pShop->closehour , pString->address );
        align_word( pString->address , 2 );
        print_string( SHOP_X + 29 , SHOP_Y + 11, SHOP_COL , pString->address );

        execute = 0;
        break;

      /* 設定商店買入的利益 範圍 0 - 999 */
      case 5 :

        pString->x_pos           = SHOP_X + 46;
        pString->y_pos           = SHOP_Y + 11;
        pString->x_len           = 3;
        pString->y_len           = 1;
        pString->save_x_cursor   = 0;
        pString->save_y_cursor   = 0;
        for ( loop = 0 ; loop < 4; loop++ ) *(pString->address+loop) = '\x0';
        number_to_string( pShop->buyprofit , pString->address );

        get_string( pString );
        pShop->buyprofit = atoi(pString->address );

        /* 檢查是否超出範圍 */
        if ( pShop->buyprofit <  0 ) pShop->buyprofit = 0;

        number_to_string( pShop->buyprofit , pString->address );
        align_word( pString->address , 3 );
        print_string( SHOP_X + 46 , SHOP_Y + 11, SHOP_COL , pString->address );

        execute = 0;
        break;

      /* 設定商店賣出的利益 範圍 0 - 999 */
      case 6 :

        pString->x_pos           = SHOP_X + 64;
        pString->y_pos           = SHOP_Y + 11;
        pString->x_len           = 3;
        pString->y_len           = 1;
        pString->save_x_cursor   = 0;
        pString->save_y_cursor   = 0;
        for ( loop = 0 ; loop < 4; loop++ ) *(pString->address+loop) = '\x0';
        number_to_string( pShop->sellprofit , pString->address );

        get_string( pString );
        pShop->sellprofit = atoi(pString->address );

        /* 檢查是否超出範圍 */
        if ( pShop->sellprofit <  0 ) pShop->sellprofit = 0;

        number_to_string( pShop->sellprofit , pString->address );
        align_word( pString->address , 3 );
        print_string( SHOP_X + 64 , SHOP_Y + 11, SHOP_COL , pString->address );

        execute = 0;

        break;

     /* 存檔 */
     case 7 :

       fclose( pFile->filehandler );
       pFile->filehandler = fopen ( pFile->filename ,"w+" );

       do_save_shop( pShop , pFile->filehandler );
       fclose( pFile->filehandler );
       return;

     /* 放棄 */
     case 8 :

       /* 若是新的檔案則刪除 , 不然則關檔. */

       if ( pFile->information == 1 ) delete_null_file( pFile );
       else fclose( pFile->filehandler );

    }
  } while ( execute != 8 );

  return;
}

/* 儲存商店檔案格式 */
void do_save_shop( SHOP_INFO * pShop , FILE * fp )
{
  fprintf( fp , "#SHOPS\n"                                 );
  fprintf( fp , "Keeper          %d\n" , pShop->keeper     );

  {
    int loop;
    int count;

    fprintf( fp , "Object          "                         );

    for ( loop = 0, count = 0; loop < 20; loop++ )
    {
      if ( pShop->object[ loop ] != 0 && count < MAX_TRADE )
      {
        if ( count != 0 ) fprintf( fp , " " );
        fprintf( fp , "%s" , item_type[ loop ].item_string_name );
        count++;
      }
    }

    if ( count == 0 ) fprintf( fp , "0" );
    fprintf( fp , "\n"                                       );
  }

  fprintf( fp , "OpenHour        %d\n" , pShop->openhour   );
  fprintf( fp , "CloseHour       %d\n" , pShop->closehour  );
  fprintf( fp , "SellProfit      %d\n" , pShop->sellprofit );
  fprintf( fp , "BuyProfit       %d\n" , pShop->buyprofit  );
  fprintf( fp , "End\n"                                    );

  return;
}

/* 設定商店的起始值 */
void set_shop_default( SHOP_INFO * pShop )
{
  int loop;

  pShop->keeper       = 0;
  pShop->openhour     = 0;
  pShop->closehour    = 23;
  pShop->buyprofit    = 100;
  pShop->sellprofit   = 120;
  pShop->object_count = 0;
  for ( loop = 0 ; loop < MAX_ITEM ; loop++ ) pShop->object[ loop ] = 0;

  return;
}

/* 從檔案載入商店格式 */
int load_shop( SHOP_INFO * pShop )
{
  char   word[ MAX_WORD_LENGTH ];
  char * pWord;
  int    Match;

  pWord = word ;

  for ( ; ; )
  {
    fread_word( pShop->pFile , pWord );
    if ( pShop->pFile->status == 1 ) return FALSE;
    Match = FALSE;

    switch( pWord[0] )
    {
      case '*' :

        fread_to_eol( pShop->pFile );
        Match = TRUE;
        break;

      case '#' :

        if ( !strcmp( pWord , "#SHOPS" ) )
        {
          Match = TRUE;
          break;
        }

       break;

     case 'B' :

       /* 買入利益 範圍 0 - 999 */
       if ( !strcmp( pWord , "BuyProfit" ) )
       {
          pShop->buyprofit = fread_number( pShop->pFile );

          /* 檢查是否超出範圍 */
          if ( pShop->pFile->status == 1                      ) return FALSE;
          if ( pShop->buyprofit < 0 || pShop->buyprofit > 999 ) return FALSE;

         Match = TRUE;
         break;
       }

       break;

     case 'C' :

       /* 關門的時間 範圍 0 - 24 */
       if ( !strcmp( pWord , "CloseHour" ) )
       {
          pShop->closehour = fread_number( pShop->pFile );

          /* 檢查是否超出範圍 */
          if ( pShop->pFile->status == 1                     ) return FALSE;
          if ( pShop->closehour < 0 || pShop->closehour > 24 ) return FALSE;


         Match = TRUE;
         break;
       }

       break;

     case 'E' :

       if ( !strcmp( pWord , "End" ) )
       {
         if ( fread_if_eof( pShop->pFile ) ) return TRUE;
         else                                return FALSE;
       }

       break;

     case 'K' :

       if ( !strcmp( pWord , "Keeper" ) )
       {
          pShop->keeper = fread_number( pShop->pFile );

          /* 檢查是否超出範圍 */
          if ( pShop->pFile->status == 1 ) return FALSE;
          if ( pShop->keeper < 0 )         return FALSE;

          Match = TRUE;
          break;
       }

       break;

     case 'O' :

       /* 開門的時間 範圍 0 - 24 */
       if ( !strcmp( pWord , "OpenHour" ) )
       {
          pShop->openhour = fread_number( pShop->pFile );
          if ( pShop->pFile->status == 1 ) return FALSE;

          /* 檢查是否超出範圍 */
          if ( pShop->openhour < 0 || pShop->openhour > 24 ) return FALSE;

          Match = TRUE;
          break;
       }

       /* 販賣物品的種類, 超過 MAX_TRADE 個數的系統不理會 */
       if ( !strcmp( pWord , "Object" ) )
       {
          int iTrade = 0;
          int object;

          while ( iTrade < MAX_TRADE )
          {
            object = fread_number( pShop->pFile );
            if ( pShop->pFile->status == 1         ) return FALSE;
            if ( object < 0 && object > MAX_ITEM   ) return FALSE;

            if ( object != 0 )
            {
              if ( item_type[ object - 1 ].sell == 0 ) return FALSE;
              pShop->object[ object-1 ] = 1;
              pShop->object_count++;
            }

            iTrade++;
            if ( fread_if_eol( pShop->pFile ) ) break;
          }

          fread_to_eol( pShop->pFile );
          Match = TRUE;
          break;
       }

       break;

     case 'S' :

       /* 售出利益 範圍 0 - 999 */
       if ( !strcmp( pWord , "SellProfit" ) )
       {
          pShop->sellprofit = fread_number( pShop->pFile );
          if ( pShop->pFile->status == 1 ) return FALSE;

          /* 檢查是否超出範圍 */
          if ( pShop->sellprofit < 0 || pShop->sellprofit > 999 )
            return FALSE;

         Match = TRUE;
         break;
       }

       break;

    }
    if ( !Match ) return FALSE;
  }
}
