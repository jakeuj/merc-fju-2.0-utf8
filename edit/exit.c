#include "edit.h"

/* 整個程式結束 */

void do_exit( void )
{
  clear_screen();
  print_string( 1 , 1 , GREEN , "謝謝你使用 Mud 3838 區域編輯器.\n" );

  #if defined (unix)
  refresh();
  endwin();
  #endif

  exit( 0 );
}
