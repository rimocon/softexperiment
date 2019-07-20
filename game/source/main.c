#include "header.h"
timers timer;
int main (int argc, char *argv[]) {
	//メインウィンドウを作成
	startup();
	while ((quits()) != 0) {
		timer.now = SDL_GetTicks(); //現在時間を取得
		//イベントを取得したら
		if(SDL_PollEvent(&inputevent)) {
			input(); //入力
		}
		//printf("input");
		//printf("time now = %d \n",timer.now);
		//printf("time leave = %d \n",timer.leave);
		switch(games) {
			case 0: //タイトル画面
				if(titledraw) drawtitle(0,0); //タイトルを描画
				startGame();
				break;
			case 1: //ゲーム画面
				if(gamedraw) {
					drawgame(0,0); //ゲーム画面描画
				}
				if(timer.now - timer.leave > 5 ) {
					me_move(); //自キャラの位置を計算
					me_draw(); //自キャラを描画
					timer.leave = timer.now; //時間更新
				}
				//printf("draw\n");
				break; 
			case 2: //ゲームオーバー画面
				break;
			default:
				break;
		}
	//SDL_Delay(1);
	//frames(); //フレームレート維持用
	}
	Destroy();
	SDL_Quit();
	return 0;
}
