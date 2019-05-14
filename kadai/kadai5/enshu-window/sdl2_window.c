/*************************************************
sdl2_window.c
Programmed by H. Mitsuhara (April, 2018)

コンパイルオプション（リンク対象）：-lSDL2
動作：
　ウィンドウが表示され，一定時間が経過するとウィンドウの設定が変化・反映される
備考：
　C99モードでコンパイルするなら，オプションに-std=c99をつける
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDL2を用いるために必要なヘッダファイルをインクルード

// メイン関数（コマンドライン引数を受け取る）
int main(int argc, char *argv[])
{
    int i;

    SDL_Window* window; // ウィンドウのデータを格納する構造体
    SDL_Renderer* renderer; // レンダラー（レンダリングコンテキスト：描画設定）を格納する構造体

    // コマンドライン引数の確認
    printf("argc=%d\n", argc);
    for(i=0; i<argc; i++){
        printf("argv[%d]=%s\n", i, argv[i]);
    }

    // SDL初期化（初期化失敗の場合は終了）
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    // ウィンドウ生成・表示
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    if(window == NULL){
        printf("Failed to create window.\n");
        exit(-1);
    }
    // SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0); // このように，宣言と同時にウィンドウデータを構造体に格納してもよい

    // 描画処理
   // renderer = SDL_CreateRenderer(window, -1, 0); // 生成したウィンドウに対してレンダリングコンテキスト（RC）を生成
   // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0); // 生成したRCに描画色として白を設定
   // SDL_RenderClear(renderer); // 生成したRCを白でクリア＝塗りつぶす（ただし，メモリに描画データを反映させただけなので，画面には表示されない）
   // SDL_RenderPresent(renderer); // 描画データを表示

    SDL_Delay(5000); // 5000ms（=5秒）待つ（sleep(5)と同等）

   // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // 生成したRCに描画色として黒を設定
   // SDL_RenderClear(renderer); // 設定色でRCをクリア
   // SDL_RenderPresent(renderer); // 描画データを表示
    SDL_Delay(5000); // 5秒待つ
    SDL_DestroyWindow(window); // 生成したウィンドウの破棄（消去）
    SDL_Delay(2000); // 2秒待つ
   // SDL_DestroyRenderer(renderer); // RCの破棄（解放）
    SDL_Quit(); // SDL使用の終了
    return 0;
}

/*************************************************
【演習】
（１）タイトルやサイズを変更してウィンドウを生成・表示する
（２）SDL_WindowFlags列挙体の値を変更してウィンドウを生成・表示する
（３）複数のウィンドウを生成・表示する
（４）矩形領域の描画設定（色やサイズ）を変えて描画する
（５）RCを生成しない場合（ウィンドウに描画処理をしない場合＝ウィンドウを生成・表示するだけの場合），どのような動作になるか確認する
*************************************************/
