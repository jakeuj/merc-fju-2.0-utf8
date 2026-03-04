#include "edit.h"

void    get_help        ( void );
void    show_insert     ( int  );
void    show_help       ( void );
void    clear_help      ( void );
void    condense_string ( STRING_INFO * );

#define REAL_ADDRESS    ( ( string->address ) + ( Y_SHIFT ) + ( cursor_x ) )
#define Y_SHIFT         ( cursor_y ) * ( string->x_len + 1 )

/* 求助的字串 */

char * const help_message = "^E 清除至行末 ^K 刪除 ^N 連結 ^P 清除"
                            "PgDn 行末 Enter 換行 TAB 完成輸入";

/* 視窗式輸入一個字串, 需要傳入一個結構如下

          struct string_info
          {
            int    x_pos            --------> 字串一開始的 X 座標
            int    y_pos;           --------> 字串一開始的 Y 座標
            int    x_len;           --------> 字串可以有幾行
            int    y_len;           --------> 字串可以有幾列
            int    front_color;     --------> 字串的前景顏色
            int    back_color;      --------> 字串的背景顏色
            int    save_x_cursor;   --------> 上一次 x 座標的位置
            int    save_y_cursor;   --------> 上一次 y 座標的位置
            int    org_front_color; --------> 字串原始的前景顏色
            int    org_back_color;  --------> 字串原始的背景顏色
            int    smash;           --------> 字串是否可以有~符號
            int    number;          --------> 字串是否為數字字串
            char * address;         --------> 字串的位址
          }
*/

