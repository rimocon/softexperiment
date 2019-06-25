#define VOL_BG 96
#define VOL_SE 128

#define efcMax 64 //�G�t�F�N�g�̍ő吔
typedef struct//�����G�t�F�N�g�̕ϐ�
{
Sint16	flg[efcMax],//�t���O���J�E���^
				posX[efcMax],//�G�@�̈ʒu�w
				posY[efcMax],//�G�@�̈ʒu�x
				cnt[efcMax],//�G�@�\���p
				spd[efcMax],//�ړ��X�s�[�h
				typ[efcMax];//���Ɠ���������
}efectBaku;
extern efectBaku eft,eft2;

typedef struct//�a�f�l�̕ϐ�
{
int	flg,//�t���O
		cnt,//�J�E���^
		num,//���t��BGM�̔ԍ�
		vol;//�{�����[��
}soundsVal;
extern soundsVal snd;

int hitChek_meB(Sint16 posX,Sint16 posY,Sint16 spd,Sint16 typ);
int hitChek_eneM(Sint16 posX,Sint16 posY,Sint16 spd,Sint16 typ);
void hitChek_Blt(int lop);
void loadSounds();
void playSE(int num,int lop);
void efect_init(Sint16 posX,Sint16 posY,Sint16 spd,Sint16 typ);
void efect_main();
void efect_init2(Sint16 posX,Sint16 posY,Sint16 typ);
void efect_main2();

void BGM_reset();
void BGM_play(int num);
void BGM_fade();