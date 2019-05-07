#include"header.h"
extern int i;
extern int j;
extern int item_num;
extern int threshold;

void question(void){
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

