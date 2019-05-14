/*************************************************
sdl2_animation.c
Programmed by H. Mitsuhara (May, 2018)

コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_image
動作
　画像が1->2->3->4と1秒おきに表示される
　Escキーを押すと終了
備考：
　画像は160*40以上で、40*40の正方形の領域ごとにキャラクタを描いておく
　なお，この例のような方法がアニメーション処理に最適であるとは限らない
　自分なりのアニメーション処理を考案してみましょう
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL_image.h> // SDL_imageを用いるために必要なヘッダファイルをインクルード

// グローバル変数
SDL_Renderer* renderer; // レンダラー（描画対象）
SDL_Texture* texture; // キャラクタ用テクスチャ
SDL_Event event; // イベントデータを格納する構造体

// タイマーで呼び出されるコールバック関数
Uint32 ExecuteAnimation(Uint32 interval, void *param){
    int *frame = param; // 受け取ったフレーム番号
    SDL_Rect src_rect = {0, 0, 40, 40}; // 転送元画像の領域（初期値を設定）
    
    src_rect.x = *frame *40; // 転送元画像の左上x座標（0 -> 40 -> 80 -> 120 -> 0）を指定
    SDL_RenderCopy(renderer, texture, &src_rect, NULL); // 転送元画像（テクスチャ）の指定した領域をレンダラーに転送

    // コールバック関数が呼び出されたことをユーザ定義イベントとしてイベントキューに追加
    event.type = SDL_USEREVENT; // イベントの種類をユーザ定義イベントにする
    event.user.code = 0; // 定義したイベントのコードに0を設定
    SDL_PushEvent(&event); // イベントキューにユーザ定義イベントを追加

    return interval;	// コールバックが呼び出される間隔を返す（必須）
}

// メイン関数
int main(int argc, char* argv[]) {
    SDL_Window* window; // ウィンドウデータ
    SDL_Surface *image; // 画像データ（サーフェイス）へのポインタ
    SDL_TimerID timer_id; // タイマ割り込みを行うためのタイマのID
    int frame = 0; // フレーム（コマ）番号（0で初期化）

    // SDL初期化
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG); // 描画する画像形式を指定して初期化

    // ウィンドウ生成・表示
    window = SDL_CreateWindow("Animation Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 120, 120, 0);

    // レンダラー（レンダリングコンテキスト）作成
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); // SDL_RENDERER_SOFTWAREを指定

    // アニメーション用画像読み込み
    image = IMG_Load("characters.png"); // 画像の読み込み
    texture = SDL_CreateTextureFromSurface(renderer, image); // 読み込んだ画像からテクスチャを作成

    // タイマー作成
    timer_id=SDL_AddTimer(1000, ExecuteAnimation, &frame); // 1秒ごとにコールバック関数を呼び出す（渡す引数はフレーム番号）

    // 無限ループ
    while(1){
        // イベント検知
        if(SDL_PollEvent(&event)){
            switch (event.type) {
            case SDL_USEREVENT: // ユーザ定義イベントなら（コールバック関数が呼び出されたなら）
                // イベントコードを参照
                switch(event.user.code){
                case 0: // イベントコードが0なら
                    SDL_RenderPresent(renderer); // レンダラー（描画データ）を表示
                    frame ++; // フレーム番号を+1
                    if(frame == 4){ // フレーム番号が4になったら
                        frame = 0; // フレーム番号を初期化
                    }
                    break;
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    printf("Terminated.\n");
                    // 終了処理
                    SDL_RemoveTimer(timer_id); // タイマの削除
                    SDL_FreeSurface(image); // サーフェイスの解放
                    SDL_DestroyTexture(texture); // テクスチャの破棄
                    SDL_DestroyRenderer(renderer); // レンダラーの破棄
                    SDL_DestroyWindow(window); // ウィンドウの破棄
                    IMG_Quit(); // SDL_imageサブシステムの終了
                    SDL_Quit(); // SDLの終了
                    return 0;
                    break;
                }
                break;
            }
        }
    }
}

/*************************************************
【演習】
（１）タイマー割込の時間間隔を変えてアニメーション表示する
（２）オリジナルの画像を作成してアニメーション表示させる
（３）背景画像に前景（キャラクタ）画像を重ねてアニメーション表示させる
*************************************************/
