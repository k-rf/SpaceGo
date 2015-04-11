#ifndef _INPUT_H
#define _INPUT_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>
#include "constants.h"
#include "gameError.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC   ((USHORT)0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE  ((USHORT)0x02)
#endif

namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;  // キー配列の大きさ

	// clear()のためのビットフラグの値
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

// デッドゾーンの範囲として20%をデフォルトとする
const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0X7FFF);
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;  // トリガーの範囲は0から255まで

const DWORD MAX_CONTROLLERS = 4;

const DWORD GAMEPAD_DPAD_UP        = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN      = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT      = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT     = 0x0008;
const DWORD GAMEPAD_START_BUTTON   = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON    = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB     = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB    = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER  = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A              = 0x1000;
const DWORD GAMEPAD_B              = 0x2000;
const DWORD GAMEPAD_X              = 0x4000;
const DWORD GAMEPAD_Y              = 0x8000;

struct ControllerState
{
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	float vibrateTimeLeft;
	float vibrateTimeRight;
	bool connected;
};

class Input
{
	// 指定のキーが押されている状態の場合にtrue
	bool keysDown[inputNS::KEYS_ARRAY_LEN];

	// 指定のキーが押された場合にtrue
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];
	std::string textIn;           // ユーザが入力したテキスト
	char charIn;                  // 最後に入力された文字
	bool newLine;                 // 新しい行の開始時にtrue
	int mouseX, mouseY;           // マウスの画面座標
	int mouseRawX, mouseRawY;     // 高精細マウスのデータ
	RAWINPUTDEVICE Rid[1];        // 高精細マウス用
	bool mouseCaptured;           // マウスがキャプチャされている場合にtrue
	bool mouseLButton;            // 左マウスボタンが押されている場合にtrue
	bool mouseMButton;            // 中央マウスボタンが押されている場合にtrue
	bool mouseRButton;            // 右マウスボタンが押されている場合にtrue
	bool mouseX1Button;           // X1マウスボタンが押されている場合にtrue
	bool mouseX2Button;           // X2マウスボタンが押されている場合にtrue
	ControllerState controllers[MAX_CONTROLLERS];  // コントローラの状態

