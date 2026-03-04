#include "edit.h"

/* 清除螢幕. */
void clear_screen( void )
{
  #if defined(MSDOS)

  asm mov ax,0003h
  asm int 10h

  #endif

  #if defined(unix)
  clear();
  #endif

  return;
}

/* 顯示插入模式的游標模式 */
void show_cursor( int x , int y , int cursor_mode )
{
  if ( cursor_mode == 0 ) set_cursor_type( NORMAL_CURSOR  );
  else                    set_cursor_type( INSERT_CURSOR  );

  locate_cursor( x , y );
  return;
}

/* 顯示游標的外型

NO_CURSOR      沒有游標
NORMAL_CURSOR  正常游標
INSERT_CURSOR  插入模式的游標

*/

void set_cursor_type( int cursor_type )
{
  #if defined (MSDOS)
  _setcursortype( cursor_type );
  #endif

  #if defined (unix)

  #endif
  return;
}

/* 定位游標 */

void locate_cursor( int x , int y )
{
#if defined (MSDOS)

  gotoxy( x , y );
  return;

#endif

#if defined (unix)

  move( y , x );
  refresh();
  return;

#endif

}

/* 儲存視窗的文字資料, 不相容. */
void save_windows( int left , int top , int right
                   , int bottom , char * string )
{

  #if defined (MSDOS)
  gettext( left , top , right , bottom , string );
  #endif

  return;
}

/* 回存視窗的文字資料, 不相容. */
void restore_windows( int left , int top , int right
                      , int bottom , char * string )
{
  #if defined (MSDOS)
  puttext( left , top , right , bottom , string );
  #endif

  return;
}

/* 低階的接收一個字元, 不相容.*/
#if defined (MSDOS)
unsigned char get_key( void )
{
  return getch();
}
#endif

#if defined (unix)
unsigned char get_key( void )
{
  struct termio save, term;
  char   key ;

  if ( ioctl( 0 , TCGETA , &term ) == -1 )
  {
    fprintf( stderr, "Standard input not a tty.\n" );
    exit( 1 );
  }

  save              =    term;
  term.c_lflag     &= ~ICANON;
  term.c_lflag     &=   ~ECHO;
  term.c_cc[VMIN]   =       1;
  term.c_cc[VTIME]  =       0;

  ioctl( 0 , TCSETA , &term );
  read(  0 , &key   , 1     );
  ioctl( 0 , TCSETA , &save );

  return( ( unsigned ) key);
}

#endif

/* 改變輸出字串的顏色 */

void change_color( int color )
{

  #if defined (MSDOS)
  textattr( color );
  #endif

  return;
}

/* 使終端機發出警告聲音 */
void warning_beep( void )
{
  printf( "\x07" );
  return;
}
