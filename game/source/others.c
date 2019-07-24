#include"header.h"
/********ヘッダファイルからの読み込み******/
extern int games; //ゲーム状態
extern int score; //得点
extern bool titledraw; //一回だけ描画するよう
extern bool gamedraw; //一回だけ描画するよう
extern bool run; //一回だけ描画するよう
extern bool enemylive; //敵が生きてるかどうか
extern bool test_enemylive[enemynumber];
extern bool up;
extern bool down;


extern SDL_Window *mainwindow; //メイン画面用ウィンドウ
extern SDL_Surface *mainsurface; //メイン画面用サーフェイス
extern SDL_Renderer *mainrenderer; //メイン画面用レンダラー

extern SDL_Texture *backtexture; //タイトル画面用テクスチャー
extern SDL_Texture *gametexture; //ゲーム画面用テクスチャー
extern SDL_Texture *metexture; //キャラ画像用テクスチャー
extern SDL_Texture *bullettexture; //弾画像用テクスチャー
extern SDL_Texture *enemytexture; //敵画像用テクスチャー
extern SDL_Texture *cleartexture; //クリア画像用テクスチャー
extern SDL_Texture *test_enemytexture[enemynumber]; //敵画像用テクスチャー

extern SDL_Surface *backimg; //タイトル背景画像読み込み用サーフェイス
extern SDL_Surface *backimg2; //ゲーム画面背景画像読み込み用サーフェイス
extern SDL_Surface *meimg; //自キャラ画像読み込み用サーフェイス
extern SDL_Surface *bulletimg; //弾画像読み込み用サーフェイス
extern SDL_Surface *enemyimg; //敵キャラ画像読み込み用サーフェイス
extern SDL_Surface *clearimg; //クリア画面用画像読み込みサーフェイス
extern SDL_Surface *test_enemyimg[enemynumber]; //敵キャラ画像読み込み用サーフェイス

extern SDL_Rect backimg_src; //ソースの矩形(画像用)
extern SDL_Rect backimg_dst; //貼り付け先の矩形(画像用)
extern SDL_Rect backimg2_src; //ソースの矩形(画像用)
extern SDL_Rect backimg2_dst; //貼り付け先の矩形(画像用)

extern SDL_Rect meimg_src; //ソースの矩形(画像用)
extern SDL_Rect meimg_dst; //貼り付け先の矩形(画像用)
extern SDL_Rect bulletimg_src; //弾の矩形
extern SDL_Rect bulletimg_dst; //弾の矩形(画像用)
extern SDL_Rect enemyimg_src; //敵の矩形
extern SDL_Rect enemyimg_dst; //敵の矩形(画像用)
extern SDL_Rect clearimg_src; //クリア画面の矩形(画像用)
extern SDL_Rect clearimg_dst; //クリア画面の矩形(画像用)
extern SDL_Rect test_enemyimg_src; //敵の矩形
extern SDL_Rect test_enemyimg_dst; //敵の矩形(画像用)
extern SDL_Event inputevent;
extern SDL_Joystick *joystick; //ジョイスティックを特定,利用するための構造体

