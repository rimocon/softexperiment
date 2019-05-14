/*************************************************
sdl2_draw.c
Programmed by H. Mitsuhara (April, 2018)

コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_gfx
動作：
　ウィンドウにいくつかの図形と文字列が表示される
備考：
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL2_gfxPrimitives.h> // gfxによる図形描画処理のために必要なヘッダファイルをインクルード

// メイン関数（コマンドライン引数を受け取る）
int main(int argc, char *argv[])
{
    int x, y;
    SDL_Rect rect1 = { 200, 0, 100, 100 }; // 矩形領域データ（左上頂点のx,y座標，幅，高さ）を格納する構造体
    SDL_Rect rect2; // 矩形領域データ
    rect2.x = 300;
    rect2.y = 100;
    rect2.w = 100;
    rect2.h = 100;
    SDL_Point points[5] = {{45,0},{0,90},{90,30},{0,30},{90,90}}; // 5頂点のデータを配列に格納

    SDL_Window* window; // ウィンドウデータを格納する構造体
    SDL_Renderer* renderer; // 2Dレンダリングコンテキスト（描画設定）を格納する構造体

    // SDL初期化（初期化失敗の場合は終了）
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    // ウィンドウ生成・表示
    window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, 0);

    // レンダリングコンテキスト（RC）作成
    renderer = SDL_CreateRenderer(window, -1, 0);


    // SDL2による図形描画処理
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 生成したRCに描画色として白を設定
    SDL_RenderClear(renderer); // 設定色でRCをクリア
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0); // 生成したRCに描画色として赤を設定
    // 点を描画
    for(x=0; x<640; x+=10){
        SDL_RenderDrawPoint(renderer, x, 200);
    }
    // 線を描画
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0); // 生成したRCに描画色として緑を設定
    SDL_RenderDrawLine(renderer, 45, 0, 90, 90); // 設定したRCで単線を描画
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0); // 生成したRCに描画色として青を設定
    SDL_RenderDrawLines(renderer, points, 5); // 設定したRCで複数の線を描画
    // 四角形（塗りつぶしなし）を描画
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0); // 生成したRCに描画色として紫を設定
    SDL_RenderDrawRect(renderer, &rect1);
    // 四角形（塗りつぶしあり）を描画
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0); // 生成したRCに描画色として黄を設定
    SDL_RenderFillRect(renderer, &rect2);

    SDL_RenderPresent(renderer); // 描画データを表示

    SDL_Delay(5000);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 生成したRCに描画色として黒を設定
    SDL_RenderClear(renderer); // 設定色でRCをクリア

    // エンディアンを確認（表示）
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        printf("BIG Endian\n");
    }
    else{
        printf("LITTLE Endian\n");
    }

    // gfxによる図形描画処理（色指定はリトルエンディアンの場合）
    // 点を描画
    for(x=0; x<640; x+=10){
        pixelColor(renderer, x, 200, 0xff0000ff); // 赤で描画
    }
    // 線を描画
    lineColor(renderer, 45, 0, 90, 90, 0xff00ff00); // 緑で描画
    // 四角形（塗りつぶしなし）を描画
    rectangleColor(renderer, 200, 0, 300, 100, 0xffff00ff); // 紫で描画
    // 四角形（塗りつぶしあり）を描画
    boxColor(renderer, 300, 100, 400, 200, 0xff00ffff); // 黄で描画
    // 円（塗りつぶしあり）を描画
    filledCircleColor(renderer, 200, 200, 100, 0xffffffff); // 白で描画
    // 円（塗りつぶしなし）を描画
    circleColor(renderer, 200, 200, 50, 0x00000000); // 黒で描画？
    // 半円（塗りつぶしなし）を描画
    pieColor(renderer, 90, 90, 50, 0, 90, 0xffaaaaaa); // 0～90度の半円
    // 三角形（塗りつぶしあり）を描画
    filledTrigonColor(renderer, 100, 0, 150, 0, 125, 25, 0xff666666);
    // 文字列を描画
    stringColor(renderer, 0, 0, "TEST", 0xffffffff); // 白で描画

    SDL_RenderPresent(renderer); // 描画データを表示

    SDL_Delay(5000);


    // 終了処理
    SDL_DestroyRenderer(renderer); // RCの破棄（解放）
    SDL_DestroyWindow(window); // 生成したウィンドウの破棄（消去）
    SDL_Quit(); // SDL使用の終了
    return 0;
}

/*************************************************
演習
（１）サンプルプログラムの図形の色や形状を変更してみる
（２）現在のプログラミング環境（コンピュータ）のエンディアンを確認し，必要であれば，16進数による色指定を調整する
（３）α値を変更することによって，どのような描画になるか確認する
（４）SDL2_gfxの関数により，さまざまな図形を描画してみる
（参考 http://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/_s_d_l2__gfx_primitives_8h.html）
*************************************************/
