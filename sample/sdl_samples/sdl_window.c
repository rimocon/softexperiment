/*************************************************
sdl_window.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -L/usr/lib
備考：
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード

// メイン関数
int main(int argc, char* argv[]) {
	SDL_Surface *window; // ウィンドウ（画像）データへのポインタ

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

	SDL_WM_SetCaption("SDL Game", "Software Exp");	// ウィンドウ名などを指定
	SDL_WM_GrabInput(SDL_GRAB_ON);	// マウスやキーボードをウィンドウに閉じ込める
//	SDL_WM_IconifyWindow();	// ウィンドウをアイコン化（最小化）

	// ウィンドウをフルスクリーン化
	if (SDL_WM_ToggleFullScreen(window)==0){
		printf("failed to make full screen.\n");
		exit(-1);
	}

	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 128,0,0));	// ウィンドウの色を変更
	SDL_Flip(window);	// ウィンドウの描画（更新）
	SDL_Delay(5000);	// 5秒間スリープ（sleep(5);と同じ）

	SDL_WM_ToggleFullScreen(window);	// ウィンドウを元に戻す
	SDL_Delay(5000);


	SDL_Quit();	// SDLの利用終了
	return 0;
}

