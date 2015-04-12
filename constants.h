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
const char CLASS_NAME[] = "Space Go";
const char APP_TITLE[] = "Space Go";
const bool FULLSCREEN = false;
const UINT GAME_WIDTH = 360 * 1.2;
const UINT GAME_HEIGHT = 640 * 1.2;
const double PI = 3.14159265;

// フレーム
const float FRAME_RATE = 200.0f;
const float MIN_FRAME_RATE = 10.0f;
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;

// キー
const UCHAR ESC_KEY   = VK_ESCAPE;
const UCHAR ALT_KEY   = VK_MENU;
const UCHAR ENTER_KEY = VK_RETURN;
const UCHAR LEFT_KEY  = VK_LEFT;
const UCHAR RIGHT_KEY = VK_RIGHT;
const UCHAR UP_KEY    = VK_UP;
const UCHAR DOWN_KEY  = VK_DOWN;

// テクスチャデータ
const char BACK_GROUND[] = "textures\\back_ground.bmp";
const char CHARA_MAP[] = "textures\\chara_map2.bmp";
const char OBSTACLE[] = "textures\\bullets.bmp";
const char NUMBER[10][13] = {
	"number\\0.png",
	"number\\1.png",
	"number\\2.png",
	"number\\3.png",
	"number\\4.png",
	"number\\5.png",
	"number\\6.png",
	"number\\7.png",
	"number\\8.png",
	"number\\9.png",
};

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