/*********初期化など*********/
timers timer; //時間計測用
inputkeys keys; //入力キー用
mychara me; //自キャラ用
enemychara enemy; //敵キャラ用
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
	//得点を初期化
	score = 0;

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
	//timer.leave = SDL_GetTicks();
	//一回だけ画面を表示するよう
	titledraw = true;
	gamedraw = true;
	//続けるかどうか
	run = true;
	//敵が生きているかどうか
	enemylive = true;

	//矩形初期化



	//コピー元画像の領域を定義
	backimg_src.x = 0;
	backimg_src.y = 0;
	backimg_src.w = backimg->w;
	backimg_src.h = backimg->h; 

	backimg_dst.x = 0;
	backimg_dst.y = 0;
	backimg_dst.w = windowwidth;
	backimg_dst.h = windowheight;

	backtexture = SDL_CreateTextureFromSurface(mainrenderer, backimg); //読み込んだ画像からテクスチャを作成

	//コピー元画像の領域を定義
	backimg2_src.x = 0;
	backimg2_src.y = 0;
	backimg2_src.w = backimg2->w;
	backimg2_src.h = backimg2->h; 

	backimg2_dst.x = 0;
	backimg2_dst.y = 0;
	backimg2_dst.w = windowwidth;
	backimg2_dst.h = windowheight;

	gametexture = SDL_CreateTextureFromSurface(mainrenderer, backimg2); //読み込んだ画像からテクスチャを作成

	//コピー元画像の領域を定義
	meimg_src.x = 0;
	meimg_src.y = 0;
	meimg_src.w = meimg->w;
	meimg_src.h = meimg->h; 

	//コピー先画像の領域を定義
	meimg_dst.x = windowwidth / 2 - 64;
	meimg_dst.y = 860;
	meimg_dst.w = 64;
	meimg_dst.h = 64;

	metexture = SDL_CreateTextureFromSurface(mainrenderer,meimg); //読み込んだ画像からテクスチャを作成

	bulletimg_src.x = 0;
	bulletimg_src.y = 0;
	bulletimg_src.w = 16;
	bulletimg_src.h = 16;

	bulletimg_dst.w = 32;
	bulletimg_dst.h = 32;

	bullettexture = SDL_CreateTextureFromSurface(mainrenderer, bulletimg); //読み込んだ画像からテクスチャを作成

	enemyimg_src.x = 0;
	enemyimg_src.y = 0;
	enemyimg_src.w = enemyimg->w;
	enemyimg_src.h = enemyimg->h;

	enemyimg_dst.x = windowwidth / 2 - 128;
	enemyimg_dst.y = 0;
	enemyimg_dst.w = 128;
	enemyimg_dst.h = 128;

	enemytexture = SDL_CreateTextureFromSurface(mainrenderer, enemyimg); //読み込んだ画像からテクスチャを作成

	clearimg_src.x = 0;
	clearimg_src.y = 0;
	clearimg_src.w = clearimg->w;
	clearimg_src.h = clearimg->h; 

	clearimg_dst.x = 0;
	clearimg_dst.y = 0;
	clearimg_dst.w = windowwidth;
	clearimg_dst.h = windowheight;

	cleartexture = SDL_CreateTextureFromSurface(mainrenderer, clearimg); //読み込んだ画像からテクスチャを作成
	rightmove = true;

	up = true;
	down = false;

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
	backimg= IMG_Load("./images/background.png"); //背景画像(サーフェイスへの)読み込み
	if(!backimg) SDL_Quit(); //もし読み込めなかったら終了する
	meimg= IMG_Load("./images/me.png"); //自キャラ画像(サーフェイスへの)読み込み
	if(!meimg) SDL_Quit(); //もし読み込めなかったら終了する
	backimg2= IMG_Load("./images/gamebackground.png"); //ゲーム画面背景画像(サーフェイスへの)読み込み
	if(!backimg2) SDL_Quit(); //もし読み込めなかったら終了する
	bulletimg = IMG_Load("./images/bullet.png"); //弾画像読み込み
	if(!bulletimg) SDL_Quit(); //もし読み込めなかったら終了する
	enemyimg = IMG_Load("./images/enemy.png"); //弾画像読み込み
	if(!enemyimg) SDL_Quit(); //もし読み込めなかったら終了する
	clearimg = IMG_Load("./images/clear.png"); //弾画像読み込み
	if(!clearimg) SDL_Quit(); //もし読み込めなかったら終了する
}

//タイトル描画用
void drawtitle(Sint16 posX,Sint16 posY) {
	SDL_RenderCopy(mainrenderer,backtexture, &backimg_src, &backimg_dst); //テクスチャをレンダラーにコピー

	if( keys.up == 1) {
		up = true;
		down = false;
	}
	if( keys.down == 1) {
		up = false;
		down = true;
	}
	if(up) {
		rectangleColor(mainrenderer, 450, 300, 750, 500, 0xffffffff);
		if(keys.start == 1) {
			reset();
			games = 1;
		}
	}
	if(down) {
		rectangleColor(mainrenderer, 450, 550, 750, 750, 0xffffffff);
		if(keys.start == 1) {
			run = false;
		}
	}
	SDL_RenderPresent(mainrenderer); //描画データを表示
	//titledraw = false; //一回だけ表示したいのでfalseに
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
				keys.right = 1;
				keys.left = 0;
			// keys.up = 0;
			//	keys.down = 0;
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
			if(inputevent.jbutton.button== 5){ //発射ボタンが押された
			//	printf("発射!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				if( games == 0){} //クリア画面と初期画面の入力はきかないように
				else if (games == 1) keys.z = 1; //発射キーが押された
				else if (games == 2) {}
			}

			if(inputevent.jbutton.button == 3){ //終了ボタンが押された
				keys.start = 1;
			}
			if(inputevent.jbutton.button== 11){ //終了ボタンが押された
				run = false;
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
	}
}

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
}
*/

//ゲームスタート用
void startGame() {
	//games = 1; //Zキーが押されたらゲームへ
	reset(); //色々リセット
}

//ゲーム背景描画用
void drawgame(Sint16 posX,Sint16 posY) {
	//メインウィンドウに対するレンダラーを生成
	SDL_RenderCopy(mainrenderer,gametexture, &backimg2_src, &backimg2_dst); //テクスチャをレンダラーにコピー
	SDL_RenderCopy(mainrenderer,metexture, &meimg_src, &meimg_dst); //テクスチャをレンダラーにコピー
	SDL_RenderPresent(mainrenderer); //描画データを表示

	gamedraw = false; //一回だけ表示したいのでfalseに
}

