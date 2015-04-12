#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <ctime>
#include "spaceGo.h"

// 関数プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND&, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// ゲームポインタ
SpaceGo* game = NULL;
HWND hwnd = NULL;

// ============================================================================
// Windowsアプリケーションの開始点
// パラメータ
//     hInstance    ：アプリケーションの現在のインスタンスへのハンドル
//     hPrevInstance：常にNULL（廃止されたパラメータ）
//     lpCmdLine    ：コマンドライン引数のNULL終端文字列へのポインタ
//     nCmdShow     ：ウィンドウをどのように表示するかを指定
// ============================================================================
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// デバッグビルドの場合、メモリリークをチェック
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	srand((unsigned)time(NULL));

	MSG msg;

	// ゲームの作成、メッセージハンドラの設定
	game = new SpaceGo;

	// ウィンドウを作成
	if(!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;

	try
	{
		game->initialize(hwnd);

		// メインのメッセージループ
		int done = 0;
		while(!done)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					done = 1;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				game->run(hwnd);  // ゲームループ
		}
		SAFE_DELETE(game);  // 終了の前にメモリを解放
		return msg.wParam;
	}
	catch(const GameError& err)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	catch(...)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occured in game.", "Error", MB_OK);
	}

	SAFE_DELETE(game);  // 終了の前にメモリを解放
	return 0;
}

// ============================================================================
// ウィンドウイベントコールバック関数
// ============================================================================
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->messageHandler(hWnd, msg, wParam, lParam);
}

// ============================================================================
// ウィンドウを作成
// 戻り値：エラーの場合、false
// ============================================================================
bool CreateMainWindow(HWND& hwnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	// ウィンドウクラスの構造体をメインウィンドウを記述するパラメータで設定します
	wcx.cbSize = sizeof(wcx);                    // 構造体のサイズ
	wcx.style = CS_HREDRAW | CS_VREDRAW;         // ウィンドウサイズ変更時に再描画
	wcx.lpfnWndProc = WinProc;                   // ウィンドウプロシージャを指す
	wcx.cbClsExtra = 0;                          // 拡張クラスメモリなし
	wcx.cbWndExtra = 0;                          // 拡張ウィンドウメモリなし
	wcx.hInstance = hInstance;                   // インスタンスへのハンドル
	wcx.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);   // 事前定義されている矢印カーソル
	wcx.hbrBackground =
		(HBRUSH)GetStockObject(BLACK_BRUSH);     // 背景ブラシ
	wcx.lpszMenuName = NULL;                     // メニューリソースの名前
	wcx.lpszClassName = CLASS_NAME;              // ウィンドウのクラスの名前
	wcx.hIconSm = NULL;                          // 小さいアイコン

	// ウィンドウクラスを登録
	// エラー時、RegisterClassExは0を戻す
	if(RegisterClassEx(&wcx) == 0)
		return false;

	// ウィンドウを作成
	hwnd = CreateWindow(
		CLASS_NAME,                    // ウィンドウクラスの名前
		APP_TITLE,                     // タイトルバーのテキスト
		WS_OVERLAPPEDWINDOW,           // ウィンドウのスタイル
		CW_USEDEFAULT,                 // ウィンドウの水平位置のデフォルト
		CW_USEDEFAULT,                 // ウィンドウの垂直位置のデフォルト
		GAME_WIDTH,                    // ウィンドウの幅
		GAME_HEIGHT,                   // ウィンドウの高さ
		(HWND)NULL,                    // 親ウィンドウなし
		(HMENU)NULL,                   // メニューなし
		hInstance,                     // アプリケーションインスタンスへのハンドル
		(LPVOID)NULL                   // ウィンドウパラメータなし
		);

	// ウィンドウ作成でエラーが発生した場合
	if(!hwnd)
		return false;

	// ウィンドウを表示
	ShowWindow(hwnd, nCmdShow);

	// ウィンドウプロシージャにWM_PAINTメッセージを送る
	UpdateWindow(hwnd);
	return true;
}