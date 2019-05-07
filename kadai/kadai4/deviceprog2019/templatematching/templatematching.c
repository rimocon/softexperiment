//---------------------------------------------------------
// 概要      : テンプレートマッチング
// File Name : template.cpp
//---------------------------------------------------------

#include <stdio.h>
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

#define THRESHOLD	50     //	2値化の際の閾値
#define THRESHOLD_MAX_VALUE	255     //	2値化の際に使用する最大値

#define LINE_THICKNESS	1	//	線の太さ
#define	LINE_TYPE	8		//	線の種類
#define SHIFT	0			//	座標の小数点以下の桁を表すビット数

int main( int argc, char **argv ) {

   char windowNameTemplate[] = "Template";			//	テンプレート画像を表示するウィンドウの名前
   char windowNameDestination[] = "Destination";	//	マッチング結果を表示するウィンドウの名前

   CvPoint minLocation;	//	相違度が最小になる場所

   //	画像を読み込む
   IplImage *sourceImage = cvLoadImage( "image.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
   IplImage *templateImage = cvLoadImage( "template.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );

   if ( sourceImage == NULL || templateImage == NULL ) {
      //	画像が見つからなかった場合
      printf( "画像が見つかりません\n" );
      return -1;
   }

   //	画像を生成する
   //	元画像をグレースケール化した画像用IplImage
   IplImage *sourceGrayImage = cvCreateImage( cvGetSize(sourceImage), IPL_DEPTH_8U, 1 );
   //	テンプレート画像をグレースケール化した画像用IplImage	
   IplImage *templateGrayImage = cvCreateImage( cvGetSize(templateImage), IPL_DEPTH_8U, 1 );
   //	元画像を2値化した画像用IplImage	
   IplImage *sourceBinaryImage = cvCreateImage( cvGetSize(sourceImage), IPL_DEPTH_8U, 1 );
   //	テンプレート画像を2値化した画像用IplImage		
   IplImage *templateBinaryImage = cvCreateImage( cvGetSize(templateImage), IPL_DEPTH_8U, 1 );
   //	相違度マップ画像用IplImage
   IplImage *differenceMapImage = cvCreateImage( cvSize( sourceImage->width - templateImage->width + 1, sourceImage->height - templateImage->height + 1 ), IPL_DEPTH_32F, 1 );	

   //	BGRからグレースケールに変換する
   cvCvtColor( sourceImage, sourceGrayImage, CV_BGR2GRAY );
   cvCvtColor( templateImage, templateGrayImage, CV_BGR2GRAY );

   //	グレースケールから2値に変換する
   cvThreshold( sourceGrayImage, sourceBinaryImage, THRESHOLD, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );
   cvThreshold( templateGrayImage, templateBinaryImage, THRESHOLD, THRESHOLD_MAX_VALUE, CV_THRESH_BINARY );

   //	テンプレートマッチングを行う
   cvMatchTemplate( sourceBinaryImage, templateBinaryImage, differenceMapImage, CV_TM_SQDIFF );

   //	テンプレートが元画像のどの部分にあるのかという情報を得る
   cvMinMaxLoc( differenceMapImage, NULL, NULL, &minLocation, NULL, NULL );

   //	一致する場所を元画像に四角で描く
   cvRectangle(
         sourceImage,
         minLocation,
         cvPoint( minLocation.x + templateImage->width, minLocation.y + templateImage->height ),
         CV_RGB( 255, 0, 0 ),
         LINE_THICKNESS,
         LINE_TYPE,
         SHIFT
         );

   //	ウィンドウを生成する
   cvNamedWindow( windowNameTemplate, CV_WINDOW_AUTOSIZE );
   cvNamedWindow( windowNameDestination, CV_WINDOW_AUTOSIZE );

   //	画像を表示する
   cvShowImage( windowNameTemplate, templateImage );
   cvShowImage( windowNameDestination , sourceImage );

   //	キー入力を待つ
   cvWaitKey(0);

   //	画像を保存する
   cvSaveImage( "destination.bmp", sourceImage, 0 );

   //	メモリを解放する
   cvReleaseImage( &sourceImage );
   cvReleaseImage( &templateImage );
   cvReleaseImage( &sourceGrayImage );
   cvReleaseImage( &templateGrayImage );
   cvReleaseImage( &sourceBinaryImage );
   cvReleaseImage( &templateBinaryImage );
   cvReleaseImage( &differenceMapImage );
   //	ウィンドウを破棄する
   cvDestroyWindow(windowNameTemplate);
   cvDestroyWindow(windowNameDestination);

   return 0;
}
