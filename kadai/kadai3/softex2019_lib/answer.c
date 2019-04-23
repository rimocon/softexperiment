#include "header.h"

extern int i;
extern int j;
extern int item_num;
extern int threshold;
int answer(void){//回答部分
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


