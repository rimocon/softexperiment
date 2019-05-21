/*************************************************
sdl2_callback_test.c
Programmed by H. Mitsuhara (May, 2019)

コンパイルオプション（リンク対象）：-lSDL2
動作
　ウィンドウ内でマウスを動かすと，ターミナルに座標表示
　5秒ごとに，10秒待機するコールバックが呼び出される
　Enterキーを押すと，5秒待機
　Escキーを押すと終了
備考：
　コールバック関数がメインスレッドとは異なるスレッドであることを確認するためのプログラム
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード

// タイマーで呼び出されるコールバック関数（メインスレッドと異なるスレッドになる）
Uint32 TestCallback(Uint32 interval, void *param){
    printf("10-sec Sleep in Callback Func.\n");
    SDL_Delay(10000); // 10秒待機
    // メインスレッドとは異なるスレッドでの待機なので，メインスレッドの処理は継続
    // （マウスを動かすと，座標は表示される）

    return interval; // コールバックが呼び出される間隔を返す（必須）
}

// この関数はメインスレッド内の関数（メインスレッドと同一）
void Sleep5(){
    printf("5-sec Sleep in Main Thread.\n");
    SDL_Delay(5000); // ここで5秒待機
    // メインスレッドの流れがここで止まるので，マウス座標は表示されない
}

// メイン関数
int main(int argc, char* argv[]) {
    SDL_Window* window; // ウィンドウデータ
    SDL_Event event; // イベントデータを格納する構造体
    SDL_TimerID timer_id; // タイマ割り込みを行うためのタイマのID

    // SDL初期化
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    // ウィンドウ生成・表示
    window = SDL_CreateWindow("Callback Func. Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 120, 120, 0);

    // タイマー作成
    timer_id=SDL_AddTimer(5000, TestCallback, NULL); // 5秒ごとにコールバック関数を呼び出す
    // タイマー設定では5秒ごとにコールバック関数が呼び出されるが，コールバック関数内で10秒待機される．
    // 結果的に10秒後にコールバック関数が呼び出されることと同じになる
    // （イベントキューには常にタイマー割込のキューが溜まっている）

    // 無限ループ
    while(1){
        // イベント検知
        if(SDL_PollEvent(&event)){
            switch (event.type) {
            case SDL_MOUSEMOTION: // マウスが移動した時
                printf("Mouse moved by %d,%d to (%d,%d)\n", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y); // マウスの座標を表示
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case SDLK_RETURN:
                    Sleep5(); // 5秒待機する関数を呼び出す
                    break;
                case SDLK_ESCAPE:
                    printf("Terminated.\n");
                    // 終了処理
                    SDL_RemoveTimer(timer_id); // タイマの削除
                    SDL_DestroyWindow(window); // ウィンドウの破棄
                    SDL_Quit(); // SDLの終了
                    return 0;
                    break;
                }
                break;
            }
        }
    }
}