typedef struct//���_�p�̕ϐ�
{
Sint16	iti,//��̈�
				zyu,//�\�̈�
				hya,//�S�̈�
				sen,//��̈�
				man,//���̈�
				cnt,//�����J�E���^
				zou,//�����p
				lv1,//�G�̃��x��
				lv2;
}scores;
extern scores sco,hiS;

typedef struct//���_�p�̕ϐ�
{
Sint16	iti,//�G�̃��x��
				zyu;
}levelVal;
extern levelVal lvs;

void imageLoadScore();
void score_rest();
void score_srcA(Sint16 pos,Sint16 width,Sint16 posX);
void score_srcB(Sint16 keta,Sint16 posX,Sint16 posY);
void power();
void score_main();

void text_src(Sint16 pX,Sint16 pY,Uint16 wid,Sint16 dX,Sint16 dY);
void winCur_src(Sint16 posY);
void winMode();
void level();