/*
 *  ファイル名	: system.h
 *  機能	: 共通変数，外部関数の定義
 */
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <SDL2/SDL.h>

/* マップサイズ */
enum {
    MAP_Width    = 30,
    MAP_Height   = 10,
    WD_Width     = 15,
    WD_Height    = 10,
    MAP_ChipSize = 48
};

/* マップの種類 */
typedef enum {
    MT_None    = 0,
    MT_Block   = 1, /* 床・壁 */
    MT_Player  = 2, /* プレイヤー */
    MT_Station = 3  /* 拠点 */
} MapType;

/* メッセージ */
typedef enum {
    MSG_None     = 0,
    MSG_GameOver = 1,
    MSG_Clear    = 2,
    MSG_Warning  = 3,
    MSG_NUM      = 4 /* メッセージの数 */
} Msg;

/* キャラクタータイプ */
typedef enum {
    CT_Player   = 0, /* プレイヤー */
    CT_Ball     = 1, /* 球 */
    CT_Enemy    = 2, /* 敵 */
    CT_Station  = 3, /* 拠点 */
    CT_Boss     = 4, /* ボス */
    CT_BossBall = 5, /* ボスの球 */
    CT_NUM      = 6  /* タイプの数 */
} CharaType;

/* キャラクターの状態 */
typedef enum {
    CS_Disable = 0, /* 非表示 */
    CS_Enable  = 1, /* 表示 */
    CS_Boss1   = 2, /* ボスの行動1 */
    CS_Boss2   = 3  /* ボスの行動2 */
} CharaStts;

/* キャラクターの向き */
typedef enum {
    CD_Right = 1,
    CD_Left  = -1
} CharaDir;

/* ゲームの状態 */
typedef enum {
    GS_End     = 0, /* 終了 */
    GS_Playing = 1, /* 通常 */
    GS_Warning = 2, /* 警告 */
    GS_Boss    = 3  /* ボス */
} GameStts;

/* 入力の状態 */
typedef struct {
    SDL_bool up;
    SDL_bool right;
    SDL_bool left;
    SDL_bool button1;
} InputStts;

/* 実数座標 */
typedef struct {
    float x;
    float y;
} FloatPoint;

/* キャラクター画像の情報 */
typedef struct {
    int w;         /* キャラの幅 */
    int h;         /* 　　　　高さ */
    int imgW;      /* 画像の幅 */
    int imgH;      /* 　　　高さ */
    SDL_Rect mask; /* 当たり判定の範囲(相対位置) */
    SDL_Texture *image;
} CharaImgInfo;

/* キャラクターの情報 */
typedef struct {
    CharaType type;
    CharaStts stts;
    int hp;              /* HP */
    CharaDir dir;        /* 向き */
    FloatPoint point;    /* 計算上の座標 */
    FloatPoint basevel;  /* 速度の基準値 */
    FloatPoint velocity; /* 速度 */
    SDL_Rect rect;       /* 位置(マップ上の座標) */
    SDL_Rect src;        /* テクスチャの範囲(アニメーション用) */
    CharaImgInfo *img;
} CharaInfo;

/* ゲームの情報 */
typedef struct {
    GameStts stts;
    InputStts input;
    int restTime;     /* 待ち時間 */
    int player;       /* プレイヤーの番号 */
    int boss;         /* ボスの番号 */
    int restStations; /* 拠点の残り数 */
    float timeStep;   /* 時間の増分 */
    Msg msg;          /* メッセージ */
    SDL_Rect rectMap; /* 画面表示領域 */
    SDL_Rect rectBg;  /* 画面表示領域 */
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Texture *bg;
    SDL_Texture *map;
    SDL_Texture *msgTexts[MSG_NUM];
    SDL_Surface *mapMask;
} GameInfo;

/* 変数 */
extern GameInfo gGame;
extern CharaImgInfo gCharaImg[CT_NUM];
extern CharaInfo *gChara;
extern int gCharaNum; /* キャラ総数 */

/* 関数 */
/* act.c */
extern int PrintError(const char *str);
/* system.c */
extern int InitSystem(void);
extern void DestroySystem(void);
extern void UpdateCharaStatus(CharaInfo *ch);
extern void MoveChara(CharaInfo *ch);
extern void Collision(CharaInfo *ci, CharaInfo *cj);
extern SDL_Rect MeasureMask(SDL_Surface *s, SDL_Rect r);
extern void SetInput(void);
extern void SetInitPoint(CharaType ct, SDL_Rect r);
extern int AdjustXrange(int x);
extern float AdjustXrangeF(float x);
/* window.c */
extern int InitWindow(void);
extern void DestroyWindow(void);
extern int RenderWindow(void);

#endif
/* end of system.h */
