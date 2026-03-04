#include "edit.h"

char *       fread_alpha        ( FILE_INFO * );
int          symbol_lookup      ( char * , int * );
FILE_INFO    file_structure;


/* 刪除空的檔案 */
void delete_null_file( FILE_INFO * pFile )
{

  if ( fgetc( pFile->filehandler ) !=  EOF )
  {
    fclose( pFile->filehandler );
  }

  else
  {

    fclose( pFile->filehandler );
    remove( pFile->filename );

    print_string( 20 , 23 , LIGHTGREEN
     , "空的檔案, 所以刪除, 按任何鍵繼續.\n" );

    get_control_key();
  }

  return;
}

/* 輸入檔名

必須引入參數一字串 lastname --> 副檔名

*/

#define FILE_X_POS      20
#define FILE_Y_POS      1
#define PRINT_LEN       ( FILE_LENGTH + 4 )
#define FILE_PER_COL    ( 80 / PRINT_LEN )
#define MAX_LINE        20

/* 讀取檔案的名字 , 他會傳回一個檔案資料的結構.
   必須引入參數一字串 lastname 副檔名 .

    傳回值 :

    filehandler ------> 檔案的讀取號碼.
    filename    ------> 檔案的名字.
    information ------> 0 檔案已經存在而且可以讀寫. 舊檔案 .
                        1 檔案不存在但是可以讀寫. 新檔案.
                       -1 檔案不存在, 且無法開啟.
                       -2 檔案存在, 但是無法讀寫.
                       -3 檔案存在, 但是無法開啟.

*/

FILE_INFO * get_file_name( char * lastname )
{
   STRING_INFO     pString;
   STRING_INFO   * filename_info;
   FILE_INFO     * file_pointer;
   char            filename[ 100 ];
   int             loop;
   int             cursor_x;
   int             cursor_y;
   DIR           * reading;
   struct dirent * next;

   /* 清除螢幕 */
   clear_screen();

   /* 列印相關的檔案 */
   reading = opendir ( LOCAL_DIRECTORY );

   if ( reading )
   {
     loop = 0;
     cursor_y = FILE_Y_POS + 2;

     while ( ( next = readdir( reading ) ) != NULL
       && loop < FILE_PER_COL * MAX_LINE  )
     {

       if ( strlen( next->d_name ) < PRINT_LEN - 1
         && string_compare( next->d_name , lastname ) )
       {

         cursor_x = ( PRINT_LEN * ( loop % FILE_PER_COL ) ) + 1;
         print_string( cursor_x , cursor_y , LIGHTGREEN , next->d_name );
         loop++;

         /* 考慮到換行 */
         if ( ( loop % FILE_PER_COL ) == 0 ) cursor_y++;

       }
     }

     /* 關閉 DIR 管道 */
     closedir( reading );
   }

   sprintf( filename , "請輸入檔名, 副檔名是 %s : " , lastname );
   print_string( FILE_X_POS , FILE_Y_POS , LIGHTGREEN , filename );

   filename_info = &pString;
   filename_info->x_pos           = FILE_X_POS + 28 ;
   filename_info->y_pos           = FILE_Y_POS ;
   filename_info->x_len           = FILE_LENGTH;
   filename_info->y_len           = 1;
   filename_info->front_color     = LIGHTBLUE;
   filename_info->back_color      = LIGHTGRAY;
   filename_info->save_x_cursor   = 0;
   filename_info->save_y_cursor   = 0;
   filename_info->org_front_color = BLUE;
   filename_info->org_back_color  = BLACK;
   filename_info->smash           = SMASH_NO_EXECUTE;
   filename_info->number          = NUMBER_ACCEPT;
   filename_info->address         = filename;

   for ( loop = 0 ; loop < 100 ; loop++ ) filename[ loop ] = '\x0';

   /* 讀取檔案的名字的字串 */
   get_string( filename_info );

   /* 設定檔案結構的資料 */
   file_pointer = &file_structure;
   strcat( filename , lastname );
   strcpy( file_pointer->filename , filename );
   file_pointer->filehandler = NULL;
   file_pointer->information = 0;

   /* 檔案名稱為空字串 */
   if ( !strcmp( file_pointer->filename , lastname ) )
   {
     file_pointer->information = -4;
     show_error( " 你的檔案名稱過短, 系統不承認.\n" );
     clear_screen();
     return file_pointer;
   }

   /* 測試檔案是否存在 -1 代表不存在 */
   if ( access( filename , 0 ) == -1 )
   {
     /* 先開檔試試看, 若無法開檔則無法讀寫, 傳回 -1 */
     /* 可以開檔則傳回 1, 代表不存在但是可以讀寫 */

     if ( ( file_pointer->filehandler = fopen( filename , "a+" ) ) == NULL )
     {
       file_pointer->filehandler = NULL;
       file_pointer->information = -1;
     }

     else
     {
       file_pointer->information  = 1;
     }
   }

   /* 檔案已經存在的情形 */

   else
   {
     /* 若無法讀寫則傳回 -2 , 代表檔案存在但是無法讀寫 */

     if ( access( filename , 6 ) == -1 )
     {
       file_pointer->filehandler = NULL;
       file_pointer->information = -2;
     }
     else
     {
       /* 開檔開開看, 不成功傳回 -3 , 代表檔案存在但是無法開檔 */

       if ( ( file_pointer->filehandler = fopen( filename , "r+" ) ) == NULL )
       {
         file_pointer->filehandler = NULL;
         file_pointer->information = -3;
       }
     }
   }

  if (      file_pointer->information == -1 )
    show_error( " 你的檔案不存在, 且無法開啟.\n" );

  else if ( file_pointer->information == -2 )
    show_error( " 你的檔案存在但是無法讀寫.\n" );

  else if ( file_pointer->information == -3 )
    show_error( " 你的檔案存在, 但是無法開啟.\n" );

  else if ( file_pointer->information == -4 )
    show_error( " 你的檔案名稱過短, 系統不承認.\n" );

  clear_screen();

  return file_pointer;
}


