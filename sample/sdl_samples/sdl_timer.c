/*************************************************
sdl_timer.c
Programmed by H. Mitsuhara (2011.7-8)

コンパイルオプション：-lSDL -L/usr/lib
備考：
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード

// タイマーで呼び出されるコールバック関数
Uint32 callbackfunc(Uint32 interval, void *param){
	printf("Callback Function 1 - 1sec Passed\n");
	return interval;
}

// タイマーで呼び出されるコールバック関数
Uint32 callbackfunc2(Uint32 interval, void *param){
	int *times = (int*)param;	// 受け取った引数（パラメータ）をキャスト演算子でintのポインタ型に変換し、変数timesに格納
	printf("Callback Function 2 - 2sec Passed (%d times)\n", *times);
	return interval;
}

// タイマーで呼び出されるコールバック関数
Uint32 callbackfunc3(Uint32 interval, void *param){
	char *str = (char*)param;	// 受け取った引数（パラメータ）をキャスト演算子でcharのポインタ型（文字列）に変換し、変数strに格納
	printf("Callback Function 3 - %s\n", str);
	return interval;
}

// メイン関数
int main(int argc, char* argv[]) {

	SDL_Surface *window; // ウィンドウ（画像）データへのポインタ
	SDL_Event event;	// SDLによるイベントを検知するための構造体

	SDL_TimerID timer_id1, timer_id2, timer_id3;	//　タイマ割り込みを行うためのタイマのID

	int times=0;	// Whileループの回数
	char str[]="3sec passed";	// コールバック関数に引数として渡す文字列

	// SDL初期化
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}
	// ウィンドウ生成（800*600、1677万色）
	if((window = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}
	// タイマの作成・設定（タイマIDの格納）
	timer_id1=SDL_AddTimer(1000, callbackfunc, NULL);	// 1秒ごとにコールバック関数を呼び出す（引数なし）
	timer_id2=SDL_AddTimer(2000, callbackfunc2, &times);	// 2秒ごとにコールバック関数を呼び出す（int型変数のアドレスを引数として渡す）
	timer_id3=SDL_AddTimer(3000, callbackfunc3, &str);	// 3秒ごとにコールバック関数を呼び出す（配列char型変数のアドレスを引数として渡す）

	// printf("timer ID=%d, %d, %d\n", timer_id1, timer_id2, timer_id3);

	// SDL_GetTicks関数を用いる時間管理
	Uint32 next_frame=SDL_GetTicks();	// SDLライブラリの初期化からの経過ミリ秒数を取得

	// 無限ループ		
	while(1){
		times++;
		// イベント検知
		if(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				printf("Terminated.\n");

				// タイマ削除
				SDL_RemoveTimer(timer_id1);
				SDL_RemoveTimer(timer_id2);
				SDL_RemoveTimer(timer_id3);

				SDL_Quit();	// SDLの利用終了
				exit(0);
				break;
			}
		}
		// 1秒ごとに表示
 		if (SDL_GetTicks() >= next_frame) {	// 現在の経過時間が前より1秒以上経過していたら
			printf("Time Passed = %d\n", SDL_GetTicks());
			next_frame += 1000;	// 現在の経過時間に1000ms（1秒）をプラスして格納
		}
	}

	return 0;
}

