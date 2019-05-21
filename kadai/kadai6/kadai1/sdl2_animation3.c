/*************************************************
sdl2_animation3.c
Programmed by H. Mitsuhara (April, 2019)

コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_image
動作：
　2つのタイマーを使って，奥（山）と手前（ビル）の画像をそれぞれ異なる時間間隔でずらして描画することで，奥行きを感じさせる多重スクロールを実行する
　より合理的な処理があると思われるので，各自で考えてみてください
備考：
　サンプル画像以外を使いたい場合は，各自でPNGやGIFなどの画像ファイルを用意すること
　手前（ビルなどの建物）の画像は「いらすとや」より（https://www.irasutoya.com/）
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL_image.h> // SDL_imageを用いるために必要なヘッダファイルをインクルード

// グローバル変数（画像の転送元・先のデータ）
SDL_Rect src_rect_bldg = { 0, 0, 480, 120 }; // 転送元画像（ビル）領域
SDL_Rect src_rect_mt= { 0, 0, 480, 160 }; // 転送元画像（山）領域
SDL_Rect dst_rect_bldg = { 0, 75 }; // 画像（ビル）の転送先（テクスチャ）領域
SDL_Rect dst_rect_mt = { 0, 0 }; // 画像（山）の転送先（テクスチャ）領域

SDL_Event event; // イベントデータを格納する構造体

// タイマーで呼び出されるコールバック関数（山の画像をスクロールさせるための座標計算）
Uint32 ScrollMountain(Uint32 interval, void *param){
    // 画像（山）の右半分全体（(480,0)-(959,160)：右端まで）を描画したら場合
    if(src_rect_mt.x == 480){
        src_rect_mt.x = 0; // 転送元画像（山）の左端を指定
    }
    else{
        src_rect_mt.x += 5; // 転送元画像（山）を右に5 pixelずらす
    }

    // コールバック関数が呼び出されたことをユーザ定義イベントとしてイベントキューに追加
    event.type=SDL_USEREVENT; // イベントの種類をユーザ定義イベントにする
    SDL_PushEvent(&event); // イベントキューにユーザ定義イベントを追加

    return interval;
}

// タイマーで呼び出されるコールバック関数（ビルの画像をスクロールさせるための座標計算）
Uint32 ScrollBuilding(Uint32 interval, void *param){
    if(src_rect_bldg.x == 480){ // 画像（ビル）の右半分全体（(480,0)-(959,120)：右端まで）を描画したら場合
        src_rect_bldg.x = 0; // 転送元画像（ビル）の左端を指定
    }
    else{
        src_rect_bldg.x += 5; // 転送元画像（ビル）を右に5 pixelずらす
    }

    // コールバック関数が呼び出されたことをユーザ定義イベントとしてイベントキューに追加
    event.type=SDL_USEREVENT; // イベントの種類をユーザ定義イベントにする
    SDL_PushEvent(&event); // イベントキューにユーザ定義イベントを追加

    return interval;
}

// メイン関数
int main(int argc, char *argv[])
{
    SDL_Texture* texture; // 合成画像転送先テクスチャ
    SDL_TimerID timer_id_bldg, timer_id_mt; // タイマ割り込みを行うためのタイマのID
    Uint32 rmask, gmask, bmask, amask; // サーフェイス作成時のマスクデータを格納する変数

    // SDL初期化
    SDL_Init(SDL_INIT_EVERYTHING);

    // 描画する画像形式を指定して初期化
    IMG_Init(IMG_INIT_PNG);

    // ウィンドウ生成・表示
    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 200, 0);

    // エンディアンに応じて，マスクを設定
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

    // 合成画像作成用サーフェイスを作成
    SDL_Surface* surface = SDL_CreateRGBSurface(0, 480, 200, 32, rmask, gmask, bmask, amask);

    // レンダラー（レンダリングコンテキスト：描画設定）作成
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 184, 136, 59, 255); // 黄土色

    // 画像読み込み
    SDL_Surface* image_mt = IMG_Load("mountain.png"); // 背景用画像（山．960*160）を読み込む
    SDL_Surface* image_bldg = IMG_Load("building.png"); // 背景用画像（ビル．960*120背景透過画像）を読み込む

    // タイマー作成
    timer_id_bldg = SDL_AddTimer(100, ScrollBuilding, NULL); // 0.1秒ごとにコールバック関数ScrollBuildingを呼び出す
    timer_id_mt = SDL_AddTimer(500, ScrollMountain, NULL); // 0.5秒ごとにコールバック関数ScrollMountainを呼び出す

    // 無限ループ
    while(1){
        // イベント検知
        if(SDL_PollEvent(&event)){
            switch (event.type) {
            // コールバック関数が呼び出された
            case SDL_USEREVENT:
                // 合成画像作成用サーフェイスを塗りつぶす
                SDL_FillRect(surface, NULL, 0xff3b88b8); // リトルエンディアンの場合の黄土色
                // 山（奥の背景）の画像を合成画像作成用サーフェイスへ転送
                SDL_BlitSurface(image_mt, &src_rect_mt, surface, &dst_rect_mt); // 山の画像をsurfaceに転送
                // ビル（手前の背景）の画像を合成画像作成用サーフェイスへ転送
                SDL_BlitSurface(image_bldg, &src_rect_bldg, surface, &dst_rect_bldg); // ビルの画像をsurfaceに転送（山の手前にビルが描画される）

                texture = SDL_CreateTextureFromSurface(renderer, surface); // 合成画像（サーフェイス）をテクスチャに転送
                SDL_RenderCopy(renderer, texture, NULL, NULL); // テクスチャをレンダラーにコピー
                SDL_RenderPresent(renderer); // 描画データを表示
                break;
            // キーが押された
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                // Escキー
                case SDLK_ESCAPE:
                    // 終了処理
                    SDL_RemoveTimer(timer_id_mt);
                    SDL_RemoveTimer(timer_id_bldg);
                    SDL_FreeSurface(image_mt);
                    SDL_FreeSurface(image_bldg);
                    SDL_FreeSurface(surface);
                    SDL_DestroyTexture(texture);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    IMG_Quit();
                    SDL_Quit();
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
（１）転送元画像をずらすタイミング（スピード）や幅を変えて，滑らかなスクロールにする
（２）左から右へ，または，上から下へスクロールさせる
（３）スクロールしている背景の手前にキャラクタ画像（背景透過画像）を描画し，キー操作でそのキャラクタを動かせるようにする（上下左右）
*************************************************/

