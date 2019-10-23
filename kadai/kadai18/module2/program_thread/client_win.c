/*****************************************************************
ファイル名	: client_win.c
機能		: クライアントのユーザーインターフェース処理
*****************************************************************/

#include<SDL/SDL.h>
#include<SDL/SDL_image.h>
#include<SDL/SDL_gfxPrimitives.h>
#include"common.h"
#include"client_func.h"

static SDL_Surface *gMainWindow;
static SDL_Rect gButtonRect[MAX_CLIENTS+2];

static int CheckButtonNO(int x,int y,int num);

/*****************************************************************
関数名	: InitWindows
機能	: メインウインドウの表示，設定を行う
引数	: int	clientID		: クライアント番号
		  int	num				: 全クライアント数
出力	: 正常に設定できたとき0，失敗したとき-1
*****************************************************************/
int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE])
{
	int i;
	SDL_Surface *image;
	char rockButton[] = "rock.jpg";
	char scissorsButton[] = "scissors.jpg";
	char paperButton[] = "paper.jpg";
	char endButton[]="END.jpg";
	char *s,title[10];

    /* 引き数チェック */
    assert(0<num && num<=MAX_CLIENTS);
	
	/* SDLの初期化 */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		return -1;
	}
	
	/* メインのウインドウを作成する */
	if((gMainWindow = SDL_SetVideoMode(500,300, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		return -1;
	}
	/* ウインドウのタイトルをセット */
	sprintf(title,"%d",clientID);
	SDL_WM_SetCaption(title,NULL);
	
	/* 背景を白にする */
	SDL_FillRect(gMainWindow,NULL,0xffffff);

	/* ボタンの作成 */
	for(i=0;i<4;i++){
		gButtonRect[i].x = 20+100*i;
		gButtonRect[i].y=10;
		gButtonRect[i].w=90;
		gButtonRect[i].h=40;
      
		if(i==0){
			s=rockButton;
		}
		else if(i==1){
			s=scissorsButton;
		}
		else if(i==2){
			s=paperButton;
		}
		else{
			s=endButton;
		}
		image = IMG_Load(s); //画像読み込み
		SDL_BlitSurface(image,NULL,gMainWindow,&(gButtonRect[i])); //画像のサーフェイスをメインウィンドウのサーフェイスにコピー
		SDL_FreeSurface(image); //解放
	}
	SDL_Flip(gMainWindow); //更新
	
	return 0;
}

/*****************************************************************
関数名	: DestroyWindow
機能	: SDLを終了する
引数	: なし
出力	: なし
*****************************************************************/
void DestroyWindow(void)
{
	SDL_Quit();
}

/*****************************************************************
関数名	: WindowEvent
機能	: メインウインドウに対するイベント処理を行う
引数	: int		num		: 全クライアント数
出力	: なし
*****************************************************************/
void WindowEvent(int num)
{
	SDL_Event event;
	SDL_MouseButtonEvent *mouse;
	int buttonNO;

    /* 引き数チェック */
    assert(0<num && num<=MAX_CLIENTS);
	
	if(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				SendEndCommand();
				break;
			case SDL_MOUSEBUTTONUP:
				mouse = (SDL_MouseButtonEvent*)&event;
				if(mouse->button == SDL_BUTTON_LEFT){
					buttonNO = CheckButtonNO(mouse->x,mouse->y,num);
#ifndef NDEBUG
					printf("#####\n");
					printf("WindowEvent()\n");
					printf("Button %d is pressed\n",buttonNO);
#endif
					if(buttonNO == 0){
            //グーのボタンが押された
						SendRockCommand();
					}
					if(buttonNO == 1){
            //チョキのボタンが押された
						SendScissorsCommand();
					}
					if(buttonNO == 2){
            //グーのボタンが押された
						SendPaperCommand();
					}
					else if(buttonNO==num+1){
						/* 「End」と書かれたボタンが押された */
						SendEndCommand();
					}
				}
				break;
		}
	}
}

