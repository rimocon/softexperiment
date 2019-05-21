#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main( int argc,char *argv[]){

	SDL_Init(SDL_INIT_VIDEO); //SDLの初期化
	IMG_Init(IMG_INIT_PNG); //PNG形式で初期化
	TTF_Init(); //SDL2_ttfを初期化
	/*変数宣言*/
	SDL_Event event; //event用の変数を宣言
	SDL_Surface *button1_string,*button2_string,*button3_string; //ボタン用の変数を宣言
	TTF_Font *font = TTF_OpenFont("fonts-japanese-gothic.ttf",24); //フォントサイズ24でフォントを読み込む
	SDL_Color white = {0xFF,0xFF,0xFF};
	SDL_Color black = {0x00,0x00,0x00};

	SDL_Window *window = SDL_CreateWindow("kadai2",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,430,400,0); //ウィンドウの生成
	if (window == NULL){
		printf("failed to create window.n");
		exit(1);
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0); //レンダラーの生成
	SDL_SetRenderDrawColor(renderer,255,255,255,0); //ウィンドウを黒に設定
	SDL_RenderClear(renderer); //設定色でクリア
	//画像の読み込み
	SDL_Surface *image1 = IMG_Load("button1.png");
	SDL_Surface *image2 = IMG_Load("button2.png");
	
	if(!(image1 && image2)){
		printf("画像を読み込めませんでした: %s\n",IMG_GetError());
	}
	//テクスチャの生成
	
	SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer,image1);
	SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer,image1);
	SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer,image1);
	SDL_Texture* texture4 = SDL_CreateTextureFromSurface(renderer,image2);
	//テクスチャの情報
	SDL_QueryTexture(texture1,NULL,NULL,&image1->w,&image1->h);
	SDL_QueryTexture(texture4,NULL,NULL,&image2->w,&image2->h);
	//コピー元画像のサイズ設定
	SDL_Rect src_rect1 = {0,0,image1->w,image1->h};
	SDL_Rect src_rect2 = {0,0,image2->w,image2->h};
	//コピー先画像のサイズ設定
	SDL_Rect dst_rect1 = {10,10,100,50};
	SDL_Rect dst_rect2 = {160,10,100,50};
	SDL_Rect dst_rect3 = {320,10,100,50};
	SDL_RenderCopy(renderer,texture1,&src_rect1,&dst_rect1);
	SDL_RenderCopy(renderer,texture2,&src_rect1,&dst_rect2);
	SDL_RenderCopy(renderer,texture3,&src_rect1,&dst_rect3);
	//好きな画像の読み込み

	SDL_Surface *picture = IMG_Load("like.png");
	SDL_Texture *likepicture =SDL_CreateTextureFromSurface(renderer,picture);
	SDL_Rect src_like ={0,0,picture->w,picture->h};
	SDL_Rect dst_like = {10,100,410,300};

	rectangleColor(renderer,10,10,110,60,0xff000000); //赤色で囲う

	int position = 0; //(0:左,1:真ん中,2:右)
	int color;

	button1_string = TTF_RenderUTF8_Blended(font,"画像",black);
	SDL_Texture* button1_texture = SDL_CreateTextureFromSurface(renderer,button1_string);
	SDL_Rect src_button1 = {0,0,button1_string->w,button1_string->h};
	SDL_Rect dst_button1 = {10,10,100,50};
	SDL_RenderCopy(renderer,button1_texture,&src_button1,&dst_button1);

	button2_string = TTF_RenderUTF8_Blended(font,"処理",black);
	SDL_Texture* button2_texture = SDL_CreateTextureFromSurface(renderer,button2_string);
	SDL_Rect src_button2 = {0,0,button2_string->w,button2_string->h};
	SDL_Rect dst_button2 = {160,10,100,50};
	SDL_RenderCopy(renderer,button2_texture,&src_button2,&dst_button2);

	button3_string = TTF_RenderUTF8_Blended(font,"終了",black);
	SDL_Texture* button3_texture = SDL_CreateTextureFromSurface(renderer,button3_string);
	SDL_Rect src_button3 = {0,0,button3_string->w,button3_string->h};
	SDL_Rect dst_button3 = {320,10,100,50};
	SDL_RenderCopy(renderer,button3_texture,&src_button3,&dst_button3);

	SDL_RenderPresent(renderer);

	//イベント処理
	while(1){
		if(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_RIGHT ){ //右矢印を押された時
						if(position == 0){
							position = 1;
						}
						else if(position == 1){
							position = 2;
						}
						SDL_RenderCopy(renderer,texture1,NULL,&dst_rect1);
						SDL_RenderCopy(renderer,texture2,NULL,&dst_rect2);
						SDL_RenderCopy(renderer,texture3,NULL,&dst_rect3);
						SDL_RenderCopy(renderer,button1_texture,&src_button1,&dst_button1);
						SDL_RenderCopy(renderer,button2_texture,&src_button2,&dst_button2);
						SDL_RenderCopy(renderer,button3_texture,&src_button3,&dst_button3);

						if( position == 1){
							rectangleColor(renderer,160,10,260,60,0xff000000);
						}
						else if (position == 2){
							rectangleColor(renderer,320,10,420,60,0xff000000);
						}
						SDL_RenderPresent(renderer);
					}
					else if (event.key.keysym.sym == SDLK_LEFT ){ //左矢印を押された時
						if(position == 1){
							position = 0;
						}
						else if(position == 2){
							position = 1;
						}
						//ここはいらないかも？
						SDL_RenderCopy(renderer,texture1,NULL,&dst_rect1);
						SDL_RenderCopy(renderer,texture2,NULL,&dst_rect2);
						SDL_RenderCopy(renderer,texture3,NULL,&dst_rect3);
						SDL_RenderCopy(renderer,button1_texture,&src_button1,&dst_button1);
						SDL_RenderCopy(renderer,button2_texture,&src_button2,&dst_button2);
						SDL_RenderCopy(renderer,button3_texture,&src_button3,&dst_button3);

						if( position == 0){
							rectangleColor(renderer,10,10,110,60,0xff000000);
						}

						else if( position == 1 ){
							rectangleColor(renderer,160,10,260,60,0xff000000);
						}
						SDL_RenderPresent(renderer);
					}

					else if(event.key.keysym.sym == SDLK_RETURN){ //エンターキーが押された時
						//ここはいらないかも？
						SDL_RenderCopy(renderer,texture1,NULL,&dst_rect1);
						SDL_RenderCopy(renderer,texture2,NULL,&dst_rect2);
						SDL_RenderCopy(renderer,texture3,NULL,&dst_rect3);
						SDL_RenderCopy(renderer,button1_texture,&src_button1,&dst_button1);
						SDL_RenderCopy(renderer,button2_texture,&src_button2,&dst_button2);
						SDL_RenderCopy(renderer,button3_texture,&src_button3,&dst_button3);

						if( position == 0){
							SDL_RenderCopy(renderer,texture4,&src_rect2,&dst_rect1);
							SDL_RenderCopy(renderer,button1_texture,&src_button1,&dst_button1);
							SDL_RenderCopy(renderer,button2_texture,&src_button2,&dst_button2);
							SDL_RenderCopy(renderer,button3_texture,&src_button3,&dst_button3);
							rectangleColor(renderer,10,10,110,60,0xff000000);
							SDL_RenderCopy(renderer,likepicture,&src_like,&dst_like);
							SDL_RenderPresent(renderer);
						}

						else if( position == 1){
							color = rand()%5;
							if(color ==0) SDL_SetRenderDrawColor(renderer,255,0,0,0);

							else if(color ==1) SDL_SetRenderDrawColor(renderer,255,0,0,0);
							else if(color ==2) SDL_SetRenderDrawColor(renderer,0,255,0,0);
							else if(color ==3) SDL_SetRenderDrawColor(renderer,0,0,255,0);
							else if(color ==4) SDL_SetRenderDrawColor(renderer,255,0,255,0);
							SDL_RenderClear(renderer);
							SDL_RenderCopy(renderer,texture1,NULL,&dst_rect1);
							SDL_RenderCopy(renderer,texture2,NULL,&dst_rect2);
							SDL_RenderCopy(renderer,texture3,NULL,&dst_rect3);
							SDL_RenderCopy(renderer,texture4,&src_rect2,&dst_rect2);
							SDL_RenderCopy(renderer,button1_texture,&src_button1,&dst_button1);
							SDL_RenderCopy(renderer,button2_texture,&src_button2,&dst_button2);
							SDL_RenderCopy(renderer,button3_texture,&src_button3,&dst_button3);
							rectangleColor(renderer,160,10,260,60,0xff000000);
							SDL_RenderPresent(renderer);
						}


						else if( position == 2){
							printf("exit\n");
							SDL_Quit();
							exit(0);
						}
					}
					

					else if(event.key.keysym.sym == SDLK_ESCAPE){
						printf("exit\n");
						SDL_FreeSurface(image1);
						SDL_FreeSurface(image2);
						SDL_DestroyTexture(texture1);
						SDL_DestroyTexture(texture2);
						SDL_DestroyTexture(texture3);
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);
						IMG_Quit();
						SDL_Quit();

						exit(0);
						break;
					}
			}
		}
	}
	return 0;
}
