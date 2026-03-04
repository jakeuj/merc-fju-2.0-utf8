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
#include <time.h>
#include <dirent.h>
#include "merc.h"

void vote_attach        args( ( CHAR_DATA * ) );
char *  show_vote_all   args( ( CHAR_DATA *, VOTE_DATA * ) );
void load_a_vote        args( ( VOTE_DATA *, FILE * ) );
bool write_vote_to_file args( ( VOTE_DATA * ) );
bool set_vote           args( ( CHAR_DATA *, VOTE_DATA *, int ) );
bool can_vote           args( ( CHAR_DATA *, VOTE_DATA * ) );
bool show_vote          args( ( CHAR_DATA *, VOTE_DATA * ) );
int  voted_char         args( ( VOTE_DATA * ) );

FUNCTION( do_vote )
{
  VOTE_DATA * pVote;
  CLUB_DATA * pClub;
  char      * strtime;
  char        arg[MAX_INPUT_LENGTH];
  char        buf[MAX_STRING_LENGTH];
  int         loop;
  int         count;

  PUSH_FUNCTION( "do_vote" );

  if ( IS_NPC( ch ) ) RETURN_NULL();

  argument = one_argument( argument, arg );

  if ( arg[0] == '\x0' || !str_prefix( arg, "list" ) )
  {
    clear_buffer();
    for ( loop = 0, pVote = vote_list; pVote; pVote = pVote->next )
    {
      str_cpy( buf, pVote->subject );
      buf[33] = '\x0';

      send_to_buffer( "\e[1;36m%3d\e[0m. 等級﹕%3d 舉辦者﹕\e[1;32m%-12s\e[0m "
        "[%s] 主旨﹕\e[1;34m%s\e[0m\n\r"
        , loop++, pVote->level, pVote->poster
        , YESNO( can_vote( ch, pVote ) )
        , buf );
    }

    if ( loop == 0 ) send_to_buffer( "目前沒有舉辦任何投票﹗\n\r" );
    print_buffer( ch );

    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "subject" ) )
  {
    if ( !argument || !*argument )
    {
      send_to_char( "你必須註明主旨﹗\n\r", ch );
      RETURN_NULL();
    }

    ansi_transcribe( argument, buf );
    smash_tilde( buf );
    vote_attach( ch );
    free_string( ch->vote->subject );
    ch->vote->subject = str_dup( buf );
    send_to_char( "完成設定投票主旨。\n\r", ch );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "text" ) )
  {
    if ( !ch->desc ) RETURN_NULL();

    vote_attach( ch );

    ch->desc->edit_mode = EDIT_VOTE;
    show_editor( ch );

    free_string( ch->stack );
    ch->stack = str_dup( "" );

    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "post" ) )
  {
    if ( !( pVote = ch->vote ) )
    {
      send_to_char( "你尚未設定好你的選項﹐所以無法舉辦﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( !pVote->subject || !*pVote->subject )
    {
      send_to_char( "你這次投票的主旨是什麼﹐你得設定好﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( !pVote->text || !*pVote->text )
    {
      send_to_char( "你這次投票的意義是什麼﹐你得設定好它﹗\n\r", ch );
      RETURN_NULL();
    }

    for ( loop = count = 0; loop < MAX_VOTES; loop++ )
      if ( pVote->message[loop] && *( pVote->message[loop] ) ) count++;

    if ( count <= 1 )
    {
      send_to_char( "你投票的選項尚未設定好﹐無法舉辦﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( ch->gold < VoteGold )
    {
      act( "對不起﹐你身上必須有$i兩銀子﹐方能舉辦投票﹗"
        , ch, &VoteGold, NULL, TO_CHAR );

      RETURN_NULL();
    }

    ch->vote->next              = NULL;
    strtime                     = ctime( &current_time );
    strtime[str_len(strtime)-1] = '\x0';
    ch->vote->date              = str_dup( strtime );
    ch->vote->stamp             = (int) current_time;

    if ( !vote_list )
    {
      vote_list = ch->vote;
    }

    else
    {
      for ( pVote = vote_list; pVote->next; pVote = pVote->next );
      pVote->next = ch->vote;
    }

    pVote    = ch->vote;
    ch->vote = NULL;

    if ( !write_vote_to_file( pVote ) )
    {
      send_to_char( "舉辦投票失敗﹐系統無法開啟﹗\n\r", ch );
      RETURN_NULL();
    }

    send_to_char( "完成舉辦投票手續。\n\r", ch );

    sprintf( buf, "%s舉辦了一場有關%s\e[0m的投票﹐請大家"
      "踴躍參加﹐謝謝﹗", mob_name( NULL, ch ), pVote->subject );

    talk_channel_2( buf, CHANNEL_BULLETIN, "投票" );

    gold_from_char( ch, VoteGold );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "set" ) )
  {
    vote_attach( ch );

    argument = one_argument( argument, arg );

    if ( arg[0] == '\x0' || !is_number( arg ) )
    {
      send_to_char( "你要設定哪一個選項呢﹖\n\r", ch );
      RETURN_NULL();
    }

    loop = atoi( arg ) - 1;
    if ( loop < 0 || loop >= MAX_VOTES )
    {
      print_to_char( ch, "第一個參數只能由 1 到 %d 而已﹗\n\r", MAX_VOTES );
      RETURN_NULL();
    }

    if ( !argument || !*argument )
    {
      send_to_char( "你要圈選的項目是什麼呢﹖\n\r", ch );
      RETURN_NULL();
    }

    ansi_transcribe( argument, buf );
    smash_tilde( buf );

    free_string( ch->vote->message[loop] );
    ch->vote->message[loop] = str_dup( buf );
    send_to_char( "完成投票圈選項目﹗\n\r", ch );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "show" ) )
  {
    if ( !ch->vote )
    {
      send_to_char( "你沒有舉辦過任何的投票﹐無法察看﹗\n\r", ch );
      RETURN_NULL();
    }

    send_to_char( show_vote_all( ch, ch->vote ), ch );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "clear" ) )
  {
    if ( !( pVote = ch->vote ) )
    {
      send_to_char( "你的行程裡沒有舉辦任何投票﹐所以無法清除﹗\n\r", ch );
      RETURN_NULL();
    }

    free_string( pVote->poster  );
    free_string( pVote->subject );
    free_string( pVote->text    );
    free_string( pVote->date    );
    free_string( pVote->club    );

    for ( loop = 0; loop < MAX_VOTES; loop++ )
      free_string( pVote->message[loop] );

    for ( loop = 0; loop < MAX_POLL; loop++ )
      free_string( pVote->poller[loop] );

    /* 釋放計憶體 */
    free_struct( ch->vote, STRUCT_VOTE_DATA );
    ch->vote = NULL;

    send_to_char( "已經清除投票設定的內容﹗\n\r", ch );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "club" ) )
  {
    if ( !( pVote = ch->vote ) )
    {
      send_to_char( "你的行程裡沒有舉辦任何投票﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( !( pClub = ch->club ) || !pClub->name || !*pClub->name )
    {
      send_to_char( "對不起﹐你沒有參加幫派﹐無法設定﹗\n\r", ch );
      RETURN_NULL();
    }

    free_string( pVote->club );
    pVote->club = str_dup( pClub->name );

    send_to_char( "你設定這次投票為幫派投票﹗", ch );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "days" ) )
  {
    if ( !( pVote = ch->vote ) )
    {
      send_to_char( "你的行程裡沒有舉辦任何投票﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( !argument || !*argument || !is_number( argument ) )
    {
      send_to_char( "你想幾天後開票呢﹖\n\r", ch );
      RETURN_NULL();
    }

    count = atoi( argument );

    if ( count < VOTE_MIN_DAYS || count > VOTE_DAYS )
    {
      send_to_char( "你設定的可投票日期不合理﹗\n\r", ch );
      RETURN_NULL();
    }

    pVote->days = count;
    act( "你設定這次投票$i天後開票﹗", ch, &count, NULL, TO_CHAR );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "limit" ) )
  {
    if ( !( pVote = ch->vote ) )
    {
      send_to_char( "你的行程裡沒有舉辦任何投票﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( !argument || !*argument || !is_number( argument ) )
    {
      send_to_char( "你限制這是等級多少的投票呢﹖\n\r", ch );
      RETURN_NULL();
    }

    count = atoi( argument );

    if ( count < VOTE_LEVEL  || count > LEVEL_HERO )
    {
      send_to_char( "你限制的等級不合理喔﹗\n\r", ch );
      RETURN_NULL();
    }

    pVote->level = count;
    act( "你限制你的投票必須等級$i以上方可圈選﹗", ch, &count, NULL, TO_CHAR );

    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "read" ) )
  {
    if ( !argument || !*argument || !is_number( argument ) )
    {
      send_to_char( "你要讀哪一個投票的內容呢﹖\n\r", ch );
      RETURN_NULL();
    }

    count = atoi( argument );

    for ( loop = 0, pVote = vote_list; pVote; pVote = pVote->next, loop++ )
    {
      if ( loop == count )
      {
        if ( pVote->lock )
        {
          send_to_char( "對不起﹐這個項目被鎖定了﹐無法讀取喔﹗\n\r", ch );
          RETURN_NULL();
        }

        send_to_char( show_vote_all( ch, pVote ), ch );
        RETURN_NULL();
      }
    }

    send_to_char( "對不起﹐沒有那項投票﹗\n\r", ch );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "moninal" ) )
  {
    if ( !( pVote = ch->vote ) )
    {
      send_to_char( "你的行程裡沒有舉辦任何投票﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( pVote->moninal )
    {
      pVote->moninal = FALSE;
      send_to_char( "你把你的投票改成不記名投票﹗\n\r", ch );
    }

    else
    {
      pVote->moninal = TRUE;
      send_to_char( "你把你的投票改成記名投票﹗\n\r", ch );
    }
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "lock" ) && IS_IMMORTAL( ch ) )
  {
    if ( !argument || !*argument || !is_number( argument ) )
    {
      send_to_char( "你要鎖定哪一個投票呢﹖\n\r", ch );
      RETURN_NULL();
    }

    count = atoi( argument );
    for ( loop = 0, pVote = vote_list; pVote; pVote = pVote->next, loop++ )
    {
      if ( loop == count )
      {
        if ( pVote->lock )
        {
          pVote->lock = FALSE;
          act( "你解除了$t的投票鎖定﹗", ch, pVote->subject, NULL, TO_CHAR );
        }
        else
        {
          pVote->lock = TRUE;
          act( "你設定了$t的投票鎖定﹗", ch, pVote->subject, NULL, TO_CHAR );
        }

        write_vote_to_file( pVote );
        RETURN_NULL();
      }
    }

    send_to_char( "對不起﹐沒有那項投票﹗\n\r", ch );
    RETURN_NULL();
  }

  else if ( !str_prefix( arg, "poll" ) )
  {
    argument = one_argument( argument, arg );

    if ( arg[0] == '\x0' || !is_number( arg ) )
    {
      send_to_char( "你要投哪一次的選舉呢﹖\n\r", ch );
      RETURN_NULL();
    }

    count = atoi( arg );

    for ( loop = 0, pVote = vote_list; pVote; pVote = pVote->next, loop++ )
      if ( loop == count ) break;

    if ( !pVote )
    {
      send_to_char( "對不起﹐沒有那項投票﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( pVote->lock )
    {
      send_to_char( "對不起﹐這個投票作業被暫時禁止的﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( !can_vote( ch, pVote ) )
    {
      send_to_char( "對不起﹐你不能參與這個選舉了﹗\n\r", ch );
      RETURN_NULL();
    }

    if ( !argument || !*argument || !is_number( argument ) )
    {
      send_to_char( "你要圈選哪一個選項呢\n\r", ch );
      RETURN_NULL();
    }

    count = atoi( argument ) - 1;
    if ( count < 0
      || count >= MAX_VOTES
      || pVote->message[count] == NULL
      || *( pVote->message[count] ) == '\x0' )
    {
      send_to_char( "對不起﹐那是無效的選項﹗\n\r", ch );
      RETURN_NULL();
    }

    set_vote( ch, pVote, count );
    write_vote_to_file( pVote );

    print_to_char( ch, "你對%s所舉辦的有關%s\e[0m選舉﹐圈選了第%d﹐"
      "有關於%s的選項﹗\n\r"
      , pVote->poster, pVote->subject, count + 1, pVote->message[count] );

    sprintf( buf, "%s沈思頗久﹐終於在\e[1;32m%s\e[0m上面蓋\上"
      "他神聖的一票﹗"
      , mob_name( NULL, ch ), pVote->subject );

    talk_channel_2( buf, CHANNEL_BULLETIN, "投票" );

    RETURN_NULL();
  }

  else
  {
    send_to_char( "你的參數錯誤﹐請查詢 vote 的使用方法﹗\n\r", ch );
    RETURN_NULL();
  }

  RETURN_NULL();
}

void vote_attach( CHAR_DATA * ch )
{
  VOTE_DATA * pVote;

  PUSH_FUNCTION( "vote_attach" );

  if ( !ch || !ch->name )
  {
    mudlog( LOG_DEBUG, "vote_attach: 來源不正確." );
    RETURN_NULL();
  }

  if ( IS_NPC( ch ) || ch->vote ) RETURN_NULL();

  set_vote_default( pVote = alloc_struct( STRUCT_VOTE_DATA ) );

  pVote->poster  = str_dup( ch->name );
  ch->vote       = pVote;

  RETURN_NULL();
}

char * show_vote_all( CHAR_DATA * ch, VOTE_DATA * pVote )
{
  static char   buf[MAX_STRING_LENGTH];
  char        * strtime;
  int           loop;
  int           count;
  time_t        end_time;
  int           iLine;

  PUSH_FUNCTION( "show_vote_all" );

  if ( !pVote )
  {
    mudlog( LOG_DEBUG, "show_vote_all: 來源不正確﹗" );
    RETURN( "" );
  }

  end_time = pVote->stamp + pVote->days * 24 * 60 * 60;
  strtime = ctime( &end_time );
  strtime[str_len(strtime)-1] = '\x0';

  clear_stack();
  send_to_stack(
    "舉行投票者  ﹕%s\e[0m\n\r"
    "投票主旨    ﹕%s\e[0m\n\r"
    "舉辦日期    ﹕%s\n\r"
    "截止日期    ﹕%s\n\r"
    "舉辦日期    ﹕%d 天\n\r"
    "幫派投票    ﹕%s\n\r"
    "已投票人數  ﹕%d\n\r"
    "投票最低等級﹕%d\n\r"
    "投票進行中  ﹕%s\n\r"
    "是否有投票權﹕%s\n\r"
    "記名投票    ﹕%s\n\r"
    "投票意義    ﹕\n\r%s%s%s"
    , pVote->poster
    , pVote->subject && *pVote->subject ? pVote->subject : "尚未決定"
    , pVote->date && *pVote->date ? pVote->date : "尚未決定"
    , pVote->stamp > 0  ? strtime : "尚未決定"
    , pVote->days
    , pVote->club && *pVote->club ? pVote->club : "非幫派投票"
    , voted_char( pVote )
    , pVote->level
    , YESNO( !vote_timeup( pVote ) )
    , ch ? YESNO( can_vote( ch, pVote ) ) : "﹖"
    , YESNO( pVote->moninal )
    , VERTICAL_LINE, pVote->text, VERTICAL_LINE );

  for ( loop = 0; loop < MAX_VOTES; loop++ )
  {
    if ( !pVote->message[loop] || !*pVote->message[loop] ) continue;
    send_to_stack( "選項 %2d﹕%3d 票 %s\e[0m\n\r"
      , loop + 1
      , ( !ch || show_vote( ch, pVote ) ) ? pVote->poll[loop] : 0
      , pVote->message[loop] );
  }

  if ( vote_timeup( pVote ) && pVote->moninal )
  {
    send_to_stack( "\n\r" );
    for ( count = 0; count < MAX_VOTES; count++ )
    {
      if ( !pVote->message[count] || !*pVote->message[count] ) continue;

      send_to_stack( "投給選項%d(%s\e[0m)的人有﹕"
        , count + 1, pVote->message[count] );

      for ( iLine = loop = 0; loop < MAX_POLL; loop++ )
      {
        if ( pVote->vote[loop] == count
          && pVote->poller[loop]
          && *( pVote->poller[loop] ) )
        {
          if ( ( iLine++ % 6 ) == 0 ) send_to_stack( "\n\r" );
          send_to_stack( "%-12s", pVote->poller[loop] );
        }
      }

      if ( iLine == 0 ) send_to_stack( "\n\r沒有人投這個選項﹗\n\r\n\r" );
      else              send_to_stack( "\n\r\n\r" );
    }
  }

  str_ncpy( buf, return_stack(), sizeof( buf ) - 10 );
  RETURN( buf );
}

bool write_vote_to_file( VOTE_DATA * pVote )
{
  char   filename[MAX_FILE_LENGTH];
  int    loop;
  FILE * pFile;

  PUSH_FUNCTION( "write_vote_to_file" );

  if ( !pVote )
  {
    mudlog( LOG_DEBUG, "write_vote_to_file: 缺乏來源." );
    RETURN( FALSE );
  }

  sprintf( filename, "%s/%d.vot", vote_dir, pVote->stamp );

  if ( ( pFile = FOPEN( filename , "w" ) ) )
  {
    fprintf( pFile, "Poster          %s~\n", pVote->poster  );
    fprintf( pFile, "Subject         %s~\n", pVote->subject );
    fprintf( pFile, "Club            %s~\n", pVote->club    );
    fprintf( pFile, "Text\n%s~\n"          , pVote->text    );
    fprintf( pFile, "Date            %s~\n", pVote->date    );
    fprintf( pFile, "Days            %d\n" , pVote->days    );
    fprintf( pFile, "Stamp           %d\n" , pVote->stamp   );
    fprintf( pFile, "Level           %d\n" , pVote->level   );
    fprintf( pFile, "Lock            %d\n" , pVote->lock    );
    fprintf( pFile, "Moninal         %d\n" , pVote->moninal );

    for ( loop = 0; loop < MAX_VOTES; loop++ )
    {
      if ( pVote->message[loop] && *( pVote->message[loop] ) )
      {
        fprintf( pFile, "Vote            %d %s~\n"
          , loop, pVote->message[loop] );
      }
    }

    for ( loop = 0; loop < MAX_POLL; loop++ )
    {
      if ( pVote->poller[loop] && *( pVote->poller[loop] ) )
      {
        fprintf( pFile, "Ballot          %d %s~\n"
          , pVote->vote[loop], pVote->poller[loop] );
      }
    }

    fprintf( pFile, "End\n" );
    FCLOSE( pFile );

    /* 改變檔案存取模式 */
    set_file_mode( filename );
    RETURN( TRUE );
  }

  RETURN( FALSE );
}

bool can_vote( CHAR_DATA * ch, VOTE_DATA * pVote )
{
  CLUB_DATA * pClub;
  int         loop;
  int         hold_times;
  bool        bVote;

  PUSH_FUNCTION( "can_vote" );

  if ( !ch || !ch->name || !pVote )
  {
    mudlog( LOG_DEBUG, "can_vote: 缺乏來源." );
    RETURN( FALSE );
  }

  /* 怪物不能投票 */
  if ( IS_NPC( ch ) ) RETURN( FALSE );

  /* 看看是否被鎖定了 */
  if ( pVote->lock ) RETURN( FALSE );

  /* 看看是否已經過期 */
  hold_times = pVote->stamp + pVote->days * 24 * 60 * 60;
  if ( current_time > hold_times ) RETURN( FALSE );

  /* 是否為幫派投票 */
  if ( pVote->club && *pVote->club )
  {
    if ( !( pClub = ch->club ) || !pClub->name || !*pClub->name )
      RETURN( FALSE );

    if ( str_cmp( pClub->name, pVote->club ) ) RETURN( FALSE );
  }

  /* 看看是否還有空位 */
  for ( bVote = FALSE, loop = 0; loop < MAX_POLL; loop++ )
  {
    if ( pVote->poller[loop] && *( pVote->poller[loop] ) == '\x0' )
    {
      bVote = TRUE;
      break;
    }
  }

  if ( !bVote ) RETURN( FALSE );

  /* 等級低的也不能投票 */
  if ( pVote->level > ch->level ) RETURN( FALSE );

  /* 不能重複投票 */
  for ( loop = 0; loop < MAX_POLL; loop++ )
    if ( pVote->poller[loop] && !str_cmp( pVote->poller[loop], ch->name ) )
      RETURN( FALSE );

  RETURN( TRUE );
}

bool set_vote( CHAR_DATA * ch, VOTE_DATA * pVote, int choice )
{
  int loop;

  PUSH_FUNCTION( "set_vote" );

  if ( !ch || !ch->name || !pVote )
  {
    mudlog( LOG_DEBUG, "set_vote: 缺乏來源." );
    RETURN( FALSE );
  }

  choice = UMAX( 0, UMIN( MAX_VOTES - 1, choice ) );

  for ( loop = 0; loop < MAX_POLL; loop++ )
  {
    if ( pVote->poller[loop] && *( pVote->poller[loop] ) == '\x0' )
    {
      pVote->poller[loop] = str_dup( ch->name );
      pVote->vote[loop]   = choice;
      pVote->poll[choice]++;
      RETURN( TRUE );
    }
  }

  RETURN( FALSE );
}

int vote_count( CHAR_DATA * ch )
{
  int         count;
  VOTE_DATA * pVote;

  PUSH_FUNCTION( "vote_count" );

  if ( !ch )
  {
    mudlog( LOG_DEBUG, "vote_count: 缺乏來源." );
    RETURN( 0 );
  }

  for ( count = 0, pVote = vote_list; pVote; pVote = pVote->next )
    if ( can_vote( ch, pVote ) ) count++;

  RETURN( count );
}

bool show_vote( CHAR_DATA * ch, VOTE_DATA * pVote )
{

  PUSH_FUNCTION( "show_vote" );

  if ( !ch || !pVote )
  {
    mudlog( LOG_DEBUG, "show_vote: 缺乏來源." );
    RETURN( FALSE );
  }

  /* 怪物一定不能看投票結果 */
  if ( IS_NPC( ch ) ) RETURN( FALSE );

  /* 神族一定可以 */
  if ( IS_IMMORTAL( ch ) ) RETURN( TRUE );

  if ( vote_timeup( pVote ) == TRUE ) RETURN( TRUE );
  RETURN( FALSE );
}

bool vote_timeup( VOTE_DATA * pVote )
{
  int  loop;
  int  hold_times;
  bool bVote;

  PUSH_FUNCTION( "vote_timeup" );

  if ( !pVote )
  {
    mudlog( LOG_DEBUG, "vote_timeup: 缺乏來源." );
    RETURN( FALSE );
  }

  /* 時間到期了 */
  hold_times = pVote->stamp + pVote->days * 24 * 60 * 60;
  if ( current_time > hold_times ) RETURN( TRUE );

  /* 人數滿了 */
  for ( bVote = TRUE, loop = 0; loop < MAX_POLL; loop++ )
  {
    if ( pVote->poller[loop] == NULL
      || ( pVote->poller[loop] && *( pVote->poller[loop] ) == '\x0' ) )
    {
      bVote = FALSE;
      break;
    }
  }

  if ( bVote ) RETURN( TRUE );

  RETURN( FALSE );
}

int voted_char( VOTE_DATA * pVote )
{
  int count;
  int loop;

  PUSH_FUNCTION( "voted_char" );

  if ( !pVote )
  {
    mudlog( LOG_DEBUG, "voted_char: 缺乏來源." );
    RETURN( 0 );
  }

  for ( loop = count = 0; loop < MAX_POLL; loop++ )
    if ( pVote->poller[loop] && *( pVote->poller[loop] ) ) count++;

  RETURN( count );
}

void vote_to_note( VOTE_DATA * pVote )
{
  NOTE_DATA   * pNote;
  static int    count = 0;
  char          buf[MAX_STRING_LENGTH];

  PUSH_FUNCTION( "vote_to_note" );

  if ( !pVote )
  {
    mudlog( LOG_DEBUG, "vote_to_note: 來源不正確." );
    RETURN_NULL();
  }

  pNote             = alloc_struct( STRUCT_NOTE_DATA );
  pNote->sender     = str_dup( pVote->poster  );
  pNote->date_stamp = time( NULL ) + count++;

  sprintf( buf, "%d", ( int ) pNote->date_stamp );
  pNote->date = str_dup( buf );

  if ( pVote->club && *pVote->club )
  {
    sprintf( buf, "@%s", pVote->club );
    pNote->to_list = str_dup( buf );
  }
  else
  {
    pNote->to_list = str_dup( "all" );
  }

  sprintf( buf, "%s/%d.%s", note_dir, ( int ) pNote->date_stamp, note_ext );
  pNote->filename = str_dup( buf );

  sprintf( buf, "(\e[1;32m投票結果\e[0m) %s", pVote->subject );
  pNote->subject = str_dup( buf );
  pNote->text    = str_dup( show_vote_all( NULL, pVote ) );

  write_note( pNote );

  free_string( pNote->filename );
  free_string( pNote->sender   );
  free_string( pNote->date     );
  free_string( pNote->to_list  );
  free_string( pNote->subject  );
  free_string( pNote->text     );

  free_struct( pNote, STRUCT_NOTE_DATA );

  RETURN_NULL();
}
