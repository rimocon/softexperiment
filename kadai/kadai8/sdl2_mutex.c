/*************************************************
  sdl2_mutex.c
  Programmed by X. Kang (2018,5)
  Edited by X. Kang (2019,5)

  コンパイルオプション：-lm -lSDL2 -lSDL2_gfx -lcwiimote -L/usr/lib
  備考：

 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>    // SDLを用いるために必要なヘッダファイル
#include <SDL2/SDL2_gfxPrimitives.h>  // 描画関係のヘッダファイル

#include <libcwiimote/wiimote.h>    // Wiiリモコンを用いるために必要なヘッダファイル
#include <libcwiimote/wiimote_api.h>    // Wiiリモコンを用いるために必要なヘッダファイル

// 注意：以下の一部の変数はスレッド実行する関数に使えるために
// 　　　グローバル変数になりました

// Wiiリモコンを用いるための構造体を宣言（初期化）
wiimote_t wiimote = WIIMOTE_INIT;   // Wiiリモコンの状態格納用

SDL_Event event; // SDLによるイベントを検知するための構造体

SDL_Window * window; // ウィンドウデータを格納する構造体
SDL_Renderer * renderer; // 2Dレンダリングコンテキスト（描画設定）を格納する構造体

SDL_Rect ground = { 0, 400, 512, 112}; // 地面の描画
SDL_Rect sky = { 10, 0, 492, 400}; // 空の描画
SDL_Rect wall1 = { 0, 0, 10, 400}; // 左の壁描画
SDL_Rect wall2 = { 502, 0, 10, 400}; // 右の壁描画

SDL_Rect mario_loc = { 100, 350, 20, 50 }; // マリオ（長方形）の座標
int mario_step = 8; // マリオステップの長さ
int interval = 10; // マリオ描画の時間間隔

// マリオ右に動く
void mario_right()
{
	if(mario_loc.x + mario_loc.w + mario_step <= wall2.x)
	{
		mario_loc.x += mario_step;
	}
}

// マリオ左に動く
void mario_left()
{
	if(mario_loc.x - mario_step >= wall1.w)
	{
		mario_loc.x -= mario_step;
	}
}

// マリオを広げる
void mario_wide()
{
	int max_w = wall2.x - mario_loc.x;
	if(max_w >= 40)
	{
		mario_loc.w = 40;
	}
	else
	{
		mario_loc.w = max_w;
	}
}

// マリオを回復
void mario_narrow()
{
	mario_loc.w = 20;
}

// Wiiボタン値によって，マリオの座標を計算
// スレッド実行する関数
int wii_func(void * args)
{
	///////////////////////////////////////
	// STEP. 7                           //
	// STEP_7_BEGINからSTEP_7_ENDまでの  //
	// コードのコメンドを外す            //
	///////////////////////////////////////

	// ***** STEP_7_BEGIN *****
	 SDL_mutex *mtx = (SDL_mutex *)args; // 引数型はmtxに変更
	// ***** STEP_7_END *****

	// Wiiリモコンがオープン（接続状態）であればループ
	while(wiimote_is_open(&wiimote))
	{
		///////////////////////////////////////
		// STEP. 2                           //
		// STEP_1で指示されたコードを        //
		// STEP_2_BEGINからSTEP_2_ENDまでに  //
		// に移動する                        //
		///////////////////////////////////////


		// ***** STEP_2_BEGIN *****
		// ***** STEP_1_BEGIN *****
		// Wiiリモコンの状態を取得・更新する
		if(wiimote_update(&wiimote))
		{
			///////////////////////////////////////
			// STEP. 8                           //
			// STEP_8_BEGINからSTEP_8_ENDまでの  //
			// コードのコメンドを外す            //
			///////////////////////////////////////

			// ***** STEP_8_BEGIN *****
			SDL_LockMutex(mtx); // Mutexをロックして、他のスレッドが共有変数にアクセスできないようにする
			// ***** STEP_8_END *****

			// ***** Wiiのキー（ボタン）ごとに処理 *****
			// Wii Homeボタンが押された時
			if(wiimote.keys.home)
			{
				wiimote_disconnect(&wiimote); // Wiiリモコンとの接続を解除
			}

			// 以下に処理を記述していく
			// Wii Downボタンが押された時
			if(wiimote.keys.down)
			{
				mario_right();
			}

			// Wii Upボタンが押された時
			if(wiimote.keys.up)
			{
				mario_left();
			}

			// Wii Oneボタンが押された時
			if(wiimote.keys.one)
			{
				mario_wide();
			}
			else{
				mario_narrow();
			}

			///////////////////////////////////////
			// STEP. 9                           //
			// STEP_9_BEGINからSTEP_9_ENDまでの  //
			// コードのコメンドを外す            //
			///////////////////////////////////////

			// ***** STEP_9_BEGIN *****
			SDL_UnlockMutex(mtx); // Mutexをアンロックし、他のスレッドが共有変数にアクセスできるようにする
			// ***** STEP_9_END *****

			SDL_Delay(interval);
		}
		else
		{
			wiimote_disconnect(&wiimote);
		}
		// ***** STEP_1_END *****

		// ***** STEP_2_END *****
	}
	return 0;
}

// キーボードの押されたキーによって，マリオの座標を計算
// スレッド実行する関数
int keyboard_func(void * args)
{
	///////////////////////////////////////
	// STEP. 7                           //
	// STEP_7_BEGINからSTEP_7_ENDまでの  //
	// コードのコメンドを外す            //
	///////////////////////////////////////

	// ***** STEP_7_BEGIN *****
	SDL_mutex *mtx = (SDL_mutex *)args; // 注意：引数はmtx
	// ***** STEP_7_END *****

	// Wiiリモコンがオープン（接続状態）であればループ
	while(wiimote_is_open(&wiimote))
	{
		// ***** キーボード処理の初め *****
		// ***** キーボード処理の初め *****
		// キーボードの状態を取得・更新する
		if(SDL_PollEvent(&event))
		{
			///////////////////////////////////////
			// STEP. 8                           //
			// STEP_8_BEGINからSTEP_8_ENDまでの  //
			// コードのコメンドを外す            //
			///////////////////////////////////////

			// ***** STEP_8_BEGIN *****
			SDL_LockMutex(mtx); // Mutexをロックして、他のスレッドが共有変数にアクセスできないようにする
			// ***** STEP_8_END *****

			switch(event.type)
			{
				case SDL_KEYDOWN: // キーボードが押された時
					switch(event.key.keysym.sym)
					{
						case SDLK_RIGHT: // 右矢印キーが押された時
							mario_right();
							break;
						case SDLK_LEFT: // 左矢印キーが押された時
							mario_left();
							break;
						case SDLK_SPACE: // スペースキーが押された時
							mario_wide();
							break;
						case SDLK_ESCAPE: // Escキーが押された時
							wiimote_disconnect(&wiimote);
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP: // キーボードが放された時
					switch(event.key.keysym.sym)
					{
						case SDLK_SPACE: // スペースキーが放された時
							mario_narrow();
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}

			///////////////////////////////////////
			// STEP. 9                           //
			// STEP_9_BEGINからSTEP_9_ENDまでの  //
			// コードのコメンドを外す            //
			///////////////////////////////////////

			// ***** STEP_9_BEGIN *****
			SDL_UnlockMutex(mtx); // Mutexをアンロックし、他のスレッドが共有変数にアクセスできるようにする
			// ***** STEP_9_END *****

			SDL_Delay(interval);
		}
		// ***** キーボード処理の終わり*****
		// ***** キーボード処理の終わり*****
	}
	return 0;
}

int min_flips = -1; // 1秒あたりの最小描画回数
// 時間間隔(flip_interval)あたりの最小描画回数を計算
Uint32 min_flips_callback(Uint32 flip_interval, void * param)
{
	int flips = * (int *)param;
	if(min_flips == -1 || flips < min_flips)
	{
		min_flips = flips;
	}
	// 描画回数を表示する
	fprintf(stderr, "Flips per sec: %d\n", flips);
	* (int *)param = 0;
	return flip_interval;
}

// メイン関数
int main(int argc, char* argv[])
{

	///////////////////////////////////////
	// STEP. 3                           //
	// STEP_3_BEGINからSTEP_3_ENDまでの  //
	// コードのコメンドを外す            //
	///////////////////////////////////////

	// ***** STEP_3_BEGIN *****
	SDL_Thread * wii_thread; // wii_threadを用いる 
	SDL_Thread * keyboard_thread;// keyboard_threadを用いる
	// ***** STEP_3_END *****

	///////////////////////////////////////
	// STEP. 6                           //
	// STEP_6_BEGINからSTEP_6_ENDまでの  //
	// コードのコメンドを外す            //
	///////////////////////////////////////

	// ***** STEP_6_BEGIN *****
	// 相互排除（Mutex）あり
	SDL_mutex *mtx = SDL_CreateMutex(); // 相互排除（Mutex）を用いる
	// ***** STEP_6_END *****

	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// ウィンドウ生成
	if((window = SDL_CreateWindow("Multi-Thread Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, 0)) == NULL)
	{
		printf("failed to create window.\n");
		exit(-1);
	}

	// レンダリングコンテキスト（RC）作成
	if((renderer = SDL_CreateRenderer(window, -1, 0)) == NULL)
	{
		printf("failed to create renderer.\n");
		exit(-1);
	}

	// 初期画面
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 生成したRCに描画色として青を設定
	SDL_RenderClear(renderer); // 設定色でRCをクリア
	stringColor(renderer, 0, 0, "Press buttons 1 and 2 on the wiimote now to connect.\n", 0xffffffff); // 文字列を描画
	SDL_RenderPresent(renderer); // 描画データを表示

	// ***** Wiiリモコン処理 *****
	if(argc < 2)
	{ // Wiiリモコン識別情報がコマンド引数で与えられなければ
		printf("Designate the wiimote ID to the application.\n");
		exit(1);
	}

	// Wiiリモコンの接続（１つのみ）
	// コマンド引数に指定したWiiリモコン識別情報を渡して接続
	if(wiimote_connect(&wiimote, argv[1]) < 0)
	{
		printf("unable to open wiimote: %s\n", wiimote_get_error());
		exit(1);
	}

	wiimote.led.one = 1; // WiiリモコンのLEDの一番左を点灯させる（接続を知らせるために）

	wiimote.mode.acc = 1; // センサからのデータを受け付けるモードに変更

	// 地面・壁の描画
	SDL_SetRenderDrawColor(renderer, 153, 76, 0, 255);
	SDL_RenderFillRect(renderer, &ground);
	SDL_RenderFillRect(renderer, &wall1);
	SDL_RenderFillRect(renderer, &wall2);

	///////////////////////////////////////
	// STEP. 4                           //
	// STEP_4_BEGINからSTEP_4_ENDまでの  //
	// コードのコメンドを外す            //
	// 注意：引数はNULLからmtxに変わった //
	///////////////////////////////////////

	// ***** STEP_4_BEGIN *****
	// スレッドを作成・実行
	wii_thread = SDL_CreateThread(wii_func, "wii_thread", mtx); // wii_threadを作成し、スレッド関数wii_funcを実行（引数mtx）
	keyboard_thread = SDL_CreateThread(keyboard_func, "keyboard_func", mtx); // keyboard_threadを作成し、スレッド関数keyboard_funcを実行（引数mtx）

	// ***** STEP_4_END *****

	int flips = 0; // 1秒あたりの描画回数
	SDL_TimerID timer_id = SDL_AddTimer(1000, min_flips_callback, &flips); // 1秒あたりの最小描画回数を計算

	// Wiiリモコンがオープン（接続状態）であればループ
	while(wiimote_is_open(&wiimote))
	{
		///////////////////////////////////////
		// STEP. 1                           //
		// STEP_1_BEGINからSTEP_1_ENDまでの  //
		// コードをStep. 2で指示されたところ //
		// に移動する                        //
		///////////////////////////////////////
		// *****図形描画処理*****
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &sky); // 空を白色にする
		SDL_SetRenderDrawColor(renderer, 255, 150, 150, 255);
		SDL_RenderFillRect(renderer, &mario_loc); // マリオを描画
		SDL_RenderPresent(renderer); // 描画データを表示
		flips += 1; // 表示回数

		SDL_Delay(interval); // 一時的に待つ
	}

	///////////////////////////////////////
	// STEP. 5                           //
	// STEP_5_BEGINからSTEP_5_ENDまでの  //
	// コードのコメンドを外す            //
	///////////////////////////////////////

	// ***** STEP_5_BEGIN *****
	// 各スレッドが終了するまでmain関数の処理を中断
	SDL_WaitThread(wii_thread, NULL);// wii_threadの処理終了を待つ
	SDL_WaitThread(keyboard_thread, NULL);// keyboard_threadの処理終了を待つ
	// ***** STEP_5_END *****

	///////////////////////////////////////
	// STEP. 10                          //
	// STEP_10_BEGINからSTEP_10_ENDまでの//
	// コードのコメンドを外す            //
	///////////////////////////////////////

	// ***** STEP_10_BEGIN *****
	SDL_DestroyMutex(mtx); // Mutexを破棄
	// ***** STEP_10_END *****

	// 終了処理
	SDL_DestroyRenderer(renderer); // RCの破棄（解放）
	SDL_DestroyWindow(window); // 生成したウィンドウの破棄（消去）
	SDL_Quit(); // SDL使用の終了

	printf("Flips per sec (minimum): %d\n", min_flips);
	return 0;
}
