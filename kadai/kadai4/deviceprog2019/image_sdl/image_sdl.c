/*
**
*****************************************************************************
**
** Project     : OpenCV sample project
** Module      : image_cv.c
** Description : OpenCVによる画像の読み込み(カラープレーンの置き換え)
**
** Version : Date:          Author:       Comment:
**     1.0   2011/09/16(金) Akinori Tsuji Creation
*****************************************************************************
**
*/
/*________ INCLUDES _______________________________________________________*/
#include <stdio.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/*________ MACROS _________________________________________________________*/
/*________ DEFINITIONS ____________________________________________________*/
//#define CONVERT_RGB
#define SDL_DISP_WIDTH   640 // SDLの画面サイズ(横)
#define SDL_DISP_HEIGHT  480 // SDLの画面サイズ(縦)

/*________ VARIABLES ______________________________________________________*/
/*________ LOCAL-FUNCTION PROTOTYPES ______________________________________*/
static SDL_Surface *ipl_to_surface(IplImage *img); // 画像をIPLからSDLへ変換

/*________ LOCAL-FUNCTIONS ________________________________________________*/

/*________ MAIN-FUNCTION __________________________________________________*/

int main(int argc, char **argv)
{
    int x, y;
    uchar p[3];
    IplImage *img;
    SDL_Surface *screen;
    SDL_Surface *sdl_img;
    SDL_Rect position;
    SDL_Event event;
    unsigned char *keys;

    img = cvLoadImage("test.jpg", CV_LOAD_IMAGE_COLOR);
    if (img == NULL) {
        fprintf(stderr, "*Error* cannot open test.jpg\n");
        return;
    }

#ifdef CONVERT_RGB
    for (y = 0; y < img->height; y++) {
	for (x = 0; x < img->width; x++) {
	    p[0] = img->imageData[img->widthStep * y + x * 3];	    // B
	    p[1] = img->imageData[img->widthStep * y + x * 3 + 1];	// G
	    p[2] = img->imageData[img->widthStep * y + x * 3 + 2];	// R

// Image Processing
	    img->imageData[img->widthStep * y + x * 3] =
    		cvRound(p[0] * 0.7);
	    img->imageData[img->widthStep * y + x * 3 + 1] =
    		cvRound(p[1] * 1.0);
	    img->imageData[img->widthStep * y + x * 3 + 2] =
    		cvRound(p[2] * 0.0);
	}
    }
#endif

    sdl_img = ipl_to_surface(img);


    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(SDL_DISP_WIDTH, SDL_DISP_HEIGHT, 0, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "*Error* cannot set 640x480 video mode: %s\n",
                            SDL_GetError());
        return;
    }

    while (1) {
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
        position.x = position.y = 100;
        SDL_BlitSurface(sdl_img, NULL, screen, &position);   
        SDL_Flip(screen);

        // イベントをポーリングで検出
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            }
        }

        // キーボードの状態を取得
        keys = SDL_GetKeyState(NULL);
        if (keys[SDLK_ESCAPE] == SDL_PRESSED) { // Escapeで終了
            break;
        }
    }

    SDL_FreeSurface(screen);
    SDL_FreeSurface(sdl_img);
    SDL_Quit();

    cvReleaseImage(&img);

    return 0;
}


/*________ LOCAL-FUNCTIONS ________________________________________________*/

/* OpenCV形式画像をSDL形式画像に変換 */
static SDL_Surface *ipl_to_surface(IplImage *img) {
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
        (void *)img->imageData,
        img->width,
        img->height,
        img->depth*img->nChannels,
        img->widthStep,
        0x00ff0000, 0x0000ff00, 0x000000ff, 0);

    return surface;
}


