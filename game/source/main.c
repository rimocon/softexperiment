#include "header.h"

int main (int argc, char *argv[]) {
	//メインウィンドウを作成
	startup();

	while ((quits()) != 0) {
		background();
		switch(games) {
			case 0: //タイトル画面
				drawtitle(0,0); //タイトルを描画
				//printf("0\n");
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