void get_string( STRING_INFO * string )
{
  int    loop;
  int    cursor_x;
  int    cursor_y;
  int    insert_mode;
  int    pic_change;
  char   temp_address[82];
  char   input;
  char * tempaddress;

  /* 設定 x ,y 為上一次的座標 */
  cursor_x    = string->save_x_cursor;
  cursor_y    = string->save_y_cursor;

  /* 設定一些基本的參數 */
  insert_mode = 0;
  pic_change  = 1;

  /* 顯示求助畫面 */
  show_help();

  while ( TRUE )
  {

    /* 如果畫面有改變, 則把視窗畫面繪出 */

    if ( pic_change == 1 ) print_long_string( 0, string );

    /* 顯示 insert 的狀態 */
    show_insert( insert_mode );

    /* 顯示游標的所在 */
    show_cursor( string->x_pos + cursor_x , string->y_pos + cursor_y
      , insert_mode );

    /* 內定畫面會有變動 */
    pic_change = 1;

    switch( input = get_key() )
    {
      case MY_CURSOR_HEAD :

        #if defined (unix)
        get_key();
        #endif

        switch ( input = get_key() )
        {

         /* 將游標回到行首 */
         case MY_CURSOR_HOME  :

           /* 畫面不會更動 */
           pic_change = 0;

           #if defined (unix)
           get_key();
           #endif

           cursor_x = 0;
           break;

         /* 將游標移到本行字串的末端 */

         case MY_CURSOR_END   :

           /* 畫面不會更動 */
           pic_change = 0;

           #if defined (unix)
           get_key();
           #endif

           for ( cursor_x = string->x_len;
             *(REAL_ADDRESS -1 ) == '\x0' && cursor_x > 0 ; cursor_x-- );
           if ( cursor_x == string->x_len ) cursor_x--;

           break;

         /* 將游標向右移一格 */
         case MY_CURSOR_RIGHT :

           /* 畫面不會更動 */
           pic_change = 0;

           if ( ++cursor_x == string->x_len )
           {
             if ( ++cursor_y == string->y_len )
             {
               cursor_y--;
               cursor_x--;
             }

             else
             {
               cursor_x = 0;
             }
           }
           break;

         /* 把游標向左移一格 */
         case MY_CURSOR_LEFT  :

           /* 畫面不會更動 */
           pic_change = 0;

           if ( --cursor_x < 0 )
           {
             if ( --cursor_y >= 0 )
             {
               cursor_x = string->x_len - 1;
             }
             else
             {
               cursor_x++;
               cursor_y++;
             }
           }

           break;

         /* 把游標向上移一格 */

         case MY_CURSOR_UP     :

           /* 畫面不會更動 */
           pic_change = 0;

           if ( cursor_y > 0 ) cursor_y--;
           break;

         /* 把游標向下移一格 */

         case MY_CURSOR_DOWN   :

           /* 畫面不會更動 */
           pic_change = 0;

           if ( ++cursor_y == string->y_len ) cursor_y--;
           break;

         /* 切換插入模式 */

         case MY_CURSOR_INSERT :

           /* 畫面不會更動 */
           pic_change = 0;

           #if defined (unix)
           get_key();
           #endif

           insert_mode = ( ++insert_mode ) & 1;
           break;

         /* 刪除一個字元 */

         case MY_CURSOR_DELETE :

           #if defined (unix)
           get_key();
           #endif

           *REAL_ADDRESS = '\x0';
           tempaddress = REAL_ADDRESS;

           do
           {
             *tempaddress = *(tempaddress+1);

           } while ( *(++tempaddress) != '\x0' );

            *(tempaddress+1) = '\x0';

          break;

         /* 將游標移到本行的末端 */

         case MY_CURSOR_PGDN   :

           /* 畫面不會更動 */
           pic_change = 0;

           #if defined (unix)
           get_key();
           #endif

           cursor_x = string->x_len - 1;
           break;

         /* 顯示求助畫面 , 只在 DOS 有用 */
         case MY_CURSOR_F1     :

           /* 畫面不會更動 */
           pic_change = 0;

           get_help();
           break;

         case MY_CURSOR_PGUP   :

           /* 畫面不會更動 */
           pic_change = 0;

           #if defined (unix)
           get_key();
           #endif

           break;

         default               :

           /* 畫面不會更動 */
           pic_change = 0;

           break;
        }

        break;

      /* 往下移一行, 若只有一行則完成輸入 */
      case MY_KEY_RETURN :

        {
          /* 若只有一行則完成輸入 */
          if ( string->y_len == 1 )
          {

           /* 記錄這一次最後的 x , y 座標 */
            string->save_x_cursor = cursor_x;
            string->save_y_cursor = cursor_y;

            /* 把字串後面無用的空白消掉 */
            condense_string( string );

            /* 如果字串不能有 ~ 符號則把 ~ 變成 - */
            if ( string->smash == SMASH_EXECUTE )
              smash_string( string );

            /* 清除求助畫面 */
            clear_help();

            /* 將游標消除 */
            set_cursor_type( NO_CURSOR );

            /* 將字串還原成原來的顏色 */
            print_long_string( 1 , string );

            return;
           }

          /* 若在最底一行, 則不處理 */
          if ( cursor_y == string->y_len - 1 ) break;

          /* 往下移一行 */
          tempaddress = string->address
            + ( ( string->y_len - 1 ) * ( string->x_len + 1 ) );

          if ( *tempaddress != '\x0' )
          {
            cursor_y++;
            cursor_x = 0;
            break;
          }

          for ( loop = string->y_len - 1 ; loop > cursor_y ; loop-- )
          {
            int i;

            tempaddress = string->address
              + ( loop * ( string->x_len + 1 ) );

            for ( i = 0 ; i < string->x_len ; i++ )
              *(tempaddress + i ) = '\x0';

            if ( loop != cursor_y + 1 )
            strcpy( tempaddress , tempaddress - ( string->x_len + 1 ) );
          }

          tempaddress = string->address
            + ( (cursor_y + 1 ) * ( string->x_len + 1 ) );

          while ( *REAL_ADDRESS != '\x0' )
          {
            *tempaddress++ = *REAL_ADDRESS;
            *REAL_ADDRESS  = '\x0';
            cursor_x++;
          }

          cursor_x = 0;
          cursor_y++;

          break;
        }

      /* 完成輸入 */

      case MY_KEY_TAB :

        /* 記錄這一次最後的 x , y 座標 */
        string->save_x_cursor = cursor_x;
        string->save_y_cursor = cursor_y;

        /* 把字串後面無用的空白消掉 */
        condense_string( string );

        /* 如果字串不能有 ~ 符號則把 ~ 變成 - */
        if ( string->smash == 1 ) smash_string( string );

        /* 清除求助畫面 */
        clear_help();

        /* 將游標消除 */
        set_cursor_type( NO_CURSOR );

        /* 將字串還原成原來的顏色 */
        print_long_string( 1 , string );

        return;

      /* 刪除一行 */

      case MY_KEY_CTRL_K :

        {
          int temp_cursor_y;
          int i;

          cursor_x      = 0;
          temp_cursor_y = cursor_y;

          for ( ; cursor_y < string->y_len ; cursor_y++ )
          {

            for ( i = 0; i < string->x_len ; i++ )
              *(REAL_ADDRESS + i ) = '\0';

            if ( cursor_y != string->y_len - 1 )
              strcpy( REAL_ADDRESS , REAL_ADDRESS + string->x_len + 1 );

          }

          cursor_x = 0;
          cursor_y = temp_cursor_y;

        }

        break;

      /* 清除一行 */

      case MY_KEY_CTRL_P :

        cursor_x = 0;
        for ( ; cursor_x < string->x_len ; cursor_x++ )
          *REAL_ADDRESS = '\x0';
        cursor_x = 0;
        break;

      /* 刪除至行末 */

      case MY_KEY_CTRL_E :

        {
          int i;

          i = cursor_x;

          for ( ; cursor_x < string->x_len ; cursor_x++ )
            *REAL_ADDRESS = '\0';

          cursor_x = i;
          break;

        }

      /* 連結兩行 */

      case MY_KEY_CTRL_N :

        {
          int temp_cursor_x;
          int temp_cursor_y;
          int i;

          if ( cursor_y == string->y_len - 1 ) break;
          tempaddress = string->address
              + ( ( cursor_y + 1 ) * ( string->x_len + 1 ) );

          temp_cursor_x = cursor_x;
          cursor_x      = 0;

          if ( strlen( tempaddress ) + strlen( REAL_ADDRESS )
                 > string->x_len )
          {
            warning_beep();
            cursor_x = temp_cursor_x;
            break;
          }

          strcat( REAL_ADDRESS , tempaddress );

          cursor_x      = 0;
          temp_cursor_y = cursor_y;
          cursor_y++;

          for ( ; cursor_y < string->y_len ; cursor_y++ )
          {

            for ( i = 0; i < string->x_len ; i++ )
              *(REAL_ADDRESS + i ) = '\0';

            if ( cursor_y != string->y_len - 1 )
              strcpy( REAL_ADDRESS , REAL_ADDRESS + string->x_len + 1 );

          }

          cursor_y = temp_cursor_y;
          cursor_x = temp_cursor_x;

          break;
        }

      /* 倒退移除一個字元 */
      case MY_KEY_BACKSPACE :

        {

          int temp_cursor_y;
          int i;

          if ( cursor_x == 0 )
          {
            if ( cursor_y == 0 )
            {
              warning_beep();
              break;
            }

            tempaddress = string->address
              + ( ( cursor_y - 1 ) * ( string->x_len + 1 ) );

            if ( strlen( tempaddress ) + strlen( REAL_ADDRESS )
                 > string->x_len )
            {
              warning_beep();
              break;
            }

            strcat( tempaddress , REAL_ADDRESS );

            cursor_x      = 0;
            temp_cursor_y = cursor_y;

            for ( ; cursor_y < string->y_len ; cursor_y++ )
            {

              for ( i = 0; i < string->x_len ; i++ )
                *(REAL_ADDRESS + i ) = '\0';

              if ( cursor_y != string->y_len - 1 )
                strcpy( REAL_ADDRESS , REAL_ADDRESS + string->x_len + 1 );

            }

            cursor_y = temp_cursor_y;
            cursor_x = strlen( tempaddress );
            if ( cursor_x == string->x_len ) cursor_x--;
            cursor_y--;
            break;
          }

          cursor_x--;
          *REAL_ADDRESS = '\0';
            tempaddress = REAL_ADDRESS;

          do
          {
            *tempaddress = *(tempaddress+1);

          } while ( *(tempaddress++) != '\0' );

          break;
        }

      default:

        /* 如果 number 設定為1 , 那只能輸入數字 */
        if ( ( !isdigit( input ) && input != '-' )
             && string->number == NUMBER_ONLY )
        {
          warning_beep();
          break;
        }

        /* 判定是否是正常字元或是中文字元 */

        if ( !iscntrl(input) || IsChinese( input ) )
        {
          loop = cursor_x ;
          for ( ; cursor_x >= 0 ; cursor_x-- )
            if ( *REAL_ADDRESS == '\x0' ) *REAL_ADDRESS = ' ';

          cursor_x = loop;

          if ( insert_mode == 0 )
          {
            *REAL_ADDRESS = input;
            if ( ++cursor_x == string->x_len ) cursor_x--;
          }

          else
          {

            tempaddress = Y_SHIFT + string->address;

            if ( strlen( tempaddress ) >= string->x_len )
            {
              warning_beep();
            }

            else
            {

              strcpy( temp_address , REAL_ADDRESS );
              *REAL_ADDRESS     = input;
              cursor_x++;
              *REAL_ADDRESS     = '\x0';
              strcat( REAL_ADDRESS , temp_address );
              if ( cursor_x == string->x_len ) cursor_x--;

            }
          }
        }
      break;
    }
  }
}

