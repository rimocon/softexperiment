/*************************************************
sdl2_image.c
Programmed by H. Mitsuhara (April, 2018)

コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_image
動作：
　3つの画像が表示される
備考：
　画像の描画の順番に留意すること
　サンプル画像以外を使いたい場合は，各自でPNGやGIFなどの画像ファイルを用意すること
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL_image.h> // SDL_imageを用いるために必要なヘッダファイルをインクルード

// メイン関数
int main(int argc, char *argv[])
{
    // SDL初期化
    SDL_Init(SDL_INIT_VIDEO);

    // 描画する画像形式を指定して初期化
    IMG_Init(IMG_INIT_PNG);

    // ウィンドウ生成・表示
    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 240, 0);
    
    // レンダラー（レンダリングコンテキスト：描画設定）作成
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
    SDL_RenderClear(renderer);

    // 画像描画処理
    SDL_Surface *image1 = IMG_Load("test1.png"); // 画像を（サーフェイスに）読み込む（test1.pngは背景透過画像）
    SDL_Surface *image2 = IMG_Load("test2.png"); // 画像を（サーフェイスに）読み込む（test2.pngは背景透過画像）
    SDL_Surface *image3 = IMG_Load("test3.png"); // 画像を（サーフェイスに）読み込む
    
    if(!(image1 && image2 && image3)){ // いずれかの画像読み込みに失敗したら
        printf("failed to load image file: %s\n", IMG_GetError());
    }
    
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, image1); // 読み込んだ画像からテクスチャを作成
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, image2);

    SDL_QueryTexture(texture1, NULL, NULL, &image1->w, &image1->h); // 画像（テクスチャ）の情報（サイズなど）を取得
    SDL_QueryTexture(texture2, NULL, NULL, &image2->w, &image2->h);

    // 画像描画（表示）のための設定
    SDL_Rect src_rect1 = {0, 0, image1->w, image1->h}; // コピー元画像の領域（x, y, w, h）（この場合，画像全体が設定される）
    SDL_Rect dst_rect1 = {0, 0, 100, 100}; // 画像のコピー先の座標と領域（x, y, w, h）
    SDL_RenderCopy(renderer, texture1, &src_rect1, &dst_rect1); // テクスチャをレンダラーにコピー（設定のサイズで）
    SDL_RenderCopy(renderer, texture2, NULL, NULL); // テクスチャをレンダラーにコピー（テクスチャ全体をレンダラー全体に）
    // SDL_Rect dst_rect2 = {0, 0, image2->w, image2->h};
    // SDL_RenderCopy(renderer, texture2, NULL, &dst_rect2); // テクスチャをレンダラーにコピー（テクスチャのオリジナルサイズで）
    
    texture1 = SDL_CreateTextureFromSurface(renderer, image3); // 読み込んだ画像からテクスチャを作成（texture1を再利用）
    SDL_QueryTexture(texture1, NULL, NULL, &image3->w, &image3->h); // 画像（テクスチャ）のサイズを取得
    SDL_Rect src_rect3 = {0, 0, image3->w, image3->h};
    SDL_Rect dst_rect3 = {100, 100, 50, 50};
    SDL_RenderCopy(renderer, texture1, &src_rect3, &dst_rect3); // テクスチャをレンダラーにコピー（設定のサイズで）

    SDL_RenderPresent(renderer); // 描画データを表示

    // 画像情報（色数）を表示
    printf("Image1 - Colors = %d bpp \n", image1->format->BitsPerPixel);
    printf("Image2 - Colors = %d bpp \n", image2->format->BitsPerPixel);
    printf("Image3 - Colors = %d bpp \n", image3->format->BitsPerPixel);
    
    SDL_Delay(5000);
    
    // 終了処理
    SDL_FreeSurface(image1); // サーフェイス（画像）の解放
    SDL_FreeSurface(image2);
    SDL_FreeSurface(image3);
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit(); // IMGライブラリの利用終了
    SDL_Quit();

    return 0;

}

/*************************************************
【演習】
（１）画像の描画の位置やサイズを変更する
（２）test1.pngがtest2.pngより手前に来るように描画する
（３）１つの画像でウィンドウを埋め尽くすように描画する
*************************************************/

