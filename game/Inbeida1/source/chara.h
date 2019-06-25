#define KUKEI 4

#define bltMax 3
typedef struct//自機用の変数
{
float	posX,//自機の位置Ｘ
			posY;//自機の位置Ｙ
Sint16	flgB[bltMax],//弾数
				bltX[bltMax],//弾のＸ座標
				bltY[bltMax],//弾のＹ座標
				pow,//パワーアップ
				spd;//弾のスピード
Uint32 hTM;
}ziki;
extern ziki mes;

#define eneMax 45
typedef struct//敵の変数
{
Sint16	flg, //出現フラグ
				posX,//敵の位置Ｘ
				posY,//敵の位置Ｙ
				cntA,//アニメ用のカウンタ
				cntB,//弾発射用のカウンタ
				cntM,//移動用のカウンタ
				spd; //移動スピード
}enemys;
extern enemys ene[eneMax],ret[eneMax],ufo;

typedef struct//その他の変数
{
Uint32	eNum,//敵出現の最大値
				time,//敵増加のタイミング
				flgR,//敵の列出現フラグ
				flgS,//敵の列出現の得点
				retT,//敵の列出現に使うタイミング
				retZ,//敵の列出現の増加値
				retM,//敵の列の最大値
				numX,//敵の列の固執
				numY;
}otherVal;
extern otherVal otr;

#define eBltMax 64
typedef struct//弾の変数
{
Sint16	flg, //出現フラグ
				posX,//敵の位置Ｘ
				posY,//敵の位置Ｙ
				spd; //移動スピード
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