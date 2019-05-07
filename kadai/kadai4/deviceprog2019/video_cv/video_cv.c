/*
**
*****************************************************************************
**
** Project     : OpenCV sample program
** Module      : video_cv.c
** Description : OpenCV によるUSBカメラ使用
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
#define CV_DISP_WIDTH   320 // 画面サイズ(横)
#define CV_DISP_HEIGHT  240 // 画面サイズ(縦)

/*________ VARIABLES ______________________________________________________*/
/*________ LOCAL-FUNCTION PROTOTYPES ______________________________________*/
/*________ LOCAL-FUNCTIONS ________________________________________________*/

/*________ MAIN-FUNCTION __________________________________________________*/

int main(int argc, char **argv)
{
    CvCapture *capture = 0;
    IplImage *frame = 0;
    double w = CV_DISP_WIDTH, h = CV_DISP_HEIGHT;
    int ckey;


    // カメラのオープン(/dev/video0)
    capture = cvCreateCameraCapture(0);
    if (capture == NULL) {
        fprintf(stderr, "*Error* cannot open /dev/video0\n");
        return -1;
    }

    // キャプチャサイズの設定
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h);

    cvNamedWindow("Capture", CV_WINDOW_AUTOSIZE);

    // カメラからフレームをキャプチャ
    while (1) {
    	frame = cvQueryFrame(capture);
    	cvShowImage("Capture", frame);
    	ckey = cvWaitKey(10);
    	if ((ckey & 0xff) == '\x1b') {
            printf("Exiting ...\n");
            cvSaveImage("image.png",frame,0);
            break; // エスケープキーで終了
        }
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("Capture");

    return 0;
}


