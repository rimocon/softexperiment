#include "headers.h"
#define IDO 0.707f
#define RETSU 30
ziki mes;
enemys ene[eneMax],ret[eneMax];
otherVal otr;
tamas tama[eBltMax];
//------------------------------------------------------------------------------���@�̕\����
void me_src(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//�`���`�p

rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*0;//��`���
rct.w=KUKEI*1; rct.h=KUKEI*2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));

rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*2;//��`���
rct.w=KUKEI*3; rct.h=KUKEI*2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));

rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*4;//��`�O��
rct.w=KUKEI*5; rct.h=KUKEI*1;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));

rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*2;//��`�l��
rct.w=KUKEI*1; rct.h=KUKEI*2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,0,0,255));
}
//------------------------------------------------------------------------------���@�̈ړ�
void me_move()
{
//�ړ��v�Z
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

if(mes.posX<=0)mes.posX=0;//��ʂ���͂ݏo���Ȃ��悤�ɂ���
else if(mes.posX>=280)mes.posX=280;
if(mes.posY<250)mes.posY=250;
else if(mes.posY>370)mes.posY=370;
}
//------------------------------------------------------------------------------���e�̕\����
void me_srcB(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//�`���`�p
rct.x=posX; rct.y=posY;	rct.w=KUKEI/2; rct.h=KUKEI*4;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,0,255,0));
}
//------------------------------------------------------------------------------���e�̕\����
void me_srcB2(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//�`���`�p
rct.x=posX; rct.y=posY;	rct.w=KUKEI/2; rct.h=KUKEI*4;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,0,255,0));
rct.x=posX+8; rct.y=posY;	rct.w=KUKEI/2; rct.h=KUKEI*4;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,0,255,0));
}
//------------------------------------------------------------------------------���e�̈ړ�
void me_bullet()
{
int lop;
//���˂���ĂȂ��e��T��������
if(keys.z==1	||	joys.z==1)
for(lop=0;		lop<bltMax;		lop++)
if(mes.flgB[lop]==0)
	{
	mes.flgB[lop]=1;
	if(mes.pow>6)	mes.bltX[lop]=mes.posX+5;
	else					mes.bltX[lop]=mes.posX+9;
	mes.bltY[lop]=mes.posY;
	playSE(0,0);//���ʉ�
	break;//�ꔭ���˂��Ĕ�����
	}
//���˂���Ă�e���ړ��v�Z
for(lop=0;		lop<bltMax;		lop++)
if(mes.flgB[lop]==1)
	{
	mes.bltY[lop]-=mes.spd;
	if(mes.pow>6)	me_srcB2(mes.bltX[lop],mes.bltY[lop]);//�e�`��
	else					me_srcB(mes.bltX[lop],mes.bltY[lop]);
	if(mes.bltY[lop]<0)mes.flgB[lop]=0;//�t���O������
	}
}
//------------------------------------------------------------------------------���@�̃p���[�A�b�v
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
//------------------------------------------------------------------------------���@�܂Ƃ�
void me_main()
{
me_power();//�p���[�A�b�v
me_move();//�ړ�
me_src((Sint16)mes.posX+0.5,(Sint16)mes.posY+0.5);//���@�`��
me_bullet();//�e
sub_main();//sub weapon
}
//------------------------------------------------------------------------------�G�̕\����
void ene_src(Sint16 ani,Sint16 posX,Sint16 posY,Uint8 clrB)
{
SDL_Rect rct;//�`���`�p
if(ani<10)//�ꖇ��
	{
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*0;//��1-1
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*0;//��1-2
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*1;//��1-3
	rct.w=KUKEI*5; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*3;//��1-4
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*2;//��1-5
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*4; rct.y=posY+KUKEI*3;//��1-6
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*1;//��1-1
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*2;//��1-2
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*2;//��1-3
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	}
else //�񖇖�
	{
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*0;//��2-1
	rct.w=KUKEI*2; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*0;//��2-2
	rct.w=KUKEI*2; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*1;//��2-3
	rct.w=KUKEI*3; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*3;//��2-4
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*4; rct.y=posY+KUKEI*3;//��2-5
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*4;//��2-6
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*4;//��2-7
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*1;//��2-1
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*2;//��2-2
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*2;//��2-3
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	}
}
//------------------------------------------------------------------------------�G�����̎���
void ene_zouka()
{
if(timer.now-otr.time>1000*13)//�������Ԋm�F
if(otr.eNum<eneMax	&&	zyouken()==1)//�G�̐����m�F
	{
	otr.eNum+=1;//�G������
	otr.time=timer.now;//���ԃ��Z�b�g
	
	lvs.iti=otr.eNum;//���x���p
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
//------------------------------------------------------------------------------�G������
void ene_init(int max,Sint16 posX,Sint16 posY)
{
int lop;
for(lop=0; lop<max; lop++)//�G�̏�����
if(ene[lop].flg==0)
	{
	ene[lop].flg=1; //�o���t���O
	ene[lop].posX=posX; ene[lop].posY=posY;//�������W
	ene[lop].cntA=0;//�A�j���J�E���^
	ene[lop].cntB=rand()%360;//�e�J�E���^
	ene[lop].cntM=0;//�ړ��p�̃J�E���^
	if(rand()%2==0)ene[lop].spd=rand()%3;//�ړ��X�s�[�h
	else ene[lop].spd=-rand()%3;
	break;
	}
}
//------------------------------------------------------------------------------�G�Q�̕\����
void ret_src(Sint16 ani,Sint16 posX,Sint16 posY,Uint8 clrB)
{
SDL_Rect rct;//�`���`�p
if(ani<10)//�ꖇ��
	{
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*0;//��1-1
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*1;//��1-2
	rct.w=KUKEI*3; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*0;//��1-1
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*4; rct.y=posY+KUKEI*0;//��1-2
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*1;//��1-3
	rct.w=KUKEI*1; rct.h=KUKEI*4;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	}
else //�񖇖�
	{
	rct.x=posX+KUKEI*0; rct.y=posY+KUKEI*1;//��1
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*1;//��2
	rct.w=KUKEI*1; rct.h=KUKEI*4;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*4; rct.y=posY+KUKEI*1;//��3
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,0,0));
	
	rct.x=posX+KUKEI*1; rct.y=posY+KUKEI*0;//��1
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*2; rct.y=posY+KUKEI*2;//��2
	rct.w=KUKEI*1; rct.h=KUKEI*1;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	
	rct.x=posX+KUKEI*3; rct.y=posY+KUKEI*0;//��3
	rct.w=KUKEI*1; rct.h=KUKEI*2;
	SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,clrB,clrB,clrB));
	}
}
//------------------------------------------------------------------------------�G�Q������
void ret_init(int max,Sint16 posX,Sint16 posY)
{
int lop;
for(lop=0; lop<max; lop++)//�G�̏�����
if(ret[lop].flg==0)
	{
	ret[lop].flg=1; //�o���t���O
	ret[lop].posX=posX; ret[lop].posY=posY;//�������W
	ret[lop].cntA=0;//�A�j���J�E���^
	ret[lop].cntB=rand()%240;//�e�J�E���^
	ret[lop].cntM=0;//�ړ��p�̃J�E���^
	if(rand()%2==0)ret[lop].spd=rand()%4;//�ړ��X�s�[�h
	else ret[lop].spd=-rand()%4;
	break;
	}
}
//------------------------------------------------------------------------------�G�Q�̗񃊃Z�b�g
void ret_reset()
{
otr.flgR=0;
otr.numX=0;
otr.numY=0;
otr.flgS=(sco.iti+sco.zyu*10+sco.hya*100+sco.sen*1000+sco.man*10000)+otr.retZ;
}
//------------------------------------------------------------------------------�G�Q�̗�o��
void ret_syutsu()
{
int lopX,lopY;
if(zyouken()==1)
	{
	if(sco.iti+sco.zyu*10+sco.hya*100+sco.sen*1000+sco.man*10000>otr.flgS)	otr.flgR=1;
	}
else if(otr.flgR==1)
	{
	bs.num+=1;//�{�X�o��p
	otr.eNum-=1;//�G�@�̐����炷
	otr.retT=0;	otr.flgR=2;
	}
else if(otr.flgR==2)
	{
	//���X�ɕ\��
	int lop;
	for(lop=0; lop<otr.numX; lop++)
	ret_src(otr.retT,20+RETSU*lop,RETSU+RETSU*otr.numY,255);
	
	//�\�����ꂽ���m�\��
	for(lopY=0; lopY<otr.numY; lopY++)
	for(lopX=0; lopX<9; lopX++)
	ret_src(otr.retT,20+RETSU*lopX,RETSU+RETSU*lopY,255);
	//�A�j�����\�����Ԍv�Z
	if(otr.retT<15)otr.retT++;
	else
		{
		otr.retT=0; playSE(2,0);
		//���W�C��
		if(otr.numX!=9)otr.numX++;
		else if(otr.numY!=otr.retM)
			{
			otr.numX=1;
			otr.numY++;
			}
		else//���ۂɃ|�b�v�����ăt���O������
			{
			for(lopY=0; lopY<otr.numY+1; lopY++)
			for(lopX=0; lopX<otr.numX; lopX++)
			ret_init(eneMax,20+RETSU*lopX,RETSU+RETSU*lopY);
			//�G��ϐ����Z�b�g
			if(otr.retZ<800)otr.retZ+=100;
			ret_reset();	otr.flgR=3;
			if(otr.retM<4)otr.retM++;//����
			else otr.retM=0;
			}
		}
	}
}
//------------------------------------------------------------------------------�G�����Ɋւ�����
void ene_move(Sint16 *posX,Sint16 *posY,Sint16 *spd,Sint16 dwnS,int lop,
							Sint16 cntA,Sint16 *cntB,Sint16 *cntM,Sint16 down,Sint16 typ)
{
//�ړ��ʌv�Z
*posX+=*spd;
*posY+=dwnS;
//�w���ʒu�C��
if		 (*posX<-20)*posX=300;
else if(*posX>300)*posX=-20;

//�e���˃`�F�b�N
if(*cntB!=-1)
if(*cntB!=0) *cntB-=1;
else 
	{
	blt_init(*posX,*posY);//�e������
	*cntB=-1;
	}
//�ړ��p�^�[���v�Z
if(typ==1 ||
	 lop==2 ||(lop>=5 &&lop<=6 )||lop==9 ||(lop>=12&&lop<=14)||lop==17||(lop>=20&&lop<=23)||
	 lop==26||(lop>=29&&lop<=33)||lop==36||(lop>=39&&lop<=44))
if(*cntM<60) *cntM+=1;
else
	{
	*cntM=0;
	//�ړ��X�s�[�h�ύX
	if(rand()%2==0)*spd=down;
	else *spd=-down;
	}
if(typ==0)ene_src(cntA,*posX,*posY,255);//�G��`��
else			ret_src(cntA,*posX,*posY,255);//�G��`��
}
//------------------------------------------------------------------------------�G�܂Ƃ�
void ene_main()
{
ret_syutsu();//�G�̗�o��
ene_zouka();//�G�����`�F�b�N
ene_init(otr.eNum,rand()%280,-20);//������
int num=0;
int lop;
for(lop=0; lop<eneMax; lop++)//�G�̃��C������
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
		//�A�j���[�V�����v�Z
		if(ret[lop].cntA<20)ret[lop].cntA++;	else ret[lop].cntA=0;
		//�����蔻��
		if( hitChek_meB(ret[lop].posX,ret[lop].posY,ret[lop].spd,1)==1)ret[lop].flg=0;//���e�ƓG
		if(hitChek_eneM(ret[lop].posX,ret[lop].posY,ret[lop].spd,0)==1)ret[lop].flg=0;//���@�ƓG
		//��ԉ��܂ŗ�����t���O������
		if(ret[lop].posY>400)ret[lop].flg=0;
		}
	
	if(ene[lop].flg==1)
		{
		ene_move(	&ene[lop].posX, &ene[lop].posY,	&ene[lop].spd,	rand()%4,lop,
							 ene[lop].cntA, &ene[lop].cntB, &ene[lop].cntM,	rand()%3+1,0);
		//�A�j���[�V�����v�Z
		if(ene[lop].cntA<20)ene[lop].cntA++;	else ene[lop].cntA=0;
		//�����蔻��
		if(hitChek_meB( ene[lop].posX,ene[lop].posY,ene[lop].spd,0)==1)ene[lop].flg=0;//���e�ƓG
		if(hitChek_eneM(ene[lop].posX,ene[lop].posY,ene[lop].spd,0)==1)ene[lop].flg=0;//���@�ƓG
		//��ԉ��܂ŗ�����t���O������
		if(ene[lop].posY>400)ene[lop].flg=0;
		}
	}
}
//------------------------------------------------------------------------------�e�̕\����
void blt_src(Sint16 posX,Sint16 posY)
{
SDL_Rect rct;//�`���`�p

rct.x=posX+1; rct.y=posY+KUKEI*2; rct.w=KUKEI*1; rct.h=KUKEI/2;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,255));