public:
	// デフォルトコンストラクタ
	Input();

	// デストラクタ
	virtual ~Input();

	// マウスとコントローラの入力を初期化
	// マウスをキャプチャする場合、capture = trueを設定
	// GameErrorをスロー
	void initialize(HWND hwnd, bool capture);

	// このキーについて、keysDown配列とkeysPressed配列にtrueが設定
	// 実行前：wParamに、仮想キーコード(0 ~ 255)が格納されている
	void keyDown(WPARAM wParam);

	// このキーについて、keysDown配列にfalseを設定
	// 実行前：wParamに、仮想キーコード(0 ~ 255)が格納されている
	void keyUp(WPARAM wParam);

	// 入力された文字をtextIn文字列に保存
	// 実行前：wParamに、文字が格納されている
	void keyIn(WPARAM wParam);

	// 指定された仮想キーが押されている場合はtrue、それ以外の場合はfalseを戻す
	bool isKeyDown(UCHAR vkey)const;

	// 直近のフレームにおいて、指定の仮想キーが押されたことがある場合、trueを戻します
	// キーの押し下げの状態は、各フレームの終了時に消去されます
	bool wasKeyPressed(UCHAR vkey)const;

	// 直近のフレームにおいて、何らかのキーが押された場合、trueを戻します
	// キーの押し下げの状態は、各フレームの終了時に消去されます
	bool anyKeyPressed()const;

	// テキスト入力を文字列として戻す
	std::string getTextIn()
	{
		return textIn;
	}

	// 最後に入力された文字を戻す
	char getCharIn()
	{
		return charIn;
	}

	// 指定されたキーの押し下げをクリア
	void clearKeyPress(UCHAR vkey);

	// テキスト入力バッファをクリア
	void clearTextIn()
	{
		textIn.clear();
	}

	// キー、マウス、テキスト入力データをクリア
	void clearAll()
	{
		clear(inputNS::KEYS_MOUSE_TEXT);
	}

	// 指定の入力バッファをクリア
	// whatの値についてはinput.hを参照
	void clear(UCHAR what);

	// マウスの画面位置を読み取り、mouseXとmouseYに保存
	void mouseIn(LPARAM lParam);

	// マウスからのローデータを読み取り、mouseRawXとmouseYに保存
	// このルーチンは、高精細マウスに対応しています
	void mouseRawIn(LPARAM lParam);

	// マウスボタンの状態を保存
	void setMouseLButton(bool b)
	{
		mouseLButton = b;
	}

	// マウスボタンの状態を保存
	void setMouseMButton(bool b)
	{
		mouseMButton = b;
	}

	// マウスボタンの状態を保存
	void setMouseRButton(bool b)
	{
		mouseRButton = b;
	}

	// マウスボタンの状態を保存
	void setMouseXButton(WPARAM wParam)
	{
		mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}

	// マウスのXの位置を戻す
	int getMouseX()const
	{
		return mouseX;
	}

	// マウスのYの位置を戻す
	int getMouseY()const
	{
		return mouseY;
	}

	// マウスのX位置の移動のローデータを戻す。左への移動は<0、右への移動は>0
	// 高精細マウスに対応
	int getMouseRawX()const
	{
		return mouseRawX;
	}

	// マウスのY位置の移動のローデータを戻す。上への移動は<0、下への移動は>0
	// 高精細マウスに対応
	int getMouseRawY()const
	{
		return mouseRawY;
	}

	// 左マウスボタンの状態を戻す
	bool getMouseLButton()const
	{
		return mouseLButton;
	}

	// 中央マウスボタンの状態を戻す
	bool getMouseMButton()const
	{
		return mouseMButton;
	}

	// 右マウスボタンの状態を戻す
	bool getMouseRButton()const
	{
		return mouseRButton;
	}

	// X1マウスボタンの状態を戻す
	bool getMouseX1Button()const
	{
		return mouseX1Button;
	}

	// X2マウスボタンの状態を戻す
	bool getMouseX2Button()const
	{
		return mouseX2Button;
	}

	// コントローラの接続状態を更新する
	void checkControllers();

	// 接続されているコントローラの状態を読み取る
	void readControllers();

	// 指定のゲームコントローラの状態を戻す
	const ControllerState* getControllerState(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}

	// コントローラnのボタンの状態を戻す
	const WORD getGamepadButton(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.wButtons;
	}

	// コントローラnの方向パッド上の状態を戻す
	bool getGamepadDPadUp(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_UP) != 0);
	}

	// コントローラnの方向パッド下の状態を戻す
	bool getGamepadDPadDown(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_DOWN) != 0);
	}

	// コントローラnの方向パッド左の状態を戻す
	bool getGamepadDPadLeft(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_LEFT) != 0);
	}

	// コントローラnの方向パッド右の状態を戻す
	bool getGamepadDPadRight(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_RIGHT) != 0);
	}

	// コントローラnのSTARTボタンの状態を戻す
	bool getGamepadStart(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_START_BUTTON) != 0);
	}

	// コントローラnのBACKボタンの状態を戻す
	bool getGamepadBack(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_BACK_BUTTON) != 0);
	}

	// コントローラnの左サムボタンの状態を戻す
	bool getGamepadLeftThumb(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_LEFT_THUMB) != 0);
	}

	// コントローラnの右サムボタンの状態を戻す
	bool getGamepadRightThumb(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_RIGHT_THUMB) != 0);
	}

	// コントローラnの左ショルダーボタンの状態を戻す
	bool getGamepadLeftShoulder(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_LEFT_SHOULDER) != 0);
	}

	// コントローラnの右ショルダーボタンの状態を戻す
	bool getGamepadRightShoulder(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	// コントローラnのAボタンの状態を戻す
	bool getGamepadA(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_A) != 0);
	}

	// コントローラnのBボタンの状態を戻す
	bool getGamepadB(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_B) != 0);
	}

	// コントローラnのXボタンの状態を戻す
	bool getGamepadX(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_X) != 0);
	}

	// コントローラnのYボタンの状態を戻す
	bool getGamepadY(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_Y) != 0);
	}

	// コントローラnの左トリガーの値を戻します
	BYTE getGamepadLeftTrigger(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bLeftTrigger);
	}

	// コントローラnの右トリガーの値を戻します
	BYTE getGamepadRightTrigger(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bRightTrigger);
	}

	// コントローラnの左サムスティック、Xの値を戻します
	SHORT getGamepadThumbLX(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLX);
	}

	// コントローラnの左サムスティック、Yの値を戻します
	SHORT getGamepadThumbLY(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLY);
	}

	// コントローラnの右サムスティック、Xの値を戻します
	SHORT getGamepadThumbRX(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRX);
	}

	// コントローラnの右サムスティック、Yの値を戻します
	SHORT getGamepadThumbRY(UINT n)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRY);
	}

	// コントローラnの左モータを振動させる
	// 左は低周波振動
	// speed：0 = オフ、65536 = 100パーセント
	// sec：振動させる時間(秒)
	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if(n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}

	// コントローラnの右モータを振動させる
	// 右は高周波振動
	// speed：0 = オフ、65536 = 100パーセント
	// sec：振動させる時間(秒)
	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if(n < MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrateTimeRight = sec;
	}

	// 接続されているコントローラを振動させる
	void vibrateControllers(float frameTime);
};

#endif