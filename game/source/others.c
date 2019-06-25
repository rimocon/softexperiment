#include"header.h"
extern int games; //ゲーム状態
extern SDL_Window *mainwindow;　//メイン画面用ウィンドウ
extern SDL_Surface *mainsurface; //メイン画面用サーフェイス
extern SDL_Renderer *mainrenderer; //メイン画面用レンダラー
extern SDL_Texture *maintexture; //メイン画面用テクスチャー
extern SDL_Surface *img; //画像読み込み用

/*********初期化など*********/
timers timer; //時間計測用

#define framerate 16
//メインウィンドウの作成
void startup() {
	//SDLの初期化がうまくいかなかったら強制終了
	if (SDL_Init(SDL_INIT_VIDEO) == -1) SDL_Quit(); 
	//ウィンドウを作成(300*400),ビットごとピクセル32,システムメモリ内に画面作成(ビデオサーフェイスを作成)
	mainwindow = SDL_CreateWindow("inbeida",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,400,320,0);
	//ウィンドウにタイトルとアイコンをつける(NULLなのでアイコンなし)
	mainsurface = SDL_GetWindowSurface(mainwindow);
	//ゲームの状態を判別する変数(とりあえず0)
	games = 0;
}

//終了用関数
int quits() {
	SDL_Event Qevents;
	//イベントを取得したら
	if (SDL_PollEvent(&Qevents)) {
		switch (Qevents.type) {
			case SDL_QUIT:
				return 0;
				break;
			//キーが押されたら
			case SDL_KEYDOWN:
			//Escキーが押されたら終了
				if ((Qevents.key.keysym.sym) == SDLK_ESCAPE)
					return 0;
					break;
			default:
				break;
		}
	}
	return 1;
}

//フレームレート用
void frames() {
	timer.now = SDL_GetTicks(); //現在時間を取得
	timer.wait = timer.now - timer.leave; //待ち時間を計算

	if(timer.wait < framerate)
		SDL_Delay(framerate - timer.wait); //framerate以下ならCPUを休ませる
	timer.leave = SDL_GetTicks(); //経過時間を更新
	SDL_UpdateWindowSurface(mainwindow); //画面を更新
}

void background() {
	mainrenderer = SDL_CreateRenderer(mainwindow,-1,0); //メインウィンドウへのレンダラーを生成
	SDL_SetRenderDrawColor(mainrenderer,255,0,0,0);
	SDL_RenderPresent(mainrenderer);
}

void imageload() {
	img = IMG_Load("background.png"); //背景画像を読み込み
	img_src = {0,0,img->w,img->h} //コピー元画像の領域
	if(img) SDL_Quit(); 
}
void drawtitle(sint16 posX,sint16 posY) {
	img_dst.x = posX;
	img_dst.y = posY;


