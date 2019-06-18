/*************************************************
sdl_event.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -L/usr/lib
備考：
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード

//　メイン関数
int main(int argc, char* argv[]) {
	SDL_Surface *window; // ウィンドウ（サーフェイス）データへのポインタ
	SDL_Event event; // SDLによるイベントを検知するための構造体
	SDL_Event quit_event = {SDL_QUIT};	// 特定のイベント名を格納
	SDL_Joystick *joystick;	// ジョイスティックを特定・利用するための構造体
	int i;

	// SDL初期化（ビデオとジョイスティック）
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// ウィンドウ生成
	if((window = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	SDL_EnableKeyRepeat(0,0);	// キーリピート無効を設定
//	SDL_EnableKeyRepeat(1000,1000);	// 1秒間隔のキーリピートを設定、1秒後に開始

	// 接続されているジョイスティックの名前を表示
	for(i=0;i<SDL_NumJoysticks();i++){
		printf("%s\n", SDL_JoystickName(i));
	}
	joystick=SDL_JoystickOpen(0);	// ジョイスティックを開いて構造体に割り当てる（有効にする）．ここではジョイスティックは1つとして引数に0を指定
	SDL_JoystickEventState(SDL_ENABLE);	// ジョイスティック関連のイベントを取得可能にする

	// ジョイスティックが開けなかったら
	if(!joystick) {
		printf("failed to open joystick.\n");
		exit(-1);
	}
	else{
		printf("The found joystick ID (index) is %d.\n", SDL_JoystickIndex(joystick));	// 接続されたジョイスティックのID番号（1つしか接続されていない場合は0
   		printf("The found joystick has %d axses.\n",SDL_JoystickNumAxes(joystick));	// ジョイスティックの方向キー数を取得
   		printf("The found joystick has %d Hat keys.\n",SDL_JoystickNumHats(joystick));	// ジョイスティックのHatキー数を取得（電算室のジョイスティックでは0?）
   		printf("The found joystick has %d balls.\n",SDL_JoystickNumBalls(joystick));	// ジョイスティックのボール数を取得（電算室のジョイスティックでは0?） */
   		printf("The found joystick has %d buttons.\n",SDL_JoystickNumButtons(joystick));	// ジョイスティックのボタン数を取得
	}

	// 無限ループ
	while(1){
		// イベントを取得したなら
		if(SDL_PollEvent(&event)){
			// イベントの種類ごとに処理
			switch (event.type) {
			case SDL_MOUSEMOTION: // マウスが移動した時
				printf("Mouse moved by %d,%d to (%d,%d)\n", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y);	// マウスの座標を表示
				break;
			case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押された時
 				printf("Mouse button %d pressed at (%d,%d)\n", event.button.button, event.button.x, event.button.y);	// 押されたマウスのボタン等を表示
				break;
			case SDL_ACTIVEEVENT:	// ウィンドウの見え方が変更された時
				printf("State=%d\n",event.active.state);	// ウィンドウの状態（番号）を表示
				break;
			case SDL_KEYDOWN:	// キーボードのキーが押された時
				// 押されたキーの名前等を表示
				printf("The pressed key is %s.\n", SDL_GetKeyName(event.key.keysym.sym));
				printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), event.key.keysym.mod);
				// 左シフトキーが押されたら
				if(SDL_GetModState()==KMOD_LSHIFT){
					printf("Left Shift + ");
				}
				// 右シフトキーが押されたら
				else if(event.key.keysym.mod==KMOD_RSHIFT){
					printf("Right Shift + ");
				}
				else{
					// 押されたキーごとに処理
					switch(event.key.keysym.sym){
					case SDLK_ESCAPE:	// Escキーが押された時
						printf("ESCAPE key -> EXIT.\n");
						SDL_Quit();
						exit(0);
						break;
					case SDLK_TAB:	// Tabキーが押された時
						SDL_PushEvent(&quit_event);	// イベントキューにSDL_QUITイベントを送信（→プログラム終了）
						break;
					}
				}					
				break;
			// ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
			case SDL_JOYAXISMOTION:
				printf("The axis ID of the operated key is %d.\n",event.jaxis.axis);	// 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
				if(event.jaxis.axis==0){
					printf("--- Analog-Direction Key: ?? Axis\n");
				}
				else if(event.jaxis.axis==1){
					printf("--- Anolag-Direction Key: ?? Axis\n");
				}
				else if(event.jaxis.axis==2){
					printf("--- Four-Direction Key: Horizontal Axis\n");
				}
				else if(event.jaxis.axis==3){
					printf("--- Four-Direction Key: Vertical Axis\n");
				}						
				printf("--- The axis value of the operated key is %d.\n",event.jaxis.value);	// ジョイスティックの操作された方向キーの値を表示（-32767（真左，真上）～32767（真右，真下））
				break;
			// ジョイスティックのボタンが押された時
			case SDL_JOYBUTTONDOWN:
				printf("The ID of the pressed button is %d.\n", event.jbutton.button);	// 押されたボタンのIDを表示（0から）
				// ボタンIDに応じた処理
				if(event.jbutton.button==0){
					printf("--- You pressed a button on the joystick.\n");
				}
				break;
			// ボタンが離された時
			case SDL_JOYBUTTONUP:
				printf("The ID of the released button is %d.\n", event.jbutton.button);	// 離されたボタンのIDを表示（0から）
				// ボタンIDに応じた処理
				if(event.jbutton.button==0){
					printf("--- You released a button on the joystick.\n");
				}
				break;
			case SDL_QUIT:	// SDLの利用を終了する時
				SDL_Quit();
				exit(0);
				break;
			}
		}
	}
}
