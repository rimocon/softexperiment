#include "headers.h"
#define IDO 0.707f
#define RETSU 30
ziki mes;
enemys ene[eneMax],ret[eneMax];
otherVal otr;
tamas tama[eBltMax];
//------------------------------------------------------------------------------自機の表示元
void me_src(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//描画矩形用

rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*0;//矩形一個目
rct.w=KUKEI*1; rct.h=KUKEI*2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));

rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*2;//矩形二個目
rct.w=KUKEI*3; rct.h=KUKEI*2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));

rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*4;//矩形三個目
rct.w=KUKEI*5; rct.h=KUKEI*1;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));

rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*2;//矩形四個目
rct.w=KUKEI*1; rct.h=KUKEI*2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,0,0,255));
}
//------------------------------------------------------------------------------自機の移動
void me_move()
{
//移動計算
if(keys.u==1	||	joys.u==1)
	{
	if		 (keys.l==1	||	joys.l==1)	{mes.posX-=3*IDO;mes.posY-=3*IDO;}
	else if(keys.r==1	||	joys.r==1)	{mes.posX+=3*IDO;mes.posY-=3*IDO;}
	else mes.posY-=3;
	}
else if(keys.d==1	||	joys.d==1)
	{
	if		 (keys.l==1	||	joys.l==1)	{mes.posX-=3*IDO;mes.posY+=3*IDO;}
	else if(keys.r==1	||	joys.r==1)	{mes.posX+=3*IDO;mes.posY+=3*IDO;}
	else mes.posY+=3;
	}
else if(keys.l==1	||	joys.l==1)mes.posX-=3;
else if(keys.r==1	||	joys.r==1)mes.posX+=3;

if(mes.posX<=0)mes.posX=0;//画面からはみ出さないようにする
else if(mes.posX>=280)mes.posX=280;
if(mes.posY<250)mes.posY=250;
else if(mes.posY>370)mes.posY=370;
}
//------------------------------------------------------------------------------自弾の表示元
void me_srcB(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//描画矩形用
rct.x=posX; rct.y=posY;	rct.w=KUKEI/2; rct.h=KUKEI*4;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,0,255,0));
}
//------------------------------------------------------------------------------自弾の表示元
void me_srcB2(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//描画矩形用
rct.x=posX; rct.y=posY;	rct.w=KUKEI/2; rct.h=KUKEI*4;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,0,255,0));
rct.x=posX+8; rct.y=posY;	rct.w=KUKEI/2; rct.h=KUKEI*4;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,0,255,0));
}
//------------------------------------------------------------------------------自弾の移動
void me_bullet()
{
int lop;
//発射されてない弾を探し初期化
if(keys.z==1	||	joys.z==1)
for(lop=0;		lop<bltMax;		lop++)
if(mes.flgB[lop]==0)
	{
	mes.flgB[lop]=1;
	if(mes.pow>6)	mes.bltX[lop]=mes.posX+5;
	else					mes.bltX[lop]=mes.posX+9;
	mes.bltY[lop]=mes.posY;
	playSE(0,0);//効果音
	break;//一発発射して抜ける
	}
//発射されてる弾を移動計算
for(lop=0;		lop<bltMax;		lop++)
if(mes.flgB[lop]==1)
	{
	mes.bltY[lop]-=mes.spd;
	if(mes.pow>6)	me_srcB2(mes.bltX[lop],mes.bltY[lop]);//弾描画
	else					me_srcB(mes.bltX[lop],mes.bltY[lop]);
	if(mes.bltY[lop]<0)mes.flgB[lop]=0;//フラグ下げる
	}
}
//------------------------------------------------------------------------------自機のパワーアップ
void me_power()
{
if		 (mes.pow>7 ){mes.spd=15;	subA.num=4;	subB.num=4;	mes.pow=7;	sco.zou+=100;}
else if(mes.pow==7){mes.spd=15;	subA.num=4;	subB.num=4;}
else if(mes.pow==6){mes.spd=13;	subA.num=4;	subB.num=4;}
else if(mes.pow==5){mes.spd=13;	subA.num=4;	subB.num=3;}
else if(mes.pow==4){mes.spd=13;	subA.num=4;	subB.num=2;}
else if(mes.pow==3){mes.spd=13;	subA.num=2;	subB.num=2;}
else if(mes.pow==2){mes.spd=13;	subA.num=2;	subB.num=0;}
else if(mes.pow==1){mes.spd=13;	subA.num=0;	subB.num=0;}
else if(mes.pow==0){mes.spd=7;	subA.num=0;	subB.num=0;}
}
//------------------------------------------------------------------------------自機まとめ
void me_main()
{
me_power();//パワーアップ
me_move();//移動
me_src((Sint16)mes.posX+0.5,(Sint16)mes.posY+0.5);//自機描画
me_bullet();//弾
sub_main();//sub weapon
}
//------------------------------------------------------------------------------敵の表示元
void ene_src(Sint16 ani,Sint16 posX,Sint16 posY,Uint8 clrB)
{
SDL_Rect rct;//描画矩形用
if(ani<10)//一枚目
	{
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*0;//白1-1
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*0;//白1-2
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*1;//白1-3
	rct.w=KUKEI*5; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*3;//白1-4
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*2;//白1-5
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*4; rct.y=posY+KUKEI*3;//白1-6
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*1;//赤1-1
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*2;//赤1-2
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*2;//赤1-3
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	}
else //二枚目
	{
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*0;//白2-1
	rct.w=KUKEI*2; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*0;//白2-2
	rct.w=KUKEI*2; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*1;//白2-3
	rct.w=KUKEI*3; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*3;//白2-4
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*4; rct.y=posY+KUKEI*3;//白2-5
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*4;//白2-6
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*4;//白2-7
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*1;//赤2-1
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*2;//赤2-2
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*2;//赤2-3
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	}
}
//------------------------------------------------------------------------------敵増加の時間
void ene_zouka()
{
if(timer.now-otr.time>1000*13)//増加時間確認
if(otr.eNum<eneMax	&&	zyouken()==1)//敵の数を確認
	{
	otr.eNum+=1;//敵数増加
	otr.time=timer.now;//時間リセット
	
	lvs.iti=otr.eNum;//レベル用
	if		 (otr.eNum<10)lvs.zyu=0;
	else if(otr.eNum>=40)
		{
		lvs.iti-=40;	lvs.zyu=4;
		}
	else if(otr.eNum>=30)
		{
		lvs.iti-=30;	lvs.zyu=3;
		}
	else if(otr.eNum>=20)
		{
		lvs.iti-=20;	lvs.zyu=2;
		}
	else if(otr.eNum>=10)
		{
		lvs.iti-=10;	lvs.zyu=1;
		}
	}
}
//------------------------------------------------------------------------------敵初期化
void ene_init(int max,Sint16 posX,Sint16 posY)
{
int lop;
for(lop=0; lop<max; lop++)//敵の初期化
if(ene[lop].flg==0)
	{
	ene[lop].flg=1; //出現フラグ
	ene[lop].posX=posX; ene[lop].posY=posY;//初期座標
	ene[lop].cntA=0;//アニメカウンタ
	ene[lop].cntB=rand()%360;//弾カウンタ
	ene[lop].cntM=0;//移動用のカウンタ
	if(rand()%2==0)ene[lop].spd=rand()%3;//移動スピード
	else ene[lop].spd=-rand()%3;
	break;
	}
}
//------------------------------------------------------------------------------敵２の表示元
void ret_src(Sint16 ani,Sint16 posX,Sint16 posY,Uint8 clrB)
{
SDL_Rect rct;//描画矩形用
if(ani<10)//一枚目
	{
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*0;//白1-1
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*1;//白1-2
	rct.w=KUKEI*3; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*0;//赤1-1
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*4; rct.y=posY+KUKEI*0;//赤1-2
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*1;//赤1-3
	rct.w=KUKEI*1; rct.h=KUKEI*4;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	}
else //二枚目
	{
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*1;//赤1
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*1;//赤2
	rct.w=KUKEI*1; rct.h=KUKEI*4;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*4; rct.y=posY+KUKEI*1;//赤3
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*0;//白1
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*2;//白2
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*0;//白3
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	}
}
//------------------------------------------------------------------------------敵２初期化
void ret_init(int max,Sint16 posX,Sint16 posY)
{
int lop;
for(lop=0; lop<max; lop++)//敵の初期化
if(ret[lop].flg==0)
	{
	ret[lop].flg=1; //出現フラグ
	ret[lop].posX=posX; ret[lop].posY=posY;//初期座標
	ret[lop].cntA=0;//アニメカウンタ
	ret[lop].cntB=rand()%240;//弾カウンタ
	ret[lop].cntM=0;//移動用のカウンタ
	if(rand()%2==0)ret[lop].spd=rand()%4;//移動スピード
	else ret[lop].spd=-rand()%4;
	break;
	}
}
//------------------------------------------------------------------------------敵２の列リセット
void ret_reset()
{
otr.flgR=0;
otr.numX=0;
otr.numY=0;
otr.flgS=(sco.iti+sco.zyu*10+sco.hya*100+sco.sen*1000+sco.man*10000)+otr.retZ;
}
//------------------------------------------------------------------------------敵２の列出現
void ret_syutsu()
{
int lopX,lopY;
if(zyouken()==1)
	{
	if(sco.iti+sco.zyu*10+sco.hya*100+sco.sen*1000+sco.man*10000>otr.flgS)	otr.flgR=1;
	}
else if(otr.flgR==1)
	{
	bs.num+=1;//ボス登場用
	otr.eNum-=1;//敵機の数減らす
	otr.retT=0;	otr.flgR=2;
	}
else if(otr.flgR==2)
	{
	//徐々に表示
	int lop;
	for(lop=0; lop<otr.numX; lop++)
	ret_src(otr.retT,20+RETSU*lop,RETSU+RETSU*otr.numY,255);
	
	//表示されたモノ表示
	for(lopY=0; lopY<otr.numY; lopY++)
	for(lopX=0; lopX<9; lopX++)
	ret_src(otr.retT,20+RETSU*lopX,RETSU+RETSU*lopY,255);
	//アニメ＆表示時間計算
	if(otr.retT<15)otr.retT++;
	else
		{
		otr.retT=0; playSE(2,0);
		//座標修正
		if(otr.numX!=9)otr.numX++;
		else if(otr.numY!=otr.retM)
			{
			otr.numX=1;
			otr.numY++;
			}
		else//実際にポップさせてフラグ下げる
			{
			for(lopY=0; lopY<otr.numY+1; lopY++)
			for(lopX=0; lopX<otr.numX; lopX++)
			ret_init(eneMax,20+RETSU*lopX,RETSU+RETSU*lopY);
			//敵列変数リセット
			if(otr.retZ<800)otr.retZ+=100;
			ret_reset();	otr.flgR=3;
			if(otr.retM<4)otr.retM++;//増加
			else otr.retM=0;
			}
		}
	}
}
//------------------------------------------------------------------------------敵動きに関するやつ
void ene_move(Sint16 *posX,Sint16 *posY,Sint16 *spd,Sint16 dwnS,int lop,
							Sint16 cntA,Sint16 *cntB,Sint16 *cntM,Sint16 down,Sint16 typ)
{
//移動量計算
*posX+=*spd;
*posY+=dwnS;
//Ｘ軸位置修正
if		 (*posX<-20)*posX=300;
else if(*posX>300)*posX=-20;

//弾発射チェック
if(*cntB!=-1)
if(*cntB!=0) *cntB-=1;
else 
	{
	blt_init(*posX,*posY);//弾初期化
	*cntB=-1;
	}
//移動パターン計算
if(typ==1 ||
	 lop==2 ||(lop>=5 &&lop<=6 )||lop==9 ||(lop>=12&&lop<=14)||lop==17||(lop>=20&&lop<=23)||
	 lop==26||(lop>=29&&lop<=33)||lop==36||(lop>=39&&lop<=44))
if(*cntM<60) *cntM+=1;
else
	{
	*cntM=0;
	//移動スピード変更
	if(rand()%2==0)*spd=down;
	else *spd=-down;
	}
if(typ==0)ene_src(cntA,*posX,*posY,255);//敵を描画
else			ret_src(cntA,*posX,*posY,255);//敵を描画
}
//------------------------------------------------------------------------------敵まとめ
void ene_main()
{
ret_syutsu();//敵の列出現
ene_zouka();//敵増加チェック
ene_init(otr.eNum,rand()%280,-20);//初期化
int num=0;
int lop;
for(lop=0; lop<eneMax; lop++)//敵のメイン処理
	{
	if(otr.flgR==3)
	if(ret[lop].flg==0)
		{
		num++;	if(num==eneMax)otr.flgR=0;
		}
	else if(ret[lop].flg==1)
		{
		ene_move(	&ret[lop].posX, &ret[lop].posY,	&ret[lop].spd,	rand()%2,lop,
							 ret[lop].cntA, &ret[lop].cntB, &ret[lop].cntM,	rand()%4,1);
		//アニメーション計算
		if(ret[lop].cntA<20)ret[lop].cntA++;	else ret[lop].cntA=0;
		//当たり判定
		if( hitChek_meB(ret[lop].posX,ret[lop].posY,ret[lop].spd,1)==1)ret[lop].flg=0;//自弾と敵
		if(hitChek_eneM(ret[lop].posX,ret[lop].posY,ret[lop].spd,0)==1)ret[lop].flg=0;//自機と敵
		//一番下まで来たらフラグ下げる
		if(ret[lop].posY>400)ret[lop].flg=0;
		}
	
	if(ene[lop].flg==1)
		{
		ene_move(	&ene[lop].posX, &ene[lop].posY,	&ene[lop].spd,	rand()%4,lop,
							 ene[lop].cntA, &ene[lop].cntB, &ene[lop].cntM,	rand()%3+1,0);
		//アニメーション計算
		if(ene[lop].cntA<20)ene[lop].cntA++;	else ene[lop].cntA=0;
		//当たり判定
		if(hitChek_meB( ene[lop].posX,ene[lop].posY,ene[lop].spd,0)==1)ene[lop].flg=0;//自弾と敵
		if(hitChek_eneM(ene[lop].posX,ene[lop].posY,ene[lop].spd,0)==1)ene[lop].flg=0;//自機と敵
		//一番下まで来たらフラグ下げる
		if(ene[lop].posY>400)ene[lop].flg=0;
		}
	}
}
//------------------------------------------------------------------------------弾の表示元
void blt_src(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//描画矩形用

rct.x=posX+1; rct.y=posY+KUKEI*2; rct.w=KUKEI*1; rct.h=KUKEI/2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));

