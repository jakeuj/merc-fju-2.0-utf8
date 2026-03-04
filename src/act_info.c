/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*  這是由輔大化學系製作群所撰寫的遊戲﹐主體由 merc 改編而來﹐所有的版權    *
*  將會被保留﹐但歡迎大家修改﹐但我們也希望你們也能提供給大家﹐所有的商    *
*  業行為將不被允許。                                                      *
*                                                                          *
*  paul@mud.ch.fju.edu.tw                                                  *
*  lc@mud.ch.fju.edu.tw                                                    *
*                                                                          *
***************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/* 區域函數 */
char *  format_obj_to_char      args( ( OBJ_DATA *, CHAR_DATA *, bool, bool ) );
void    show_list_to_char       args( ( OBJ_DATA *, CHAR_DATA *, bool, bool, bool ) );
void    show_char_to_char_0     args( ( CHAR_DATA *, CHAR_DATA * ) );
void    show_char_to_char_1     args( ( CHAR_DATA *, CHAR_DATA * ) );
void    show_char_to_char       args( ( CHAR_DATA *, CHAR_DATA * ) );
void    show_percentage         args( ( CHAR_DATA *, int, int, int, int  ) );
int     who_compare             args( ( const void *, const void * ) );

#define NAME_ROW                ( 80 / NAME_LENGTH )

#ifdef __FreeBSD__
const char * crypt( const char *, const char * );
#endif

static ARRAY_DATA ObjArmor[] =
  { { 501, "\e[1;33m大量增強\e[0m" },
    { 400, "\e[1;33m四倍增強\e[0m" },
    { 300, "\e[1;33m三倍增強\e[0m" },
    { 200, "\e[1;33m二倍增強\e[0m" },
    { 101, "\e[1;33m一倍增強\e[0m" },
    { 100, "\e[1;32m完好無缺\e[0m" },
    {  70, "\e[1;35m輕度損壞\e[0m" },
    {  40, "\e[1;34m中度損壞\e[0m" },
    {  10, "\e[1;36m嚴重損壞\e[0m" },
    {  -1, "\e[1;31m該送修了\e[0m" } };

static ARRAY_DATA AlignmentMessage[] =
  { {   900, "\e[1;37m好像佛陀再世\e[0m" },
    {   700, "\e[1;37m天性純真善良\e[0m" },
    {   350, "\e[1;37m看來滿純潔的\e[0m" },
    {   100, "\e[1;37m還有一點人性\e[0m" },
    {  -100, "\e[1;37m是中立和平的\e[0m" },
    {  -350, "\e[1;37m實在是好壞喔\e[0m" },
    {  -700, "\e[1;37m應該是邪惡的\e[0m" },
    {  -900, "\e[1;37m一點都沒人性\e[0m" },
    { -1001, "\e[1;37m簡直是大惡魔\e[0m" } };

/*
static ARRAY_DATA ArmorMessage[] =
  { {  101, "\e[1;36m在裸奔唷﹗\e[0m"   },
    {   80, "\e[1;36m光溜溜的耶\e[0m"   },
    {   60, "\e[1;36m只穿一點衣服\e[0m" },
    {   40, "\e[1;36m有點裝備啦\e[0m"   },
    {   20, "\e[1;36m裝備還不錯\e[0m"   },
    {    0, "\e[1;36m有輕裝甲\e[0m"     },
    { - 20, "\e[1;36m後臺還不錯\e[0m"   },
    { - 40, "\e[1;36m有力的裝備\e[0m"   },
    { - 60, "\e[1;36m有重裝甲\e[0m"     },
    { - 80, "\e[1;36m可比沙石車\e[0m"   },
    { -100, "\e[1;36m有超級裝甲\e[0m"   },
    { -101, "\e[1;36m是天下無敵\e[0m"   } };
*/

static ARRAY_DATA HpMessage[] =
  { { 150 , " 你比$E更有機會成為血牛﹗"    },
    { 100 , " 你的生命力似乎比$E多不少。"  },
    { 50  , " 你的生命力比$E高出一些些。"  },
    { 0   , " $E的生命力和你差不多。"      },
    { -50 , " $E生命力比你高出一些。"      },
    { -100, " $E的生命力高出你很多很多。"  } };

static ARRAY_DATA LevelDiffMessage[] =
{ { 5 , "不要去招惹$N﹐而且你不會從他身上得到任何好處的﹗" },
  { 2 , "打倒$N要一點技巧﹐但是你還是有經驗值的﹗"         },
  { -3, "$N十分適合你冒險的等級﹐盡你能力宰掉他就對了﹗"   },
  { -6, "打這種像$N的肉腳就對了﹐而且還會有些經驗值﹗"     },
  { -7, "$N低於你冒險的等級﹐你不用花太多工夫在他身上。"   } };

static ARRAY_DATA LevelDiff2Message[] =
{ {  10, "你可以不費吹灰之力宰掉$N。"       },
  {  5 , "$N不是你的對手。"                 },
  {  3 , "$N很容易解決。"                   },
  { -2 , "你和$N旗鼓相當。"                 },
  { -5 , "你可能需要一些運氣喔。"           },
  { -10, "$N毫不憐憫的恥笑你這個肉腳。"     },
  { -15, "嘿﹗真巧﹐閻羅王正在嘗試召喚你。" } };

static ARRAY_DATA PryChance [] =
{
  { 300, "極高\n\r" },
  { 250, "很高\n\r" },
  { 200, "高\n\r"   },
  { 150, "一般\n\r" },
  { 100, "低\n\r"   },
  { 50 , "很低\n\r" },
  { -1 , "極低\n\r" }
};

char * check_damage( OBJ_DATA * obj )
{
  static char buf[MAX_STRING_LENGTH];

  PUSH_FUNCTION( "check_damage" );

  if ( obj && obj->max_armor > 0 )
  {
    sprintf( buf, "[%s]的"
      ,return_array( ObjArmor, obj->armor * 100 / UMAX( 1, obj->max_armor )
        , sizeof( ObjArmor ) / sizeof( ObjArmor[0] ) ) );

    RETURN( buf );
  }

  RETURN( "" );
}

char * format_obj_to_char( OBJ_DATA * obj, CHAR_DATA * ch
                            , bool fShort, bool fDamage )
{
  PUSH_FUNCTION( "format_obj_to_char" );

  clear_stack();

  if ( fDamage )
  {
    if ( obj->venom > 0 ) send_to_stack( "「\e[1;31m抹毒\e[0m」" );
    send_to_stack( "%s", check_damage( obj ) );
  }

  if ( obj->Invis ) send_to_stack( "\e[1;37m(隱形)\e[0m " );

  if ( is_affected( ch, SLOT_DETECT_EVIL ) && obj->Evil )
    send_to_stack( "\e[1;34m(邪氣)\e[0m " );

  if ( is_affected( ch, SLOT_DETECT_MAGIC ) && obj->Magic )
    send_to_stack( "\e[1;31m(魔法)\e[0m " );

  if ( obj->Glow ) send_to_stack( "\e[1;33m(閃閃發亮)\e[0m " );
  if ( obj->Hum )  send_to_stack( "\e[1;32m(嗡嗡作響)\e[0m " );

  if ( obj->Cancachet )
  {
    if ( obj->cachet ) send_to_stack( "\e[1;35m(封印)\e[0m " );
    else               send_to_stack( "\e[1;35m(可封印)\e[0m " );
  }

  if ( fShort )
  {
    if ( obj->cname ) send_to_stack( "%s", obj_name( ch, obj ) );
  }
  else
  {
    if ( obj->description ) send_to_stack( "%s", obj->description );
  }

  RETURN( return_stack() );
}

/* 顯現一串物品給玩家, 可以處理重複的裝備 */
void show_list_to_char( OBJ_DATA * list, CHAR_DATA * ch , bool fShort
                       , bool fShowNothing, bool fDamage )
{
  char    ** prgpstrShow;
  char    ** pUnit;
  int      * prgnShow;
  char     * pstrShow;
  OBJ_DATA * obj;
  int        nShow;
  int        iShow;
  int        count;
  bool       fCombine;

  PUSH_FUNCTION( "show_list_to_char" );

  if ( !ch || !ch->desc ) RETURN_NULL();

  clear_buffer();
  for ( count = 0, obj = list ; obj; obj = obj->next_content ) count++;

  if ( count > 1000 )
  {
    send_to_char( "哇﹐你的東西實在太多了喔。\n\r", ch );
    RETURN_NULL();
  }

  prgpstrShow = alloc_string( count * sizeof( char * ) );
  pUnit       = alloc_string( count * sizeof( char * ) );
  prgnShow    = alloc_string( count * sizeof( int )    );

  for ( nShow = 0, obj = list ; obj; obj = obj->next_content )
  {
    if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
    {
      pstrShow = format_obj_to_char( obj, ch, fShort, fDamage );
      fCombine = FALSE;

      if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
      {
        /* 找尋重複但相同種類的物品 */
        for ( iShow = nShow - 1; iShow >= 0; iShow-- )
        {
          if ( !str_cmp( prgpstrShow[iShow], pstrShow ) )
          {
            prgnShow[iShow]++;
            fCombine = TRUE;
            break;
          }
        }
      }

      /* 不需要整理重複種類的物品 */
      if ( !fCombine )
      {
        prgpstrShow[nShow] = str_dup( pstrShow );
        pUnit      [nShow] = str_dup( obj->unit );
        prgnShow   [nShow] = 1;
        nShow++;
      }
    }
  }

  /* 輸出格式化過的物品資料 */
  for ( iShow = 0 ; iShow < nShow ; iShow++ )
  {
    if ( IS_NPC( ch ) || IS_SET( ch->act, PLR_COMBINE ) )
    {
      char buffer[MAX_STRING_LENGTH];

      chinese_number( prgnShow[iShow], buffer );
      send_to_buffer( "%s", buffer );
      send_to_buffer( "%s", pUnit[iShow] );
    }

    send_to_buffer( "%s\n\r" , prgpstrShow[iShow] );
    free_string( prgpstrShow[iShow] );
    free_string( pUnit      [iShow] );
  }


  if ( fShowNothing && nShow == 0 ) send_to_buffer( "沒有任何的東西。\n\r" );

  print_buffer( ch );

  /* 釋放先前配置的記憶體 */
  free_string( ( void * ) prgpstrShow );
  free_string( ( void * ) pUnit       );
  free_string( ( void * ) prgnShow    );
  RETURN_NULL();
}

void show_char_to_char_0( CHAR_DATA * victim, CHAR_DATA * ch )
{
  CHAR_DATA      * mount;
  CHAR_DATA      * target;
  MOB_INDEX_DATA * pMob;

  PUSH_FUNCTION( "show_char_to_char_0" );

  clear_buffer();

  if ( is_affected( victim, SLOT_MASK )
    && !can_see_mask( ch, victim )
    && victim->in_room
    && ( pMob = get_mob_index( victim->mask ) ) )
  {
    if ( ( target = create_mobile( pMob, -1 ) ) )
    {
      char_to_room( target, victim->in_room );
      target->position = victim->position;
      target->fighting = victim->fighting;
    }
    else
    {
      target = victim;
    }
  }

  else
  {
    target = victim;
  }

  if ( is_affected( target, SLOT_INVIS )
    || is_affected( target, SLOT_MASS_INVIS ) )
    send_to_buffer( "(\e[1;34m隱形\e[0m) " );

  if ( is_affected( target, SLOT_MASK ) )
    send_to_buffer( "(\e[1;37m易容\e[0m) " );

  if ( is_affected( target, SLOT_HIDE ) )
    send_to_buffer( "(\e[36m藏匿\e[0m) " );

  if ( is_affected( target, SLOT_CHARM_PERSON ) )
    send_to_buffer( "(\e[32m迷惑\e[0m) " );

  if ( is_affected( target, SLOT_PASS_DOOR ) )
    send_to_buffer( "(\e[33m穿牆\e[0m) " );

  if ( is_affected( target, SLOT_FAERIE_FIRE ) )
    send_to_buffer( "(\e[1;35m粉紅光芒\e[0m) " );

  if ( is_affected( target, SLOT_SANCTUARY ) )
    send_to_buffer( "(\e[1;37m白色光芒\e[0m) " );

  if ( IS_EVIL( target ) && is_affected( ch, SLOT_DETECT_EVIL ) )
    send_to_buffer( "(\e[30;47m黑色光芒\e[0m) " );

  if ( IS_NPC( target ) && target->bounty )
    send_to_buffer( "(\e[1;36m懸賞\e[0m) " );

  if ( target->jade.high > 0 && target->jade.low > 0 && !target->boss )
    send_to_buffer( "(\e[1;36m迷失\e[0m) " );

  if ( !IS_NPC(target) && IS_SET(target->act, PLR_KILLER ) )
    send_to_buffer( "(\e[1;31m兇手\e[0m) " );

  if ( !IS_NPC(target) && IS_SET(target->act, PLR_THIEF  ) )
    send_to_buffer( "(\e[1;33m小偷\e[0m) " );

  if ( !IS_NPC(target) && IS_SET(target->act, PLR_BOLTER ) )
    send_to_buffer( "(\e[1;31m判幫者\e[0m) " );

  if ( !IS_NPC( target ) )
  {
    if ( target->level > LEVEL_HERO )
      send_to_buffer( "(\e[1;33m神之一族\e[0m) " );

    else send_to_buffer( "(\e[1;34m冒險者\e[0m) " );
  }

  send_to_buffer( "%s", mob_name( ch, target ) );
  send_to_buffer( "\e[0m" );

  if ( ( mount = get_mount( target ) ) )
    send_to_buffer( "騎著%s", mob_name( ch, mount ) );

  switch ( target->position )
  {
  case POS_DEAD:
    send_to_buffer( "死在此處﹐可憐啊﹗" );
    break;

  case POS_SLEEPING:
    send_to_buffer( "在這睡覺。" );
    break;

  case POS_RESTING:
    send_to_buffer( "在這裡休息。" );
    break;

  case POS_STANDING:
    send_to_buffer( "站在這裡。" );
    break;

  case POS_FIGHTING:

    send_to_buffer( "在這裡和" );

    if ( !target->fighting )
    {
      send_to_buffer( "空氣打架﹖" );
    }

    else if ( target->fighting == ch )
    {
      send_to_buffer( "你戰鬥。" );
    }

    else if ( target->in_room == target->fighting->in_room )
    {
      send_to_buffer( "%s戰鬥中。", mob_name( ch, target->fighting ) );
    }
    else
      send_to_buffer( "不知名人物打鬥中。" );

    break;
  }

  if ( !IS_NPC( target ) && !target->desc ) send_to_buffer( " <斷線中>" );

  if ( target->desc && set_descriptor_stack( target->desc ) )
  {
    target->desc->stack = FALSE;
    send_to_buffer( " <\e[1;31m訊息暫存\e[0m>" );
  }

  if ( target->desc && is_edit( target->desc ) )
    send_to_buffer( " <\e[1;32m編輯中\e[0m>" );

  if ( !IS_NPC( target ) && IS_SET( target->act, PLR_WIZINVIS ) )
    send_to_buffer( " <\e[1;33m神族隱形\e[0m>" );

  send_to_buffer( "\e[0m\n\r" );
  print_buffer( ch );

  if ( target != victim )
  {
    target->position = POS_STANDING;
    target->fighting = NULL;
    extract_char( target, TRUE );
  }
  RETURN_NULL();
}

void show_char_to_char_1( CHAR_DATA * victim, CHAR_DATA * ch )
{
  OBJ_DATA   * obj;
  CHAR_DATA  * mount;
  SITUS_DATA * pSitus;
  bool         found;

  PUSH_FUNCTION( "chow_char_to_char_1" );

  clear_buffer();

  if ( can_see( victim, ch ) )
  {
    act( "$n正注視著你耶﹐不知道有什麼企圖。" , ch, NULL, victim, TO_VICT );
    act( "$n注視著$N。"                     , ch, NULL, victim, TO_NOTVICT );

    if ( victim != ch && is_affected( victim, SLOT_MASK ) && victim->mask )
      act( "你的易容術似乎被$N識破了﹗", victim, NULL, ch, TO_CHAR );
  }

  if ( victim->pcdata )
  {
    if ( victim->pcdata->mater && *victim->pcdata->mater )
    {
      send_to_buffer( "配偶﹕%s\n\r", victim->pcdata->mater );
    }

    else if ( victim->pcdata->wooer )
    {
      send_to_buffer( "追求者﹕%s\n\r", victim->pcdata->wooer->name );
    }

    else
    {
      switch( victim->sex )
      {
      case SEX_FEMALE:
        send_to_buffer( "婚姻﹕待字閨中。\n\r" );
        break;

      case SEX_MALE:
        send_to_buffer( "婚姻﹕光棍一個。\n\r" );
        break;

      default:
        send_to_buffer( "婚姻﹕交給上天安排。\n\r" );
        break;
      }
    }
  }

  ( victim->description[0] )
    ? ( send_to_buffer( "%s", victim->description ) )
    : ( send_to_buffer( "%s看起來沒有什麼特別的。\n\r"
      , mob_name( ch, victim ) ) );

  send_to_buffer( "\e[0m%s%s\n\r"
    , mob_name( ch, victim ), status_message( victim ) );

  if ( ( mount = get_mount( victim ) ) )
    send_to_buffer( "座騎﹕%s\n\r", mob_name( ch, mount ) );

  if ( ( mount = get_rider( victim ) ) )
    send_to_buffer( "主人﹕%s\n\r", mob_name( ch, mount ) );

  for ( found = FALSE, pSitus = situs_list; pSitus; pSitus = pSitus->next )
  {
    if ( ( obj = get_eq_char( victim, pSitus->location ) )
      && can_see_obj( ch, obj ) )
    {
      if ( !found )
      {
        send_to_buffer( "\n\r%s正裝備著﹕\n\r" , mob_name( NULL, victim ) );
        found = TRUE;
      }

      send_to_buffer( "%s%s\n\r"
        , pSitus->eq_name
        , format_obj_to_char( obj, ch, TRUE, FALSE ) );
    }

    if ( buffer_full() ) break;
  }

  print_buffer( ch );

  /* 偷窺 , 先改神族可以看 */
  if ( victim != ch && !IS_NPC( ch ) && IS_IMMORTAL( ch ) )
  {
    act( "$A$1你偷偷地看$N身上所攜帶的東西$0", ch, NULL, victim, TO_CHAR );
    show_list_to_char( victim->carrying, ch, TRUE, TRUE, FALSE );
  }

  RETURN_NULL();
}

void show_char_to_char( CHAR_DATA * list, CHAR_DATA * ch )
{
  CHAR_DATA * rch;

  PUSH_FUNCTION( "show_char_to_char" );

  for ( rch = list; rch; rch = rch->next_in_room )
  {
    if ( rch == ch || rch->mount_by ) continue;

    if ( !IS_NPC(rch)
      && IS_SET(rch->act, PLR_WIZINVIS)
      && get_trust( ch ) < get_trust( rch ) ) continue;

    if ( can_see( ch, rch ) )
    {
      show_char_to_char_0( rch, ch );
    }

    else if ( room_is_dark( ch->in_room )
      && is_affected( rch, SLOT_INFRAVISION ) )
    {
      send_to_char( "\e[1;31m彷彿有人在某暗處瞪著你﹗\e[0m\n\r", ch );
    }
  }

  RETURN_NULL();
}