/* 輸入字串的求助畫面 */

void get_help( void )
{

  char buffer[ 80 * 25 ];
  int  color;

  save_windows( 19, 10 , 60 , 19 , buffer );

  color = LIGHTGREEN + ( BLUE << 4 );
  set_cursor_type( NO_CURSOR );

  print_string( 19 , 10 , color , " ┌──────────────────┐ " );
  print_string( 19 , 11 , color , " │ctrl+P  清除整行.   <HOME> 移到行首 │ " );
  print_string( 19 , 12 , color , " │ctrl+K  刪除整行.   <END>  移到文末 │ " );
  print_string( 19 , 13 , color , " │ctrl+E  清除至行末. <PgDw> 移到行末 │ " );
  print_string( 19 , 14 , color , " │ctrl+N  連結兩列    <Curs> 上下左右 │ " );
  print_string( 19 , 15 , color , " │<F1>    求助        <Back> 倒退刪除 │ " );
  print_string( 19 , 16 , color , " │<TAB>   完成輸入    <Ins>  插入模式 │ " );
  print_string( 19 , 17 , color , " │<ENTER> 換行        <Del>  刪除文字 │ " );
  print_string( 19 , 18 , color , " │           * 按任何鍵繼續 *         │ " );
  print_string( 19 , 19 , color , " └──────────────────┘ " );

  get_control_key();

  restore_windows( 19, 10, 60, 19, buffer );

}

