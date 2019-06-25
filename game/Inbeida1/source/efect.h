#define VOL_BG 96
#define VOL_SE 128

#define efcMax 64 //エフェクトの最大数
typedef struct//爆発エフェクトの変数
{
Sint16	flg[efcMax],//フラグ＆カウンタ
				posX[efcMax],//敵機の位置Ｘ
				posY[efcMax],//敵機の位置Ｙ
				cnt[efcMax],//敵機表示用
				spd[efcMax],//移動スピード
				typ[efcMax];//何と当たったか
}efectBaku;
extern efectBaku eft,eft2;

typedef struct//ＢＧＭの変数
{
int	flg,//フラグ
		cnt,//カウンタ
		num,//演奏中BGMの番号
		vol;//ボリューム
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