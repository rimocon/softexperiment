#ifndef HEAD_H
#define HEAD_H

/******C言語のヘッダファイル****/
#include <math.h> //数学的計算用
#include <stdarg.h> //可変な型や引数をとることができる
#include <stdio.h> //標準
#include <stdlib.h> //標準マクロなど(NULLとか)
#include <string.h> //文字列を扱いやすくする
#include <time.h> //時間用

/************SDL関連************/
#include <SDL2/SDL.h> //SDLを使う
#include <SDL2/SDL2_gfxPrimitives.h> //SDLで図形描画
#include <SDL2/SDL_ttf.h> //SDLで文字描画
#include <SDL2/SDL_image.h> //SDLで画像を扱う
#include <SDL2/SDL_mixer.h> //SDLで音楽を扱う

/*********グローバル変数***********/
int games; //ゲームの現在の状態を取得するために使う
SDL_Window *mainwindow; //メイン画面用
SDL_Surface *mainsurface; //メイン画面用サーフェイス
SDL_Renderer *mainrenderer; //メイン画面用レンダラー
SDL_Texture *maintexture; //メイン画面用テクスチャー
SDL_Surface *img; //画像読み込み

SDL_Rect img_src; //ソースの矩形(画像用)
SDL_Rect img_dst; //貼り付け先の矩形(画像用)
SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体
typedef struct { //フレームレート用の構造体
	uint now, //現在時間
		  wait, //待ち時間
		  leave; //経過時間
}timers;

typedef struct { //キー入力用の構造体
	uint32 left, //左矢印
			 right, //右矢印
			 up, //上矢印
			 down. //下矢印
			 time; //発射キーの押された時間
}inputkeys;


/**********関数のプロトタイプ宣言******/
void startup(); //メインとなるウィンドウを作成する関数
int quits(); //終了のための関数
void frames(); //フレームレート用関数
void background(); //背景描画用
void imageload(); //画像読み込み用
void drawtitle(Sint16 posX, Sint16 posY); //タイトル描画関数
void input(); //入力用関数
#endif
