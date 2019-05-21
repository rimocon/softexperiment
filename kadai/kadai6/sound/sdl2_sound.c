/*************************************************
  sdl2_sound.c
  Programmed by H. Mitsuhara (May, 2018)

  コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_mixer
  動作：
  　BGMを繰り返し再生しながら，キー押下により効果音を再生する（Escで終了）
  備考：
  　適当な2つのサウンドファイル（test_music.mp3, test_chunk.wav）ファイルを用意する必要あり
 *************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL_mixer.h> // SDLでサウンドを用いるために必要なヘッダファイルをインクルード

int main(int argc, char* argv[]) {
	SDL_Window *window;	// ウィンドウ
	Mix_Music *music; // BGMデータ格納用構造体
	Mix_Chunk *chunk_test,chunk_famima; // 効果音データ格納用構造体
	SDL_Event event; // イベントデータ格納用構造体

	// SDL初期化（SDL_INIT_AUDIOを指定）
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// SDL_mixerの初期化（MP3ファイルを使用）
	Mix_Init(MIX_INIT_MP3);

	// オーディオデバイスの初期化
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("failed to initialize SDL_mixer.\n");
		SDL_Quit();
		exit(-1);
	}

	// BGMと効果音のサウンドファイルの読み込み
	if((music = Mix_LoadMUS("invincible.mp3")) == NULL || (chunk_test = Mix_LoadWAV("test_chunk.wav")) == NULL|| (chunck_famima = Mix_LoadWav("famima.wav")) == NULL){
		printf("failed to load music and chunk.\n");
		Mix_CloseAudio(); // オーディオデバイスの終了
		SDL_Quit();
		exit(-1);
	}

	// ウィンドウ生成
	if((window = SDL_CreateWindow("Press [Esc] to exit",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,320,240,SDL_WINDOW_RESIZABLE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	Mix_PlayMusic(music, -1); // BGMの再生（繰り返し再生）

	while(1){
		if(SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_KEYDOWN: // キーボードのキーが押された時
					switch(event.key.keysym.sym){
						case SDLK_SCANCODE_0:
							Mix_PlayChannel(1, chunk_test, 0); // 効果音の再生（1回再生）
							break;
						case SDLK_SCANCODE_1:
							Mix_PlayChannel(2,chunk_famima,0);
							break;
						case SDLK_RETURN: // Returnキーが押された時
							Mix_ResumeMusic();
							break;
						case SDLK_ESCAPE: // Escキーが押された時
							Mix_HaltChannel(-1); // 効果音の停止
							Mix_FreeChunk(chunk); // 効果音の解放
							Mix_HaltMusic(); // BGMの停止
							Mix_FreeMusic(music); // BGMの解放
							Mix_CloseAudio(); // オーディオデバイスの停止
							SDL_Quit(); // SDLの停止
							exit(0);
							break;
						case SDLK_DOWN: // ↓キーが押された時
							Mix_VolumeMusic(MIX_MAX_VOLUME/2); // BGMの音量を半減
							printf("Current volume: %d\n", Mix_VolumeMusic(-1)); // 現在の音量を表示
							break;
						case SDLK_UP: // ↑キーが押された時
							Mix_VolumeMusic(MIX_MAX_VOLUME*22); // BGMの音量を半減
							printf("Current volume: %d\n", Mix_VolumeMusic(-1)); // 現在の音量を表示
							break;
						case SDLK_LSHIFT:
							Mix_PauseMusic();
							break;
					}
					break;
			}
		}
	}
	return 0;
}

/*************************************************
  【演習】
  （１）別のサウンドファイルを再生してみる
  （２）BGMをキーまたはマウスボタンの押下により一時停止／解除できるようにする
  （３）複数の効果音を用意し，キーの押下によりそれらが同時に再生されるようにする
  （４）複数のBGMを用意し，マウスボタンの押下により切り替えて再生されるようにする
 *************************************************/


