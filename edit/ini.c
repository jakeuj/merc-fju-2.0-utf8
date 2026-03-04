#include "edit.h"

void    	mem_error		( void );
STRING_INFO  *  malloc_string           ( void );

ROOM_INFO * pRoom;

void initial_room( void )
{

  int 	loop;

  /* 配置房間結構的記憶體 */
  if ( ( pRoom = ( ROOM_INFO * ) malloc( sizeof( ROOM_INFO ) ) ) == NULL )
    mem_error();

  /* 配置房間中房間號碼字串的空間 */
  pRoom->pVnum = malloc_string();

  /* 設定房間號碼字串起啟值 */
  pRoom->pVnum->x_pos           = ROOM_X + 13;
  pRoom->pVnum->y_pos           = ROOM_Y + 3;
  pRoom->pVnum->x_len           = 5;
  pRoom->pVnum->y_len           = 1;
  pRoom->pVnum->front_color     = BLUE;
  pRoom->pVnum->back_color      = LIGHTGRAY;
  pRoom->pVnum->org_front_color = ROOM_COL;
  pRoom->pVnum->org_back_color  = BLACK;
  pRoom->pVnum->smash           = SMASH_EXECUTE;
  pRoom->pVnum->number          = NUMBER_ONLY;
  pRoom->pVnum->address         = pRoom->aVnum;
  clear_string( pRoom->pVnum );

  /* 配置房間中房間名稱字串的空間 */
  pRoom->pName = malloc_string();

  /* 設定房間名稱字串起啟值 */
  pRoom->pName->x_pos           = ROOM_X + 30;
  pRoom->pName->y_pos           = ROOM_Y +  3;
  pRoom->pName->x_len           = 40;
  pRoom->pName->y_len           =  1;
  pRoom->pName->front_color     = BLUE;
  pRoom->pName->back_color      = LIGHTGRAY;
  pRoom->pName->org_front_color = ROOM_COL;
  pRoom->pName->org_back_color  = BLACK;
  pRoom->pName->smash           = SMASH_EXECUTE;
  pRoom->pName->number          = NUMBER_ACCEPT;
  pRoom->pName->address         = pRoom->name;
  clear_string( pRoom->pName );

  /* 配置房間中房間描述字串的空間 */
  pRoom->pDesc = malloc_string();

  /* 設定房間描述字串起啟值 */
  pRoom->pDesc->x_pos           = ROOM_X + 7;
  pRoom->pDesc->y_pos           = ROOM_Y + 5;
  pRoom->pDesc->x_len           = 68;
  pRoom->pDesc->y_len           =  8;
  pRoom->pDesc->front_color     = BLUE;
  pRoom->pDesc->back_color      = LIGHTGRAY;
  pRoom->pDesc->org_front_color = ROOM_COL;
  pRoom->pDesc->org_back_color  = BLACK;
  pRoom->pDesc->smash           = SMASH_EXECUTE;
  pRoom->pDesc->number          = NUMBER_ACCEPT;
  pRoom->pDesc->address         = pRoom->description;
  clear_string( pRoom->pDesc );

  /* 配置房間中六個出口的結構記憶體 */
  for ( loop = 0; loop < 6; loop++ )
  {
    /* 配置每一個出口結構的記憶體 */
    pRoom->exit[loop] = ( EXIT_INFO * ) malloc( sizeof( EXIT_INFO ) );
    if ( pRoom->exit[loop] == NULL ) mem_error();

    /* 配置每一個出口房間的字串空間 */
    pRoom->exit[loop]->pRoom = malloc_string();

    /* 設定房間號碼字串起啟值 */
    pRoom->exit[loop]->pRoom->x_pos           = EXIT_X + 22;
    pRoom->exit[loop]->pRoom->y_pos           = EXIT_Y + 6;
    pRoom->exit[loop]->pRoom->x_len           = 5;
    pRoom->exit[loop]->pRoom->y_len           = 1;
    pRoom->exit[loop]->pRoom->front_color     = BLUE;
    pRoom->exit[loop]->pRoom->back_color      = LIGHTGRAY;
    pRoom->exit[loop]->pRoom->org_front_color = EXIT_COL;
    pRoom->exit[loop]->pRoom->org_back_color  = BLACK;
    pRoom->exit[loop]->pRoom->smash           = SMASH_EXECUTE;
    pRoom->exit[loop]->pRoom->number          = NUMBER_ONLY;
    pRoom->exit[loop]->pRoom->address         = pRoom->exit[loop]->aRoom;
    clear_string( pRoom->exit[loop]->pRoom );

    /* 配置每一個出口鑰匙的字串空間 */
    pRoom->exit[loop]->pKey = malloc_string();

    /* 設定房間鑰匙號碼字串起啟值 */
    pRoom->exit[loop]->pKey->x_pos           = EXIT_X + 12;
    pRoom->exit[loop]->pKey->y_pos           = EXIT_Y + 9;
    pRoom->exit[loop]->pKey->x_len           = 5;
    pRoom->exit[loop]->pKey->y_len           = 1;
    pRoom->exit[loop]->pKey->front_color     = BLUE;
    pRoom->exit[loop]->pKey->back_color      = LIGHTGRAY;
    pRoom->exit[loop]->pKey->org_front_color = EXIT_COL;
    pRoom->exit[loop]->pKey->org_back_color  = BLACK;
    pRoom->exit[loop]->pKey->smash           = SMASH_EXECUTE;
    pRoom->exit[loop]->pKey->number          = NUMBER_ONLY;
    pRoom->exit[loop]->pKey->address         = pRoom->exit[loop]->aKey;
    clear_string( pRoom->exit[loop]->pKey );

    /* 配置每一個出口關鍵字的字串空間 */
    pRoom->exit[loop]->pKeyword = malloc_string();

    /* 設定房間出口關鍵字字串起啟值 */
    pRoom->exit[loop]->pKeyword->x_pos           = EXIT_X + 9;
    pRoom->exit[loop]->pKeyword->y_pos           = EXIT_Y + 11;
    pRoom->exit[loop]->pKeyword->x_len           = 40;
    pRoom->exit[loop]->pKeyword->y_len           = 1;
    pRoom->exit[loop]->pKeyword->front_color     = BLUE;
    pRoom->exit[loop]->pKeyword->back_color      = LIGHTGRAY;
    pRoom->exit[loop]->pKeyword->org_front_color = EXIT_COL;
    pRoom->exit[loop]->pKeyword->org_back_color  = BLACK;
    pRoom->exit[loop]->pKeyword->smash           = SMASH_EXECUTE;
    pRoom->exit[loop]->pKeyword->number          = NUMBER_ACCEPT;
    pRoom->exit[loop]->pKeyword->address = pRoom->exit[loop]->keyword;

    /* 配置每一個出口描述的字串空間 */
    pRoom->exit[loop]->pDesc = malloc_string();

    /* 設定房間出口描述字字串起啟值 */
    pRoom->exit[loop]->pDesc->x_pos           = EXIT_X + 9;
    pRoom->exit[loop]->pDesc->y_pos           = EXIT_Y + 13;
    pRoom->exit[loop]->pDesc->x_len           = 40;
    pRoom->exit[loop]->pDesc->y_len           = 1;
    pRoom->exit[loop]->pDesc->front_color     = BLUE;
    pRoom->exit[loop]->pDesc->back_color      = LIGHTGRAY;
    pRoom->exit[loop]->pDesc->org_front_color = EXIT_COL;
    pRoom->exit[loop]->pDesc->org_back_color  = BLACK;
    pRoom->exit[loop]->pDesc->smash           = SMASH_EXECUTE;
    pRoom->exit[loop]->pDesc->number          = NUMBER_ACCEPT;
    pRoom->exit[loop]->pDesc->address = pRoom->exit[loop]->description;

  }

  /* 配置房間中五個額外描述的結構的記憶體 */
  for ( loop = 0; loop < 5; loop++ )
  {
    pRoom->desc[loop] = ( ROOM_DESC * ) malloc ( sizeof( ROOM_DESC ) );
    if ( pRoom->desc[loop] == NULL ) mem_error();

    /* 配置房間額外描述關鍵字字串空間 */
    pRoom->desc[loop]->pKeyword = malloc_string();

    /* 設定房間關鍵字字串起啟值 */
    pRoom->desc[loop]->pKeyword->x_pos           = DESC_X + 9;
    pRoom->desc[loop]->pKeyword->y_pos           = DESC_Y + 5;
    pRoom->desc[loop]->pKeyword->x_len           = 30;
    pRoom->desc[loop]->pKeyword->y_len           = 1;
    pRoom->desc[loop]->pKeyword->front_color     = BLUE;
    pRoom->desc[loop]->pKeyword->back_color      = LIGHTGRAY;
    pRoom->desc[loop]->pKeyword->org_front_color = DESC_COL;
    pRoom->desc[loop]->pKeyword->org_back_color  = BLACK;
    pRoom->desc[loop]->pKeyword->smash           = SMASH_EXECUTE;
    pRoom->desc[loop]->pKeyword->number          = NUMBER_ACCEPT;
    pRoom->desc[loop]->pKeyword->address  = pRoom->desc[loop]->keyword;

    /* 設定房間關鍵字字串起啟值 */
    pRoom->desc[loop]->pDesc = malloc_string();

    /* 設定房間關鍵字字串起啟值 */
    pRoom->desc[loop]->pDesc->x_pos           = DESC_X + 7;
    pRoom->desc[loop]->pDesc->y_pos           = DESC_Y + 7;
    pRoom->desc[loop]->pDesc->x_len           = 65;
    pRoom->desc[loop]->pDesc->y_len           = 8;
    pRoom->desc[loop]->pDesc->front_color     = BLUE;
    pRoom->desc[loop]->pDesc->back_color      = LIGHTGRAY;
    pRoom->desc[loop]->pDesc->org_front_color = DESC_COL;
    pRoom->desc[loop]->pDesc->org_back_color  = BLACK;
    pRoom->desc[loop]->pDesc->smash           = SMASH_EXECUTE;
    pRoom->desc[loop]->pDesc->number          = NUMBER_ACCEPT;
    pRoom->desc[loop]->pDesc->address  = pRoom->desc[loop]->description;

  }

  return;
}

void mem_error( void )
{

  perror( "Cannot allocate memory." );
  exit( 0 );

}

STRING_INFO * malloc_string( void )
{

  STRING_INFO * temp;

  temp = ( STRING_INFO * ) malloc( sizeof( STRING_INFO ) );
  if ( temp == NULL )
  {
    mem_error();
    return NULL;
  }

  else
  {
   return temp;
  }
}
