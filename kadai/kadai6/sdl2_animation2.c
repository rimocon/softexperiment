/*************************************************
sdl2_animation2.c
Programmed by H. Mitsuhara (May, 2018)

コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_image
動作
　背景画像上の四角内に1->2->3->4と1秒おきに表示される
　十字キーによりその四角を移動できる
　Escキーを押すと終了
備考：
　※マルチスレッドとして処理されるコールバック関数内では，（開発・実行環境に依存することがあるようだが）レンダラーを表示できないようなので，コールバック関数の呼び出しをユーザ定義イベントとしてイベントキューに追加し，メインスレッドでレンダラーを表示している
　キャラクタ用画像は160*40以上で、40*40の正方形の領域ごとにキャラクタを描いておく
　背景画像はウィンドウサイズに合わせて480*360を用意するか，ウィンドウサイズを画像に合わせる
　なお，この例のような方法がアニメーション処理に最適であるとは限らない
　自分なりのアニメーション処理を考案してみましょう
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL_image.h> // SDL_imageを用いるために必要なヘッダファイルをインクルード

// Animation_data構造体を定義
typedef struct{
    SDL_Rect src_rect; // キャラクタ等の貼り付け元の画像の領域情報を格納
    SDL_Rect dst_rect; // キャラクタ等の貼り付け先の画像の領域情報を格納
    int frame; // キャラクタの現在（貼り付ける）のフレーム番号（0～3）
    int last_frame; // キャラクタの最後のフレーム番号
} Animation_data;

// グローバル変数
SDL_Window* window; // ウィンドウデータ
SDL_Renderer* renderer; // レンダラー（描画対象）
SDL_Texture* texture_char; // キャラクタ用テクスチャ
SDL_Texture* texture_bg; // 背景用テクスチャ
SDL_Event event; // イベントデータを格納する構造体

// タイマーで呼び出されるコールバック関数
Uint32 ExecuteAnimation(Uint32 interval, void *param){
    static SDL_Rect previous_char; // 前回キャラクタをコピーした座標とサイズ（静的変数）

    Animation_data *anim = param; // Animation_data型で渡されたパラメータ（引数）を格納
    // Animation_data *anim = (Animation_data*)param; // 明示的に型変換する場合はこのような記述になる

    SDL_RenderCopy(renderer, texture_bg, &previous_char, &previous_char); // 前回キャラクタをコピーした座標に背景画像を貼り直す（これがないと前のキャラクタが残る）

    // 描画（コピー）するキャラクタのフレームを選択（フレームの左上座標を取得）
    if(anim->frame >= 0 && anim->frame < anim->last_frame){
        anim->src_rect.x = anim->frame * anim->src_rect.w; // フレームごとにx座標をキャラ1体分ずらす
        anim->frame++; // フレーム番号を更新する
    }
    else{
        anim->src_rect.x = anim->frame * anim->src_rect.w;
        anim->frame=0; // フレーム番号を0に戻す
    }

    SDL_RenderCopy(renderer, texture_char, &anim->src_rect, &anim->dst_rect); // フレーム番号に対応する画像の一領域をレンダラーに貼り付ける

    // printf("Current Frame=%d (%d,%d,%d,%d)\n", anim->frame, anim->src_rect.x, anim->src_rect.y, anim->src_rect.w, anim->src_rect.h);
    // printf("Previous Frame=(%d,%d,%d,%d)\n", previous_char.x, previous_char.y, previous_char.w, previous_char.h);
    // printf("Current Position = (%d, %d)\n", anim->dst_rect.x, anim->dst_rect.y);
    
    // キャラクタの貼り付け座標を格納（前回キャラクタを貼り付けた座標として）
    previous_char.x = anim->dst_rect.x;
    previous_char.y = anim->dst_rect.y;
    previous_char.w = anim->src_rect.w;	// 値は不変だが
    previous_char.h = anim->src_rect.h;	// 値は不変だが

    // コールバック関数が呼び出されたことをユーザ定義イベントとしてイベントキューに追加
    event.type=SDL_USEREVENT; // イベントの種類をユーザ定義イベントにする
    SDL_PushEvent(&event); // イベントキューにユーザ定義イベントを追加

    return interval;	// コールバックが呼び出される間隔を返す（必須）
}

// メイン関数
int main(int argc, char* argv[]) {
    SDL_Surface *image_char, *image_bg; // 画像データ（サーフェイス）へのポインタ（キャラクタ用と背景用）
    SDL_TimerID timer_id; // タイマ割り込みを行うためのタイマのID

    Animation_data animation; // Animation_data型構造体

    // SDL初期化
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG); // 描画する画像形式を指定して初期化

    // ウィンドウ生成・表示
    window = SDL_CreateWindow("Animation Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 360, 0);

    // レンダラー（レンダリングコンテキスト）作成
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); // SDL_RENDERER_SOFTWAREを指定

    // アニメーション用画像設定
    image_char = IMG_Load("characters.png"); // キャラクタ画像の読み込み
    texture_char = SDL_CreateTextureFromSurface(renderer, image_char); // 読み込んだキャラクタ画像からテクスチャを作成

    image_bg=IMG_Load("background.jpg"); // 背景画像の読み込み
    texture_bg = SDL_CreateTextureFromSurface(renderer, image_bg); // 読み込んだ背景画像からテクスチャを作成

    // 1キャラクタ分の画像（コピー元）のサイズを指定
    animation.src_rect.x=0;
    animation.src_rect.y=0;
    animation.src_rect.w=40;
    animation.src_rect.h=40;
    animation.src_rect.h=40;
    // 画像の貼り付け先の初期座標
    animation.dst_rect.x=0;
    animation.dst_rect.y=0;
    animation.dst_rect.w=40;
    animation.dst_rect.h=40;

    // コピー元の現在のフレーム（領域）番号
    animation.frame=0;
    // コピー元の最後のフレーム（領域）番号
    animation.last_frame=3;

    SDL_RenderCopy(renderer, texture_bg, NULL, NULL); // 背景画像（テクスチャ）全体をレンダラーにコピー（転送）
    SDL_RenderPresent(renderer); // 描画データを表示

    timer_id=SDL_AddTimer(1000, ExecuteAnimation, &animation); // 1秒ごとにコールバック関数を呼び出す（渡す引数は&animation）

    // 無限ループ
    while(1){
        // イベント検知
        if(SDL_PollEvent(&event)){
            switch (event.type) {
            // コールバック関数が呼び出された
            case SDL_USEREVENT:
                SDL_RenderPresent(renderer); // 描画データを表示
                break;
            case SDL_KEYDOWN:
                // 十字キー操作でキャラクタを移動
                switch(event.key.keysym.sym){
                case SDLK_UP:
                    animation.dst_rect.y-=40;
                    break;
                case SDLK_DOWN:
                    animation.dst_rect.y+=40;
                    break;
                case SDLK_RIGHT:
                    animation.dst_rect.x+=40;
                    break;
                case SDLK_LEFT:
                    animation.dst_rect.x-=40;
                    break;
                case SDLK_ESCAPE:
                    printf("Terminated.\n");
                    // 終了処理
                    SDL_RemoveTimer(timer_id); // タイマの削除
                    SDL_FreeSurface(image_char); // サーフェイスの解放
                    SDL_FreeSurface(image_bg);
                    SDL_DestroyTexture(texture_char); // テクスチャの破棄
                    SDL_DestroyTexture(texture_bg);
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
（１）オリジナルのキャラクタ画像（背景透過画像），背景画像を作成してアニメーション表示させる
（２）キャラクタ移動の幅やフレーム切り替えの間隔を変えてアニメーション表示させる
（３）キーを連打すると，キャラクタがワープしたように表示されてしまう．その原因を考えて，改善する
*************************************************/

