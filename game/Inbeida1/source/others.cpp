#include "headers.h"
int games;
SDL_Surface *mWIN;
timers timer;
inputKeys keys,joys;
SDL_Surface *imgT;//�摜�p
SDL_Rect src,drw;//�`��p
backStars starA,starB,starC;//�o�b�N�n�p
SDL_Joystick *Joy;
//------------------------------------------------------------------------------�N�����ڂȂ�
void startUp()
{
if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_AUDIO)==-1)	SDL_Quit();
mWIN=SDL_SetVideoMode(300,400,32,SDL_SWSURFACE	//|SDL_FULLSCREEN
);
SDL_WM_SetCaption("Inbeida",NULL);
games=-1;
imageLoad();//�摜���[�h
loadSounds();//�����[�h
imageLoadUFO();//UFO�ǂݍ���
imageLoadScore();//���_�摜���[�h
imageLoadBoss();//boss�摜
Joy=SDL_JoystickOpen(0);//�Q�[���R���g���[��
hiS.iti=2;//VENIO�̃n�C�X�R�A
hiS.zyu=3;
hiS.hya=8;
hiS.sen=0;
hiS.man=2;
}
//------------------------------------------------------------------------------�I���ݒ�
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
//------------------------------------------------------------------------------FPS����
void frames()
{
timer.now=SDL_GetTicks();//���ݎ��Ԃ��擾
timer.wit=timer.now-timer.lev;//�҂����Ԃ��v�Z

if(timer.wit<16)//16�ȉ��Ȃ�CPU���x�܂���
	{
	SDL_Delay(16-timer.wit);
	}
timer.lev=SDL_GetTicks();//�o�ߎ��Ԃ��X�V
SDL_UpdateRect(mWIN,0,0,0,0);//��ʂ��X�V
}
//------------------------------------------------------------------------------�o�b�N�n
void backGraund()
{
if(games==-1)SDL_FillRect(mWIN,NULL,SDL_MapRGB(mWIN->format,0,0,0));
else if(games==0)SDL_FillRect(mWIN,NULL,SDL_MapRGB(mWIN->format,255,0,0));
else
	{
	bg_gra();//�O���f
	stars();//�o�b�N�n�̐�
	}
}
//------------------------------------------------------------------------------�摜�ǂݍ���
void imageLoad()
{
imgT=IMG_Load("data\\start_end.dat");
if(!imgT)	SDL_Quit();
}
//------------------------------------------------------------------------------�摜�\��_�^�C�g��
void dr_Title(Sint16 posX,Sint16 posY)
{
src.x=0; src.y=0; src.w=100; src.h=50;
drw.x=posX; drw.y=posY;
SDL_BlitSurface(imgT,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------�摜�\��_�Q�[���I�[�o�[
void dr_end(Sint16 posX,Sint16 posY)
{
src.x=0; src.y=50; src.w=100; src.h=50;
drw.x=posX; drw.y=posY;
SDL_BlitSurface(imgT,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------���͑���
void inputEngine()
{
Uint8 *getKeys=SDL_GetKeyState(NULL);
//-----------------------------------���L�[�m�F
if(getKeys[SDLK_LEFT]==SDL_PRESSED)		keys.l=1; else keys.l=0;
if(getKeys[SDLK_RIGHT]==SDL_PRESSED)	keys.r=1; else keys.r=0;
if(getKeys[SDLK_UP]==SDL_PRESSED)			keys.u=1; else keys.u=0;
if(getKeys[SDLK_DOWN]==SDL_PRESSED)		keys.d=1; else keys.d=0;
//-----------------------------------�y��L�[�m�F
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
//PS2�̃R���g���[��
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
//------------------------------------------------------------------------------�Q�[���X�^�[�g
void startGame()
{
if(keys.z==1	||	joys.z==1)
	{
	games=1;//�y�L�[�������ꂽ��Q�[����
	resets();//�F�X���Z�b�g
	}
}
//------------------------------------------------------------------------------�^�C�g����ʂ֖߂�
void backTitle()
{
if(timer.now-otr.time>1000*1.5)
if(keys.z==1	||	joys.z==1)
games=0;//�y�L�[�������ꂽ��Q�[����
}
//------------------------------------------------------------------------------�o�b�N�n�̐��̔z�u
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
//------------------------------------------------------------------------------�o�b�N�n�̐��̕`��
void stars()
{
SDL_Rect rc;//���p
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
	//�����i�ȉ�����j
	if(starA.flg[lop]==0)
		{
		starA.flg[lop]=1;
		starA.posX[lop]=rand()%300;
		starA.posY[lop]=-rand()%5;
		}
	else if(starA.flg[lop]==1)
		{
		starA.posY[lop]+=starA.spd;//�ړ��ʌv�Z�����
		rc.x=starA.posX[lop]; rc.y=starA.posY[lop];
		//�`��
		SDL_FillRect(mWIN,&rc,SDL_MapRGB(mWIN->format,starA.R,starA.G,starA.B));
		//�t���O������
		if(starA.posY[lop]>(400-rc.h)+rc.h)starA.flg[lop]=0;
		}
	//������
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
	//�΂̐�
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
//------------------------------------------------------------------------------�o�b�N�n�̃O���f�[�V����
void bg_gra()
{
Uint8 B=0;//�F�p

SDL_Rect gra;//��`�̈�ݒ�
gra.x=0; gra.y=0;
gra.w=300; gra.h=4;

int lop;
for(lop=0; lop<100; lop++)//�`��
	{
	//�h��ׂ�
	if(bs.flg==0)	SDL_FillRect(mWIN,&gra,SDL_MapRGB(mWIN->format,0,0,B));
	else					SDL_FillRect(mWIN,&gra,SDL_MapRGB(mWIN->format,B,0,0));
	gra.y+=4;//�x������
	B++;//�F���l����
	}
}
//------------------------------------------------------------------------------�C�x���g�̏���
int zyouken()
{
if(otr.flgR==0	&&	ufo.flg==0	&&	bs.flg==0)return 1;
return 0;
}
