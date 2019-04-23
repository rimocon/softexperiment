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
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CN 1000
#define MAX_LENGTH 55

typedef struct authorInfo {
  char name[256];
  char sex; // 性別 (M/F)
  int age; // 現在の年齢 or 享年 の年代
  int birth_year; // 誕生年
  int death_year; // 没年
  char job[256]; // 職業
  char place[256]; // 生誕地
  char works[4][256]; // 作品
}AI;

/*グローバル変数*/

int i = 0;
int j = 0;
AI ai[MAX_LENGTH];
int threshold = 4; // yの数がthreshold以上のときに質問を打ち切る
int kouho[MAX_LENGTH][5];
int item_num = 0;

int read_database(){// データベースファイルを読み込んで構造体の配列に格納する関数
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

int question(){
  /*乱数初期化*/
  srand(time(NULL));
  item_num = i;
  char gen[2][10];
  strcpy( gen[0], "男性");
  strcpy( gen[1], "女性");

  // 質問
  int n = 0;
  int maxC = 30; // 質問ループの上限
  for (i=0; i<item_num; i++){
    for (j=0; j<5; j++){
      kouho[i][j] = 0;
    }
  }

  int k = 0;
  char dummy;
  AI ans; // 正解候補格納用
  ans.sex = 'N';
  ans.age = 0;
  ans.birth_year = 0;
  ans.death_year = 0;
  strcpy(ans.job, "N");
  strcpy(ans.place, "N");

  int ji = 0;
  char job_nglist[MAX_LENGTH][256];
  int pi = 0;
  char place_nglist[MAX_LENGTH][256];
  int yi = 0;
  int age_nglist[MAX_LENGTH];
  int check_ids[MAX_LENGTH];
  for(i=0; i<MAX_LENGTH; i++){
    check_ids[i] = 0;
  }
  int ci = 0;
  int l = 0;
  while(n < maxC)
  {
    int bflag = 0;
    int chk =0;
    char yn = '-';
    int x = rand() % item_num; 
    // チェック済みか否か
    if ( check_ids[x] != 0){ bflag = 1; }
    // 既にチェックして該当しない出生地か否か
    for(l=0; l<pi; l++){
      if ( strcmp(place_nglist[l], ai[x].place) == 0 ){
        bflag=1;
        break;
      }
    }
    // 既にチェックして該当しない職業か否か
    for(l=0; l<ji; l++){
      if ( strcmp(job_nglist[l], ai[x].job) == 0 ){
        bflag=1;
        break;
      }
    }
    // 既にチェックして該当しない年代か否か
    for(l=0; l<yi; l++){
      if ( age_nglist[l] == ai[x].age ){
        bflag=1;
        break;
      }
    }
    if (bflag!=0){ n++; continue; } // 該当しなければ次をチェック

    check_ids[ci] = x;
    ci++;
    if ( strcmp(ans.place, "N") == 0 ){ 
      printf("その人は「%s」出身ですか？[y/n]\n", ai[x].place);
      yn = 'n';
      scanf("%c", &yn); scanf("%c", &dummy);
      if (yn == 'y'){ 
        kouho[x][0]++; 
        strcpy( ans.place, ai[x].place );
        chk++;
      }else{
        strcpy(place_nglist[pi], ai[x].place);
        pi++;
      }
    }
    else{
      if (strcmp(ans.place, ai[x].place) != 0){
        continue;
      }
    }
    if ( ans.sex == 'N' ){
      int gi = 0;
      if ( ai[x].sex == 'F' ){ gi = 1; }
      printf("その人は「%s」ですか？[y/n]\n", gen[gi] );   
      yn = 'n'; 
      scanf("%c", &yn); scanf("%c", &dummy);
      if (yn == 'y'){ 
        kouho[x][1]++; 
        ans.sex = ai[x].sex;
        chk++;
      }
      else{
        if (ai[x].sex == 'F'){ ans.sex = 'M'; }
        else{ ans.sex = 'F';}
      }
    }
    else{
      if (ans.sex != ai[x].sex){
        continue;
      }else{
        kouho[x][1]++;
      }
    }
    if (ans.age == 0)
    {
      if ( ai[x].death_year == 9999 ){
        printf("その人の年齢は%d代ですか？[y/n]\n", ai[x].age );
        yn = 'n';
        scanf("%c", &yn); scanf("%c", &dummy);
        if ( yn == 'y'){ 
          kouho[x][2]++; 
          ans.age = ai[x].age;
          chk++; 
        }else{
          age_nglist[yi] = ai[x].age;
          yi++;
        }
      }else{
        printf("その人は[%d]年に亡くなりましたか？[y/n]\n", ai[x].death_year);
        yn = 'n';
        scanf("%c", &yn); scanf("%c", &dummy);
        if ( yn == 'y'){ 
          kouho[x][3]++; 
          ans.death_year = ai[x].death_year;
          ans.birth_year = ai[x].birth_year;
          ans.age = ai[x].age;
          chk++; 
        }
      }
    }
    else{
      if (ans.age != ai[x].age){
        continue;
      }else{
        kouho[x][2]++;
      }
      if ( ans.death_year == ai[x].death_year ){
        kouho[x][3]++;
      }
    }
    if ( strcmp(ans.job, "N") == 0 )
    {
      printf("その人は、「%s」ですか？[y/n]\n", ai[x].job);
      yn = 'n';
      scanf("%c", &yn); scanf("%c", &dummy);
      if ( yn == 'y'){ 
        kouho[x][4]++; 
        strcpy(ans.job, ai[x].job);
        chk++; 
      }else{
        strcpy(job_nglist[ji], ai[x].job);
        ji++;
      }
    }
    else{
      if ( strcmp(ans.job, ai[x].job) != 0 ){
        continue;
      }else{
        kouho[x][4]++;
      }
    }
    n++; 

    int kvmax = 0;
    for(i=0; i<item_num;i++){
      int kv = 0;
      for(j=0; j<5; j++){
        kv += kouho[i][j];
      }
      if ( kvmax < kv ){
        kvmax = kv;
      }
    }
    if ( kvmax >= threshold ){
      break;
    }
  }
}

int answer(){//回答部分
  int max_vote = 0;
  int max_kouho = -1;
  for(i=0; i<item_num; i++)
  {
    int kv = 0;
    for(j=0;j<5;j++){
      kv += kouho[i][j];
    }
    if( max_vote < kv ){
      max_vote = kv;
      max_kouho = i;
      printf("<%s>\t%d\n", ai[i].name, max_vote);
    }    
  }
  if (max_vote >= threshold){
    printf("その人は、[%s]ですね？\n", ai[max_kouho].name);
    printf("-------[ 作品例 ]--------\n");
    for(i=0; i<4; i++){
      if ( strcmp(ai[max_kouho].works[i], "NONE") != 0 ){
        printf("\t%s\n", ai[max_kouho].works[i]);
      }
    }
    printf("-------------------------\n");
  }else{
    printf("わかりませんでした！\n");
  }
  return 1;
}

int main( int argc, char** argv )
{
  //読み込み
  read_database();
  //質問
  question();
  // 回答
  answer();
}