rct.x=posX+2; rct.y=posY; rct.w=KUKEI/2; rct.h=KUKEI*3;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,0));
}
//------------------------------------------------------------------------------弾の初期化
void blt_init(Sint16 posX,Sint16 posY)
{
int lop;
for(lop=0; lop<eBltMax; lop++)
if(tama[lop].flg==0)
	{
	tama[lop].flg=1;//フラグ立てる
	tama[lop].posX=posX+8; tama[lop].posY=posY;//座標代入
	tama[lop].spd=3+rand()%3;//スピード決める
	break;//ループ抜ける
	}
}
//------------------------------------------------------------------------------弾のまとめ
void blt_main()
{
int lop;
for(lop=0; lop<eBltMax; lop++)
if(tama[lop].flg==1)
	{
	//移動量計算
	tama[lop].posY+=tama[lop].spd;
	//弾を描画
	blt_src(tama[lop].posX,tama[lop].posY);
	//当たり判定
	hitChek_Blt(lop);
	//画面から出たら消去
	if(tama[lop].posY>400)tama[lop].flg=0;
	}
}
//------------------------------------------------------------------------------リセット
void resets()
{
srand( (unsigned)time( NULL ) );//乱数リセット
ufo_reset();//UFO
score_rest();//点数
BGM_reset();//BGM
Mix_Volume(0,VOL_SE-16);
sub_reset();//sub weapon
mes.posX=140; mes.posY=370;//自機の初期座標
mes.pow=0;	mes.spd=7;
int lop;
for(lop=0;		lop<bltMax;		lop++) mes.flgB[lop]=0;//弾フラグ下げる
for(lop=0;		lop<eneMax;		lop++)	{ene[lop].flg=0;ret[lop].flg=0;}//敵フラグ下げる
otr.eNum=1;//敵の最大数の初期値
otr.time=timer.now;//敵増加の為の時間リセット
otr.retZ=200;	ret_reset();	otr.retM=0;//敵列変数リセット
for(lop=0;		lop<eBltMax;		lop++) tama[lop].flg=0;//弾フラグ下げる
for(lop=0;		lop<efcMax;		lop++)//爆発エフェクト
	{
	eft.flg[lop]=0; eft2.flg[lop]=0;
	}
bos_reset();//ボス
stars_reset();//バック地の星を配置する
}
//------------------------------------------------------------------------------キャラのまとめ
void chara()
{
me_main();//自機
bos_main();//boss
ene_main();//敵
ufo_main();//UFO
blt_main();//弾
efect_main();//爆発エフェクト
efect_main2();
BGM_fade();//BGM
}