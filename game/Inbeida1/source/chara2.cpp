#include "headers.h"
subWeapon subA,subB;
//------------------------------------------------------------------------------ƒtƒ‰ƒO‰º‚°‚é
void sub_reset()
{
int lop;
for(lop=0; lop<subWmax; lop++)
	{
	subA.flg[lop]=0;	subB.flg[lop]=0;
	}
}
//------------------------------------------------------------------------------ƒtƒ‰ƒO‰º‚°‚é
void sub_init()
{
int lop;
if(keys.z==1	||	joys.z==1)
for(lop=0; lop<subWmax; lop++)
if(mes.pow>=2)
if(subA.flg[lop]==0)
	{
	subA.flg[lop]=1;
	if(subA.vec==0){subA.vec=1;	subA.posX[lop]=mes.posX-4;}//’e‚Ì‚wÀ•W
	else					 {subA.vec=0;	subA.posX[lop]=mes.posX+24;}
	subA.posY[lop]=mes.posY+8;
	break;
	}
if(keys.z==1	||	joys.z==1)
for(lop=0; lop<subWmax; lop++)
if(mes.pow>=3)
if(subB.flg[lop]==0)
	{
	subB.flg[lop]=1;
	subB.posX[lop]=mes.posX+4;//’e‚Ì‚wÀ•W
	subB.posY[lop]=mes.posY+12;
	if(subB.vec==0){subB.vec=1;	subB.spd[lop]=4;}
	else					 {subB.vec=0;	subB.spd[lop]=-4;}
	break;
	}
}
//------------------------------------------------------------------------------ƒƒCƒ“ˆ—
void sub_main()
{
sub_init();
//”­Ë‚³‚ê‚Ä‚é’e‚ğˆÚ“®ŒvZ
int lop;
for(lop=0;		lop<subA.num;		lop++)
if(subA.flg[lop]==1)
	{
	subA.posY[lop]-=4;
	subA_src(subA.posX[lop],subA.posY[lop]);//’e•`‰æ
	if(subA.posY[lop]<0)subA.flg[lop]=0;//ƒtƒ‰ƒO‰º‚°‚é
	}
for(lop=0;		lop<subB.num;		lop++)
if(subB.flg[lop]==1)
	{
	subB.posX[lop]+=subB.spd[lop];
	if(lop<=1)subB.posY[lop]-=1;
	else subB.posY[lop]-=2;
	subB_src(subB.posX[lop],subB.posY[lop]);//’e•`‰æ
	if(subB.posX[lop]<-8	||	subB.posX[lop]>300)subB.flg[lop]=0;//ƒtƒ‰ƒO‰º‚°‚é
	}
}
//------------------------------------------------------------------------------’eŠG‚`
void subA_src(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//•`‰æ‹éŒ`—p

rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*0;//‹éŒ`ˆêŒÂ–Ú
rct.w=KUKEI*1; rct.h=KUKEI*1;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,0));

rct.x=posX+1; rct.y=posY+KUKEI*0;//‹éŒ`“ñŒÂ–Ú
rct.w=KUKEI/2; rct.h=KUKEI*3;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));
}
//------------------------------------------------------------------------------’eŠG‚a
void subB_src(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//•`‰æ‹éŒ`—p

rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*0;//‹éŒ`ˆêŒÂ–Ú
rct.w=KUKEI*3; rct.h=KUKEI/2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,0));
}