/*
 *  ファイル名	：system.c
 *  機能	：ゲームシステム処理
 */
#include "system.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* データファイルパス */
static char gCharaDataFile[] = "chara.data";

/* 変数初期化 */
GameInfo gGame = {
    GS_Playing,
    { SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE },
    30,
    0,
    0,
    0,
    0.0,
    MSG_None,
    { 0, 0, WD_Width *MAP_ChipSize, WD_Height *MAP_ChipSize },
    { 0, 0, WD_Width *MAP_ChipSize, WD_Height *MAP_ChipSize },
    NULL
};
CharaImgInfo gCharaImg[CT_NUM] = {
    { 48, 48, 0, 0, { 0, 0, 0, 0 }, NULL },
    { 24, 24, 0, 0, { 0, 0, 0, 0 }, NULL },
    { 48, 48, 0, 0, { 0, 0, 0, 0 }, NULL },
    { 96, 96, 0, 0, { 0, 0, 0, 0 }, NULL },
    { 96, 96, 0, 0, { 0, 0, 0, 0 }, NULL },
    { 48, 48, 0, 0, { 0, 0, 0, 0 }, NULL }
};
CharaInfo *gChara;
int gCharaNum;
static const float gGravity = 980; /* 重力加速度 */

/* 関数 */
static SDL_bool IsOutsideWindowLR(CharaInfo *ch, FloatPoint *point, SDL_bool adjust);
static SDL_bool AdjustOverlapBlock(CharaInfo *ch, FloatPoint *point, SDL_Point *adjusted);
static CharaDir GetDirection(CharaInfo *src, CharaInfo *dst);

/* ゲームシステム初期化
 *
 * 返値
 *   正常終了: 0
 *   エラー  : 負数
 */
int InitSystem(void)
{
    int ret = 0;

    /* 乱数初期化 */
    srand(time(NULL));

    /* キャラクター情報初期化 */
    /* ファイルオープン */
    FILE *fp = fopen(gCharaDataFile, "r");
    if (fp == NULL) {
        return PrintError("failed to open data file.");
    }
    /* キャラ総数読込 */
    if (1 != fscanf(fp, "%d", &gCharaNum)) {
        ret = PrintError("failed to read the number of chara data.");
        goto CLOSEFILE;
    }
    /* 領域確保 */
    gChara = (CharaInfo *)malloc(sizeof(CharaInfo) * gCharaNum);
    if (gChara == NULL) {
        ret = PrintError("failed to allocate memory.");
        goto CLOSEFILE;
    }
    /* キャラ情報読込 */
    for (int i = 0; i < gCharaNum; i++) {
        /* タイプ，HP */
        int type;
        if (2 != fscanf(fp, "%d%d", &type, &(gChara[i].hp))) {
            ret = PrintError("failed to read the chara data 1.");
            break;
        }
        gChara[i].type = type;
        /* キャラの速度（x，y方向） */
        if (2 != fscanf(fp, "%f%f", &(gChara[i].basevel.x), &(gChara[i].basevel.y))) {
            ret = PrintError("failed to read the chara data 2.");
            break;
        }

        /* 初期値の設定 */
        switch (gChara[i].type) {
        case CT_Player:
            gGame.player = i;
            break;
        case CT_Boss:
            gGame.boss = i;
            break;
        case CT_Station:
            gGame.restStations++;
            break;
        default:
            break;
        }
    }

CLOSEFILE:
    fclose(fp);

    return ret;
}

/* システム終了処理 */
void DestroySystem(void)
{
    free(gChara);
    return;
}

/* 初期位置の設定
 * 指定タイプのキャラクターに初期位置を設定する
 * 複数ある場合は，非表示で位置設定されていない(0,0)
 * ものを探して設定する
 * 
 * 引数
 *   ct: タイプ
 *   r : 初期位置
 */
void SetInitPoint(CharaType ct, SDL_Rect r)
{
    for (int i = 0; i < gCharaNum; i++) {
        if (gChara[i].type == ct && gChara[i].stts == CS_Disable) {
            if (!gChara[i].rect.x && !gChara[i].rect.y) {
                gChara[i].stts    = CS_Enable;
                gChara[i].point.x = gChara[i].rect.x = r.x;
                gChara[i].point.y = gChara[i].rect.y = r.y;
            }
            break;
        }
    }
}

/* 画像のマスクから当たり判定用矩形の算出
 * マスクは一つの矩形であることを前提とする
 * 1ピクセル32ビットを想定
 * 
 * 引数
 *   s: マスクのサーフェイス
 *   r: 検出範囲
 *
 * 返値: 算出した矩形
 */
