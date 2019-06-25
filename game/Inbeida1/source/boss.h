typedef struct//----------------------------------------------------------------ボスAの変数
{
Sint16	fg[3],
				posX,
				posY,
				blt[3],//弾のカウント
				vec;//回転方向
Uint8 ani;//アニメカウント
double	rt[2],
				zom;
}bosses;
extern bosses bsA;

#define BB_PART 5
typedef struct//----------------------------------------------------------------ボスBの変数
{
Sint16	fg[BB_PART],
				pX[BB_PART],
				pY[BB_PART],
				bt[BB_PART],//弾のカウント
				ptn,//左右移動
				mov,//bitの移動
				bit;
}bossB;
extern bossB bsB;

#define BC_PART 13
typedef struct//----------------------------------------------------------------ボスCの変数
{
Sint16	fg[BC_PART],
				pX[BC_PART],
				pY[BC_PART],
				bt[BC_PART];//弾のカウント
}bossC;
extern bossC bsC;

typedef struct//----------------------------------------------------------------ボスのその他変数
{
Sint16	flg,
				num;
}bossVal;
extern bossVal bs;



void imageLoadBoss();

void bos_test();

void bos_srcA(Sint16 posX,Sint16 posY,double rot,double zom);

void bos_reset();
void bosA_init();

void bosA_mov(Sint16 posX,Sint16 posY);
void bosA_main();

void bos_main();

int hitChek_Bos(Sint16 posX,Sint16 posY,Uint16 wid,Uint16 hig,Sint16 typ);

void bosB_init();
void bosB_bit();
void bosB_mov();
void bosB_main();

void bosC_init();
void bosC_bit();
void bosC_mov();
void bosC_main();