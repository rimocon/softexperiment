typedef struct//得点用の変数
{
Sint16	iti,//一の位
				zyu,//十の位
				hya,//百の位
				sen,//千の位
				man,//万の位
				cnt,//増加カウンタ
				zou,//増加用
				lv1,//敵のレベル
				lv2;
}scores;
extern scores sco,hiS;

typedef struct//得点用の変数
{
Sint16	iti,//敵のレベル
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