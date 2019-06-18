/*************************************************
sdl_animation.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lSDL_image -L/usr/lib
備考：
　キャラクタ用画像は400*100以上で、100*100の正方形の領域ごとにキャラクタを描いてください．
　背景画像は適当に用意してください．
　なお、この例のような方法がアニメーション処理に最適であるとは限りません（いくつかのトピックを説明するための教材として作成しています）．自分なりのアニメーション処理を考案してください．
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード

// 構造体のanimation_data型を作成
typedef struct{
	SDL_Surface *character_image, *bg_image;	// 画像データへのポインタ（キャラクタ用と背景用）
	SDL_Rect src_rect;	// キャラクタ等の貼り付け元の画像の情報を格納
	SDL_Rect dst_rect;	// キャラクタ等の貼り付け先の画像の情報を格納
	int frame;	// キャラクタの現在（貼り付ける）のフレーム番号
	int last_frame;	// キャラクタの最後のフレーム番号
} animation_data;

SDL_Surface *window; // ウィンドウ（画像）データへのポインタ（グローバル）
animation_data animation;	// animation_data型構造体

// タイマーで呼び出されるコールバック関数
Uint32 callbackfunc(Uint32 interval, void *param){
	animation_data *anim = (animation_data*)param;	// animation_data型で渡されたパラメータ（引数）を格納
	static SDL_Rect previous_character;	//　前回キャラクタを貼り付けた座標とサイズ
	
	previous_character.w = anim->src_rect.w;	// キャラクタの横幅を格納
	previous_character.h = anim->src_rect.h;	// キャラクタの高さを格納

	SDL_BlitSurface(anim->bg_image, &previous_character, window, &previous_character);	// 前回キャラクタを貼り付けた座標に背景画像を貼り直す（これがないと、キャラクタが重ね書きされてしまう）
 
	// 貼り付けるキャラクタのフレームを選択（フレームの左上座標を取得）
	if(anim->frame >= 0 && anim->frame < anim->last_frame){
		anim->src_rect.x = anim->frame * 100;	// フレームごとにx座標を100ずらす
		anim->frame++;	// フレーム番号を更新する
	}
	else{
		anim->src_rect.x = anim->frame * 100;
		anim->frame=0;	// フレーム番号を0に戻す
	}

	SDL_BlitSurface(anim->character_image, &anim->src_rect, window, &anim->dst_rect);	// フレーム番号に対応する画像の一領域をウィンドウに貼り付ける

	// キャラクタの貼り付け座標を格納（前回キャラクタを貼り付けた座標として）
	previous_character.x = anim->dst_rect.x;
	previous_character.y = anim->dst_rect.y;

	SDL_Flip(window);	// 画面にアニメーションを表示（反映）
	
	return interval;	// コールバックが呼び出される間隔を返す（必須）
}

// メイン関数
int main(int argc, char* argv[]) {

	SDL_Event event; // SDLによるイベントを検知するための構造体
	SDL_TimerID timer_id;	//　タイマ割り込みを行うためのタイマのID

	// SDL初期化
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// ウィンドウ生成（800*600、1677万色）
	if((window = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	// アニメーションの背景画像
	animation.bg_image=IMG_Load("background.jpg");	// アニメーション背景画像の読み込み
	SDL_BlitSurface(animation.bg_image, NULL, window, NULL);	// 読み込んだ画像のウィンドウへの貼り付け

	// アニメーションのキャラクタ画像
	animation.character_image=IMG_Load("character.png");	// アニメーションのキャラクタ画像の読み込み
	SDL_SetColorKey(animation.character_image, SDL_SRCCOLORKEY, -1);	// 読み込んだ画像にアルファチャンネル透過を指定

	// キャラクタ画像（コピー元）の大きさ（横幅と高さ）を指定
	animation.src_rect.w=100;
	animation.src_rect.h=100;
	// 画像のコピー先の座標（初期座標）
	animation.dst_rect.x=0;
	animation.dst_rect.y=0;
	// コピー元の現在のフレーム（領域）番号
	animation.frame=0;
	// コピー元の最後のフレーム（領域）番号
	animation.last_frame=3;

	timer_id=SDL_AddTimer(1000, callbackfunc, &animation);	// 1秒ごとにコールバック関数を呼び出す（渡す引数は&animation）

	// 無限ループ
	while(1){
		// イベント検知
		if(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				printf("Terminated.\n");
				SDL_RemoveTimer(timer_id);	// タイマの削除
				SDL_FreeSurface(window);	//　ウィンドウの解放
				SDL_Quit();	// SDLの利用終了
				return 0;
				break;
			case SDL_KEYDOWN:
				// 十字キー操作でキャラクタを移動
				switch(event.key.keysym.sym){
				case SDLK_UP:
					animation.dst_rect.y-=50;
					break;
				case SDLK_DOWN:
					animation.dst_rect.y+=50;
					break;
				case SDLK_RIGHT:
					animation.dst_rect.x+=50;
					break;
				case SDLK_LEFT:
					animation.dst_rect.x-=50;
					break;
				}
				break;
			}
		}
	}
}

