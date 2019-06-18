/*************************************************
sdl_mutex.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -L/usr/lib
備考：
*************************************************/

#include "SDL/SDL.h"

int global_data=0; // 共有変数（グローバル変数）

// スレッド実行する関数１
int thread1(void* args){
	int i;
	int temp;

	// 共有変数に1〜50まで加算する処理
	for(i = 1; i <= 50; i++){
		temp = global_data;	// tempに共有変数を格納して計算
		SDL_Delay(10);	// 処理に10msかかることを意図的に設定
		temp = temp + i;	// 加算処理
		global_data = temp;	// 計算結果を共有変数に格納
	}
	return 0;
}

// スレッド実行する関数２
int thread2(void* args){
	int i;
	int temp;

	// 共有変数に51〜100まで加算する処理
	for(i = 51; i <= 100; i++){
		temp = global_data;
		SDL_Delay(10);
		temp = temp + i;
		global_data = temp;
	}
	return 0;
}

// スレッド実行する関数３
int thread3(void* args){
	int i;
	int temp;
	SDL_mutex *mtx = (SDL_mutex *)args;

	// 共有変数に1〜50まで加算する処理
	for(i = 1; i <= 50; i++){
		SDL_mutexP(mtx);	// Mutexをロックして、他のスレッドが共有変数にアクセスできないようにする
		// 後はthread1と同様の加算処理
		temp = global_data;
		SDL_Delay(10);
		temp = temp + i;
		global_data = temp;
		SDL_mutexV(mtx);	// Mutexをアンロックし、他のスレッドが共有変数にアクセスできるようにする
	}
	return 0;
}

// スレッド実行する関数４
int thread4(void* args){
	int i;
	int temp;
	SDL_mutex *mtx = (SDL_mutex *)args;

	// 共有変数に51〜100まで加算する処理
	for(i = 51; i <= 100; i++){
		SDL_mutexP(mtx);	// Mutexをロック
		temp = global_data;
		SDL_Delay(10);
		temp = temp + i;
		global_data = temp;
		SDL_mutexV(mtx);	// Mutexをアンロック
	}
	return 0;
}

// メイン関数
int main(int args,char *argp[]){
	SDL_Thread *thr1, *thr2;	// ２つのスレッドを用いる
	SDL_mutex *mtx;	// 相互排除（Mutex）を用いる

	// 相互排除（Mutex）なし
	thr1 = SDL_CreateThread(thread1, NULL);	// スレッドthr1を作成し、スレッド関数thread1を実行（引数なし）
	thr2 = SDL_CreateThread(thread2, NULL);	// スレッドthr2を作成し、スレッド関数thread2を実行（引数なし）

	SDL_WaitThread(thr1, NULL); // thr1の処理終了を待つ
	SDL_WaitThread(thr2, NULL); // thr2の処理終了を待つ

	printf("global_data = %d (=1+2+...100?)\n", global_data);	// 共有変数（加算結果）の値表示

	global_data = 0;	// 共有変数をリセット

	// 相互排除（Mutex）あり
	mtx = SDL_CreateMutex();	// Mutexを生成

	thr1 = SDL_CreateThread(thread3, mtx);	// スレッドthr1を再作成し、スレッド関数thread3を実行（引数としてMutexを渡す）
	thr2 = SDL_CreateThread(thread4, mtx);	// スレッドthr2を再作成し、スレッド関数thread4を実行（引数としてMutexを渡す）

	SDL_WaitThread(thr1, NULL); // thr1の処理終了を待つ
	SDL_WaitThread(thr2, NULL); // thr2の処理終了を待つ

	SDL_DestroyMutex(mtx); // Mutexを破棄

	printf("global_data = %d (=1+2+...100?)\n", global_data);	// 共有変数（加算結果）の値表示

	SDL_Quit();
	return 0;
}

