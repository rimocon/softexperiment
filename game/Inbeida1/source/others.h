extern int games;
extern SDL_Surface *mWIN;//���C���E�B���h�E

typedef struct//----------------------------------------------------------------�t���[�����[�g�p�̕ϐ�
{
Uint32	now,//���ݎ���
				wit,//�҂�����
				lev;//�o�ߎ���
}timers;
extern timers timer;

typedef struct//----------------------------------------------------------------�L�[���͗p�̕ϐ�
{
Uint32	l,//�����
				r,//�E���
				u,//����
				d,//�����
				z,//�y�L�[
				time;//�y�L�[�̉����ꂽ����
}inputKeys;
extern inputKeys keys,joys;

#define starMax 8
typedef struct//----------------------------------------------------------------�o�b�N�n�̐��p�̕ϐ�
{
Sint16	flg[starMax],//�t���O
				posX[starMax],//���W�w
				posY[starMax],//���W�x
				spd;//�X�s�[�h
Uint8	R,
			G,
			B;
}backStars;
extern backStars starA,starB,starC;

void startUp();
int quits();
void frames();
void backGraund();
void imageLoad();
void dr_Title(Sint16 posX,Sint16 posY);
void dr_end(Sint16 posX,Sint16 posY);
void inputEngine();
void startGame();
void backTitle();

void stars_reset();
void stars();

void bg_gra();
int zyouken();