/*************************************************
  sdl_wii.c
  Programmed by S. Nishide (2019,5)

  コンパイルオプション：-lSDL2 -lSDL2_gfx -lcwiimote -D_ENABLE_TILT -D_ENABLE_FORCE -L/usr/lib
  備考：実行する時は./実行ファイル名 WiiリモコンのMACアドレス

 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>	// SDLを用いるために必要なヘッダファイル
#include <SDL2/SDL2_gfxPrimitives.h>	// 描画関係のヘッダファイル
#include <SDL2/SDL_mixer.h> //BGM用https://wingless-seraph.net/material/Burning-Cavern.mp3より
#include "wiimote.h"	// Wiiリモコンを用いるために必要なヘッダファイル
#include "wiimote_api.h"	// Wiiリモコンを用いるために必要なヘッダファイル

int null_thread(void *);

// メイン関数
int main(int argc, char* argv[]) {
	//変数宣言
	bool flag_jump = false; //ジャンプ検知用
	bool flag_blue = false; //青丸当たり判定検知用
	bool flag_red = false; //赤丸当たり判定検知用
	bool flag_spin = false; //スピンジャンプ判定用
	SDL_Window* window; // ウィンドウデータを格納する構造体
	SDL_Renderer* renderer; // 2Dレンダリングコンテキスト(描画設定)を格納する構造体

	SDL_Rect ground = {0, 400, 512, 112}; // 地面
	SDL_Rect sky = {10, 0, 492, 400};     // 空
	SDL_Rect wall1 = {0, 0, 10, 400};     // 左の壁
	SDL_Rect wall2 = {502, 0, 10, 400};   // 右の壁

	SDL_Rect chara = {100, 350, 20, 50};  // キャラの位置情報
	SDL_Rect chara_prev = chara;
	SDL_Rect chara_temp = chara;

	SDL_Thread *thread;
	SDL_atomic_t atm;

	/*****************************/
	/*******オーディオ関連********/
	/*****************************/
	Mix_Music *music_bgm; //BGMデータ格納用構造体
	Mix_Init(MIX_INIT_MP3); //オーディオ形式を指定して初期化
	//オーディオデバイスの初期化
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("failed to initialize SDL_mixer.\n");
		SDL_Quit();
		exit(-1);
	}
	//BGMの読み込み
	music_bgm= Mix_LoadMUS("bgm.mp3");
	if(music_bgm == NULL){
		printf("failed to load music.\n");
		Mix_CloseAudio();
		SDL_Quit();
		exit(-1);
	}


	/**************************************/
	/* Wiiリモコン関連の初期化（ここから） */
	/**************************************/

	// Wiiリモコンを用いるための構造体を宣言（初期化）
	wiimote_t wiimote = WIIMOTE_INIT;	// Wiiリモコンの状態格納用
	wiimote_report_t report = WIIMOTE_REPORT_INIT;	// レポートタイプ用

	// Wiiリモコンのスピーカで鳴らす音のデータ
	uint8_t sample[20] = {
		0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,
		0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c
	};

	// ***** Wiiリモコン処理 *****
	if (argc < 2) {	// Wiiリモコン識別情報がコマンド引数で与えられなければ
		printf("Designate the wiimote ID to the application.\n");
		exit(1);
	}

	// Wiiリモコンの接続（１つのみ）
	if (wiimote_connect(&wiimote, argv[1]) < 0) {	// コマンド引数に指定したWiiリモコン識別情報を渡して接続
		printf("unable to open wiimote: %s\n", wiimote_get_error());
		exit(1);
	}
	wiimote.led.one  = 1;	// WiiリモコンのLEDの一番左を点灯させる（接続を知らせるために）
	// wiimote.led.four  = 1;	// WiiリモコンのLEDの一番右を点灯させる場合

	// Wiiリモコンスピーカの初期化
	wiimote_speaker_init(&wiimote, WIIMOTE_FMT_S4, WIIMOTE_FREQ_44800HZ);

	// センサからのデータを受け付けるモードに変更
	wiimote.mode.acc = 1;

	/**************************************/
	/* Wiiリモコン関連の初期化（ここまで） */
	/**************************************/


	/**************************************/
	/* SDL関連の初期化（ここから）         */
	/**************************************/

	// SDL初期化
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// ウィンドウ生成・表示
	window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, 0);
	if(window == NULL){
		printf("Failed to create window.\n");
		exit(-1);
	}

	// 描画処理
	renderer = SDL_CreateRenderer(window, -1, 0); // 生成したウィンドウに対してレンダリングコンテキスト（RC）を生成
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // 生成したRCに描画色として白を設定
	SDL_RenderClear(renderer); // 生成したRCを白でクリア＝塗りつぶす（ただし，メモリに描画データを反映させただけなので，画面には表示されない）

	// 初期画面
	stringColor(renderer, 0, 0, "Press buttons 1 and 2 on the wiimote now to connect.\n", 0xffffffff);  
	SDL_RenderPresent(renderer); // 描画データを表示

	/**************************************/
	/* SDL関連の初期化（ここまで）         */
	/**************************************/

	// 地面・壁・空の描画
	SDL_SetRenderDrawColor(renderer, 153, 76, 0, 0);
	SDL_RenderFillRect(renderer, &ground);           // 地面を描画
	SDL_RenderFillRect(renderer, &wall1);            // 左の壁を描画 
	SDL_RenderFillRect(renderer, &wall2);            // 右の壁を描画
	SDL_SetRenderDrawColor(renderer, 200, 200, 255, 0);
	SDL_RenderFillRect(renderer, &sky);              // 空の描画

	// キャラの描画
	SDL_SetRenderDrawColor(renderer, 255, 150, 150, 0);
	SDL_RenderFillRect(renderer, &chara);              // キャラの描画
	SDL_RenderPresent(renderer); // 描画データを表示

	// 描画画面が暗くならないようにイベント処理をマルチスレッドで入れる
	SDL_AtomicSet(&atm, 1);
	thread = SDL_CreateThread(null_thread, "none", &atm);
	SDL_DetachThread(thread);

	/**************************************/
	/* 処理開始                            */
	/**************************************/

	Mix_PlayMusic(music_bgm,-1); //BGMを無限ループで再生

	// Wiiリモコンがオープン（接続状態）であればループ
	while (wiimote_is_open(&wiimote)) {

		// Wiiリモコンの状態を取得・更新する
		if (wiimote_update(&wiimote) < 0) {
			wiimote_disconnect(&wiimote);
			break;
		}

		// ***** Wiiのキー（ボタン）ごとに処理 *****
		// HOMEボタンが押された時(終了処理)
		if (wiimote.keys.home) {
			SDL_AtomicSet(&atm, 0);
			wiimote_speaker_free(&wiimote);	// Wiiリモコンのスピーカを解放
			wiimote_disconnect(&wiimote);	// Wiiリモコンとの接続を解除
		}

		/***********************************/
		/* 以下に処理を記述していく         */
		/***********************************/
		if(wiimote.keys.down){ //右ボタン
			chara.x += 2;
		}
		if(wiimote.keys.up){ //左ボタン
			chara.x -= 2;
		}
		if(wiimote.keys.left !=0 && flag_jump == false){ //下ボタン
			chara.y = 370;
			chara.h = 20; //しゃがむ
		}
		else if (wiimote.keys.left == 0 && flag_jump == false){
			chara.y = 350;
			chara.h = 40;
		}

		/*
			if(aiimote.keys.right){ //上ボタン
			chara.y -= 2;
			}
			if(wiimote.keys.one){
			chara.w = 40;
			}
			else{
			chara.w = 20;
			}
		 */
		if(wiimote.keys.one && wiimote.keys.down){ //1ボタン+右ボタン
			chara.x += 3;
		} 
		if(wiimote.keys.one && wiimote.keys.up){ //1ボタン+左ボタン
			chara.x -= 3; 
		}

		if(flag_jump == true){
			chara_temp.y = chara.y;
			chara.y += (chara.y - chara_prev.y)+1;
			chara_prev.y = chara_temp.y;
			printf("chara-prev = %d\n",chara.y-chara_prev.y);
			printf("chara.y= %d\n",chara.y);
			if(chara.y == 350){
				flag_jump = false;
			}
		}
		if(wiimote.keys.two != 0 && flag_jump == false){
			flag_jump = true;
			chara_prev.y = chara.y;
			chara.y -=20;
		}
		/**********************************************************************/
		/*キャラ横幅20,赤丸の横幅5で赤丸のx座標が25なのでx座標は0<=x<=30で当たり判定*/
		/*キャラの縦幅50,赤丸の縦幅5で赤丸のy座標が380なのでy座標は325<=y<=430で当たり判定*/
		if(0 <= chara.x  && chara.x <= 30 && 325 <= chara.y && chara.y <= 430 ){
			flag_red = true;
			flag_blue = false;
		}
		/*キャラ横幅20,青丸の横幅5で青丸のx座標が485なのでx座標は460<=x<=510で当たり判定*/
		/*キャラの縦幅50,青丸の縦幅5で青丸のy座標が380なのでy座標は325<=y<=430で当たり判定*/
		if(460 <= chara.x  && chara.x <= 510 && 325 <= chara.y && chara.y <= 430 ){
			flag_red = false;
			flag_blue = true;
		}
		if ( abs(126 - wiimote.axis.x) > 10 && abs(130 - wiimote.axis.y) > 10 && abs(153 - wiimote.axis.z) > 50 && flag_spin == false ){ //事前にprintfした値との差の二乗平均があるしきい値を超えたら
			printf("SPIN JUMP\n");
			flag_spin = true;
			chara_prev.y = chara.y;
			chara.y -=20;
		}
		if ( flag_spin == true ){
			chara_temp.y = chara.y;
			chara.y += (chara.y - chara_prev.y)+1;
			chara_prev.y = chara_temp.y;
			printf("chara-prev = %d\n",chara.y-chara_prev.y);
			printf("chara.y= %d\n",chara.y);
			if(chara.y == 351){
				flag_spin = false;
			}
		}
		//デバッグ用
		/*
			printf("flag_blue = %d\n",flag_blue);
			printf("flag_red= %d\n",flag_red);
		 */
		// 加速度の状態
		printf("AXIS x=%03d [%03d] y=%03d [%03d] z=%03d [%03d]\n", 
				wiimote.axis.x,
				wiimote.axis.x - 127,
				wiimote.axis.y,
				wiimote.axis.y - 127,
				wiimote.axis.z,
				wiimote.axis.z - 127);	// 分かりやすくするために-127をして±の値で表示


		/*
			printf("charax = %d\n",chara.x);
			printf("charay = %d\n",chara.y);
		 */
		/***********************************/
		/* 処理の記述ここまで               */
		/***********************************/

		// *****図形描画処理*****

		SDL_SetRenderDrawColor(renderer, 153, 76, 0, 0);
		SDL_RenderFillRect(renderer, &ground);           // 地面を描画
		SDL_RenderFillRect(renderer, &wall1);            // 左の壁を描画 
		SDL_RenderFillRect(renderer, &wall2);            // 右の壁を描画
		SDL_SetRenderDrawColor(renderer, 200, 200, 255, 0);
		SDL_RenderFillRect(renderer, &sky);              // 空の描画
		filledCircleColor(renderer, 25, 380, 5, 0xff0000ff); // 左に赤丸アイテム
		filledCircleColor(renderer, 485, 380, 5, 0xffff0000); // 右に青丸アイテム
		if(flag_blue == true){ //青丸に触れた時
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
			SDL_RenderFillRect(renderer, &chara);              // キャラの描画
		}
		else if(flag_red == true){ //赤丸に触れた時
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
			SDL_RenderFillRect(renderer, &chara);              // キャラの描画
		}
		else if(flag_blue == false && flag_red == false){
			SDL_SetRenderDrawColor(renderer, 255, 150, 150, 0);
			SDL_RenderFillRect(renderer, &chara);              // キャラの描画
		}
		SDL_RenderPresent(renderer);	// 描画データを表示
		//chara_prev = chara;
		//chara_temp = chara;

	}

	SDL_Delay(300);

	// 終了処理
	SDL_DestroyRenderer(renderer); // RCの破棄（解放）
	SDL_DestroyWindow(window); // 生成したウィンドウの破棄（消去）
	SDL_AtomicDecRef(&atm);
	SDL_Quit();
	return 0;
}

int null_thread(void *atm){

	SDL_Event event;

	while(1){
		if (SDL_PollEvent(&event)){
			SDL_Delay(100);
		}
		if (SDL_AtomicGet(atm) == 0 ){
			break;
		}
	}

}
