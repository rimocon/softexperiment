/*************************************************
sdl2_font.c
Programmed by H. Mitsuhara (April, 2018)

コンパイルオプション（リンク対象）：-lSDL2 -lSDL2_ttf
動作：
　拡大した文字列（TrueTypeフォント）を表示する
備考：
　TrueTypeフォント（*.ttf）は/usr/share/fonts以下にあるはず
　フォントファイルをパスで指定するか，カレントディレクトリにコピーしておく
　フリーのTrueTypeフォントをダウンロードして使ってもよい
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL_ttf.h> // TrueTypeフォントを表示するために必要なヘッダファイルをインクルード

// メイン関数（コマンドライン引数を受け取る）
int main(int argc, char *argv[])
{
    SDL_Window* window; // ウィンドウデータを格納する構造体
    SDL_Renderer* renderer; // レンダリングコンテキスト（描画設定）を格納する構造体
    SDL_Surface* strings; // サーフェイス（メインメモリ上の描画データ）を格納する構造体
    SDL_Texture* texture; // テクスチャ（VRAM上の描画データ）を格納する構造体
    TTF_Font* font; // TrueTypeフォントデータを格納する構造体
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};	// フォントの色を指定するための構造体（白色）

    // SDL初期化
    SDL_Init(SDL_INIT_VIDEO);

    // ウィンドウ生成・表示
    window = SDL_CreateWindow("Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,0);

    // レンダリングコンテキスト（RC）作成
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    // 出力デバイスの違いに対応
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // 拡大縮小を滑らかにする（線形フィルタで）
    SDL_RenderSetLogicalSize(renderer, 640, 480); // デバイスに依存しない分解能を設定

    
    // 文字列描画処理
    TTF_Init(); // TrueTypeフォントを用いるための初期化
    font = TTF_OpenFont("./fonts-japanese-gothic.ttf", 12); // fonts-japanese-gothicフォントを12ポイントで使用（読み込み）
    strings = TTF_RenderUTF8_Blended(font, "Hello!\nこんにちは！改行するには？…", white); // 描画文字の作成と格納（白色のfonts-japanese-gothicフォントで，文字列をサーフェイスに描画＝データとして格納）

    // RCテクスチャ作成
    texture = SDL_CreateTextureFromSurface(renderer, strings); // サーフェイス（文字列の描画データが格納されている）からRCのテクスチャを生成（サーフェイスからテクスチャへ変換）

    // 文字列描画（表示）のための設定
    SDL_Rect src_rect = {0, 0, strings->w, strings->h}; // コピー元文字列の領域（x, y, w, h）（strings->wとstrings->hにはそれぞれ文字列の横幅と高さが格納される）
    SDL_Rect dst_rect = {0, 0, 640, 480}; // 文字列のコピー先の座標と領域（x, y, w, h）
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect); // テクスチャをRCにコピー（設定のサイズで）
//    SDL_RenderCopyEx(renderer, texture, &src_rect, &dst_rect, 45, NULL, SDL_FLIP_HORIZONTAL); // テクスチャをその中心で45度回転させ水平方向に反転させてRCにコピー

    SDL_RenderPresent(renderer); // 描画データを表示

    SDL_Delay(5000);

    // 終了処理
    TTF_CloseFont(font); // 使用したフォントを閉じる
    TTF_Quit();	// TTFライブラリの使用を終了
    SDL_FreeSurface(strings); // サーフェイスの解放
    SDL_DestroyTexture(texture); // テクスチャの解放
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


/*************************************************
【演習】
（１）好みのフォントを描画する
（２）「出力デバイスの違いに対応」の2つの関数をコメントアウトした際の動作を確認する
（３）フォントを適切なサイズで描画する（拡大しすぎないように）
（４）SDL_RenderCopyEx関数により文字列を回転・反転させる
*************************************************/
