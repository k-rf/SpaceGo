#ifndef _SHIP_H
#define _SHIP_H

#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace shipNS
{
	const int WIDTH = 32;                        // ��
	const int HEIGHT = 32;                       // ����
	const int X = GAME_WIDTH / 2 - WIDTH / 2;    // �X�^�[�g�ʒu
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 2.0f;                     // �P�O�O�s�N�Z�����b
	const float MAX_SPEED = 10;
	const float MASS = 300.0f;                   // ����
	const int TEXTURE_COLS = 8;
	const int SHIP_START_FRAME = 0;              // Ship�̍ŏ��̉摜
	const int SHIP_END_FRAME = 4;                // Ship�A�j���[�V�����̃t���[����
	const float SHIP_ANIMATION_DELAY = 0.2F;     // �t���[���Ԃ̎���
}

class Ship : public Entity
{
public:
	// �R���X�g���N�^
	Ship();

	void update(float frameTime);
};

#endif