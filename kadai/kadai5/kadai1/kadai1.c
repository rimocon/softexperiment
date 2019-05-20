#include <stdio.h>
#include <SDL2/SDL.h> // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h> // TrueTypeフォントを表示するために必要なヘッダファイルをインクルード
// メイン関数
int main(int argc, char* argv[])
{
	int i;
	int Circx = 300;
	int Circy = 240;
	char buf[32];
	SDL_Window* window; // ウィンドウデータを格納する構造体
	SDL_Renderer* renderer; // レンダリングコンテキスト（描画設定）を格納する構造体
	Uint32 window_id; // ウィンドウID
	SDL_Surface* strings;
	SDL_Surface* surface; // サーフェイス（メインメモリ上の描画データ）を格納する構造体
	SDL_Texture* texture,*texture2; // テクスチャ（VRAM上の描画データ）を格納する構造体
	TTF_Font* font; // TrueTypeフォントデータを格納する構造体
	SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};	// フォントの色を指定するための構造体（白色）
	SDL_Color black = {0x00, 0x00, 0x00, 0x00}; //フォントの色を指定するための構造体(黒色)


	SDL_Event event; // SDLによるイベントを検知するための構造体
	SDL_Event quit_event = {SDL_QUIT}; // 特定のイベント名を格納

	// SDL初期化（ビデオとジョイスティック）
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// ウィンドウ生成・表示
	if((window = SDL_CreateWindow("Press [Esc] or [Tab] to exit",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,600,480,SDL_WINDOW_RESIZABLE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	window_id = SDL_GetWindowID(window); // ウィンドウIDを取得
	printf("Main Window ID=%d\n", window_id);

	// レンダリングコンテキスト（RC）作成
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_RenderClear(renderer);

	rectangleColor(renderer,400,380, 500, 480, 0xffff00ff);
	SDL_RenderPresent(renderer);
	TTF_Init(); // TrueTypeフォントを用いるための初期化
	font = TTF_OpenFont("./fonts-japanese-gothic.ttf", 16); // fonts-japanese-gothicフォントを12ポイントで使用（読み込み）

	// 文字列描画（表示）のための設定
	SDL_Rect src_rect = {0, 0, 100, 30}; // コピー元文字列の領域（x, y, w, h）（strings->wとstrings->hにはそれぞれ文字列の横幅と高さが格納される）
	SDL_Rect dst_rect = {0, 0, 100, 30}; // 文字列のコピー先の座標と領域（x, y, w, h）
	SDL_Rect src_rect2 = {0, 0, 100, 50}; // コピー元文字列の領域（x, y, w, h）（strings->wとstrings->hにはそれぞれ文字列の横幅と高さが格納される）
	SDL_Rect dst_rect2 = {230, 210, 150, 50}; // 文字列のコピー先の座標と領域（x, y, w, h）
	// RCテクスチャ作成
	SDL_RenderPresent(renderer);

	// イベント処理（無限ループ内でイベントを取得し，対応する処理を行う）
	while(1){
		// イベントを取得したなら
		char position[32];
		if(SDL_PollEvent(&event)){
			// イベントの種類ごとに処理
			switch (event.type) {
				case SDL_MOUSEMOTION: // マウスが移動した時
					sprintf(position,"x=%d y=%d",event.motion.x,event.motion.y);

					strings = TTF_RenderUTF8_Blended(font,(const char*)position,black); // 描画文字の作成と格納（白色のfonts-japanese-gothicフォントで，文字列をサーフェイスに描画＝データとして格納）
					printf("Mouse moved by %d,%d to (%d,%d)\n", event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y); // マウスの座標を表示
					break;
				case SDL_MOUSEBUTTONDOWN: // マウスのボタンが押された時
					printf("Mouse button %d pressed at (%d,%d)\n", event.button.button, event.button.x, event.button.y); // 押されたマウスのボタン等を表示
					// マウスのボタンごとに処理
					switch(event.button.button){
						case SDL_BUTTON_LEFT:
							Circx = event.motion.x;
							Circy = event.motion.y;
							printf("x=%d,y=%d\n",Circx,Circy);
							if(400 < Circx && Circx< 480 && 380 < Circy && Circy < 480 ){
								printf("GOOD JOB\n");
								surface = TTF_RenderUTF8_Blended(font,"Good Job!",black); // 描画文字の作成と格納（白色のfonts-japanese-gothicフォントで，文字列をサーフェイスに描画＝データとして格納）
							}
							else{
								surface =TTF_RenderUTF8_Blended(font,"Good Job!",white);
							}
							printf("Left button pressed\n");
							break;
						case SDL_BUTTON_MIDDLE:
							printf("Middle button pressed\n");
							break;
						case SDL_BUTTON_RIGHT:
							printf("Right button pressed\n");
							break;
					}
					break;
				case SDL_KEYDOWN: // キーボードのキーが押された時
					// 押されたキーの名前等を表示
					printf("The pressed key is %s.\n", SDL_GetKeyName(event.key.keysym.sym));
					printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), event.key.keysym.mod);
					// 押されたキーごとに処理
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE: // Escキーが押された時
							printf("ESCAPE key -> EXIT.\n");
							SDL_Quit();
							exit(0);
							break;
						case SDLK_TAB:	// Tabキーが押された時
							SDL_PushEvent(&quit_event); // イベントキューにSDL_QUITイベントを追加（→プログラム終了）
							break;
					}
					break;
				case SDL_QUIT:
					TTF_CloseFont(font); // 使用したフォントを閉じる
					TTF_Quit();	// TTFライブラリの使用を終了
					SDL_FreeSurface(strings); // サーフェイスの解放
					SDL_FreeSurface(surface); // サーフェイスの解放
					SDL_DestroyTexture(texture); // テクスチャの解放
					SDL_DestroyTexture(texture2); // テクスチャの解放
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					SDL_Quit();
					exit(0);
					break;
			}
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			SDL_RenderClear(renderer);

			rectangleColor(renderer,400,380, 500, 480, 0xffff00ff);
			filledCircleColor(renderer,Circx,Circy, 10, 0xffff00ff);
			texture = SDL_CreateTextureFromSurface(renderer, strings); // サーフェイス（文字列の描画データが格納されている）からRCのテクスチャを生成（サーフェイスからテクスチャへ変換）
			SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect); // テクスチャをRCにコピー（設定のサイズで）
			texture2 = SDL_CreateTextureFromSurface(renderer, surface); // サーフェイス（文字列の描画データが格納されている）からRCのテクスチャを生成（サーフェイスからテクスチャへ変換）
			SDL_RenderCopy(renderer, texture2, &src_rect2, &dst_rect2); // テクスチャをRCにコピー（設定のサイズで)

			SDL_RenderPresent(renderer);

			SDL_RenderPresent(renderer);
		}
	}
}

