/*

   著者当てプログラム
   2019.04.17: Kazuyuki Matsumoto

   データベースの中の著者から、1名選び、
   その著者についての質問に Yes/No で回答する。
   プログラムは、回答結果をもとにどの著者かを言い当てる

 */

// コンパイル方法:
// $ gcc -o sample_lib sample_lib.c
//
// 実行方法:
// $ ./sample_lib
//
// (1) 関数に切り分ける
// (2) ヘッダファイルに共通して使用する変数やマクロを記述する
// (3) 冗長な記述をまとめる
// (4) （発展課題）：　質問回数を減らし、ほぼ確実に言い当てることが
//    できるように改良（作品名は固有なので、そのままの形で質問に含めてはいけない）
//unko
#include "header.h"
int i,j = 0;
int item_num = 0;
int threshold = 4;

int main( int argc, char** argv )
{ 
  //読み込み
  read_database();
  //質問
  question();
  // 回答
  answer();
}