/* 得到一些特殊的按鍵, 傳回值 如下

傳回值 游標上   RETURN_UP        1
傳回值 游標下   RETURN_DOWN      2
傳回值 游標左   RETURN_LEFT      3
傳回值 游標右   RETURN_RIGHT     4
傳回值 游標HOME RETURN_HOME      5
傳回值 游標END  RETURN_END       6
傳回值 RETURN   RETURN_RETURN   10
傳回值 TAB      RETURN_TAB      11
傳回值 SPACE    RETURN_SPACE    12
傳回值 PGDN     RETURN_PGDN     13
傳回值 PGUP     RETURN_PGUP     14
其他的按鍵      RETURN_NONE      0

*/

int get_control_key( void )
{
  char control_key;

  locate_cursor( IDLE_X_CURSOR , IDLE_Y_CURSOR );

  control_key = get_key();
  switch ( control_key )
  {
    case MY_CURSOR_HEAD :

      #if defined (unix)
      get_key();
      #endif

      control_key = get_key();
      switch ( control_key )
      {
        case MY_CURSOR_UP     : return RETURN_UP   ;
        case MY_CURSOR_DOWN   : return RETURN_DOWN ;
        case MY_CURSOR_LEFT   : return RETURN_LEFT ;
        case MY_CURSOR_RIGHT  : return RETURN_RIGHT;

        case MY_CURSOR_HOME   :

          #if defined (unix)
          get_key();
          #endif
          return RETURN_HOME ;

        case MY_CURSOR_END    :

          #if defined (unix)
          get_key();
          #endif
          return RETURN_END ;

        case MY_CURSOR_PGDN   :

          #if defined (unix)
          get_key();
          #endif
          return RETURN_PGDN ;

        case MY_CURSOR_PGUP   :

          #if defined (unix)
          get_key();
          #endif
          return RETURN_PGUP ;

        #if defined (unix)

        case MY_CURSOR_INSERT :
        case MY_CURSOR_DELETE :

          get_key();
          return RETURN_NONE;

        #endif

        default              : return RETURN_NONE ;
      }

    case MY_KEY_SPACE        : return RETURN_SPACE;
    case MY_KEY_RETURN       : return RETURN_RETURN;
    case MY_KEY_TAB          : return RETURN_TAB;
    default                  :
    return RETURN_NONE;
  }

}

