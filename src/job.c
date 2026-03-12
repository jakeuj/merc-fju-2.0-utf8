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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merc.h"

extern void       check_contraband args( ( CHAR_DATA * ) );

/* 定義可以到競技場的等級 */
#define LEVEL_CAN_PK               35

/* 定義到競技場要交的錢 */
#define FIGHT_MONEY                10000

/* 定義最小可以回書院學宮的等級 */
#define LEVEL_NO_NEW               15

DECLARE_JOB_FUN( job_recall_new    );
DECLARE_JOB_FUN( job_goto_pk_area   );
DECLARE_JOB_FUN( job_bore_hole      );
DECLARE_JOB_FUN( job_pull_bar       );
DECLARE_JOB_FUN( job_pray_yama      );
DECLARE_JOB_FUN( job_touch_stone    );

#if defined(FUNC_NAME)
#undef FUNC_NAME
#endif

#define FUNC_NAME( nFunction )                             \
  if ( !str_cmp( name, #nFunction ) ) RETURN( nFunction ); \

JOB_FUN * job_lookup( const char * name )
{
  PUSH_FUNCTION( "job_lookup" );
  FUNC_NAME( job_recall_new   );
  FUNC_NAME( job_goto_pk_area );
  FUNC_NAME( job_bore_hole    );
  FUNC_NAME( job_pull_bar     );
  FUNC_NAME( job_pray_yama    );
  FUNC_NAME( job_touch_stone  );
  RETURN( NULL );
}

JOB( job_recall_new )
{
  ROOM_INDEX_DATA * pRoom;

  PUSH_FUNCTION( "job_recall_new" );

  if ( !( pRoom = RoomSchool )
    || !ch->in_room
    || !can_char_from_room( ch, TRUE ) ) RETURN_NULL();

  if ( ch->level >= LEVEL_NO_NEW )
  {
    send_to_char( "你都已經那麼老了還想回書院學宮啊﹐真不害臊﹗\n\r", ch );
    RETURN_NULL();
  }

  act( "$n向天神祈禱回到書院學宮繼續訓練。", ch, NULL, NULL, TO_ROOM );
  send_to_char( "\e[1;33m你狼狽的逃回書院學宮﹗\e[0m\n\r\n\r", ch );

  char_from_room( ch );
  char_to_room( ch, pRoom );

  act( "$n狼狽的逃了回來﹗", ch, NULL, NULL, TO_ROOM );
  do_look( ch, "auto" );

  /* 清除追蹤紀錄點 */
  clear_trace( ch, TRUE );

  RETURN_NULL();
}

JOB( job_goto_pk_area )
{
  char              buf[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA * pIndex1;
  ROOM_INDEX_DATA * pIndex2;

  PUSH_FUNCTION( "job_goto_pk_area" );

  if ( !ch || !verify_char( ch ) ) RETURN_NULL();

  if ( !( pIndex1 = get_room_index( 13043 ) )
    || !( pIndex2 = get_room_index( 13044 ) ) )
  {
    mudlog( LOG_DEBUG, "job_goto_pk_area: 沒有校場試煉區." );
    send_to_char( "對不起, 目前系統沒有校場試煉區.\n\r", ch );
    RETURN_NULL();
  }

  if ( ch->level <= LEVEL_CAN_PK )
  {
    chinese_number( LEVEL_CAN_PK, buf );
    act( "對不起﹐你的等級必須超過$t級才能參加大混戰﹗",
      ch, buf, NULL, TO_CHAR );
    RETURN_NULL();
  }

  if ( get_age( ch ) <= pk_age )
  {
    chinese_number( pk_age, buf );
    act( "對不起﹐你的年齡必須超過$t歲才能參加大混戰﹗",
      ch, buf, NULL, TO_CHAR );
    RETURN_NULL();
  }

  if ( ch->master || ch->leader )
  {
    send_to_char( " 對不起﹐你正跟隨別人﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( auction_info->seller && auction_info->seller == ch )
  {
    send_to_char( "對不起﹐等你賣完東西再來廝殺吧﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( auction_info->buyer && auction_info->buyer == ch )
  {
    send_to_char( "對不起﹐等你買完東西再來廝殺吧﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( ch->jail > 0 )
  {
    send_to_char( "先把你的刑期服完再說吧﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( IS_SET( ch->act, PLR_KILLER )
    || IS_SET( ch->act, PLR_BOLTER )
    || IS_SET( ch->act, PLR_THIEF ) )
  {
    send_to_char( "你如果參加會讓大家知道你跑路的行蹤﹗\n\r", ch );
    RETURN_NULL();
  }

  if ( ch->mount )
  {
    act( "你還在$N上﹐先下馬吧﹗", ch, NULL, ch->mount, TO_CHAR );
    RETURN_NULL();
  }

  if ( ch->spirit )
  {
    act( "對不起﹐你不能帶著$N參加大混戰喔﹗", ch, NULL, ch->spirit, TO_CHAR );
    RETURN_NULL();
  }

  if ( ch->gold < FIGHT_MONEY )
  {
    send_to_char( "你的錢不夠付門票﹗\n\r", ch );
    RETURN_NULL();
  }

  check_contraband( ch );

  act( "天上飄來一朵雲﹐把$n傳送到三國競技場囉﹗", ch, NULL, NULL, TO_ALL );

  sprintf( buf, "%s偷偷的進入了三國競技場, 快去扁他喔!"
    , mob_name( NULL, ch ) );
  talk_channel_2( buf, CHANNEL_PK, "" );

  char_from_room( ch );

  if ( number_percent() > 50 )
    char_to_room( ch, pIndex1 );
  else
    char_to_room( ch, pIndex2 );

  do_look( ch, "auto" );

  /* 清除追蹤紀錄點 */
  clear_trace( ch, TRUE );

  act( "$n來送死囉﹗", ch, NULL, NULL, TO_ROOM );
  gold_from_char( ch, FIGHT_MONEY );

  RETURN_NULL();
}

JOB( job_bore_hole )
{
  ROOM_INDEX_DATA * pRoom;

  PUSH_FUNCTION( "job_bore_hole" );

  if ( argument[0] && str_cmp( argument, "hole" ) )
  {
    send_to_char( "你想鑽進什麼裡面﹖\n\r", ch );
    RETURN_NULL();
  }

  if ( !ch->in_room
    || !can_char_from_room( ch, TRUE ) ) RETURN_NULL();

  if ( !( pRoom = get_room_index( 13012 ) ) )
  {
    mudlog( LOG_DEBUG, "job_bore_hole: 沒有時間結界房間." );
    send_to_char( "裂縫後方的時空似乎暫時封閉了。\n\r", ch );
    RETURN_NULL();
  }

  act( "$n鼓起勇氣鑽進裂縫裡﹐身影瞬間沒入淡藍色光芒中。"
    , ch, NULL, NULL, TO_ROOM );
  send_to_char( "\e[1;36m你順著裂縫的藍光鑽了進去﹐眼前的時空忽然扭曲起來﹗\e[0m\n\r\n\r", ch );

  char_from_room( ch );
  char_to_room( ch, pRoom );

  do_look( ch, "auto" );
  clear_trace( ch, TRUE );

  act( "$n從扭曲的時空裂縫中跌了出來﹗", ch, NULL, NULL, TO_ROOM );
  RETURN_NULL();
}

JOB( job_pull_bar )
{
  ROOM_INDEX_DATA * pRoom;

  PUSH_FUNCTION( "job_pull_bar" );

  if ( argument[0] && str_cmp( argument, "bar" ) )
  {
    send_to_char( "你想拉動什麼﹖\n\r", ch );
    RETURN_NULL();
  }

  if ( !ch->in_room
    || !can_char_from_room( ch, TRUE ) ) RETURN_NULL();

  if ( !( pRoom = get_room_index( 13006 ) ) )
  {
    mudlog( LOG_DEBUG, "job_pull_bar: 沒有新手動物園外部房間." );
    send_to_char( "拉把卡得死緊﹐怎麼也扳不動。\n\r", ch );
    RETURN_NULL();
  }

  act( "$n用力拉下牆上的拉把﹐一旁的暗門忽然打了開來。"
    , ch, NULL, NULL, TO_ROOM );
  send_to_char( "\e[1;33m你猛然一拉﹐牆邊的暗門應聲滑開﹐把你送回時空空間。\e[0m\n\r\n\r", ch );

  char_from_room( ch );
  char_to_room( ch, pRoom );

  do_look( ch, "auto" );
  clear_trace( ch, TRUE );

  act( "$n狼狽地從隱藏暗門裡跌了出來。", ch, NULL, NULL, TO_ROOM );
  RETURN_NULL();
}

JOB( job_pray_yama )
{
  PUSH_FUNCTION( "job_pray_yama" );

  if ( argument[0] && str_cmp( argument, "pray" ) && str_cmp( argument, "god" ) )
  {
    send_to_char( "你想向誰祈禱﹖\n\r", ch );
    RETURN_NULL();
  }

  act( "$n虔誠地向閻羅王神像伏地祈禱﹐殿中的熱浪似乎稍稍退去了。"
    , ch, NULL, NULL, TO_ROOM );
  send_to_char( "\e[1;31m你誠心祈求閻羅王垂憐﹐一股清涼的氣息暫時撫平了灼熱與痛苦。\e[0m\n\r", ch );

  ch->hit  = UMAX( 1, get_curr_hit( ch ) );
  ch->mana = UMAX( 1, get_curr_mana( ch ) );
  ch->move = UMAX( 1, get_curr_move( ch ) );

  RETURN_NULL();
}

JOB( job_touch_stone )
{
  ROOM_INDEX_DATA * pRoom;

  PUSH_FUNCTION( "job_touch_stone" );

  if ( argument[0] && str_cmp( argument, "stone" ) )
  {
    send_to_char( "你想碰觸什麼﹖\n\r", ch );
    RETURN_NULL();
  }

  if ( !ch->in_room
    || !can_char_from_room( ch, TRUE ) ) RETURN_NULL();

  if ( !( pRoom = get_room_index( 13020 ) ) )
  {
    mudlog( LOG_DEBUG, "job_touch_stone: 沒有冰穴魔堡房間." );
    send_to_char( "冰玄石前的結界猛然一震﹐逼得你不敢再靠近。\n\r", ch );
    RETURN_NULL();
  }

  act( "$n忍不住伸手碰向冰玄石﹐卻被結界爆出的寒氣猛然震飛。"
    , ch, NULL, NULL, TO_ROOM );
  send_to_char( "\e[1;36m你的指尖才剛碰到冰玄石﹐結界便爆出刺骨寒流﹐把你震回魔堡內部﹗\e[0m\n\r\n\r", ch );

  ch->hit  = UMAX( 1, ch->hit - UMAX( 1, get_curr_hit( ch ) / 8 ) );
  ch->move = UMAX( 0, ch->move - UMAX( 1, get_curr_move( ch ) / 8 ) );

  char_from_room( ch );
  char_to_room( ch, pRoom );

  do_look( ch, "auto" );
  clear_trace( ch, TRUE );

  act( "$n被冰玄石的寒氣狠狠震了回來﹐看起來相當狼狽。", ch, NULL, NULL, TO_ROOM );
  RETURN_NULL();
}
