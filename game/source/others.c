#include"header.h"
/********ヘッダファイルからの読み込み******/
extern int games; //ゲーム状態
extern SDL_Window *mainwindow; //メイン画面用ウィンドウ
extern SDL_Surface *mainsurface; //メイン画面用サーフェイス
extern SDL_Renderer *mainrenderer; //メイン画面用レンダラー
extern SDL_Texture *maintexture; //メイン画面用テクスチャー
extern SDL_Surface *img; //画像読み込み用サーフェイス
extern SDL_Rect img_src; //ソースの矩形(画像用)
extern SDL_Rect img_dst; //貼り付け先の矩形(画像用)
extern SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体

/*********初期化など*********/
timers timer; //時間計測用
inputkeys keys; //キー入力用

/************定義************/
#define framerate 16
#define windowwidth 1280
#define windowheight 960

//メインウィンドウの作成
void startup() {
	//SDLの初期化がうまくいかなかったら強制終了
	if (SDL_Init(SDL_INIT_VIDEO) == -1) SDL_Quit(); 
	//ウィンドウを作成(1280*960),ビットごとピクセル32,システムメモリ内に画面作成(ビデオサーフェイスを作成)
	mainwindow = SDL_CreateWindow("inbeida",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowwidth,windowheight,0);
	//ウィンドウにタイトルとアイコンをつける(NULLなのでアイコンなし)
	//mainsurface = SDL_CreateRenderer(mainwindow, -1 , 0);
	//ゲームの状態を判別する変数(とりあえず0)
	games = 0;
	imageload();
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
//背景用(単色のみ),正直画像で重ねるからいらないが,透過画像とかで背景使う時用
void background() {
	mainrenderer = SDL_CreateRenderer(mainwindow,-1,0); //メインウィンドウへのレンダラーを生成
	if (games == 0){
		SDL_SetRenderDrawColor(mainrenderer,255,0,0,0); //生成したレンダラーの描画色として赤を設定
	}
	else if (games == 1) {
		SDL_SetRenderDrawColor(mainrenderer,0,0,0,0); //生成したレンダラーの描画色として黒を設定
	}
	SDL_RenderClear(mainrenderer); //生成したレンダラーをクリア
	SDL_RenderPresent(mainrenderer); //描画データを表示
}
//画像読み込み関数
void imageload() {
	img = IMG_Load("./images/background.png"); //背景画像(サーフェイスへの)読み込み
	if(!img) SDL_Quit(); //もし読み込めなかったら終了する
}

//タイトル描画用
void drawtitle(Sint16 posX,Sint16 posY) {
	//コピー元画像の領域を定義
	img_src.x = 0;
	img_src.y = 0;
	img_src.w = img->w;
	img_src.h = img->h; 

	img_dst.x = posX;
	img_dst.y = posY;
	/* 背景確認用
		img_dst.w = 800;
		img_dst.h = 600;
	 */
	img_dst.w = windowwidth;
	img_dst.h = windowheight;
	maintexture = SDL_CreateTextureFromSurface(mainrenderer, img); //読み込んだ画像からテクスチャを作成
	SDL_RenderCopy(mainrenderer,maintexture, &img_src, &img_dst); //テクスチャをレンダラーにコピー
	SDL_RenderPresent(mainrenderer); //描画データを表示
}
//入力用関数
void input() {
	// 接続されているジョイスティックの名前を表示
	for(i=0; i<SDL_NumJoysticks(); i++){ // 接続されているジョイスティックの数だけ繰り返す
		joystick = SDL_JoystickOpen(i); // ジョイスティックを開く
		printf("%s\n", SDL_JoystickName(joystick)); // 開いたジョイスティック名を表示
	}

	// ジョイスティックが開けなかったら
	if(!joystick) {
		printf("failed to open joystick.\n");
		exit(-1);
	}
	else{
		printf("The joystick has %d axses.\n",SDL_JoystickNumAxes(joystick)); // 方向キー数を取得
		printf("The joystick has %d buttons.\n",SDL_JoystickNumButtons(joystick)); // ボタン数を取得
		printf("The joystick has %d Hat keys.\n",SDL_JoystickNumHats(joystick)); // Hatキー数を取得
		printf("The joystick has %d balls.\n",SDL_JoystickNumBalls(joystick)); // ボール数を取得
	}
	// ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
	case SDL_JOYAXISMOTION:
	printf("The axis ID of the operated key is %d.\n",event.jaxis.axis); // 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
	if(event.jaxis.axis==0){
		printf("--- Analog-Direction Key: 0 Axis\n");
	}
	else if(event.jaxis.axis==1){
		printf("--- Anolag-Direction Key: 1 Axis\n");
	}
	else if(event.jaxis.axis==2){
		printf("--- Four-Direction Key: Horizontal Axis\n");
	}
	else if(event.jaxis.axis==3){
		printf("--- Four-Direction Key: Vertical Axis\n");
	}
	printf("--- The axis value of the operated key is %d.\n",event.jaxis.value); // ジョイスティックの操作された方向キーの値を表示（-32767（真左，真上）～32767（真右，真下））
	break;
	/*
	// ジョイスティックのボタンが押された時
	case SDL_JOYBUTTONDOWN:
	printf("The ID of the pressed button is %d.\n", event.jbutton.button); // 押されたボタンのIDを表示（0から）
	// ボタンIDに応じた処理
	if(event.jbutton.button==0){
		printf("--- You pressed a button on the joystick.\n");
	}
	break;
	// ボタンが離された時
	case SDL_JOYBUTTONUP:
	printf("The ID of the released button is %d.\n", event.jbutton.button); // 離されたボタンのIDを表示（0から）
	// ボタンIDに応じた処理
	if(event.jbutton.button==0){
		printf("--- You released a button on the joystick.\n");
	}
	break;
	*/
}
