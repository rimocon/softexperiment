/*************************************************
sdl2_image2.c
Programmed by H. Mitsuhara (April, 2018)

コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_image
動作：
　読み込んだ画像データを保持する2つのサーフェイスを別のサーフェイスに転送し，転送先サーフェイスからテクスチャを生成して，最終的にレンダラーに転送・表示する
備考：
　サンプル画像以外を使いたい場合は，各自でPNGやGIFなどの画像ファイルを用意すること
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL_image.h> // SDL_imageを用いるために必要なヘッダファイルをインクルード

// メイン関数
int main(int argc, char *argv[])
{
    Uint32 rmask, gmask, bmask, amask; // サーフェイス作成時のマスクデータを格納する変数

    // SDL初期化
    SDL_Init(SDL_INIT_VIDEO);

    // 描画する画像形式を指定して初期化
    IMG_Init(IMG_INIT_PNG);

    // ウィンドウ生成・表示
    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

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

    // サーフェイスを作成
    SDL_Surface* surface = SDL_CreateRGBSurface(0, 640, 480, 32, rmask, gmask, bmask, amask);

    // レンダラー（レンダリングコンテキスト：描画設定）作成
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderClear(renderer);

    // 画像描画処理
    SDL_Surface* image1 = IMG_Load("test1.png"); // 画像を（サーフェイスに）読み込む（test1.pngは背景透過画像）
    SDL_Surface* image2 = IMG_Load("test2.png"); // 画像を（サーフェイスに）読み込む（test2.pngは背景透過画像）
    
    SDL_Rect src_rect = { 0, 0, 100, 100 }; // 矩形領域を(0,0)-(100,100) に設定
    SDL_Rect dst_rect = { 100, 100 }; // 矩形領域の左上座標を指定（幅と高さは省略）
    SDL_BlitSurface(image1, &src_rect, surface, &dst_rect); // image1の描画データ（一部）をsurfaceに転送
    SDL_BlitSurface(image2, NULL, surface, NULL); // image2の描画データ（全体）をsurfaceに転送（image1の描画データに上書き）

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // サーフェイスからテクスチャを作成

    // 転送元の起点座標と領域（サイズ）を設定
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = 640;
    src_rect.h = 240;
    // 転送先の起点座標と領域（サイズ）を設定
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = 320;
    dst_rect.h = 120;
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect); // テクスチャをレンダラーにコピー
    SDL_RenderPresent(renderer); // 描画データを表示
    
    SDL_Delay(5000);
    
    // 終了処理
    SDL_FreeSurface(image1);
    SDL_FreeSurface(image2);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;

}

/*************************************************
【演習】
（１）転送元と転送先の座標や領域を変更して描画する
（２）SDL_RenderCopyEx関数を使って，回転・反転させて描画する
（３）0.1秒おきに画像を回転して描画する
*************************************************/
