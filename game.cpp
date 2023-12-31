#include "game.h"

// ==================================================================
// コンストラクタ
// ==================================================================
Game::Game()
{
	input = new Input();  // キーボード入力を即時に初期化

	// その他の初期化は、後で
	// input->initialize()を呼び出して処理
	paused = false;  // ゲームは一時停止中ではない
	graphics = NULL;
	initialized = false;
}

// ==================================================================
// デストラクタ
// ==================================================================
Game::~Game()
{
	deleteAll();      // 予約されていたメモリをすべて解放
	ShowCursor(true); // カーソルを表示
}

// ==================================================================
// Windowsメッセージハンドラ
// ==================================================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(initialized)  // 初期化されていない場合はメッセージを処理しない
	{
		switch(msg)
		{
		case WM_DESTROY:
			// Windowsにこのプログラムを終了するように伝える
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:        // キーが押された
			input->keyDown(wParam);
			return 0;

		case WM_KEYUP:
		case WM_SYSKEYUP:          // キーが離された
			input->keyUp(wParam);
			return 0;

		case WM_CHAR:              // 文字が入力された
			input->keyIn(wParam);
			return 0;

		case WM_MOUSEMOVE:         // マウスが移動された
			input->mouseIn(lParam);
			return 0;

		case WM_INPUT:             // マウスからのローデータ入力
			input->mouseRawIn(lParam);
			return 0;

		case WM_LBUTTONDOWN:       // 左マウスボタンが押された
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_LBUTTONUP:         // 左マウスボタンが離された
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_MBUTTONDOWN:       // 中央マウスボタンが押された
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_MBUTTONUP:         // 中央マウスボタンが離された
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_RBUTTONDOWN:       // 右マウスボタンが押された
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_RBUTTONUP:         // 右マウスボタンが離された
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_XBUTTONDOWN:       // マウスのXボタンが押された
		case WM_XBUTTONUP:         //                離された
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return 0;

		case WM_DEVICECHANGE:      // コントローラをチェック
			input->checkControllers();
			return 0;
		}
	}

	// Windowsに処理を任せる
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ==================================================================
// ゲームを初期化
// エラー時にGameErrorをスロー
// ==================================================================
void Game::initialize(HWND hw)
{
	hwnd = hw;  // ウィンドウハンドルを保存

	// グラフィックを初期化
	graphics = new Graphics();

	// GameErrorをスロー
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// 入力を初期化、マウスはキャプチャしない
	input->initialize(hwnd, false);  // GameErrorをスロー

	// 高分解能タイマーの準備を試みる
	if(QueryPerformanceFrequency(&timeFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
	QueryPerformanceCounter(&timeStart);  // 開始時間を取得
	initialized = true;
}

// ==================================================================
// 消失したグラフィックスデバイスを処理
// ==================================================================
void Game::handleLostGraphicsDevice()
{
	// デバイスの消失をテストし、それに応じて処理を実行
	hr = graphics->getDeviceState();
	if(FAILED(hr))                   // グラフィックスデバイスが有効な状態でない場合
	{
		// デバイスが消失しており、リセットできる状態にない場合
		if(hr == D3DERR_DEVICELOST)
		{
			Sleep(100);              // CPU時間を明け渡す(100ミリ秒)
			return;
		}

		// デバイスが消失しているが、リセットできる状態にある場合
		else if(hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();  // グラフィックスデバイスのリセットを試みる
			if(FAILED(hr))           // リセットが失敗した場合
				return;
			resetAll();
		}
		else
			return;                  // 他のデバイスエラー
	}
}

// ==================================================================
// ゲームアイテムをレンダー
// ==================================================================
void Game::renderGame()
{
	// レンダリングを開始
	if(SUCCEEDED(graphics->beginScene()))
	{
		// renderは、継承したクラス側で記述する必要のある純粋仮想関数です
		render();  // 派生クラスのrender()を呼び出す

		// レンダリングを終了
		graphics->endScene();
	}
	handleLostGraphicsDevice();

	// バックバッファを画面に表示
	graphics->showBackbuffer();
}

// ==================================================================
// WinMain内のメインのメッセージループで繰り返し呼び出される
// ==================================================================
void Game::run(HWND hwnd)
{
	if(graphics == NULL)  // グラフィックスが初期化されていない場合
		return;

	// エスケープキーで終了
	if(input->isKeyDown(ESC_KEY))
	{
		PostQuitMessage(0);
		return;
	}

	// 最後のフレームからの経過時間を計算、frameTimeに保存
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timeFreq.QuadPart;

	// 省電力コード(winmm.libが必要)
	// 希望するフレームレートに対して経過時間が短い場合
	if(frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);  // 1ミリ秒の分解能をWindowsタイマーに要求
		Sleep(sleepTime);    // sleepTimeの間、CPUを解放
		timeEndPeriod(1);    // 1ミリ秒のタイマー分解能を終了
		return;
	}

	if(frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime);  // 平均fps

	if(frameTime > MAX_FRAME_TIME)        // フレームレートが非常に遅い場合
		frameTime = MAX_FRAME_TIME;       // 最大frameTimeを制限

	timeStart = timeEnd;

	input->readControllers();             // コントローラの状態を読み取る

	// update()、ai()、collisions()は純粋仮想関数です
	// これらの関数は、Gameを継承しているクラス側で記述する必要があります
	if(!paused)                           // 一時停止中でない
	{
		update();                         // すべてのゲームアイテムを更新
		ai();                             // 人工知能
		collisions();                     // 衝突を処理
		input->vibrateControllers(frameTime);  //コントローラの振動を処理
	}
	renderGame();                         // すべてのゲームアイテムを描画

	// 入力をクリア
	// すべてのキーチェックが行われた後これを呼び出す
	input->clear(inputNS::KEYS_PRESSED);
}

// ============================================================================
// グラフィックスデバイスの消失
// すべてのビデオメモリを解放してグラフィックスデバイスをリセットする
// ============================================================================
void Game::releaseAll()
{}

// ============================================================================
// すべてリセットする
// ============================================================================
void Game::resetAll()
{}

// ============================================================================
// すべてのメモリを削除する
// ============================================================================
void Game::deleteAll()
{
	releaseAll();
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	initialized = false;
}
