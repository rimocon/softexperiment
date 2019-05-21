/*************************************************
sdl2_window2.c
Programmed by H. Mitsuhara (April, 2018)

コンパイルオプション（リンク対象）：-lSDL2
動作：
　ウィンドウが表示され，一定時間が経過するとウィンドウの設定が変化・反映される
　メッセージボックスが表示される（ボタンをクリックする必要あり）
備考：
*************************************************/

#include <stdio.h>
#include <SDL2/SDL.h> // SDL2を用いるために必要なヘッダファイルをインクルード

// メイン関数（コマンドライン引数を受け取る）
int main(int argc, char *argv[]){
    SDL_Window* window; // ウィンドウのデータを格納する構造体   
    SDL_Renderer* renderer; // レンダリングコンテキスト（描画設定）を格納する構造体
    SDL_Rect rect; // 矩形領域データ（左上頂点のx,y座標，幅，高さ）を格納する構造体
    int i,j;
    
    // SDL初期化（初期化失敗の場合は終了）
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL.\n");
        exit(-1);
    }

    // ウィンドウ生成・表示＋レンダリングコンテキストの生成
    if(SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)){ // windowにrendererが対応づけられる
        // 生成失敗の場合
        printf("Failed to create window and renderer.\n");
        exit(-1);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // 生成したRCに描画色として白を設定
    SDL_RenderClear(renderer); // 生成したRCを白でクリア＝塗りつぶす（ただし，メモリに描画データを反映させただけなので，画面には表示されない）
    SDL_RenderPresent(renderer); // 描画データを表示

    // ディスプレイ関連
    SDL_DisplayMode request, closest, current; // ディスプレイモードを格納する構造体
    request.w = 1500; // 要求するディスプレイモード（DM）の解像度（幅）
    request.h = 1000; // 要求するDMの解像度（高さ）
    request.refresh_rate = 60; // ディスプレイ更新周期（Hz）
    request.driverdata   = 0; // ドライバ固有のデータ（0で初期化）
    printf("Requested: (%d, %d) px @ %d Hz\n", request.w, request.h, request.refresh_rate);

    if (SDL_GetClosestDisplayMode(0, &request, &closest) == NULL){ // 要求に近いDMを取得
        // 取得失敗
        printf("Not found\n\n");
    }
    else{
        // 取得成功
        printf("Found: (%d, %d) px @ %d Hz\n", closest.w, closest.h, closest.refresh_rate);
    }

    for(i = 0; i < SDL_GetNumVideoDisplays(); ++i){ // 利用可能なディスプレイの数だけ繰り返す
        SDL_GetCurrentDisplayMode(i, &current); // ディスプレイに対する現在のDMを取得
        SDL_Log("Display #%d: (%d, %d) px @ %d Hz", i, current.w, current.h, current.refresh_rate); // ログとしてデータを出力（表示）

        for(j = 0; j < SDL_GetNumDisplayModes(i); ++j){ // ディスプレイに対するDMの数だけ繰り返す
            SDL_GetDisplayMode(i, j, &current); // 現在のディスプレイ（およびDM）における利用可能なディスプレイモードを得る
            SDL_Log("Available diaplay mode #%d: (%d, %d) px @ %d Hz", j, current.w, current.h, current.refresh_rate);
        }
        SDL_GetDisplayBounds(i, &rect); // ディスプレイの表示領域を取得
        SDL_Log("Display bound(area): (%d, %d, %d, %d)", rect.x, rect.y, rect.w, rect.h);

        printf("Display Name: %s\n", SDL_GetDisplayName(i)); // ディスプレイ名を取得
    }

    for(i = 0; i < SDL_GetNumVideoDrivers(); ++i){ // 利用可能なビデオドライバの数だけ繰り返す
        printf("Video driver #%d: %s\n", i, SDL_GetVideoDriver(i)); // ビデオドライバを取得
    }

    // ウィンドウ関連
    int x, y, w, h, max_w, max_h;
    printf("Window display index: %d\n", SDL_GetWindowDisplayIndex(window)); // ウィンドウの存在するディスプレイ番号を取得
    printf("Window flag: %d\n", SDL_GetWindowFlags(window)); // ウィンドウのフラグを取得

    SDL_SetWindowTitle(window, "New Title"); // ウィンドウのタイトルを設定
    printf("Window title: %s\n", SDL_GetWindowTitle(window)); // ウィンドウのタイトルを取得

    SDL_GetWindowSize(window, &w, &h); // ウィンドウサイズを取得
    printf("Window size: (%d, %d)\n", w, h);
    SDL_GetWindowPosition(window, &x, &y); // ウィンドウの位置(x, y)を取得
    printf("Window position: (%d, %d)\n", x, y);
    SDL_SetWindowPosition(window, 300, 300); // ウィンドウの位置を設定
    SDL_GetWindowPosition(window, &x, &y); // ウィンドウの位置(x, y)を取得
    printf("Window position: (%d, %d)\n", x, y);
    SDL_SetWindowBordered(window, 0); // ウィンドウの枠を設定（0:枠なし）
    SDL_RaiseWindow(window); // ウィンドウを最前面にして入力フォーカスを与える
    SDL_Delay(2000); // 2秒待機

    SDL_SetWindowMaximumSize(window, 480, 320); // ウィンドウの最大サイズを設定
    SDL_GetWindowMaximumSize(window, &max_w, &max_h); // ウィンドウの最大サイズを取得
    printf("Window max size: (%d, %d)\n", max_w, max_h);
    SDL_MaximizeWindow(window); // ウィンドウを最大化（サイズ変更・反映）
    SDL_GetWindowSize(window, &w, &h); // ウィンドウサイズを取得
    printf("Window size: (%d, %d)\n", w, h);
    SDL_Delay(2000);

    SDL_MinimizeWindow(window); // ウィンドウ最小化
    SDL_Delay(2000);

    printf("Window brightness: %f\n", SDL_GetWindowBrightness(window)); // ウィンドウに存在するディスプレイの輝度を取得
    SDL_SetWindowBrightness(window, 0.1); // 輝度を設定（0.0:暗～1.0:通常）
    printf("Window brightness: %f\n", SDL_GetWindowBrightness(window));
    SDL_SetWindowBordered(window, 1); // ウィンドウの枠を設定（1:枠あり）
    SDL_RestoreWindow(window); // 最大化または最小化されたウィンドウを元のサイズと位置に戻す
    SDL_Delay(2000);

    SDL_SetWindowMinimumSize(window, 240, 180); // ウィンドウの最小サイズを設定
    SDL_SetWindowSize(window, 480, 60); // ウィンドウのサイズを設定（サイズ変更・反映）
    SDL_GetWindowSize(window, &w, &h); // ウィンドウサイズを取得
    printf("Window size: (%d, %d)\n", w, h);
    SDL_Delay(2000);

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); // ウィンドウをフルスクリーンに設定（ビデオモードをフルスクリーンに変更）
    SDL_Delay(2000);

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); // ウィンドウをフルスクリーンに設定（疑似フルスクリーン）
    SDL_Delay(2000);

    SDL_SetWindowFullscreen(window, 0); // ウィンドウをウィンドウモードに変更

    // SDL2.0.5以降で有効
    // SDL_GetWindowOpacity(window, &opacity); // ウィンドウの不透明度を取得
    // printf("Window opacity: %f\n", opacity);

    // メッセージボックス関連
    int buttonid;

    const SDL_MessageBoxButtonData buttons[] = { // ボタンの設定
        {0, 0, "Yes"}, // フラグ，ボタンID，ボタン文字（日本語OK?）
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "No"},
        {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Cancel"},
    };
    const SDL_MessageBoxColorScheme colorScheme = { // 色の設定
        {
            // メッセージボックス背景色
            {255, 255, 255}, // R, B, G
            // メッセージ文字色
            {0, 0, 0},
            // ボタン外枠色
            {100, 100,  100},
            // ボタン背景色
            {200, 200, 200},
            // 選択されたボタンの色
            {0, 0, 120}
        }
    };
    const SDL_MessageBoxData messageboxdata = { // メッセージ文字の設定
        SDL_MESSAGEBOX_INFORMATION, // フラグ
        window, // 親ウインドウ（NULLも可）
        "サンプル", // タイトル
        "Press a button.", // メッセージ
        SDL_arraysize(buttons), // 配置するボタン数
        buttons, // 設定したボタン
        &colorScheme // 設定した色
    };

    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) { // メッセージボックスの表示
        SDL_Log("Failre in showing message box.\n");
        return 0;
    }
    if (buttonid == -1) {
        SDL_Log("Button not selected.\n");
    } else {
       SDL_Log("%s was selected.\n", buttons[buttonid].text); // 選択されたボタン文字を表示
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Sample Simple Message", "Please press the button.", NULL); // シンプルなメッセージボックスを表示
    
    SDL_HideWindow(window);
    SDL_Delay(2000);

    // 終了処理
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/*************************************************
【演習】
（１）ウィンドウのサイズや位置を変更する
（２）ディスプレイ関連やウィンドウ関連の関数に対し，（可能なものに）エラー処理を記述する
（３）メッセージボックスの内容を変更する
（４）リサイズ可能なウィンドウを生成・表示して，マウス操作によるサイズ変更の動作を確認する
（５）ウィンドウを生成する際，SDL_WindowFlagsの違いによる挙動の変化を確認する（例．SDL_WINDOW_INPUT_GRABBEDが指定された際の動作）
（６）SDL2.0.5以降がインストールされていれば，SDL2.0.5以降で有効な関数の動作を確認する（関数の詳細はWeb上のリファレンスなどを参照）
*************************************************/


