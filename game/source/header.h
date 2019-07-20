#ifndef HEAD_H
#define HEAD_H


#define bulletmax 3
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
bool titledraw; //一回だけ描画する用
bool gamedraw; //一回だけ描画する用

SDL_Window *mainwindow; //メイン画面用
SDL_Surface *mainsurface; //メイン画面用サーフェイス
SDL_Renderer *mainrenderer; //メイン画面用レンダラー
SDL_Texture *maintexture; //メイン画面用テクスチャー
SDL_Texture *metexture; //自キャラ画像用テクスチャー
SDL_Surface *img; //タイトル背景画像読み込み
SDL_Surface *meimg; //自キャラ画像読み込み
SDL_Surface *gameimg; //ゲーム画面背景画像読み込み

SDL_Rect img_src; //ソースの矩形(画像用)
SDL_Rect img_dst; //貼り付け先の矩形(画像用)
SDL_Rect meimg_src; //ソースの矩形(画像用)
SDL_Rect meimg_dst; //貼り付け先の矩形(画像用)
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
				time; //発射キーの押された時間
}inputkeys;

typedef struct { //自キャラ用の構造体
	Sint16	posX, //自キャラの位置X
				posY, //自キャラの位置Y
				flagB[bulletmax], //弾発射用フラグ
				bulletX[bulletmax], //弾の位置X
				bulletY[bulletmax]; //弾の位置Y
}mychara;
				
/**********関数のプロトタイプ宣言******/
void startup(); //メインとなるウィンドウを作成する関数
int quits(); //終了のための関数
void frames(); //フレームレート用関数
void imageload(); //画像読み込み用
void drawtitle(Sint16 posX, Sint16 posY); //タイトル描画関数
void input(); //入力用関数
void startGame(); //タイトル画面->ゲーム画面用関数
void drawgame(Sint16 posX, Sint16 posY); //ゲーム画面の背景描画用
void reset(); //リセット用
void me_move(); //自キャラ移動計算用
void me_draw(); //自キャラ描画用
void Destroy(); //破棄用
#endif
