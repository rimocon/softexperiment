#include"header.h"
/********ヘッダファイルからの読み込み******/
extern int games; //ゲーム状態
extern bool titledraw; //一回だけ描画するよう
extern bool gamedraw; //一回だけ描画するよう
extern SDL_Window *mainwindow; //メイン画面用ウィンドウ
extern SDL_Surface *mainsurface; //メイン画面用サーフェイス
extern SDL_Renderer *mainrenderer; //メイン画面用レンダラー
extern SDL_Texture *maintexture; //メイン画面用テクスチャー
extern SDL_Texture *metexture; //キャラ画像用テクスチャー

extern SDL_Surface *img; //タイトル背景画像読み込み用サーフェイス
extern SDL_Surface *meimg; //自キャラ画像読み込み用サーフェイス
extern SDL_Surface *gameimg; //ゲーム画面背景画像読み込み用サーフェイス
extern SDL_Rect img_src; //ソースの矩形(画像用)
extern SDL_Rect img_dst; //貼り付け先の矩形(画像用)
extern SDL_Rect meimg_src; //ソースの矩形(画像用)
extern SDL_Rect meimg_dst; //貼り付け先の矩形(画像用)

extern SDL_Event inputevent;
extern SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体

/*********初期化など*********/
timers timer; //時間計測用
inputkeys keys; //入力キー用
mychara me; //自キャラ用

/************定義************/
#define framerate 15
#define windowwidth 1280
#define windowheight 960

//メインウィンドウの作成
void startup() {
	//SDLの初期化がうまくいかなかったら強制終了
	if (SDL_Init(SDL_INIT_VIDEO) == -1) SDL_Quit(); 
	//ウィンドウを作成(1280*960),ビットごとピクセル32,システムメモリ内に画面作成(ビデオサーフェイスを作成)
	mainwindow = SDL_CreateWindow("inbeida",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowwidth,windowheight,0);
	//メインウィンドウに対するレンダラーを生成
	mainrenderer = SDL_CreateRenderer(mainwindow,-1,0); 
	SDL_SetRenderDrawColor(mainrenderer,0,0,0,0); //生成したレンダラーの描画色として黒を設定
	SDL_RenderClear(mainrenderer); //生成したレンダラーをクリア
	SDL_RenderPresent(mainrenderer); //描画データを表示
	//ゲームの状態を判別する変数(とりあえず0)
	games = 0;
	SDL_Init(SDL_INIT_JOYSTICK); //SDL初期化
	// 接続されているジョイスティックの名前を表示
	if(SDL_NumJoysticks() > 0) {
		joystick = SDL_JoystickOpen(0); // ジョイスティックを開く
		printf("%s\n", SDL_JoystickName(joystick)); //開いたジョイスティック名を表示
	}
	// ジョイスティックが開けなかったら
	if(!joystick) {
		printf("failed to open joystick.\n");
		exit(-1);
	}

	imageload(); //画像を読み込み
	timer.leave = SDL_GetTicks();
	//一回だけ画面を表示するよう
	titledraw = true;
	gamedraw = true;
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

	if(timer.wait < framerate) {
		SDL_Delay(framerate - timer.wait); //framerate以下ならCPUを休ませる
	}
	timer.leave = SDL_GetTicks(); //経過時間を更新
	SDL_UpdateWindowSurface(mainwindow); //画面を更新
}

//画像読み込み関数
void imageload() {
	img= IMG_Load("./images/background2.png"); //背景画像(サーフェイスへの)読み込み
	if(!img) SDL_Quit(); //もし読み込めなかったら終了する
	meimg= IMG_Load("./images/me.png"); //自キャラ画像(サーフェイスへの)読み込み
	if(!meimg) SDL_Quit(); //もし読み込めなかったら終了する
	gameimg = IMG_Load("./images/gamebackground.png"); //ゲーム画面背景画像(サーフェイスへの)読み込み
	if(!gameimg) SDL_Quit(); //もし読み込めなかったら終了する

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
	// 背景確認用
	/*
		img_dst.w = 800;
		img_dst.h = 600;
	 */
	img_dst.w = windowwidth;
	img_dst.h = windowheight;
	printf("title\n");
	maintexture = SDL_CreateTextureFromSurface(mainrenderer, img); //読み込んだ画像からテクスチャを作成
	SDL_RenderCopy(mainrenderer,maintexture, &img_src, &img_dst); //テクスチャをレンダラーにコピー
	SDL_RenderPresent(mainrenderer); //描画データを表示
	titledraw = false; //一回だけ表示したいのでfalseに
}

