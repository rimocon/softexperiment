#include "headers.h"

int games;
SDL_Window *mainwindow;
SDL_Surface *mainsurface;

int main (int argc, char *argv[]) {
	//メインウィンドウを作成
	startUp();

	while ((quits()) != 0) {
		switch(games) {
			case 0: //タイトル画面
				break;
			case 1: //ゲーム画面
				break; 
			case 2: //ゲームオーバー画面
				break;
			default:
				break;
		}
	}
	SDL_Quit();
	return 0;
}
//メインウィンドウの作成
void startUp() {
	//SDLの初期化がうまくいかなかったら強制終了
	if (SDL_Init(SDL_INIT_VIDEO) == -1) SDL_Quit(); 
	//ウィンドウを作成(300*400),ビットごとピクセル32,システムメモリ内に画面作成(ビデオサーフェイスを作成)
	mainwindow = SDL_CreateWindow("inbeida",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,400,320,0);
	mainsurface = SDL_GetWindowSurface(mainwindow);
	//ゲームの状態を判別する変数(とりあえず0)
	games = 0;
}

//終了用関数
int quits() {
	SDL_Event Qevents;
	//イベントを取得したら
	if (SDL_PollEvent(&Qevents)) {
		switch (Qevents.type) {
			case SDL_QUIT:
				return 0;
				break;
			//キーが押されたら
			case SDL_KEYDOWN:
			//Escキーが押されたら終了
				if ((Qevents.key.keysym.sym) == SDLK_ESCAPE)
					return 0;
					break;
			default:
				break;
		}
	}
	return 1;
}