/* 顯示插入模式的狀態 */

void show_insert( int insert_mode )
{

  if ( insert_mode == 0 )
    print_string( 75 , 23 , WHITE , "正常" );

  else
    print_string( 75 , 23 , WHITE , "插入" );

  return;

}

/* 顯示輸入的求助的畫面 */
void show_help( void )
{
  print_string( 1 , 23 , WHITE , help_message );
  return;
}

/* 清除輸入的求助畫面 */
void clear_help( void )
{

  int   loop;
  char  blank_message[ 82 ];

  for ( loop = 0 ; loop < strlen( help_message ) ; loop++ )
    blank_message[loop] = ' ';

  blank_message[loop] = '\x0';
  print_string( 1 , 23 , WHITE , blank_message );
  return;

}

/* 把字串後面無用的空白消掉 */

void condense_string( STRING_INFO * string )
{
  int cursor_x;
  int cursor_y;

  for ( cursor_y = 0; cursor_y < string->y_len ; cursor_y++ )
  {
    for ( cursor_x = string->x_len - 1 ; cursor_x >= 0 ; cursor_x-- )
    {
      if ( *REAL_ADDRESS == '\x0' ) continue;
      if ( *REAL_ADDRESS == ' '   ) *REAL_ADDRESS = '\x0';
      else                          cursor_x      = 0;
    }
  }

  return;
}

/* 清除字串裡面含有 ~ 的字元 */
void smash_string( STRING_INFO *pString )
{
  int    loop;
  int    fChinese = FALSE;
  int    fLast;
  char * string;

  for ( loop = 0; loop < pString->y_len ; loop++ )
  {
    string = pString->address + ( loop * ( pString->x_len + 1 ) );
    for ( ; *string != '\0'; string++ )
    {
      fLast = fChinese;

      if ( fChinese )
      {
        fChinese = FALSE;
      }

      else
      {
        if ( *string < 0 ) fChinese = TRUE;
        else               fChinese = FALSE;
      }

      if ( *string == '~' && !fLast) *string = '-';
    }

    if ( fChinese ) *--string = '\x0';
  }

  return;
}

void clear_string( STRING_INFO * pString )
{
  int i;
  int j;

  for ( i = 0; i < pString->y_len ; i++ )
  {
    for ( j = 0; j < pString->x_len ; j++ )
    {
      /* 清除每一個字元 */
      *( pString->address + ( i * ( pString->x_len + 1 ) ) + j ) = '\x0';
    }

    /* 每一行字尾設定為 0 */
    *( pString->address + ( i * ( pString->x_len + 1 ) ) + j ) = '\x0';

  }

  return;
}

