#include "headers.h"
#define SE_MAX 7
#define BGM_MAX 4
Mix_Chunk *se[SE_MAX];//���ʉ�
Mix_Music *BGM[BGM_MAX];//BGM
efectBaku eft,eft2;//�����G�t�F�N�g
soundsVal snd;//BGM�p
//------------------------------------------------------------------------------���e�ƓG�@�̓����蔻��
int hitChek_meB(Sint16 posX,Sint16 posY,Sint16 spd,Sint16 typ)
{
SDL_Rect R1,R2;
//�G�̍��W�Ƒ傫���ݒ�
R1.x=posX;	R1.y=posY; R1.w=20; R1.h=20;

int blt;//�\�����̎��e��T��
for(blt=0;blt<bltMax;blt++)
if(mes.flgB[blt]==1)
	{
	//���e�̍��W�Ƒ傫���ݒ�
	if(mes.pow>6)	{R2.x=mes.bltX[blt];	R2.y=mes.bltY[blt]; R2.w=10;	R2.h=24;}
	else					{R2.x=mes.bltX[blt];	R2.y=mes.bltY[blt]; R2.w=2;		R2.h=24;}
	//���蔻��
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		mes.flgB[blt]=0;//���e�t���O
		playSE(1,0);//���ʉ�
		efect_init(posX,posY,spd,typ);//�����G�t�F�N�g������
		sco.zou+=3;//���_+3
		return 1;
		}
	}
for(blt=0;blt<subWmax;blt++)
if(subA.flg[blt]==1)
	{
	//���e�̍��W�Ƒ傫���ݒ�
	R2.x=subA.posX[blt]; R2.y=subA.posY[blt]; R2.w=4;	R2.h=12;
	//���蔻��
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		subA.flg[blt]=0;//���e�t���O
		playSE(1,0);//���ʉ�
		efect_init(posX,posY,spd,typ);//�����G�t�F�N�g������
		sco.zou+=1;//���_+3
		return 1;
		}
	}
for(blt=0;blt<subWmax;blt++)
if(subB.flg[blt]==1)
	{
	//���e�̍��W�Ƒ傫���ݒ�
	R2.x=subB.posX[blt]; R2.y=subB.posY[blt]; R2.w=12;	R2.h=2;
	//���蔻��
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		subB.flg[blt]=0;//���e�t���O
		playSE(1,0);//���ʉ�
		efect_init(posX,posY,spd,typ);//�����G�t�F�N�g������
		sco.zou+=2;//���_+3
		return 1;
		}
	}