rct.x=posX+2; rct.y=posY; rct.w=KUKEI/2; rct.h=KUKEI*3;
SDL_FillRect(mWIN,&rct,SDL_MapRGB(mWIN->format,255,255,0));
}
//------------------------------------------------------------------------------�e�̏�����
void blt_init(Sint16 posX,Sint16 posY)
{
int lop;
for(lop=0; lop<eBltMax; lop++)
if(tama[lop].flg==0)
	{
	tama[lop].flg=1;//�t���O���Ă�
	tama[lop].posX=posX+8; tama[lop].posY=posY;//���W���
	tama[lop].spd=3+rand()%3;//�X�s�[�h���߂�
	break;//���[�v������
	}
}
//------------------------------------------------------------------------------�e�̂܂Ƃ�
void blt_main()
{
int lop;
for(lop=0; lop<eBltMax; lop++)
if(tama[lop].flg==1)
	{
	//�ړ��ʌv�Z
	tama[lop].posY+=tama[lop].spd;
	//�e��`��
	blt_src(tama[lop].posX,tama[lop].posY);
	//�����蔻��
	hitChek_Blt(lop);
	//��ʂ���o�������
	if(tama[lop].posY>400)tama[lop].flg=0;
	}
}
//------------------------------------------------------------------------------���Z�b�g
void resets()
{
srand( (unsigned)time( NULL ) );//�������Z�b�g
ufo_reset();//UFO
score_rest();//�_��
BGM_reset();//BGM
Mix_Volume(0,VOL_SE-16);
sub_reset();//sub weapon
mes.posX=140; mes.posY=370;//���@�̏������W
mes.pow=0;	mes.spd=7;
int lop;
for(lop=0;		lop<bltMax;		lop++) mes.flgB[lop]=0;//�e�t���O������
for(lop=0;		lop<eneMax;		lop++)	{ene[lop].flg=0;ret[lop].flg=0;}//�G�t���O������
otr.eNum=1;//�G�̍ő吔�̏����l
otr.time=timer.now;//�G�����ׂ̈̎��ԃ��Z�b�g
otr.retZ=200;	ret_reset();	otr.retM=0;//�G��ϐ����Z�b�g
for(lop=0;		lop<eBltMax;		lop++) tama[lop].flg=0;//�e�t���O������
for(lop=0;		lop<efcMax;		lop++)//�����G�t�F�N�g
	{
	eft.flg[lop]=0; eft2.flg[lop]=0;
	}
bos_reset();//�{�X
stars_reset();//�o�b�N�n�̐���z�u����
}
//------------------------------------------------------------------------------�L�����̂܂Ƃ�
void chara()
{
me_main();//���@
bos_main();//boss
ene_main();//�G
ufo_main();//UFO
blt_main();//�e
efect_main();//�����G�t�F�N�g
efect_main2();
BGM_fade();//BGM
}