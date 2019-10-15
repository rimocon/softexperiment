/*****************************************************************
ファイル名	: client_main.c
機能		: クライアントのメインルーチン
*****************************************************************/

#include<SDL/SDL.h>
#include"common.h"
#include"client_func.h"

static int NetworkEvent(void *data);

int main(int argc,char *argv[])
{
    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    int		endFlag=1;
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;
	SDL_Thread	*thr;
	
    /* 引き数チェック */
    if(argc == 1){
    	serverName = localHostName;
    }
    else if(argc == 2){
    	serverName = argv[1];
    }
    else{
		fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
		return -1;
    }

    /* サーバーとの接続 */
    if(SetUpClient(serverName,&clientID,&num,name)==-1){
		fprintf(stderr,"setup failed : SetUpClient\n");
		return -1;
	}
    /* ウインドウの初期化 */
	if(InitWindows(clientID,num,name)==-1){
		fprintf(stderr,"setup failed : InitWindows\n");
		return -1;
	}

    /* ネットワークイベント処理スレッドの作成 */
	thr = SDL_CreateThread(NetworkEvent, &endFlag);
	
	/* メインイベントループ */
    while(endFlag){
		WindowEvent(num);
    };

    /* 終了処理 */
	SDL_WaitThread(thr,NULL);
	DestroyWindow();
	CloseSoc();

    return 0;
}

/*****************************************************************
関数名  : NetworkEvent
機能    : ネットワークのイベントループ
引数    : void		*data		: 終了判定フラグ
出力    : スレッド終了時に０を返す
*****************************************************************/
static int NetworkEvent(void *data)
{
	int *endFlag;
	
	endFlag = (int*)data;
    while(*endFlag){
        *endFlag = SendRecvManager();
    }
	return 0;
}
