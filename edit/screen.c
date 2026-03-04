#include "edit.h"


/* 定位且有輸出有顏色的字串

    x       ----> 字串的 x 座標
    y       ----> 字串的 y 座標
    color   ----> 字串的顏色碼
    string  ----> 字串的位址
*/

void print_string( int x , int  y , int color , char * string )
{

  #if defined (MSDOS)

  locate_cursor( x , y );
  change_color( color );
  cprintf( string );

  #endif

  #if defined (unix)
  {
    int reverse;
    int bold;

    reverse = ( color > 15 ) ? 1 : 0;
    bold   = ( ( color & 15 ) > 8 ) ? 1 : 0;
    if ( reverse == 1 ) attron( A_REVERSE );
    if ( bold    == 1 ) attron( A_BOLD    );

    mvaddstr( y , x , string );

    if ( reverse == 1 ) attroff( A_REVERSE );
    if ( bold    == 1 ) attroff( A_BOLD    );

  }

  #endif

  return;
}


/* 定位且有輸出有顏色的有長度字串, 長度不滿的填入空白

    x       ----> 字串的 x 座標
    y       ----> 字串的 y 座標
    color   ----> 字串的顏色碼
    string  ----> 字串的位址
    len     ----> 字串的長度
*/

void show_one_string( int x , int y , int len , char * address , int color )
{
  int loop;
  char buffer[ 82 ];

  /* 若是長度為零, 則設定長度 */
  if  ( len == 0 ) len = strlen( address );

  /* 先把字串設定為空白列 */
  for ( loop = 0 ; loop < 82 ; loop++ ) buffer[ loop ] = ' ';

  /* 拷貝字串 */
  for ( loop = 0 ; loop <= len ; loop++ )
  {
    if ( *(address + loop ) != '\x0' ) buffer[loop] = *(address+loop);
    else break;
  }

  buffer[ len ] = '\x0';
  print_string( x , y , color , buffer );
  return;

}

void print_number( int len , int number , STRING_INFO * pString )
{

  clear_string( pString );
  number_to_string( number , pString->address );
  align_word( pString->address , len );
  print_long_string( 1 , pString );

  return;
}

void print_long_string( int mode , STRING_INFO * pString )
{
  int loop;

  for ( loop = 0 ; loop < pString->y_len ; loop++ )
    show_one_string( pString->x_pos , pString->y_pos + loop
    , pString->x_len
    , pString->address + ( loop * ( pString->x_len + 1 ) )
    , (   mode == 0 ? pString->front_color : pString->org_front_color )
    + ( ( mode == 0 ? pString->back_color  : pString->org_back_color  )
      << 4  ) );

  return;

}

void show_error( char * string )
{
  clear_screen();
  set_cursor_type( NO_CURSOR );
  print_string( 25 , 12 , LIGHTRED , string );
  get_control_key();
  return;
}
