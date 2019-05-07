/*
**
*****************************************************************************
**
** Project     : OpenCV sample project
** Module      : video_sdl.c
** Description : OpenCV と SDL の連携
**               OpenCV で取得したフレームをSDLのスクリーン中央に貼り付け 
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

#include "include/ipl2surface.h"

/*________ MACROS _________________________________________________________*/

/*________ DEFINITIONS ____________________________________________________*/
#define SDL_DISP_WIDTH   1280 // SDLの画面サイズ(横)
#define SDL_DISP_HEIGHT  1024 // SDLの画面サイズ(縦)

/*________ VARIABLES ______________________________________________________*/

/*________ LOCAL-FUNCTION PROTOTYPES ______________________________________*/

/*________ MAIN-FUNCTION __________________________________________________*/

int main(int argc, char **argv)
{
    CvCapture *capture = 0;
    IplImage *frame = 0;
    SDL_Surface *screen;
    SDL_Rect position;
    SDL_Surface *sdl_frame;
    SDL_Event event;
    unsigned char *keys;


    // カメラデバイスのオープン
    capture = cvCreateCameraCapture(0);
    if (capture == NULL) {
        fprintf(stderr, "*Error* cannot open /dev/video0\n");
        return -1;
    }

    // SDLの初期化と画面設定
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(SDL_DISP_WIDTH, SDL_DISP_HEIGHT, 0, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "*Error* cannot set 640x480 video mode: %s\n",
                            SDL_GetError());
        return -1;
    }

    while (1) {
        // 1フレームの取り込み
    	frame = cvQueryFrame(capture);

        // OpenCV形式からSDL形式にフレームを変換
        sdl_frame = ipl_to_surface(frame);

        // フレーム(sdl_frame)のSDLでの表示(screen)位置(左上から)を設定
        position.x = 320;
        position.y = 240;
        SDL_BlitSurface(sdl_frame, NULL, screen, &position);

        // 画面全体を更新，バッファ更新
        SDL_UpdateRect(screen, 0, 0, 0, 0);
        SDL_Flip(screen);

        // イベントをポーリングで検出
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return -1;
            }
        }

        // キーボードの状態を取得
        keys = SDL_GetKeyState(NULL);
        if (keys[SDLK_ESCAPE] == SDL_PRESSED) { // Escapeで終了
            break;
        }
    }

    // カメラデバイス解放
    cvReleaseCapture(&capture);

    // SDL後始末
    SDL_FreeSurface(screen);
    SDL_FreeSurface(sdl_frame);
    SDL_Quit();

    return 0;
}


/*________ LOCAL-FUNCTIONS ________________________________________________*/