void number_to_string( int number , char * string )
{

  char   temp[ 20 ];
  int    len    = 0;
  int    sign   = 0;

  if ( number < 0 )
  {
    number = 0 - number;
    sign = 1;
  }

  do
  {
    temp[ len ] = '0' + number % 10;
    len++;
  } while ( ( number /= 10 ) > 0 );


  if ( sign == 1 ) *( string++ ) = '-';
  len--;

  do
  {
    *(string++) = temp[ len ];
    len--;
  } while ( len >= 0 );

  *string = '\x0';

  return;
}

void align_word( char * address , int len )
{
  int   real_length;
  int   loop;
  char  temp[ 20 ];

  if ( ( real_length = strlen( address ) ) >= len )  return;

  for ( loop = 0; loop < len - real_length; loop++ )  temp[loop] = ' ';
  temp[ loop ] = '\x0';
  strcat( temp , address );
  strcpy( address , temp );

  return;
}

/* 找尋 source 字串裡面是否有 dest 字串 */

int string_compare( char * source , char * dest )
{
  char * string_1;
  char * string_2;

  /* 錯誤的起使值 */
  if ( *dest   == '\x0' ) return FALSE;
  if ( *source == '\x0' ) return FALSE;

  /* 如果在 DOS , 則轉換小寫成大寫 */
  #if defined (MSDOS)

  string_1 = dest;
  while ( *string_1 != '\x0' )
  {
    *string_1 = toupper( *string_1 );
    string_1++;
  }

  #endif

  while ( *source != '\x0' )
  {
    string_1 = source ;
    string_2 = dest   ;

    while ( *string_1 == *string_2 )
    {
      if ( *(++string_2 ) == '\x0' ) return TRUE;
      if ( *(++string_1 ) == '\x0' ) return FALSE;
    }
    source++;
  }

  return FALSE;
}

void write_string( FILE * fp , STRING_INFO * pString )
{

  int    line;
  int    loop;
  char * address;

  for ( line = pString->y_len
    ; (*( pString->address + ( (line-1) * (pString->x_len+1))) == '\x0' )
      && ( line > 1 )
    ; line-- );

  for ( loop = 0 ; loop < line ; loop++)
  {
    address = pString->address + ( loop * ( pString->x_len + 1 ) );

    if ( *address == '.' && *(address+1) == '\x0' )
    {
      fprintf( fp , "\n" );
    }

    else
    {
      if ( loop != line - 1 )
        fprintf( fp , "%s\n"  , address );
      else
        fprintf( fp , "%s"    , address );
    }
  }

  fprintf( fp , "~\n" );

  return;
}

void write_select( FILE * fp , int selection , CONST_STRING respect[] )
{

  int loop;
  int touch;

  loop  = 0;
  touch = 0;

  while ( respect[loop].string[0] != '\x0' )
  {
    if ( respect[loop].value == selection )
    {
      touch = 1;
      fprintf( fp , "%s\n" , respect[loop].string );
    }

    loop++;
  }

  if ( touch == 0 ) fprintf( fp , "0\n" );

  return;
}

void write_bit( FILE * fp , int selection , CONST_STRING respect[] )
{

  int loop;
  int touch = 0;

  for ( loop = 0 ; respect[loop].string[0] != '\x0' ; loop++ )
  {
    if ( IS_SET( selection , respect[loop].value ) )
    {
      if ( touch != 0 )
        fprintf( fp , "|%s" , respect[loop].string );

      else
        fprintf( fp , "%s"  , respect[loop].string );

      touch = 1;

    }
  }

  if ( touch == 0 ) fprintf( fp , "0\n" );
  else              fprintf( fp , "\n"  );

  return;
}

#undef    REAL_ADDRESS
#undef    Y_SHIFT
