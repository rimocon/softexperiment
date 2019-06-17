/*
 * ファイル名	: window.c
 * 機能		: ユーザーインターフェース処理
 */
#include "system.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/* データファイルパス */
static char gMapDataFile[] = "map.data";

/* 画像ファイルパス */
static char gMapImgFile[]          = "map.png";
static char gBgImgFile[]           = "bg.png";
static char *gCharaImgFile[CT_NUM] = { "player.png", "ball.png", "enemy.png", "station.png", "enemy.png", "ball.png" };

/* フォント関連 */
static char *gFontFile[] = {
    "/usr/share/fonts/truetype/freefont/FreeSerifBoldItalic.ttf",
    "/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf"
};
static char *gMsgStr[MSG_NUM] = { " ", "GameOver", "Clear!!", "Warning!" };

static const SDL_Color gWhite = { 255, 255, 255, 255 };
static const SDL_Color gBlack = { 0, 0, 0, 255 };
static const SDL_Color gRed   = { 255, 0, 0, 255 };

/* 関数 */
static void PlaceChara(void);
static int MakeMap(void);
static int MakeMessage(void);

/* メインウインドウの表示，設定
 *
 * 返値
 *   正常終了: 0
 *   エラー  : 負数
 */
int InitWindow(void)
{
    /* image初期化 */
    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
        return PrintError("failed to initialize SDL_Image");
    }

    /* メインのウインドウ(表示画面)の作成 */
    gGame.window = SDL_CreateWindow(
        "act", 80, 50,
        WD_Width * MAP_ChipSize, WD_Height * MAP_ChipSize, 0);
    if (gGame.window == NULL)
        return PrintError(SDL_GetError());

    gGame.render = SDL_CreateRenderer(gGame.window, -1, 0);
    if (gGame.render == NULL)
        return PrintError(SDL_GetError());

    /* 画像の読み込み */
    /* 背景 */
    SDL_Surface *s = IMG_Load(gBgImgFile);
    if (s == NULL)
        return PrintError("failed to open background image.");
    gGame.bg = SDL_CreateTextureFromSurface(gGame.render, s);
    SDL_FreeSurface(s);
    if (gGame.bg == NULL) {
        return PrintError(SDL_GetError());
    }
    /* キャラ */
    /* 画像は，横にアニメーションパターン，
        下に「向き」など，状態ごとの画像， 
        最下がマスクパターンであると想定 */
    int i;
    for (i = 0; i < CT_NUM; i++) {
        if (NULL == (s = IMG_Load(gCharaImgFile[i]))) {
            return PrintError("failed to open character image.");
        }
        gCharaImg[i].imgW  = s->w;
        gCharaImg[i].imgH  = s->h;
        gCharaImg[i].image = SDL_CreateTextureFromSurface(gGame.render, s);
        /* 画像のマスクから当たり判定用矩形の算出 */
        SDL_Rect r = { 0, s->h - gCharaImg[i].h, gCharaImg[i].w, gCharaImg[i].h };
        /* ボス(と球)は敵画像を縦横2倍に拡大表示するので
           マスクは元のサイズで算出する */
        if (i == CT_Boss || i == CT_BossBall) {
            r.w /= 2;
            r.h /= 2;
            r.y = s->h - r.h;
        }
        /* マスク算出 */
        gCharaImg[i].mask = MeasureMask(s, r);
        if (i == CT_Boss || i == CT_BossBall) {
            r.w *= 2;
            r.h *= 2;
        }
        /* サーフェイス解放(テクスチャに転送後はゲーム中に使わないので) */
        SDL_FreeSurface(s);
        if (gCharaImg[i].image == NULL) {
            return PrintError(SDL_GetError());
        }
    }

    /* キャラ情報設定 */
    for (i = 0; i < gCharaNum; i++) {
        gChara[i].stts    = CS_Disable;
        gChara[i].dir     = CD_Right;
        gChara[i].point.x = gChara[i].point.y = 0.0;
        gChara[i].velocity.x = gChara[i].velocity.y = 0.0;
        gChara[i].rect.x = gChara[i].rect.y = 0;
        gChara[i].rect.w                    = gCharaImg[gChara[i].type].w;
        gChara[i].rect.h                    = gCharaImg[gChara[i].type].h;
        gChara[i].src                       = gChara[i].rect;
        gChara[i].img                       = &(gCharaImg[gChara[i].type]);
        if (gChara[i].type == CT_Boss || gChara[i].type == CT_BossBall) {
            gChara[i].src.h = gChara[i].rect.h / 2;
            gChara[i].src.w = gChara[i].rect.w / 2;
        }
    }

    /* メッセージ作成 */
    if (MakeMessage())
        return -1;

    /* マップの作成 */
    if (MakeMap())
        return -1;

    /* ウインドウへの描画 */
    RenderWindow();

    /* image利用終了(テクスチャに転送後はゲーム中に使わないので) */
    IMG_Quit();

    return 0;
}

