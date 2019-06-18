/*************************************************
sdl_draw.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lSDL_gfx -lSDL_ttf -L/usr/lib
備考：
　TrueTypeフォント（*.ttf）は/usr/share/fontsにあると思います．
　フリーのTrueTypeフォントをダウンロードして使ってもよいでしょう．
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>	// SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_gfxPrimitives.h>	// gfxによる図形処理をするために必要なヘッダファイルをインクルード
#include <SDL/SDL_ttf.h>	// TrueTypeフォントを表示するために必要なヘッダファイルをインクルード

// メイン関数
int main(int argc, char* argv[]) {

	SDL_Surface *window, *strings; // ウィンドウ（画像）データ、及び、文字列（画像）へのポインタ
	SDL_Rect a = { 0, 0, 1, 1 }; // 点を描画するための矩形領域指定
	TTF_Font* font;	// TrueTypeフォントデータへのポインタ
	SDL_Color white = {0xFF, 0xFF, 0xFF};	// フォントの色を指定するための構造体（白色）

	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}
	// ウィンドウ生成（800*600、1677万色）
	if((window = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	// *****図形描画処理*****
	SDL_FillRect(window, NULL, 0xffffffff);	// ウィンドウの（背景）色を変更（白色の矩形で塗りつぶし）
	pixelColor(window, 100, 100, 0xff0000ff);	// 赤で(100,100)に点を描画
	hlineColor(window, 100, 200, 200, 0xff0000ff);	// 赤で(100,200)から(200,200)に水平線を描画

	SDL_UpdateRect(window, 0, 0, 0, 0);	// 図形表示（サーフェイス全体の描画データをウィンドウに反映）
	SDL_LockSurface(window);	// ウィンドウへの描画をロック（禁止）する．

	rectangleColor(window, 300, 100, 400, 200, 0x00ff00ff);	// 四角形を描画
	boxColor(window, 300, 200, 400, 300, 0x00ff0000);	// 四角形を描画（塗りつぶす）
	lineColor(window, 400, 100, 500, 200, 0x00ff00);	// 直線（2点間）を描画
	circleColor(window, 300, 200, 10, SDL_MapRGB(window->format, 0,0,255));	// 円を描画
	filledCircleColor(window, 400, 200, 10, 0x0000ffff);	// 円を描画（青で塗りつぶす）
	filledCircleColor(window, 400, 250, 10, 0x0000ff00);	// 円を描画（青で塗りつぶしているが、透明）
	filledCircleColor(window, 400, 300, 10, SDL_MapRGBA(window->format, 0,0,255,255));	// 円を描画（青？で塗りつぶす）
	pieColor(window, 100, 400, 100, 0, -90, SDL_MapRGB(window->format, 0x00,0xff,0xff));	// 半円を描画
	trigonColor(window, 0, 0, 100, 0, 50, 50, 0xffffff);	// 三角形を描画

	int i;
	for(i=0; i<=100; i++){
		a.x=i; a.y=i*2;
		SDL_FillRect(window, &a, 0xff0000ff);	// ウィンドウに点を打つ
	}

	stringColor(window, 300, 50, "TEST", 0x00ffffff);	// 文字列"TEST"を描画

	SDL_Flip(window);	// 画面に図形を表示（反映）
	SDL_Delay(5000);	// 5秒待機


	// *****文字列描画処理*****
	TTF_Init();	// TrueTypeフォントを用いるための初期化

	font = TTF_OpenFont("kochi-gothic-subst.ttf", 24); // kochi-gothic-substフォントを24ポイントで使用（読み込み）

	SDL_FillRect(window, NULL, 0x00000000);	// ウィンドウの（背景）色を変更（黒色の矩形で塗りつぶし）

	// 描画文字の作成と格納
	strings = TTF_RenderUTF8_Blended(font, "こんにちは、Hello!\nうまく表示されましたか？改行ってどうするんだろうね…", white);	// 白色のkochi-gothic-substフォントで、文字列を作成

	// 文字列の座標とサイズ（sdl_image.cを参考にしてください）
	SDL_Rect src_rect = { 0, 0, strings->w, strings->h }; // コピー元文字列のコピーする一領域の座標とサイズ（x, y, w, h）（strings->wとstrings->hにはそれぞれ文字列の横幅と高さが格納されます）
	SDL_Rect dst_rect = { 0, 0 }; // 画像のコピー先の座標（x, y）

	SDL_BlitSurface(strings, &src_rect, window, &dst_rect);	// ウィンドウに文字列を貼り付ける

	SDL_Flip(window);	// 画面に文字を表示（反映）
	SDL_Delay(5000);

	SDL_UnlockSurface(window);	// ウィンドウへの描画をロック（禁止）を解除する

	TTF_CloseFont(font);	// 使用したフォントを終了する
	TTF_Quit();	// TTFライブラリの使用を終了

	SDL_Quit();	// SDLの利用終了

	return 0;
}