bool check_blind( CHAR_DATA * ch )
{
  PUSH_FUNCTION( "check_blind" );

  if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_HOLYLIGHT ) ) RETURN( TRUE );

  if ( is_affected( ch, SLOT_BLINDNESS ) )
  {
    send_to_char( "要是沒有瞎﹐我一定能看見。唉……\n\r", ch );
    RETURN( FALSE );
  }

  RETURN( TRUE );
}

FUNCTION( do_look )
{
  extern AREA_DATA * FoggyArea;
  char               buf[MAX_STRING_LENGTH];
  char               buf1[MAX_STRING_LENGTH];
  char               arg1[MAX_INPUT_LENGTH];
  char               arg2[MAX_INPUT_LENGTH];
  EXIT_DATA        * pexit;
  CHAR_DATA        * victim;
  OBJ_DATA         * obj;
  LIQ_DATA         * pLiq;
  SHIP_DATA        * pShip;
  AREA_DATA        * pArea;
  BOARD_DATA       * pBoard;
  MINE_DATA        * pMine;
  char             * pdesc;
  int                door;
  int                loop;
  char               Char;
  bool               foggy;
  bool               fChinese;

  PUSH_FUNCTION( "do_look" );

  if ( !IS_NPC( ch ) && !ch->desc ) RETURN_NULL();

  if ( ch->position < POS_SLEEPING )
  {
    send_to_char( "你只看到星星啦﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( ch->position == POS_SLEEPING )
  {
    send_to_char( "你在睡覺ㄋㄟ﹗zｚZＺ ……\n\r", ch );
    RETURN_NULL();
  }

  if ( !check_blind( ch ) ) RETURN_NULL();

  if ( !IS_NPC( ch )
    && !IS_SET( ch->act, PLR_HOLYLIGHT )
    && room_is_dark( ch->in_room ) )
  {
    act( "對不起﹐$r黑漆漆的﹐你什麼也看不到。", ch, NULL, NULL, TO_CHAR );
    show_char_to_char( ch->in_room->people, ch );
    RETURN_NULL();
  }

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if ( !arg1[0] || !str_cmp( arg1, "auto" ) )
  {
    if ( FoggyArea && ch->in_room->area && ch->in_room->area == FoggyArea )
      foggy = TRUE;

    else
      foggy = FALSE;

    act( "$t$7$r$0$A"
      , ch, ch->in_room->Memorize ? "\e[1;32m*\e[0m" : "", NULL, TO_CHAR );

    if ( !arg1[0] || ( !IS_NPC( ch ) && !IS_SET( ch->act, PLR_BRIEF ) ) )
    {
      if ( foggy && str_len( ch->in_room->description ) < sizeof( buf ) - 10 )
      {
        str_cpy( buf, ch->in_room->description );

        for ( fChinese = FALSE, loop = 0; buf[loop]; loop++ )
        {
          Char = buf[loop];

               if ( fChinese )                                 fChinese = FALSE;
          else if ( ( Char >= '\x81' ) && ( Char <= '\xFF' ) ) fChinese =  TRUE;
          else                                                 fChinese = FALSE;

          if ( number_range( 1, 100 ) < FoggyRoom
            && Char != '\n'
            && Char != '\r'
            && Char != '\t')
          {
            buf[loop] = '.';
            if ( fChinese && buf[loop+1] != '\x0' ) buf[loop+1] = '.';
          }

          if ( fChinese && buf[loop+1] != '\x0' )
          {
            fChinese = FALSE;
            loop++;
          }
        }

        send_to_char( buf, ch );
      }

      else
      {
        send_to_char( ch->in_room->description , ch );
        send_to_char( "\e[0m", ch );
      }
    }

    /* 此房間是幫會 */
    if ( ch->in_room->Club )
    {
      char buf1[MAX_INPUT_LENGTH];
      char buf2[MAX_INPUT_LENGTH];

      chinese_number( club_notes( ch ) , buf1 );
      chinese_number( club_unread( ch ), buf2 );

      act( "$3這裡有$t封幫會信「$T封未讀」。$0", ch, buf1, buf2, TO_CHAR );
    }

    show_list_to_char( ch->in_room->contents, ch, TRUE, FALSE, FALSE );

    if ( foggy )
       act( "      $7這裡的四周籠罩的一片大霧﹐你沒有辦法看的清楚﹗$0"
      , ch, NULL, NULL, TO_CHAR );

    if ( is_owner( ch, ch->in_room, FALSE ) )
      act( "       $2這裡是你自己買的房子﹐你覺得特別溫暖﹗$0"
        , ch, NULL, NULL, TO_CHAR );

    /* 留言版 */
    if ( ch->pcdata
      && ( pBoard = ch->in_room->board )
      && ch->level >= pBoard->read_level
      && ( !pBoard->lock || IS_IMMORTAL( ch ) ) )
    {
      chinese_number( post_count( pBoard, POST_ALL ), buf );
      chinese_number( unread_post( ch, pBoard ), buf1 );

      print_to_char( ch,
        "\n\r      \e[1;33m%s\e[1;32m留言版(board)﹐"
        "上面共有「\e[1;37m%s\e[1;32m」張留言﹐「\e[1;37m%s\e[1;32m」"
        "張未讀。\e[0m\n\r", pBoard->name, buf, buf1 );
    }

    send_to_char( "\n\r" , ch );

    /* 礦物 */
    for ( pMine = ch->in_room->mine; pMine; pMine = pMine->next )
    {
      if ( pMine->mineral
        && pMine->mineral->message
        && *pMine->mineral->message )
      {
        if ( *pMine->mineral->message == '.' )
          act( "$t", ch, pMine->mineral->message + 1, NULL, TO_CHAR );

        else
          act( "$t", ch, pMine->mineral->message, NULL, TO_CHAR );
      }
    }

    /* 此間是否為船的出發點或是船艙 */
    for ( pShip = ship_list; pShip; pShip = pShip->next )
    {
      if ( pShip->starting == ch->in_room && pShip->waiting_tick > 0 )
      {
        chinese_number( pShip->waiting_tick, buf1 );
        print_to_char( ch, "\n\r\e[1;32m這裡有艘%s將在%s小時"
          "後出發前往%s的%s。\n\r欲搭乘的旅客請查詢 ship。\e[0m\n\r"
          , pShip->name, buf1
        , ( pShip->destination
          && ( pArea = pShip->destination->area )
          && pArea->name ) ? pArea->name : "不知名區域"
        , pShip->starting && pShip->destination->name
          ? pShip->destination->name : "不知名地方" );
      }

      if ( pShip->cabin == ch->in_room && pShip->sailing_tick > 0 )
      {
        chinese_number( pShip->sailing_tick, buf1 );
        print_to_char( ch, "\n\r\e[1;33m這裡正是%s﹐它將在%s小時"
          "後抵達%s的%s。\e[0m\n\r"
          , pShip->name, buf1
        , ( pShip->destination
          && ( pArea = pShip->destination->area )
          && pArea->name ) ? pArea->name : "不知名區域"
        , pShip->starting && pShip->destination->name
          ? pShip->destination->name : "不知名地方" );
      }
    }

    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_AUTOEXIT ) )
      do_exits( ch, "auto" );

    show_char_to_char( ch->in_room->people, ch );
    RETURN_NULL();
  }

  if ( !str_prefix( arg1, "in" ) )
  {
    if ( !arg2[0] )
    {
      send_to_char( "你想看什麼東西﹖\n\r", ch );
      RETURN_NULL();
    }

    if ( !( obj = get_obj_here( ch, arg2 ) ) )
    {
      act( "你沒看到這個東西 $2$T$0 耶。", ch, NULL, arg2, TO_CHAR );
      RETURN_NULL();
    }

    switch ( obj->item_type )
    {
    default:
      act( "$p裡面不可能有東西。", ch, obj, NULL, TO_CHAR );
      message_driver( ch, obj, ACT_WHEN_LOOK );
      break;

    case ITEM_URN:

      if ( get_mob_index( obj->value[0] ) )
        act( "$p有封印著某種妖物﹐請多加小心﹗", ch, obj, NULL, TO_CHAR );
      else
        act( "$p從未被封印過﹐是個空壺﹗"      , ch, obj, NULL, TO_CHAR );
      break;

    case ITEM_DRINK_CON:

      if ( obj->value[1] <= 0 )
      {
        act( "嗯﹐$p空空如也。", ch, obj, NULL, TO_CHAR );
        message_driver( ch, obj, ACT_WHEN_LOOK );
        break;
      }

      if ( !( pLiq = liq_lookup( obj->value[2] ) ) )
      {
        mudlog( LOG_DEBUG, "do_look: 沒有編號 %d 的液體。", obj->value[2] );
        break;
      }

      sprintf( buf, "它裝了%s的%s液體。\n\r",
        obj->value[1] < obj->value[0] / 4
          ? "少於二分之一"
          : obj->value[1] < 3 * obj->value[0] / 4
            ? "大約二分之一" : "大於二分之一"
         , pLiq->color );

      send_to_char( buf, ch );
      message_driver( ch, obj, ACT_WHEN_LOOK );
      break;

    case ITEM_CONTAINER:
    case ITEM_CORPSE_NPC:
    case ITEM_CORPSE_PC:

      if ( IS_SET( obj->value[1], CONT_CLOSED ) )
      {
        act( "$p是關起來的。", ch, obj, NULL, TO_CHAR );
        message_driver( ch, obj, ACT_WHEN_LOOK );
        break;
      }

      act( "$p裡面有﹕", ch, obj, NULL, TO_CHAR );
      show_list_to_char( obj->contains, ch, TRUE, TRUE, FALSE );

      message_driver( ch, obj, ACT_WHEN_LOOK );
      break;

    case ITEM_MAGICSTONE:
      act( "$p似乎有一些不一樣的地方。", ch, obj, NULL, TO_CHAR );
      message_driver( ch, obj, ACT_WHEN_LOOK );
      break;

    case ITEM_SPIRITJADE:
      act( "$p看起來是一塊鬼玉﹐但不知道好不好﹗", ch, obj, NULL, TO_CHAR );
      message_driver( ch, obj, ACT_WHEN_LOOK );
      break;
    }

    RETURN_NULL();
  }

  /* 觀看留言板 */
  if ( !str_cmp( arg1, "board" ) )
  {
    if ( !( pBoard = ch->in_room->board ) )
    {
      act( "$r又沒有留言板﹐怎麼看呢﹖", ch, NULL, NULL, TO_CHAR );
      RETURN_NULL();
    }

    if ( pBoard->lock )
    {
      act( "$t留言板目前關閉中﹐所以不能觀看﹗"
        , ch, pBoard->name, NULL, TO_CHAR );

      RETURN_NULL();
    }

    if ( pBoard->read_level > ch->level )
    {
      act( "對不起﹐$p留言板是給等級比較高的人觀看的。"
        , ch, pBoard->name, NULL, TO_CHAR );

      RETURN_NULL();
    }

    if ( !pBoard->post )
    {
      act( "$p留言板乏人問津﹐竟然半張留言也沒有﹗"
        , ch, NULL, pBoard->name, TO_CHAR );

      RETURN_NULL();
    }

    /* 顯示出留言 */
    show_board_title( ch, pBoard );

    RETURN_NULL();
  }

  if ( ( obj = get_obj_here ( ch ,arg1 ) ) )
  {
    switch( obj->item_type )
    {
    default :
      break;

    case ITEM_DRINK_CON:

      if ( obj->value[1] <= 0 )
      {
        act( "$p裡面什麼也沒有﹗", ch, obj, NULL, TO_CHAR );
        message_driver( ch, obj, ACT_WHEN_LOOK );
        RETURN_NULL();
      }

      if ( !( pLiq = liq_lookup( obj->value[2] ) ) )
      {
        mudlog( LOG_DEBUG, "do_look: 沒有編號 %d 的液體.", obj->value[2] );
        RETURN_NULL();
      }

      sprintf( buf, "%s裝了%s的%s液體。\n\r",
        obj_name( ch, obj ), obj->value[1] < obj->value[0] / 4
          ? "少於二分之一"
          : obj->value[1] < 3 * obj->value[0] / 4
            ? "大約二分之一" : "大於二分之一"
         , pLiq->color );

      send_to_char( buf, ch );
      message_driver( ch, obj, ACT_WHEN_LOOK );
      RETURN_NULL();

    case ITEM_CORPSE_NPC:
    case ITEM_CORPSE_PC:

      act( " $p裡面有﹕\n\r" , ch, obj, NULL, TO_CHAR );
      show_list_to_char( obj->contains, ch, TRUE, TRUE, FALSE );
      message_driver( ch, obj, ACT_WHEN_LOOK );
      RETURN_NULL();
    }
  }

  if ( ( victim = get_char_room( ch, arg1 ) ) )
  {
    show_char_to_char_1( victim, ch );
    RETURN_NULL();
  }

  for ( obj = ch->carrying; obj; obj = obj->next_content )
  {
    if ( can_see_obj( ch, obj ) )
    {
      if ( ( pdesc = get_extra_descr( arg1, obj->extra_descr ) ) )
      {
        act( "$t", ch, pdesc, NULL, TO_CHAR );
        message_driver( ch, obj, ACT_WHEN_LOOK );
        RETURN_NULL();
      }

      if ( ( pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr ) ) )
      {
        act( "$t", ch, pdesc, NULL, TO_CHAR );
        message_driver( ch, obj, ACT_WHEN_LOOK );
        RETURN_NULL();
      }
    }

    if ( is_name( arg1, obj->name ) )
    {
      act( "$t", ch, obj->description, NULL, TO_CHAR );

      if ( obj->venom > 0 )
        act( "$p看起來似乎是有毒的東西。", ch, obj, NULL, TO_CHAR );

      message_driver( ch, obj, ACT_WHEN_LOOK );
      RETURN_NULL();
    }
  }

  for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
  {
    if ( can_see_obj( ch, obj ) )
    {
      if ( ( pdesc = get_extra_descr( arg1, obj->extra_descr ) ) )
      {
        act( "$t", ch, pdesc, NULL, TO_CHAR );
        message_driver( ch, obj, ACT_WHEN_LOOK );
        RETURN_NULL();
      }

      if ( ( pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr ) ) )
      {
        act( "$t", ch, pdesc, NULL, TO_CHAR );
        message_driver( ch, obj, ACT_WHEN_LOOK );
        RETURN_NULL();
      }
    }

    if ( is_name( arg1, obj->name ) )
    {
      act( "$t", ch, obj->description, NULL, TO_CHAR );

      if ( obj->venom > 0 )
        act( "$p看起來像是有毒的東西。", ch, obj, NULL, TO_CHAR );

      message_driver( ch, obj, ACT_WHEN_LOOK );
      RETURN_NULL();
    }
  }

  if ( ( pdesc = get_extra_descr( arg1, ch->in_room->extra_descr ) ) )
  {
    send_to_char( pdesc, ch );
    RETURN_NULL();
  }

       if ( !str_prefix( arg1, "north" ) ) door = DIR_NORTH;
  else if ( !str_prefix( arg1, "east"  ) ) door = DIR_EAST;
  else if ( !str_prefix( arg1, "south" ) ) door = DIR_SOUTH;
  else if ( !str_prefix( arg1, "west"  ) ) door = DIR_WEST;
  else if ( !str_prefix( arg1, "up"    ) ) door = DIR_UP;
  else if ( !str_prefix( arg1, "down"  ) ) door = DIR_DOWN;
  else
  {
    send_to_char( "對不起﹐那裡沒有東西﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( !( pexit = ch->in_room->exit[door] ) || pexit->foggy )
  {
    send_to_char( "沒什麼特別的。\n\r", ch );
    RETURN_NULL();
  }

  if ( pexit->description && pexit->description[0] )
    send_to_char( pexit->description, ch );
  else
    send_to_char( "沒什麼特別的。\n\r", ch );

  if ( pexit->keyword && pexit->keyword[0] && pexit->keyword[0] != ' ' )
  {
    if ( pexit->closed )
    {
      act( " $d邊的門關著。", ch, NULL, pexit->keyword, TO_CHAR );
    }

    else if ( pexit->isdoor )
    {
      act( " $d邊的門是開著的。", ch, NULL, pexit->keyword, TO_CHAR );
    }
  }

  RETURN_NULL();
}

FUNCTION( do_examine )
{
  char       arg[MAX_INPUT_LENGTH];
  char       buf[MAX_STRING_LENGTH];
  OBJ_DATA * obj;

  PUSH_FUNCTION( "do_examine" );

  one_argument( argument, arg );

  if ( ( obj = get_obj_here( ch, arg ) ) )
  {
    switch ( obj->item_type )
    {
    default:
      break;

    case ITEM_DRINK_CON:
    case ITEM_CONTAINER:
    case ITEM_CORPSE_NPC:
    case ITEM_CORPSE_PC:
    case ITEM_MAGICSTONE:
    case ITEM_SPIRITJADE:
    case ITEM_URN:
      send_to_char ( "你仔細一看﹐在" , ch );
      sprintf( buf, "in %s", arg );
      do_look( ch, buf );
      RETURN_NULL();
    }
  }

  do_look( ch , arg );

  RETURN_NULL();
}

FUNCTION( do_exits )
{
  EXIT_DATA * pexit;
  bool        found = FALSE;
  bool        fAuto;
  int         door;

  PUSH_FUNCTION( "do_exits" );

  /* 眼盲看不到出口 */
  if ( !check_blind( ch ) ) RETURN_NULL();

  clear_buffer();
  send_to_buffer( ( fAuto = !str_cmp( argument, "auto" ) ) ?
    "「\e[1;36m出口﹕\e[0m" : "\e[1;36m明顯的出口﹕\e[0m\n\r" );

  for ( door = 0; door < DIR_MAX; door++ )
  {
    if ( ( pexit = ch->in_room->exit[door] )
      && pexit->to_room
      && pexit->closed == FALSE
      && ( !pexit->foggy || IS_IMMORTAL( ch ) ) )
    {
      if ( fAuto )
      {
        found = TRUE;
        send_to_buffer( " %s%s\e[0m"
          , pexit->foggy ? "\e[1;31m" : "\e[1;33m"
          , direction_name( door ) );
      }

      else
      {
        if ( !room_is_dark( pexit->to_room ) || IS_SET( ch->act, PLR_HOLYLIGHT ) )
        {
          found = TRUE;

          send_to_buffer( "%s%s方是通往\e[1;35m%s\e[0m。\n\r"
            , pexit->foggy ? "(迷霧)" : ""
            ,direction_name( door )
            , pexit->to_room->name );
        }
      }
    }
  }

  if ( !found )
    send_to_buffer( fAuto ? " 無 " : "沒有發現到任何光亮的出口。\n\r" );

  if ( fAuto ) send_to_buffer( "」\n\r\n\r" );
  print_buffer( ch );
  RETURN_NULL();
}

FUNCTION( do_hp )
{
  CHAR_DATA * vi;
  char        pellet[MAX_STRING_LENGTH];
  char        arg[MAX_INPUT_LENGTH];

  PUSH_FUNCTION( "do_hp" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  one_argument( argument, arg );

  if ( arg[0] == '\x0' )
  {
    vi = ch;
  }

  else
  {
    if ( !( vi = get_char_world( ch, arg ) ) )
    {
      act( "找不到你要觀看的對象 $2$T$0。", ch , NULL, arg, TO_CHAR );
      RETURN_NULL();
    }

    if ( IS_NPC( vi ) || !vi->pcdata )
    {
      act( "$N是非玩家﹐無法察看﹗", ch, NULL, vi, TO_CHAR );
      RETURN_NULL();
    }

    if ( !biliwick( ch, vi ) )
    {
      act( "你的權力不夠看$N的狀態。", ch, NULL, vi, TO_CHAR );
      RETURN_NULL();
    }
  }

  tablize( FORMAT_SET,
"[1;33m\r╭─────┬────────┬────────────────────╮[0m\n\r"
"[1;33m\r│[1;32m等級﹕[0m $LV[1;33m│[1;32mID﹕[0m $NAME      [1;33m│[1;32m姓名﹕[0m $CNAME                           [1;33m│[0m\n\r"
"[1;33m\r├─────┴────────┴────────────────────┤[0m\n\r"
"[1;33m\r│[0m生命力﹕[0m($HIT  /$MHIT ) [0m法力﹕[0m($MANA /$MMAN ) [0m體力﹕[0m($MOVE /$MMOV )   [1;33m│[0m\n\r"
"[1;33m\r│[0m金錢﹕[0m $GOLD        [0m銀行存款﹕[0m $BANK        [0m經驗值﹕[0m $EXP             [1;33m│[0m\n\r"
"[1;33m\r│[0m免死金牌﹕[0m $FIR [0m張 天牢﹕[0m $JAI [0m天[0m 思過﹕[0m $FAL [0m小時[0m PK﹕WIN 勝 LOS 敗  [1;33m│[0m\n\r"
"[1;33m\r│[0m求愛對象﹕[0m $WOOER       [0m配偶﹕[0m $MATER       [0m是否在線上﹕[0m $Z           [1;33m│[0m\n\r"
"[1;33m\r│[0m狀態﹕[0m $STA                                                           [1;33m│[0m\n\r"
"[1;33m\r╰───────────────────────────────────╯[0m\n\r"
  , 0, NULL, 0 );

  tablize( FORMAT_NUM, NULL, vi->level           , "$LV"    ,  3 );
  tablize( FORMAT_NUM, NULL, vi->hit             , "$HIT"   ,  6 );
  tablize( FORMAT_NUM, NULL, get_curr_hit( vi )  , "$MHIT"  ,  6 );
  tablize( FORMAT_NUM, NULL, vi->mana            , "$MANA"  ,  6 );
  tablize( FORMAT_NUM, NULL, get_curr_mana( vi ) , "$MMAN"  ,  6 );
  tablize( FORMAT_NUM, NULL, vi->move            , "$MOVE"  ,  6 );
  tablize( FORMAT_NUM, NULL, get_curr_move( vi ) , "$MMOV"  ,  6 );
  tablize( FORMAT_NUM, NULL, vi->gold            , "$GOLD"  , 11 );
  tablize( FORMAT_NUM, NULL, vi->bank            , "$BANK"  , 11 );
  tablize( FORMAT_NUM, NULL, vi->exp             , "$EXP"   , 11 );
  tablize( FORMAT_NUM, NULL, vi->firman          , "$FIR"   ,  4 );
  tablize( FORMAT_NUM, NULL, vi->jail            , "$JAI"   ,  4 );
  tablize( FORMAT_NUM, NULL, vi->failed          , "$FAL"   ,  4 );
  tablize( FORMAT_NUM, NULL, vi->pcdata->fight_win , "WIN"    ,  3 );
  tablize( FORMAT_NUM, NULL, vi->pcdata->fight_lost, "LOS"    ,  3 );

  tablize( FORMAT_STRING
    , vi->pcdata->wooer ? vi->pcdata->wooer->name : "(從缺)"
    , 0, "$WOOER" , 12 );

  tablize( FORMAT_STRING
    , vi->pcdata->mater && *vi->pcdata->mater ? vi->pcdata->mater : "(從缺)"
    , 0, "$MATER" , 12 );

  tablize( FORMAT_STRING, YESNO( vi->pcdata->mate ), 0, "$Z", 2 );
  tablize( FORMAT_STRING, vi->name , 0 , "$NAME"  , 11 );
  tablize( FORMAT_STRING, vi->cname, 0 , "$CNAME" , 33 );
  tablize( FORMAT_STRING
    , vi->pcdata->dosage >= DosageLimit ? "藥物中毒" : "無事"
    , 0 , "$STA" , 10 );

  tablize( FORMAT_RETURN, pellet, 0, NULL, MAX_STRING_LENGTH );
  send_to_char( pellet, ch );

  RETURN_NULL();
}

FUNCTION( do_score )
{
  char          buf[MAX_STRING_LENGTH ];
  char          pellet[MAX_STRING_LENGTH * 2 ];
  char          output[MAX_STRING_LENGTH * 2 ];
  char          arg[MAX_INPUT_LENGTH];
  AFFECT_DATA * paf;
  AREA_DATA   * pArea;
  CHAR_DATA   * vi;
  SKILL_DATA  * pSkill;
  int           iNum;

  PUSH_FUNCTION( "do_score" );

  if ( IS_NPC( ch ) ) RETURN_NULL();
  vi = ch;

  one_argument( argument, arg );

  if ( !arg[0] )
  {
    vi = ch;
  }

  else
  {
    if ( !( vi = get_char_world( ch, arg ) ) )
    {
      act( "找不到你要觀看的對象 $2$T$0。", ch, NULL, arg, TO_CHAR );
      RETURN_NULL();
    }

    if ( IS_NPC( vi ) || !vi->pcdata )
    {
      act( "$N是非玩家﹐無法察看﹗", ch, NULL, vi, TO_CHAR );
      RETURN_NULL();
    }

    if ( !biliwick( ch, vi ) )
    {
      act( "你的權力不夠看$N的狀態。", ch, NULL, vi, TO_CHAR );
      RETURN_NULL();
    }
  }

  tablize( FORMAT_SET,
"[1;33m\r"
"╭──┬──┬──────────────────────┬────────╮[0m\n\r"
"[1;33m│[1;32m等級[1;33m│[0m$LVL[1;33m│[0m$CNAME                                      [1;33m│ [1;32m祖籍﹕ [0m$HOME   [1;33m│[0m\n\r"
"[1;33m├──┼──┴────┬──┬────────┬──┬─┬┴─┬──────┤[0m\n\r"
"[1;33m│[1;32m職業[1;33m│[0m$CLASS        [1;33m│[1;32m年齡[1;33m│[0mYEAR[0m歲([0mREAL[0m小時)[1;33m│[1;32m性別[1;33m│[0mSEXN[1;32m財產[1;33m│[0m$GOLD   [0m金幣[1;33m│[0m\n\r"
"[1;33m├─┬┴─────┬─┴──┴────────┴──┴─┴──┴──────┤[0m\n\r"
"[1;33m│[1;32m人[1;33m│[0m力量[0m( MSSS )[1;33m│[0m生命力[0m(HP   /MMHP )[0m 靈力[0m(MANA /MMMNA) [0m體力[0m(MOVE /MMMVE) [1;33m│[0m\n\r"
"\e[1;33m│\e[1;32m物\e[1;33m│\e[0m智力\e[0m( MIII )\e[1;33m│\e[0m技能\e[0m(TRA) \e[0m致能\e[0m(EN) \e[0m訓練點數\e[0m(LEAR) \e[0m銀行存款\e[0m($BANK       )[1;33m│[0m\n\r"
"\e[1;33m│\e[1;32m基\e[1;33m│\e[0m學識\e[0m( MWWW )\e[1;33m│\e[0m攜帶物品﹕ヾ數量﹕\e[0mNUM /MNU \e[0m件 ゝ重量﹕\e[0mWEI  / MEI  \e[0m公斤  [1;33m│[0m\n\r"
"\e[1;33m│\e[1;32m本\e[1;33m│\e[0m敏捷\e[0m( MDDD )\e[1;33m│\e[0m目前經驗值﹕\e[0m$EXP     \e[0m點﹐還需要再 \e[0m$LEFT    \e[0m點升下一級。 [1;33m│[0m\n\r"
"\e[1;33m│\e[1;32m狀\e[1;33m│\e[0m體質\e[0m( MCCC )\e[1;33m│\e[0m目前陣營﹕\e[0mLIGN                                          [1;33m│[0m\n\r"
"\e[1;33m│\e[1;32m況\e[1;33m│\e[0m極限\e[0m( LUML )\e[1;33m│\e[0m目前狀態﹕\e[0mPOSITION        \e[0m飢餓（HU  口渴（DR  酒醉（DU  [1;33m│[0m\n\r"
"\e[1;33m│  \e[1;33m│\e[0m房間數(\e[1;32mHMN\e[0m )\e[1;33m│\e[0m免死金牌﹕\e[0m$FIR \e[0m張 天牢﹕\e[0m$JAI \e[0m天 思過﹕\e[0m$FAL\e[0m小時[1;33m          │[0m\n\r"
"\e[1;33m├─┴──┬───┴────────────────────────────┤[0m\n\r"
"\e[1;33m│\e[1;32m個人設定\e[1;33m│\e[0mヾ生命力降至 \e[0m$WIMPY \e[0m以下自動逃跑 ゝ每頁\e[0m PAG\e[0m行暫停 ゞ \e[0m$A \e[0m巨集指令[1;33m│[0m\n\r"
"\e[1;33m│        \e[1;33m│\e[0m自動出口\e[0m（GEXI \e[0m自動搜刮\e[0m（GLOO \e[0m自動奉獻屍體\e[0m（GSAC \e[0m遭受天遣\e[0m（GGOD [1;33m│[0m\n\r"
"\e[1;33m├────┼────────────────────────────────┤[0m\n\r"
"\e[1;33m│\e[1;32m國家幫派\e[1;33m│\e[0m隸屬 \e[0m$CLUB      \e[0m幫﹐現任 \e[0m$NOW                                   [1;33m│[0m\n\r"
"\e[1;33m├────┼────────────────────────────────┤[0m\n\r"
"\e[1;33m│\e[1;32m婚姻狀態\e[1;33m│\e[0m求愛對象﹕\e[0m$WOOER      \e[0m配偶﹕\e[0m$MATER       \e[0m是否在線上﹕\e[0m$ON        [1;33m│[0m\n\r"
"\e[1;33m├─┬──┴─────┬──────────────────────────┤[0m\n\r"
"\e[1;33m│\e[1;32m武\e[1;33m│\e[0m攻擊命中率\e[0m(HIT )\e[1;33m│\e[0m防護力\e[0m(DEF  ) \e[0m\e[0m 藥物中毒﹕$POI($STA)                 [1;33m│[0m\n\r"
"\e[1;33m│\e[1;32m裝\e[1;33m│\e[0m攻擊傷害力\e[0m(DAM )\e[1;33m│\e[0m裝備吸收程度﹕百分之 $EQ                            [1;33m│[0m\n\r"  , 0, NULL, 0 );

  tablize( FORMAT_NUM, NULL, vi->eq_capcity            , "$EQ"   , 3 );
  tablize( FORMAT_NUM, NULL, vi->level                 , "$LVL"  , 4 );
  tablize( FORMAT_NUM, NULL, get_age( vi )             , "YEAR"  , 4 );
  tablize( FORMAT_NUM, NULL, ( get_age( vi ) - 17 ) * 2, "REAL"  , 4 );
  tablize( FORMAT_NUM, NULL, vi->gold                  , "$GOLD" , 7 );
  tablize( FORMAT_NUM, NULL, get_curr_str( vi )        , "MSSS"  , 4 );
  tablize( FORMAT_NUM, NULL, get_curr_int( vi )        , "MIII"  , 4 );
  tablize( FORMAT_NUM, NULL, get_curr_wis( vi )        , "MWWW"  , 4 );
  tablize( FORMAT_NUM, NULL, get_curr_dex( vi )        , "MDDD"  , 4 );
  tablize( FORMAT_NUM, NULL, get_curr_con( vi )        , "MCCC"  , 4 );
  tablize( FORMAT_NUM, NULL, vi->limit                 , "LUML"  , 4 );
  tablize( FORMAT_NUM, NULL, vi->hit                   , "HP"    , 5 );
  tablize( FORMAT_NUM, NULL, get_curr_hit( vi )        , "MMHP"  , 5 );
  tablize( FORMAT_NUM, NULL, vi->mana                  , "MANA"  , 5 );
  tablize( FORMAT_NUM, NULL, get_curr_mana( vi )       , "MMMNA" , 5 );
  tablize( FORMAT_NUM, NULL, vi->move                  , "MOVE"  , 5 );
  tablize( FORMAT_NUM, NULL, get_curr_move( vi )       , "MMMVE" , 5 );
  tablize( FORMAT_NUM, NULL, learn_count( vi )         , "TRA"   , 3 );
  tablize( FORMAT_NUM, NULL, enable_count( vi )        , "EN"    , 2 );
  tablize( FORMAT_NUM, NULL, vi->practice              , "LEAR"  , 4 );
  tablize( FORMAT_NUM, NULL, vi->bank                  , "$BANK" ,12 );
  tablize( FORMAT_NUM, NULL, can_carry_n( vi )         , "MNU"   , 3 );
  tablize( FORMAT_NUM, NULL, can_carry_w( vi )         , "MEI"   , 4 );
  tablize( FORMAT_NUM, NULL, vi->carry_number          , "NUM"   , 3 );
  tablize( FORMAT_NUM, NULL, get_carry_weight( vi )    , "WEI"   , 4 );
  tablize( FORMAT_NUM, NULL, vi->wimpy                 , "$WIMPY", 6 );
  tablize( FORMAT_NUM, NULL, vi->pcdata->pagelen       , "PAG"   , 3 );
  tablize( FORMAT_NUM, NULL, vi->exp                   , "$EXP"  , 8 );
  tablize( FORMAT_NUM, NULL, get_alias_count( vi )     , "$A"    , 2 );
  tablize( FORMAT_NUM, NULL, count_home_number( vi )   , "HMN"   , 3 );

  iNum = kill_table[ URANGE( 1, vi->level, MAX_LEVEL - 1 ) ].exp - vi->exp;
  if ( vi->level >= LEVEL_HERO ) iNum = 0;

  tablize( FORMAT_NUM, NULL, iNum                      , "$LEFT", 8 );

  sprintf( buf, "%s \e[0m( \e[1;36m%s \e[0m) %s\e[0m"
    , vi->cname
    , vi->name
    , vi->pcdata->title );
  tablize( FORMAT_STRING, buf, 0, "$CNAME", 42 );

  pArea = get_home( vi );
  tablize( FORMAT_STRING, pArea ? pArea->name : "不詳", 0, "$HOME", 8 );

  tablize( FORMAT_STRING, club_name( vi ), 0, "$CLUB", 10 );

  tablize( FORMAT_STRING, is_club_class( vi ), 0, "$NOW", 12 );

  tablize( FORMAT_STRING, IS_IMMORTAL( vi ) ?
    "\e[1;33m系統管理者\e[0m" : class_name( vi->class, FALSE ) , 0, "$CLASS", 14 );

  tablize( FORMAT_STRING, IS_MALE( vi ) ? "\e[1;32m男\e[1;33m│" :
    ( IS_FEMALE( vi ) ? "\e[1;31m女\e[1;33m│" : "\e[33m中\e[1;33m│" )
    , 0, "SEXN" , 4 );

  switch ( vi->position )
  {
  default:
    str_cpy( buf, "\e[1;37m閒晃中。\e[0m" );
    break;

  case POS_DEAD:
    str_cpy( buf, "\e[1;31m被殺死了﹗\e[0m" );
    break;

  case POS_SLEEPING:
    str_cpy( buf, "\e[1;34m正在睡覺。\e[0m" );
    break;

  case POS_RESTING:
    str_cpy( buf, "\e[1;36m正在休息。\e[0m" );
    break;

  case POS_STANDING:
    str_cpy( buf, "\e[1;37m正站立著。\e[0m" );
    break;

  case POS_FIGHTING:
    str_cpy( buf, "\e[1;31m正在戰鬥。\e[0m" );
    break;
  }

  tablize( FORMAT_STRING, buf, 0, "POSITION", 15 );

  if ( !IS_NPC(vi) && vi->pcdata->condition[COND_DRUNK]   > 10 )
    tablize( FORMAT_STRING, "\e[5;1;33m☉\e[0m）", 0, "DU", 4 );
  else
    tablize( FORMAT_STRING, "  ）", 0, "DU", 4 );

  if ( !IS_NPC(vi) && vi->pcdata->condition[COND_THIRST] ==  0 )
    tablize( FORMAT_STRING, "\e[5;1;33m☉\e[0m）", 0, "DR", 4 );
  else
    tablize( FORMAT_STRING, "  ）", 0, "DR", 4 );

  if ( !IS_NPC(vi) && vi->pcdata->condition[COND_FULL]   ==  0 )
    tablize( FORMAT_STRING, "\e[5;1;33m☉\e[0m）", 0, "HU", 4 );
  else
    tablize( FORMAT_STRING, "  ）", 0, "HU", 4 );

  if ( IS_SET( vi->act, PLR_AUTOEXIT ) )
    tablize( FORMAT_STRING, "\e[1;32m☉\e[0m）", 0, "GEXI", 4 );
  else
    tablize( FORMAT_STRING, "  ）", 0, "GEXI", 4 );

  if ( IS_SET( vi->act, PLR_SILENCE  )
    || IS_SET( vi->act, PLR_NO_EMOTE )
    || IS_SET( vi->act, PLR_NO_TELL  ) )
    tablize( FORMAT_STRING, "\e[1;32m☉\e[0m）", 0, "GGOD", 4 );
  else
    tablize( FORMAT_STRING, "  ）", 0, "GGOD", 4 );

  if ( IS_SET( vi->act, PLR_AUTOLOOT ) )
    tablize( FORMAT_STRING, "\e[1;32m☉\e[0m）", 0, "GLOO", 4 );
  else
    tablize( FORMAT_STRING, "  ）", 0, "GLOO", 4 );

  if ( IS_SET( vi->act, PLR_AUTOSAC ) )
    tablize( FORMAT_STRING, "\e[1;32m☉\e[0m）", 0, "GSAC", 4 );
  else
    tablize( FORMAT_STRING, "  ）", 0, "GSAC", 4 );

  str_cpy( buf, return_array( AlignmentMessage, vi->alignment
            , sizeof( AlignmentMessage ) / sizeof( AlignmentMessage[0] ) ) );

  tablize( FORMAT_STRING, buf, 0, "LIGN", 45 );

  /*
  str_cpy( buf, return_array( ArmorMessage, get_ac( vi )
            , sizeof( ArmorMessage ) / sizeof( ArmorMessage[0] ) ) );
  */

  tablize( FORMAT_STRING
    , vi->pcdata->dosage >= DosageLimit ? "中毒" : "沒有"
    , 0, "$POI", 4 );

  tablize( FORMAT_NUM, NULL, vi->pcdata->dosage, "$STA", 4 );
  tablize( FORMAT_NUM, NULL, get_ac( vi ),        "DEF", 5 );
  tablize( FORMAT_NUM, NULL, get_hitroll( vi )  , "HIT", 4 );
  tablize( FORMAT_NUM, NULL, get_damroll( vi )  , "DAM", 4 );
  tablize( FORMAT_NUM, NULL, vi->firman         , "$FIR",4 );
  tablize( FORMAT_NUM, NULL, vi->jail           , "$JAI",4 );
  tablize( FORMAT_NUM, NULL, vi->failed         , "$FAL",4 );

  tablize( FORMAT_STRING
    , vi->pcdata->wooer ? vi->pcdata->wooer->name : "(從缺)"
    , 0, "$WOOER" , 12 );

  tablize( FORMAT_STRING
    , vi->pcdata->mater && *vi->pcdata->mater ? vi->pcdata->mater : "(從缺)"
    , 0, "$MATER" , 12 );

  tablize( FORMAT_STRING, YESNO( vi->pcdata->mate ), 0, "$ON", 3 );

  tablize( FORMAT_RETURN, pellet, 0, NULL, MAX_STRING_LENGTH );
  str_cpy( output, pellet );

  if ( vi->affected )
  {
    bool Touch;

    tablize( FORMAT_SET,
      "\e[1;33m\r├─┴──┬─────┴───────────────"
      "───────────┤\e[0m\n\r", 0, NULL, 0 );

    tablize( FORMAT_RETURN, pellet, 0, NULL, MAX_STRING_LENGTH );

    if ( str_len( pellet ) + str_len( output ) < sizeof( output ) - 10 )
      str_cat( output, pellet );

    for ( Touch = FALSE, paf = vi->affected; paf; paf = paf->next )
    {
      if ( !( pSkill = get_skill( paf->type ) ) )
      {
        mudlog( LOG_DEBUG, "do_score: 錯誤的型態 %d.", paf->type );
        continue;
      }

      if ( !Touch )
      {
        Touch = TRUE;

        tablize( FORMAT_SET,
           "\e[1;33m│\e[1;32m法術效應\e[1;33m│\e[0mSKILLNAME    SKILLCNAME   SKILLAFFECT        "
           "                   \e[1;33m│\e[0m\n\r" , 0, NULL, 0 );
      }

      else
      {
        tablize( FORMAT_SET,
           "\e[1;33m│        │\e[0mSKILLNAME    SKILLCNAME   SKILLAFFECT        "
           "                   \e[1;33m│\e[0m\n\r" , 0, NULL, 0 );
      }

      tablize( FORMAT_STRING, pSkill->name,  0, "SKILLNAME",  12 );
      tablize( FORMAT_STRING, pSkill->cname, 0, "SKILLCNAME", 12 );

      if ( ch->level > 20 )
      {
        sprintf( buf,
          "\e[0m 影響 \e[1;37m%s %d\e[0m 點﹐時間 \e[1;37m%d\e[0m 小時",
          affect_loc_name( paf->location ),
          paf->modifier,
          paf->duration );
        tablize( FORMAT_STRING, buf, 0, "SKILLAFFECT", 37 );
      }

      else
      {
        tablize( FORMAT_STRING, " ", 0, "SKILLAFFECT", 37 );
      }

      tablize( FORMAT_RETURN, pellet, 0, NULL, MAX_STRING_LENGTH );

      if ( str_len( pellet ) + str_len( output ) < sizeof( output ) - 10 )
        str_cat( output, pellet );
    }

    tablize( FORMAT_SET,
      "\e[1;33m╰────┴─────────────────────"
      "───────────╯\e[0m\n\r" , 0, NULL, 0 );

    tablize( FORMAT_RETURN, pellet, 0, NULL, MAX_STRING_LENGTH );

    if ( str_len( pellet ) + str_len( output ) < sizeof( output ) - 10 )
      str_cat( output, pellet );
  }

  else
  {
    tablize( FORMAT_SET,
      "\r\e[1;33m╰─┴────────┴──────────────"
      "────────────╯\e[0m\n\r", 0, NULL, 0 );
    tablize( FORMAT_RETURN, pellet, 0, NULL, MAX_STRING_LENGTH );

    if ( str_len( pellet ) + str_len( output ) < sizeof( output ) - 10 )
      str_cat( output, pellet );
  }

  send_to_char( output, ch );
  RETURN_NULL();
}

FUNCTION( do_time )
{
  char             arg[MAX_INPUT_LENGTH];
  struct timeval   sTime;
  CHAR_DATA      * victim;
  CHAR_DATA      * people;
  int              iTime;

  PUSH_FUNCTION( "do_time" );

  one_argument( argument, arg );

  if ( !arg[0] )
  {
    clear_buffer();
    send_to_buffer( "\n\r你屈指一算﹐才發現原來已經是\e[1;32m%s\e[0m之月"
                    "\e[1;33m%s\e[0m之日\e[1;31m%s\e[0m時了。\n\r"
      , MonthName[time_info.month]
      , DayName[time_info.day]
      , HourName[time_info.hour] );

    send_to_buffer( "%s啟動於 %s。\n\r"
      , mud_name , time_format( mud_boot_time , "%r, %a-%d-%b-%y" ) );

    send_to_buffer( "現在系統的時間是 %s。\n\r"
      , time_format( current_time , "%r, %a-%d-%b-%y" ) );

    if ( reboot_time > 0 ) send_to_buffer( "%s\e[0m下一次重新開機時間是%s。\n\r"
      , mud_name , time_format( reboot_time, "%r, %a-%d-%b-%y" ) );

    if ( shutdown_time > 0 ) send_to_buffer( "%s\e[0m關閉時間是%s。\n\r"
      , mud_name , time_format( shutdown_time, "%r, %a-%d-%b-%y" ) );

    sTime.tv_sec  = current_time - mud_boot_time;
    sTime.tv_usec = 0;

    send_to_buffer( "%s\e[0m到目前為止已經正常運作了%s。\n\r"
      , mud_name, get_worktime_string() );

    print_buffer( ch );
  }

  else
  {
    if ( !str_cmp( arg, "all" ) )
    {
      victim = NULL;
    }

    else
    {
      if ( !( victim = get_char_world( ch, arg ) ) )
      {
        act( "找不到你要查詢的人物 $2$T$0。", ch, NULL, arg, TO_CHAR );
        RETURN_NULL();
      }

      if ( IS_NPC( victim ) )
      {
        act( "$N是非玩家﹐不能查詢上線時間。", ch, NULL, victim, TO_CHAR );
        RETURN_NULL();
      }
    }

    for ( clear_buffer(), people = char_list; people; people = people->next )
    {
      if ( !verify_char( people ) || IS_NPC( people ) || !can_see( ch, people ) )
        continue;

      if ( !victim || people == victim )
      {
        iTime = people->played + current_time - people->logon;

        send_to_buffer( "%s上線總時數為%s。\n\r"
          , mob_name( ch, people ), date_string( iTime ) );

        if ( buffer_full() ) break;
      }
    }

    print_buffer( ch );
    RETURN_NULL();
  }

  RETURN_NULL();
}

FUNCTION( do_weather )
{
  char arg[MAX_INPUT_LENGTH];

  static char * const sky_look[4] =
  {
    "\e[1;36m晴空萬里的\e[0m",
    "\e[1;32m多雲的\e[0m",
    "\e[1;34m下雨的\e[0m",
    "\e[1;33m夜空中帶閃電\e[0m"
  };

  int size = sizeof( sky_look ) / sizeof( sky_look[0] );

  PUSH_FUNCTION( "do_weather" );

  argument = one_argument( argument, arg );

  if ( arg[0] == '\x0' )
  {
    if ( !IS_OUTSIDE( ch ) )
    {
      send_to_char( "\e[1m你無法從室內看到外面的天氣﹗\e[0m\n\r", ch );
    }
    else if ( weather_info.sky < 0 || weather_info.sky >= size )
    {
      send_to_char( "系統發生錯誤﹗\n\r", ch );
    }

    else
    {
      act( "\e[1m天氣是$t$7而且\e[1m$2$T$0"
        , ch, sky_look[weather_info.sky]
        , weather_info.change >= 0 ? "南風徐徐吹來﹗" : "北風冷颼颼的吹﹗"
        , TO_CHAR );
    }
  }

  else if ( !str_prefix( arg, "fog" ) && IS_IMMORTAL( ch ) )
  {
    if ( !FoggyArea )
    {
      send_to_char( "目前沒有任何起霧的區域﹗\n\r", ch );
    }
    else
    {
      act( "目前霧區是$t。", ch, FoggyArea->name, NULL, TO_CHAR );
    }
  }

  else if ( !str_prefix( arg, "set" ) && IS_IMMORTAL( ch ) )
  {
    if ( FoggyRoom <= 0 )
    {
      send_to_char( "出現霧的機率小於零﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( FoggyArea )
    {
      send_to_char( "目前已經有霧區了﹐不需要再手動觸發﹗\n\r", ch );
      RETURN_NULL();
    }

    foggy_update( FOGGY_SET );
    send_to_char( "你手動觸發了大霧的產生﹗\n\r", ch );
  }

  else
  {
    send_to_char( "你的語法錯誤﹐請查詢 weather 的使用方法﹗\n\r", ch );
  }

  RETURN_NULL();
}

FUNCTION( do_help )
{
  CMD_DATA   * pCommand;
  SKILL_DATA * pSkill;
  HELP_DATA  * pHelp;
  int          trust;

  PUSH_FUNCTION( "do_help" );

  if ( !argument[0] ) argument = "summary";

  for ( pHelp = help_first; pHelp; pHelp = pHelp->next )
  {
    if ( pHelp->level > get_trust( ch ) ) continue;

    if ( is_name( argument, pHelp->keyword ) )
    {
      if ( pHelp->level >= 0 ) act( "$t", ch, pHelp->keyword, NULL, TO_CHAR );

      if ( pHelp->text[0] == '.' ) send_to_char( pHelp->text+1, ch );
      else                         send_to_char( pHelp->text  , ch );
      RETURN_NULL();
    }
  }

  trust = get_trust( ch );

  for ( pCommand = cmd_list; pCommand; pCommand = pCommand->next )
  {
    if ( pCommand->level <= trust
      && !pCommand->mobonly
      && !str_prefix( argument, pCommand->name )
      && pCommand->help
      && pCommand->help != &str_empty[0] )
    {
      send_to_char( pCommand->help, ch );
      RETURN_NULL();
    }
  }

  if ( ( pSkill = skill_isname( argument ) )
    && pSkill->help
    && pSkill->help != &str_empty[0] )
  {
    send_to_char( pSkill->help, ch );
    RETURN_NULL();
  }

  send_to_char( "對不起﹐線上沒有這個輔助說明。請向管理者反應﹐謝謝。\n\r", ch );
  RETURN_NULL();
}

FUNCTION( do_who )
{
  DESCRIPTOR_DATA * d;
  DESCRIPTOR_DATA ** pMem;
  CLASS_DATA      * pClass = NULL;
  CLUB_DATA       * pClub  = NULL;
  CHAR_DATA       * wch;
  char            * rank;
  char              arg[ MAX_INPUT_LENGTH ];
  char              buf[MAX_STRING_LENGTH];
  char              name[BUFSIZ];
  int               iLevelLower = 0;
  int               iLevelUpper = MAX_LEVEL;
  int               nMatch;
  int               count;
  int               loop;
  int               iLen;
  bool              NotPrintLong;
  bool              fClassRestrict = FALSE;
  bool              fClubRestrict  = FALSE;
  bool              fImmortalOnly  = FALSE;
  bool              CheckNumber    = FALSE;
  bool              Touch          = FALSE;
  bool              bSend;
  bool              fHeroRestrict  = FALSE;

  PUSH_FUNCTION( "do_who" );

  for ( NotPrintLong = TRUE; *argument; NotPrintLong = FALSE )
  {
    argument = one_argument( argument , arg );

    if ( !arg[0] ) break;

    if ( arg[0] != '-' )
    {
      send_to_char( "那不是一個正確的查詢方式﹐請查詢 who 的用法﹗\n\r", ch );
      RETURN_NULL();
    }

    switch( LOWER( arg[1] ) )
    {
    default:
      send_to_char( "那不是一個正確的參數﹐請查詢 who 的用法﹗\n\r", ch );
      RETURN_NULL();

    case 'n':

      CheckNumber = TRUE;
      break;

    case 'w':

      fImmortalOnly = TRUE;
      break;

    case 'c':

      if ( !( pClass = arg[2] ? is_class( arg+2, FALSE ) : ch->class ) )
      {
        act( "$t不是一種職業﹐請查詢 who 的用法。", ch, arg + 2, NULL, TO_CHAR );
        RETURN_NULL();
      }

      fClassRestrict = TRUE;

      break;

    case 'h':

      fHeroRestrict = TRUE;
      break;

    case 'l':

      NotPrintLong = FALSE;
      break;

    case 'a':

      if ( !arg[2] || !is_number( arg + 2 ) )
      {
        send_to_char( "請輸入數字查詢。\n\r", ch );
        RETURN_NULL();
      }

      if ( ( iLevelLower = atoi( arg + 2 ) ) < 0 || iLevelLower > MAX_LEVEL )
      {
        send_to_char( "輸入的等級不合理。\n\r", ch );
        RETURN_NULL();
      }
      break;

    case 'b':

      if ( !arg[2] || !is_number( arg + 2 ) )
      {
        send_to_char( "請輸入數字查詢。\n\r", ch );
        RETURN_NULL();
      }

      if ( ( iLevelUpper = atoi( arg + 2 ) ) < 0 || iLevelUpper > MAX_LEVEL )
      {
        send_to_char( "輸入的等級不合理。\n\r", ch );
        RETURN_NULL();
      }

      break;

    case 'g':

      iLevelLower = UMAX(          1, ch->level - 5 );
      iLevelUpper = UMIN( LEVEL_HERO, ch->level + 5 );
      break;

    case 'z':

      pClub = arg[2] == '\x0' ? ch->club : clubname_lookup( arg+2, CLUB_NAME );

      if ( !pClub )
      {
        send_to_char( "找不到你要看的幫派名稱。\n\r", ch );
        RETURN_NULL();
      }

      fClubRestrict = TRUE;
      break;
    }
  }

  if ( iLevelUpper < iLevelLower )
  {
    int temp;

    temp        = iLevelUpper;
    iLevelUpper = iLevelLower;
    iLevelLower = temp;
  }

  /* 先計算有幾個描述表 */
  for ( count = 0, d = descriptor_list; d; d = d->next )
  {
    if ( verify_desc( d )
      && d->character
      && d->connected == CON_PLAYING
      && can_see( ch, d->character ) )
      count++;
  }

  /* 配置記憶體 */
  if ( !( pMem = alloc_string( count * sizeof( DESCRIPTOR_DATA * ) ) ) )
  {
    mudlog( LOG_DEBUG, "do_who: 無法配置記憶體." );
    send_to_char( "系統錯誤﹐無法查詢﹗\n\r", ch );
    RETURN_NULL();
  }

  /* 設定變數表 */
  for ( loop = 0, d = descriptor_list; loop < count; d = d->next )
  {
    if ( verify_desc( d )
      && d->character
      && d->connected == CON_PLAYING
      && can_see( ch, d->character ) )
      pMem[loop++] = d;
  }

  /* 排序 */
  qsort( pMem, count, sizeof( pMem[0] ), who_compare );

  if ( !CheckNumber )
  {
    tablize( FORMAT_SET,
    "\e[1;32m╭───                     \e[1;32m玩家列表"
    " ────────────────────╮\n\r\e[0m", 0, NULL, 0 );

    tablize( FORMAT_LOCATE_STRING, mud_name, 0, "8", 20 );
    tablize( FORMAT_RETURN, buf, 0, NULL, MAX_STRING_LENGTH );
    send_to_buffer( "%s", buf );
  }

  for ( bSend = TRUE, nMatch = loop = 0; loop < count; loop++ )
  {
    wch = pMem[loop]->character;

    if ( wch->level < iLevelLower
      || wch->level > iLevelUpper
      || ( fImmortalOnly  && wch->level < LEVEL_IMMORTAL )
      || ( fClassRestrict && wch->class != pClass )
      || ( fClubRestrict && wch->club != pClub )
      || ( fHeroRestrict && !IS_HERO( wch ) ) ) continue;

    nMatch++;

         if ( CheckNumber ) continue;
    else if ( NotPrintLong )
    {
      int order;

      if ( ( nMatch % NAME_ROW ) == 1 )
      {
        bSend = Touch = FALSE;
        tablize( FORMAT_SET, "\e[1;32m│\e[0m                                   "
          "                                         \e[1;32m│\e[0m\n\r"
          , 0 , NULL, 0 );
      }

      if ( ( order = nMatch % NAME_ROW ) == 0 ) order = NAME_ROW;

      sprintf( arg, "%d", order * 12 - 8 );

      if ( is_friend( ch, wch->name ) )
        sprintf( name, "\e[1;33m%s\e[0m", wch->name );

      else if ( is_badfriend( ch, wch->name ) )
        sprintf( name, "\e[0;37m%s\e[0m", wch->name );

      else
        str_cpy( name, wch->name );

      tablize( FORMAT_LOCATE_STRING, name, 0, arg, 12 );

      Touch = TRUE;
      if ( ( nMatch % NAME_ROW ) == 0 ) bSend = TRUE;
    }

    else
    {
      rank = class_name( wch->class, FALSE );

      if ( !fClubRestrict )
      {
        switch ( wch->level )
        {
        default:
          break;

        case MAX_LEVEL :
          rank = "\e[1;33;44m創世神\e[0m";
          break;

        case MAX_LEVEL - 1:
          rank = "\e[1;32m造世神\e[0m";
          break;

        case MAX_LEVEL - 2:
          rank = "\e[1;32m大巫神\e[0m";
          break;

        case MAX_LEVEL - 3:
          rank = "\e[1;32m實習巫神\e[0m";
          break;

        case MAX_LEVEL - 4:
          rank = "\e[1;32m大天使\e[0m";
          break;

        case MAX_LEVEL - 5:
          rank = "\e[1;32m實習天使\e[0m";
          break;
        }
      }

      tablize( FORMAT_SET,
        "\e[1;32m│             \e[1;32m│\e[0m                   "
        "                                          \e[1;32m│\n\r\e[0m"
        , 0, NULL, 0 );

      tablize( FORMAT_LOCATE_NUM, NULL, wch->level, "3", 3 );

      tablize( FORMAT_LOCATE_STRING
       , fClubRestrict ? club_class( wch, pClub ) : rank, 0, "7", 8 );

      sprintf( buf, "[0m%s%s%s %s \e[0m( %s ) %s [0m"
        , IS_SET(wch->act, PLR_KILLER ) ? "\e[1;31m(兇手)\e[0m" : ""
        , IS_SET(wch->act, PLR_THIEF  ) ? "\e[1;32m(小偷)\e[0m" : ""
        , IS_SET(wch->act, PLR_BOLTER ) ? "\e[1;32m(叛幫)\e[0m" : ""
        , wch->cname , wch->name , wch->pcdata->title );
      tablize( FORMAT_LOCATE_STRING, buf, 0, "18", 56 );
      bSend = TRUE;
    }

    if ( bSend )
    {
      tablize( FORMAT_RETURN, buf, 0, NULL, MAX_STRING_LENGTH );
      send_to_buffer( "%s", buf );
      bSend = FALSE;
      Touch = FALSE;
    }
  }

  if ( CheckNumber )
  {
    act( "現在有 $4$i$0位玩家在$2$T$0之中。", ch, &nMatch, mud_name, TO_CHAR );
    RETURN_NULL();
  }

  if ( !bSend && NotPrintLong && Touch )
  {
    tablize( FORMAT_RETURN, buf, 0, NULL, MAX_STRING_LENGTH );
    send_to_buffer( "%s", buf );
  }

  tablize( FORMAT_SET,
     "\e[1;32m\r╰─你看到 \e[1;34m   \e[0m (\e[1;37m    \e[0m) "
    "\e[1;32m位玩家正在──────────\e[1;32m──────"
    "───────╯\n\r\e[0m", 0, NULL, 0 );

  tablize( FORMAT_LOCATE_NUM, NULL, nMatch    , "12", 3 );
  tablize( FORMAT_LOCATE_NUM, NULL, MaxConnect, "18", 3 );

  if ( ( iLen = str_len( mud_name ) ) % 2 != 0 ) iLen++;
  tablize( FORMAT_LOCATE_STRING, mud_name, 0, "33", iLen );
  tablize( FORMAT_RETURN, buf, 0, NULL, MAX_STRING_LENGTH );

  send_to_buffer( "%s", buf );
  print_buffer( ch );

  /* 釋放記憶體 */
  free_string( ( void * ) pMem );

  RETURN_NULL();
}

/* 傳回使用者的人數 */
size_t get_usernum( void )
{
  DESCRIPTOR_DATA * d;
  size_t            nUser;

  PUSH_FUNCTION( "get_usernum" );

  for ( nUser = 0 , d = descriptor_list; d; d = d->next )
  {
    if ( verify_desc( d )
      && d->character
      && d->connected == CON_PLAYING
      && !IS_SET( d->character->act , PLR_WIZINVIS ) )
    nUser++;
  }

  RETURN( nUser );
}

/* 傳回嘗試連線的人數 */
size_t get_attemptnum( void )
{
  DESCRIPTOR_DATA * d;
  size_t            num;

  PUSH_FUNCTION( "get_attemptnum" );

  for ( num = 0 , d = descriptor_list; d; d = d->next )
    if ( verify_desc( d ) && d->connected != CON_PLAYING ) num++;

  RETURN( num );
}

size_t get_total_user( void )
{
  DESCRIPTOR_DATA * man;
  size_t            num;

  PUSH_FUNCTION( "get_total_user" );

  for ( num = 0, man = descriptor_list; man; man = man->next )
    if ( verify_desc( man ) ) num++;

  RETURN( num );
}

FUNCTION( do_inventory )
{
  CHAR_DATA * victim;
  char        arg[MAX_INPUT_LENGTH];
  char        buf[MAX_STRING_LENGTH];

  PUSH_FUNCTION( "do_inventory" );

  one_argument( argument, arg );

  if ( !arg[0] )
  {
    victim = ch;
    sprintf( buf, "%4d/%4d", get_carry_weight( ch ), can_carry_w( ch ) );
    act( "你身上目前帶有﹕「負重量﹕ $t」$A", ch, buf, NULL, TO_CHAR );
  }

  else
  {
    if ( !( victim = get_char_world( ch, arg ) ) )
    {
      act( "找不到你要觀看的對象 $2$T$0。", ch, NULL, arg, TO_CHAR );
      RETURN_NULL();
    }

    if ( !biliwick( ch, victim ) )
    {
      act( "你的權力不夠看$N的裝備。", ch, NULL, victim, TO_CHAR );
      RETURN_NULL();
    }

    sprintf( buf, "%4d/%4d", get_carry_weight( victim ), can_carry_w( victim ) );
    act( "$N身上目前帶有﹕「負重量﹕$t 」$A", ch, buf, victim, TO_CHAR );
  }

  show_list_to_char( victim->carrying, ch, TRUE, TRUE, TRUE );

  if ( victim->firman > 0 )
  {
    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_EXACT ) )
      act( "$i張免死金牌(firman)", ch, &victim->firman, NULL, TO_CHAR );
    else
      act( "$i張免死金牌", ch, &victim->firman, NULL, TO_CHAR );
  }

  RETURN_NULL();
}

FUNCTION( do_equipment )
{
  OBJ_INDEX_DATA * pObjIndex;
  OBJ_INDEX_DATA * zObjIndex;
  SITUS_DATA     * pSitus;
  OBJ_DATA       * obj;
  CHAR_DATA      * mount;
  CHAR_DATA      * victim;
  char             arg[MAX_INPUT_LENGTH];
  int              vnum;
  int              nMatch;
  int              count;
  int              level;
  int              total;
  bool             found;

  PUSH_FUNCTION( "do_equipment" );

  argument = one_argument( argument, arg );

  if ( !arg[0] )
  {
    victim = ch;
    act( "你正裝備著﹕「裝備吸收程度百分之$i」\n\r"
      , ch, &victim->eq_capcity, NULL, TO_CHAR );
  }

  else if ( !str_prefix( arg, "!search" ) )
  {
    if ( ch->gold < EQSearchCost )
    {
      act( "等你湊足了$x兩黃金才能查詢﹗", ch, &EQSearchCost, NULL, TO_CHAR );
      RETURN_NULL();
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\x0' )
    {
      send_to_char( "各位置裝備的英文對照表如下﹕\n\r\n\r"
        "戒指類 \e[1;32mfinger\e[0m  項鍊類 \e[1;32mneck\e[0m    "
        "衣服類 \e[1;32mbody\e[0m    頭盔類 \e[1;32mhead\e[0m\n\r"
        "護膝類 \e[1;32mlegs\e[0m    鞋子類 \e[1;32mfeet\e[0m    "
        "手套類 \e[1;32mhands\e[0m   護臂類 \e[1;32marms\e[0m\n\r"
        "盾牌類 \e[1;32mshield\e[0m  衣袍類 \e[1;32mabout\e[0m   "
        "腰帶類 \e[1;32mwaist\e[0m   手腕類 \e[1;32mwrist\e[0m\n\r"
        "耳環類 \e[1;32mearring\e[0m 武器類 \e[1;32mhold\e[0m    "
        "武器類 \e[1;32mwield\e[0m   背袋類 \e[1;32mback\e[0m\n\r"
        , ch );

      RETURN_NULL();
    }

    for ( pSitus = situs_list; pSitus; pSitus = pSitus->next )
    {
      if ( !str_prefix( arg, pSitus->name ) )
      {
        clear_buffer();
        level = -1;
        gold_from_char( ch, 100 );

        for ( nMatch = vnum = 0; nMatch < top_obj_index; vnum++ )
        {
          if ( ( pObjIndex = get_obj_index( vnum ) ) )
          {
            nMatch++;

            if ( pObjIndex->count > 0
              && ( pObjIndex->wear_flags & pSitus->wear )
              && ch->level >= pObjIndex->level )
            {
              if ( level < pObjIndex->level ) level = pObjIndex->level;
            }
          }
        }

        if ( level < 0 )
        {
          send_to_buffer( "在你的等級內﹐沒有找到指定的裝備資料﹗\n\r" );
          RETURN_NULL();
        }

        zObjIndex = NULL;
        for ( count = nMatch = vnum = 0; nMatch < top_obj_index; vnum++ )
        {
          if ( ( pObjIndex = get_obj_index( vnum ) ) )
          {
            nMatch++;
            if ( pObjIndex->count > 0
              && ( pObjIndex->wear_flags & pSitus->wear )
              && pObjIndex->level == level
              && number_range( 0, count++ ) == 0 )
            {
              zObjIndex = pObjIndex;
            }
          }
        }

        if ( zObjIndex )
        {
          send_to_buffer( "傳說中有種裝備名喚\e[1;32m%s(%s)\e[0m﹐可能是"
            "你需要的﹐你趕快去找找﹗\n\r"
            , zObjIndex->short_descr, zObjIndex->name );
        }

        else
        {
          send_to_buffer( "在你的等級內﹐沒有找到指定的裝備資料﹗\n\r" );
        }

        print_buffer( ch );
        RETURN_NULL();
      }
    }

    send_to_char( "各位置裝備的英文對照表如下﹕\n\r\n\r"
      "戒指類 \e[1;32mfinger\e[0m  項鍊類 \e[1;32mneck\e[0m    "
      "衣服類 \e[1;32mbody\e[0m    頭盔類 \e[1;32mhead\e[0m\n\r"
      "護膝類 \e[1;32mlegs\e[0m    鞋子類 \e[1;32mfeet\e[0m    "
      "手套類 \e[1;32mhands\e[0m   護臂類 \e[1;32marms\e[0m\n\r"
      "盾牌類 \e[1;32mshield\e[0m  衣袍類 \e[1;32mabout\e[0m   "
      "腰帶類 \e[1;32mwaist\e[0m   手腕類 \e[1;32mwrist\e[0m\n\r"
      "耳環類 \e[1;32mearring\e[0m 武器類 \e[1;32mhold\e[0m    "
      "武器類 \e[1;32mwield\e[0m   背袋類 \e[1;32mback\e[0m\n\r"
      , ch );

    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "!info" ) )
  {
    clear_buffer();
    found = FALSE;
    total = 0;

    for ( pSitus = situs_list; pSitus; pSitus = pSitus->next )
      total += pSitus->chance;

    if ( total <= 0 ) total = 1;

    for ( pSitus = situs_list; pSitus; pSitus = pSitus->next )
    {
      send_to_buffer( "%s", pSitus->eq_name );

      if ( IS_IMMORTAL( ch ) )
      {
        send_to_buffer( "%4d( %3d%% ) "
          , pSitus->chance, pSitus->chance * 100 / total );
      }

      if ( ( obj = get_eq_char( ch, pSitus->location ) ) )
      {
        send_to_buffer( "%s\n\r"
          , can_see_obj( ch, obj )
            ? format_obj_to_char( obj, ch, TRUE, TRUE )
            : "某件看不見的東西。" );
      }

      else
      {
        send_to_buffer( "（\e[1;31m從缺\e[0m）\n\r" );
      }
    }

    if ( ( mount = get_mount( ch ) ) )
      send_to_buffer( "[座騎]     %s\n\r", mob_name( ch, mount ) );
    else
      send_to_buffer( "[座騎]     （\e[1;32m從缺\e[0m）\n\r" );

    print_buffer( ch );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "!set" ) )
  {
    int degree;

    one_argument( argument, arg );
    if ( arg[0] == '\x0' || !is_number( arg ) )
    {
      send_to_char( "你必須註明裝備的吸收程度﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( ( degree = atoi( arg ) ) < eq_min_capcity || degree > eq_max_capcity )
    {
      act( "對不起﹐裝備的吸收程度只能由百分之$i到$I。"
        , ch, &eq_min_capcity, &eq_max_capcity, TO_CHAR );

      RETURN_NULL();
    }

    ch->eq_capcity = degree;
    act( "你設定裝備吸收程度為百分之$i。", ch, &degree, NULL, TO_CHAR );
    RETURN_NULL();
  }

  else
  {
    if ( !( victim = get_char_world( ch, arg ) ) )
    {
      act( "找不到你要觀看的對象 $2$T$0。", ch, NULL, arg, TO_CHAR );
      RETURN_NULL();
    }

    if ( IS_NPC( victim ) )
    {
      act( "你不能觀看$N的資料。", ch, NULL, victim, TO_CHAR );
      RETURN_NULL();
    }

    if ( !biliwick( ch, victim ) )
    {
      act( "你的權力不夠看$N的裝備。", ch, NULL, victim, TO_CHAR );
      RETURN_NULL();
    }

    act( "$N正裝備著﹕「裝備吸收程度百分之$i」"
      , ch, &victim->eq_capcity, victim, TO_CHAR );
  }

  clear_buffer();
  found = FALSE;

  for ( pSitus = situs_list; pSitus; pSitus = pSitus->next )
  {
    if ( !( obj = get_eq_char( victim, pSitus->location ) ) ) continue;

    send_to_buffer( "%s%s\n\r"
      , pSitus->eq_name
      , can_see_obj( victim, obj )
        ? format_obj_to_char( obj, victim, TRUE, TRUE )
        : "某件看不見的東西。" );

    found = TRUE;
  }

  if ( ( mount = get_mount( victim ) ) )
  {
    send_to_buffer( "[座騎]     %s\n\r", mob_name( ch, mount ) );
    found = TRUE;
  }

  if ( !found ) send_to_buffer( "沒有任何裝備﹗\n\r", ch );
  print_buffer( ch );
  RETURN_NULL();
}

FUNCTION( do_compare )
{
  char       arg1[MAX_INPUT_LENGTH];
  char       arg2[MAX_INPUT_LENGTH];
  OBJ_DATA * obj1;
  OBJ_DATA * obj2;
  int        value1;
  int        value2;
  char     * msg;

  PUSH_FUNCTION( "do_compare" );

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

  if ( !arg1[0] )
  {
    send_to_char( "你要比較哪兩樣東西﹖\n\r", ch );
    RETURN_NULL();
  }

  if ( !( obj1 = get_obj_carry( ch, arg1 ) ) )
  {
    act( "你沒有這個東西 $2$T$0喔﹗", ch, NULL, arg1, TO_CHAR );
    RETURN_NULL();
  }

  if ( !arg2[0] )
  {
    for ( obj2 = ch->carrying; obj2; obj2 = obj2->next_content )
    {
      if ( obj2->wear_loc != WEAR_NONE
        && can_see_obj( ch, obj2 )
        && obj1->item_type == obj2->item_type )
          break;
    }

    if ( !obj2 )
    {
      send_to_char( "你身上沒有那種東西可以比較。\n\r", ch );
      RETURN_NULL();
    }
  }
  else
  {
    if ( !( obj2 = get_obj_carry( ch, arg2 ) ) )
    {
      act( "你沒有這樣裝備 $2$T$0。", ch, NULL, arg2, TO_CHAR );
      RETURN_NULL();
    }
  }

  /* 物品是否有餵毒 */
  if ( poison_char( ch, obj1 ) ) RETURN_NULL();
  if ( poison_char( ch, obj2 ) ) RETURN_NULL();

  msg    = NULL;
  value1 = value2 = 0;

  if ( obj1 == obj2 )
  {
    msg = "你拿著$p和$p來比較﹐它們兩個不是一樣嗎﹖";
  }
  else if ( obj1->item_type != obj2->item_type )
  {
    msg = "你無法拿$p和$P比較﹐因為它們是不同種類的東西。";
  }
  else
  {
    switch ( obj1->item_type )
    {
    default:
      msg = "你沒有辦法比較$p和$P。";
      break;

    case ITEM_ARMOR:
      value1 = obj1->value[0];
      value2 = obj2->value[0];
      break;

    case ITEM_WEAPON:
      value1 = obj1->value[1] + obj1->value[2];
      value2 = obj2->value[1] + obj2->value[2];
      break;

    case ITEM_ARROW:
      value1 = obj1->value[0] + obj1->value[1];
      value2 = obj2->value[0] + obj2->value[1];
      break;
    }
  }

  if ( !msg )
  {
         if ( value1 == value2 ) msg = "$p和$P看起來差不多。";
    else if ( value1  > value2 ) msg = "$p看起來比$P好。";
    else                         msg = "$p看起來比$P差。";
  }

  act( msg, ch, obj1, obj2, TO_CHAR );

  message_driver( ch, obj1, ACT_WHEN_COMPARE );
  message_driver( ch, obj2, ACT_WHEN_COMPARE );

  RETURN_NULL();
}

FUNCTION( do_where )
{
  char              arg[MAX_INPUT_LENGTH];
  char              buf[MAX_STRING_LENGTH];
  CHAR_DATA       * victim;
  DESCRIPTOR_DATA * d;
  bool              found;
  int               gold;

  PUSH_FUNCTION( "do_where" );

  if ( IS_NPC( ch ) || !ch->in_room ) RETURN_NULL();

  if ( ch->in_room->NoWhere )
  {
    act( "在$r是沒有辦法打聽到別人的行蹤的﹗", ch, NULL, NULL, TO_CHAR );
    RETURN_NULL();
  }

  one_argument( argument, arg );

  if ( ch->gold < ( gold = search_cost( ch ) ) )
  {
    act( "小狗對你搖了搖頭說道﹕你至少要有$i元的包子才行幫你找人。"
      , ch, &gold, NULL, TO_CHAR );

    RETURN_NULL();
  }

  gold_from_char( ch, gold );

  clear_buffer();
  chinese_number( gold, buf );
  send_to_buffer( "你把你身上的%s兩銀子換成包子﹐用味道請來附近的小"
                  "狗打聽附近的人﹐發現有﹕\n\r" , buf );

  if ( !arg[0] )
  {
    for ( found = FALSE , d = descriptor_list; d; d = d->next )
    {
      if ( !verify_desc( d ) ) continue;

      if ( d->connected == CON_PLAYING
        && ( victim = d->character )
        && !IS_NPC( victim )
        && victim->in_room
        && victim->in_room->NoWhere == FALSE
        && victim->in_room->area == ch->in_room->area
        && can_see( ch, victim )
        && can_see_mask( ch, victim )
        && ch != victim )
      {
        gold  = search_cost( ch );
        found = TRUE;
        if ( ch->gold >= gold )
        {
          chinese_number( gold, buf );
          gold_from_char( ch, gold );
          send_to_buffer( "你花了%s兩銀子打聽到%s正在%s附近出沒。\n\r",
            buf, mob_name( ch, victim ), victim->in_room->name );

          if ( buffer_full() ) break;
        }

        else
        {
          send_to_buffer( "小狗對你說﹕「你的包子不夠﹐我不找了﹐BYEBYE﹗」\n\r" );
          break;
        }
      }
    }

    if ( !found ) send_to_buffer( "對不起﹐沒有任何玩家靠近你﹗\n\r" );
  }

  else
  {
    for ( found = FALSE, victim = char_list; victim; victim = victim->next )
    {
      if ( !verify_char( victim ) || !victim->in_room ) continue;

      if ( victim->in_room
        && victim->in_room->NoWhere == FALSE
        && victim->in_room->area == ch->in_room->area
        && !is_affected( victim, SLOT_HIDE )
        && !is_affected( victim, SLOT_SNEAK )
        && can_see( ch, victim )
        && can_see_mask( ch, victim )
        && is_name( arg, victim->name ) )
      {
        gold  = search_cost( ch );
        found = TRUE;
        if ( ch->gold >= gold )
        {
          found = TRUE;
          gold_from_char( ch, gold );
          chinese_number( gold, buf );
          send_to_buffer( "你花了%s兩銀子打聽到%s\e[0m正在%s附近出沒。\n\r"
            , buf , mob_name( ch, victim ), victim->in_room->name );

          if ( buffer_full() ) break;
        }

        else
        {
          send_to_buffer( "小狗對你說﹕「你的包子不夠﹐我不找了﹐BYEBYE﹗」\n\r" );
          break;
        }
      }
    }
    if ( !found ) send_to_buffer( "對不起﹐你沒有發現任何的%s﹗\n\r", arg );
  }

  print_buffer( ch );
  RETURN_NULL();
}

FUNCTION( do_lore )
{
  CHAR_DATA * victim;
  char        buf[MAX_STRING_LENGTH];
  char        arg[MAX_INPUT_LENGTH];

  PUSH_FUNCTION( "do_lore" );

  one_argument( argument, arg );

  if ( !( victim = get_char_room( ch, arg ) ) )
  {
    act( "你想評量的對象 $2$T$0 不在這裡。", ch, NULL, arg, TO_CHAR );
    RETURN_NULL();
  }

  if ( !IS_NPC( victim )
    || ( IS_NPC( victim ) && IS_SET( victim->act, ACT_NOKILL ) ) )
  {
    send_to_char( "天神不允許\你有這種邪惡的思想。\n\r", ch );
    RETURN_NULL();
  }

  if ( victim->mount_by )
  {
    act( "$N是別人的坐騎耶﹗", ch, NULL, victim, TO_CHAR );
    RETURN_NULL();
  }

  if ( victim->boss )
  {
    act( "$N是別人養的小鬼耶﹗", ch, NULL, victim, TO_CHAR );
    RETURN_NULL();
  }

  str_cpy( buf, return_array( LevelDiffMessage, victim->level - ch->level
            , sizeof( LevelDiffMessage ) / sizeof( LevelDiffMessage[0] ) ) );

  act( smash_act_keyword( buf, "nesNES" ), ch, NULL, victim, TO_CHAR );
  RETURN_NULL();
}

FUNCTION( do_consider )
{
  char        arg[MAX_INPUT_LENGTH];
  char        buf[MAX_STRING_LENGTH];
  CHAR_DATA * victim;
  int         diff;
  int         hpdiff;

  PUSH_FUNCTION( "do_consider" );

  one_argument( argument, arg );

  if ( !( victim = get_char_room( ch, arg ) ) )
  {
    act( "你想評量的對象 $2$T$0 不在這裡。", ch, NULL, arg, TO_CHAR );
    RETURN_NULL();
  }

  if ( !IS_NPC( victim )
    || ( IS_NPC( victim ) && IS_SET( victim->act, ACT_NOKILL ) ) )
  {
    send_to_char( "天神不允許\你有這種邪惡的思想。\n\r", ch );
    RETURN_NULL();
  }

  if ( victim->mount_by )
  {
    act( "$N是別人的坐騎耶﹗", ch, NULL, victim, TO_CHAR );
    RETURN_NULL();
  }

  if ( victim->boss )
  {
    act( "$N是別人養的小鬼耶﹗", ch, NULL, victim, TO_CHAR );
    RETURN_NULL();
  }

  diff = victim->level - ch->level;

  str_cpy( buf, return_array( LevelDiff2Message, -diff
            , sizeof( LevelDiff2Message ) / sizeof( LevelDiff2Message[0] ) ) );

  act( smash_act_keyword( buf, "nesNES" ) , ch, NULL, victim, TO_CHAR );

  hpdiff = ch->hit - victim->hit;

  if ( ( ( diff >= 0 ) && ( hpdiff <= 0 ) )
    || ( ( diff <= 0 ) && ( hpdiff >= 0 ) ) )
  {
    send_to_char( "而且﹐", ch );
  }
  else
  {
    send_to_char( "但是﹐", ch );
  }

  str_cpy( buf, return_array( HpMessage, hpdiff
     , sizeof( HpMessage ) / sizeof( HpMessage[0] ) ) );

  act( smash_act_keyword( buf, "NESnes" ), ch, NULL, victim, TO_CHAR );
  RETURN_NULL();
}

void set_title( CHAR_DATA * ch, char * title )
{
  PUSH_FUNCTION( "set_title" );

  if ( IS_NPC( ch ) || !ch->pcdata || !ch->pcdata->title || !title )
    RETURN_NULL();

  free_string( ch->pcdata->title );
  ch->pcdata->title = str_dup( title );
  RETURN_NULL();
}

FUNCTION( do_cname )
{
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];

  PUSH_FUNCTION( "do_cname" );

  if ( IS_NPC( ch ) || !ch->pcdata ) RETURN_NULL();

  if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_SILENCE ) )
  {
    send_to_char( "對不起﹐你失去了改名的資格﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( argument[0] == '\x0' )
  {
    if ( !ch->cname || !*ch->cname )
      act( "你目前沒有中文名字﹐你還有$i次改名字的機會。"
        , ch, &ch->pcdata->rechristen, NULL, TO_CHAR );

    else
      act( "你的中文名字為 $t﹐你還有$I次改名字的機會。"
        , ch, ch->cname, &ch->pcdata->rechristen, TO_CHAR );

    RETURN_NULL();
  }

  if ( ch->pcdata->rechristen <= 0 )
  {
    send_to_char( "對不起﹐你沒有機會可以改你的中文名稱囉﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( MaxCNameLen < MAX_INPUT_LENGTH && str_len( argument ) > MaxCNameLen )
    argument[ MaxCNameLen ] = '\x0';

  if ( str_len( argument ) < MinCNameLen )
  {
    send_to_char( "嗯﹐你的名字也太短了吧﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( check_cname_xname( argument ) )
  {
    send_to_char( "這個名字不好吧﹗換個名字看看。\n\r", ch );
    RETURN_NULL();
  }

  filter_ansi( argument );
  ansi_transcribe( argument, buf );
  smash_tilde( buf );
  smash_point( buf );
  fix_color( buf );

  sprintf( buf2, "%s從此改名換姓為%s\e[0m﹐這個消息很快就傳了開來﹗"
    , mob_name( NULL, ch ), buf );

  free_string( ch->cname );

  ch->cname = str_dup( buf );
  ch->pcdata->rechristen--;

  act( "你把你的中文名字改成$t﹐你還有$I次機會改中文名稱。"
    , ch, ch->cname, &ch->pcdata->rechristen, TO_CHAR );

  talk_channel_2( buf2, CHANNEL_BULLETIN, "快報" );

  RETURN_NULL();
}

FUNCTION( do_email )
{
  PUSH_FUNCTION( "do_email" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  if ( argument[0] == '\x0' )
  {
    if ( !ch->email || !*ch->email )
      send_to_char( "你目前沒有電子郵件信箱位址﹗\n\r", ch );

    else
      act( "你的電子郵件信箱位址為 $t。", ch, ch->email, NULL, TO_CHAR );

    RETURN_NULL();
  }

  if ( str_len( argument ) > 50 ) argument[50] = '\x0';

  if ( str_len( argument ) < 10 )
  {
    send_to_char( "你的電子信箱的位址也太短了吧﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( !check_email_address( argument ) )
  {
    send_to_char( "你的電子郵件信箱位址不合法﹗\n\r", ch );
    RETURN_NULL();
  }

  smash_tilde( argument );
  smash_point( argument );
  free_string( ch->email );
  ch->email = str_dup( argument );
  act( "你把你的電子信箱位址改成 $t。" , ch, ch->email, NULL, TO_CHAR );
  RETURN_NULL();
}

FUNCTION( do_title )
{
  char buf[MAX_STRING_LENGTH];

  PUSH_FUNCTION( "do_title" );

  if ( IS_NPC( ch ) || !ch->pcdata ) RETURN_NULL();

  if ( argument[0] == '\x0' )
  {
    if ( !ch->pcdata->title || !*ch->pcdata->title )
      send_to_char( "你沒有任何抬頭名稱﹗\n\r", ch );

    else
      act( "你的抬頭名稱為 $t。", ch, ch->pcdata->title, NULL, TO_CHAR );

    RETURN_NULL();
  }

  if ( MaxCNameLen < sizeof( argument ) && str_len( argument ) > MaxCNameLen )
    argument[ MaxCNameLen ] = '\x0';

  if ( str_len( argument ) < MinCNameLen )
  {
    send_to_char( "你的抬頭也太短了吧﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( check_cname_xname( argument ) )
  {
    send_to_char( "這個抬頭不好吧﹗換個抬頭看看。\n\r", ch );
    RETURN_NULL();
  }

  ansi_transcribe( argument, buf );
  filter_ansi( buf );
  smash_tilde( buf );
  smash_point( buf );
  fix_color( buf );
  set_title( ch, buf );
  act( "你把你的抬頭改成 $t。" , ch, ch->pcdata->title, NULL, TO_CHAR );
  RETURN_NULL();
}

FUNCTION( do_description )
{
  char arg[MAX_STRING_LENGTH];

  PUSH_FUNCTION( "do_description" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  argument = one_argument( argument, arg );

  if ( arg[0] == '\x0' || !str_prefix( arg, "show" ) )
  {
    act( "你的描述是﹕$A$t$0", ch
      , ch->description && *ch->description ? ch->description : "「沒有」"
      , NULL, TO_CHAR );
  }

  else if ( !str_prefix( arg, "edit" ) )
  {
    if ( !ch->desc ) RETURN_NULL();

    ch->desc->edit_mode = EDIT_SELF_DESCRIPTION;
    show_editor( ch );

    free_string( ch->stack );
    ch->stack = str_dup( "" );
  }

  else if ( !str_prefix( arg, "clear" ) )
  {
    free_string( ch->description );
    ch->description = str_dup( "" );
    send_to_char( "系統已經清除你的描述。\n\r", ch );
  }

  else
  {
    send_to_char( "你的語法錯誤﹐請查詢 description 的使用方法﹗\n\r", ch );
  }

  RETURN_NULL();
}

FUNCTION( do_report )
{
  char buf[MAX_STRING_LENGTH];

  PUSH_FUNCTION( "do_report" );

  print_to_char( ch
    , "你回報﹕生命力 %d/%d 法力 %d/%d 體力 %d/%d 經驗值 %d\n\r"
       , ch->hit,  get_curr_hit( ch ) , ch->mana, get_curr_mana( ch )
       , ch->move, get_curr_move( ch ) , ch->exp );

  sprintf( buf,
    "$n回報﹕生命力 %d/%d 法力 %d/%d 體力 %d/%d 經驗值 %d"
    , ch->hit,  get_curr_hit( ch )  , ch->mana, get_curr_mana( ch )
    , ch->move, get_curr_move( ch ) , ch->exp   );

  act( smash_act_keyword( buf, "nesNES" ), ch, NULL, NULL, TO_ROOM );

  RETURN_NULL();
}

FUNCTION( do_wimpy )
{
  char arg[MAX_INPUT_LENGTH];
  int  wimpy;

  PUSH_FUNCTION( "do_wimpy" );

  one_argument( argument, arg );

  if ( ( wimpy = arg[0] ? atoi( arg ) : get_curr_hit( ch ) / 5 ) < 0 )
  {
    send_to_char( "你的勇氣可嘉﹐可是請把逃跑點數再設高一點。\n\r", ch );
    RETURN_NULL();
  }

  if ( wimpy > get_curr_hit( ch ) )
  {
    send_to_char( "你有那麼膽小嗎﹖逃跑點數竟然比你的生命力來的高。\n\r", ch );
    RETURN_NULL();
  }

  ch->wimpy = wimpy;
  act( "你設定為低於生命力$i點開始嘗試逃跑。", ch, &wimpy, NULL, TO_CHAR );
  RETURN_NULL();
}

FUNCTION( do_password )
{
  char   arg1[MAX_INPUT_LENGTH];
  char   arg2[MAX_INPUT_LENGTH];
  char   cEnd;
  char * pArg;
  char * pwdnew;
  char * p;

  PUSH_FUNCTION( "do_password" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  pArg = arg1;
  while ( isSpace( ( int ) *argument ) ) argument++;

  cEnd = ' ';
  if ( *argument == '\'' || *argument == '"' ) cEnd = *argument++;

  while ( *argument )
  {
    if ( *argument == cEnd )
    {
      argument++;
      break;
    }
    *pArg++ = *argument++;
  }
  *pArg = '\x0';

  pArg = arg2;
  while ( isSpace( ( int ) *argument ) ) argument++;

  cEnd = ' ';
  if ( *argument == '\'' || *argument == '"' ) cEnd = *argument++;

  while ( *argument )
  {
    if ( *argument == cEnd )
    {
      argument++;
      break;
    }
    *pArg++ = *argument++;
  }
  *pArg = '\x0';

  if ( !arg1[0] || !arg2[0] )
  {
    send_to_char( "語法﹕password <舊密碼> <新密碼>\n\r", ch );
    RETURN_NULL();
  }

  if ( str_cmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
  {
    send_to_char( "舊密碼錯誤﹐更改密碼動作取消。\n\r", ch );
    RETURN_NULL();
  }

  if ( !check_password( arg2 ) )
  {
    const char * pMessage;

    pMessage = ( strict_password )
      ? "你的密碼太簡單了﹐請設定包涵數字和字母且五個字以上的密碼﹗"
        "\n\r請輸入密碼﹕"
      : "你的密碼太簡單了﹐請設定五個字以上的密碼﹗\n\r請輸入密碼﹕";

    send_to_char( pMessage, ch );
    RETURN_NULL();
  }

  pwdnew = ( char * ) crypt( arg2, ch->name );
  for ( p = pwdnew; *p; p++ )
  {
    if ( *p == '~' )
    {
      send_to_char( "系統無法接受你的新密碼﹐請重新更改。\n\r", ch );
      RETURN_NULL();
    }
  }

  free_string( ch->pcdata->pwd );
  ch->pcdata->pwd = str_dup( pwdnew );
  save_char_obj( ch , SAVE_FILE );
  send_to_char( "更改密碼動作完成。\n\r", ch );
  RETURN_NULL();
}

FUNCTION( do_socials )
{
  SOCIAL_DATA * social;
  int           col = 0;

  PUSH_FUNCTION( "do_social" );

  clear_buffer();
  send_to_buffer( "\e[1;33m所有的社交指令以及其中文說明﹐"
                  "若有其他好的社交指令請告訴我們。\e[0m\n\r");

  for ( social = social_first; social; social = social->next )
  {
    send_to_buffer( "\e[1;37m%-10s \e[1;32m%-15s\e[0m"
      , social->name , social->chinese_name );

    if ( ++col % 3 == 0 ) send_to_buffer( "\n\r" );
    if ( buffer_full() ) break;
  }

  if ( col % 3 ) send_to_buffer( "\n\r" );
  print_buffer( ch );
  RETURN_NULL();
}

FUNCTION( do_commands )
{
  CMD_DATA * pCommand;
  int        col;

  PUSH_FUNCTION( "do_commands" );

  clear_buffer();

  for ( col = 0, pCommand = cmd_list; pCommand; pCommand = pCommand->next )
  {
    if ( pCommand->level < LEVEL_HERO && pCommand->level <= get_trust( ch ) )
    {
      send_to_buffer( "%-12s %-20s", pCommand->name , pCommand->cname );
      if ( ++col % 2 == 0 ) send_to_buffer( "\n\r" );
      if ( buffer_full() ) break;
    }
  }

  if ( col % 2 ) send_to_buffer( "\n\r" );
  print_buffer( ch );
  RETURN_NULL();
}

FUNCTION( do_channels )
{
  char arg[MAX_INPUT_LENGTH];
  bool fClear;
  int  bit;

  PUSH_FUNCTION( "do_channels" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  one_argument( argument, arg );

  if ( !arg[0] )
  {
    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_SILENCE ) )
    {
      send_to_char( "對不起﹐你所有的頻道都被取消了。\n\r", ch );
      RETURN_NULL();
    }

    clear_buffer();
    send_to_buffer( "[AUCTION]  拍賣頻道 ﹕ [%s]\n\r"
                    "[CHAT]     漫談頻道 ﹕ [%s]\n\r"
                    "[IMMTALK]  神族頻道 ﹕ [%s]\n\r"
                    "[MUSIC]    音樂頻道 ﹕ [%s]\n\r"
                    "[QUESTION] 發問頻道 ﹕ [%s]\n\r"
                    "[SHOUT]    嘶喊頻道 ﹕ [%s]\n\r"
                    "[YELL]     尖叫頻道 ﹕ [%s]\n\r"
                    "[GAMBLE]   賭博頻道 ﹕ [%s]\n\r"
                    "[CLASS]    職業頻道 ﹕ [%s]\n\r"
                    "[CLUB]     幫派頻道 ﹕ [%s]\n\r"
                    "[SEMOTE]   動作頻道 ﹕ [%s]\n\r"
                    "[WEATHER]  天氣預報 ﹕ [%s]\n\r"
                    "[PHONE]    呼叫頻道 ﹕ [%s]\n\r"
                    "[SUICIDE]  自裁頻道 ﹕ [%s]\n\r"
                    "[RUMOR]    謠言頻道 ﹕ [%s]\n\r"
                    "[BULLETIN] 快報頻道 ﹕ [%s]\n\r"
                    "[GROUP]    組隊頻道 ﹕ [%s]\n\r"
                    "[PK]       ＰＫ頻道 ﹕ [%s]\n\r"
      , YESNO( !IS_SET( ch->deaf, CHANNEL_AUCTION  ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_CHAT     ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_IMMTALK  ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_MUSIC    ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_QUESTION ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_SHOUT    ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_YELL     ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_GAMBLE   ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_CLASS    ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_CLUB     ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_SEMOTE   ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_WEATHER  ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_PHONE    ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_SUICIDE  ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_RUMOR    ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_BULLETIN ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_GROUP    ) )
      , YESNO( !IS_SET( ch->deaf, CHANNEL_PK       ) ) );

    print_buffer( ch );
  }

  else
  {
         if ( arg[0] == '+' ) fClear = TRUE;
    else if ( arg[0] == '-' ) fClear = FALSE;
    else
    {
      send_to_char( "你要打開(\e[1;32m+\e[0m)或是關閉"
        "(\e[1;32m-\e[0m)那個頻道﹖\n\r", ch );
      RETURN_NULL();
    }

         if ( !str_prefix( arg+1, "auction"  ) ) bit = CHANNEL_AUCTION;
    else if ( !str_prefix( arg+1, "chat"     ) ) bit = CHANNEL_CHAT;
    else if ( !str_prefix( arg+1, "immtalk"  ) ) bit = CHANNEL_IMMTALK;
    else if ( !str_prefix( arg+1, "music"    ) ) bit = CHANNEL_MUSIC;
    else if ( !str_prefix( arg+1, "question" ) ) bit = CHANNEL_QUESTION;
    else if ( !str_prefix( arg+1, "shout"    ) ) bit = CHANNEL_SHOUT;
    else if ( !str_prefix( arg+1, "yell"     ) ) bit = CHANNEL_YELL;
    else if ( !str_prefix( arg+1, "gamble"   ) ) bit = CHANNEL_GAMBLE;
    else if ( !str_prefix( arg+1, "class"    ) ) bit = CHANNEL_CLASS;
    else if ( !str_prefix( arg+1, "club"     ) ) bit = CHANNEL_CLUB;
    else if ( !str_prefix( arg+1, "semote"   ) ) bit = CHANNEL_SEMOTE;
    else if ( !str_prefix( arg+1, "weather"  ) ) bit = CHANNEL_WEATHER;
    else if ( !str_prefix( arg+1, "phone"    ) ) bit = CHANNEL_PHONE;
    else if ( !str_prefix( arg+1, "suicide"  ) ) bit = CHANNEL_SUICIDE;
    else if ( !str_prefix( arg+1, "rumor"    ) ) bit = CHANNEL_RUMOR;
    else if ( !str_prefix( arg+1, "bulletin" ) ) bit = CHANNEL_BULLETIN;
    else if ( !str_prefix( arg+1, "group"    ) ) bit = CHANNEL_GROUP;
    else if ( !str_prefix( arg+1, "pk"       ) ) bit = CHANNEL_PK;
    else
    {
      send_to_char( "你是要開啟(\e[1;32m+\e[0m)或是關閉"
        "(\e[1;32m-\e[0m)哪一個頻道﹖\n\r", ch );
      RETURN_NULL();
    }

    if ( fClear ) REMOVE_BIT( ch->deaf, bit );
    else          SET_BIT   ( ch->deaf, bit );
    do_channels( ch , "" );
  }

  RETURN_NULL();
}

FUNCTION( do_turn )
{
  char arg[MAX_INPUT_LENGTH];
  bool fSet;
  int  bit;

  PUSH_FUNCTION( "do_turn" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  one_argument( argument , arg );

  if ( !arg[0] )
  {
    clear_buffer();

    send_to_buffer( "[  關鍵字  ]     意義              選項\n\r"
                    "[ \e[1;32mrebirth\e[0m  ] 幫別人轉職             %s\n\r"
                    "[ \e[1;32mtrain\e[0m    ] 幫別人升級屬性         %s\n\r"
                    "[ \e[1;32mpractice\e[0m ] 幫別人訓練技能         %s\n\r"
                    "[ \e[1;32mansi\e[0m     ] 是否過濾彩色輸出       %s\n\r"
                    "[ \e[1;32mlotto\e[0m    ] 是否訂購樂透快報       %s\n\r"
      , YESNO( IS_SET( ch->turn, PLR_REBIRTH  ) )
      , YESNO( IS_SET( ch->turn, PLR_TRAIN    ) )
      , YESNO( IS_SET( ch->turn, PLR_PRACTICE ) )
      , YESNO( IS_SET( ch->turn, PLR_ANSI     ) )
      , YESNO( IS_SET( ch->turn, PLR_LOTTO    ) ) );

    print_buffer( ch );
  }

  else
  {
         if ( arg[0] == '+' ) fSet = TRUE;
    else if ( arg[0] == '-' ) fSet = FALSE;
    else
    {
      send_to_char( "Turn [\e[1;32m-\e[0m]選項 或是 "
        "[\e[1;32m+\e[0m]選項\n\r", ch );

      RETURN_NULL();
    }

         if ( !str_prefix( arg+1, "rebirth"  ) ) bit = PLR_REBIRTH;
    else if ( !str_prefix( arg+1, "train"    ) ) bit = PLR_TRAIN;
    else if ( !str_prefix( arg+1, "practice" ) ) bit = PLR_PRACTICE;
    else if ( !str_prefix( arg+1, "ansi"     ) ) bit = PLR_ANSI;
    else if ( !str_prefix( arg+1, "lotto"    ) ) bit = PLR_LOTTO;
    else
    {
      send_to_char( "找不到你要設定的選項﹐請查詢 turn 的使用方法。\n\r", ch );
      RETURN_NULL();
    }

    if ( fSet ) SET_BIT   ( ch->turn , bit );
    else        REMOVE_BIT( ch->turn , bit );
    do_turn( ch , "" );
  }

  RETURN_NULL();
}

FUNCTION( do_notify )
{
  char arg[ MAX_INPUT_LENGTH ];
  bool fSet;
  int  bit;

  PUSH_FUNCTION( "do_notify" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  one_argument( argument , arg );

  if ( !arg[0] )
  {
    clear_buffer();

    send_to_buffer(
      "[  關鍵字  ]     意義              選項\n\r"
      "[ \e[1;32mdebug   \e[0m ]   除錯訊息            [%s]\n\r"
      "[ \e[1;32minfo    \e[0m ]   資訊訊息            [%s]\n\r"
      "[ \e[1;32mwizard  \e[0m ]   鎖定神族訊息        [%s]\n\r"
      "[ \e[1;32mnewplayer\e[0m]   新玩家訊息          [%s]\n\r"
      "[ \e[1;32mfailpass\e[0m ]   密碼錯誤訊息        [%s]\n\r"
      "[ \e[1;32memerg   \e[0m ]   嚴重錯誤訊息        [%s]\n\r"
      "[ \e[1;32mcrit    \e[0m ]   緊急錯誤訊息        [%s]\n\r"
      "[ \e[1;32merr     \e[0m ]   臨界訊息            [%s]\n\r"
      "[ \e[1;32mxname   \e[0m ]   不雅字訊息          [%s]\n\r"
      "[ \e[1;32mnet     \e[0m ]   網際連線訊息        [%s]\n\r"
      , YESNO( !IS_SET( ch->notify, NOTIFY_DEBUG     ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_INFO      ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_WIZARD    ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_NEWPLAYER ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_FAILPASS  ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_EMERG     ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_CRIT      ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_ERR       ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_XNAME     ) )
      , YESNO( !IS_SET( ch->notify, NOTIFY_NET       ) ) );

    print_buffer( ch );
  }

  else
  {
         if ( arg[0] == '+' ) fSet = TRUE;
    else if ( arg[0] == '-' ) fSet = FALSE;
    else
    {
      send_to_char( "notify [\e[1;32m-\e[0m]選項 或是 "
        "[\e[1;32m+\e[0m]選項\n\r", ch );
      RETURN_NULL();
    }

         if ( !str_prefix( arg+1, "debug"     ) ) bit = NOTIFY_DEBUG;
    else if ( !str_prefix( arg+1, "info"      ) ) bit = NOTIFY_INFO;
    else if ( !str_prefix( arg+1, "wizard"    ) ) bit = NOTIFY_WIZARD;
    else if ( !str_prefix( arg+1, "newplayer" ) ) bit = NOTIFY_NEWPLAYER;
    else if ( !str_prefix( arg+1, "failpass"  ) ) bit = NOTIFY_FAILPASS;
    else if ( !str_prefix( arg+1, "emerg"     ) ) bit = NOTIFY_EMERG;
    else if ( !str_prefix( arg+1, "crit"      ) ) bit = NOTIFY_CRIT;
    else if ( !str_prefix( arg+1, "err"       ) ) bit = NOTIFY_ERR;
    else if ( !str_prefix( arg+1, "xname"     ) ) bit = NOTIFY_XNAME;
    else if ( !str_prefix( arg+1, "net"       ) ) bit = NOTIFY_NET;
    else
    {
      send_to_char( "找不到你要設定的選項﹐請查詢 notify。\n\r", ch );
      RETURN_NULL();
    }

    if ( fSet ) REMOVE_BIT( ch->notify , bit );
    else        SET_BIT   ( ch->notify , bit );
    do_notify( ch , "" );
  }

  RETURN_NULL();
}

FUNCTION( do_config )
{
  char arg[MAX_INPUT_LENGTH];
  bool fSet;
  int  bit;

  PUSH_FUNCTION( "do_config" );

  if ( IS_NPC(ch) ) RETURN_NULL();

  one_argument( argument, arg );

  if ( !arg[0] )
  {
    clear_buffer();

    send_to_buffer(
      "[  關鍵字   ]  意義         選項\n\r"
      "[ \e[1;32mautoexit \e[0m ]  自動出口      %s\n\r"
      "[ \e[1;32mautoloot \e[0m ]  自動搜尋屍體  %s\n\r"
      "[ \e[1;32mautosac  \e[0m ]  自動奉獻屍體  %s\n\r"
      "[ \e[1;32mblank    \e[0m ]  提示符號空行  %s\n\r"
      "[ \e[1;32mbrief    \e[0m ]  完整敘述      %s\n\r"
      "[ \e[1;32mcombine  \e[0m ]  物品組合模式  %s\n\r"
      "[ \e[1;32mprompt   \e[0m ]  提示符號      %s\n\r"
      "[ \e[1;32mexact    \e[0m ]  完整名稱      %s\n\r"
      "[ \e[1;32mmessage  \e[0m ]  攻擊敘述      %s\n\r"
      "[ \e[1;32mflee     \e[0m ]  斷線自動逃跑  %s\n\r"
      "[ \e[1;32mangel    \e[0m ]  新手守護神    %s\n\r"
      "[ \e[1;32mautofood \e[0m ]  自動吃食物    %s\n\r"
      "[ \e[1;32mautodrink\e[0m ]  自動喝水      %s\n\r"
      "[ --------  ]  消音          %s\n\r"
      "[ --------  ]  不能做 EMOTE  %s\n\r"
      "[ --------  ]  不能使用漫談  %s\n\r"
      ,  YESNO( IS_SET( ch->act, PLR_AUTOEXIT  ) )
      ,  YESNO( IS_SET( ch->act, PLR_AUTOLOOT  ) )
      ,  YESNO( IS_SET( ch->act, PLR_AUTOSAC   ) )
      ,  YESNO( IS_SET( ch->act, PLR_BLANK     ) )
      ,  YESNO( IS_SET( ch->act, PLR_BRIEF     ) )
      ,  YESNO( IS_SET( ch->act, PLR_COMBINE   ) )
      ,  YESNO( IS_SET( ch->act, PLR_PROMPT    ) )
      ,  YESNO( IS_SET( ch->act, PLR_EXACT     ) )
      ,  YESNO( IS_SET( ch->act, PLR_MESSAGE   ) )
      ,  YESNO( IS_SET( ch->act, PLR_FLEE      ) )
      ,  YESNO( IS_SET( ch->act, PLR_ANGEL     ) )
      ,  YESNO( IS_SET( ch->act, PLR_AUTOFOOD  ) )
      ,  YESNO( IS_SET( ch->act, PLR_AUTODRINK ) )
      ,  YESNO( IS_SET( ch->act, PLR_SILENCE   ) )
      ,  YESNO( IS_SET( ch->act, PLR_NO_EMOTE  ) )
      ,  YESNO( IS_SET( ch->act, PLR_NO_TELL   ) ) );

    print_buffer( ch );
  }
  else
  {
         if ( arg[0] == '+' ) fSet = TRUE;
    else if ( arg[0] == '-' ) fSet = FALSE;
    else
    {
      send_to_char( "config [\e[1;32m-\e[0m]選項 或是 "
        "[\e[1;32m+\e[0m]選項﹗\n\r", ch );
      RETURN_NULL();
    }

         if ( !str_prefix( arg+1, "autoexit"  ) ) bit = PLR_AUTOEXIT;
    else if ( !str_prefix( arg+1, "autoloot"  ) ) bit = PLR_AUTOLOOT;
    else if ( !str_prefix( arg+1, "autosac"   ) ) bit = PLR_AUTOSAC;
    else if ( !str_prefix( arg+1, "blank"     ) ) bit = PLR_BLANK;
    else if ( !str_prefix( arg+1, "brief"     ) ) bit = PLR_BRIEF;
    else if ( !str_prefix( arg+1, "combine"   ) ) bit = PLR_COMBINE;
    else if ( !str_prefix( arg+1, "prompt"    ) ) bit = PLR_PROMPT;
    else if ( !str_prefix( arg+1, "telnetga"  ) ) bit = PLR_TELNET_GA;
    else if ( !str_prefix( arg+1, "exact"     ) ) bit = PLR_EXACT;
    else if ( !str_prefix( arg+1, "message"   ) ) bit = PLR_MESSAGE;
    else if ( !str_prefix( arg+1, "angel"     ) ) bit = PLR_ANGEL;
    else if ( !str_prefix( arg+1, "flee"      ) ) bit = PLR_FLEE;
    else if ( !str_prefix( arg+1, "autofood"  ) ) bit = PLR_AUTOFOOD;
    else if ( !str_prefix( arg+1, "autodrink" ) ) bit = PLR_AUTODRINK;
    else
    {
      send_to_char( "請問你要設定哪一個選項﹖\n\r", ch );
      RETURN_NULL();
    }

    if ( fSet ) SET_BIT   ( ch->act, bit);
    else        REMOVE_BIT( ch->act, bit);

    do_config( ch , "" );
  }

  RETURN_NULL();
}

FUNCTION( do_autoexit )
{
  PUSH_FUNCTION( "do_autoexit" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  ( IS_SET ( ch->act, PLR_AUTOEXIT )
    ? do_config( ch, "-autoexit" )
    : do_config( ch, "+autoexit" ) );

  RETURN_NULL();
}

FUNCTION( do_autoloot )
{
  PUSH_FUNCTION( "do_autoloot" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  ( IS_SET( ch->act, PLR_AUTOLOOT )
    ? do_config( ch, "-autoloot" )
    : do_config( ch, "+autoloot" ) );

  RETURN_NULL();
}

FUNCTION( do_autosac )
{
  PUSH_FUNCTION( "do_autosac" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  ( IS_SET( ch->act, PLR_AUTOSAC )
    ? do_config( ch, "-autosac" )
    : do_config( ch, "+autosac" ) );

  RETURN_NULL();
}

FUNCTION( do_mode )
{
  char arg[MAX_INPUT_LENGTH];

  PUSH_FUNCTION( "do_mode" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  one_argument( argument, arg );

  if ( arg[0] == '\x0' )
  {
    send_to_char( "你要把你的輸出模式改成什麼樣子呢﹐請查詢 mode /?\n\r", ch );
  }

  else if ( !str_cmp( arg, "1" ) || !str_cmp( arg, "merc" ) )
  {
    SET_BIT( ch->act, PLR_PROMPT );
    SET_BIT( ch->act, PLR_BLANK  );
    send_to_char( "你把輸出模式改成 \e[1;32mMerc Mud\e[0m 模式﹗\n\r", ch );
  }

  else if ( !str_cmp( arg, "2" ) || !str_cmp( arg, "lp" ) )
  {
    REMOVE_BIT( ch->act, PLR_PROMPT );
    REMOVE_BIT( ch->act, PLR_BLANK  );
    send_to_char( "你把輸出模式改成 \e[1;32mLP Mud\e[0m 模式﹗\n\r", ch );
  }

  else
  {
    send_to_char( "你的參數錯誤﹐請查詢 mode 的正確使用方法﹗\n\r", ch );
  }

  RETURN_NULL();
}

FUNCTION( do_blank )
{
  PUSH_FUNCTION( "do_blank" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  ( IS_SET( ch->act, PLR_BLANK )
    ? do_config( ch, "-blank" )
    : do_config( ch, "+blank" ) );

  RETURN_NULL();
}

FUNCTION( do_brief )
{
  PUSH_FUNCTION( "do_brief" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  ( IS_SET( ch->act, PLR_BRIEF )
    ? do_config( ch, "-brief" )
    : do_config( ch, "+brief" ) );

  RETURN_NULL();
}

FUNCTION( do_combine )
{
  PUSH_FUNCTION( "do_combine" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  ( IS_SET( ch->act, PLR_COMBINE )
    ? do_config( ch, "-combine" )
    : do_config( ch, "+combine" ) );

  RETURN_NULL();
}

FUNCTION( do_pagelen )
{
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  int  lines;

  PUSH_FUNCTION( "do_pagelen" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  one_argument( argument, arg );

  lines = arg[0] == '\x0' ? 20 : atoi( arg );

  if ( lines < MIN_PAGELEN )
  {
    chinese_number( MIN_PAGELEN, buf );
    act( "每頁換行的數字不能小於$t。", ch, buf, NULL, TO_CHAR );
    RETURN_NULL();
  }

  if ( lines > MAX_PAGELEN )
  {
    chinese_number( MAX_PAGELEN, buf );
    act( "每頁換行的數字不能大於$t。", ch, buf, NULL, TO_CHAR );
    RETURN_NULL();
  }

  ch->pcdata->pagelen = lines;
  act( "你設定每$i行換頁。", ch, &lines, NULL, TO_CHAR );
  RETURN_NULL();
}

FUNCTION( do_prompt )
{
  char buf[MAX_STRING_LENGTH];

  PUSH_FUNCTION( "do_prompt" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  if ( !argument[0] )
  {
    ( IS_SET( ch->act, PLR_PROMPT )
      ? do_config( ch, "-prompt" )
      : do_config( ch, "+prompt" ) );
    RETURN_NULL();
  }

  if ( !str_cmp( argument, "all" ) )
     str_cpy( buf, "<生命力 ^hp1 法力 ^mp1 體力 ^mv1 金錢 ^gol> ");

  else
  {
   if ( str_len( argument ) > 50 ) argument[50] = '\x0';
   str_cpy( buf, argument );
   smash_tilde( buf );
   smash_point( buf );
  }

  free_string( ch->prompt );
  ch->prompt = str_dup( buf );
  send_to_char( "組態設定完畢。\n\r", ch );
  RETURN_NULL();
}

FUNCTION( do_auto )
{
  PUSH_FUNCTION( "do_auto" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  do_config( ch, "" );
  RETURN_NULL();
}

/* 清除螢幕 */
FUNCTION( do_cls )
{
  PUSH_FUNCTION( "do_cls" );
  send_to_char( "\e[2J\e[H\e[0m" , ch );
  RETURN_NULL();
}

/* 觀看地圖 由 robin 提供 */
FUNCTION( do_map )
{
  PUSH_FUNCTION( "do_map" );
  do_help( ch , "map" );
  RETURN_NULL();
}

FUNCTION( do_credit )
{
  PUSH_FUNCTION( "do_credit" );
  do_help( ch, "credit" );
  RETURN_NULL();
}

FUNCTION( do_status )
{
  CMD_DATA  * pCommand;
  CMD_DATA  * aCommand;
  CMD_DATA  * bCommand;
  CMD_DATA ** zCommand;
  char        arg[ MAX_INPUT_LENGTH ];
  bool        Found;
  int         loop;
  int         count;
  int         how;
  int         total;

  PUSH_FUNCTION( "do_status" );

  one_argument( argument, arg );
  clear_buffer();

  if ( !str_cmp( arg, "all" ) )
  {
    for ( pCommand = cmd_list; pCommand; pCommand = pCommand->next )
    {
      send_to_buffer(
        "\e[1;32m%-12s\e[0m [%6d] "
        "\e[1;33m%-14s\e[0m"
        "狀態﹕\e[1;35m%4s\e[0m "
        "等級﹕[ \e[1;31m%3d\e[0m ] "
        "標記﹕\e[1;36m%-10s\e[0m\n\r"
          , pCommand->name
          , pCommand->exec
          , pCommand->cname
          , get_position( pCommand->position )
          , pCommand->level
          , get_log( pCommand->log ) );

       if ( buffer_full() ) break;
     }
  }

  else if ( !str_cmp( arg, "sort" ) )
  {
    total = count = 0;
    for ( pCommand = cmd_list; pCommand; pCommand = pCommand->next )
    {
      total += pCommand->exec;
      count++;
    }

    zCommand = alloc_string( count * sizeof( CMD_DATA * ) );

    loop = 0;
    for ( pCommand = cmd_list; pCommand; pCommand = pCommand->next, loop++ )
      zCommand[loop] = pCommand;

    for ( loop = 0; loop < count - 1; loop++ )
    {
      for ( how = loop + 1; how < count; how++ )
      {
        aCommand = zCommand[loop];
        bCommand = zCommand[how];

        if ( aCommand->exec < bCommand->exec )
        {
          zCommand[loop] = bCommand;
          zCommand[how ] = aCommand;
        }
      }
    }

    clear_buffer();
    for ( how = loop = 0; loop < count; loop++ )
    {
      pCommand = zCommand[loop];

      if ( pCommand->exec <= 0 ) continue;

      send_to_buffer(
        "\e[1;37m%3d.\e[0m \e[1;32m%-12s\e[0m %6d %3d.%d%% "
        "\e[1;33m%-14s\e[0m"
        "狀態﹕\e[1;35m%4s\e[0m "
        "等級﹕\e[1;31m%3d\e[0m\n\r"
          , ++how
          , pCommand->name
          , pCommand->exec
          , pCommand->exec * 100 / total
          , ( pCommand->exec * 1000 / total ) % 10
          , pCommand->cname
          , get_position( pCommand->position )
          , pCommand->level );

        if ( buffer_full() ) break;
    }

    free_string( ( void * ) zCommand );
  }

  else
  {
    Found = FALSE;
    for ( pCommand = cmd_list; pCommand; pCommand = pCommand->next )
    {
      if ( !str_prefix( arg, pCommand->name ) )
      {
        send_to_buffer(
          "指令名稱  ﹕%s\n\r"
          "中文名稱  ﹕%s\n\r"
          "最低狀態  ﹕%s\n\r"
          "標記方式  ﹕%s\n\r"
          "最低等級  ﹕%d\n\r"
          "執行次數  ﹕%d\n\r"
          "是否能鎖定﹕%s\n\r"
          "是否已上鎖﹕%s\n\r"
          "是否要完整﹕%s\n\r"
          "是否要參數﹕%s\n\r"
          "怪物指令  ﹕%s\n\r"
          "閒聊標記  ﹕%s\n\r"
          "神族鎖定  ﹕%s\n\r"
          "監獄執行  ﹕%s\n\r"
          "死亡執行  ﹕%s\n\r"
          "命令執行  ﹕%s\n\r"
          "失敗執行  ﹕%s\n\r"
          "屬性限制  ﹕%s\n\r"
          "執行追蹤  ﹕%s\n\r"
          , pCommand->name, pCommand->cname
          , get_position( pCommand->position ), get_log( pCommand->log )
          , pCommand->level, pCommand->exec
          , YESNO( pCommand->canlock  )
          , YESNO( pCommand->lock     )
          , YESNO( pCommand->exact    )
          , YESNO( pCommand->argument )
          , YESNO( pCommand->mobonly  )
          , YESNO( pCommand->chat     )
          , YESNO( pCommand->wizlog   )
          , YESNO( pCommand->jail     )
          , YESNO( pCommand->dead     )
          , YESNO( pCommand->order    )
          , YESNO( pCommand->lost     )
          , YESNO( pCommand->limit    )
          , YESNO( pCommand->trace    ) );

        Found = TRUE;
        break;
      }
    }

    if ( !Found ) send_to_buffer( "未找到相近的指令集。\n\r" );
  }

  print_buffer( ch );
  RETURN_NULL();
}

/* 列印出生命力法力或是體力的百分比函數 */
void show_percentage( CHAR_DATA * ch , int value , int max
                      , int color ,int frag )
{
  char buf[ MAX_STRING_LENGTH ];

  PUSH_FUNCTION( "show_percentage" );

  sprintf( buf , "\e[1;%dm%s\e[0m" , color
    , return_percentage( ch , value , max , frag ) );

  send_to_char( buf , ch );
  RETURN_NULL();
}

char * return_percentage( CHAR_DATA * ch , int value , int max , int frag )
{
  static char buf[ MAX_STRING_LENGTH ];
  int         loop;

  PUSH_FUNCTION( "return_percentage" );

  if ( max < frag )
  {
    mudlog( LOG_DEBUG , "show_percentage: %s的基本數值小於 %d. 無法列印百分比."
      , ch->name ,frag );

    buf[0] = '\x0';
    RETURN( buf );
  }

  for ( buf[0] = '\x0', loop = 0 ; loop < frag ; loop++ )
  {
    if ( str_len( buf ) > sizeof( buf ) - 10 ) break;

    if ( value >= ( ( max * ( loop + 1 ) ) / frag ) ) str_cat( buf , "■" );
    else                                              str_cat( buf , "□" );
  }
  RETURN( buf );
}

/* 察看系統資源 */
FUNCTION( do_system )
{
  extern bool   signal_segv;
  extern bool   signal_fpe;
  extern bool   signal_bus;
  extern bool   signal_ill;
  extern int    traceloop;
  extern int    FunctionPointer;
  extern char * FunctionName[MAX_DEBUG];
  int           loop;
  int           count;

  PUSH_FUNCTION( "do_system" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  clear_buffer();
  send_to_buffer(
    "遊戲版本﹕%s\n\r"
    "有關序號方面﹕\n\r"
    "目前系統的流水序號 %d-%d。\n\r"
    "遊戲目前%s被鎖定。\n\r"
    "%s防止多重連線。\n\r"
    "系統採用%s密碼方式。\n\r"
    "系統採用%s方式檢查電子信箱。\n\r"
    "全體計數器%s被鎖定。\n\r"
    "%s內部除錯誤模式。\n\r"
    "防止無窮迴圈記數器 %d\n\r"
    "最大連線人數       %d\n\r"
    "目前幫派房間編號   %d\n\r"
    "最多可以有幾個幫派 %d\n\r"
    "目前幫派的數目     %d\n\r"
    "最多版面數目       %d\n\r"
    "超過幾封信系統清除 %d\n\r"
    "每位玩家好友名單   %d\n\r"
    "最多每位玩家巨集   %d\n\r"
    "最多限制連線位址   %d\n\r"
    "最多巨集重覆次數   %d\n\r"
    "最多每位玩家致能   %d\n\r"
    "最多攜帶傭兵隻數   %d\n\r"
    "最低能給錢的等級   %d\n\r"
    "樂透快報價格       %d\n\r"
    "玩家檔案保留天數   %d\n\r"
    "玩家檔案限制大小   %d 位元組\n\r"
    "是否限制有無FQDN   %s\n\r"
    "%s 存取非法位址 訊號\n\r"
    "%s 浮點運算錯誤 訊號\n\r"
    "%s 參考不當位址 訊號\n\r"
    "%s 非法指令集   訊號\n\r\n\r"
    , MUD_VERSION
    , serial_time
    , serial_loop
    , wizlock         ? "已經" : "尚未"
    , multilogin      ? "已經" : "尚未"
    , strict_password ? "嚴謹" : "一般"
    , strict_email    ? "嚴謹" : "一般"
    , tick_permit     ? "尚未" : "已經"
    , internal_debug  ? "開啟" : "關閉"
    , traceloop
    , max_connect
    , club_room
    , max_club
    , club_count()
    , max_board
    , max_notes
    , max_friend
    , max_alias
    , max_ip
    , alias_repeat
    , max_enable
    , max_hire
    , level_limit
    , lotto_paper
    , hold_day
    , file_quota
    , YESNO( fqdn_limit )
    , signal_segv ? "開啟" : "關閉"
    , signal_fpe  ? "開啟" : "關閉"
    , signal_bus  ? "開啟" : "關閉"
    , signal_ill  ? "開啟" : "關閉" );

  /* 是否開啟除錯模式 */
  if ( internal_debug )
  {
    SystemCrash = TRUE;
    for ( count = 0, loop = FunctionPointer + 1; loop < MAX_DEBUG; loop++ )
    {
      if ( !FunctionName[loop] ) break;
      send_to_buffer( "(%2d) 函數﹕%-20s\n\r", ++count, FunctionName[loop] );
      if ( buffer_full() ) break;
    }

    SystemCrash = FALSE;
  }

  print_buffer( ch );
  RETURN_NULL();
}

/* 察看自己先前的輸入 */
FUNCTION( do_history )
{
  int               loop;
  char              arg[MAX_INPUT_LENGTH];
  CHAR_DATA       * victim;
  DESCRIPTOR_DATA * pDesc;

  PUSH_FUNCTION( "do_history" );

  /* 虛擬怪物一定不能執行 */
  if ( IS_NPC( ch ) ) RETURN_NULL();

  one_argument( argument, arg );
  clear_buffer();

  if ( !arg[0] )
  {
    send_to_buffer( "\n\r你前%d個輸入命令的列表﹕\n\n\r" , MAX_HISTORY );
    victim = ch;
  }

  else if ( !IS_IMMORTAL( ch ) )
  {
    send_to_char( "你的權力不夠看別人的指令。\n\r", ch );
    RETURN_NULL();
  }

  else
  {
    if ( !( victim = get_char_world( ch, arg ) ) )
    {
      act( "找不到你要觀看的對象 $2$T$0。", ch, NULL, arg, TO_CHAR );
      RETURN_NULL();
    }

    if ( IS_NPC( victim ) )
    {
      act( "$N是沒有指令暫存區的。", ch, NULL, victim, TO_CHAR );
      RETURN_NULL();
    }

    if ( get_trust( ch ) < get_trust( victim ) )
    {
      act( "你的權力不夠看$N的指令。", ch, NULL, victim, TO_CHAR );
      RETURN_NULL();
    }

    send_to_buffer( "\n\r%s前%d個輸入命令的列表﹕\n\n\r"
      , mob_name( ch, victim ), MAX_HISTORY );
  }

  if ( !( pDesc = victim->desc ) )
  {
    act( "$N離線且沒有描述表。", ch, NULL, victim, TO_CHAR );
    RETURN_NULL();
  }

  for ( loop = MAX_HISTORY - 1; loop >= 0; loop-- )
  {
    send_to_buffer( "!%d = %s\n\r" , loop + 1
      , ( pDesc->inlast[loop][0] ) ? pDesc->inlast[loop] : "(空)");

    if ( buffer_full() ) break;
  }

  print_buffer( ch );
  RETURN_NULL();
}

char * get_worktime_string( void )
{
  static char buffer[ MAX_STRING_LENGTH ];
  int         iTime;

  PUSH_FUNCTION( "get_worktime_string" );

  iTime = ( int ) difftime( current_time, mud_boot_time );
  str_cpy( buffer, date_string( iTime ) );

  RETURN( buffer );
}

char * date_string( int iTime )
{
  static char   buf[MAX_STRING_LENGTH];
  char          temp[MAX_INPUT_LENGTH];
  char          chinese[MAX_INPUT_LENGTH];
  unsigned int  work_second;
  unsigned int  work_minute;
  unsigned int  work_hour;
  unsigned int  work_day;
  bool          Status;

  PUSH_FUNCTION( "date_string" );

  buf[0] = '\x0';

  if ( iTime < 0 ) iTime = 0;

  work_second   = iTime % 60;
  iTime        /= 60;
  work_minute   = iTime % 60;
  iTime        /= 60;
  work_hour     = iTime % 24;
  work_day      = iTime / 24;
  Status        = FALSE;

  if ( work_day > 0 )
  {
    Status = TRUE;
    chinese_number( work_day, chinese );
    sprintf( temp , "%s天" , chinese );
    str_cat( buf, temp );
  }

  if ( work_hour || Status )
  {
    Status = TRUE;
    chinese_number( work_hour, chinese );
    sprintf( temp, "%s小時" , chinese );
    str_cat( buf, temp );
  }

  if ( work_minute || Status )
  {
    chinese_number( work_minute, chinese );
    sprintf( temp, "%s分" , chinese );
    str_cat( buf, temp );
  }

  chinese_number( work_second, chinese );
  sprintf( temp, "%s秒" , chinese );
  str_cat( buf, temp );

  RETURN( buf );
}

char * time_format( time_t clock, const char * formats )
{
  static char buf[ MAX_STRING_LENGTH ];
  PUSH_FUNCTION( "time_format" );
  strftime( buf, sizeof( buf ), formats, localtime( &clock ) );
  RETURN( buf );
}

char * time_string( struct timeval * timeval_info )
{
  static char buf[MAX_STRING_LENGTH];
  int    day;
  int    hour;
  int    minute;
  int    second;
  char * pString;

  PUSH_FUNCTION( "time_string" );

  str_cpy( buf , "00-00:00:00.00" );

  if ( timeval_info )
  {
    second = timeval_info->tv_sec;
    buf[0] = '\x0';

    day     = 0;
    hour    = second / 3600;
    second -= hour   * 3600;
    minute  = second / 60;
    second -= minute * 60;
    day     = hour / 24;
    hour    = hour % 24;

    sprintf( buf , "%2d-%2d:%2d:%2d.%2d"
      , UMIN( 99, day ), hour, minute , second
      , ( int ) ( timeval_info->tv_usec / 100000 ) );

    for ( pString = buf; *pString; pString++ )
      if ( *pString == ' ' ) *pString = '0';
  }

  RETURN( buf );
}

char * showtime( int iTime )
{
  static char buf[MAX_STRING_LENGTH];
  int         num;

  PUSH_FUNCTION( "showtime" );

  str_cpy( buf , "00:00:00" );

  if ( iTime > 0 )
  {
    iTime -= ( num = ( iTime / 36000 ) ) * 36000;
    buf[0] += UMIN( 9, num );

    iTime -= ( num = ( iTime / 3600 ) ) * 3600;
    buf[1] += UMIN( 9, num );

    iTime -= ( num = ( iTime / 600 ) ) * 600;
    buf[3] += UMIN( 9, num );

    iTime -= ( num = ( iTime / 60 ) ) * 60;
    buf[4] += UMIN( 9, num );

    iTime -= ( num = ( iTime / 10 ) ) * 10;
    buf[6] += UMIN( 9, num );

    buf[7] += UMIN( 9, iTime );
  }

  RETURN( buf );
}

FUNCTION( do_pry )
{
  char             arg[MAX_INPUT_LENGTH];
  char             buf[MAX_STRING_LENGTH];
  int              count;
  int              choice;
  CHAR_DATA      * victim;
  SKILL_DATA     * pSkill;
  SKILL_DATA     * aSkill;
  CLASS_DATA     * pClass;
  LIMIT_DATA     * pLimit;
  TEACH_DATA     * teach;
  RESTRICT_DATA  * pRest;
  MOB_INDEX_DATA * pMobIndex;

  char           * PryMessage[] =
  {
    "找他準",
    "而且他十分樂意",
    "我想他會樂意",
    "他可能會",
    "他應該是會",
    "但是他不太想",
    "但是他不願意",
    "但是他十分不願意",
    "你可能要試試運氣看他是否會"
  };

  PUSH_FUNCTION( "do_pry" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  if ( ch->gold < UMAX( 1, PryCost ) )
  {
    act( "等你湊足了$i兩銀子再來問吧﹗", ch, &PryCost, NULL, TO_CHAR );
    RETURN_NULL();
  }

  argument = one_argument( argument, arg );

  if ( arg[0] == '\x0' )
  {
    send_to_char( "你要查詢那個技能呢﹖\n\r", ch );
    RETURN_NULL();
  }

  if ( !( pSkill = skill_isname( arg ) ) || !pSkill->valid )
  {
    act( "對不起﹐找不到那個技能 $2$T$0 耶﹗", ch, NULL, arg, TO_CHAR );
    RETURN_NULL();
  }

  gold_from_char( ch, UMAX( 1, PryCost ) );

  clear_buffer();
  send_to_buffer(
    "英文名稱﹕%s\n\r"
    "中文名稱﹕%s\n\r"
    "攻擊武器﹕%s\n\r"
    "互相教導﹕%s\n\r"
    "技能功\能﹕%s\n\r"
    "技能分類﹕%s\n\r"
    "浪費數值﹕"
    , pSkill->name
    , pSkill->cname
    , weapon_type_name( pSkill->weapon )
    , pSkill->teach ? "可以" : "不可以"
    , skill_type( pSkill->type )
    , pSkill->rating ? skill_rating( pSkill->rating ) : "從缺" );

  switch( pSkill->cost_type )
  {
  default:        send_to_buffer( "未明\n\r"   ); break;
  case COST_HIT:  send_to_buffer( "生命力\n\r" ); break;
  case COST_MOVE: send_to_buffer( "體力\n\r"   ); break;
  case COST_MANA: send_to_buffer( "法力\n\r"   ); break;
  case COST_GOLD: send_to_buffer( "金錢\n\r"   ); break;
  }

  send_to_buffer( "領悟技能﹕" );
  if ( ( aSkill = get_skill( pSkill->associate ) ) )
  {
    send_to_buffer( "%s(%s)\n\r", aSkill->cname, aSkill->name );
    send_to_buffer( "領悟機率﹕" );

    send_to_buffer( "%s", return_array( PryChance, pSkill->degree
            , sizeof( PryChance ) / sizeof( PryChance[0] ) ) );
  }
  else
  {
    send_to_buffer( "從缺\n\r" );
  }

  send_to_buffer( "預備功\夫﹕" );

  for ( aSkill = skill_list; aSkill; aSkill = aSkill->next )
    if ( aSkill->associate == pSkill->slot ) break;

  if ( aSkill )
  {
    send_to_buffer( "%s(%s)\n\r", aSkill->cname, aSkill->name );
    send_to_buffer( "領悟機率﹕" );

    send_to_buffer( "%s", return_array( PryChance, aSkill->degree
            , sizeof( PryChance ) / sizeof( PryChance[0] ) ) );
  }
  else
  {
    send_to_buffer( "從缺\n\r" );
  }

  /* 性別限制 */
  if ( pSkill->sex && pSkill->sex > ERRORCODE && pSkill->sex < MAX_SEX )
  {
    send_to_buffer( "\n\r職業性別限制﹕%s\n\r", sex_name( pSkill->sex ) );
  }

  send_to_buffer( "\n\r職業限制﹕\n\r" );
  for ( pClass = class_list; pClass; pClass = pClass->next )
  {
    for ( pLimit = pSkill->limit; pLimit; pLimit = pLimit->next )
    {
      if ( pClass == pLimit->class )
      {
        chinese_number( pLimit->level, buf );

        send_to_buffer(
          "%s則等級最低要%4s級才能學習﹐熟練度最高可以到%s﹗\n\r"
          , class_name( pClass, FALSE )
          , buf
          , adeptation_name( pLimit->adept ) );

        break;
      }
    }
  }

  for ( pClass = class_list; pClass; pClass = pClass->next )
  {
    for ( pLimit = pSkill->limit; pLimit; pLimit = pLimit->next )
      if ( pClass == pLimit->class ) break;

    if ( !pLimit )
    {
      send_to_buffer( "%s不能學習此技能﹗\n\r", class_name( pClass, FALSE ) );
    }
  }

  send_to_buffer( "\n\r限    制﹕\n\r" );
  count = 0;

  for ( pRest = pSkill->restrict; pRest; pRest = pRest->next )
  {
    chinese_number( ++count, buf );
    send_to_buffer( "%s、本技能%s\n\r", buf, restrict_value( pRest, ch ) );
  }

  if ( count == 0 ) send_to_buffer( "本技能沒有任何限制﹗\n\r" );

  send_to_buffer( "\n\r教    導﹕\n\r" );
  count  = 0;
  choice = sizeof( PryMessage ) / sizeof( PryMessage[0] );

  for ( victim = char_list; victim; victim = victim->next )
  {
    if ( !verify_char( victim )
      || !IS_NPC( victim )
      || !( pMobIndex = victim->pIndexData )
      || !victim->in_room
      || !victim->in_room->area ) continue;

    for ( teach = pMobIndex->teach; teach; teach = teach->next )
    {
      if ( pSkill->slot == teach->slot )
      {
        int sel;

        chinese_number( ++count, buf );
        sel = UMAX( 0, UMIN( choice - 1, teach->inventory * choice / 100 ) );

        send_to_buffer( "%s、%s的%s有教導%s﹐%s教導你﹐費用 %d 兩。\n\r"
          , buf, victim->in_room->area->name, mob_name( ch, victim )
          , pSkill->cname, PryMessage[sel], teach->cost );
      }
    }
  }

  if ( count == 0 ) send_to_buffer( "對不起﹐這種技能沒有人教導﹗\n\r" );
  print_buffer( ch );

  RETURN_NULL();
}

size_t add_firman( CHAR_DATA * ch, int amount )
{
  size_t count;

  PUSH_FUNCTION( "add_firman" );

  if ( !ch )
  {
    mudlog( LOG_DEBUG, "add_firman: 來源不正確." );
    RETURN( 0 );
  }

  if ( amount <= 0 )
  {
    mudlog( LOG_DEBUG, "add_firman: 增加數目 %d 不合理.", amount );
    RETURN( 0 );
  }

  if ( ( count = UMIN( amount, MaxFirman - ch->firman ) ) <= 0 )
  {
    send_to_char( "對不起﹐你的免死金牌數太多了﹐所以無法送給你﹗\n\r", ch );
  }

  else
  {
    ch->firman += count;
  }

  RETURN( count );
}

int who_compare( const void * a, const void * b )
{
  char ** c = ( char ** ) a;
  char ** d = ( char ** ) b;
  DESCRIPTOR_DATA * aDesc;
  DESCRIPTOR_DATA * bDesc;

  aDesc = ( DESCRIPTOR_DATA * ) *c;
  bDesc = ( DESCRIPTOR_DATA * ) *d;
  return( bDesc->character->level > aDesc->character->level );
}
