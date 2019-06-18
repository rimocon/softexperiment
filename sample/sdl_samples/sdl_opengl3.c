/*************************************************
sdl_opengl3.c
Programmed by H. Mitsuhara (2011,7-8)

コンパイルオプション：-lSDL -lGL -lSDL_image -L/usr/lib
備考：
　2のn乗×2のn乗ピクセルのtexture.pngまたはtexture.bmpを用意してください．
*************************************************/

#include <stdio.h>
#include <SDL/SDL.h>        // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_opengl.h> // SDLでOpenGLを扱うために必要なヘッダファイルをインクルード
#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード

GLuint texture = 0;	// テクスチャの管理番号を格納する変数（int型に相当）

// 平面を描画する関数
void drawPlate(){
	glClearColor(1.0, 1.0, 1.0, 1.0);	// 塗り潰し色を白色に設定
	glClear(GL_COLOR_BUFFER_BIT); // 塗りつぶし

	glColor3d(1.0, 1.0, 1.0); // 各面の頂点に色を設定

	glBegin(GL_QUADS);	// 四角形の開始
		// 以下の四角形に（管理番号が指す）テクスチャを描画する		
		glTexCoord2f(0.0f, 0.0f);	// 画像の左上と次の頂点を対応させる
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);	// 画像の左下と次の頂点を対応させる
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);	// 画像の右下と次の頂点を対応させる
		glVertex3f(0.5f, -0.5f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);	// 画像の右上と次の頂点を対応させる
		glVertex3f(0.5f, 0.5f, 0.0f);
	glEnd(); // 図形終了
	SDL_GL_SwapBuffers(); // 描画を反映する
}

// メイン関数
int main(int argc, char* argv[]) {
	SDL_Surface *window, *texture_surface, *texture_surface2; // サーフェイスへのポインタ
	SDL_Event event; // SDLによるイベントを検知するための構造体
	GLenum texture_format;	// テクスチャのフォーマット（番号）を格納する変数（unsigned int型に相当）

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

	// テクスチャ画像を読み込む
	texture_surface = IMG_Load("texture.png");		// テクスチャ画像の読み込み（2のn乗×2のn乗ピクセルの画像を指定）
//	texture_surface = SDL_LoadBMP("texture.bmp");		// テクスチャ画像の読み込み（2のn乗×2のn乗ピクセルの画像を指定）

	// 画像の読み込みに失敗した場合
	if (!texture_surface){
		printf("failed to load the texture file.\n");
		exit(-1);
	}

	// 画像（を読み込んだサーフェイス）の各ピクセルを表わすバイト数（色深度）に応じてテクスチャフォーマットを変更
	switch(texture_surface->format->BytesPerPixel){
	case 3:	// 24bit per Pixel
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN){	// ビッグエンディアン（連続したデータを桁の大きい方からByteごとに格納）なら
			texture_format = GL_BGR;
		}
		else{
			texture_format = GL_RGB;
		}
		break;
	case 4:	// 32bit per Pixel
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
			texture_format = GL_BGRA;
		}
		else{
			texture_format = GL_RGBA;
		}
		break;
	}
	
	// RGBAサーフェイスを生成、テクスチャを変換（テクスチャを扱うために必要です）
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
	    	texture_surface2 = SDL_CreateRGBSurface(SDL_SWSURFACE, texture_surface->w, texture_surface->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);	// 空のサーフェイスを生成
	}
	else{
	    	texture_surface2 = SDL_CreateRGBSurface(SDL_SWSURFACE, texture_surface->w, texture_surface->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	}
	// サーフェイス生成に失敗した場合
	if (!texture_surface2) {
		printf("failed to convert to RGBA format: %s\n", SDL_GetError());
		exit(-1);
	}		
	else{
		SDL_BlitSurface(texture_surface, NULL, texture_surface2, NULL);	// テクスチャを読み込んだサーフェイスを新たに作成したRGBAサーフェイスに貼り付け
	}

	// テクスチャを作る
	glEnable(GL_TEXTURE_2D);	// テクスチャを有効にする
	glGenTextures(1, &texture);	// ある１つのテクスチャ名（整数値）を生成
	glBindTexture(GL_TEXTURE_2D, texture);	// 指定したテクスチャの実体（2次元テクスチャ）を作成
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// テキスチャのパラメータを指定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, texture_surface2->format->BytesPerPixel, texture_surface2->w, texture_surface2->h, 0, texture_format, GL_UNSIGNED_BYTE, texture_surface2->pixels);	// 画像データとテクスチャを関連付ける

	SDL_FreeSurface(texture_surface);	// サーフェイスの解放
	SDL_FreeSurface(texture_surface2);	// サーフェイスの解放

	drawPlate();	// 平面描画

	while(1){
		if(SDL_PollEvent(&event)){
			switch (event.type) {
			case SDL_KEYDOWN:	// キーボードのキーが押された時
				// 押されたキーごとに処理
				switch(event.key.keysym.sym){
				case SDLK_ESCAPE:	// Escキーが押された時
					glDeleteTextures(1, &texture);	// テクスチャを削除する
					SDL_Quit();
					exit(0);
					break;
				}
				break;
			case SDL_MOUSEMOTION: // マウスが移動した時
				// マウスの相対移動量に応じてポリゴン図形を回転移動
				if(event.motion.xrel > 0){
					glRotatef(5.0, 1.0, 0.0, 0.0);	// x軸に対して+5度回転
					drawPlate();	// 立方体を描画する関数を呼び出す
				}
				else if(event.motion.xrel < 0){
					glRotatef(-5.0, 1.0, 0.0, 0.0);	// x軸対して-5度回転
					drawPlate();	// 立方体を描画する関数を呼び出す
				}

				if(event.motion.yrel > 0){
					glRotatef(5.0, 0.0, 1.0, 0.0);	// y軸対して+5度回転
					drawPlate();	// 立方体を描画する関数を呼び出す
				}
				else if(event.motion.yrel < 0){
					glRotatef(-5.0, 0.0, 1.0, 0.0);	// y軸対して-5度回転
					drawPlate();	// 立方体を描画する関数を呼び出す
				}
				break;
			case SDL_MOUSEBUTTONDOWN:	// マウスのボタンが押された時
				if(event.button.button==4){	// ホイルボタンが上にスライドされたなら
					glRotatef(5.0, 0.0, 0.0, 1.0);	// z軸対して+5度回転
					drawPlate();	// 立方体を描画する関数を呼び出す
				}
				else if(event.button.button==5){	// ホイルボタンが下にスライドされたなら
					glRotatef(-5.0, 0.0, 0.0, 1.0);	// z軸対して-5度回転
					drawPlate();	// 立方体を描画する関数を呼び出す
				}
				break;
			}
		}
	}
  
	return 0;
}
