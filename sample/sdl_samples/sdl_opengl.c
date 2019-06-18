/*************************************************
sdl_opengl.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lGL -L/usr/lib
備考：
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>        // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_opengl.h> // SDLでOpenGLを扱うために必要なヘッダファイルをインクルード

// 多角形を描画する関数
void drawPolygon(){
	glClearColor(0.0, 0.0, 0.0, 1.0);	// 塗り潰し色を黒色に設定
	glClear(GL_COLOR_BUFFER_BIT); // 塗りつぶし
	glBegin(GL_POLYGON); // 多角形の開始
		glColor3d(1.0, 0.0, 0.0); // 頂点を赤色に設定
		glVertex2d(0.0, 0.5);     // 頂点を作成

		glColor3d(0.0, 1.0, 0.0); // 頂点を緑色に設定
		glVertex2d(0.5, -0.5);    // 頂点を作成

		glColor3d(0.0, 0.0, 1.0); // 頂点を青色に設定
		glVertex2d(-0.5, -0.5);   // 頂点を作成
	glEnd(); // 図形終了
	SDL_GL_SwapBuffers(); // 描画を反映する
}

// 線を描画する関数
void drawLine(){
	glClearColor(0.5, 0.5, 0.5, 1.0);	// 塗り潰し色を灰色に設定
	glClear(GL_COLOR_BUFFER_BIT); // 塗りつぶし
	glColor3d(1.0, 0.0, 0.0); // すべての頂点（線の描画色）を赤色に設定
	glBegin(GL_LINE_LOOP);	// 連続した線の開始
		glVertex2d(-0.9, -0.9);
		glVertex2d(0.0, 0.0);
		glVertex2d(0.9, -0.9);
		glVertex2d(0.9, 0.9);
		glVertex2d(-0.9, 0.9);
	glEnd();
	SDL_GL_SwapBuffers(); // 描画を反映する
}

// 三角形を描画する関数
void drawTriangle(){
	glClearColor(1.0, 1.0, 1.0, 1.0);	// 塗り潰し色を白色に設定
	glClear(GL_COLOR_BUFFER_BIT); // 塗りつぶし
	glColor3d(0.0, 0.0, 0.0); // すべての頂点（線の描画色）を黒色に設定
	glBegin(GL_TRIANGLES);	// 三角形の開始
		glVertex2f(0.0 , 0.0);
		glVertex2f(-1.0 , 0.9);
		glVertex2f(1.0 , 0.9);
		glVertex2f(0.0 , 0);
		glVertex2f(-1.0 , -0.9);
		glVertex2f(1.0 , -0.9);
	glEnd();
	SDL_GL_SwapBuffers(); // 描画を反映する
}

// メイン関数
int main(int argc, char* argv[]) {
	SDL_Surface *window; // ウィンドウ（サーフェイス）データへのポインタ
	SDL_Event event; // SDLによるイベントを検知するための構造体

	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// OpenGLの設定
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);	// 赤色の確保するサイズ
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);	// 緑色の確保するサイズ
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);	// 青色の確保するサイズ
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);	// 深度の確保するサイズ
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// ダブルバッファを使用する

	// ウィンドウ生成（OpenGLを用いるウィンドウを生成）
 	if((window = SDL_SetVideoMode(320, 240, 32, SDL_OPENGL)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	while(1){
		if(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_KEYDOWN:	// キーボードのキーが押された時
				// 押されたキーごとに処理
				switch(event.key.keysym.sym){
				case SDLK_ESCAPE:	// Escキーが押された時
					printf("ESCAPE key -> EXIT.\n");
					SDL_Quit();
					exit(0);
					break;
				case SDLK_0:	// 0キーが押された時
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				case SDLK_1:	// 1キーが押された時
					drawLine();	// 線を描画する関数を呼び出す
					break;
				case SDLK_2:	// 2キーが押された時
					drawTriangle();	// 三角形を描画する関数を呼び出す
					break;
				case SDLK_UP:	// 上キーが押された時
					glTranslated(0.0, 0.1, 0.0);	// y+0.1平行移動
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				case SDLK_DOWN:	// 下キーが押された時
					glTranslated(0.0, -0.1, 0.0);	// y-0.1平行移動
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				case SDLK_RIGHT:	// 右キーが押された時
					glTranslated(0.1, 0.0, 0.0);	// x+0.1平行移動
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				case SDLK_LEFT:	// 左キーが押された時
					glTranslated(-0.1, 0.0, 0.0);	// x-0.1平行移動
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				case SDLK_F1:	// F1キーが押された時
					glScaled(1.5, 1.0, 1.0);	// x軸に対して拡大
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				case SDLK_F2:	// F2キーが押された時
					glScaled(0.5, 1.0, 1.0);	// x軸に対して縮小
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				case SDLK_F3:	// F3キーが押された時
					glScaled(1.0, 1.5, 1.0);	// y軸に対して拡大
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				case SDLK_F4:	// F4キーが押された時
					glScaled(1.0, 0.5, 1.0);	// y軸に対して縮小
					drawPolygon();	// 多角形を描画する関数を呼び出す
					break;
				}
				break;
			case SDL_MOUSEMOTION: // マウスが移動した時
				// マウスの相対移動量に応じてポリゴン図形を回転移動
				if(event.motion.xrel > 0){
					glRotatef(5.0, 1.0, 0.0, 0.0);	// x軸に対して+5度回転
					drawPolygon();	// 多角形を描画する関数を呼び出す
				}
				else if(event.motion.xrel < 0){
					glRotatef(-5.0, 1.0, 0.0, 0.0);	// x軸対して-5度回転
					drawPolygon();	// 多角形を描画する関数を呼び出す
				}

				if(event.motion.yrel > 0){
					glRotatef(5.0, 0.0, 1.0, 0.0);	// y軸対して+5度回転
					drawPolygon();	// 多角形を描画する関数を呼び出す
				}
				else if(event.motion.yrel < 0){
					glRotatef(-5.0, 0.0, 1.0, 0.0);	// y軸対して-5度回転
					drawPolygon();	// 多角形を描画する関数を呼び出す
				}
				break;
			case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押された時
				if(event.button.button==4){	// ホイルボタンが上にスライドされたなら
					glRotatef(5.0, 0.0, 0.0, 1.0);	// z軸対して+5度回転
					drawPolygon();	// 多角形を描画する関数を呼び出す
				}
				else if(event.button.button==5){	// ホイルボタンが下にスライドされたなら
					glRotatef(-5.0, 0.0, 0.0, 1.0);	// z軸対して-5度回転
					drawPolygon();	// 多角形を描画する関数を呼び出す
				}
				break;
			}
		}
	}
  
	return 0;
}
