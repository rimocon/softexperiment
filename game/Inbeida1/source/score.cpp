#include "headers.h"
scores sco,hiS;//���_�p
levelVal lvs;//���x���p
SDL_Surface *imgS;//���_�摜�p
SDL_Rect src,drw;
//------------------------------------------------------------------------------���_�摜�ǂݍ���
void imageLoadScore()
{
imgS=IMG_Load("data\\score.dat");
if(!imgS)	SDL_Quit();
}
//------------------------------------------------------------------------------���_���Z�b�g
void score_rest()
{
sco.iti=0;
sco.zyu=0;
sco.hya=0;
sco.sen=0;
sco.man=0;
sco.zou=0;

lvs.iti=1;
lvs.zyu=0;
}
//------------------------------------------------------------------------------���_-�����摜�\������
void score_srcA(Sint16 pos,Sint16 width,Sint16 posX)
{
src.x=10*pos;
src.y=0; src.w=10*width; src.h=10;
drw.x=posX; drw.y=10;
SDL_BlitSurface(imgS,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------���_-�����摜�\������
void score_srcB(Sint16 keta,Sint16 posX,Sint16 posY)
{
src.x=10*keta;
src.y=10; src.w=10; src.h=10;
drw.x=posX; drw.y=posY;
SDL_BlitSurface(imgS,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------power
void power()
{
text_src(0,2,6,1,38);//�����`��

int lop;
for(lop=0; lop<mes.pow; lop++)
	{
	drw.w=9; drw.h=9; drw.x=70+10*lop; 
	SDL_FillRect(mWIN,&drw,SDL_MapRGB(mWIN->format,255,255,255));
	}
}
//------------------------------------------------------------------------------���_�\��
void score_main()
{
//�����\��
score_srcA(4,6,10);//SCORE:
score_srcA(0,4,135); score_srcA(4,6,180);//HIGH SCORE:

//���_�v�Z���\��
if(sco.zou!=0)
	{
	if(sco.cnt<2)sco.cnt++;
	else
		{
		sco.cnt=0;
		sco.zou--;
		sco.iti++;
		}
	}
if(sco.iti>9){sco.iti-=10; sco.zyu+=1;}
if(sco.zyu>9){sco.zyu-=10; sco.hya+=1;}
if(sco.hya>9){sco.hya-=10; sco.sen+=1;}
if(sco.sen>9){sco.sen-=10; sco.man+=1;}
if(sco.man>9) score_rest();//�_�����Z�b�g
//�_���\��
score_srcB(sco.man,70,10);//��̈�
score_srcB(sco.sen,80,10);//��̈�
score_srcB(sco.hya,90,10);//�S�̈�
score_srcB(sco.zyu,100,10);//�\�̈�
score_srcB(sco.iti,110,10);//��̈�

//�n�C�X�R�A�͂�������
if( (sco.iti+sco.zyu*10+sco.hya*100+sco.sen*1000+sco.man*10000)>=
	  (hiS.iti+hiS.zyu*10+hiS.hya*100+hiS.sen*1000+hiS.man*10000)
	)
	{
	hiS.iti=sco.iti;
	hiS.zyu=sco.zyu;
	hiS.hya=sco.hya;
	hiS.sen=sco.sen;
	hiS.man=sco.man;
	}
//�n�C�X�R�A�\��
score_srcB(hiS.man,240,10);//��̈�
score_srcB(hiS.sen,250,10);//��̈�
score_srcB(hiS.hya,260,10);//�S�̈�
score_srcB(hiS.zyu,270,10);//�\�̈�
score_srcB(hiS.iti,280,10);//��̈�

//POWER
power();
//level
level();
}
//------------------------------------------------------------------------------�E�B���h�E�I���̕���
void text_src(Sint16 pX,Sint16 pY,Uint16 wid,Sint16 dX,Sint16 dY)
{
src.x=10*pX;	src.y=10*pY; 
src.w=10*wid; src.h=10;
drw.x=10*dX; drw.y=10*dY;
SDL_BlitSurface(imgS,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------�E�B���h�E�I���J�[�\��
void winCur_src(Sint16 posY)
{
if(ufo.flg<18)ufo.flg++;	else ufo.flg=0;

if		 (ufo.flg>=0	&&	ufo.flg<3)	src.x=60;
else if(ufo.flg>=3	&&	ufo.flg<6)	src.x=70;
else if(ufo.flg>=6	&&	ufo.flg<9)	src.x=80;
else if(ufo.flg>=9	&&	ufo.flg<12)	src.x=90;
else if(ufo.flg>=12	&&	ufo.flg<15)	src.x=80;
else if(ufo.flg>=15	&&	ufo.flg<18)	src.x=70;

src.y=20; src.w=10; src.h=10;
drw.x=90; drw.y=190+10*posY;
SDL_BlitSurface(imgS,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------�E�B���h�E�I��
void winMode()
{
//�J�[�\���ړ�
if(keys.u==1	||	joys.u==1)ufo.spd=0;
else if(keys.d==1	||	joys.d==1)ufo.spd=1;
else if(keys.z==1	||	joys.z==1)//�E�B���h�E���[�h����
	{
	keys.z=0;joys.z=0;//�A�˖h�~
	playSE(0,0);//���ʉ�
	games=0;//�^�C�g����ʂ�
	if(ufo.spd==0)//�t���X�N���[��
		{
		mWIN=SDL_SetVideoMode(300,400,32,SDL_SWSURFACE	|	SDL_FULLSCREEN);
		SDL_ShowCursor(SDL_DISABLE);//�J�[�\��������
		}
	else if(ufo.spd==1)//�E�B���h�E
		mWIN=SDL_SetVideoMode(300,400,32,SDL_SWSURFACE);
	}
text_src(0,3,10,10,19);//�����`��
text_src(0,4, 6,10,20);//�����`��
winCur_src(ufo.spd);//�J�[�\���`��
}
//------------------------------------------------------------------------------���x���\��
void level()
{
text_src(6,4,4,23,38);//�����`��
score_srcB(lvs.zyu,270,380);
score_srcB(lvs.iti,280,380);
}