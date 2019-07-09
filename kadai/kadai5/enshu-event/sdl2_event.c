/*************************************************
sdl2_event.c
Programmed by H. Mitsuhara (April, 2018)

コンパイルオプション（リンク対象）：-lSDL2
動作：
　ウィンドウ上でマウス移動やキーボードのキー押下に応じて動作する
　[Esc]キーでプログラムを終了する
備考：
　ジョイスティック（JS）イベントの処理については，コメントを外してコンパイルし，JSを接続して動作を確認すること
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード

// メイン関数
int main(int argc, char* argv[])
{
    int i;

    SDL_Window* window; // ウィンドウデータを格納する構造体
    SDL_Renderer* renderer; // レンダリングコンテキスト（描画設定）を格納する構造体
    Uint32 window_id, window_sub_id; // ウィンドウID

    SDL_Event event; // SDLによるイベントを検知するための構造体
    SDL_Event quit_event = {SDL_QUIT}; // 特定のイベント名を格納
    SDL_Joystick *joystick; // ジョイスティックを特定・利用するための構造体

    // SDL初期化（ビデオとジョイスティック）
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    // ウィンドウ生成・表示
    if((window = SDL_CreateWindow("Press [Esc] or [Tab] to exit",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,320,240,SDL_WINDOW_RESIZABLE)) == NULL) {
        printf("failed to initialize videomode.\n");
        exit(-1);
    }
    SDL_Window* window_sub = SDL_CreateWindow("Sub Window",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,320,240,0);

    window_id = SDL_GetWindowID(window); // ウィンドウIDを取得
    window_sub_id = SDL_GetWindowID(window_sub); // ウィンドウIDを取得
    printf("Main Window ID=%d, Sub Window ID=%d\n", window_id, window_sub_id);

    // レンダリングコンテキスト（RC）作成
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    // 接続されているジョイスティックの名前を表示
    for(i=0; i<SDL_NumJoysticks(); i++){ // 接続されているジョイスティックの数だけ繰り返す
        joystick = SDL_JoystickOpen(i); // ジョイスティックを開く
        printf("%s\n", SDL_JoystickName(joystick)); // 開いたジョイスティック名を表示
    }

    // ジョイスティックが開けなかったら
    if(!joystick) {
        printf("failed to open joystick.\n");
        exit(-1);
    }
    else{
        printf("The joystick has %d axses.\n",SDL_JoystickNumAxes(joystick)); // 方向キー数を取得
        printf("The joystick has %d buttons.\n",SDL_JoystickNumButtons(joystick)); // ボタン数を取得
        printf("The joystick has %d Hat keys.\n",SDL_JoystickNumHats(joystick)); // Hatキー数を取得
        printf("The joystick has %d balls.\n",SDL_JoystickNumBalls(joystick)); // ボール数を取得
    }

    // イベント処理（無限ループ内でイベントを取得し，対応する処理を行う）
    while(1){
        // イベントを取得したなら
        if(SDL_PollEvent(&event)){
            // イベントの種類ごとに処理
            switch (event.type) {
            case SDL_MOUSEMOTION: // マウスが移動した時
                printf("Mouse moved by %d,%d to (%d,%d)\n", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y); // マウスの座標を表示
                break;
            case SDL_MOUSEBUTTONDOWN: // マウスのボタンが押された時
                printf("Mouse button %d pressed at (%d,%d)\n", event.button.button, event.button.x, event.button.y); // 押されたマウスのボタン等を表示
                // マウスのボタンごとに処理
                switch(event.button.button){
                case SDL_BUTTON_LEFT:
                    printf("Left button pressed\n");
                    break;
                case SDL_BUTTON_MIDDLE:
                    printf("Middle button pressed\n");
                    break;
                case SDL_BUTTON_RIGHT:
                    printf("Right button pressed\n");
                    break;
                case SDL_BUTTON_X1:
                    printf("Back button pressed\n");
                    break;
                case SDL_BUTTON_X2:
                    printf("Forward button pressed\n");
                    break;
                }
                break;
            case SDL_KEYDOWN: // キーボードのキーが押された時
                // 押されたキーの名前等を表示
                printf("The pressed key is %s.\n", SDL_GetKeyName(event.key.keysym.sym));
                printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), event.key.keysym.mod);
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
                    case SDLK_ESCAPE: // Escキーが押された時
                        printf("ESCAPE key -> EXIT.\n");
                        SDL_Quit();
                        exit(0);
                        break;
                    case SDLK_TAB:	// Tabキーが押された時
                        SDL_PushEvent(&quit_event); // イベントキューにSDL_QUITイベントを追加（→プログラム終了）
                        break;
                    }
                }
                break;
            // ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
            case SDL_JOYAXISMOTION:
                printf("The axis ID of the operated key is %d.\n",event.jaxis.axis); // 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
                if(event.jaxis.axis==0){
                    printf("--- Analog-Direction Key: 0 Axis\n");
                }
                else if(event.jaxis.axis==1){
                    printf("--- Anolag-Direction Key: 1 Axis\n");
                }
                else if(event.jaxis.axis==2){
                    printf("--- Four-Direction Key: Horizontal Axis\n");
                }
                else if(event.jaxis.axis==3){
                    printf("--- Four-Direction Key: Vertical Axis\n");
                }
                printf("--- The axis value of the operated key is %d.\n",event.jaxis.value); // ジョイスティックの操作された方向キーの値を表示（-32767（真左，真上）～32767（真右，真下））
                break;
            // ジョイスティックのボタンが押された時
            case SDL_JOYBUTTONDOWN:
                printf("The ID of the pressed button is %d.\n", event.jbutton.button); // 押されたボタンのIDを表示（0から）
                // ボタンIDに応じた処理
                if(event.jbutton.button==0){
                    printf("--- You pressed a button on the joystick.\n");
                }
                break;
            // ボタンが離された時
            case SDL_JOYBUTTONUP:
                printf("The ID of the released button is %d.\n", event.jbutton.button); // 離されたボタンのIDを表示（0から）
                // ボタンIDに応じた処理
                if(event.jbutton.button==0){
                    printf("--- You released a button on the joystick.\n");
                }
                break;
            // ウィンドウイベントが発生した時
            case SDL_WINDOWEVENT:
                printf("Generated window's ID=%d, Event-catched window's ID=%d\n", window_id, event.window.windowID);
                if(event.window.windowID == window_id){
                    switch (event.window.event){
                    // ウィンドウサイズが変化
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        printf("The window size changed to (%d, %d).\n", event.window.data1, event.window.data1);
                        break;
                    // ウィンドウが移動
                    case SDL_WINDOWEVENT_MOVED:
                        printf("The window is on (%d, %d).\n", event.window.data1, event.window.data1);
                        break;
                    // ウィンドウが現れた
                    case SDL_WINDOWEVENT_EXPOSED:
                        printf("The window appeared---Not re-rendered.\n");
                        break;
                    }
                }
                break;
            // SDLの利用を終了する時（イベントキューにSDL_QUITイベントが追加された時）
            case SDL_QUIT:
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_DestroyWindow(window_sub);
                SDL_Quit();
                exit(0);
                break;
            }
            // マウスポインタの位置に点を描画
            SDL_RenderDrawPoint(renderer, event.motion.x, event.motion.y);
            SDL_RenderPresent(renderer);
        }
    }
}

/*************************************************
【演習】
（１）プログラムを他のイベントにも対応するように改良する
（２）マウスをクリックしている時だけ，点を描画するように改良する
（３）マウスホイールのイベント（操作）を検知できるようにする
（４）「○キーを押しながら□キーを押す」というイベント（操作）を検知できるようにする
（５）より多くのウィンドウイベントに対応する
（６）2つのウィンドウに対するイベントの動作を確認し，可能ならば改良する
（７）ジョイスティックがあれば，接続して動作を確認する
*************************************************/
