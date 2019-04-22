/*
   0,1が8x8並んだ行列を読み込み, 
   1に縦または横で挟まれている0を1に変換して
   テキストファイルに出力するプログラム
   (条件）2個以上の0を挟むことはできない
   101 -> 111

   1      1
   0  ->  1
   1      1

   コンパイル方法： gcc -o sample_prog sample_prog.c
   実行方法: ./sample_prog -i in.txt -o o.txt

 */
/////////////////////////////////////////////////////////////////////////////////////////////
//
// (1) 斜めで挟まれている0も1に変換するように修正せよ。 (sample_prog_kai1.c)
//
// (2) 縦横の両方が１で囲まれている場合のみ1に変換するように修正せよ。(sample_prog_kai2.c)
//
// (3) 出力されるファイルに、1にはさまれているのに1に変換できていないものが
//     ある（変換後の１に挟まれる）ので、それが残らないように修正せよ。(sample_prog_kai3.c)
//
// 注意）プログラミング作法の講義で学んだことを実践すること。
// 　　　関数化、インデント、コメントの付与, etc.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>

#define MAX_ROW 8
#define MAX_COL 8

/* 文字を数値に変換して返す */
int ctoi(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  return 0;
}

// 1に挟まれた0を1に変換する関数zero_to_one
// 更新後の数値は別の配列に保存
void zero_to_one(int karival[][MAX_COL],int nval[][MAX_COL]){
  for(int i=0; i<MAX_ROW; i++){
    for(int j=0; j<MAX_COL; j++){
      nval[i][j] = karival[i][j];//nvalにkarivalを代入
    }
  }

  for(int i=0; i<MAX_ROW; i++){
    for (int j=0; j<MAX_COL; j++){
      if ( karival[i][j] == 0 && j-1 >= 0 && j+1 < MAX_COL ){
        if ( ( karival[i][j-1] + karival[i][j+1] ) == 2 ){//横で挟んだとき
          nval[i][j] = 1;
        }
      }
      if ( karival[i][j] == 0 && i-1 >= 0 && i+1 < MAX_ROW ){
        if ( ( karival[i-1][j] + karival[i+1][j] ) == 2 ){//縦で挟んだとき
          nval[i][j] = 1;
        }
      }
      if ( karival[i][j] == 0 && j-1 >= 0 && j+1 <MAX_COL && i-1 >=0 && i+1 < MAX_ROW ){//縦の端じゃないかつ横の端じゃないとき
        if( ( karival[i-1][j-1] + karival[i+1][j+1] ) == 2 || ( karival[i-1][j+1] + karival[i+1][j-1] ) == 2 ){//斜めで挟んだ時 
          nval[i][j] = 1;
        }
      }
    }
  }
}

/* メイン関数：引数をとる */
int main(int argc, char** argv)
{
  int i, j, opt;
  int input = 0;
  int output = 0;
  char *ipath = NULL;
  char *opath = NULL;
  // オプション解析
  while ((opt = getopt(argc, argv, "i:o:")) != -1) {
    switch (opt) {
      case 'i':
        input = 1;
        ipath = optarg;
        break;
      case 'o':
        output = 1;
        opath = optarg;
        break;
      default: /* '?' */
        //指定していないオプションが渡された場合
        printf("Usage: %s [-i input file path] [-o output file path] arg1 ...\n", argv[0]);
        break;
    }
  }

  // ファイルが指定されていなければエラーメッセージを表示して終了
  if ( input == 1 ){
    printf("Input file: %s\n", ipath );
  }
  else{
    printf("No input file specified!\n");
    return -1;
  }

  if ( output == 1 ){
    printf("Output file: %s\n", opath );
  }
  else{
    printf("No output file specified!\n");
    return -1;
  }

  // 入力テキスト:ipathを読み込み配列に格納
  int val[MAX_ROW][MAX_COL] = { { 0 } };
  int nval[MAX_ROW][MAX_COL] = { { 0 } };
  FILE* fp = fopen(ipath, "r");
  if(fp == NULL) {
    printf("%s file not open!\n", ipath);
    return -1;
  }
  char line[MAX_COL+1];
  i = 0;
  while(fgets(line, MAX_COL+2, fp) != NULL) {
    for (j = 0; j<MAX_COL; j++){
      val[i][j] = ctoi( line[j] );
    }
    i++;
  }
  fclose(fp);
 
  zero_to_one(val,nval);//0を1に変換する関数

  FILE* wfp = fopen(opath, "w");//ファイル読み込み
  if( wfp == NULL) {
    printf("%s file not open!\n", opath);
    return -1;
  }
  for( i=0; i<MAX_ROW; i++){
    for( j=0; j<MAX_COL; j++){
      printf("%d,", nval[i][j]);
      fprintf(wfp, "%d", nval[i][j]);//ファイルに出力する.
    }
    printf("\n");
    fprintf(wfp, "\n");
  }
  fclose(wfp);
  return 0;
}
