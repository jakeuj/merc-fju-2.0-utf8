#define MSDOS   1

#if defined (unix)
#undef MSDOS
#endif

#if defined (MSDOS)
#define FILE_LENGTH    8
#endif

#if defined (unix)
#define FILE_LENGTH    12
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>

#if defined(MSDOS)
#include <conio.h>
#endif

#if defined(unix)
#include <termio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ncurses.h>
#endif

#define isSpace( c)             ( isspace( c ) && !IsChinese( c ) )
#define IsChinese( c )          ( c < 0 )
#define IS_SET(flag, bit)       ( ( flag ) &  ( bit ) )
#define SET_BIT(var, bit)       ( ( var ) |=  ( bit ) )
#define REMOVE_BIT(var, bit)    ( ( var ) &= ~( bit ) )

/* 定義系統所需的常數 */

#undef  TRUE
#undef  FALSE
#define TRUE                    1
#define FALSE                   0
#define SMASH_EXECUTE           1
#define SMASH_NO_EXECUTE        0
#define NUMBER_ONLY             1
#define NUMBER_ACCEPT           0
#define MAX_WORD_LENGTH         30
#define MAX_CHOICE              5
#define MAX_ITEM                20
#define MAX_TRADE               5
#define LOCAL_DIRECTORY         "./"
#define IDLE_X_CURSOR           1
#define IDLE_Y_CURSOR           23

/* 定義游標鍵 */

#if defined (MSDOS)
#define MY_CURSOR_HEAD          '\x00'
#define MY_CURSOR_F1            '\x3B'
#define MY_CURSOR_HOME          '\x47'
#define MY_CURSOR_UP            '\x48'
#define MY_CURSOR_LEFT          '\x4B'
#define MY_CURSOR_RIGHT         '\x4D'
#define MY_CURSOR_END           '\x4F'
#define MY_CURSOR_DOWN          '\x50'
#define MY_CURSOR_PGDN          '\x51'
#define MY_CURSOR_PGUP          '\x4E'
#define MY_CURSOR_INSERT        '\x52'
#define MY_CURSOR_DELETE        '\x53'
#endif

#if defined (unix)
#define MY_CURSOR_HEAD          '\x1B'
#define MY_CURSOR_F1            '\x3B'
#define MY_CURSOR_HOME          '1'
#define MY_CURSOR_UP            'A'
#define MY_CURSOR_LEFT          'D'
#define MY_CURSOR_RIGHT         'C'
#define MY_CURSOR_END           '4'
#define MY_CURSOR_DOWN          'B'
#define MY_CURSOR_PGDN          '6'
#define MY_CURSOR_PGUP          '5'
#define MY_CURSOR_INSERT        '2'
#define MY_CURSOR_DELETE        '3'
#endif

/* 定義一些按鍵 */

#if defined (MSDOS)
#define MY_KEY_BACKSPACE        '\x08'
#define MY_KEY_TAB              '\x09'
#define MY_KEY_RETURN           '\x0D'
#define MY_KEY_CTRL_E           '\x05'
#define MY_KEY_CTRL_N           '\x0E'
#define MY_KEY_CTRL_K           '\x0B'
#define MY_KEY_CTRL_P           '\x10'
#define MY_KEY_ESC              '\x1B'
#define MY_KEY_SPACE            ' '
#endif

#if defined (unix)
#define MY_KEY_BACKSPACE        '\x7F'
#define MY_KEY_TAB              '\x09'
#define MY_KEY_RETURN           '\x0A'
#define MY_KEY_ESC              '\x1B'
#define MY_KEY_CTRL_E           '\x05'
#define MY_KEY_CTRL_N           '\x0E'
#define MY_KEY_CTRL_K           '\x0B'
#define MY_KEY_CTRL_P           '\x10'
#define MY_KEY_SPACE            ' '
#endif

/* 定義游標的外型 */

#define NO_CURSOR      0
#define NORMAL_CURSOR  2
#define INSERT_CURSOR  1

