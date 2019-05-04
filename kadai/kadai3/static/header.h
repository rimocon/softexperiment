#ifndef _HEADER
#define _HEADER

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

AI ai[MAX_LENGTH];

int read_database(void);

int kouho[MAX_LENGTH][5];

void question(void);

int answer(void);
#endif
