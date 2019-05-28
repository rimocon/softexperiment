//ヘッダファイルをインクルード

#include <stdio.h>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
// グローバル変数
SDL_Renderer* renderer; // レンダラー(描画対象）
SDL_Texture* texture; // キャラクタ用テクスチャ
SDL_Event event; // イベントデータを格納する構造体

// タイマーで呼び出されるコールバック関数
Uint32 ExecuteAnimation(Uint32 interval, void *param){
	int *frame = param; // 受け取ったフレーム番号
	SDL_Rect src_rect = {0, 0, 200, 325}; // 転送元画像の領域（初期値を設定）

	src_rect.x = *frame *200; // 転送元画像の左上x座標（0 -> 40 -> 80 -> 120 -> 0）を指定
	SDL_RenderCopy(renderer, texture, &src_rect, NULL); // 転送元画像（テクスチャ）の指定した領域をレンダラーに転送

	// コールバック関数が呼び出されたことをユーザ定義イベントとしてイベントキューに追加
	event.type = SDL_USEREVENT; // イベントの種類をユーザ定義イベントにする
	event.user.code = 0; // 定義したイベントのコードに0を設定
	SDL_PushEvent(&event); // イベントキューにユーザ定義イベントを追加

	return interval;	// コールバックが呼び出される間隔を返す（必須）
}

// メイン関数
int main(int argc, char* argv[]) {
	SDL_Window *window; // ウィンドウデータ
	SDL_Surface *image; // 画像データ（サーフェイス）へのポインタ
	SDL_TimerID timer_id; // タイマ割り込みを行うためのタイマのID
	Mix_Music *music_tell,*music_unknownbrain; // BGMデータ格納用構造体
	int frame = 0; // フレーム（コマ）番号（0で初期化）

	// SDL初期化
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	IMG_Init(IMG_INIT_PNG); // 描画する画像形式を指定して初期化
	Mix_Init(MIX_INIT_MP3); //オーディオ形式を指定して初期化

	//オーディオデバイスの初期化
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("failed to initialize SDL_mixer.\n");
		SDL_Quit();
		exit(-1);
	}
	//BGMの読み込み
	music_tell = Mix_LoadMUS("tell.mp3");
	music_unknownbrain = Mix_LoadMUS("unknownbrain.mp3");
		if(music_tell == NULL && music_unknownbrain == NULL){
			printf("failed to load music.\n");
			Mix_CloseAudio();
			SDL_Quit();
			exit(-1);
		}

	// ウィンドウ生成・表示
	window = SDL_CreateWindow("kadai1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if(window == NULL){
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	// レンダラー（レンダリングコンテキスト）作成
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); // SDL_RENDERER_SOFTWAREを指定

	// アニメーション用画像読み込み
	image = IMG_Load("character_animation2.png"); // 画像の読み込み
	texture = SDL_CreateTextureFromSurface(renderer, image); // 読み込んだ画像からテクスチャを作成

	// タイマー作成
	timer_id=SDL_AddTimer(1000, ExecuteAnimation, &frame); // 1秒ごとにコールバック関数を呼び出す（渡す引数はフレーム番号）

	Mix_PlayMusic(music_tell,-1); //BGMを無限ループで再生
	// 無限ループ
	while(1){
		// イベント検知
		if(SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_USEREVENT: // ユーザ定義イベントなら（コールバック関数が呼び出されたなら）
					// イベントコードを参照
					switch(event.user.code){
						case 0: // イベントコードが0なら
							SDL_RenderPresent(renderer); // レンダラー（描画データ）を表示
							frame ++; // フレーム番号を+1
							if(frame == 4){ // フレーム番号が4になったら
								frame = 0; // フレーム番号を初期化
							}
							break;
					}
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_1:
							Mix_HaltMusic();
							Mix_PlayMusic(music_unknownbrain,-1);
							Mix_VolumeMusic(100);
							break;
						case SDLK_2:
							Mix_HaltMusic();
							Mix_PlayMusic(music_tell,-1);
							Mix_VolumeMusic(100);
							break;
						case SDLK_ESCAPE:
							printf("Terminated.\n");
							// 終了処理
							SDL_RemoveTimer(timer_id); // タイマの削除
							SDL_FreeSurface(image); // サーフェイスの解放
							SDL_DestroyTexture(texture); // テクスチャの破棄
							SDL_DestroyRenderer(renderer); // レンダラーの破棄
							SDL_DestroyWindow(window); // ウィンドウの破棄
							IMG_Quit(); // SDL_imageサブシステムの終了
							SDL_Quit(); // SDLの終了
							return 0;
							break;
					}
					break;
			}
		}
	}
}

