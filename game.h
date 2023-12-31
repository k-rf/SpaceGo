#ifndef _GAME_H
#define _GAME_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"

class Game
{
protected:
	// 共通のゲームプロパティ
	Graphics* graphics;         // Graphicsへのポインタ
	Input* input;               // Inputへのポインタ
	HWND hwnd;                  // ウィンドウハンドル
	HRESULT hr;                 // 標準の戻り型
	LARGE_INTEGER timeStart;    // パフォーマンスカウンターの開始値
	LARGE_INTEGER timeEnd;      // パフォーマンスカウンターの終了値
	LARGE_INTEGER timeFreq;     // パフォーマンスカウンターの周波数
	float frameTime;            // 最後のフレームに要した時間
	float fps;                  // フレームレート(1秒あたりのフレーム数)
	DWORD sleepTime;            // フレーム間でスリープする時間(ミリ秒)
	bool paused;                // ゲームが一時停止されている場合、true
	bool initialized;

public:
	// コンストラクタ
	Game();

	// デストラクタ
	virtual ~Game();

	// メンバ関数

	// Windowsメッセージハンドラ
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ゲームを初期化
	// 実行前：hwndはウィンドウへのハンドル
	virtual void initialize(HWND hwnd);

	//WinMain内のメインのメッセージループでrunを繰り返し呼び出す
	virtual void run(HWND);

	// グラフィックスデバイスが焼失した時に呼び出す
	// グラフィックスデバイスをリセット可能にするため、
	// 予約されていたビデオメモリをすべて解放
	virtual void releaseAll();

	// すべてのサーフェイスを再作成し、すべてのエンティティをリセット
	virtual void resetAll();

	// 予約されていたメモリをすべて削除
	virtual void deleteAll();

	// ゲームアイテムをレンダー
	virtual void renderGame();

	// 消失したグラフィックスデバイスを処理
	virtual void handleLostGraphicsDevice();

	// Graphicsへのポインタを戻す
	Graphics* getGraphics() { return graphics; }

	// Inputへのポインタを戻す
	Input* getInput() { return input; }

	// ゲームを終了
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// ------------------------------------------------------------------------
	// 純粋仮想関数の宣言
	// ------------------------------------------------------------------------
	
	// ゲームアイテムを更新
	virtual void update() = 0;

	// AI計算を実行
	virtual void ai() = 0;

	// 衝突をチェック
	virtual void collisions() = 0;

	// グラフィックスをレンダー
	// graphics->spriteBegin();を呼び出す
	// スプライトを描画
	// graphics->spriteEnd();を呼び出す
	// スプライト以外を描画
	virtual void render() = 0;
};

#endif
