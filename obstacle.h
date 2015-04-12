#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace obstacleNS
{
	const int WIDTH = 32;                        // ��
	const int HEIGHT = 32;                       // ����
	const int X = GAME_WIDTH / 2 - WIDTH / 2;    // �X�^�[�g�ʒu
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 0;                       // �P�O�O�s�N�Z�����b
	const float MASS = 300.0f;                   // ����
	const int TEXTURE_COLS = 29;
	const int OBSTACLE_START_FRAME = 0;              // Ship�̍ŏ��̉摜
	const int OBSTACLE_END_FRAME = 0;                // Ship�A�j���[�V�����̃t���[����
	const float OBSTACLE_ANIMATION_DELAY = 0.2F;     // �t���[���Ԃ̎���
}

class Obstacle : public Entity
{
public:
	// �R���X�g���N�^
	Obstacle();

	virtual ~Obstacle() {}

	void update(float frameTime, float bg);
};

#endif
