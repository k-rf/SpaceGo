#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "constants.h"

class TextureManager
{
	UINT width;         // テクスチャの幅（ピクセル単位）
	UINT height;        // テクスチャの高さ（ピクセル単位）
	LP_TEXTURE texture; // テクスチャへのポインタ
	const char* file;   // ファイルの名前
	Graphics* graphics; // グラフィックスへのポインタを保存
	bool initialized;   // 初期化に成功したかどうか
	HRESULT hr;         // 普通の返却型

public:
	// コンストラクタ
	TextureManager();

	// デストラクタ
	virtual ~TextureManager();

	// テクスチャへのポインタを戻す
	LP_TEXTURE getTexture()const
	{
		return texture;
	}

	// テクスチャの幅を戻す
	UINT getWidth()const
	{
		return width;
	}

	// テクスチャの高さを戻す
	UINT getHeight()const
	{
		return height;
	}

	// テクスチャファイルをディスクから読み込む
	// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
	virtual bool initialize(Graphics* g, const char* f);

	// グラフィックスデバイスが消失した時に呼び出される
	virtual void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出される
	virtual void onResetDevice();
};

#endif