#undef FILE_X_POS
#undef FILE_Y_POS
#undef PRINT_LEN
#undef FILE_PER_COL
#undef MAX_LINE

/* 測試是否到達檔案底端 */
int fread_if_eof( FILE_INFO *pFile )
{
  char character;

  do
  {
    if ( ( character = getc( pFile->filehandler ) ) == EOF ) return TRUE;
  }
  while ( character == ' ' || character == '\n' || character == '\r' );

  return FALSE;
}


/* 測試是否達到檔案這一行的底端 */
int fread_if_eol( FILE_INFO *pFile )
{
  char character;

  do
  {
    character = getc( pFile->filehandler );
  }
  while ( character == ' ' );

  if ( character == '\n' || character == '\r' )
  {
    ungetc( character , pFile->filehandler );
    return TRUE;
  }

  ungetc( character , pFile->filehandler );
  return FALSE;
}

/* 讀完整行 */
void fread_to_eol( FILE_INFO * pFile )
{

  char c;

  do
  {
    c = getc( pFile->filehandler );
  }
  while ( c != '\n' && c != '\r' );

  do
  {
    c = getc( pFile->filehandler );
  }
  while ( c == '\n' || c == '\r' );

  ungetc( c, pFile->filehandler );
  return;

}

/* 從檔案中讀取一個單字 */
void fread_word( FILE_INFO * pFile , char * address )
{
    char *pword;
    char character;
    int  len;

    /* 先把前面的空白讀完 */

    do
    {
      if ( ( character  = getc( pFile->filehandler ) ) == EOF )
      {
        pFile->status = 1;
        return;
      }
    }
    while ( isSpace( character ) );
    ungetc( character , pFile->filehandler );

    for ( pword = address , len = 0; len < MAX_WORD_LENGTH ; pword++ , len++)
    {

      if ( ( *pword = getc( pFile->filehandler ) ) == EOF )
      {
        pFile->status = 1;
        return;
      }

      if ( isSpace( *pword ) )
      {
        ungetc( *pword , pFile->filehandler );
        *pword        = '\x0';
        return;
      }
    }

    pFile->status = 1;
    return;
}

/* 從檔案讀取一個數字 */