//色々リセット用
void reset() {
	me.posX = windowwidth /2 - 64;
	me.posY = 800;
	enemy.posX = 0;
	enemy.posY = 0;
	int loop;
	for (loop = 0;loop < bulletmax ;loop++){
		me.flagB[loop] = 0; //フラグを下げる
	}
	me.bulletX[0] = 0;
	me.bulletY[0] = windowheight;
	score = 0;
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
	//if(keys.up == 1) me.posY --; //上キーが押されたら上に移動
	//if(keys.down == 1) me.posY ++; //下キーがおされたら下に移動
	
	/*********画面外にでないようにする処理****************/
	if(me.posX > windowwidth - 64) me.posX = windowwidth - 64; 
	else if(me.posX < 0 ) me.posX = 0;

	if(me.posY > windowheight - 64) me.posY = windowheight - 64; 
	else if(me.posY < 0) me.posY = 0;
	//printf("posX = %d\n",me.posX);
	//printf("posY = %d\n",me.posY);

}

//自キャラ描画	
void me_draw() {

	//背景
	//	SDL_RenderClear(mainrenderer); //レンダラーをクリア
	SDL_RenderCopy(mainrenderer,gametexture, &backimg2_src, &backimg2_dst); //テクスチャをレンダラーにコピー

	//キャラ

	//コピー先画像の領域を定義
	meimg_dst.x = me.posX;
	meimg_dst.y = me.posY;
	SDL_RenderCopy(mainrenderer,metexture, &meimg_src, &meimg_dst); //テクスチャをレンダラーにコピー

	if(me.flagB[0] == 1) {
		bulletimg_dst.x = me.bulletX[0];
		bulletimg_dst.y = me.bulletY[0];
		SDL_RenderCopy(mainrenderer,bullettexture, &bulletimg_src, &bulletimg_dst); //テクスチャをレンダラーにコピー
	}
	
	if(enemylive){
		enemyimg_dst.x = enemy.posX;
		enemyimg_dst.y = enemy.posY;
		SDL_RenderCopy(mainrenderer,enemytexture, &enemyimg_src, &enemyimg_dst); //テクスチャをレンダラーにコピー
	}
	SDL_RenderPresent(mainrenderer); //描画データを表示
	
}

void enemy_draw() {
	if(enemylive){
		enemyimg_dst.x = enemy.posX;
		enemyimg_dst.y = enemy.posY;
		SDL_RenderCopy(mainrenderer,enemytexture, &enemyimg_src, &enemyimg_dst); //テクスチャをレンダラーにコピー
	}
	SDL_RenderPresent(mainrenderer);
}

//自キャラ弾計算
void me_bullet() {
	if(keys.z == 1) {
		if (me.flagB[0] ==1) {}
		else {
			me.flagB[0] = 1;
			me.bulletX[0] = me.posX + 12;
			me.bulletY[0] = me.posY - 8;
			keys.z = 0;
		}
	}
	if(me.flagB[0] == 1) {
		me.bulletY[0] -= 4;
		if(me.bulletY[0] < 0){
			me.bulletY[0] = windowheight;
			me.flagB[0] = 0;
			/*
			if(me.bulletX[0] < windowidth/2 -128){
				enemylive = false;
			}
			*/
		}
	}
	/*
		for (int loop = 0; loop < bulletmax; loop++) {
		if(me.flagB[loop] == 0){
		me.flagB[loop] = 1;
		me.bulletX[loop] = me.posX;
		me.bulletY[loop] = me.posY;
		break;
		}
		}
		for (int loop = 0; loop < bulletmax; loop++) {
		if(me.flagB[loop] == 1) {
		me.bulletY[loop] -= 5;
	*/
}

void enemy_move() {
	if(rightmove) enemy.posX++;
	else enemy.posX--;
	/*********画面外にでないようにする処理****************/
	if(enemy.posX > windowwidth - 64) {
		enemy.posX = windowwidth - 64;
		rightmove = false;
		
	}
	else if(enemy.posX < 0 ) {
		enemy.posX = 0;
		rightmove = true;
	}

	if(enemy.posY > windowheight - 128) enemy.posY = windowheight - 128; 
	else if(enemy.posY < 0) enemy.posY = 0;
}

void judge() {
	if( abs(me.bulletX[0] - enemy.posX) < 128 && abs(me.bulletY[0] - enemy.posY) < 128 ) {
		//enemylive = false;
		//games = 2;
		score++;
		me.bulletX[0] = windowwidth;
		me.bulletY[0] = 0;
		keys.z = 0;
	}
}

void drawclear() {
	printf("スコア: %d\n",score);
	SDL_RenderCopy(mainrenderer,cleartexture, &clearimg_src, &clearimg_dst); //テクスチャをレンダラーにコピー
	SDL_RenderPresent(mainrenderer); //描画データを表示
	SDL_Delay(2000);
	
	titledraw = true;
	gamedraw = true;
	//敵が生きているかどうか
	enemylive = true;
	reset();
	keys.z = 0;
	keys.start = 0;
	games = 0;

}

void Destroy() {
	SDL_DestroyRenderer(mainrenderer);
	SDL_DestroyWindow(mainwindow);
}

