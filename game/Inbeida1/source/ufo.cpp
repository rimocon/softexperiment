#include "headers.h"
enemys ufo;//UFO用
SDL_Surface *imgUfo;//画像用
Uint16 ufoStart;//UFO出現条件用
//------------------------------------------------------------------------------画像読み込み
void imageLoadUFO()
{
imgUfo=IMG_Load("data\\ufo.dat");
if(!imgUfo)	SDL_Quit();
}
//------------------------------------------------------------------------------画像読み込み
void ufo_reset()
{
ufo.flg=0; ufoStart=1;//UFO
}
//------------------------------------------------------------------------------画像表示用
void ufo_src(Sint16 posX,Sint16 posY)
{
SDL_Rect src,drw;//UFO描画用
src.x=0;

//アニメ
if(ufo.cntA<16)ufo.cntA++;
else ufo.cntA=0;
//アニメをＹ軸に反映
if(ufo.cntA>=0	&&	ufo.cntA<2)src.y=12*0;
else if(ufo.cntA>=2	&&	ufo.cntA<4)src.y=12*1;
else if(ufo.cntA>=4	&&	ufo.cntA<6)src.y=12*2;
else if(ufo.cntA>=6	&&	ufo.cntA<8)src.y=12*3;
else if(ufo.cntA>=8	&&	ufo.cntA<10)src.y=12*4;
else if(ufo.cntA>=10	&&	ufo.cntA<12)src.y=12*5;
else if(ufo.cntA>=12	&&	ufo.cntA<14)src.y=12*6;
else if(ufo.cntA>=14	&&	ufo.cntA<16)src.y=12*7;

src.w=40; src.h=12;//表示する画像の範囲決定
drw.x=posX; drw.y=posY;//表示する画像の範囲決定
SDL_BlitSurface(imgUfo,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------初期化
void ufo_init()
{
if(zyouken()==1 && otr.eNum>ufoStart)//登場条件
	{
	ufo.flg=1;//フラグ立てる
	ufo.posX=rand()%260;//登場座標決定
	ufo.posY=-300;
	ufo.cntA=0;//アニメ用のカウンタ
	ufo.cntB=0;//退散時間のカウンタリセット
	ufo.cntM=rand()%30;//移動用のカウンタ
	//移動スピード
	if(rand()%2==0)ufo.spd=1+rand()%5;
	else ufo.spd=-(1+rand()%5);
	snd.flg=1;//BGMフェードフラグ立てる
	ufoStart+=(rand()%2+2);
	}
else if(ufo.flg==1)//登場
	{
	if(ufo.posY<40)ufo.posY++;
	else ufo.flg=2;
	
	ufo_src(ufo.posX,ufo.posY);//描画
	}
}
//------------------------------------------------------------------------------自弾とUFOの当たり判定
void hitChek_ufo()
{
SDL_Rect R1,R2;
//UFOの座標と大きさ設定
R1.x=ufo.posX;	R1.y=ufo.posY; R1.w=40; R1.h=12;
	
int blt;//表示中の自弾を探す
for(blt=0;blt<bltMax;blt++)
if(mes.flgB[blt]==1)
	{
	//自弾の座標と大きさ設定
	R2.x=mes.bltX[blt]-1; R2.y=mes.bltY[blt]; R2.w=4;	R2.h=24;
	//当り判定、当たってたらフラグ加算
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		if(ufo.flg<8)ufo.flg++;//UFOフラグ加算
		else
			{
			otr.eNum-=1;//敵機の数減らす
			sco.zou+=100;//得点
			ufo.flg=-2;
			snd.flg=1;//BGM変える
			if(rand()%6==0){mes.pow+=2;playSE(3,1);}//自機パワーアップ
			else {mes.pow++;playSE(3,0);}
			}
		mes.flgB[blt]=0;//自弾フラグ
		playSE(1,0);//効果音
		//爆発エフェクトその２初期化
		efect_init2(ufo.posX,ufo.posY,1);
		break;
		}
	}
for(blt=0;blt<subWmax;blt++)
if(subA.flg[blt]==1)
	{
	//自弾の座標と大きさ設定
	R2.x=subA.posX[blt]; R2.y=subA.posY[blt]; R2.w=4;	R2.h=12;
	//当り判定、当たってたらフラグ加算
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		if(ufo.flg<8)ufo.flg++;//UFOフラグ加算
		else
			{
			otr.eNum-=1;//敵機の数減らす
			sco.zou+=50;//得点
			ufo.flg=-2;
			snd.flg=1;//BGM変える
			if(rand()%13==0){mes.pow+=2;playSE(3,1);}//自機パワーアップ
			else {mes.pow++;playSE(3,0);}
			}
		subA.flg[blt]=0;//自弾フラグ
		playSE(1,0);//効果音
		//爆発エフェクトその２初期化
		efect_init2(ufo.posX,ufo.posY,1);
		break;
		}
	}
}
//------------------------------------------------------------------------------メイン処理
void ufo_main()
{
ufo_init();//初期化
if(ufo.flg>=2)//メイン処理
	{
	//移動量計算
	static Sint16 upDwon=1;
	ufo.posX+=ufo.spd;
	ufo.posY+=upDwon;
	if(ufo.posY==20)upDwon=1;
	else if(ufo.posY==160)upDwon=-1;
	//Ｘ軸位置修正
	if(ufo.posX<-40)ufo.posX=300;
	else if(ufo.posX>300)ufo.posX=-40;
	//退散までのカウンタ
	if(ufo.cntB<2100)ufo.cntB++;
	else
		{
		ufo.flg=-1;
		snd.flg=1;//BGM変える
		}
	//移動と弾
	if(ufo.cntM<60)ufo.cntM++;
	else
		{
		blt_init(ufo.posX,ufo.posY);//弾初期化
		ufo.cntM=rand()%30;
		if(rand()%2==0)ufo.spd=1+rand()%5;
		else ufo.spd=-(1+rand()%5);
		}
	ufo_src(ufo.posX,ufo.posY);//描画
	hitChek_ufo();//当たり判定
	}
else if(ufo.flg<=-1)//退散
	{
	if(ufo.posY>-12)
		{
		ufo.posY--;
		ufo_src(ufo.posX,ufo.posY);//描画
		
		if(ufo.flg==-2)//破壊後は爆発エフェクト表示
		if(ufo.cntB<30)ufo.cntB++;
		else
			{
			//爆発エフェクトその２初期化
			efect_init2(ufo.posX,ufo.posY,1);
			ufo.cntB=0;
			}
		}
	else//UFO終了
		{
		ufo.cntB=0; ufo.flg=0;
		}
	}
}