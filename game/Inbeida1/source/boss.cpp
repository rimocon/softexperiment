#include "headers.h"
#define BOSS_PART 2
bosses bsA;//ボス用
bossVal bs;
SDL_Surface *bosA[BOSS_PART];//画像用
SDL_Surface *rzm;//変換用
SDL_Rect src,drw;
int lop;
Sint16 zmn,wid,hig;
//------------------------------------------------------------------------------画像読み込み
void imageLoadBoss()
{
bosA[0]=IMG_Load("data\\bosA0.dat");
bosA[1]=IMG_Load("data\\bosA1.dat");
for(lop=0; lop<BOSS_PART; lop++)
if(!bosA[lop]) SDL_Quit();
}
//------------------------------------------------------------------------------テスト表示
void bos_test()
{
drw.x=0;	drw.w=300;	drw.h=1;
drw.y=100;		SDL_FillRect(mWIN,&drw,SDL_MapRGB(mWIN->format,0,0,255));
drw.y=130;	SDL_FillRect(mWIN,&drw,SDL_MapRGB(mWIN->format,0,255,255));
drw.y=0;	drw.w=1;	drw.h=400;
drw.x=100;		SDL_FillRect(mWIN,&drw,SDL_MapRGB(mWIN->format,0,0,255));
drw.x=130;	SDL_FillRect(mWIN,&drw,SDL_MapRGB(mWIN->format,0,255,255));

bos_srcA( 90, 95,bsA.rt[1],0.5);
bos_srcA(100,100,bsA.rt[1],1);
bos_srcA(120,120,bsA.rt[1],2);
bos_srcA(160,150,bsA.rt[1],4);
bos_srcA(210,190,bsA.rt[1],6);
bos_srcA(270,240,bsA.rt[1],9);

if(bsA.rt[1]<354)bsA.rt[1]+=6;	else bsA.rt[1]=0;

ene_src(0,50,300,bsA.ani);
if(bsA.ani>0)bsA.ani-=5;	else bsA.ani=255;
}
//------------------------------------------------------------------------------表示
void bos_srcA(Sint16 posX,Sint16 posY,double rot,double zom)
{
if(bs.num<13)	rzm=rotozoomSurface(bosA[0],rot,zom,0);//画像変換
else					rzm=rotozoomSurface(bosA[1],rot,zom,0);
zmn=(Sint16)(20*zom-20)	;//座標修正
wid=(Sint16)(	(	(rzm->w-20*zom)+zmn	)/2+0.5	);
hig=(Sint16)(	(	(rzm->h-20*zom)+zmn	)/2+0.5	);

drw.x=posX-wid; drw.y=posY-hig;//座標決定
SDL_BlitSurface(rzm,NULL,mWIN,&drw);//描画
SDL_FreeSurface(rzm);//サーフェイス解放
}
//------------------------------------------------------------------------------リセット
void bos_reset()
{
bs.num=0;	bs.flg=0;
bsA.fg[0]=0;	bsB.fg[0]=0;		bsC.fg[0]=0;
}
//------------------------------------------------------------------------------初期化
void bosA_init()
{
bsA.fg[0]=1;	bsA.fg[1]=1;	bsA.fg[2]=1;
bsA.posX=110;	bsA.posY=800;
bsA.rt[0]=360;	bsA.rt[1]=0;
bsA.blt[0]=rand()%120+30;	bsA.blt[1]=rand()%90+30;	bsA.blt[2]=rand()%90+30;
bsA.zom=0;
bs.num+=1;
otr.eNum-=1;//敵の数
bs.flg=1;
snd.flg=1;//BGM変える
}
//------------------------------------------------------------------------------行動
void bosA_mov(Sint16 posX,Sint16 posY)
{
bos_srcA(posX+25,	posY,	bsA.rt[0],	bsA.zom);
bos_srcA(posX+35,	posY,	bsA.rt[1],	bsA.zom);
if(bsA.fg[1]>0)bos_srcA(posX+0,		posY,	bsA.rt[0],	1);else bos_srcA(posX+0,	posY,	30,	1);
if(bsA.fg[2]>0)bos_srcA(posX+60,	posY,	bsA.rt[1],	1);else bos_srcA(posX+60,	posY,	85,	1);
if(bs.num>13)	ret_src(0,posX+30,	posY,	bsA.ani);
else					ene_src(0,posX+30,	posY,	bsA.ani);

if(bsA.ani>0)bsA.ani-=5;	else bsA.ani=255;
if(bsA.vec==0)
	{
	if(bsA.zom<2)bsA.zom+=0.05;
	else	{	bsA.zom=2;	bsA.vec=1;	}
	}
else if(bsA.vec<50)bsA.vec++;
else
	{
	if(bsA.zom>0.5)bsA.zom-=0.05;
	else	{	bsA.zom=0.5;	bsA.vec=0;	}
	}
if(bsA.rt[0]>6  )bsA.rt[0]-=6;	else bsA.rt[0]=360;
if(bsA.rt[1]<354)bsA.rt[1]+=6;	else bsA.rt[1]=0;
}
//------------------------------------------------------------------------------メイン処理
void bosA_main()
{
if(bsA.fg[0]==1)
	{
	bosA_mov(bsA.posX,bsA.posY);
	if(bsA.posY>100)bsA.posY--;
	else {bsA.fg[0]=2; bsA.posY=rand()%2;}
	}
else if(bsA.fg[0]==100)//退場
	{
	bosA_mov(bsA.posX,bsA.posY);
	if(bsA.posY>-300)bsA.posY-=1;
	else//終了へ
		{
		bsA.fg[0]=-1;
		bs.flg=0;
		}
	}
else if(bsA.fg[0]>1)
	{
	bosA_mov(bsA.posX,100);
	
	//パーツ毎の処理-
	for(lop=2;lop>-1;lop--)
	if(bsA.fg[lop]>0)
		{
		//弾発射
		if(bsA.blt[lop]<0)
			{
			bsA.blt[lop]=rand()%90+30;
			if		 (lop==0)blt_init(bsA.posX+30,	100);
			else if(lop==1)blt_init(bsA.posX+00,	100);
			else if(lop==2)blt_init(bsA.posX+60,	100);
			}
		else bsA.blt[lop]-=1;
		//当たり判定
		if		 (lop==0 && hitChek_Bos(bsA.posX+25,	95,		30,30,2)==1) bsA.fg[0]+=1;
		else if(lop==1 && hitChek_Bos(bsA.posX+00,	100,	20,20,3)==1){if(bsA.fg[0]<30)bsA.fg[0]+=1;bsA.fg[1]+=1;}
		else if(lop==2 && hitChek_Bos(bsA.posX+60,	100,	20,20,4)==1){if(bsA.fg[0]<30)bsA.fg[0]+=1;bsA.fg[2]+=1;}
		}
	//終了へ//パーツ破壊チェック
	if(bsA.fg[0]>60){bsA.fg[0]=100;bsA.posY=100;sco.zou+=100;playSE(5,0);snd.flg=1;}//得点
	if(bsA.fg[1]>15) bsA.fg[1]=0;
	if(bsA.fg[2]>15) bsA.fg[2]=0;
	//左右移動
	if(bsA.posY==0)
		{
		if(bsA.posX<0)bsA.posY=1;		else bsA.posX-=2;
		}
	else if(bsA.posY==1)
		{
		if(bsA.posX>220)bsA.posY=0;	else bsA.posX+=2;
		}
	}
}
//------------------------------------------------------------------------------メイン処理
void bos_main()
{
if(zyouken()==1)
	{
	if		 (bs.num==3	||	bs.num==15)	bosA_init();
	else if(bs.num==7	||	bs.num==19)	bosB_init();
	else if(bs.num==11||	bs.num==23)	bosC_init();
	}
else
	{
	if		 (bsA.fg[0]>0)bosA_main();
	else if(bsB.fg[0]>0)bosB_main();
	else if(bsC.fg[0]>0)bosC_main();
	}
}
//------------------------------------------------------------------------------自弾と敵機の当たり判定
int hitChek_Bos(Sint16 posX,Sint16 posY,Uint16 wid,Uint16 hig,Sint16 typ)
{
SDL_Rect R1,R2;
//敵の座標と大きさ設定
R1.x=posX;	R1.y=posY; R1.w=wid; R1.h=hig;

int blt;//表示中の自弾を探す
for(blt=0;blt<bltMax;blt++)
if(mes.flgB[blt]==1)
	{
	//自弾の座標と大きさ設定
	if(mes.pow>6)	{R2.x=mes.bltX[blt];	R2.y=mes.bltY[blt]; R2.w=10;	R2.h=24;}
	else					{R2.x=mes.bltX[blt];	R2.y=mes.bltY[blt]; R2.w=2;		R2.h=24;}
	//当り判定
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		mes.flgB[blt]=0;//自弾フラグ
		playSE(6,0);//効果音
		efect_init2(posX-15,posY-15,typ);//爆発エフェクト初期化
		return 1;
		}
	}
for(blt=0;blt<subWmax;blt++)
if(subA.flg[blt]==1)
	{
	//自弾の座標と大きさ設定
	R2.x=subA.posX[blt]; R2.y=subA.posY[blt]; R2.w=4;	R2.h=12;
	//当り判定
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		subA.flg[blt]=0;//自弾フラグ
		playSE(6,0);//効果音
		efect_init2(posX-15,posY-15,typ);//爆発エフェクト初期化
		return 1;
		}
	}
return 0;
}