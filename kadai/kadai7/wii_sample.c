/*************************************************
sdl_wii.c
Programmed by S. Nishide (2019,5)

コンパイルオプション：-lSDL2 -lSDL2_gfx -lcwiimote -D_ENABLE_TILT -D_ENABLE_FORCE -L/usr/lib
備考：実行する時は./実行ファイル名 WiiリモコンのMACアドレス

*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>	// SDLを用いるために必要なヘッダファイル
#include <SDL2/SDL2_gfxPrimitives.h>	// 描画関係のヘッダファイル

#include "wiimote.h"	// Wiiリモコンを用いるために必要なヘッダファイル
#include "wiimote_api.h"	// Wiiリモコンを用いるために必要なヘッダファイル

//#define WII_DEBUG

// メイン関数
int main(int argc, char* argv[]) {

  SDL_Window* window; // ウィンドウデータを格納する構造体
  SDL_Renderer* renderer; // 2Dレンダリングコンテキスト(描画設定)を格納する構造体

  SDL_Rect a = { 320, 100, 5, 5 }; // 点（四角）を描画するための矩形領域指定
  SDL_Rect b = { 320, 200, 5, 5 }; // 点（四角）を描画するための矩形領域指定
  SDL_Rect c = { 320, 300, 5, 5 }; // 点（四角）を描画するための矩形領域指定
  
  /**************************************/
  /* Wiiリモコン関連の初期化（ここから） */
  /**************************************/

  // Wiiリモコンを用いるための構造体を宣言（初期化）
  wiimote_t wiimote = WIIMOTE_INIT;	// Wiiリモコンの状態格納用
  wiimote_report_t report = WIIMOTE_REPORT_INIT;	// レポートタイプ用
  
  // Wiiリモコンのスピーカで鳴らす音のデータ
  uint8_t sample[20] = {
    0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,
    0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c,0x3c
  };

  // ***** Wiiリモコン処理 *****
  if (argc < 2) {	// Wiiリモコン識別情報がコマンド引数で与えられなければ
    printf("Designate the wiimote ID to the application.\n");
    exit(1);
  }
  
  // Wiiリモコンの接続（１つのみ）
  if (wiimote_connect(&wiimote, argv[1]) < 0) {	// コマンド引数に指定したWiiリモコン識別情報を渡して接続
    printf("unable to open wiimote: %s\n", wiimote_get_error());
    exit(1);
  }
  wiimote.led.one  = 1;	// WiiリモコンのLEDの一番左を点灯させる（接続を知らせるために）
  // wiimote.led.four  = 1;	// WiiリモコンのLEDの一番右を点灯させる場合
  
  // Wiiリモコンスピーカの初期化
  wiimote_speaker_init(&wiimote, WIIMOTE_FMT_S4, WIIMOTE_FREQ_44800HZ);

  // センサからのデータを受け付けるモードに変更
  wiimote.mode.acc = 1;

  /**************************************/
  /* Wiiリモコン関連の初期化（ここまで） */
  /**************************************/

  
  /**************************************/
  /* SDL関連の初期化（ここから）         */
  /**************************************/
  
  // SDL初期化
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("failed to initialize SDL.\n");
    exit(-1);
  }

  // ウィンドウ生成・表示
  window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
  if(window == NULL){
    printf("Failed to create window.\n");
    exit(-1);
  }

  // 描画処理
  renderer = SDL_CreateRenderer(window, -1, 0); // 生成したウィンドウに対してレンダリングコンテキスト（RC）を生成
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // 生成したRCに描画色として白を設定
  SDL_RenderClear(renderer); // 生成したRCを白でクリア＝塗りつぶす（ただし，メモリに描画データを反映させただけなので，画面には表示されない）

  // 初期画面
  stringColor(renderer, 0, 0, "Press buttons 1 and 2 on the wiimote now to connect.\n", 0xffffffff);  
  SDL_RenderPresent(renderer); // 描画データを表示
  
  /**************************************/
  /* SDL関連の初期化（ここまで）         */
  /**************************************/



  /**************************************/
  /* 処理開始                            */
  /**************************************/

  // Wiiリモコンがオープン（接続状態）であればループ
  while (wiimote_is_open(&wiimote)) {
    
    // Wiiリモコンの状態を取得・更新する
    if (wiimote_update(&wiimote) < 0) {
      wiimote_disconnect(&wiimote);
      break;
    }

    // ***** Wiiのキー（ボタン）ごとに処理 *****
    // HOMEボタンが押された時(終了処理)
    if (wiimote.keys.home) {
      wiimote_speaker_free(&wiimote);	// Wiiリモコンのスピーカを解放
      wiimote_disconnect(&wiimote);	// Wiiリモコンとの接続を解除
    }

    
    // Aボタンが押されたら
    if (wiimote.keys.a) {
      // if (wiimote.keys.bits & WIIMOTE_KEY_A) {	// このような押下ボタン取得もある
      wiimote.mode.acc = 1;	// 加速度センサをON（センサを受け付ける）
    }

    // Bボタンが押された時
    if (wiimote.keys.b) {
      wiimote.mode.ir = 1;	// 赤外線センサをON
    }
    // Bボタンが離された時
    else {
      wiimote.mode.ir = 0;	// 赤外線センサをOFF
    }

    // 1ボタンが押された時		
    if (wiimote.keys.one) {
      wiimote.rumble = 1;	// Wiiリモコンを振動させる
    }
    // 1ボタンが離された時		
    else {
      wiimote.rumble = 0;	// 振動を止める
    }

    // 2ボタンが押された時		
    if (wiimote.keys.two) {
      wiimote_speaker_freq(&wiimote, wiimote.tilt.x + 90);	// Wiiリモコンの傾きで音の周波数を変更（tiltの値を使うので、加速度センサをONにしておく→Aボタンを押す）
      wiimote_speaker_volume(&wiimote, 100);	// ボリュームを指定
      wiimote_speaker_play(&wiimote, sample, 20);	//　用意した音のデータを再生
    }

    // +ボタンが押された時
    if (wiimote.keys.plus) {
      wiimote.led.bits += 1;	// 光らせるLEDを増やす（2進数的に）
    }
    // -ボタンが押されたとき
    if (wiimote.keys.minus) {
      wiimote.led.bits -= 1;	// 光らせるLEDを減らす（2進数的に）
    }
		
    // 十字ボタンが押された時
    if(wiimote.keys.left){	// 左
      a.x -= 5;	// 点を移動させる
    }
    if (wiimote.keys.right){	// 左
      a.x += 5;
    }
    if(wiimote.keys.up){	// 上
      a.y -= 5;
    }
    if(wiimote.keys.down){	// 下
      a.y += 5;
    }
    if (wiimote.keys.bits & WIIMOTE_KEY_UPLEFT) {	// 左上
      a.x -= 5;
      a.y -= 5;
    }
    if (wiimote.keys.bits & WIIMOTE_KEY_UPRIGHT) {	// 右上
      a.x += 5;
      a.y -= 5;
    }
    if (wiimote.keys.bits & WIIMOTE_KEY_DOWNLEFT) {	// 左下
      a.x -= 5;
      a.y += 5;
    }
    if (wiimote.keys.bits & WIIMOTE_KEY_DOWNRIGHT) {	// 右下
      a.x += 5;
      a.y += 5;
    }


#ifdef WII_DEBUG
    // ***** Wiiリモコンの状態を表示 *****
    printf("MODE %02x\n", wiimote.mode.bits);	// モード
    printf("BAT %02x\n", wiimote.battery);	// バッテリ残量

    // ボタンの状態
    printf("KEYS %04x one=%d two=%d a=%d b=%d <=%d >=%d ^=%d v=%d h=%d +=%d -=%d\n",
	   wiimote.keys.bits, 
	   wiimote.keys.one,
	   wiimote.keys.two,
	   wiimote.keys.a,
	   wiimote.keys.b,
	   wiimote.keys.left,
	   wiimote.keys.right,
	   wiimote.keys.up,
	   wiimote.keys.down,
	   wiimote.keys.home,
	   wiimote.keys.plus,
	   wiimote.keys.minus);

    /*
    // Wiiリモコン拡張機能の状態			
    printf("JOY1 joyx=%03d joyy=%03d x=%03d y=%03d z=%03d keys.z=%d keys.c=%d\n", 
    wiimote.ext.nunchuk.joyx,
    wiimote.ext.nunchuk.joyy,
    wiimote.ext.nunchuk.axis.x,
    wiimote.ext.nunchuk.axis.y,
    wiimote.ext.nunchuk.axis.z,
    wiimote.ext.nunchuk.keys.z,
    wiimote.ext.nunchuk.keys.c);
    printf("JOY2 joyx=%03d joyy=%03d\n", 
    wiimote.ext.classic.joyx1,
    wiimote.ext.classic.joyy1);
    printf("JOY3 joyx=%03d joyy=%03d\n", 
    wiimote.ext.classic.joyx2,
    wiimote.ext.classic.joyy2);
    */

    // 加速度の状態
    printf("AXIS x=%03d [%03d] y=%03d [%03d] z=%03d [%03d]\n", 
	   wiimote.axis.x,
	   wiimote.axis.x - 127,
	   wiimote.axis.y,
	   wiimote.axis.y - 127,
	   wiimote.axis.z,
	   wiimote.axis.z - 127);	// 分かりやすくするために-127をして±の値で表示

    // 赤外線センサの状態（ポインティングの座標等）
    printf("IR1 x=%04d y=%04d ss=%d\n",
	   wiimote.ir1.x,
	   wiimote.ir1.y,
	   wiimote.ir1.size);
    printf("IR2 x=%04d y=%04d ss=%d\n",
	   wiimote.ir2.x,
	   wiimote.ir2.y,
	   wiimote.ir2.size);
    /*
      printf("IR3 x=%04d y=%04d ss=%d\n",
      wiimote.ir3.x,
      wiimote.ir3.y,
      wiimote.ir3.size);
      printf("IR4 x=%04d y=%04d ss=%d\n",
      wiimote.ir4.x,
      wiimote.ir4.y,
      wiimote.ir4.size);
    */

    // コンパイルオプションに、-D_ENABLE_TILTを指定することで処理（コンパイル）される
#ifdef _ENABLE_TILT
    // Wiiリモコンの傾きの状態
    printf("TILT x=%.3f y=%.3f z=%.3f\n", 
	   wiimote.tilt.x,
	   wiimote.tilt.y,
	   wiimote.tilt.z);
#endif
    // コンパイルオプションに、-D_ENABLE_FORCEを指定することで処理（コンパイル）される
#ifdef _ENABLE_FORCE		
    // Wiiリモコンの重力加速度の状態
    printf("FORCE x=%.3f y=%.3f z=%.3f (sum=%.3f)\n", 
	   wiimote.force.x,
	   wiimote.force.y,
	   wiimote.force.z,
	   sqrt(wiimote.force.x*wiimote.force.x+wiimote.force.y*wiimote.force.y+wiimote.force.z*wiimote.force.z));
#endif

    printf("**********\n");
#endif 

    // *****図形描画処理*****

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);                  // ウィンドウを白色にする
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
    SDL_RenderFillRect(renderer, &a);           // 長方形aを描画

    // Wiiリモコンの傾きで点を移動させる
    b.x = wiimote.tilt.x + 100;
    b.y = wiimote.tilt.y + 100;
    b.w = wiimote.tilt.z;
    b.h = wiimote.tilt.z;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &b);           // 長方形bを描画

    // Wiiリモコンの（赤外線）ポインティングで点を移動させる（２つ赤外線を見つけた時の値）
    // x,y軸とも0-255の範囲でポインティング座標を取得するようにしている
    if(wiimote.ir2.x < 0){
      wiimote.ir2.x = 0;
      wiimote.rumble = 1;	// 枠外→振動ON
    }
    else if(wiimote.ir2.x > 255){
      wiimote.ir2.x = 255;
      wiimote.rumble = 1;	// 振動ON
    }
    else{
      wiimote.rumble = 0;	// 枠内→振動OFF
    }
    if(wiimote.ir2.y < 0){
      wiimote.ir2.y = 0;
      wiimote.rumble = 1;	// 振動ON
    }
    else if(wiimote.ir2.y > 255){
      wiimote.ir2.x = 255;
      wiimote.rumble = 1;	// 振動ON
    }
    else{
      wiimote.rumble = 0;	// 振動OFF
    }
    int w, h;
    SDL_GetWindowSize(window, &w, &h); // ウィンドウサイズを取得
    c.x = abs(wiimote.ir2.x * 2 - w);	// 描画座標を調整しウィンドウサイズを基に左右を反転させる
    c.y = wiimote.ir2.y * 2;	// 描画座標を調整
    c.w = 5;
    c.h = 5;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
    SDL_RenderFillRect(renderer, &c);           // 長方形cを描画

    SDL_RenderPresent(renderer);	// 描画データを表示
  }

  // 終了処理
  SDL_DestroyRenderer(renderer); // RCの破棄（解放）
  SDL_DestroyWindow(window); // 生成したウィンドウの破棄（消去）
  SDL_Quit();
  return 0;
}