SDL_Rect MeasureMask(SDL_Surface *s, SDL_Rect r)
{
    SDL_Rect ret  = { 0 };
    SDL_Rect ret2 = { 0 };
    /* 検出範囲がマップの高さを超えるときは，
		はみ出た範囲は調べない */
    if (r.y < 0) {
        r.h += r.y;
        r.y = 0;
    } else if (r.y + r.h > MAP_Height * MAP_ChipSize) {
        return ret;
    }
    /* 検出範囲がマップの幅を超えるときは，
		超えた範囲を(0,y)から調査する
		(マップ左右はつながっているので) */
    if (r.x + r.w > MAP_Width * MAP_ChipSize) {
        SDL_Rect r2 = r;
        r2.x        = 0;
        r2.w        = r.w - (MAP_Width * MAP_ChipSize - r.x);
        ret2        = MeasureMask(s, r2);
        r.w         = MAP_Width * MAP_ChipSize - r.x;
        ret2.x += MAP_Width * MAP_ChipSize;
    }

    /* ピクセルデータの(r.x,r.y)の位置まで移動
		データはOpenCVの画素と同様に保存されている
		（デバイスプログラミング資料p.7参照）
		ただし1ピクセルは32bit */
    Uint32 *p = (Uint32 *)(s->pixels) + s->w * r.y + r.x; //

    /* (0,0)から走査して，最初に色のある点を(x,y)とする */
    for (ret.y = 0; ret.y < r.h; ret.y++, p += s->w) {
        for (ret.x = 0; ret.x < r.w; ret.x++) {
            if (p[ret.x] & s->format->Amask) {
                /* (x,y)からx方向に走査して，色のなくなる点までをwとする */
                for (ret.w = 0; ret.x + ret.w < r.w; ret.w++) {
                    if (!(p[ret.x + ret.w] & s->format->Amask)) //色がなくなったら
                        break;
                }
                /* (x,y)からy方向に走査して，色のなくなる点までをhとする */
                for (ret.h = 0; ret.y + ret.h < r.h; ret.h++, p += s->w) {
                    if (!(p[ret.x] & s->format->Amask))
                        break;
                }
                if (ret2.w && ret2.h) {
                    ret.w += ret2.w;
                }
                return ret;
            }
        }
    }
    if (ret2.w && ret2.h) {
        ret = ret2;
    }
    return ret;
}

/* x座標のマップ範囲への補正 */
int AdjustXrange(int x)
{
    while (x < 0) {
        x += MAP_Width * MAP_ChipSize;
    }
    while (x >= MAP_Width * MAP_ChipSize) {
        x -= MAP_Width * MAP_ChipSize;
    }
    return x;
    //return x % (MAP_Width * MAP_ChipSize);
}

/* x座標のマップ範囲への補正float版 */
float AdjustXrangeF(float x)
{
    while (x < 0) {
        x += MAP_Width * MAP_ChipSize;
    }
    while (x >= MAP_Width * MAP_ChipSize) {
        x -= MAP_Width * MAP_ChipSize;
    }
    return x;
}

/* srcから見たdstの方向を返す */
CharaDir GetDirection(CharaInfo *src, CharaInfo *dst)
{
    int diff = src->rect.x - dst->rect.x;
    if ((0 < diff && diff < MAP_Width * MAP_ChipSize / 2)
        || (diff < 0 && -diff > MAP_Width * MAP_ChipSize / 2)) {
        return CD_Left;
    } else {
        return CD_Right;
    }
}

/* 床・壁との重なりを補正する 
 *
 * 引数
 *   ch: キャラ
 *   point: 対象座標，補正後座標
 *   adjusted: 補正した値
 * 
 * 返値: 補正したときSDL_TRUE
 */
SDL_bool AdjustOverlapBlock(CharaInfo *ch, FloatPoint *point, SDL_Point *adjusted)
{
    SDL_bool ret  = SDL_FALSE;
    SDL_Rect mask = ch->img->mask;
    /* x方向の重なりの補正 */
    /* マスクのマップ上の位置を計算し， */
    mask.x += point->x;
    mask.y += ch->point.y;
    /* その位置の床・壁のマスクを取得 */
    SDL_Rect rr = MeasureMask(gGame.mapMask, mask);
    /* マスクがあるときは重なっていることになるので補正する */
    if (rr.w && rr.h) {
        ret = SDL_TRUE;
        if (rr.x) {
            point->x -= rr.w;
            adjusted->x = -rr.w;
        } else {
            point->x += rr.w;
            adjusted->x = rr.w;
        }
    }
    /* y方向の重なりの補正 */
    mask = ch->img->mask;
    mask.x += point->x;
    mask.y += point->y;
    rr = MeasureMask(gGame.mapMask, mask);
    if (rr.w && rr.h) {
        ret = SDL_TRUE;
        if (rr.y) {
            point->y -= rr.h;
            adjusted->y = -rr.h;
        } else {
            point->y += rr.h;
            adjusted->y = rr.h;
        }
    }
    return ret;
}

