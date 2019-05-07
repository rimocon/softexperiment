/*
**
*****************************************************************************
**
** Project     : OpenCV sample project
** Module      : ipl2surface.c
** Description : OpenCVの画像データをSDLの画像データに変換する関数
**
** Version : Date:          Author:       Comment:
**     1.0   2011/09/28(水) Akinori Tsuji Creation
*****************************************************************************
**
*/

/*________ INCLUDES _______________________________________________________*/
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <SDL/SDL.h>

#include "include/ipl2surface.h"

/*________ MACROS _________________________________________________________*/

/*________ DEFINITIONS ____________________________________________________*/

/*________ VARIABLES ______________________________________________________*/

/*________ LOCAL-FUNCTION PROTOTYPES ______________________________________*/

/*________ LOCAL-FUNCTIONS ________________________________________________*/

/* OpenCV形式画像をSDL形式画像に変換 */
SDL_Surface *ipl_to_surface(IplImage *img) {
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
        (void *)img->imageData,
        img->width,
        img->height,
        img->depth*img->nChannels,
        img->widthStep,
        0x00ff0000, 0x0000ff00, 0x000000ff, 0);

    return surface;
}


