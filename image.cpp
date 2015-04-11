#include "image.h"

// ==================================================================
// デフォルトコンストラクタ
// ==================================================================
Image::Image()
{
	initialized = false;
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0;
	animTimer = 0.0;
	visible = true;
	loop = true;
	animComplete = false;
	graphics = NULL;
	colorFilter = graphicsNS::WHITE;
}

// ==================================================================
// デストラクタ
// ==================================================================
Image::~Image()
{}

// ==================================================================
// Imageを初期化
// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
// Graphicsへのポインタ
// Imageの幅（ピクセル単位）（0 = テクスチャ全体の幅を使用）
// Imageの高さ（ピクセル単位）（0 = テクスチャ全体の高さを使用）
// テクスチャ内の列数（1からnまで）（0は1と同じ）
// TextureManagerへのポインタ
// ==================================================================
bool Image::initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM)
{
	try
	{
		graphics = g;                  // grahpicsオブジェクト
		textureManager = textureM;     // テクスチャオブジェクトへのポインタ

		spriteData.texture = textureManager->getTexture();

		if(width == 0)
			width = textureManager->getWidth();  // 全体の幅を使用
		spriteData.width = width;

		if(height == 0)
			height = textureManager->getHeight();  // 全体の高さを使用
		spriteData.height = height;

		cols = ncols;
		if(cols == 0)
			cols = 1;  // colsが0の場合は、1を使用

		// currentFrameを描画するspriteData.rectを構成
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		// 右端　+ 1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame % cols) * spriteData.height;
		// 下端 + 1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch(...)
	{
		return false;
	}

	initialized = true;  // 正常に初期化された場合

	return true;
}

// ==================================================================
// colorをフィルタとして使って画像を描画
// colorパラメータはオプション
// （image.hで、デフォルトとして白が割り当てられている）
// 実行前：spriteBegin()を呼び出す
// 実行後：spriteEnd()を呼び出す
// ==================================================================
void Image::draw(COLOR_ARGB color)
{
	if(!visible || graphics == NULL)
		return;

	// onReset()が呼び出されたときに新しいテクスチャを取得
	spriteData.texture = textureManager->getTexture();

	if(color == graphicsNS::FILTER)  // フィルタを使って描画する場合
		// colorFilterを使用
		graphics->drawSprite(spriteData, colorFilter);
	else
		// フィルタとしてcolorを使用
		graphics->drawSprite(spriteData, color);
}

// ==================================================================
// 指定されたSpriteDataを使ってこの画像を描画
// 現在のSpriteData.rectを使って、テクスチャを選択
// 実行前：spriteBegin()を呼び出す
// 実行後：spriteEnd()を呼び出す
// ==================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
	if(!visible || graphics == NULL)
		return;

	// この画像の矩形を使ってテクスチャを選択
	sd.rect = spriteData.rect;

	// onReset()が呼び出されたときに新しいテクスチャを取得
	sd.texture = textureManager->getTexture();

	if(color == graphicsNS::FILTER)              // フィルタを使って描画する場合
		graphics->drawSprite(sd, colorFilter);   // colorFilterを使用
	else
		graphics->drawSprite(sd, color);         // フィルタとしてcolorを使用
}

// ============================================================================
// 更新
// 通常1フレームに1回呼び出される
// frameTimeは動作とアニメーションの速度を管理するのに用いられる
// ============================================================================
void Image::update(float frameTime)
{
	if(endFrame - startFrame > 0)
	{
		animTimer += frameTime;
		if(animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if(currentFrame < startFrame || currentFrame > endFrame)
			{
				if(loop == true)
					currentFrame = startFrame;
				else
				{
					currentFrame = endFrame;
					animComplete = true;
				}
			}
			setRect();
		}
	}
}

// ============================================================================
// テクスチャの現在のフレームを設定
// ============================================================================
void Image::setCurrentFrame(int c)
{
	if(c >= 0)
	{
		currentFrame = c;
		animComplete = false;
		setRect();
	}
}

// ============================================================================
// 現在のフレームに描画するためのspriteData.rectを設定
// ============================================================================
inline void Image::setRect()
{
	// currentFrameを描画するspriteData.rectを構成
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	// 右端　+ 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame % cols) * spriteData.height;
	// 下端 + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}