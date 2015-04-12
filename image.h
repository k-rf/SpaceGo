#ifndef _IMAGE_H
#define _IMAGE_H

#define WIN32_LEAN_AND_MEAN

#include "textureManager.h"
#include "constants.h"

class Image
{
protected:
	Graphics* graphics;      // �O���t�B�b�N�X�ւ̃|�C���^
	TextureManager* textureManager;    // �e�N�X�`���}�l�[�W���[�ւ̃|�C���^

	// Graphics::drawSprite()���K�v�Ƃ���f�[�^���i�[����\����
	SpriteData spriteData;   // SpriteData��"graphics.h"�Œ�`����Ă��܂�
	COLOR_ARGB colorFilter;  // �J���[�t�B���^�[
	int cols;           // �����̃t���[�������X�v���C�g���̗񐔁i1����n�܂Łj
	int startFrame;     // �����̃t���[�������A�j���[�V�����V�[�P���X�̍ŏ��̃t���[��
	int endFrame;       // �����̃t���[�������A�j���[�V�����V�[�P���X�̍Ō�̃t���[��
	int currentFrame;   // �A�j���[�V�����V�[�P���X�̌��݂̃t���[���ԍ�
	float frameDelay;   // �A�j���[�V�����̊e�t���[����\������Ԋu�i�b�P�ʁj
	float animTimer;    // �A�j���[�V�����̎��Ԃ��v�邽�߂Ɏg�p����ϐ�
	HRESULT hr;         // �W���̖߂�^
	bool loop;          // �A�j���[�V�������J��Ԃ��ꍇ�́Atrue
	bool visible;       // true�̏ꍇ�A�摜�����ɂȂ�܂�
	bool initialized;   // �摜������ɏ��������ꂽ�ꍇ�Atrue�ł�
	bool animComplete;  // ���[�v���Ȃ��A�j���[�V�����V�[�P���X�������������ɁAtrue�ɂȂ�

public:
	// �R���X�g���N�^
	Image();

	// �f�X�g���N�^
	virtual ~Image();

	// --------------------------------------------------------------
	// get�֐�
	// --------------------------------------------------------------

	// SpriteData�\���̂ւ̎Q�Ƃ�߂�
	const virtual SpriteData& getSpriteInfo() { return spriteData; }

	// visible�p�����[�^��߂�
	virtual bool getVisible() { return visible; }

	// X�ʒu��߂�
	virtual float getX() { return spriteData.x; }

	// Y�ʒu��߂�
	virtual float getY() { return spriteData.y; }

	// �{����߂�
	virtual float getScale() { return spriteData.scale; }

	// ����߂�
	virtual float getWidth() { return spriteData.width; }

	// ������߂�
	virtual float getHeight() { return spriteData.height; }

	// ���S�_��X��߂�
	virtual float getCenterX() { return spriteData.x + spriteData.width / 2 * getScale(); }

	// ���S�_��Y��߂�
	virtual float getCenterY() { return spriteData.y + spriteData.height / 2 * getScale(); }

	// ��]�p�x�i�x�P�ʁj��߂�
	virtual float getDegrees() { return spriteData.angle * (180.0f / (float)PI); }

	// ��]�p�x�i���W�A���P�ʁj��߂�
	virtual float getRadians() { return spriteData.angle; }

	// �A�j���[�V�����̃t���[���Ԃ̊Ԋu��߂�
	virtual float getFrameDelay() { return frameDelay; }

	// �J�n�t���[���̔ԍ���߂�
	virtual int getStartFrame() { return startFrame; }

	// �I���t���[���̔ԍ���߂�
	virtual int getEndFrame() { return endFrame; }

	// ���݂̃t���[���ԍ���߂�
	virtual int getCurrentFrame() { return currentFrame; }

	// Image��RECT�\���̂�߂�
	virtual RECT getSpriteDataRect() { return spriteData.rect; }

	// �A�j���[�V�����̊����̏�Ԃ�߂�
	virtual bool getAnimationComplete() { return animComplete; }

	// colorFilter��߂�
	virtual COLOR_ARGB getColorFilter() { return colorFilter; }


	// --------------------------------------------------------------
	// set�֐�
	// --------------------------------------------------------------

	// X�ʒu��ݒ�
	virtual void setX(float newX) { spriteData.x = newX; }

	// Y�ʒu��ݒ�
	virtual void setY(float newY) { spriteData.y = newY; }

	// �g��k����ݒ�
	virtual void setScale(float s) { spriteData.scale = s; }

	// ��]�p�x��ݒ�i�x�P�ʁj
	// 0�x�͏�����ł��B�p�x�͎��v���ɐi�݂܂��B
	virtual void setDegrees(float deg) { spriteData.angle = deg * ((float)PI / 180.0f); }

	// ��]�p�x��ݒ�i���W�A���P�ʁj
	// 0���W�A���͏�����ł��B�p�x�͎��v���ɐi�݂܂��B
	virtual void setRadians(float rad) { spriteData.angle = rad; }

	// ������ݒ�
	virtual void setVisible(bool v) { visible = v; }

	// �A�j���[�V�����̃t���[���Ԃ̊Ԋu��ݒ�
	virtual void setFrameDelay(float d) { frameDelay = d; }

	// �A�j���[�V�����̊J�n�t���[���ƏI���t���[����ݒ�
	virtual void setFrames(int s, int e) { startFrame = s; endFrame = e; }

	// �A�j���[�V�����̌��݂̃t���[����ݒ�
	virtual void setCurrentFrame(int c);

	// currentFrame��`�悷��spriteData.rect��ݒ�
	virtual void setRect();

	// spriteData.rect��r�ɐݒ�
	virtual void setSpriteDataRect(RECT r) { spriteData.rect = r; }

	// �A�j���[�V�������[�v��ݒ�ilp = true�̏ꍇ�Ƀ��[�v����j
	virtual void setLoop(bool lp) { loop = lp; }

	// �A�j���[�V���������u�[���l��ݒ�
	virtual void setAnimationComplete(bool a) { animComplete = a; }

	// �F�t�B���^��ݒ�iWHITE���g�p����ƁA�ω����Ȃ��j
	virtual void setcolorFilter(COLOR_ARGB color) { colorFilter = color; }


	// --------------------------------------------------------------
	// ���̑��̊֐�
	// --------------------------------------------------------------

	// Image��������
	// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
	// Graphics�ւ̃|�C���^
	// Image�̕��i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̕����g�p�j
	// Image�̍����i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̍������g�p�j
	// �e�N�X�`�����̗񐔁i1����n�܂Łj�i0��1�Ɠ����j
	// TextureManager�ւ̃|�C���^
	virtual bool initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM);

	// color���t�B���^�Ƃ��Ďg���ĉ摜��`��Bcolor�̃f�t�H���g�͔��B
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

	// �w�肳�ꂽSpriteData���g���Ă��̉摜��`��
	// ���݂�SpriteData.rect���g���āA�e�N�X�`����I��
	virtual void draw(SpriteData sd, COLOR_ARGB color);

	// �摜�𐅕������ɔ��]�i���j
	virtual void flipHorizontal(bool flip) { spriteData.flipHorizontal = flip; }

	// �摜�𐂒������ɔ��]
	virtual void flipVertical(bool flip) { spriteData.flipVertical = flip; }

	// �A�j���[�V�����̍X�V
	virtual void update(float frameTime);
};

#endif