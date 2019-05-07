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
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

/*________ MACROS _________________________________________________________*/
/*________ DEFINITIONS ____________________________________________________*/
#define CV_DISP_WIDTH   320 // 画面サイズ(横)
#define CV_DISP_HEIGHT  240 // 画面サイズ(縦)
#define THRESHOLD	127     //	2値化の際の閾値
#define THRESHOLD_MAX_VALUE	255     //	2値化の際に使用する最大値
#define LINE_THICKNESS	1	//	線の太さ
#define	LINE_TYPE	8		//	線の種類
#define SHIFT	0			//	座標の小数点以下の桁を表すビット数



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
   char windowNameTemplate[] = "Template";			//	テンプレート画像を表示するウィンドウの名前
   char windowNameBinaryTemplate[] = "BinaryTemplate";	//2値化テンプレート画像を表示するウィンドウの名前
   CvPoint minLocation;	//	相違度が最小になる場所

   //	画像を読み込む
   IplImage *templateImage = cvLoadImage( "template.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );

   if ( templateImage == NULL ){
      //	画像が見つからなかった場合
      printf( "画像が見つかりません\n" );
      return -1;
   }
   //	テンプレート画像をグレースケール化した画像用IplImage	
   IplImage *templateGrayImage = cvCreateImage( cvGetSize(templateImage), IPL_DEPTH_8U, 1 );
   //	テンプレート画像を2値化した画像用IplImage		
   IplImage *templateBinaryImage = cvCreateImage( cvGetSize(templateImage), IPL_DEPTH_8U, 1 );
   //	BGRからグレースケールに変換する
   cvCvtColor( templateImage, templateGrayImage, CV_BGR2GRAY );
   //	グレースケールから2値に変換する
   cvThreshold( templateGrayImage, templateBinaryImage, THRESHOLD, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );
   // カメラのオープン(/dev/video0)
   capture = cvCreateCameraCapture(0);
   if (capture == NULL) {
      fprintf(stderr, "*Error* cannot open /dev/video0\n");
      return -1;
   }

   // キャプチャサイズの設定
   cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, w);
   cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, h);
   // ウィンドウを生成
   cvNamedWindow("Capture", CV_WINDOW_AUTOSIZE);
   cvNamedWindow("binaryCapture",CV_WINDOW_AUTOSIZE);
   cvNamedWindow( windowNameTemplate, CV_WINDOW_AUTOSIZE);
   cvNamedWindow( windowNameBinaryTemplate,CV_WINDOW_AUTOSIZE);

   cvShowImage( windowNameTemplate,templateImage);
   cvShowImage( windowNameBinaryTemplate,templateBinaryImage);
   // カメラからフレームをキャプチャ
   while (1) {
      frame = cvQueryFrame(capture);

      //	元画像をグレースケール化した画像用IplImage
      IplImage *sourceGrayImage = cvCreateImage( cvGetSize(frame),IPL_DEPTH_8U,1);
      //	元画像を2値化した画像用IplImage	
      IplImage *sourceBinaryImage = cvCreateImage( cvGetSize(frame),IPL_DEPTH_8U,1);
      //	相違度マップ画像用IplImage
      IplImage *differenceMapImage = cvCreateImage( cvSize( frame->width - templateImage->width + 1, frame->height - templateImage->height + 1 ), IPL_DEPTH_32F, 1 );


      //	BGRからグレースケールに変換する
      cvCvtColor ( frame,sourceGrayImage,CV_BGR2GRAY );
      //	グレースケールから2値に変換する
      cvThreshold( sourceGrayImage, sourceBinaryImage, THRESHOLD, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );
      //	テンプレートマッチングを行う
      cvMatchTemplate( sourceBinaryImage, templateBinaryImage, differenceMapImage, CV_TM_SQDIFF );
      //	テンプレートが元画像のどの部分にあるのかという情報を得る
      cvMinMaxLoc( differenceMapImage, NULL, NULL, &minLocation, NULL, NULL );

      //	一致する場所を元画像に四角で描く
      cvRectangle(
            frame,
            minLocation,
            cvPoint( minLocation.x + templateImage->width, minLocation.y + templateImage->height ),
            CV_RGB( 255, 0, 0 ),
            LINE_THICKNESS,
            LINE_TYPE,
            SHIFT
            );
      //キャプチャ映像の表示
      cvShowImage("Capture", frame);
      // 2値化の映像を表示
      cvShowImage("binaryCapture",sourceBinaryImage);


      ckey = cvWaitKey(10);
      cvReleaseImage(&sourceBinaryImage);
      cvReleaseImage(&sourceGrayImage);
      cvReleaseImage(&differenceMapImage);
      if ((ckey & 0xff) == '\x1b') {
         printf("Exiting ...\n");
         cvSaveImage("image.png",frame,0);
         break; // エスケープキーで終了
      }
   }

   //メモリを解放する
   cvReleaseImage(&templateImage);
   cvReleaseImage(&templateGrayImage);
   cvReleaseImage(&templateBinaryImage);
   cvReleaseCapture(&capture);

   //windowを閉じる
   cvDestroyWindow("Capture");
   cvDestroyWindow("binaryCapture");
   cvDestroyWindow(windowNameTemplate);
   cvDestroyWindow(windowNameBinaryTemplate);
   return 0;
}


