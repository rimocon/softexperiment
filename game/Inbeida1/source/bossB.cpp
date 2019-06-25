#include "headers.h"
#define ZR 45
bossB bsB;
bossC bsC;
int lop;
//------------------------------------------------------------------------------boss初期化
void bosB_init()
{
for(lop=0;	lop<BB_PART;	lop++)
	{
	bsB.fg[lop]=1;	bsB.bt[lop]=rand()%100+30;
	}
bsB.pX[0]=140;	bsB.pY[0]=800;
bsB.ptn=rand()%2;

bsA.rt[0]=360;	bsA.rt[1]=0;
bsA.zom=1;	bsA.ani=255;

bs.num+=1;
bs.flg=1;
otr.eNum-=1;//敵の数
snd.flg=1;//BGM変える
}
//------------------------------------------------------------------------------bossのbit移動計算
void bosB_bit()
{
if(bsB.bit==0)
	{
	bsB.pX[1]=bsB.pX[0]-ZR+bsB.mov;	bsB.pY[1]=bsB.pY[0]-ZR;
	bsB.pX[2]=bsB.pX[0]+ZR;					bsB.pY[2]=bsB.pY[0]-ZR+bsB.mov;
	bsB.pX[3]=bsB.pX[0]-ZR;					bsB.pY[3]=bsB.pY[0]+ZR-bsB.mov;
	bsB.pX[4]=bsB.pX[0]+ZR-bsB.mov;	bsB.pY[4]=bsB.pY[0]+ZR;
	}
else if(bsB.bit==1)
	{
	bsB.pX[1]=bsB.pX[0]+ZR;					bsB.pY[1]=bsB.pY[0]-ZR+bsB.mov;
	bsB.pX[2]=bsB.pX[0]+ZR-bsB.mov;	bsB.pY[2]=bsB.pY[0]+ZR;
	bsB.pX[3]=bsB.pX[0]-ZR+bsB.mov;	bsB.pY[3]=bsB.pY[0]-ZR;
	bsB.pX[4]=bsB.pX[0]-ZR;					bsB.pY[4]=bsB.pY[0]+ZR-bsB.mov;
	}
else if(bsB.bit==2)
	{
	bsB.pX[1]=bsB.pX[0]+ZR-bsB.mov;	bsB.pY[1]=bsB.pY[0]+ZR;
	bsB.pX[2]=bsB.pX[0]-ZR;					bsB.pY[2]=bsB.pY[0]+ZR-bsB.mov;
	bsB.pX[3]=bsB.pX[0]+ZR;					bsB.pY[3]=bsB.pY[0]-ZR+bsB.mov;
	bsB.pX[4]=bsB.pX[0]-ZR+bsB.mov;	bsB.pY[4]=bsB.pY[0]-ZR;
	}
else if(bsB.bit==3)
	{
	bsB.pX[1]=bsB.pX[0]-ZR;					bsB.pY[1]=bsB.pY[0]+ZR-bsB.mov;
	bsB.pX[2]=bsB.pX[0]-ZR+bsB.mov;	bsB.pY[2]=bsB.pY[0]-ZR;
	bsB.pX[3]=bsB.pX[0]+ZR-bsB.mov;	bsB.pY[3]=bsB.pY[0]+ZR;
	bsB.pX[4]=bsB.pX[0]+ZR;					bsB.pY[4]=bsB.pY[0]-ZR+bsB.mov;
	}
if(bsB.mov<90)bsB.mov+=3;
else
	{
	bsB.mov=0;
	if(bsB.bit<3)bsB.bit+=1;	else bsB.bit=0;
	}
}
//------------------------------------------------------------------------------bossのbit移動計算
void bosB_mov()
{
bos_srcA(bsB.pX[0],			bsB.pY[0],	45,					bsA.zom+1);
bos_srcA(bsB.pX[0]-20,	bsB.pY[0],	bsA.rt[0],	2);
bos_srcA(bsB.pX[0]+20,	bsB.pY[0],	bsA.rt[1],	2);

if(bsB.fg[1]>0)bos_srcA(bsB.pX[1],bsB.pY[1],bsA.rt[0],bsA.zom); else bos_srcA(bsB.pX[1],bsB.pY[1],11,0.9);
if(bsB.fg[2]>0)bos_srcA(bsB.pX[2],bsB.pY[2],bsA.rt[1],bsA.zom); else bos_srcA(bsB.pX[2],bsB.pY[2],23,0.7);
if(bsB.fg[3]>0)bos_srcA(bsB.pX[3],bsB.pY[3],bsA.rt[1],bsA.zom); else bos_srcA(bsB.pX[3],bsB.pY[3],48,0.8);
if(bsB.fg[4]>0)bos_srcA(bsB.pX[4],bsB.pY[4],bsA.rt[0],bsA.zom); else bos_srcA(bsB.pX[4],bsB.pY[4],93,0.7);

if(bs.num>13)	ret_src(0,bsB.pX[0],bsB.pY[0],bsA.ani);
else					ene_src(0,bsB.pX[0],bsB.pY[0],bsA.ani);
//方向
if(bsA.vec==0)
	{
	if(bsA.zom>0.1)bsA.zom-=0.05;	else {bsA.vec=1;	bsA.zom=0.1;}
	}
else if(bsA.vec==1)
	{
	if(bsA.zom<1.7)bsA.zom+=0.05;	else {bsA.vec=0;	bsA.zom=1.7;}
	}
//角度
if(bsA.rt[0]>6  )bsA.rt[0]-=6;	else bsA.rt[0]=360;
if(bsA.rt[1]<354)bsA.rt[1]+=6;	else bsA.rt[1]=0;
//アニメ
if(bsA.ani>0)bsA.ani-=5;	else bsA.ani=255;
bosB_bit();
}
//------------------------------------------------------------------------------bossのbit移動計算
void bosB_main()
{
if(bsB.fg[0]==1)
	{
	if(bsB.pY[0]>80)bsB.pY[0]-=1;	else bsB.fg[0]=2;
	}
else if(bsB.fg[0]==500)//退場
	{
	if(bsB.pY[0]>-500)bsB.pY[0]-=1;
	else
		{
		bsB.fg[0]=-1;	bs.flg=0;
		}
	}
else if(bsB.fg[0]>1)//メイン処理
	{
	//左右移動
	if(bsB.ptn==0)
		{
		if(bsB.pX[0]<40)	bsB.ptn=1;	else bsB.pX[0]-=2;
		}
	else if(bsB.ptn==1)
		{
		if(bsB.pX[0]>240)	bsB.ptn=0;	else bsB.pX[0]+=2;
		}
	//ループ出来るヤツ
	for(lop=0;	lop<BB_PART;	lop++)
	if(bsB.fg[lop]>0)
		{
		//弾発射
		if(bsB.bt[lop]>0)bsB.bt[lop]-=1;
		else
			{
			blt_init(bsB.pX[lop],	bsB.pY[lop]);
			bsB.bt[lop]=rand()%100+30;
			}
		//当たり判定
		if(lop==0)
			{
			if(bsB.fg[lop]>90)
				{
				bsB.fg[lop]=500;
				sco.zou+=200;
				playSE(5,0);
				snd.flg=1;
				}
			else if(hitChek_Bos(bsB.pX[lop]-5,bsB.pY[lop]-5,30,30,lop+5)==1)bsB.fg[lop]+=1;
			}
		else
			{
			if(hitChek_Bos(bsB.pX[lop],bsB.pY[lop],20,20,lop+5)==1)
				{
				bsB.fg[lop]+=1;
				if(bsB.fg[0]<60)	bsB.fg[0]+=1;
				}
			else if(bsB.fg[lop]>15)bsB.fg[lop]=0;
			}
		}
	}
bosB_mov();//描画
}
//------------------------------------------------------------------------------boss初期化
void bosC_init()
{
for(lop=0;	lop<BC_PART;	lop++)
	{
	bsC.fg[lop]=1;	bsC.bt[lop]=rand()%110+30;
	}
bsC.pX[0]=140;	bsC.pY[0]=800;

bsA.rt[0]=360;	bsA.rt[1]=0;
bsA.zom=1;
bsA.ani=255;
bsA.vec=0;

bsB.bit=0;
bsB.mov=0;
bsB.ptn=rand()%2;

bs.num+=1;
bs.flg=1;
otr.eNum-=1;//敵の数
snd.flg=1;//BGM変える
}
//------------------------------------------------------------------------------bossのbit移動計算
void bosC_bit()
{
Sint16 mov[2];
if(bsC.fg[1]+bsC.fg[2]+bsC.fg[3]+bsC.fg[4]==0)mov[0]=0; else mov[0]=bsB.mov;
if(bsC.fg[5]+bsC.fg[6]+bsC.fg[7]+bsC.fg[8]==0)mov[1]=0; else mov[1]=bsB.mov;

if(bsB.bit==0)
	{
	bsC.pX[1]=bsC.pX[0]-80+mov[0];	bsC.pY[1]=bsC.pY[0]+00;
	bsC.pX[2]=bsC.pX[0]-40;					bsC.pY[2]=bsC.pY[0]+00+mov[0];
	bsC.pX[3]=bsC.pX[0]-40-mov[0];	bsC.pY[3]=bsC.pY[0]+40;
	bsC.pX[4]=bsC.pX[0]-80;					bsC.pY[4]=bsC.pY[0]+40-mov[0];
	
	bsC.pX[5]=bsC.pX[0]+40;					bsC.pY[5]=bsC.pY[0]+00+mov[1];
	bsC.pX[6]=bsC.pX[0]+80-mov[1];	bsC.pY[6]=bsC.pY[0]+00;
	bsC.pX[7]=bsC.pX[0]+80;					bsC.pY[7]=bsC.pY[0]+40-mov[1];
	bsC.pX[8]=bsC.pX[0]+40+mov[1];	bsC.pY[8]=bsC.pY[0]+40;
	}
else if(bsB.bit==1)
	{
	bsC.pX[1]=bsC.pX[0]-40;					bsC.pY[1]=bsC.pY[0]+00+mov[0];
	bsC.pX[2]=bsC.pX[0]-40-mov[0];	bsC.pY[2]=bsC.pY[0]+40;
	bsC.pX[3]=bsC.pX[0]-80;					bsC.pY[3]=bsC.pY[0]+40-mov[0];
	bsC.pX[4]=bsC.pX[0]-80+mov[0];	bsC.pY[4]=bsC.pY[0]+00;
	
	bsC.pX[5]=bsC.pX[0]+40+mov[1];	bsC.pY[5]=bsC.pY[0]+40;
	bsC.pX[6]=bsC.pX[0]+40;					bsC.pY[6]=bsC.pY[0]+00+mov[1];
	bsC.pX[7]=bsC.pX[0]+80-mov[1];	bsC.pY[7]=bsC.pY[0]+00;
	bsC.pX[8]=bsC.pX[0]+80;					bsC.pY[8]=bsC.pY[0]+40-mov[1];
	}
else if(bsB.bit==2)
	{
	bsC.pX[1]=bsC.pX[0]-40-mov[0];	bsC.pY[1]=bsC.pY[0]+40;
	bsC.pX[2]=bsC.pX[0]-80;					bsC.pY[2]=bsC.pY[0]+40-mov[0];
	bsC.pX[3]=bsC.pX[0]-80+mov[0];	bsC.pY[3]=bsC.pY[0]+00;
	bsC.pX[4]=bsC.pX[0]-40;					bsC.pY[4]=bsC.pY[0]+00+mov[0];

	bsC.pX[5]=bsC.pX[0]+80;					bsC.pY[5]=bsC.pY[0]+40-mov[1];
	bsC.pX[6]=bsC.pX[0]+40+mov[1];	bsC.pY[6]=bsC.pY[0]+40;
	bsC.pX[7]=bsC.pX[0]+40;					bsC.pY[7]=bsC.pY[0]+00+mov[1];
	bsC.pX[8]=bsC.pX[0]+80-mov[1];	bsC.pY[8]=bsC.pY[0]+00;
	}
else if(bsB.bit==3)
	{
	bsC.pX[1]=bsC.pX[0]-80;					bsC.pY[1]=bsC.pY[0]+40-mov[0];
	bsC.pX[2]=bsC.pX[0]-80+mov[0];	bsC.pY[2]=bsC.pY[0]+00;
	bsC.pX[3]=bsC.pX[0]-40;					bsC.pY[3]=bsC.pY[0]+00+mov[0];
	bsC.pX[4]=bsC.pX[0]-40-mov[0];	bsC.pY[4]=bsC.pY[0]+40;
	
	bsC.pX[5]=bsC.pX[0]+80-mov[1];	bsC.pY[5]=bsC.pY[0]+00;
	bsC.pX[6]=bsC.pX[0]+80;					bsC.pY[6]=bsC.pY[0]+40-mov[1];
	bsC.pX[7]=bsC.pX[0]+40+mov[1];	bsC.pY[7]=bsC.pY[0]+40;
	bsC.pX[8]=bsC.pX[0]+40;					bsC.pY[8]=bsC.pY[0]+00+mov[1];
	}
bsC.pX[9] =bsC.pX[0]-30;	bsC.pY[9] =bsC.pY[0]-20;
bsC.pX[10]=bsC.pX[0]+30;	bsC.pY[10]=bsC.pY[0]-20;
bsC.pX[11]=bsC.pX[0]+20;	bsC.pY[11]=bsC.pY[0]+20;
bsC.pX[12]=bsC.pX[0]-20;	bsC.pY[12]=bsC.pY[0]+20;

//移動
if(bsB.mov<40)bsB.mov+=1;
else
	{
	bsB.mov=0;
	if(bsB.bit<3)bsB.bit+=1;	else bsB.bit=0;
	}
//方向
if(bsA.vec==0)
	{
	if(bsA.zom>0.5)bsA.zom-=0.025;	else {bsA.vec=1;	bsA.zom=0.5;}
	}
else if(bsA.vec==1)
	{
	if(bsA.zom<1.5)bsA.zom+=0.025;	else {bsA.vec=0;	bsA.zom=1.5;}
	}
//角度
if(bsA.rt[0]>6  )bsA.rt[0]-=6;	else bsA.rt[0]=360;
if(bsA.rt[1]<354)bsA.rt[1]+=6;	else bsA.rt[1]=0;
//アニメ
if(bsA.ani>0)bsA.ani-=5;	else bsA.ani=255;
}
//------------------------------------------------------------------------------bossのbit移動計算
void bosC_mov()
{
bos_srcA( bsC.pX[0],bsC.pY[0],45,bsA.zom+1);
if(bs.num>13)	ret_src(0,bsC.pX[0],bsC.pY[0],bsA.ani);
else					ene_src(0,bsC.pX[0],bsC.pY[0],bsA.ani);

if(bsC.fg[1]+bsC.fg[2]+bsC.fg[3]+bsC.fg[4]==0)
	{
	bos_srcA( bsC.pX[0]-35,bsC.pY[0]+5,15,1);
	bos_srcA(bsC.pX[0]-60,bsC.pY[0]+20,40,1);
	}
else
	{
	bos_srcA( bsC.pX[0]-35,bsC.pY[0]+5,bsA.rt[0],bsA.zom+0.5);
	bos_srcA(bsC.pX[0]-60,bsC.pY[0]+20,bsA.rt[1],1.5);
	}
if(bsC.fg[5]+bsC.fg[6]+bsC.fg[7]+bsC.fg[8]==0)
	{
	bos_srcA( bsC.pX[0]+35,bsC.pY[0]+5,25,1);
	bos_srcA(bsC.pX[0]+60,bsC.pY[0]+20,70,1);
	}
else
	{
	bos_srcA( bsC.pX[0]+35,bsC.pY[0]+5,bsA.rt[1],bsA.zom+0.5);
	bos_srcA(bsC.pX[0]+60,bsC.pY[0]+20,bsA.rt[0],1.5);
	}

if(bsC.fg[1]>0)bos_srcA(bsC.pX[1],	bsC.pY[1],	45,bsA.zom-0.5);	else bos_srcA(bsC.pX[1],	bsC.pY[1],	50,0.7);
if(bsC.fg[2]>0)bos_srcA(bsC.pX[2],	bsC.pY[2],	45,bsA.zom-0.5);	else bos_srcA(bsC.pX[2],	bsC.pY[2],	55,0.7);
if(bsC.fg[3]>0)bos_srcA(bsC.pX[3],	bsC.pY[3],	45,bsA.zom-0.5);	else bos_srcA(bsC.pX[3],	bsC.pY[3],	60,0.7);
if(bsC.fg[4]>0)bos_srcA(bsC.pX[4],	bsC.pY[4],	45,bsA.zom-0.5);	else bos_srcA(bsC.pX[4],	bsC.pY[4],	65,0.7);

if(bsC.fg[5]>0)bos_srcA(bsC.pX[5],	bsC.pY[5],	45,bsA.zom-0.5);	else bos_srcA(bsC.pX[5],	bsC.pY[5],	70,0.7);
if(bsC.fg[6]>0)bos_srcA(bsC.pX[6],	bsC.pY[6],	45,bsA.zom-0.5);	else bos_srcA(bsC.pX[6],	bsC.pY[6],	75,0.7);
if(bsC.fg[7]>0)bos_srcA(bsC.pX[7],	bsC.pY[7],	45,bsA.zom-0.5);	else bos_srcA(bsC.pX[7],	bsC.pY[7],	80,0.7);
if(bsC.fg[8]>0)bos_srcA(bsC.pX[8],	bsC.pY[8],	45,bsA.zom-0.5);	else bos_srcA(bsC.pX[8],	bsC.pY[8],	85,0.7);

if(bsC.fg[9] >0)bos_srcA(bsC.pX[9],		bsC.pY[9],	bsA.rt[1],bsA.zom-0.5);	else bos_srcA(bsC.pX[9],	bsC.pY[9],	45,0.7);
if(bsC.fg[10]>0)bos_srcA(bsC.pX[10],	bsC.pY[10],	bsA.rt[0],bsA.zom-0.5);	else bos_srcA(bsC.pX[10],	bsC.pY[10],	40,0.7);
if(bsC.fg[11]>0)bos_srcA(bsC.pX[11],	bsC.pY[11],	bsA.rt[1],bsA.zom-0.5);	else bos_srcA(bsC.pX[11],	bsC.pY[11],	35,0.7);
if(bsC.fg[12]>0)bos_srcA(bsC.pX[12],	bsC.pY[12],	bsA.rt[0],bsA.zom-0.5);	else bos_srcA(bsC.pX[12],	bsC.pY[12],	30,0.7);
	
bosC_bit();
}
//------------------------------------------------------------------------------bossのbit移動計算
void bosC_main()
{
if(bsC.fg[0]==1)
	{
	if(bsC.pY[0]>50)bsC.pY[0]-=1;	else bsC.fg[0]=2;
	}
else if(bsC.fg[0]==500)//退場
	{
	if(bsC.pY[0]>-500)bsC.pY[0]-=1;
	else
		{
		bsC.fg[0]=-1;
		bs.flg=0;
		if(bs.num>21)bs.num=0;
		}
	}
else if(bsC.fg[0]>1)//メイン処理
	{
	//左右移動
	if(bsB.ptn==0)
		{
		if(bsC.pX[0]<80)	bsB.ptn=1;	else bsC.pX[0]-=2;
		}
	else if(bsB.ptn==1)
		{
		if(bsC.pX[0]>200)	bsB.ptn=0;	else bsC.pX[0]+=2;
		}
	//ループ出来るヤツ
	for(lop=0;	lop<BC_PART;	lop++)
	if(bsC.fg[lop]>0)
		{
		//弾発射
		if(bsC.bt[lop]>0)bsC.bt[lop]-=1;
		else
			{
			blt_init(bsC.pX[lop],	bsC.pY[lop]);
			bsC.bt[lop]=rand()%110+30;
			}
		//当たり判定
		if(lop==0)
			{
			if(bsC.fg[lop]>210)
				{
				bsC.fg[lop]=500;
				sco.zou+=300;
				playSE(5,0);
				snd.flg=1;
				}
			else if(hitChek_Bos(bsC.pX[lop]-5,bsC.pY[lop]-5,30,30,lop+10)==1)	bsC.fg[lop]+=1;
			}
		else
			{
			if(hitChek_Bos(bsC.pX[lop],bsC.pY[lop],20,20,lop+10)==1)
				{
				bsC.fg[lop]+=1;
				if(bsC.fg[0]<180)	bsC.fg[0]+=1;
				}
			else if(bsC.fg[lop]>15)bsC.fg[lop]=0;
			}
		}
	}
bosC_mov();//描画
}