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

/*________ MACROS _________________________________________________________*/
/*________ DEFINITIONS ____________________________________________________*/
#define CONVERT_RGB

/*________ VARIABLES ______________________________________________________*/
/*________ LOCAL-FUNCTION PROTOTYPES ______________________________________*/
/*________ LOCAL-FUNCTIONS ________________________________________________*/

/*________ MAIN-FUNCTION __________________________________________________*/

int main(int argc, char **argv)
{
   int x, y;
   uchar p[3];
   IplImage *img;
   IplImage *img2;
   img = cvLoadImage("test.jpg", CV_LOAD_IMAGE_COLOR);
   if (img == NULL) {
      fprintf(stderr, "*Error* cannot open test.jpg\n");
      return -1;
   }
   img2 = cvCloneImage(img);


#ifdef CONVERT_RGB
   for (y = 0; y < img->height; y++) {
      for (x = 0; x < img->width; x++) {
         p[0] = img->imageData[img->widthStep * y + x * 3];	        // B
         p[1] = img->imageData[img->widthStep * y + x * 3 + 1];	// G
         p[2] = img->imageData[img->widthStep * y + x * 3 + 2];	// R

         // Image Processing

         img->imageData[img->widthStep * y + x * 3] =
            cvRound(p[0] * 0.0);
         img->imageData[img->widthStep * y + x * 3 + 1] =
            cvRound(p[1] * 0.0);
         img->imageData[img->widthStep * y + x * 3 + 2] =
            cvRound(p[2] * 1.0);
      }
   }
#endif

   cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
   cvShowImage("Image", img);
   cvWaitKey(0);

   cvDestroyWindow("Image");
   cvReleaseImage(&img);
   
   cvNamedWindow("Image2",CV_WINDOW_AUTOSIZE);
   cvShowImage("Image2",img2);
   cvWaitKey(0);

   cvDestroyWindow("Image2");
   cvReleaseImage(&img2);

   return 0;
}

