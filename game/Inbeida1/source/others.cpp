#include "headers.h"
int games;
SDL_Surface *mWIN;
timers timer;
inputKeys keys,joys;
SDL_Surface *imgT;//画像用
SDL_Rect src,drw;//描画用
backStars starA,starB,starC;//バック地用
SDL_Joystick *Joy;
//------------------------------------------------------------------------------起動項目など
void startUp()
{
if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_AUDIO)==-1)	SDL_Quit();
mWIN=SDL_SetVideoMode(300,400,32,SDL_SWSURFACE	//|SDL_FULLSCREEN
);
SDL_WM_SetCaption("Inbeida",NULL);
games=-1;
imageLoad();//画像ロード
loadSounds();//音ロード
imageLoadUFO();//UFO読み込み
imageLoadScore();//得点画像ロード
imageLoadBoss();//boss画像
Joy=SDL_JoystickOpen(0);//ゲームコントローラ
hiS.iti=2;//VENIOのハイスコア
hiS.zyu=3;
hiS.hya=8;
hiS.sen=0;
hiS.man=2;
}
//------------------------------------------------------------------------------終了設定
int quits()
{
SDL_Event Qevnts;
if(SDL_PollEvent(&Qevnts))
	{
	switch(Qevnts.type)
		{
		case SDL_QUIT:
			return 0;
			break;
			
		case SDL_KEYDOWN:
			if(Qevnts.key.keysym.sym==SDLK_ESCAPE||Qevnts.key.keysym.sym==SDLK_q) return 0;
			break;
		
		default: break;
		}
	}
return 1;
}
//------------------------------------------------------------------------------FPS調整
void frames()
{
timer.now=SDL_GetTicks();//現在時間を取得
timer.wit=timer.now-timer.lev;//待ち時間を計算

if(timer.wit<16)//16以下ならCPUを休ませる
	{
	SDL_Delay(16-timer.wit);
	}
timer.lev=SDL_GetTicks();//経過時間を更新
SDL_UpdateRect(mWIN,0,0,0,0);//画面を更新
}
//------------------------------------------------------------------------------バック地
void backGraund()
{
if(games==-1)SDL_FillRect(mWIN,NULL,SDL_MapRGB(mWIN->format,0,0,0));
else if(games==0)SDL_FillRect(mWIN,NULL,SDL_MapRGB(mWIN->format,255,0,0));
else
	{
	bg_gra();//グラデ
	stars();//バック地の星
	}
}
//------------------------------------------------------------------------------画像読み込み
void imageLoad()
{
imgT=IMG_Load("data\\start_end.dat");
if(!imgT)	SDL_Quit();
}
//------------------------------------------------------------------------------画像表示_タイトル
void dr_Title(Sint16 posX,Sint16 posY)
{
src.x=0; src.y=0; src.w=100; src.h=50;
drw.x=posX; drw.y=posY;
SDL_BlitSurface(imgT,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------画像表示_ゲームオーバー
void dr_end(Sint16 posX,Sint16 posY)
{
src.x=0; src.y=50; src.w=100; src.h=50;
drw.x=posX; drw.y=posY;
SDL_BlitSurface(imgT,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------入力操作
void inputEngine()
{
Uint8 *getKeys=SDL_GetKeyState(NULL);
//-----------------------------------矢印キー確認
if(getKeys[SDLK_LEFT]==SDL_PRESSED)		keys.l=1; else keys.l=0;
if(getKeys[SDLK_RIGHT]==SDL_PRESSED)	keys.r=1; else keys.r=0;
if(getKeys[SDLK_UP]==SDL_PRESSED)			keys.u=1; else keys.u=0;
if(getKeys[SDLK_DOWN]==SDL_PRESSED)		keys.d=1; else keys.d=0;
//-----------------------------------Ｚ印キー確認
if(timer.now-keys.time>350)
	{
	if(getKeys[SDLK_z]==SDL_PRESSED)
		{
		keys.z=1;	 keys.time=timer.now;
		}
	}
else
	{
	keys.z=0;
	if(getKeys[SDLK_z]!=SDL_PRESSED) keys.time=0;
	}
//PS2のコントローラ
if(Joy)
	{
	SDL_JoystickUpdate();
	if(SDL_JoystickGetAxis(Joy,1)<=-256)		joys.u=1;		else joys.u=0;
	if(SDL_JoystickGetAxis(Joy,1)>=256)			joys.d=1;		else joys.d=0;
	if(SDL_JoystickGetAxis(Joy,0)<=-256)		joys.l=1;		else joys.l=0;
	if(SDL_JoystickGetAxis(Joy,0)>=256)			joys.r=1;		else joys.r=0;
	
	if(timer.now-joys.time>350)
		{
		if(SDL_JoystickGetButton(Joy,3)==SDL_PRESSED)
			{
			joys.z=1;	 joys.time=timer.now;
			}
		}
	else
		{
		joys.z=0;
		if(SDL_JoystickGetButton(Joy,3)!=SDL_PRESSED) joys.time=0;
		}
	}
}
//------------------------------------------------------------------------------ゲームスタート
void startGame()
{
if(keys.z==1	||	joys.z==1)
	{
	games=1;//Ｚキーが押されたらゲームへ
	resets();//色々リセット
	}
}
//------------------------------------------------------------------------------タイトル画面へ戻る
void backTitle()
{
if(timer.now-otr.time>1000*1.5)
if(keys.z==1	||	joys.z==1)
games=0;//Ｚキーが押されたらゲームへ
}
//------------------------------------------------------------------------------バック地の星の配置
void stars_reset()
{
int lop;
for(lop=0;lop<starMax;lop++)
	{
	starA.flg[lop]=1;
	starA.posX[lop]=rand()%300;
	starA.posY[lop]=rand()%400;
	
	starB.flg[lop]=1;
	starB.posX[lop]=rand()%300;
	starB.posY[lop]=rand()%400;
	
	starC.flg[lop]=1;
	starC.posX[lop]=rand()%300;
	starC.posY[lop]=rand()%400;
	}
}
//------------------------------------------------------------------------------バック地の星の描画
void stars()
{
SDL_Rect rc;//星用
if(bs.flg==0)
	{
	starA.spd=1;	starA.R=0;		starA.G=126;	starA.B=255;	rc.w=1;	rc.h=1;
	starB.spd=2;	starB.R=255;	starB.G=255;	starB.B=255;
	starC.spd=3;	starC.R=0;		starC.G=255;	starC.B=0;
	}
else
	{
	starA.spd=15;	starA.R=64;		starA.G=0;	starA.B=0;	rc.w=2;	rc.h=100;
	starB.spd=25;	starB.R=192;	starB.G=0;	starB.B=0;
	starC.spd=20;	starC.R=126;	starC.G=0;	starC.B=0;
	}
int lop;
for(lop=0;lop<starMax;lop++)
	{
	//青い星（以下同一）
	if(starA.flg[lop]==0)
		{
		starA.flg[lop]=1;
		starA.posX[lop]=rand()%300;
		starA.posY[lop]=-rand()%5;
		}
	else if(starA.flg[lop]==1)
		{
		starA.posY[lop]+=starA.spd;//移動量計算し代入
		rc.x=starA.posX[lop]; rc.y=starA.posY[lop];
		//描画
		SDL_FillRect(mWIN,&rc,SDL_MapRGB(mWIN->format,starA.R,starA.G,starA.B));
		//フラグ下げる
		if(starA.posY[lop]>(400-rc.h)+rc.h)starA.flg[lop]=0;
		}
	//白い星
	if(starB.flg[lop]==0)
		{
		starB.flg[lop]=1;
		starB.posX[lop]=rand()%300; starB.posY[lop]=-rand()%5;
		}
	else if(starB.flg[lop]==1)
		{
		starB.posY[lop]+=starB.spd;
		rc.x=starB.posX[lop]; rc.y=starB.posY[lop];
		SDL_FillRect(mWIN,&rc,SDL_MapRGB(mWIN->format,starB.R,starB.G,starB.B));
		if(starB.posY[lop]>(400-rc.h)+rc.h)starB.flg[lop]=0;
		}
	//緑の星
	if(starC.flg[lop]==0)
		{
		starC.flg[lop]=1;
		starC.posX[lop]=rand()%300; starC.posY[lop]=-rand()%5;
		}
	else if(starC.flg[lop]==1)
		{
		starC.posY[lop]+=starC.spd;
		rc.x=starC.posX[lop]; rc.y=starC.posY[lop];
		SDL_FillRect(mWIN,&rc,SDL_MapRGB(mWIN->format,starC.R,starC.G,starC.B));
		if(starC.posY[lop]>(400-rc.h)+rc.h)starC.flg[lop]=0;
		}
	}
}
//------------------------------------------------------------------------------バック地のグラデーション
void bg_gra()
{
Uint8 B=0;//色用

SDL_Rect gra;//矩形領域設定
gra.x=0; gra.y=0;
gra.w=300; gra.h=4;

int lop;
for(lop=0; lop<100; lop++)//描画
	{
	//塗り潰し
	if(bs.flg==0)	SDL_FillRect(mWIN,&gra,SDL_MapRGB(mWIN->format,0,0,B));
	else					SDL_FillRect(mWIN,&gra,SDL_MapRGB(mWIN->format,B,0,0));
	gra.y+=4;//Ｙ軸調整
	B++;//色数値増加
	}
}
//------------------------------------------------------------------------------イベントの条件
int zyouken()
{
if(otr.flgR==0	&&	ufo.flg==0	&&	bs.flg==0)return 1;
return 0;
}