#include "headers.h"
enemys ufo;//UFO�p
SDL_Surface *imgUfo;//�摜�p
Uint16 ufoStart;//UFO�o�������p
//------------------------------------------------------------------------------�摜�ǂݍ���
void imageLoadUFO()
{
imgUfo=IMG_Load("data\\ufo.dat");
if(!imgUfo)	SDL_Quit();
}
//------------------------------------------------------------------------------�摜�ǂݍ���
void ufo_reset()
{
ufo.flg=0; ufoStart=1;//UFO
}
//------------------------------------------------------------------------------�摜�\���p
void ufo_src(Sint16 posX,Sint16 posY)
{
SDL_Rect src,drw;//UFO�`��p
src.x=0;

//�A�j��
if(ufo.cntA<16)ufo.cntA++;
else ufo.cntA=0;
//�A�j�����x���ɔ��f
if(ufo.cntA>=0	&&	ufo.cntA<2)src.y=12*0;
else if(ufo.cntA>=2	&&	ufo.cntA<4)src.y=12*1;
else if(ufo.cntA>=4	&&	ufo.cntA<6)src.y=12*2;
else if(ufo.cntA>=6	&&	ufo.cntA<8)src.y=12*3;
else if(ufo.cntA>=8	&&	ufo.cntA<10)src.y=12*4;
else if(ufo.cntA>=10	&&	ufo.cntA<12)src.y=12*5;
else if(ufo.cntA>=12	&&	ufo.cntA<14)src.y=12*6;
else if(ufo.cntA>=14	&&	ufo.cntA<16)src.y=12*7;

src.w=40; src.h=12;//�\������摜�͈̔͌���
drw.x=posX; drw.y=posY;//�\������摜�͈̔͌���
SDL_BlitSurface(imgUfo,&src,mWIN,&drw);
}
//------------------------------------------------------------------------------������
void ufo_init()
{
if(zyouken()==1 && otr.eNum>ufoStart)//�o�����
	{
	ufo.flg=1;//�t���O���Ă�
	ufo.posX=rand()%260;//�o����W����
	ufo.posY=-300;
	ufo.cntA=0;//�A�j���p�̃J�E���^
	ufo.cntB=0;//�ގU���Ԃ̃J�E���^���Z�b�g
	ufo.cntM=rand()%30;//�ړ��p�̃J�E���^
	//�ړ��X�s�[�h
	if(rand()%2==0)ufo.spd=1+rand()%5;
	else ufo.spd=-(1+rand()%5);
	snd.flg=1;//BGM�t�F�[�h�t���O���Ă�
	ufoStart+=(rand()%2+2);
	}
else if(ufo.flg==1)//�o��
	{
	if(ufo.posY<40)ufo.posY++;
	else ufo.flg=2;
	
	ufo_src(ufo.posX,ufo.posY);//�`��
	}
}
//------------------------------------------------------------------------------���e��UFO�̓����蔻��
void hitChek_ufo()
{
SDL_Rect R1,R2;
//UFO�̍��W�Ƒ傫���ݒ�
R1.x=ufo.posX;	R1.y=ufo.posY; R1.w=40; R1.h=12;
	
int blt;//�\�����̎��e��T��
for(blt=0;blt<bltMax;blt++)
if(mes.flgB[blt]==1)
	{
	//���e�̍��W�Ƒ傫���ݒ�
	R2.x=mes.bltX[blt]-1; R2.y=mes.bltY[blt]; R2.w=4;	R2.h=24;
	//���蔻��A�������Ă���t���O���Z
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		if(ufo.flg<8)ufo.flg++;//UFO�t���O���Z
		else
			{
			otr.eNum-=1;//�G�@�̐����炷
			sco.zou+=100;//���_
			ufo.flg=-2;
			snd.flg=1;//BGM�ς���
			if(rand()%6==0){mes.pow+=2;playSE(3,1);}//���@�p���[�A�b�v
			else {mes.pow++;playSE(3,0);}
			}
		mes.flgB[blt]=0;//���e�t���O
		playSE(1,0);//���ʉ�
		//�����G�t�F�N�g���̂Q������
		efect_init2(ufo.posX,ufo.posY,1);
		break;
		}
	}
for(blt=0;blt<subWmax;blt++)
if(subA.flg[blt]==1)
	{
	//���e�̍��W�Ƒ傫���ݒ�
	R2.x=subA.posX[blt]; R2.y=subA.posY[blt]; R2.w=4;	R2.h=12;
	//���蔻��A�������Ă���t���O���Z
	if(	((R1.x)<(R2.x+R2.w))	&& ((R2.x)<(R1.x+R1.w))&&((R1.y)<(R2.y+R2.h))	&& ((R2.y)<(R1.y+R1.h))	)
		{
		if(ufo.flg<8)ufo.flg++;//UFO�t���O���Z
		else
			{
			otr.eNum-=1;//�G�@�̐����炷
			sco.zou+=50;//���_
			ufo.flg=-2;
			snd.flg=1;//BGM�ς���
			if(rand()%13==0){mes.pow+=2;playSE(3,1);}//���@�p���[�A�b�v
			else {mes.pow++;playSE(3,0);}
			}
		subA.flg[blt]=0;//���e�t���O
		playSE(1,0);//���ʉ�
		//�����G�t�F�N�g���̂Q������
		efect_init2(ufo.posX,ufo.posY,1);
		break;
		}
	}
}
//------------------------------------------------------------------------------���C������
void ufo_main()
{
ufo_init();//������
if(ufo.flg>=2)//���C������
	{
	//�ړ��ʌv�Z
	static Sint16 upDwon=1;
	ufo.posX+=ufo.spd;
	ufo.posY+=upDwon;
	if(ufo.posY==20)upDwon=1;
	else if(ufo.posY==160)upDwon=-1;
	//�w���ʒu�C��
	if(ufo.posX<-40)ufo.posX=300;
	else if(ufo.posX>300)ufo.posX=-40;
	//�ގU�܂ł̃J�E���^
	if(ufo.cntB<2100)ufo.cntB++;
	else
		{
		ufo.flg=-1;
		snd.flg=1;//BGM�ς���
		}
	//�ړ��ƒe
	if(ufo.cntM<60)ufo.cntM++;
	else
		{
		blt_init(ufo.posX,ufo.posY);//�e������
		ufo.cntM=rand()%30;
		if(rand()%2==0)ufo.spd=1+rand()%5;
		else ufo.spd=-(1+rand()%5);
		}
	ufo_src(ufo.posX,ufo.posY);//�`��
	hitChek_ufo();//�����蔻��
	}
else if(ufo.flg<=-1)//�ގU
	{
	if(ufo.posY>-12)
		{
		ufo.posY--;
		ufo_src(ufo.posX,ufo.posY);//�`��
		
		if(ufo.flg==-2)//�j���͔����G�t�F�N�g�\��
		if(ufo.cntB<30)ufo.cntB++;
		else
			{
			//�����G�t�F�N�g���̂Q������
			efect_init2(ufo.posX,ufo.posY,1);
			ufo.cntB=0;
			}
		}
	else//UFO�I��
		{
		ufo.cntB=0; ufo.flg=0;
		}
	}
}