//入力用関数
void input() {
	switch (inputevent.type) {
		// ジョイスティックの方向キーまたはアナログキー（スティック)が押された時
		case SDL_JOYAXISMOTION:
			printf("The axis ID of the operated key is %d.\n",inputevent.jaxis.axis); // 操作された方向キーの方向軸を表示（0：アナログキー，1：アナログキー，2：方向キー左右方向，3：方向キー上下方向）
			if(inputevent.jaxis.axis==0){
				printf("--- Analog-Direction Key: 0 Axis\n");
				if(inputevent.jaxis.value > 0) { //右キーが押されたら
					printf("press right\n"); //確認用
					keys.right = 1;
					keys.left = 0;
					//keys.up = 0;
					//keys.down = 0;
				}
				else if(inputevent.jaxis.value < 0) { //左キーが押されたら
					printf("press left\n");
					keys.right = 0;
					keys.left = 1;
					//keys.up = 0;
					//keys.down = 0;
				}
				else if(inputevent.jaxis.value == 0) { //真ん中にスティックが戻ったら
					printf("reverse center\n");
					keys.right = 0;
					keys.left = 0;
				}
			}
			else if(inputevent.jaxis.axis==1){
				printf("--- Analag-Direction Key: 1 Axis\n");
				if(inputevent.jaxis.value > 0) { //下キーが押されたら
					printf("press down\n"); //確認用
					//keys.right = 0;
					//keys.left = 0;
					keys.up = 0;
					keys.down = 1;
				}
				else if(inputevent.jaxis.value < 0) { //上キーが押されたら
					printf("press up\n"); //確認用
					//keys.right = 0;
					//keys.left = 0;
					keys.up = 1;
					keys.down = 0;
				}
				else if(inputevent.jaxis.value == 0) { //真ん中にスティックが戻ったら
					printf("reverse center\n");
					keys.up= 0;
					keys.down= 0;
				}
			}
			else if(inputevent.jaxis.axis==2){
				printf("--- Four-Direction Key: Horizontal Axis\n");
			}
			else if(inputevent.jaxis.axis==3){
				printf("--- Four-Direction Key: Vertical Axis\n");
			}
			break;
			// ジョイスティックのボタンが押された時
		case SDL_JOYBUTTONDOWN:
			printf("The ID of the pressed button is %d.\n", inputevent.jbutton.button); // 押されたボタンのIDを表示（0から）
			// ボタンIDに応じた処理
			if(inputevent.jbutton.button==0){
				printf("--- You pressed a button on the joystick.\n");
			}
			if(inputevent.jbutton.button==5){ //発射ボタンが押された
				printf("発射!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				keys.z = 1; //発射キーが押された
			}
			break;
			// ボタンが離された時
		case SDL_JOYBUTTONUP:
			printf("The ID of the released button is %d.\n",inputevent.jbutton.button); // 離されたボタンのIDを表示（0から）
			// ボタンIDに応じた処理
			if(inputevent.jbutton.button==0){
				printf("--- You released a button on the joystick.\n");
			}
			break;

			/*******************キーボードの場合*************************/
			/*
				case SDL_KEYDOWN: // キーボードのキーが押された時
			// 押されたキーの名前等を表示
			printf("The pressed key is %s.\n", SDL_GetKeyName(inputevent.key.keysym.sym));
			printf("Keysym Mode=%x (SDL_GetModState), %x (event/key/keysym.mode)\n", SDL_GetModState(), inputevent.key.keysym.mod);
			// 押されたキーごとに処理
			switch(inputevent.key.keysym.sym){
			case	SDLK_UP:
			printf("press up\n");
			keys.right = 0;
			keys.left = 0;
			keys.up = 1;
			keys.down = 0;

			//me.posY -= 2;
			//if(me.posY < 0) me.posY = 0;
			break;
			case SDLK_DOWN:
			printf("press down\n");

			keys.right = 0;
			keys.left = 0;
			keys.up = 0;
			keys.down = 1;

			//me.posY += 2;
			//if(me.posY > windowheight -64) me.posY = windowheight - 64;
			break;
			case SDLK_RIGHT:
			printf("press right\n");

			keys.right = 1;
			keys.left = 0;
			keys.up = 0;
			keys.down = 0;

			//me.posX += 2;
			//if (me.posX > windowwidth -64) me.posX = windowwidth - 64;
			break;
			case SDLK_LEFT:
			printf("press left\n");

			keys.right = 0;
			keys.left = 1;
			keys.up = 0;
			keys.down = 0;

			//if (me.posX < 0) me.posX = 0; 
			//break;
			case SDLK_z:
			printf("発射!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			keys.z = 1; //発射キーが押された
			}
			break;
			}
			 */
}
}

//ゲームスタート用
void startGame() {
	if(keys.z == 1) {
		games = 1; //Zキーが押されたらゲームへ
		reset(); //色々リセット
	}
}

//ゲーム背景描画用
void drawgame(Sint16 posX,Sint16 posY) {
	//メインウィンドウに対するレンダラーを生成
	//コピー元画像の領域を定義
	img_src.x = 0;
	img_src.y = 0;
	img_src.w = gameimg->w;
	img_src.h = gameimg->h; 

	img_dst.x = posX;
	img_dst.y = posY;
	/* 背景確認用
		img_dst.w = 800;
		img_dst.h = 600;

		printf("game\n");
	 */
	img_dst.w = windowwidth;
	img_dst.h = windowheight;

	//SDL_RenderClear(mainrenderer); //生成したレンダラーをクリア
	maintexture = SDL_CreateTextureFromSurface(mainrenderer, gameimg); //読み込んだ画像からテクスチャを作成
	SDL_RenderCopy(mainrenderer,maintexture, &img_src, &img_dst); //テクスチャをレンダラーにコピー
	//SDL_RenderPresent(mainrenderer); //描画データを表示

	//コピー元画像の領域を定義
	meimg_src.x = 0;
	meimg_src.y = 0;
	meimg_src.w = 64;
	meimg_src.h = 64; 

	//コピー先画像の領域を定義
	meimg_dst.x = windowwidth / 2 - 64;
	meimg_dst.y = 860;
	meimg_dst.w = 64;
	meimg_dst.h = 64;

	metexture = SDL_CreateTextureFromSurface(mainrenderer,meimg); //読み込んだ画像からテクスチャを作成
	SDL_RenderCopy(mainrenderer,metexture, &meimg_src, &meimg_dst); //テクスチャをレンダラーにコピー
	SDL_RenderPresent(mainrenderer); //描画データを表示
	gamedraw = false; //一回だけ表示したいのでfalseに

}
//色々リセット用
void reset() {
	me.posX = windowwidth /2 - 64;
	me.posY = 800;
	int loop;
	for (loop = 0;loop < bulletmax ;loop++){
		me.flagB[loop] = 0; //フラグを下げる
	}
}

//自キャラ座標計算用
void me_move() {
	/***************押されたキーによって座標を計算************/
	/*
		if(keys.left == 1) me.posX -= 2; //左キーが押されたら左に移動
		if(keys.right == 1) me.posX += 2; //右キーが押されたら右に移動
		if(keys.up == 1) me.posY -= 2; //上キーが押されたら上に移動
		if(keys.down == 1) me.posY += 2; //下キーがおされたら下に移動
	 */
	if(keys.left == 1) me.posX --; //左キーが押されたら左に移動
	if(keys.right == 1) me.posX ++; //右キーが押されたら右に移動
	if(keys.up == 1) me.posY --; //上キーが押されたら上に移動
	if(keys.down == 1) me.posY ++; //下キーがおされたら下に移動
	/*********画面外にでないようにする処理****************/
	if(me.posX > windowwidth - 64) me.posX = windowwidth - 64; 
	else if(me.posX < 0 ) me.posX = 0;

	if(me.posY > windowheight - 64) me.posY = windowheight - 64; 
	else if(me.posY < 0) me.posY = 0;
	printf("posX = %d\n",me.posX);
	printf("posY = %d\n",me.posY);

}

//自キャラ描画	
void me_draw() {
	/*
	//コピー元画像の領域を定義
	img_src.x = 0;
	img_src.y = 0;
	img_src.w = gameimg->w;
	img_src.h = gameimg->h; 

	//コピー先画像の領域を定義
	img_dst.x = 0;
	img_dst.y = 0;
	img_dst.w = windowwidth;
	img_dst.h = windowheight;
	 */
	//SDL_RenderClear(mainrenderer); //生成したレンダラーをクリア
	//maintexture = SDL_CreateTextureFromSurface(mainrenderer, gameimg); //読み込んだ画像からテクスチャを作成
	SDL_RenderClear(mainrenderer); //レンダラーをクリア
	SDL_RenderCopy(mainrenderer,maintexture, &img_src, &img_dst); //テクスチャをレンダラーにコピー
	//SDL_RenderPresent(mainrenderer); //描画データを表示

	/*
	//コピー元画像の領域を定義
	img_src.x = 0;
	img_src.y = 0;
	img_src.w = 64;
	img_src.h = 64; 
	 */
	//コピー先画像の領域を定義
	meimg_dst.x = me.posX;
	meimg_dst.y = me.posY;
	//meimg_dst.w = 64;
	//meimg_dst.h = 64;

	//charatexture = SDL_CreateTextureFromSurface(mainrenderer,meimg); //読み込んだ画像からテクスチャを作成
	SDL_RenderCopy(mainrenderer,metexture, &meimg_src, &meimg_dst); //テクスチャをレンダラーにコピー
	SDL_RenderPresent(mainrenderer); //描画データを表示
}

void Destroy() {
	SDL_DestroyRenderer(mainrenderer);
	SDL_DestroyWindow(mainwindow);
}
