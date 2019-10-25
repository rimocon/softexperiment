/*****************************************************************
ファイル名	: server_command.c
機能		: サーバーのコマンド処理
*****************************************************************/

#include"server_common.h"
#include"server_func.h"

static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static int GetRandomInt(int n);
static void judge(int command0,int command1);
static void waitclient(int pos,int command);
/*****************************************************************
関数名	: ExecuteCommand
機能	: クライアントから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
		  int	pos			: コマンドを送ったクライアント番号
出力	: プログラム終了コマンドが送られてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;

    /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n",command);
#endif
    switch(command){
	    case END_COMMAND:
			dataSize = 0;
			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);

			/* 全ユーザーに送る */
			SendData(ALL_CLIENTS,data,dataSize);

			endFlag = 0;
			break;
	    case CIRCLE_COMMAND:
			/* 円を表示するクライアント番号を受信する */
			RecvIntData(pos,&intData);

			dataSize = 0; //キャスト
			/* コマンドのセット */
			SetCharData2DataBlock(data,command,&dataSize);
			/* 左上の x 座標のセット */
			SetIntData2DataBlock(data,GetRandomInt(500),&dataSize);
			/* 左上の y 座標のセット */
			SetIntData2DataBlock(data,GetRandomInt(300),&dataSize);
			/* 直径のセット */
			SetIntData2DataBlock(data,GetRandomInt(100),&dataSize);

			/* 指定されたクライアントに送る */
			SendData(intData,data,dataSize);
			break;
	    case ROCK_COMMAND: //グーの場合
      ////ここに判定用関数
        waitclient(pos,0); //他のクライアントを待つ
      break;
      case SCISSORS_COMMAND: //チョキの場合
        waitclient(pos,1); //他のクライアントを待つ
      break;
      case PAPER_COMMAND: //パーの場合
        waitclient(pos,2);
      break;

	    default:
			/* 未知のコマンドが送られてきた */
			fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
}

/*****
static
*****/
/*****************************************************************
関数名	: SetIntData2DataBlock
機能	: int 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}

/*****************************************************************
関数名	: SetCharData2DataBlock
機能	: char 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}

/*****************************************************************
関数名	: GetRandomInt
機能	: 整数の乱数を得る
引数	: int		n	: 乱数の最大値
出力	: 乱数値
*****************************************************************/
static int GetRandomInt(int n)
{
    return rand()%n;
}

static void waitclient(int pos,int command)
{
  int flag0;
  int flag1;
  int command0;
  int command1;

  if(pos == 0) { //クライアント0の方セット
    flag0 = 1;
    command0 = command;
    fprintf(stderr,"client0 set\n");
  }
  else if(pos == 1) { //クライアント1の方セット
    flag1 = 1;
    command1 = command;
    fprintf(stderr,"client1 set\n");
  }
  if(flag0 == 1 && flag1 == 1) { //どっちからも送られてきたら判定
    
    fprintf(stderr,"judge start\n");
    judge(command0,command1);
    flag0 = 0;
    flag1 = 0;
  }
}
static void judge(int command0,int command1){
  int result;
  unsigned char	data1[MAX_DATA];
  unsigned char	data2[MAX_DATA];
	int dataSize = 0; //キャスト
  result = (command0 - command1 +3) % 3; 
  switch(result){
  case 2: //1勝ち2負け
    SetCharData2DataBlock(data1,WIN,&dataSize);
	  SendData(0,data1,dataSize);
    SetCharData2DataBlock(data2,LOSE,&dataSize);
    SendData(1,data2,dataSize);
    fprintf(stderr,"1win2lose\n");
    break;
  case 1: //1負け2勝ち
    SetCharData2DataBlock(data1,LOSE,&dataSize);
	  SendData(0,data1,dataSize);
    SetCharData2DataBlock(data2,WIN,&dataSize);
    SendData(1,data2,dataSize);
    fprintf(stderr,"1lose2win\n");
    break;
  case 0://あいこ
    SetCharData2DataBlock(data1,DRAW,&dataSize);
    SendData(ALL_CLIENTS,data1,dataSize);
    fprintf(stderr,"draw\n");
    break;
  }
  fprintf(stderr,"data1= %s \n",data1);
  fprintf(stderr,"data2= %s \n",data2);
}
