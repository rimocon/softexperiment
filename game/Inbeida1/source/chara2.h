#define subWmax 4
typedef struct//���@�T�u�V���b�g�p�̕ϐ�
{
Sint16	flg[subWmax],//�t���O
				posX[subWmax],//�e�̂w���W
				posY[subWmax],//�e�̂x���W
				spd[subWmax],
				vec,//�e�̕���
				num;//�e�̐�
}subWeapon;
extern subWeapon subA,subB;

void sub_reset();
void sub_init();
void sub_main();

void subA_src(Sint16 posX,Sint16 posY);
void subB_src(Sint16 posX,Sint16 posY);