/* ウインドウの終了処理 */
void DestroyWindow(void)
{
    /* テクスチャなど */
    int i;
    for (i = 0; i < CT_NUM; i++)
        SDL_DestroyTexture(gCharaImg[i].image);
    for (i = 0; i < MSG_NUM; i++)
        SDL_DestroyTexture(gGame.msgTexts[i]);
    SDL_DestroyTexture(gGame.map);
    SDL_DestroyTexture(gGame.bg);
    SDL_FreeSurface(gGame.mapMask);
    SDL_DestroyRenderer(gGame.render);
    SDL_DestroyWindow(gGame.window);
}

/* メッセージ作成
 *
 * 返値
 *   正常終了: 0
 *   エラー  : 負数
 */
int MakeMessage(void)
{
    int ret = 0;
    /* フォントからメッセージテクスチャ作成 */
    /* 初期化 */
    if (TTF_Init() < 0) {
        return PrintError(TTF_GetError());
    }
    /* フォントを開く */
    TTF_Font *ttf[2];
    if (NULL == (ttf[0] = TTF_OpenFont(gFontFile[0], 88))) {
        ret = PrintError(TTF_GetError());
    }
    if (NULL == (ttf[1] = TTF_OpenFont(gFontFile[1], 120))) {
        ret = PrintError(TTF_GetError());
    }
    /* メッセージ作成 */
    int nos[MSG_NUM]        = { 0, 0, 0, 1 };
    SDL_Color cols[MSG_NUM] = { gBlack, gBlack, gWhite, gRed };
    for (int i = 0; i < MSG_NUM && ttf[nos[i]]; i++) {
        SDL_Surface *sf;
        if (NULL == (sf = TTF_RenderUTF8_Blended(ttf[nos[i]], gMsgStr[i], cols[i]))) {
            ret = PrintError(TTF_GetError());
        } else {
            /* テクスチャへ */
            if (NULL == (gGame.msgTexts[i] = SDL_CreateTextureFromSurface(gGame.render, sf))) {
                ret = PrintError(SDL_GetError());
            }
            SDL_FreeSurface(sf);
        }
    }

    /* フォントを閉じる */
    TTF_CloseFont(ttf[1]);
    TTF_CloseFont(ttf[0]);
    /* フォント利用終了(テクスチャに転送後はゲーム中に使わないので) */
    TTF_Quit();

    return ret;
}

/* マップデータを基に背景作成
 *
 * 返値
 *   正常終了: 0
 *   エラー  : 負数
 */
int MakeMap(void)
{
    int ret = 0;
    /* 画像読み込み */
    SDL_Surface *img = IMG_Load(gMapImgFile);
    if (img == NULL)
        return PrintError(IMG_GetError());

    /* 画像フォーマットを元にマップのサーフェイス生成 */
    SDL_Surface *map = SDL_CreateRGBSurfaceWithFormat(0, MAP_Width * MAP_ChipSize, MAP_Height * MAP_ChipSize, 32, img->format->format);
    if (map == NULL) {
        ret = PrintError(SDL_GetError());
    }
    /* 重なり判定用マスクサーフェイス作成 */
    gGame.mapMask = SDL_CreateRGBSurfaceWithFormat(0, MAP_Width * MAP_ChipSize, MAP_Height * MAP_ChipSize, 32, img->format->format);
    if (gGame.mapMask == NULL) {
        ret = PrintError(SDL_GetError());
    }

    /* マップの読み込みと配置 */
    /* ファイルオープン */
    FILE *fp = fopen(gMapDataFile, "r");
    if (fp == NULL) {
        ret = PrintError("failed to open map data.");
    } else if (map && gGame.mapMask) {
        int i, j;
        MapType mt   = MT_None;
        SDL_Rect src = { 0, 0, MAP_ChipSize, MAP_ChipSize };
        SDL_Rect dst = { 0 };
        for (j = 0; j < MAP_Height; j++, dst.y += MAP_ChipSize) {
            dst.x = 0;
            for (i = 0; i < MAP_Width; i++, dst.x += MAP_ChipSize) {
                /* 値の読込(意味はMapTypeに準ずる) */
                if (fscanf(fp, "%u", &mt) != 1) {
                    ret = PrintError("failed to load map data.");
                }

                if (mt == MT_Station) {
                    SetInitPoint(CT_Station, dst);
                    mt = MT_None;
                } else if (mt == MT_Player) {
                    SetInitPoint(CT_Player, dst);
                    mt = MT_None;
                }

                src.x = mt * MAP_ChipSize;
                src.y = 0;
                if (0 > SDL_BlitSurface(img, &src, map, &dst)) {
                    ret = PrintError(SDL_GetError());
                }

                src.y = MAP_ChipSize;
                if (0 > SDL_BlitSurface(img, &src, gGame.mapMask, &dst)) {
                    ret = PrintError(SDL_GetError());
                }
            }
        }
        /* マップはテクスチャに */
        if (NULL == (gGame.map = SDL_CreateTextureFromSurface(gGame.render, map))) {
            ret = PrintError(SDL_GetError());
        }
    }
    /* ファイルを閉じる */
    fclose(fp);
    /* サーフェイス解放(テクスチャに転送後はゲーム中に使わないので) */
    SDL_FreeSurface(map);
    SDL_FreeSurface(img);

    return ret;
}

