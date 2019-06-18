/*************************************************
sdl_thread.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -L/usr/lib
備考：
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>

int global_data=0;	// メイン関数とスレッドで共有する変数（共有変数、グローバル変数）

// タイマーで呼び出されるコールバック関数
Uint32 callbackfunc1(Uint32 interval, void *args){
	global_data++;	// 1秒ごとに1増やす
	printf("Thread1 - 1sec Passed (global_data = %d)\n", global_data);	// 共有変数の値を表示
	return interval;
}

Uint32 callbackfunc2(Uint32 interval, void *args){
	global_data+=2;	// 2秒ごとに2増やす
	printf("Thread2 - 2sec Passed (global_data = %d)\n", global_data);
	return interval;
}

// スレッド実行する関数１
int thread1(void* args){
	SDL_TimerID timer_id;	//　タイマ割り込みを行うためのタイマのID

	// タイマの作成・設定（タイマIDの格納）
	timer_id=SDL_AddTimer(1000, callbackfunc1, NULL);	// 1秒ごとにコールバック関数を呼び出す（引数なし）

	// global_data<30の間は繰り返す
	while(global_data < 30){}
	SDL_RemoveTimer(timer_id);	// タイマの削除

	return 0;
}

// スレッド実行する関数２
int thread2(void* args){
	SDL_TimerID timer_id;
	timer_id=SDL_AddTimer(2000, callbackfunc2, NULL);	// 2秒ごとにコールバック関数を呼び出す（引数なし）
	while(global_data < 30){}
	SDL_RemoveTimer(timer_id);
	return 0;
}

// スレッド実行する関数３
int thread3(void* args){
	SDL_Event event;

	while(global_data < 30){
		// イベントを取得したなら
		if(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押された時
				global_data--;	// マウスのボタンが押されるたびに1減らす
				printf("Thread3 - Mouse Button Down (global_data = %d)\n", global_data);
				break;
			}
		}
	}
	return 0;
}


int main(int args,char *argp[]){
	SDL_Surface *window; // ウィンドウ（サーフェイス）データへのポインタ
	SDL_Thread *thr1, *thr2, *thr3;	// ３つのスレッドを用いる

	// SDL初期化
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}
	// ウィンドウ生成
	if((window = SDL_SetVideoMode(320, 240, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	// スレッドを作成・実行
	thr1 = SDL_CreateThread(thread1, NULL); // スレッドthr1を作成し、スレッド関数thread1を実行（引数なし）
	if (thread1 == NULL ) {
		printf("Unable to create thread1: %s\n", SDL_GetError());
	        exit(-1);
	}
	thr2 = SDL_CreateThread(thread2, NULL); // スレッドthr2を作成し、スレッド関数thread2を実行（引数なし）
	thr3 = SDL_CreateThread(thread3, NULL); // スレッドthr3を作成し、スレッド関数thread3を実行（引数なし）

	printf("Thread ID (thr1, thr2, thr3) = (%d, %d, %d)\n", SDL_GetThreadID(thr1), SDL_GetThreadID(thr2), SDL_GetThreadID(thr3));	// 共有変数の値を表示

	printf("global_data = %d\n", global_data);	// 共有変数の値を表示

	// 各スレッドが終了するまでmain関数の処理を中断
	SDL_WaitThread(thr1, NULL); // thr1の処理終了を待つ
	SDL_WaitThread(thr2, NULL); // thr2の処理終了を待つ
	SDL_WaitThread(thr3, NULL); // thr3の処理終了を待つ

	SDL_Quit();
	return 0;
}


