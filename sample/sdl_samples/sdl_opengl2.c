/*************************************************
sdl_opengl2.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lGL -L/usr/lib
備考：
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>        // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_opengl.h> // SDLでOpenGLを扱うために必要なヘッダファイルをインクルード


float vertices[6][4][3] = {
	// 立方体の頂点データ
	// 正面
	{{-0.5f, 0.5f, -0.5f},
	{-0.5f, -0.5f, -0.5f},
	{0.5f, -0.5f, -0.5f},
	{0.5f, 0.5f, -0.5f}},

	// 右側面
	{{0.5f, 0.5f, -0.5f},
	{0.5f, -0.5f, -0.5f},
	{0.5f, -0.5f, 0.5f},
	{0.5f, 0.5f, 0.5f}},
	
	// 裏面
	{{0.5f, 0.5f, 0.5f},
	{0.5f, -0.5f, 0.5f},
	{-0.5f, -0.5f, 0.5f},
	{-0.5f, 0.5f, 0.5f}},
	
	// 左側面
	{{-0.5f, 0.5f, 0.5f},
	{-0.5f, -0.5f, 0.5f},
	{-0.5f, -0.5f, -0.5f},
	{-0.5f, 0.5f, -0.5f}},

	// 上面
	{{-0.5f, 0.5f, 0.5f},
	{-0.5f, 0.5f, -0.5f},
	{0.5f, 0.5f, -0.5f},
	{0.5f, 0.5f, 0.5f}},

	// 底面
	{{-0.5f, -0.5f, -0.5f},
	{-0.5f, -0.5f, 0.5f},
	{0.5f, -0.5f, 0.5f},
	{0.5f, -0.5f, -0.5f}}
};

// 立方体を描画する関数
void drawCube(){
	int i, j;

	glClearColor(1.0, 1.0, 1.0, 1.0);	// 塗り潰し色を白色に設定
	glClear(GL_COLOR_BUFFER_BIT); // 塗りつぶし

	glBegin(GL_QUADS);	// 四角形の開始
	for (i = 0; i < 6; i++) {
		glColor3d(i*0.2, 0.0, 0.0); // 各面の頂点を赤系の色に設定
		for (j = 0; j < 4; j++){
			glVertex3f(vertices[i][j][0], vertices[i][j][1], vertices[i][j][2]);
		}
	}
	glEnd(); // 図形終了
	SDL_GL_SwapBuffers(); // 描画を反映する
}

// メイン関数
int main(int argc, char* argv[]) {
	SDL_Surface *window; // サーフェイスへのポインタ
	SDL_Event event; // SDLによるイベントを検知するための構造体

	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	// ウィンドウ生成（OpenGLを用いるウィンドウを生成）
 	if((window = SDL_SetVideoMode(320, 240, 32, SDL_OPENGL)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

	// OpenGLの設定
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);	// 赤色の確保するサイズ
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);	// 緑色の確保するサイズ
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);	// 青色の確保するサイズ
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);	// 深度の確保するサイズ
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// ダブルバッファを使用する

	drawCube();

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
				}
				break;
			case SDL_MOUSEMOTION: // マウスが移動した時
				// マウスの相対移動量に応じてポリゴン図形を回転移動
				if(event.motion.xrel > 0){
					glRotatef(5.0, 1.0, 0.0, 0.0);	// x軸に対して+5度回転
					drawCube();	// 立方体を描画する関数を呼び出す
				}
				else if(event.motion.xrel < 0){
					glRotatef(-5.0, 1.0, 0.0, 0.0);	// x軸対して-5度回転
					drawCube();	// 立方体を描画する関数を呼び出す
				}

				if(event.motion.yrel > 0){
					glRotatef(5.0, 0.0, 1.0, 0.0);	// y軸対して+5度回転
					drawCube();	// 立方体を描画する関数を呼び出す
				}
				else if(event.motion.yrel < 0){
					glRotatef(-5.0, 0.0, 1.0, 0.0);	// y軸対して-5度回転
					drawCube();	// 立方体を描画する関数を呼び出す
				}
				break;
			case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押された時
				if(event.button.button==4){	// ホイルボタンが上にスライドされたなら
					glRotatef(5.0, 0.0, 0.0, 1.0);	// z軸対して+5度回転
					drawCube();	// 立方体を描画する関数を呼び出す
				}
				else if(event.button.button==5){	// ホイルボタンが下にスライドされたなら
					glRotatef(-5.0, 0.0, 0.0, 1.0);	// z軸対して-5度回転
					drawCube();	// 立方体を描画する関数を呼び出す
				}
				break;
			}
		}
	}
  
	return 0;
}
