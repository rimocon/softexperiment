#include "header.h"
extern int i;
extern int j;
int read_database(void){// データベースファイルを読み込んで構造体の配列に格納する関数
  FILE* fp = fopen("./database.csv", "r");
  if ( fp == NULL ){
    printf("database file open error.\n");
    return -1;
  }
  char line[CN];//最初の列読み込み用
  int id;//ID
  char sex;//性別
  char name[256];//名前
  int by;//生まれた年
  int dy;//死んだ年
  char job[256];//職
  char place[256];//場所
  char works[4][256];//有名作品4つ
  int r;//行末確認用
  int lc = 0;//配列読み込み確認用カウンタ
  fscanf(fp, "%s", line); // 1行目は列名なので読み込んでおく
  int cy = 2019; // 現在の西暦
  // カンマ区切り(CSV)のファイルを1行ずつ読み込み、構造体に格納していく
  while( ( r = fscanf(fp, "%d,%c,%[^,],%d,%d,%[^,],%[^,],%[^,],%[^,],%[^,],%s", &id, &sex, name, &by, &dy, job, place, works[0], works[1], works[2], works[3]) ) != EOF ){
    lc += 1;
    int age = 0;
    if ( dy == 9999 ){//年齢計算
      age = (int)( (double)(cy - by) / 10.0 ) * 10;
    }else{
      age = (int)( (double)(dy - by) / 10.0 ) * 10;
    }
    //printf("%d:%s\t%d\t%s\n", lc, name, age, works[0]);//配列読み込み確認
    strcpy( ai[i].name, name);//構造体にそれぞれ代入
    ai[i].sex = sex;
    ai[i].age = age;
    ai[i].birth_year = by;
    ai[i].death_year = dy;
    strcpy(ai[i].job, job);
    strcpy(ai[i].place, place);
    for(j=0; j<4; j++){
      strcpy( ai[i].works[j], works[j] );
    }
    i++;
  }
  fclose(fp);
}
