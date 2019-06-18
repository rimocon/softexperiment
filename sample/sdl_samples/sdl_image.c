/*************************************************
sdl_image.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lSDL_image -L/usr/lib
備考：画像は適当に用意してください（白背景、アルファチャンネルで背景透過した画像等）．
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード

// メイン関数
int main(int argc, char* argv[]) {
	SDL_Surface *window; // ウィンドウ（画像）データへのポインタ
	SDL_Surface *image, *image2;  // 画像データへのポインタ

	// 画像の座標とサイズ
	SDL_Rect src_rect = { 300, 0, 200, 200 }; // コピー元画像のコピーする一領域の座標とサイズ（x, y, w, h）
	SDL_Rect dst_rect = { 200, 100 }; // 画像のコピー先の座標（x, y）
	SDL_Rect src_rect2 = { 0, 0, 100, 100 };
	SDL_Rect dst_rect2 = { 0, 0 };
	SDL_Rect src_rect3 = { 0, 0, 150, 150 };
	SDL_Rect dst_rect3 = { 100, 100 };

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

	// ウィンドウの情報を表示
	printf("Window - Colors = %d bpp \n", window->format->BitsPerPixel);
	printf("Window - Color Key = %d \n", window->format->colorkey);
	printf("Window - Alpha Value = %d \n", window->format->alpha);

	// BMP画像
	image=SDL_LoadBMP("test.bmp");	// BMP画像をサーフェイス（フレームバッファ）に読み込み
	SDL_BlitSurface(image, NULL, window, NULL);	// 読み込んだ画像（全体）をウィンドウに貼り付ける

	// BMP画像以外
	image=IMG_Load("test.jpg");	//
	SDL_SetAlpha(image, SDL_SRCALPHA, 128);	// test.jpgに対して半透明化
	SDL_BlitSurface(image, &src_rect, SDL_GetVideoSurface(), &dst_rect);	// ウィンドウ（ここではSDL_GetVideoSurface関数で指定）に画像を貼り付ける

	image=IMG_Load("test2.png");	// PNG画像をサーフェイス（フレームバッファ）読み込み
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 255, 255));	// 白を透過色に指定
	image=SDL_DisplayFormat(image);	// 透過を反映させる
	SDL_BlitSurface(image, &src_rect2, SDL_GetVideoSurface(), &dst_rect2);	// ウィンドウに画像を貼り付ける

	image=IMG_Load("test3.png");
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, -1);	// アルファチャンネル透過を指定（第3引数を-1にすればアルファチャンネルで透過）※SDL_SetColorKey関数がなくてもそのまま透過するみたい
	SDL_BlitSurface(image, &src_rect3, SDL_GetVideoSurface(), &dst_rect3);	// ウィンドウに画像を貼り付ける（アルファチャンネル透過の場合、SDL_DisplayFormat関数は必要なし）

	// 画像の情報を表示
	printf("Image - Colors = %d bpp \n", image->format->BitsPerPixel);
	printf("Image - Color Key = %d \n", image->format->colorkey);
	printf("Image - Alpha Value = %d \n\n", image->format->alpha);
	printf("Image2 - Colors = %d bpp \n", image2->format->BitsPerPixel);
	printf("Image2 - Color Key = %d \n", image2->format->colorkey);
	printf("Image2 - Alpha Value = %d \n", image2->format->alpha);

	SDL_Flip(window);	// 画面にアニメーションを表示（反映）

	SDL_Delay(5000);	// 5秒間スリープ

	SDL_FreeSurface(window);	// ウィンドウ（サーフェイス）の解放
	SDL_FreeSurface(image);	// 画像（サーフェイス）の解放

	SDL_Quit();	// SDLの利用終了
	return 0;
}

