#ifndef _IMAGE_H
#define _IMAGE_H

#define WIN32_LEAN_AND_MEAN

#include "textureManager.h"
#include "constants.h"

class Image
{
protected:
	Graphics* graphics;      // グラフィックスへのポインタ
	TextureManager* textureManager;    // テクスチャマネージャーへのポインタ

	// Graphics::drawSprite()が必要とするデータを格納する構造体
	SpriteData spriteData;   // SpriteDataは"graphics.h"で定義されています
	COLOR_ARGB colorFilter;  // カラーフィルター
	int cols;           // 複数のフレームを持つスプライト内の列数（1からnまで）
	int startFrame;     // 複数のフレームを持つアニメーションシーケンスの最初のフレーム
	int endFrame;       // 複数のフレームを持つアニメーションシーケンスの最後のフレーム
	int currentFrame;   // アニメーションシーケンスの現在のフレーム番号
	float frameDelay;   // アニメーションの各フレームを表示する間隔（秒単位）
	float animTimer;    // アニメーションの時間を計るために使用する変数
	HRESULT hr;         // 標準の戻り型
	bool loop;          // アニメーションを繰り返す場合は、true
	bool visible;       // trueの場合、画像が可視になります
	bool initialized;   // 画像が正常に初期化された場合、trueです
	bool animComplete;  // ループしないアニメーションシーケンスが完了した時に、trueになる

public:
	// コンストラクタ
	Image();

	// デストラクタ
	virtual ~Image();

	// --------------------------------------------------------------
	// get関数
	// --------------------------------------------------------------

	// SpriteData構造体への参照を戻す
	const virtual SpriteData& getSpriteInfo() { return spriteData; }

	// visibleパラメータを戻す
	virtual bool getVisible() { return visible; }

	// X位置を戻す
	virtual float getX() { return spriteData.x; }

	// Y位置を戻す
	virtual float getY() { return spriteData.y; }

	// 倍率を戻す
	virtual float getScale() { return spriteData.scale; }

	// 幅を戻す
	virtual float getWidth() { return spriteData.width; }

	// 高さを戻す
	virtual float getHeight() { return spriteData.height; }

	// 中心点のXを戻す
	virtual float getCenterX() { return spriteData.x + spriteData.width / 2 * getScale(); }

	// 中心点のYを戻す
	virtual float getCenterY() { return spriteData.y + spriteData.height / 2 * getScale(); }

	// 回転角度（度単位）を戻す
	virtual float getDegrees() { return spriteData.angle * (180.0f / (float)PI); }

	// 回転角度（ラジアン単位）を戻す
	virtual float getRadians() { return spriteData.angle; }

	// アニメーションのフレーム間の間隔を戻す
	virtual float getFrameDelay() { return frameDelay; }

	// 開始フレームの番号を戻す
	virtual int getStartFrame() { return startFrame; }

	// 終了フレームの番号を戻す
	virtual int getEndFrame() { return endFrame; }

	// 現在のフレーム番号を戻す
	virtual int getCurrentFrame() { return currentFrame; }

	// ImageのRECT構造体を戻す
	virtual RECT getSpriteDataRect() { return spriteData.rect; }

	// アニメーションの完了の状態を戻す
	virtual bool getAnimationComplete() { return animComplete; }

	// colorFilterを戻す
	virtual COLOR_ARGB getColorFilter() { return colorFilter; }


	// --------------------------------------------------------------
	// set関数
	// --------------------------------------------------------------

	// X位置を設定
	virtual void setX(float newX) { spriteData.x = newX; }

	// Y位置を設定
	virtual void setY(float newY) { spriteData.y = newY; }

	// 拡大縮小を設定
	virtual void setScale(float s) { spriteData.scale = s; }

	// 回転角度を設定（度単位）
	// 0度は上向きです。角度は時計回りに進みます。
	virtual void setDegrees(float deg) { spriteData.angle = deg * ((float)PI / 180.0f); }

	// 回転角度を設定（ラジアン単位）
	// 0ラジアンは上向きです。角度は時計回りに進みます。
	virtual void setRadians(float rad) { spriteData.angle = rad; }

	// 可視性を設定
	virtual void setVisible(bool v) { visible = v; }

	// アニメーションのフレーム間の間隔を設定
	virtual void setFrameDelay(float d) { frameDelay = d; }

	// アニメーションの開始フレームと終了フレームを設定
	virtual void setFrames(int s, int e) { startFrame = s; endFrame = e; }

	// アニメーションの現在のフレームを設定
	virtual void setCurrentFrame(int c);

	// currentFrameを描画するspriteData.rectを設定
	virtual void setRect();

	// spriteData.rectをrに設定
	virtual void setSpriteDataRect(RECT r) { spriteData.rect = r; }

	// アニメーションループを設定（lp = trueの場合にループする）
	virtual void setLoop(bool lp) { loop = lp; }

	// アニメーション完了ブール値を設定
	virtual void setAnimationComplete(bool a) { animComplete = a; }

	// 色フィルタを設定（WHITEを使用すると、変化しない）
	virtual void setcolorFilter(COLOR_ARGB color) { colorFilter = color; }


	// --------------------------------------------------------------
	// その他の関数
	// --------------------------------------------------------------

	// Imageを初期化
	// 実行後：成功した場合はtrue、失敗した場合はfalseを戻す
	// Graphicsへのポインタ
	// Imageの幅（ピクセル単位）（0 = テクスチャ全体の幅を使用）
	// Imageの高さ（ピクセル単位）（0 = テクスチャ全体の高さを使用）
	// テクスチャ内の列数（1からnまで）（0は1と同じ）
	// TextureManagerへのポインタ
	virtual bool initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM);

	// colorをフィルタとして使って画像を描画。colorのデフォルトは白。
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

	// 指定されたSpriteDataを使ってこの画像を描画
	// 現在のSpriteData.rectを使って、テクスチャを選択
	virtual void draw(SpriteData sd, COLOR_ARGB color);

	// 画像を水平方向に反転（鏡）
	virtual void flipHorizontal(bool flip) { spriteData.flipHorizontal = flip; }

	// 画像を垂直方向に反転
	virtual void flipVertical(bool flip) { spriteData.flipVertical = flip; }

	// アニメーションの更新
	virtual void update(float frameTime);
};

#endif