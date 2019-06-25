#define KUKEI 4

#define bltMax 3
typedef struct//���@�p�̕ϐ�
{
float	posX,//���@�̈ʒu�w
			posY;//���@�̈ʒu�x
Sint16	flgB[bltMax],//�e��
				bltX[bltMax],//�e�̂w���W
				bltY[bltMax],//�e�̂x���W
				pow,//�p���[�A�b�v
				spd;//�e�̃X�s�[�h
Uint32 hTM;
}ziki;
extern ziki mes;

#define eneMax 45
typedef struct//�G�̕ϐ�
{
Sint16	flg, //�o���t���O
				posX,//�G�̈ʒu�w
				posY,//�G�̈ʒu�x
				cntA,//�A�j���p�̃J�E���^
				cntB,//�e���˗p�̃J�E���^
				cntM,//�ړ��p�̃J�E���^
				spd; //�ړ��X�s�[�h
}enemys;
extern enemys ene[eneMax],ret[eneMax],ufo;

typedef struct//���̑��̕ϐ�
{
Uint32	eNum,//�G�o���̍ő�l
				time,//�G�����̃^�C�~���O
				flgR,//�G�̗�o���t���O
				flgS,//�G�̗�o���̓��_
				retT,//�G�̗�o���Ɏg���^�C�~���O
				retZ,//�G�̗�o���̑����l
				retM,//�G�̗�̍ő�l
				numX,//�G�̗�̌Ŏ�
				numY;
}otherVal;
extern otherVal otr;

#define eBltMax 64
typedef struct//�e�̕ϐ�
{
Sint16	flg, //�o���t���O
				posX,//�G�̈ʒu�w
				posY,//�G�̈ʒu�x
				spd; //�ړ��X�s�[�h
}tamas;
extern tamas tama[eBltMax];

void me_src(Sint16 posX,Sint16 posY);
void me_move();
void me_srcB(Sint16 posX,Sint16 posY);
void me_srcB2(Sint16 posX,Sint16 posY);
void me_bullet();
void me_power();
void me_main();

void ene_src(Sint16 ani,Sint16 posX,Sint16 posY,Uint8 clrB);
void ene_zouka();
void ene_init(int max,Sint16 posX,Sint16 posY);
void ene_move(Sint16 *posX,Sint16 *posY,Sint16 *spd,Sint16 dwnS,int lop,Sint16 cntA,Sint16 *cntB,Sint16 *cntM,Sint16 down,Sint16 typ);

void ret_src(Sint16 ani,Sint16 posX,Sint16 posY,Uint8 clrB);
void ret_init(int max,Sint16 posX,Sint16 posY);
void ret_reset();
void ret_syutsu();

void ene_main();

void blt_src(Sint16 posX,Sint16 posY);
void blt_init(Sint16 posX,Sint16 posY);
void blt_main();

void resets();
void chara();