return 0;
}
//------------------------------------------------------------------------------���@�ƓG�@�̓����蔻��
int hitChek_eneM(Sint16 posX,Sint16 posY,Sint16 spd,Sint16 typ)
{
SDL_Rect R1,R2;
//�G�̍��W�Ƒ傫���ݒ�
R1.x=posX;	R1.y=posY; R1.w=20; R1.h=20;
//���@�̍��W�Ƒ傫���ݒ�
R2.x=mes.posX+8; R2.y=mes.posY+8; R2.w=4;	R2.h=8;

//���蔻��A�������Ă���t���O������
if(timer.now-mes.hTM>500)
if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
	{
	if(mes.pow==0)games=2;	else mes.pow--;//�Q�[���I�[�o�[��ʂ֖��̓p���[����
	otr.time=timer.now;//�ڐG���Ԏ擾
	playSE(4,0);//���ʉ�
	efect_init(posX,posY,spd,typ);
	efect_init2(mes.posX-15,mes.posY-15,0);
	mes.hTM=timer.now;//�A���œ�����Ȃ���[�ɂ���
	return 1;
	}
return 0;
}
//------------------------------------------------------------------------------�G�e�Ǝ��@�̓����蔻��
void hitChek_Blt(int lop)
{
SDL_Rect R1,R2;
//�G�e�̍��W�Ƒ傫���ݒ�
R1.x=tama[lop].posX;	R1.y=tama[lop].posY; R1.w=4; R1.h=12;
//���@�̍��W�Ƒ傫���ݒ�
R2.x=mes.posX+8; R2.y=mes.posY+8; R2.w=4;	R2.h=8;

//���蔻��A�������Ă���t���O������
if(timer.now-mes.hTM>500)
if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
	{
	if(mes.pow==0)games=2;	else mes.pow--;//�Q�[���I�[�o�[��ʂ֖��̓p���[����
	tama[lop].flg=0;
	otr.time=timer.now;//�ڐG���Ԏ擾
	playSE(4,0);//���ʉ�
	efect_init2(mes.posX-15,mes.posY-15,0);
	mes.hTM=timer.now;//�A���œ�����Ȃ���[�ɂ���
	}
}
//------------------------------------------------------------------------------���t�@�C���ǂݍ���
void loadSounds()
{
//�ݒ�
if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1)SDL_Quit();
//�ǂݍ���
se[0]=Mix_LoadWAV("data\\se0.dat");
se[1]=Mix_LoadWAV("data\\se1.dat");
se[2]=Mix_LoadWAV("data\\se2.dat");
se[3]=Mix_LoadWAV("data\\se3.dat");
se[4]=Mix_LoadWAV("data\\se4.dat");
se[5]=Mix_LoadWAV("data\\se5.dat");
se[6]=Mix_LoadWAV("data\\se6.dat");
//BGM
BGM[0]=Mix_LoadMUS("data\\BGM0.dat");
BGM[1]=Mix_LoadMUS("data\\BGM1.dat");
BGM[2]=Mix_LoadMUS("data\\BGM2.dat");
BGM[3]=Mix_LoadMUS("data\\BGM3.dat");
int lop;
for(lop=0; lop<SE_MAX; lop++)//�ǂݍ��݃G���[�`�F�b�N
if(!se[lop]) SDL_Quit();
for(lop=0; lop<BGM_MAX; lop++)//�ǂݍ��݃G���[�`�F�b�N
if(!BGM[lop]) SDL_Quit();
Mix_VolumeMusic(VOL_BG);
Mix_Volume(-1,VOL_SE);
Mix_Volume(0,127);
}
//------------------------------------------------------------------------------���ʉ��炷
void playSE(int num,int lop)
{
Mix_PlayChannel(num, se[num], lop);
}
//------------------------------------------------------------------------------�����G�t�F�N�g�������p
void efect_init(Sint16 posX,Sint16 posY,Sint16 spd,Sint16 typ)
{
int lop;
for(lop=0;		lop<efcMax;		lop++) 
if(eft.flg[lop]==0)
	{
	eft.flg[lop]=1;//�t���O�I��
	eft.posX[lop]=posX-20;//�G�@�̈ʒu�i�[
	eft.posY[lop]=posY-20;
	eft.cnt[lop]=0;
	eft.spd[lop]=spd;
	eft.typ[lop]=typ;
	break;//���[�v������
	}
}
//------------------------------------------------------------------------------�����G�t�F�N�g�\��
void efect_main()
{
int lop;
for(lop=0;		lop<efcMax;		lop++) 
if(eft.flg[lop]>=1)
	{
	SDL_Rect efe;//�`���`�p
	//�ړ��ʌv�Z
	eft.posX[lop]+=eft.spd[lop];
	eft.posY[lop]+=rand()%3;
	//���W���傫�����
	efe.x=eft.posX[lop]+rand()%40;
	efe.y=eft.posY[lop]+rand()%40;
	efe.w=5+rand()%25; efe.h=5+rand()%25;
	//�G�̓_�ŕ\��
	if(eft.cnt[lop]<3)eft.cnt[lop]++;
	else
		{
		if		 (eft.typ[lop]==0)ene_src(0,eft.posX[lop]+20,eft.posY[lop]+20,255);
		else if(eft.typ[lop]==1)ret_src(0,eft.posX[lop]+20,eft.posY[lop]+20,255);
		eft.cnt[lop]=0;
		}
	//�F���肵�����\��
	Uint8 R=rand()%256;
	Uint8 G=rand()%256;
	SDL_FillRect(mWIN,&efe,SDL_MapRGB(mWIN->format,R,G,255));
	//�\���J�E���^
	if(eft.flg[lop]<30)eft.flg[lop]++;
	else eft.flg[lop]=0;//�t���O������
	}
}
//------------------------------------------------------------------------------�����G�t�F�N�g�������p���̂Q
void efect_init2(Sint16 posX,Sint16 posY,Sint16 typ)
{
int lop;
for(lop=0;		lop<efcMax;		lop++) 
if(eft2.flg[lop]==0)
	{
	eft2.flg[lop]=1;//�t���O�I��
	eft2.posX[lop]=posX;//�G�@�̈ʒu�i�[
	eft2.posY[lop]=posY;
	eft2.cnt[lop]=0;
	//eft2.spd[lop] ���g�p
	eft2.typ[lop]=typ;
	break;//���[�v������
	}
}
//------------------------------------------------------------------------------�����G�t�F�N�g�\�����̂Q
void efect_main2()
{
int lop;
for(lop=0;		lop<efcMax;		lop++) 
if(eft2.flg[lop]>=1)
	{
	SDL_Rect efe;//�`���`�p
	//�ړ��ʌv�Z
	if(eft2.typ[lop]==0)
		{
		eft2.posX[lop]=mes.posX-10;	eft2.posY[lop]=mes.posY-10;
		}
	else if(eft2.typ[lop]==1)
		{
		eft2.posX[lop]=ufo.posX;	eft2.posY[lop]=ufo.posY-15;
		}
	else if(eft2.typ[lop]==2){	eft2.posX[lop]=bsA.posX+30-10;	eft2.posY[lop]=100-10;}
	else if(eft2.typ[lop]==3){	eft2.posX[lop]=bsA.posX-10;			eft2.posY[lop]=100-10;}
	else if(eft2.typ[lop]==4){	eft2.posX[lop]=bsA.posX+60-10;	eft2.posY[lop]=100-10;}
	else if(eft2.typ[lop]>=10){	eft2.posX[lop]=bsC.pX[eft2.typ[lop]-10]-10;	eft2.posY[lop]=bsC.pY[eft2.typ[lop]-10]-10;}
	else if(eft2.typ[lop]>=5){	eft2.posX[lop]=bsB.pX[eft2.typ[lop]-5]-10;	eft2.posY[lop]=bsB.pY[eft2.typ[lop]-5]-10;}
	
	//���W���傫�����
	efe.x=eft2.posX[lop]+rand()%30;
	efe.y=eft2.posY[lop]+rand()%30;
	efe.w=3+rand()%20; efe.h=3+rand()%20;
	//�F���肵�����\��
	//Uint8 clr=rand()%3;
	if(rand()%3==0)SDL_FillRect(mWIN,&efe,SDL_MapRGB(mWIN->format,255,0,0));
	else if(rand()%2==0)SDL_FillRect(mWIN,&efe,SDL_MapRGB(mWIN->format,255,255,0));
	else SDL_FillRect(mWIN,&efe,SDL_MapRGB(mWIN->format,255,255,255));
	//�\���J�E���^
	if(eft2.flg[lop]<80)eft2.flg[lop]++;
	else eft2.flg[lop]=0;//�t���O������
	}
}
//------------------------------------------------------------------------------BGM�̉��t
void BGM_play(int num)
{
Mix_PlayMusic(BGM[num],-1);
}
//------------------------------------------------------------------------------BGM�̃��Z�b�g
void BGM_reset()
{
snd.flg=0; snd.cnt=0;
snd.num=0; snd.vol=VOL_BG;//BGM
Mix_VolumeMusic(snd.vol);
BGM_play(snd.num);
}
//------------------------------------------------------------------------------BGM�̃t�F�[�h�C���^�A�E�g
void BGM_fade()
{
if(snd.flg==1)//�t�F�[�h�A�E�g
	{
	if(snd.cnt<1)snd.cnt++;//���ʒ��ߎ���
	else
		{
		if(snd.vol>0)Mix_VolumeMusic(snd.vol--);//���ʒ���
		else
			{
			snd.flg=2;//�t���O�ύX
			//BGM�ԍ��ύX
			if(snd.num==0	||	snd.num==3)
				{
				if(bs.flg!=0)	snd.num=2;
				else					snd.num=1;
				}
			else
				{
				if(sco.man<1)	snd.num=0;
				else					snd.num=3;
				}
			BGM_play(snd.num);
			}
		snd.cnt=0;//�J�E���^�O��
		}
	}
else if(snd.flg==2)//�t�F�[�h�C��
	{
	if(snd.cnt<1)snd.cnt++;
	else
		{
		if(snd.vol<VOL_BG)Mix_VolumeMusic(snd.vol++);
		else snd.flg=0;
		
		snd.cnt=0;
		}
	}
}