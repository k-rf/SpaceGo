#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

// 透過色キー（マゼンタ）
#define TRANSCOLOR SETCOLOR_ARGB(0, 255, 0, 255)

// ============================================================================
// 定数
// ============================================================================

// ウィンドウ
const char CLASS_NAME[] = "Block Break";
const char APP_TITLE[] = "Block Break";
const bool FULLSCREEN = false;
const UINT GAME_WIDTH = 300;
const UINT GAME_HEIGHT = 600;

// フレーム
const float FRAME_RATE = 200.0f;
const float MIN_FRAME_RATE = 10.0f;
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;

// キー
const UCHAR ESC_KEY = VK_ESCAPE;
const UCHAR ALT_KEY = VK_MENU;
const UCHAR ENTER_KEY = VK_RETURN;
const UCHAR LEFT_KEY = VK_LEFT;
const UCHAR RIGHT_KEY = VK_RIGHT;
const UCHAR UP_KEY = VK_UP;
const UCHAR DOWN_KEY = VK_DOWN;

// テクスチャデータ
const char BALL_IMAGE[] = "pictures\\ball2.png";

const double PI = 3.14159265;
const float GRAVITY = 700.0f;  // 重力加速度（1秒あたりのピクセル数）
const float BALL_SPEED = 200.0f;  // ボールの速度（1秒あたりのピクセル数）


// ============================================================================
// テンプレート関数
// ============================================================================

// 安全にポインタを解放する
template <class T>
void safeRelease(T& ptr)
{
	if(ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease

// 安全にポインタを削除する
template <class T>
void safeDelete(T& ptr)
{
	if(ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE safeDelete

// 安全に配列を削除する
template <class T>
void safeDeleteArray(T& ptr)
{
	if(ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}
#define SAFE_DELETE_ARRAY safeDeleteArray

// 安全に消失したデバイスを呼び出す
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
	if(ptr)
		ptr->onLostDevice();
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice

// 安全にリセットしたデバイスを呼び出す
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
	if(ptr)
		ptr->onResetDevice();
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice

#endif