#define subWmax 4
typedef struct//自機サブショット用の変数
{
Sint16	flg[subWmax],//フラグ
				posX[subWmax],//弾のＸ座標
				posY[subWmax],//弾のＹ座標
				spd[subWmax],
				vec,//弾の方向
				num;//弾の数
}subWeapon;
extern subWeapon subA,subB;

void sub_reset();
void sub_init();
void sub_main();

void subA_src(Sint16 posX,Sint16 posY);
void subB_src(Sint16 posX,Sint16 posY);