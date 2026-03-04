#include "edit.h"

#define 	LINK_X 		30
#define		LINK_Y 		5
#define		LINK_COLOR	YELLOW

void  link_file		( FILE * , char * , int );

/* 檔案連結主程式 */
void do_link( void )
{
  FILE_INFO     * pFile;
  char            buffer[1024];

  /* 輸入檔名並且開啟檔案 */
  pFile = get_file_name( ".are" );

  /* 檔案無法讀取 */
  if ( pFile->information < 0 ) return;

  /* 重新開啟為可以寫的方式 */
  fclose( pFile->filehandler );
  pFile->filehandler = fopen( pFile->filename ,"w+" );

  sprintf( buffer , "連結 %s 檔案中, 請稍後." , pFile->filename );
  print_string( LINK_X -5 , LINK_Y , LINK_COLOR , buffer );

  /* 連結附檔名為 .sec 的檔案 */
  link_file( pFile->filehandler , ".sec" , 2 );

  /* 連結附檔名為 .roo 的檔案 */
  link_file( pFile->filehandler , ".roo" , 3 );

  /* 連結附檔名為 .sho 的檔案 */
  link_file( pFile->filehandler , ".sho" , 4 );

  /* 連結附檔名為 .res 的檔案 */
  link_file( pFile->filehandler , ".res" , 5 );

  /* 連結附檔名為 .spe 的檔案 */
  link_file( pFile->filehandler , ".spe" , 6 );

  /* 檔案最後面寫入 #$ 符號 */
  fprintf( pFile->filehandler , "#$\n" );
  fclose( pFile->filehandler );

  /* 完成連結 */
  print_string( LINK_X - 4 , LINK_Y + 8 , LINK_COLOR
    , "連結完成, 請按任何鍵繼續" );
  get_control_key();

  return;
}

/* 輸入附檔名來進行檔案連結 */
void link_file( FILE * pFileDest , char * lastname , int shift )
{
  DIR           * reading;
  struct dirent * next;
  FILE          * pRead;
  char            transfer;
  int             file_count;
  char            buffer [1024];

  sprintf( buffer , "%s 連結個數 : %d " , lastname , ( file_count = 0 ) );
  print_string( LINK_X , LINK_Y + shift , LINK_COLOR , buffer );

  /* 開啟目前目錄 */
  if ( ( reading = opendir( LOCAL_DIRECTORY ) ) != NULL )
  {
    /* 讀取目錄中檔案的名字 */
    while ( ( next = readdir( reading ) ) != NULL )
    {
      /* 比較檔案的名稱 */
      if ( string_compare( next->d_name , lastname ) )
      {
        /* 以先前的檔名來進行開檔 */
        if ( ( pRead = fopen( next->d_name , "r+" ) ) != NULL )
        {
          /* 一直讀取直到檔案末端 */
          while ( ( transfer = fgetc( pRead ) ) != EOF )
            fputc( transfer , pFileDest );

          /* 檔案末端送出換行的命令 */
          fputc( '\n' , pFileDest );
          fclose( pRead );

          /* 印出已連結的檔案數目 */
          sprintf( buffer , "%4s 連結個數 : %d "
            , lastname , ++file_count );

          print_string( LINK_X , LINK_Y + shift , LINK_COLOR , buffer );

        }
      }
    }

    /* 關閉先前開啟的目錄結構 */
    closedir( reading );
  }

  return;
}

#undef		LINK_X
#undef          LINK_Y
#undef          LINK_COLOR
