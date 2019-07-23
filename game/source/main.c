#include "header.h"
int main (int argc, char *argv[]) {
	//メインウィンドウを作成
	startup();
	while (run) {
		//イベントを取得したら
		if(SDL_PollEvent(&inputevent)) {
			input(); //入力
		}
		switch(games) {
			case 0: //タイトル画面
				//if(titledraw) drawtitle(0,0); //タイトルを描画
				drawtitle(0,0); //タイトルを描画
				//startGame();
				break;
			case 1: //ゲーム画面
				if(gamedraw) {
					drawgame(0,0); //ゲーム画面描画
				}
				else {
					SDL_RenderClear(mainrenderer); //レンダラーをクリア
					me_move(); //自キャラの位置を計算
					enemy_move(); //敵キャラの位置を計算
					me_bullet(); //自キャラ弾描画
					judge(); //当たり判定
					me_draw(); //自キャラを描画
				//	enemy_draw(); //敵キャラを描画
				}
				break; 
			case 2: //ゲームクリア画面
				drawclear(); //クリア画面描画
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