int fread_number( FILE_INFO *pFile )
{
  int  number;
  int  sign;
  char c;

  do
  {
    if ( ( c = getc( pFile->filehandler ) ) == EOF )
    {
      pFile->status = 1;
      return -1;
    }
  }
  while ( isSpace( c ) );

  number = 0;
  sign   = FALSE;

  if ( c == '+' || c == '|' )
  {
     if ( ( c = getc( pFile->filehandler ) ) == EOF )
     {
       pFile->status = 1;
       return -1;
     }
  }

  else if ( c == '-' )
  {
    sign = TRUE;
    if ( ( c = getc( pFile->filehandler ) ) == EOF )
    {
      pFile->status = 1;
      return -1;
    }
  }

  if ( !isdigit( c ) )
  {
    ungetc( c , pFile->filehandler );
    if ( !isalpha( c ) )
    {
      pFile->status = 1;
      return -1;
    }

    if ( symbol_lookup( fread_alpha( pFile )
       , &number) == FALSE)
    {
      pFile->status = 1;
      return -1;
    }

    if ( ( c = getc( pFile->filehandler ) ) == EOF )
    {
      pFile->status = 1;
      return -1;
    }

    ungetc( c, pFile->filehandler );

    if ( c != '+' && c != '-' && c != '|' ) return number;
    else return ( number | fread_number( pFile ) );
  }

  while ( isdigit( c ) )
  {
    number = number * 10 + c - '0';

    if ( ( c  = getc( pFile->filehandler ) ) == EOF )
    {
      pFile->status = 1;
      return -1;
    }
  }

  if ( sign != 0 ) number = 0 - number;

       if ( c == '|' ) number = number | fread_number( pFile );
  else if ( c != ' ' ) ungetc( c, pFile->filehandler );

  return number;
}

/* 讀取字串常數專用的函數 */

char *fread_alpha( FILE_INFO * pFile)
{
  static char word[ MAX_WORD_LENGTH ];
  char   *pword;
  char   cEnd;
  int    len;

  do
  {
    if ( ( cEnd = getc( pFile->filehandler ) ) == EOF )
    {
      pFile->status = 1;
      return NULL;
    }
  }
  while ( isSpace( cEnd ) );

  ungetc( cEnd , pFile->filehandler );
  pword = word;

  for ( len = 0 , pword = word ; len < MAX_WORD_LENGTH; pword++ )
  {
    if ( ( *pword = getc( pFile->filehandler ) ) == EOF )
    {
      pFile->status = 1;
      return NULL;
    }

    if (  *pword == ' ' || *pword == '|' || *pword == '\r'
      || *pword == '\n' || *pword == '-' || *pword == '+' )
    {
      ungetc( *pword , pFile->filehandler );
      *pword = '\x0';
      return word;
    }
  }

  pFile->status = 1;
  return NULL;
}

/* 檢查字串常數表 */
int  symbol_lookup( char * symbol, int * num )
{
  int loop;

  if ( symbol == '\x0' || !isalpha( symbol[0] ) ) return FALSE;

  for ( loop = 0 ; const_table[loop].string[0] != '\x0' ; loop++ )
  {
    if ( !strcmp( const_table[loop].string , symbol ) )
    {
       *num = const_table[loop].value;
       return TRUE;
    }
  }

  return FALSE;
}

/* 從檔案中讀取一個字串 */
void fread_string( FILE_INFO *pFile , STRING_INFO * pString )
{

  char   character;
  int    cursor_x;
  int    cursor_y;
  int    fChinese;
  int    fLast;

  /* 先清除目的的字串 */
  clear_string( pString );

  /* 先把空白讀取 */
  do
  {
    if ( ( character = getc( pFile->filehandler ) ) == EOF )
    {
      pFile->status = 1;
      return;
    }

  } while ( isSpace( character ) );

  ungetc( character , pFile->filehandler );

  cursor_x = 0;
  cursor_y = 0;
  fChinese = FALSE;
  fLast    = FALSE;

  for ( ; ; )
  {
    fLast = fChinese;

    /* 讀取字元到辨識字元裡 */
    character = getc( pFile->filehandler );

    /* 是不是中文字 */
    if ( fChinese )
    {
      fChinese = FALSE;
    }

    else
    {
      if ( character < 0 )  fChinese = TRUE;
      else                  fChinese = FALSE;
    }

    switch ( character )
    {

      default :

        if ( ( ++cursor_x ) > pString->x_len )
        {
          pFile->status = 1;
          return;
        }

        /* 讀取一個字元到特定的位址 */
        *( pString->address + ( cursor_y * ( pString->x_len + 1 ) )
          + cursor_x - 1 ) = character;

        break;

      case EOF:

        pFile->status = 1;
        return;

      case '\r':

        break;

      case '\n':

        if ( ++cursor_y > pString->y_len )
        {
          pFile->status = 1;
          return;
        }

        cursor_x = 0;
        break;

      case '~' :

        if ( fLast )
        {
          if ( ( ++cursor_x ) > pString->x_len )
          {
            pFile->status = 1;
            return;
          }

          /* 讀取一個字元到特定的位址 */
          *( pString->address + ( cursor_y * ( pString->x_len + 1 ) )
            + cursor_x - 1 ) = character;

          break;
        }

        return;
    }
  }
}
