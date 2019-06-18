/*************************************************
sdl_sound.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lSDL_mixer -L/usr/lib
備考：
　適当な２つのWAV（test_music.wav, test_chunk.wav）ファイルを用意してください．
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_mixer.h> // SDLでサウンドを用いるために必要なヘッダファイルをインクルード

int main(int argc, char* argv[]) {
	SDL_Surface *window;	// ウィンドウ
	Mix_Music *music; // BGM
	Mix_Chunk *chunk; // 効果音
	SDL_Event event; // SDLによるイベントを検知するための構造体

	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// サウンドの初期化
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("failed to initialize SDL_mixer.\n");
		SDL_Quit();
		exit(-1);
	}

	// BGMの読み込み
	if((music = Mix_LoadMUS("test_music.wav")) == NULL) {
		printf("failed to load music.\n");
		SDL_Quit();
		exit(-1);
	}
	// 効果音の読み込み
	if((chunk = Mix_LoadWAV("test_chunk.wav")) == NULL) {
		printf("failed to load chunk.\n");
		SDL_Quit();
		exit(-1);
	}
	
	// ウィンドウ生成
	if((window = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	Mix_PlayMusic(music, -1); // BGMの再生（繰り返し再生）

	while(1){
		if(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_KEYDOWN:	// キーボードのキーが押された時
				Mix_PlayChannel(-1, chunk, 0); // 効果音の再生（自動で割り当てたチャンネルで1回再生）
				switch(event.key.keysym.sym){
				case SDLK_ESCAPE:	// Escキーが押された時
					Mix_HaltChannel(-1);  // 効果音の停止
					Mix_FreeChunk(chunk); // 効果音の解放

					Mix_HaltMusic();      // BGMの停止
					Mix_FreeMusic(music); // BGMの解放

					Mix_CloseAudio(); // サウンドの停止

					SDL_Quit(); // SDLの停止

					exit(0);
					break;
				}
				break;
			}
		}
	}
	return 0;
}