/* 定義顏色 */
#if defined (MSDOS)
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN      10
#define LIGHTCYAN       11
#define LIGHTRED        12
#define LIGHTMAGENTA    13
#define YELLOW          14
#define WHITE           15
#endif

#if defined (unix)
#define BLACK           0
#define RED             1
#define GREEN           2
#define BROWN           3
#define BLUE            4
#define MAGENTA         5
#define CYAN            6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTRED        9
#define LIGHTGREEN      10
#define YELLOW          11
#define LIGHTBLUE       12
#define LIGHTMAGENTA    13
#define LIGHTCYAN       14
#define WHITE           15
#endif

/* 定義特殊按鍵的傳回值 */

#define RETURN_UP        1
#define RETURN_DOWN      2
#define RETURN_LEFT      3
#define RETURN_RIGHT     4
#define RETURN_HOME      5
#define RETURN_END       6
#define RETURN_RETURN   10
#define RETURN_TAB      11
#define RETURN_SPACE    12
#define RETURN_PGDN     13
#define RETURN_PGUP     14
#define RETURN_NONE      0

#define ROOM_DARK                     1
#define ROOM_NO_MOB                   2
#define ROOM_INDOOR                   4
#define ROOM_PRIVATE                  8
#define ROOM_SAFE                    16
#define ROOM_FOREVER_LIGHT           32
#define ROOM_NO_RECALL               64
#define ROOM_DEPOSIT_MONEY          128
#define ROOM_STOREROOM              256
#define ROOM_NOFIGHT                512
#define ROOM_NOQUIT                1024

typedef struct string_info  STRING_INFO;
typedef struct file_info    FILE_INFO;
typedef struct area_info    AREA_INFO;
typedef struct shop_info    SHOP_INFO;
typedef struct room_info    ROOM_INFO;
typedef struct exit_info    EXIT_INFO;
typedef struct room_desc    ROOM_DESC;
typedef struct const_string CONST_STRING;

struct string_info
{
  int    x_pos;
  int    y_pos;
  int    x_len;
  int    y_len;
  int    front_color;
  int    back_color;
  int    org_front_color;
  int    org_back_color;
  int    save_x_cursor;
  int    save_y_cursor;
  int    smash;
  int    number;
  char * address;
};

struct file_info
{
  FILE * filehandler;
  int    information;
  char   filename[20];
  int    status;
};

struct const_string
{
  char * string;
  int    value;
};

struct item_struct
{
  char * item_name;
  char * item_string_name;
  int    sell;
};

struct area_info
{
  int           valid;
  int           attribution;
  char          editor[ 50 ];
  STRING_INFO * pEditor;
  FILE_INFO   * pFile;
};

struct shop_info
{
  int           valid;
  int           keeper;
  int           object[20];
  int           openhour;
  int           closehour;
  int           buyprofit;
  int           sellprofit;
  int           object_count;
  FILE_INFO   * pFile;
  STRING_INFO   aString;
  STRING_INFO * pString;
};

struct exit_info
{
  int           room;
  char          aRoom[10];
  STRING_INFO * pRoom;
  int           lock;
  int           key;
  char          aKey[10];
  STRING_INFO * pKey;
  char          keyword[ 80 ];
  STRING_INFO * pKeyword;
  char          description[ 80 ];
  STRING_INFO * pDesc;
};

struct room_desc
{
  char          keyword[50];
  STRING_INFO * pKeyword;
  char          description[1000];
  STRING_INFO * pDesc;
};

struct room_info
{
  int           valid;
  FILE_INFO   * pFile;
  int           vnum;
  char          aVnum[10];
  STRING_INFO * pVnum;
  char          name[ 50 ];
  STRING_INFO * pName;
  char          description[ 1000 ];
  STRING_INFO * pDesc;
  int           room_sector;
  int           room_flag;
  EXIT_INFO  *  exit[6];
  ROOM_DESC  *  desc[5];
};

/* MAIN.C */
extern  void        check_boundary          ( int , int , int * );

