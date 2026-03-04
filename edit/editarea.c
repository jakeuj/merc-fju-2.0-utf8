#include "edit.h"

#define AREA_X          10
#define AREA_Y          6
#define AREA_FRONT_COL  YELLOW
#define AREA_BACK_COL   BLACK
#define AREA_TAG_COL    RED
#define AREA_COL        ( AREA_FRONT_COL + ( AREA_BACK_COL << 4 ) )
#define AREA_TAG        ( AREA_FRONT_COL + ( AREA_TAG_COL  << 4 ) )

void show_area_shape            ( AREA_INFO * , FILE_INFO * );
void show_edit_area             ( int );
void set_area_default           ( AREA_INFO * );
int  load_area                  ( AREA_INFO * );

void show_area_shape( AREA_INFO *pArea , FILE_INFO * pFile )
{
  print_string( AREA_X , AREA_Y   , AREA_COL ,"┌────────────────────────────┐" );
  print_string( AREA_X , AREA_Y+1 , AREA_COL ,"│ 區域檔名:                                              │" );
  print_string( AREA_X , AREA_Y+2 , AREA_COL ,"├────────────────────────────┤" );
  print_string( AREA_X , AREA_Y+3 , AREA_COL ,"│ 區域的屬性: (*) 不顯示 (*) 需要顯示                    │" );
  print_string( AREA_X , AREA_Y+4 , AREA_COL ,"├────────────────────────────┤" );
  print_string( AREA_X , AREA_Y+5 , AREA_COL ,"│ 區域描述  :                                            │" );
  print_string( AREA_X , AREA_Y+6 , AREA_COL ,"├────────────────────────────┤" );
  print_string( AREA_X , AREA_Y+7 , AREA_COL ,"│ <存檔>  <放棄>                                         │" );
  print_string( AREA_X , AREA_Y+8 , AREA_COL ,"└────────────────────────────┘" );

  /* 列印檔名 */
  print_string( AREA_X + 15 , AREA_Y + 1 , AREA_COL , pFile->filename );

  /* 列印檔案的情形 */
  if ( pArea->valid == 0 )
    print_string( AREA_X + 40 , AREA_Y + 1 , AREA_COL , "舊檔案" );

  else if ( pArea->valid == 1 )
    print_string( AREA_X + 40 , AREA_Y + 1 , AREA_COL , "新檔案" );

  else if ( pArea->valid == 2 )
    print_string( AREA_X + 40 , AREA_Y + 1 , AREA_COL , "不正確的舊檔案" );


  /* 列印屬性 */
  if ( pArea->attribution == 0 )
  {
    print_string( AREA_X + 16 , AREA_Y + 3 , AREA_COL , "*" );
    print_string( AREA_X + 27 , AREA_Y + 3 , AREA_COL , " " );
  }

  else
  {
    print_string( AREA_X + 16 , AREA_Y + 3 , AREA_COL , " " );
    print_string( AREA_X + 27 , AREA_Y + 3 , AREA_COL , "*" );
  }

  /* 列印編輯者 */

  print_string( AREA_X + 15 , AREA_Y + 5 , AREA_COL , pArea->editor );

  return;
}

/* 顯示編輯區域格式的畫面 */

void show_edit_area( int control )
{
  print_string( AREA_X + 3 , AREA_Y + 3
    ,( ( control == 1 ) ? AREA_TAG : AREA_COL )
    , "區域的屬性" );

  print_string( AREA_X + 3 , AREA_Y + 5
    ,( ( control == 2 ) ? AREA_TAG : AREA_COL )
    , "區域描述" );

  print_string( AREA_X + 4 , AREA_Y + 7
    ,( ( control == 3 ) ? AREA_TAG : AREA_COL )
    , "存檔" );

  print_string( AREA_X + 12 , AREA_Y + 7
    ,( ( control == 4 ) ? AREA_TAG : AREA_COL )
    , "放棄" );

  return;

}

/* 編輯新格式區域的主程式 */