/*****************************************************************
関数名	: DrawRectangle
機能	: メインウインドウに四角を表示する
引数	: int		x			: 四角の左上の x 座標
		  int		y			: 四角の左上の y 座標
		  int		width		: 四角の横幅
		  int		height		: 四角の高さ
出力	: なし
*****************************************************************/
void DrawRectangle(int x,int y,int width,int height)
{
#ifndef NDEBUG
    printf("#####\n");
    printf("DrawRectangle()\n");
    printf("x=%d,y=%d,width=%d,height=%d\n",x,y,width,height);
#endif

	rectangleColor(gMainWindow,x,y,x+width,y+height,0x000000ff);
	SDL_Flip(gMainWindow);
}

/*****************************************************************
関数名	: DrawCircle
機能	: メインウインドウに円を表示する
引数	: int		x		: 円の x 座標
		  int		y		: 円の y 座標
		  int		r		: 円の半径
出力	: なし
*****************************************************************/
void DrawCircle(int x,int y,int r)
{
#ifndef NDEBUG
	printf("#####\n");
    printf("DrawCircle()\n");
    printf("x=%d,y=%d,tyokkei=%d\n",x,y,r);
#endif

    circleColor(gMainWindow,x,y,r,0x000000ff);
	SDL_Flip(gMainWindow);
}

/*****************************************************************
関数名	: DrawDiamond
機能	: メインウインドウに菱形を表示する
引数	: int		x		: 左上の x 座標
		  int		y		: 左上の y 座標
		  int		height		: 高さ
出力	: なし
*****************************************************************/
void DrawDiamond(int x,int y,int height)
{
	Sint16	vx[5],vy[5];
	int	i;

#ifndef NDEBUG
    printf("#####\n");
    printf("DrawDiamond()\n");
    printf("x=%d,y=%d,height=%d\n",x,y,height);
#endif

    for(i=0;i<4;i++){
        vx[i] = x + height*((1-i)%2)/2;
        vy[i] = y + height*((2-i)%2);
    }
    vx[4]=vx[0];
    vy[4]=vy[0];
	
	polygonColor(gMainWindow, vx, vy, 5 , 0x000000ff);
	SDL_Flip(gMainWindow);
}
void DrawRock(){
	  SDL_Surface *image;
    image = IMG_Load("janken_gu.png"); //画像読み込み
    SDL_Rect recsrc;
    recsrc.x = 50;
    recsrc.y = 50;
    recsrc.w = 50;
    recsrc.h = 50;
		SDL_BlitSurface(image,NULL,gMainWindow,&(recsrc)); //画像のサーフェイスをメインウィンドウのサーフェイスにコピー
		SDL_FreeSurface(image); //解放
	  SDL_Flip(gMainWindow); //更新
}

void DrawScissors(){
	  SDL_Surface *image;
    image = IMG_Load("janken_choki.png"); //画像読み込み
    SDL_Rect recsrc;
    recsrc.x = 50;
    recsrc.y = 50;
    recsrc.w = 50;
    recsrc.h = 50;
		SDL_BlitSurface(image,NULL,gMainWindow,&(recsrc)); //画像のサーフェイスをメインウィンドウのサーフェイスにコピー
		SDL_FreeSurface(image); //解放
	  SDL_Flip(gMainWindow); //更新
}

void DrawPaper(){

	  SDL_Surface *image;
		image = IMG_Load("janken_pa.png"); //画像読み込み
    SDL_Rect recsrc;
    recsrc.x = 50;
    recsrc.y = 50;
    recsrc.w = 50;
    recsrc.h = 50;
		SDL_BlitSurface(image,NULL,gMainWindow,&(recsrc)); //画像のサーフェイスをメインウィンドウのサーフェイスにコピー
		SDL_FreeSurface(image); //解放
	  SDL_Flip(gMainWindow); //更新
}



/*****
static
*****/
/*****************************************************************
関数名	: CheckButtonNO
機能	: クリックされたボタンの番号を返す
引数	: int	   x		: マウスの押された x 座標
		  int	   y		: マウスの押された y 座標
		  char	   num		: 全クライアント数
出力	: 押されたボタンの番号を返す
		  ボタンが押されていない時は-1を返す
*****************************************************************/
static int CheckButtonNO(int x,int y,int num)
{
	int i;

 	for(i=0;i<num+2;i++){
		if(gButtonRect[i].x < x &&
			gButtonRect[i].y < y &&
      		gButtonRect[i].x + gButtonRect[i].w > x &&
			gButtonRect[i].y + gButtonRect[i].h > y){
			return i;
		}
	}
 	return -1;
}