/* STRING.C */
extern  void        get_string              ( STRING_INFO * );
extern  int         get_control_key         ( void );
extern  void        smash_string            ( STRING_INFO * );
extern  void        clear_string            ( STRING_INFO * );
extern  void        number_to_string        ( int , char * );
extern  void        align_word              ( char * , int );
extern  int         string_compare          ( char * , char * );
extern  void        write_string            ( FILE * , STRING_INFO * );
extern  void        write_select            ( FILE * , int , CONST_STRING [] );
extern  void        write_bit               ( FILE * , int , CONST_STRING [] );

/* FILE.C */
extern  void        delete_null_file        ( FILE_INFO * );
extern  FILE_INFO * get_file_name           ( char * );
extern  int         fread_if_eof            ( FILE_INFO * );
extern  int         fread_if_eol            ( FILE_INFO * );
extern  void        fread_to_eol            ( FILE_INFO * );
extern  void        fread_word              ( FILE_INFO * , char * );
extern  int         fread_number            ( FILE_INFO * );
extern  void        fread_string            ( FILE_INFO * , STRING_INFO * );
extern  FILE_INFO   file_structure;

/* INPUT.C */
extern  void        get_string              ( STRING_INFO * );

/* SCREEN.C */
extern  void        print_string            ( int , int , int , char * );
extern  void        show_one_string         ( int , int , int , char * , int );
extern  void        print_long_string       ( int , STRING_INFO * );
extern  void        print_number            ( int , int , STRING_INFO * );
extern  void        show_error              ( char * );

/* io.c */
extern  void        clear_screen            ( void );
extern  void        show_cursor             ( int , int , int );
extern  void        set_cursor_type         ( int );
extern  void        locate_cursor           ( int , int );
extern  void        save_windows    ( int , int , int , int , char * );
extern  void        restore_windows ( int , int , int , int , char * );
extern  unsigned char   get_key             ( void );
extern  void        change_color            ( int );
extern  void        warning_beep            ( void );

/* const.c */
extern  const   struct  const_string    const_table     [];
extern  const   struct  item_struct     item_type       [ MAX_ITEM ];

/* ini.c */
extern  void    initial_room                ( void );

/* 選單的項目的函數定義 */
extern  void        do_edit_area ( void );
extern  void        do_edit_room ( void );
extern  void        do_edit_shop ( void );
extern  void        do_link      ( void );
extern  void        do_exit      ( void );

extern  int          now_choice;
extern  FILE_INFO    file_structure;
extern  STRING_INFO  pString;

/* ini.c */
extern  ROOM_INFO * pRoom;

/* editroom.c */

#define ROOM_X          1
#define ROOM_Y          1
#define ROOM_FRONT_COL  LIGHTGREEN
#define ROOM_BACK_COL   BLACK
#define ROOM_TAG_COL    RED
#define ROOM_COL        ( ROOM_FRONT_COL + ( ROOM_BACK_COL << 4 ) )
#define ROOM_TAG        ( ROOM_FRONT_COL + ( ROOM_TAG_COL  << 4 ) )

#define EXIT_X          1
#define EXIT_Y          4
#define EXIT_FRONT_COL  LIGHTGREEN
#define EXIT_BACK_COL   BLACK
#define EXIT_TAG_COL    RED
#define EXIT_COL        ( EXIT_FRONT_COL + ( EXIT_BACK_COL << 4 ) )
#define EXIT_TAG        ( EXIT_FRONT_COL + ( EXIT_TAG_COL  << 4 ) )

#define DESC_X          1
#define DESC_Y          4
#define DESC_FRONT_COL  LIGHTGREEN
#define DESC_BACK_COL   BLACK
#define DESC_TAG_COL    RED
#define DESC_COL        ( DESC_FRONT_COL + ( DESC_BACK_COL << 4 ) )
#define DESC_TAG        ( DESC_FRONT_COL + ( DESC_TAG_COL  << 4 ) )
