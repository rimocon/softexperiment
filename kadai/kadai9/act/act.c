/*
 * ファイル名： act.c
 * 機能　　　： メインルーチン
 */
#include "system.h"
#include <stdio.h>

/* 関数 */
static Uint32 AniTimer(Uint32 interval, void* param);
static int InputEvent(void* data);

/* main */
int main(int argc, char* argv[])
{
    /* 初期化処理 */
    /* SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        return PrintError(SDL_GetError());
    }
    /* ゲームシステム */
    if (InitSystem() < 0) {
        PrintError("failed to initialize System");
        goto DESTROYSYSTEM;
    }
    /* ウインドウ */
    if (InitWindow() < 0) {
        PrintError("failed to initialize Windows");
        goto DESTROYALL;
    }
    /* スレッドセーフな整数変数を扱う 
     * スレッドの生存確認(0以下でスレッドを終了させる)と
     * 簡易フレームカウンタとして利用 */
    SDL_atomic_t atm;
    SDL_AtomicSet(&atm, 1);
    /* スレッド */
    SDL_Thread* thread = SDL_CreateThread(InputEvent, "InputEvent", &atm);
    if (thread == NULL) {
        PrintError(SDL_GetError());
        goto DESTROYALL;
    }
    SDL_DetachThread(thread);
    /* タイマー */
    SDL_TimerID timer = SDL_AddTimer(100, AniTimer, &atm);
    if (timer == 0) {
        PrintError(SDL_GetError());
        goto RELEASETHREAD;
    }

    /* メインループ */
    while (SDL_AtomicGet(&atm) > 0) {
        /* 終了時は何もしない */
        if (gGame.stts) {
            SetInput();
            for (int i = 0; i < gCharaNum; i++) {
                UpdateCharaStatus(&(gChara[i]));
                MoveChara(&(gChara[i]));
            }
            for (int i = 0; i < gCharaNum; i++) {
                for (int j = i + 1; j < gCharaNum; j++)
                    Collision(&(gChara[i]), &(gChara[j]));
            }
        }
        RenderWindow();

        /* 少し待つ
            PC環境によって遅く(速く)なる時に調整してください
         */
        SDL_Delay(10);
        /* フレームカウント */
        SDL_AtomicIncRef(&atm);
    }

    /* 終了処理 */
    SDL_RemoveTimer(timer);
RELEASETHREAD:
    SDL_AtomicSet(&atm, -10);
DESTROYALL:
    DestroyWindow();
DESTROYSYSTEM:
    DestroySystem();
    SDL_Quit();

    return 0;
}

/* エラーメッセージ表示
 *
 * 引数
 *   str: エラーメッセージ
 *
 * 返値: -1
 */
int PrintError(const char* str)
{
    fprintf(stderr, "%s\n", str);
    return -1;
}

/* タイマー処理2(アニメーションの更新) */
Uint32 AniTimer(Uint32 interval, void* param)
{
    /* 待ち時間の更新 */
    if (gGame.stts == GS_Warning) {
        gGame.restTime--;
        if (gGame.restTime < 0) {
            gGame.stts = GS_Boss;
        }
    }

    /* 時間増分の更新 */
    if (SDL_AtomicGet((SDL_atomic_t*)param) > 0) {
        gGame.timeStep = 0.1 / SDL_AtomicGet((SDL_atomic_t*)param);
        printf("FPS: %d\r", SDL_AtomicGet((SDL_atomic_t*)param) * 10);
        SDL_AtomicSet((SDL_atomic_t*)param, 1);
    }

    /* 転送元範囲の更新(アニメーション) */
    for (int i = 0; i < gCharaNum; i++) {
        /* アニメーションパターンの更新 */
        gChara[i].src.x = (gChara[i].src.x + gChara[i].src.w) % gChara[i].img->imgW;

        /* 状態別画像の更新 */
        gChara[i].src.y = 0;
        switch (gChara[i].type) {
        case CT_Station:
            gChara[i].src.y += (gChara[i].hp > 1) ? 0 : gChara[i].src.h;
            break;
        case CT_Boss:
            gChara[i].src.y += (gChara[i].hp > 1) ? 0 : gChara[i].src.h * 2;
        case CT_Player:
        case CT_Enemy:
            gChara[i].src.y += (gChara[i].dir == CD_Right) ? 0 : gChara[i].src.h;
        default:
            break;
        }
    }
    return interval;
}

/* 入力イベント処理
 *
 * 返値
 *   終了: 0
 */
int InputEvent(void* data)
{
    SDL_Event event;
    while (SDL_AtomicGet((SDL_atomic_t*)data) > 0) {
        /* イベント取得を行わないと，
         * OSが，プロセス停止中と判断し，
         * 画面をグレーにしてしまうので，
         * イベントを使わなくてもイベント取得は行う
         */
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                SDL_AtomicSet((SDL_atomic_t*)data, -10);
                break;
            case SDL_KEYDOWN:
                if (event.key.repeat)
                    break;
                /* キーが押された方向を設定 */
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    SDL_AtomicSet((SDL_atomic_t*)data, -10);
                    break;
                case SDLK_UP:
                    gGame.input.up = SDL_TRUE;
                    break;
                case SDLK_RIGHT:
                    gGame.input.right = SDL_TRUE;
                    break;
                case SDLK_LEFT:
                    gGame.input.left = SDL_TRUE;
                    break;
                case SDLK_SPACE:
                    gGame.input.button1 = SDL_TRUE;
                    break;
                default:
                    break;
                }
                break;
            case SDL_KEYUP:
                if (event.key.repeat)
                    break;
                /* 離されたときは解除 */
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    gGame.input.up = SDL_FALSE;
                    break;
                case SDLK_RIGHT:
                    gGame.input.right = SDL_FALSE;
                    break;
                case SDLK_LEFT:
                    gGame.input.left = SDL_FALSE;
                    break;
                case SDLK_SPACE:
                    gGame.input.button1 = SDL_FALSE;
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
    }
    return 0;
}

/* end of act.c */
