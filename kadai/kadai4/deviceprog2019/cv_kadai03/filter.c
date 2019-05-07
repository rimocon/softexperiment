//インクルード
#include <stdio.h>
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

int main( int argc, char **argv ) {

   char windowNameInputImage[] = "Input";
   char windowNameOutputImage[] = "Output";
   IplImage *inputImage,*outputImage;

   //画像を読み込む
   //入力用と出力用のIplImageを作成
   inputImage = cvLoadImage("lena.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR );
   if ( inputImage== NULL) {
      //	画像が見つからなかった場合
      printf( "画像が見つかりません\n" );
      return -1;
   }
   outputImage =cvCloneImage(inputImage);
   
   //平滑化フィルタをかける 
   cvSmooth( inputImage ,outputImage,CV_GAUSSIAN,1,0,0,0);

   //ウィンドウを作成
   cvNamedWindow( windowNameInputImage,CV_WINDOW_AUTOSIZE );
   cvNamedWindow( windowNameOutputImage,CV_WINDOW_AUTOSIZE );

   //画像を表示する
   cvShowImage( windowNameInputImage, inputImage);
   cvShowImage( windowNameOutputImage, outputImage);

   //	キー入力を待つ
   cvWaitKey(0);
   
   //	画像を保存する
   cvSaveImage( "output.bmp", outputImage, 0 );
   cvReleaseImage( &inputImage);
   cvReleaseImage( &outputImage);
   //	ウィンドウを破棄する
   cvDestroyWindow(windowNameInputImage);
   cvDestroyWindow(windowNameOutputImage);

   return 0;
}