void do_edit_area( void )
{
  AREA_INFO     area_formation;
  AREA_INFO   * pArea;
  STRING_INFO   aEditor;
  STRING_INFO * pEditor;
  FILE_INFO   * pFile;
  int           control = 1;
  int           execute = 0;
  int           control_key;

  /* 清除螢幕以及消除游標 */
  clear_screen();
  set_cursor_type( NO_CURSOR );

  /* 開啟檔案 */
  pFile = get_file_name( ".sec" );

  /* 檔案無法讀取 */
  if ( pFile->information < 0 ) return;

  /* 設定區域的起使值 */
  pArea              = &area_formation;
  pArea->valid       = pFile->information;
  pArea->pEditor     = &aEditor;
  pArea->pFile       = pFile;

  /* 設定編輯者字串起啟值 */
  {
    pEditor                  = &aEditor;
    pEditor->x_pos           = AREA_X + 15;
    pEditor->y_pos           = AREA_Y +  5;
    pEditor->x_len           = 40;
    pEditor->y_len           =  1;
    pEditor->front_color     = BLUE;
    pEditor->back_color      = LIGHTGRAY;
    pEditor->org_front_color = AREA_COL;
    pEditor->org_back_color  = BLACK;
    pEditor->save_x_cursor   = 0;
    pEditor->save_y_cursor   = 0;
    pEditor->smash           = SMASH_EXECUTE;
    pEditor->number          = NUMBER_ACCEPT;
    pEditor->address         = pArea->editor;
    clear_string( pEditor );
  }

  /* 設定區域的起使值 */
  set_area_default( pArea );

  /* 載入區域檔案 */
  if ( pFile->information == 0 )
  {

    /* 如果有錯誤 */
    if ( !load_area( pArea ) )
    {
      pArea->valid = 2;
      set_area_default( pArea );
    }
  }

  /* 顯示一開始的編輯螢幕 */
  show_area_shape( pArea , pFile );

  do
  {
    /* 顯示螢幕的變動 */
    show_edit_area( control );

    control_key = get_control_key();

    if ( control_key == RETURN_TAB    ) control++;
    if ( control_key == RETURN_UP     ) control--;
    if ( control_key == RETURN_DOWN   ) control++;
    if ( control_key == RETURN_RETURN ) execute = control;
    if ( control_key == RETURN_HOME   ) control = 1;
    if ( control_key == RETURN_END    ) control = 4;

    /* 檢查是否超出範圍 */
    check_boundary( 1 , 4 , &control );

    switch ( execute )
    {

     /* 編輯區域的屬性 */
     case 1 :

       while ( TRUE )
       {
         control_key = get_control_key();

         if ( control_key == RETURN_RETURN || control_key == RETURN_TAB )
           break;

         if ( control_key == RETURN_LEFT  ) pArea->attribution++;
         if ( control_key == RETURN_RIGHT ) pArea->attribution--;

         /* 檢查是否超出範圍 */
         check_boundary( 0 , 1 , &(pArea->attribution ) );

         show_edit_area( control );

         if ( pArea->attribution == 0 )
         {
           print_string( AREA_X + 16 , AREA_Y + 3 , AREA_COL , "*" );
           print_string( AREA_X + 27 , AREA_Y + 3 , AREA_COL , " " );
         }

         else
         {
           print_string( AREA_X + 16 , AREA_Y + 3 , AREA_COL , " " );
           print_string( AREA_X + 27 , AREA_Y + 3 , AREA_COL , "*" );
         }
       }

       execute = 0;
       break;

     /* 編輯編輯者 */
     case 2 :

       get_string( pEditor );

       execute = 0;
       break;

     /* 存檔 */
     case 3 :

       fclose( pFile->filehandler );
       pFile->filehandler = fopen ( pFile->filename ,"w+" );

       fprintf( pFile->filehandler , "#AREA\n" );

       if ( pArea->attribution == 0 )
         fprintf( pFile->filehandler , "Echo    NO_ECHO\n" );

       else
         fprintf( pFile->filehandler , "Echo    WILL_ECHO\n" );

       fprintf( pFile->filehandler , "Editor  %s~\n" ,
         pArea->editor );

       fprintf( pFile->filehandler , "End\n" );

       fclose( pFile->filehandler );
       return;

     /* 放棄編輯 */

     case 4 :

       /* 若是新的檔案則刪除 , 不然則關檔. */

       if ( pFile->information == 1 )
         delete_null_file( pFile );
       else fclose( pFile->filehandler );

    }
  } while ( execute != 4 );
}

#undef AREA_X
#undef AREA_Y
#undef AREA_FRONT_COL
#undef AREA_BACK_COL
#undef AREA_COLOR
#undef AREA_TAG

/* 設定區域的預設值 */

void set_area_default( AREA_INFO *pArea )
{
  clear_string( pArea->pEditor );
  pArea->attribution = 1;
  return;
}

int load_area( AREA_INFO * pArea )
{
  char   word[ MAX_WORD_LENGTH ];
  char * pWord;
  int    Match;

  pWord = word ;
  for ( ; ; )
  {
    fread_word( pArea->pFile , pWord );
    if ( pArea->pFile->status == 1 ) return FALSE;
    Match = FALSE;

    switch( pWord[0] )
    {
      case '*' :

        fread_to_eol( pArea->pFile );
        Match = TRUE;
        break;

      case '#' :

        if ( !strcmp( pWord , "#AREA" ) )
        {
          Match = TRUE;
          break;
        }

       break;

     case 'E' :

       if ( !strcmp( pWord , "End" ) )
       {
         if ( fread_if_eof( pArea->pFile ) ) return TRUE;
         else                                return FALSE;
       }

       if ( !strcmp( pWord , "Echo" ) )
       {
          pArea->attribution = fread_number( pArea->pFile );
          if ( pArea->pFile->status == 1 ) return FALSE;
          if ( pArea->attribution > 1 && pArea->attribution < 0 )
            return FALSE;

          Match = TRUE;
          break;
       }

       if ( !strcmp( pWord , "Editor" ) )
       {
          fread_string( pArea->pFile , pArea->pEditor );
          if ( pArea->pFile->status == 1 ) return FALSE;
          Match = TRUE;
          break;
       }

       break;

    }

    if ( !Match ) return FALSE;

  }
}
