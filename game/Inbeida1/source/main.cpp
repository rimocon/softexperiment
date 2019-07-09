#include "headers.h"

int main(int argc, char *argv[])
{
startUp();
while(quits()!=0)
	{
	backGraund();	//バック地
	inputEngine();//キー入力
	switch(games)
		{
		case -1://ウィンドウモード選択
			bos_test();
			winMode();
			break;
			
		case 0://タイトル画面
			dr_Title(100,100);
			startGame();//ゲームへ
			break;
		
		case 1://ゲーム
			chara();
			break;
		
		case 2://ゲームオーバー
			dr_end(100,100);
			Mix_HaltMusic();//BGM止める
			backTitle();//タイトル画面へ
			break;
		
		default: break;
		}
	score_main();//得点
	frames();
	}
SDL_Quit();
return 0;
}