/* キャラの配置 */
void PlaceChara(void)
{
    /* キャラのコピー */
    int i;
    for (i = 0; i < gCharaNum; i++) {
        if (gChara[i].stts != CS_Disable) {
            SDL_Rect dst = gChara[i].rect;
            /* 表示画面の左端がgGame.rectMap.xなので
               これを引くことで画面上の相対座標に変換 */
            dst.x -= gGame.rectMap.x;
            dst.x = AdjustXrange(dst.x);
            if (0 > SDL_RenderCopy(gGame.render, gChara[i].img->image, &(gChara[i].src), &dst)) {
                PrintError(SDL_GetError());
            }
        }
    }
}

/* ウインドウ描画
 *
 * 返値
 *   正常終了: 0
 *   エラー  : 負数
 */
int RenderWindow(void)
{
    /* メインウインドウへの転送 */
    int bgw = 0;
    int ret = SDL_QueryTexture(gGame.bg, NULL, NULL, &bgw, NULL);
    if (ret < 0) {
        PrintError(SDL_GetError());
    }
    /* 背景はマップの半分の移動量でスクロールする */
    if (gGame.stts != GS_Boss && gGame.stts != GS_End) {
        gGame.rectBg.x = (gChara[gGame.player].rect.x - (WD_Width * MAP_ChipSize - gChara[gGame.player].rect.w) / 2) / 2;
        if (gGame.rectBg.x < 0) {
            gGame.rectBg.x += bgw;
        }
    }
    SDL_Rect src = gGame.rectBg;
    SDL_Rect dst = gGame.rectBg;
    if (src.x + src.w >= bgw) {
        dst.x = bgw - src.x;
        src.x = 0;
        src.w = dst.w = src.w - dst.x;

        ret = SDL_RenderCopy(gGame.render, gGame.bg, &src, &dst);
        if (ret < 0) {
            PrintError(SDL_GetError());
        }
        src.x = gGame.rectBg.x;
        src.w = dst.w = dst.x;
    }
    dst.x = 0;
    ret   = SDL_RenderCopy(gGame.render, gGame.bg, &src, &dst);
    if (ret < 0) {
        PrintError(SDL_GetError());
    }

    /* マップ */
    if (gGame.stts != GS_Boss && gGame.stts != GS_End) {
        gGame.rectMap.x = AdjustXrange(gChara[gGame.player].rect.x - (WD_Width * MAP_ChipSize - gChara[gGame.player].rect.w) / 2);
    }
    src = dst = gGame.rectMap;
    if (src.x + src.w >= MAP_Width * MAP_ChipSize) {
        dst.x = MAP_Width * MAP_ChipSize - src.x;
        src.x = 0;
        src.w = dst.w = src.w - dst.x;

        ret = SDL_RenderCopy(gGame.render, gGame.map, &src, &dst);
        if (ret < 0) {
            PrintError(SDL_GetError());
        }
        src.x = gGame.rectMap.x;
        src.w = dst.w = dst.x;
    }
    dst.x = 0;
    ret   = SDL_RenderCopy(gGame.render, gGame.map, &src, &dst);
    if (ret < 0) {
        PrintError(SDL_GetError());
    }

    /* キャラ */
    PlaceChara();

    /* メッセージ */
    src.x = src.y = 0;

    ret = SDL_QueryTexture(gGame.msgTexts[gGame.msg], NULL, NULL, &src.w, &src.h);
    if (ret < 0) {
        PrintError(SDL_GetError());
    }

    dst.x = (WD_Width * MAP_ChipSize - src.w) / 2,
    dst.y = (WD_Height * MAP_ChipSize - src.h) / 2;
    dst.w = src.w;
    dst.h = src.h;

    ret = SDL_RenderCopy(gGame.render, gGame.msgTexts[gGame.msg], &src,
        &dst);
    if (ret < 0) {
        PrintError(SDL_GetError());
    }

    /* 描画更新 */
    SDL_RenderPresent(gGame.render);

    return ret;
}

/* end of window.c */
