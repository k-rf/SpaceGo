#ifndef _SPACEGO_H
#define _SPACEGO_H

#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "ship.h"

// ==================================================================
// ���̃N���X�̓Q�[���G���W���̃R�A
// ==================================================================
class SpaceGo : public Game
{
	// �Q�[���A�C�e��
	TextureManager shipTexture;
	TextureManager spaceTexture;
	Ship ship;
	Image space;

public:
	// �R���X�g���N�^
	SpaceGo();

	// �f�X�g���N�^
	virtual ~SpaceGo();

	// �Q�[����������
	void initialize(HWND hwnd);
	void update();      // Game����̏������z�֐����I�[�o�[���C�h����K�v������
	void ai();          // �V
	void collisions();  // �V
	void render();      // �V
	void releaseAll();
	void resetAll();
};

#endif