#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main( int argc,char *argv[]);
{
	SDL_init(SDL_INIT_VIDEO); //SDLの初期化
	IMG_Init(IMG_INIT_PNG); //PNG形式で初期化
	TTF_Init(); //SDL2_ttfを初期化
	/*変数宣言*/
	SDL_Event event; //event用の変数を宣言
	SDL_Surface *button1_string,*button2_string,*button3_string; //ボタン用の変数を宣言
	TTF_Font *font = TTF_OpenFont("font-japanese-gothic.ttf",24); //フォントサイズ24でフォントを読み込む
	SDL_Color white = {0xFF,0xFF,0xFF};
	SDL_Color black = {0x00,0x00,0x00};

	SDL_Window *window = SDL_CreateWindow("kadai2",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,400,0); //ウィンドウの生成
	if (window == NULL){
		printf("ウィンドウを生成できませんでした.\n");
		exit(1);
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0); //レンダラーの生成
	SDL_SetRenderDrawColor(renderer,0,0,0,0); //ウィンドウを黒に設定
	SDL_RenderClear(renderer); //設定色でクリア
	//画像の読み込み
	SDL_Surface *image1 = IMG_Load("button1.png");
	SDL_Surface *image2 = IMG_Load("button2.png");

	if(!(image1 && image2)){
		printf("画像を読み込めませんでした\n",IMG_GetError());
	}
	//テクスチャの生成
	SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer,image1);
	SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer,image1);
	SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer,image1);
	SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer,image2);
	//テクスチャの情報
	SDL_QueryTexture(texture1,NULL,NULL,&image1->w,image1->h);
	SDL_QueryTexture(texture4,NULL,NULL,&image2->w,image2->h);
	//コピー元画像のサイズ設定
	SDL_Rect src_rect1 = {0,0,image1->w,image1->h}
	SDL_Rect src_rect2 = {0,0,image2->w,image2->h}
	//コピー先画像のサイズ設定
	SDL_Rect dst_rect1 = {20,20,100,50};
	SDL_Rect dst_rect2 = {170,20,100,50};
	SDL_Rect dst_rect3 = {320,20,100,50};
	//テクスチャをレンダラーにコピー
	SDL_RenderCopy(renderer,texture1,&src_rect1,&dst_rect1);
	SDL_RenderCopy(renderer,texture2,&src_rect2,&dst_rect2);
	SDL_RenderCopy(renderer,texture3,&src_rect3,&dst_rect3);

	//好きな画像の読み込み

	SDL_Surface *picture = IMG_Load("like.png");
	SDL_Texture *likepicture =SDL_CreateTextureFromSurface(renderer,picture);
	SDL_Rect src_like ={0,0,picture->w,picture->h};
	SDL_Rect dst_like = {10,100,300,250};

	rectangle Color(20,20,120,70,0xff000000); //赤色で囲う









