extern int games;
extern SDL_Surface *mWIN;//メインウィンドウ

typedef struct//----------------------------------------------------------------フレームレート用の変数
{
Uint32	now,//現在時間
				wit,//待ち時間
				lev;//経過時間
}timers;
extern timers timer;

typedef struct//----------------------------------------------------------------キー入力用の変数
{
Uint32	l,//左矢印
				r,//右矢印
				u,//上矢印
				d,//下矢印
				z,//Ｚキー
				time;//Ｚキーの押された時間
}inputKeys;
extern inputKeys keys,joys;

#define starMax 8
typedef struct//----------------------------------------------------------------バック地の星用の変数
{
Sint16	flg[starMax],//フラグ
				posX[starMax],//座標Ｘ
				posY[starMax],//座標Ｙ
				spd;//スピード
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