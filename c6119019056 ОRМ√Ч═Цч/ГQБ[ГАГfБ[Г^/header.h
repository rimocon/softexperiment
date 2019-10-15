#ifndef HEAD_H
#define HEAD_H


#define bulletmax 3
#define enemynumber 6
/******C言語のヘッダファイル****/
#include <math.h> //数学的計算用
#include <stdarg.h> //可変な型や引数をとることができる
#include <stdio.h> //標準
#include <stdlib.h> //標準マクロなど(NULLとか)
#include <string.h> //文字列を扱いやすくする
#include <time.h> //時間用
#include <stdbool.h> //bool型を使うため

/************SDL関連************/
#include <SDL2/SDL.h> //SDLを使う
#include <SDL2/SDL2_gfxPrimitives.h> //SDLで図形描画
#include <SDL2/SDL_ttf.h> //SDLで文字描画
#include <SDL2/SDL_image.h> //SDLで画像を扱う
#include <SDL2/SDL_mixer.h> //SDLで音楽を扱う

/*********グローバル変数***********/
int games; //ゲームの現在の状態を取得するために使う
int score; //得点
char scorestr[32]; //得点の文字列
bool titledraw; //一回だけ描画する用
bool gamedraw; //一回だけ描画する用
bool run; //終了するかどうか
bool enemylive; //敵が生きているかどうか
bool rightmove; //敵きゃら右移動するかどうか
bool rightmove2; //敵きゃら右移動するかどうか
bool rightmove3; //敵きゃら右移動するかどうか
bool up; //上かどうか
bool down; //下かどうか	

bool test_enemylive[enemynumber];

TTF_Font *font; //フォント用

SDL_Window *mainwindow; //メイン画面用

SDL_Renderer *mainrenderer; //メイン画面用レンダラー

SDL_Texture *backtexture; //タイトル画面用テクスチャー
SDL_Texture *gametexture; //ゲーム画面用テクスチャー
SDL_Texture *metexture; //自キャラ画像用テクスチャー
SDL_Texture *bullettexture; //弾画像用テクスチャー
SDL_Texture *enemytexture; //敵画像用テクスチャー
SDL_Texture *enemytexture2; //敵画像用テクスチャー
SDL_Texture *enemytexture3; //敵画像用テクスチャー
SDL_Texture *cleartexture; //クリア画像用テクスチャー
SDL_Texture *test_enemytexture[enemynumber]; //敵画像用テクスチャー
SDL_Texture *fonttexture; //フォント用テクスチャー

SDL_Surface *mainsurface; //メイン画面用サーフェイス
SDL_Surface *backimg; //タイトル背景画像読み込み用サーフェイス
SDL_Surface *backimg2; //ゲーム画面背景画像読み込み用サーフェイス
SDL_Surface *meimg; //自キャラ画像読み込み
SDL_Surface *bulletimg; //弾画像読み込み
SDL_Surface *enemyimg; //敵キャラ画像読み込み用サーフェイス
SDL_Surface *enemyimg2; //敵キャラ画像読み込み用サーフェイス
SDL_Surface *enemyimg3; //敵キャラ画像読み込み用サーフェイス
SDL_Surface *clearimg; //クリア画面用画像読み込みサーフェイス
SDL_Surface *test_enemyimg[enemynumber]; //敵キャラ画像読み込み用サーフェイス
SDL_Surface *strings; //フォント用サーフェイス

SDL_Rect backimg_src; //ソースの矩形(画像用)
SDL_Rect backimg_dst; //貼り付け先の矩形(画像用)
SDL_Rect backimg2_src; //ソースの矩形(画像用)
SDL_Rect backimg2_dst; //貼り付け先の矩形(画像用)
SDL_Rect meimg_src; //ソースの矩形(画像用)
SDL_Rect meimg_dst; //貼り付け先の矩形(画像用)
SDL_Rect bulletimg_src; //弾の矩形
SDL_Rect bulletimg_dst; //弾の矩形(画像用)
SDL_Rect enemyimg_src; //敵の矩形
SDL_Rect enemyimg_dst; //敵の矩形(画像用)
SDL_Rect enemyimg2_src; //敵の矩形
SDL_Rect enemyimg2_dst; //敵の矩形(画像用)
SDL_Rect enemyimg3_src; //敵の矩形
SDL_Rect enemyimg3_dst; //敵の矩形(画像用)
SDL_Rect clearimg_src; //クリア画面の矩形(画像用)
SDL_Rect clearimg_dst; //クリア画面の矩形(画像用)
SDL_Rect font_src; //ソースの矩形(フォント用)
SDL_Rect font_dst; //貼り付け先の矩形(フォント用)

SDL_Rect test_enemyimg_src; //敵の矩形
SDL_Rect test_enemyimg_dst; //敵の矩形(画像用)

SDL_Event inputevent; //入力用
SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体
typedef struct { //フレームレート用の構造体
	uint now, //現在時間
		  wait, //待ち時間
		  leave; //経過時間
}timers;

typedef struct { //キー入力用の構造体
	Uint32	left, //左矢印
	 			right, //右矢印
				up, //上矢印
				down, //下矢印
				z, //発射キー(ジョイパッドの6番)
				start,
				time; //発射キーの押された時間
}inputkeys;

typedef struct { //自キャラ用の構造体
	Sint16	posX, //自キャラの位置X
				posY, //自キャラの位置Y
				flagB[bulletmax], //弾発射用フラグ
				bulletX[bulletmax], //弾の位置X
				bulletY[bulletmax]; //弾の位置Y
}mychara;
typedef struct { //敵キャラ用の構造体
	Sint16	posX, //敵キャラの位置X
				posY, //敵キャラの位置Y
				flagB[bulletmax], //弾発射用フラグ
				bulletX[bulletmax], //弾の位置X
				bulletY[bulletmax]; //弾の位置Y
}enemychara;

typedef struct { //敵キャラ用の構造体
	Sint16	posX, //敵キャラの位置X
				posY, //敵キャラの位置Y
				flagB[bulletmax], //弾発射用フラグ
				bulletX[bulletmax], //弾の位置X
				bulletY[bulletmax]; //弾の位置Y
}enemychara2;
	
typedef struct { //敵キャラ用の構造体
	Sint16	posX, //敵キャラの位置X
				posY, //敵キャラの位置Y
				flagB[bulletmax], //弾発射用フラグ
				bulletX[bulletmax], //弾の位置X
				bulletY[bulletmax]; //弾の位置Y
}enemychara3;
		
/**********関数のプロトタイプ宣言******/
void startup(); //メインとなるウィンドウを作成する関数
void frames(); //フレームレート用関数
void imageload(); //画像読み込み用
void drawtitle(Sint16 posX, Sint16 posY); //タイトル描画関数
void input(); //入力用関数
void startGame(); //タイトル画面->ゲーム画面用関数
void drawgame(Sint16 posX, Sint16 posY); //ゲーム画面の背景描画用
void reset(); //リセット用
void me_move(); //自キャラ移動計算用
void me_draw(); //自キャラ描画用
void me_bullet(); //自キャラ弾描画
void enemy_move(); //敵キャラ移動計算
void enemy_draw(); //敵キャラ描画
void judge(); //当たり判定
void drawclear(); //クリア画面描画
void Destroy(); //破棄用
#endif
