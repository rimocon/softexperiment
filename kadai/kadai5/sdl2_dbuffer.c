/*************************************************
sdl2_dbuffer.c
Programmed by H. Mitsuhara (May, 2018)

コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_image
動作
　上キーを押すとダブルバッファリングなしの描画
　下キーを押すとダブルバッファリングありの描画
　右キーを押すとサーフェイスを用いたダブルバッファリングなしの描画（表示なし）
　左キーを押すとサーフェイスを用いたダブルバッファリングなしの表示
　Delキーを押すと黒で画面クリア
　Escキーを押すと終了
備考
　画像は適当に用意すること
　・背景画像（480*360）backgroundg.jpg
　・キャラクタ画像（40*40）character.png
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL_image.h> // SDL_imageを用いるために必要なヘッダファイルをインクルード

// メイン関数
int main(int argc, char* argv[]) {
    SDL_Window *window; // ウィンドウ
    SDL_Surface* image_char; // 画像格納用サーフェイス
    SDL_Surface* image_bg;
    SDL_Surface* buffer; // ダブルバッファリング用サーフェイス
    SDL_Renderer* renderer; // レンダラー
    SDL_Texture* texture; // テクスチャ
    SDL_Texture* texture_char; // キャラクタ用テクスチャ
    SDL_Texture* texture_bg; // 背景用テクスチャ

    // キャラクタ画像の座標とサイズ
    SDL_Rect src_rect = { 0, 0, 40, 40 }; // 転送元の領域（x, y, w, h）
    SDL_Rect dst_rect = { 0, 0, 40, 40 }; // 転送先の領域（x, y, w, h）

    // その他
    SDL_Event event;
    int i, j;

    // SDL初期化
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG); // 描画する画像形式を指定して初期化

    // ウィンドウ生成・表示
    window = SDL_CreateWindow("Double Buffer Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,480,360,0);

    // レンダラー（レンダリングコンテキスト）作成
    renderer = SDL_CreateRenderer(window, -1, 0);

    // 画像の読み込み
    image_char = IMG_Load("character.png"); // キャラクタ画像の読み込み
    texture_char = SDL_CreateTextureFromSurface(renderer, image_char); // 読み込んだキャラクタ画像からテクスチャを作成

    image_bg=IMG_Load("background.jpg"); // 背景画像の読み込み
    texture_bg = SDL_CreateTextureFromSurface(renderer, image_bg); // 読み込んだ背景画像からテクスチャを作成

    // バッファ用サーフェイスを作成
    buffer = SDL_CreateRGBSurface(0, 480, 360, 32, 0, 0, 0, 0); // ウィンドウサイズに合わせて作成

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // 生成したレンダラーに描画色として黒を設定
    SDL_RenderClear(renderer); // 設定色でレンダラーをクリア
    SDL_RenderPresent(renderer); // レンダラー（描画データ）をディスプレイに表示

    while(1){
        if(SDL_PollEvent(&event)){
            switch (event.type) {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){

                // キャラクタを1体ずつ転送・表示する方法
                case SDLK_UP: // 上キーで転送・表示
                    printf("NO Double Buffering\n");
                    SDL_RenderCopy(renderer, texture_bg, NULL, NULL); // 背景画像をレンダラーに転送
                    for(i=0; i<480; i=i+40){
                        dst_rect.x = i; // 転送先x座標
                        for(j=0; j<360; j=j+40){
                            dst_rect.y = j; // 転送先y座標
                            SDL_RenderCopy(renderer, texture_char, &src_rect, &dst_rect); // キャラクタ画像をレンダラーに転送
                            SDL_RenderPresent(renderer); // レンダラー（描画データ）をディスプレイに表示
                        }
                    }
                    break;

                // すべてのキャラクタを一度に転送・表示方法
                case SDLK_DOWN: // 下キーで転送・表示
                    printf("Double Buffering to Renderer\n");
                    SDL_RenderCopy(renderer, texture_bg, NULL, NULL); // 背景画像をレンダラーに転送
                    for(i=0; i<480; i=i+40){
                        dst_rect.x = i; // 転送先x座標
                        for(j=0; j<360; j=j+40){
                            dst_rect.y = j; // 転送先y座標
                            SDL_RenderCopy(renderer, texture_char, &src_rect, &dst_rect); // キャラクタ画像をレンダラーに転送
                        }
                    }
                    SDL_RenderPresent(renderer); // レンダラーをディスプレイに表示
                    break;

                // ダブルバッファリング（すべてのキャラクタをバッファに描画するが，レンダラーへの転送・表示まではしない）
                case SDLK_RIGHT: // 右キーで転送（この段階では見えない）
                    printf("Double Buffering using Surface (Data Storing)\n");
                    SDL_BlitSurface(image_bg, NULL, buffer, NULL); // 背景画像をバッファ（サーフェイス）に転送
                    for(i=0; i<480; i=i+40){
                        dst_rect.x = i; // 転送先x座標
                        for(j=0; j<360; j=j+40){
                            dst_rect.y = j;  // 転送先y座標
                            SDL_BlitSurface(image_char, &src_rect, buffer, &dst_rect); // キャラクタ画像をバッファに転送
                        }
                    }
                    break;

                // ダブルバッファリング（バッファの描画内容をレンダラーへ転送・表示）
                case SDLK_LEFT: // 左キーで表示
                    printf("Double Buffering using Surface (Display)\n");
                    texture = SDL_CreateTextureFromSurface(renderer, buffer); // バッファからテクスチャを作成
                    SDL_RenderCopy(renderer, texture, NULL, NULL); // テクスチャ（バッファから作成）をレンダラーに転送
                    SDL_RenderPresent(renderer); // レンダラーをディスプレイに表示
                    break;

                // 描画内容消去
                case SDLK_DELETE:
                    printf("Display Clear (Black)\n");
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // 生成したレンダラーに描画色として黒を設定
                    SDL_RenderClear(renderer); // 設定色でレンダラーをクリア
                    SDL_RenderPresent(renderer); // 描画データを表示
                    break;
                // 終了処理
                case SDLK_ESCAPE: // Escキーが押された時
                    printf("Terminated.\n");
                    SDL_FreeSurface(image_char); // サーフェイスの解放
                    SDL_FreeSurface(image_bg);
                    SDL_DestroyTexture(texture); // テクスチャの破棄
                    SDL_DestroyTexture(texture_char);
                    SDL_DestroyTexture(texture_bg);
                    SDL_DestroyRenderer(renderer); // レンダラーの破棄
                    SDL_DestroyWindow(window); // ウィンドウの破棄
                    IMG_Quit(); // SDL_imageサブシステムの終了
                    SDL_Quit(); // SDLの終了
                    exit(0);
                    break;
                }
            }
        }
    }
    return 0;
}

/*************************************************
【演習】
（１）キャラクタの数を増やして，ちらつきを確認する
*************************************************/

