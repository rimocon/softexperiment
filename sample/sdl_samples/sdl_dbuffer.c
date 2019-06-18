/*************************************************
sdl_dbuffer.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lSDL_image -L/usr/lib
備考：画像は適当に用意してください．
　bg.jpg　背景画像（640*480）
　chara1.png　キャラクタ画像1（40*40で、アルファチャンネルで背景透過した画像）
　chara2.png　キャラクタ画像2（40*40で、アルファチャンネルで背景透過した画像）
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード

SDL_Surface *window; // ウィンドウ用サーフェイス
SDL_Surface *buffer;	// ダブルバッファリング用サーフェイス

// 画像の座標とサイズ
SDL_Rect src_rect = { 0, 0, 40, 40 }; // コピー元画像のコピーする一領域の座標とサイズ（x, y, w, h）
SDL_Rect dst_rect = { 0, 0 }; // 画像のコピー先の座標（x, y）

// キャラクタ一体ずつウィンドウに描いていく方法（ちらつきがある）
void drawChara1(SDL_Surface *image1, SDL_Surface *image2){
	int i, j;

	SDL_BlitSurface(image1, NULL, window, NULL);	// 背景画像をウィンドウに貼り付ける
	for(i=0; i<= 600; i=i+40){
		dst_rect.x=i;
		for(j=0; j<=440; j=j+40){
			dst_rect.y=j;
			SDL_BlitSurface(image2, &src_rect, window, &dst_rect);	// ウィンドウに画像を貼り付ける
			SDL_Flip(window);	// 画面に画像を表示（反映）
		}
	}
}

// キャラクタすべてを一度にウィンドウに描く方法（ダブルバッファリング的．ちらつきなし）
void drawChara2(SDL_Surface *image1, SDL_Surface *image2){
	int i, j;

	SDL_BlitSurface(image1, NULL, window, NULL);	// 背景画像をウィンドウに貼り付ける
	for(i=0; i<= 600; i=i+40){
		dst_rect.x=i;
		for(j=0; j<=440; j=j+40){
			dst_rect.y=j;
			SDL_BlitSurface(image2, &src_rect, window, &dst_rect);	// ウィンドウに画像を貼り付ける
		}
	}
	SDL_Flip(window);	// 画面に画像を表示（反映）
}

// 非可視サーフェイス（バッファ）に描く方法（ダブルバッファリング．ちらつきなし）
void drawCharaByDBuffer(SDL_Surface *image1, SDL_Surface *image2){
	int i, j;

	SDL_BlitSurface(image1, NULL, buffer, NULL);	// 背景画像をバッファに貼り付ける
	for(i=0; i<= 600; i=i+40){
		dst_rect.x=i;
		for(j=0; j<=440; j=j+40){
			dst_rect.y=j;
			SDL_BlitSurface(image2, &src_rect, buffer, &dst_rect);	// キャラクタ画像をバッファに画貼り付ける
		}
	}
	// ここではバッファ内容をウィンドウに反映（表示）しない
}


// メイン関数
int main(int argc, char* argv[]) {
	SDL_Surface *image1, *image2, *image3;
	SDL_Event event;
	SDL_Rect rect = {0, 0, 640, 480};
	int frame=0;	// 画像切替用変数

	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}
	// ウィンドウ生成
	if((window = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}
	
	// 画像の読み込み
	image1=IMG_Load("bg.jpg");	// 背景画像
	image2=IMG_Load("chara1.png");	// キャラクタ画像1
	image3=IMG_Load("chara2.png");	// キャラクタ画像2

	buffer = SDL_CreateRGBSurface(SDL_HWSURFACE, window->w, window->h, 32, 0, 0, 0, 0);
//	buffer = SDL_DisplayFormat(buffer);	// サーフェイスを表示用フォーマットに変換

	while(1){
		if(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_KEYDOWN:	// キーボードのキーが押された時
				switch(event.key.keysym.sym){
				case SDLK_ESCAPE:	// Escキーが押された時
					printf("ESCAPE key -> EXIT.\n");
					SDL_Quit();
					exit(0);
					break;
				// キャラクタを1体ずつ描く方法
				case SDLK_UP:	// 上キーを押すごとにキャラクタが変わる
					if(frame==0){
						drawChara1(image1, image2);
						frame=1;
					}
					else{
						drawChara1(image1, image3);
						frame=0;
					}
					break;
				// すべてのキャラクタを一度に描く方法
				case SDLK_DOWN:	// 下キーを押すごとにキャラクタが変わる
					if(frame==0){
						drawChara2(image1, image2);
						frame=1;
					}
					else{
						drawChara2(image1, image3);
						frame=0;
					}
					break;
				// ダブルバッファリング（裏ですべてのキャラクタを描画．表示まではしない）
				case SDLK_RIGHT:	// 右キーを押すごとにキャラクタが変わる（この段階では見えない）
					if(frame==0){
						drawCharaByDBuffer(image1, image2);
						frame=1;
					}
					else{
						drawCharaByDBuffer(image1, image3);
						frame=0;
					}
					break;
				// ダブルバッファリング（バッファ内容をウィンドウへ表示（反映））
				case SDLK_LEFT:
					SDL_BlitSurface(buffer, NULL, window, NULL);	// ダブルバッファに描画しておいた画像をウィンドウに貼り付ける
					SDL_Flip(window);	// 画面に画像を表示（反映）
					break;
				}
			}
		}
	}
			
	SDL_FreeSurface(window);	// ウィンドウ（サーフェイス）の解放
	SDL_FreeSurface(image1);	// 画像（サーフェイス）の解放
	SDL_FreeSurface(image2);	// 画像（サーフェイス）の解放

	SDL_Quit();	// SDLの利用終了
	return 0;
}

