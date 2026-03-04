#include "edit.h"

#define DEFAULT_X       8
#define DEFAULT_Y       30
#define FRONT_COLOR     ( WHITE )
#define BACK_COLOR      ( WHITE + ( RED << 4 ) )

void        main_shell              ( void );
void        print_main_shell        ( void );

/* 設定選項函數的位址 */
static void ( *task[] ) () =
{
  do_edit_area ,
  do_edit_room ,
  do_edit_shop ,
  do_link      ,
  do_exit
};

/* 設定選單的項目 */

char const * choice_title[ MAX_CHOICE ] =
{
 " 1. 編輯區域  ",
 " 2. 編輯房間  ",
 " 3. 編輯商店  ",
 " 4. 連結檔案  ",
 " 5. 結束編輯  "
};

int          now_choice  = 0;

void print_main_shell           ( void );

int main( void )
{

#if defined (unix)

  /* 起始化 curses.h */
  initscr();
  refresh();

#endif

  initial_room();

  main_shell();

#if defined (unix )
  endwin();
#endif

  return 1;
}

void main_shell()
{

  int main_control;
  int memory_choice;

  print_main_shell();
  print_string( DEFAULT_Y , DEFAULT_X + now_choice , BACK_COLOR
    , ( char * ) choice_title[now_choice] );

  for ( ; ; )
  {
    set_cursor_type( NO_CURSOR );
    main_control   = get_control_key();
    memory_choice  = now_choice;

    if ( main_control == RETURN_UP    ) now_choice--;
    if ( main_control == RETURN_DOWN  ) now_choice++;
    if ( main_control == RETURN_TAB   ) now_choice++;
    if ( main_control == RETURN_HOME  ) now_choice = 0;
    if ( main_control == RETURN_END   ) now_choice = MAX_CHOICE - 1;

    /* 檢查是否超出範圍 */
    check_boundary( 0 , MAX_CHOICE - 1 , &now_choice );

    if ( memory_choice != now_choice )
    {
      print_string( DEFAULT_Y , DEFAULT_X + memory_choice , FRONT_COLOR
        , ( char * ) choice_title[ memory_choice ] );

      print_string( DEFAULT_Y ,DEFAULT_X + now_choice     , BACK_COLOR
        , ( char * ) choice_title[ now_choice    ] );

    }

    if ( main_control == RETURN_RETURN || main_control == RETURN_SPACE )
    {
      ( *task[now_choice] ) ();
      print_main_shell();
      print_string( DEFAULT_Y , DEFAULT_X+now_choice , BACK_COLOR
        , ( char * ) choice_title[now_choice] );

    }
  }
}

void print_main_shell()
{

  int x = DEFAULT_X ;
  int y = DEFAULT_Y ;
  int loop;

  clear_screen();

  print_string( DEFAULT_Y - 3 , DEFAULT_X - 2 , FRONT_COLOR
    , "MUD 區域產生器 V1.00" );

  for ( loop = 0; loop < MAX_CHOICE; loop++ )
    print_string( y, x++ , FRONT_COLOR , ( char * ) choice_title[loop] );

  return;

}

#undef DEFAULT_X
#undef DEFAULT_Y
#undef FRONT_COLOR
#undef BACK_COLOR

void check_boundary( int up , int down , int * number )
{

  if ( *number < up   ) *number = down;
  if ( *number > down ) *number = up;
  return;

}