/* 画面外に出たか調べる（左右のみ）
 *
 * 引数
 *   ch: キャラ
 *   point: 対象座標，補正後座標
 *   adjust: 補正するか（する場合は画面内に収まるように）
 *
 * 返値: 画面外に出たらSDL_TRUE
 */
SDL_bool IsOutsideWindowLR(CharaInfo *ch, FloatPoint *point, SDL_bool adjust)
{
    SDL_bool ret = SDL_FALSE;
    SDL_Rect r   = ch->img->mask;
    /* 補正しない時は画面から出た時に判断するので，
		画像の幅も考慮する */
    int dw = adjust ? 0 : r.w;
    /* 画像端付近の調整 */
    r.x += point->x;
    if (gGame.rectMap.x + gGame.rectMap.w > MAP_Width * MAP_ChipSize) {
        if (r.x < (gGame.rectMap.x + gGame.rectMap.w - MAP_Width * MAP_ChipSize)) {
            r.x += MAP_Width * MAP_ChipSize;
        }
    }
    /* 左端の調査 */
    if (r.x < (gGame.rectMap.x - dw)) {
        if (adjust)
            point->x += gGame.rectMap.x - r.x;
        ret = SDL_TRUE;
    } else {
        /* 右端の調査 */
        r.x += r.w;
        if (r.x > (gGame.rectMap.x + gGame.rectMap.w + dw)) {
            if (adjust)
                point->x -= r.x - (gGame.rectMap.x + gGame.rectMap.w);
            ret = SDL_TRUE;
        }
    }

    return ret;
}

/* 入力状態から方向の設定 */
void SetInput(void)
{
    gChara[gGame.player].velocity.x = 0.0;
    if (gGame.input.left && !gGame.input.right) {
        gChara[gGame.player].dir        = CD_Left;
        gChara[gGame.player].velocity.x = gChara[gGame.player].basevel.x;
    }
    if (gGame.input.right && !gGame.input.left) {
        gChara[gGame.player].dir        = CD_Right;
        gChara[gGame.player].velocity.x = gChara[gGame.player].basevel.x;
    }
}

/* キャラの状態更新 */
void UpdateCharaStatus(CharaInfo *ch)
{
    switch (ch->stts) {
    case CS_Disable:
        switch (ch->type) {
        case CT_Player:
            gGame.stts = GS_End;
            gGame.msg  = MSG_GameOver;
            break;
        case CT_Enemy:
            if (gGame.stts == GS_Playing) {
                /* 敵を出現させる */
                ch->stts       = CS_Enable;
                ch->point.y    = -(ch->rect.h);
                ch->point.x    = rand() % (MAP_Width * MAP_ChipSize);
                ch->velocity.x = ch->basevel.x;
                ch->velocity.y = 0.0;
                ch->hp         = 1;
            }
            break;
        case CT_Boss:
            if (ch->hp <= 0) {
                gGame.stts = GS_End;
                /* 体当たりで倒したときはGameOverにする */
                gGame.msg = (gChara[gGame.player].stts == CS_Disable) ? MSG_GameOver : MSG_Clear;
            } else if (gGame.stts == GS_Boss) {
                ch->stts    = CS_Enable;
                ch->dir     = CD_Left;
                ch->point.x = gGame.rectMap.x + gGame.rectMap.w - ch->rect.w * 2;
                ch->point.y = -ch->rect.h;
            }
            break;
        case CT_Ball:
            if (gGame.input.button1) {
                ch->rect.x   = gChara[gGame.player].rect.x;
                ch->rect.y   = gChara[gGame.player].rect.y;
                ch->point    = gChara[gGame.player].point;
                ch->dir      = gChara[gGame.player].dir;
                ch->stts     = CS_Enable;
                ch->hp       = 1;
                ch->velocity = ch->basevel;
            }
            break;
        case CT_BossBall:
            if (gChara[gGame.boss].stts == CS_Boss2) {
                ch->rect.x   = gChara[gGame.boss].rect.x;
                ch->rect.y   = gChara[gGame.boss].rect.y;
                ch->point    = gChara[gGame.boss].point;
                ch->dir      = gChara[gGame.boss].dir;
                ch->stts     = CS_Enable;
                ch->velocity = ch->basevel;
            }
            break;
        default:
            break;
        }
        break;
    case CS_Boss1:
    case CS_Boss2:
        if (ch->hp % 2) {
            ch->stts = CS_Boss2;
        } else {
            ch->stts = CS_Boss1;
        }
    case CS_Enable:
        switch (ch->type) {
        case CT_Boss:
            /* 画面下に落ちたら上から出てくる */
            if (ch->point.y >= MAP_Height * MAP_ChipSize) {
                ch->point.y = -ch->rect.h;
            }
            break;
        case CT_Enemy:
            if (gGame.stts == GS_Boss) {
                ch->stts = CS_Disable;
            }
        default:
            /* 画面下に落ちたら消す */
            if (ch->point.y >= MAP_Height * MAP_ChipSize) {
                ch->stts = CS_Disable;
            }
            break;
        }
        break;
    }
}

/* キャラの移動 */
void MoveChara(CharaInfo *ch)
{
    /* 非表示キャラは計算しない */
    if (ch->stts == CS_Disable) {
        return;
    }
    /* 拠点は移動させない */
    if (ch->type == CT_Station) {
        return;
    }
    FloatPoint newpoint = ch->point;
    float newvely       = ch->velocity.y;
    /* x方向の移動(等速運動 x=vt) */
    newpoint.x += ch->dir * ch->velocity.x * gGame.timeStep;

    /* y方向の移動(投げ上げ運動 v=v0-gt, y = v0t - 1/2 gt^2) */
    newvely = ch->velocity.y + gGravity * gGame.timeStep;
    newpoint.y += ch->velocity.y * gGame.timeStep + 1.0 / 2.0 * gGravity * gGame.timeStep * gGame.timeStep;

    /* 床・壁との重なり調整 */
    SDL_Point adjusted = { 0 };
    if (AdjustOverlapBlock(ch, &newpoint, &adjusted)) {
        switch (ch->type) {
        case CT_Boss:
            if (ch->stts == CS_Enable && adjusted.y < 0) {
                ch->stts = CS_Boss1;
            }
        case CT_Enemy:
            if (adjusted.y < 0) {
                /* 床についたときにプレイヤー方向に向く */
                ch->dir = GetDirection(ch, &(gChara[gGame.player]));
                /* 警告時雑魚は逃げる */
                if (gGame.stts == GS_Warning) {
                    ch->dir = -ch->dir;
                }
                /* ランダムの速度でジャンプ */
                newvely = ch->basevel.y * (rand() % 10);
            }
            break;
        case CT_Player:
            /* 床についたときにジャンプ */
            if (adjusted.y < 0) {
                newvely = (gGame.input.up) ? ch->basevel.y : 0.0;
            }
            break;
        case CT_Ball:
        case CT_BossBall:
            ch->stts = CS_Disable;
        default:
            break;
        }
    }

    /* 球は画面から外れたら消える */
    if ((ch->type == CT_Ball || ch->type == CT_BossBall)
        && IsOutsideWindowLR(ch, &newpoint, SDL_FALSE)) {
        ch->stts = CS_Disable;
    }
    if (ch->type == CT_Ball) {
        ch->velocity.x = ch->basevel.x;
        ch->velocity.y = ch->basevel.y;
    }
    ch->point      = newpoint;
    ch->velocity.y = newvely;
    ch->rect.x     = newpoint.x;
    ch->rect.y     = newpoint.y;
}

/* 当たり判定 */
void Collision(CharaInfo *ci, CharaInfo *cj)
{
    /* 判定が不要な組み合わせを除外 */
    if (ci->stts == CS_Disable || cj->stts == CS_Disable)
        return;
    if (ci->type == CT_Player && cj->type == CT_Ball)
        return;
    if (cj->type == CT_Player && ci->type == CT_Ball)
        return;
    if (ci->type != CT_Player && ci->type != CT_Ball
        && cj->type != CT_Player && cj->type != CT_Ball)
        return;

    /* マスクをマップ座標に合わせる */
    SDL_Rect mi = ci->img->mask;
    mi.x += ci->rect.x;
    mi.y += ci->rect.y;
    SDL_Rect mj = cj->img->mask;
    mj.x += cj->rect.x;
    mj.y += cj->rect.y;
    /* 当たった */
    if (SDL_HasIntersection(&mi, &mj)) {
        /* HPを減らし，0以下なら消す */
        int n = (ci->hp < cj->hp) ? ci->hp : cj->hp;
        ci->hp -= n;
        if (ci->hp <= 0) {
            ci->stts = CS_Disable;
            if (ci->type == CT_Station) {
                gGame.restStations--;
            }
        }
        cj->hp -= n;
        if (cj->hp <= 0) {
            cj->stts = CS_Disable;
            if (cj->type == CT_Station) {
                gGame.restStations--;
            }
        }
        if (gGame.restStations == 0) {
            gGame.stts = GS_Warning;
            gGame.msg  = MSG_Warning;
        }
    }
}

/* end of system.c */
