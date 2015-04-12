#include "spaceGo.h"

// ==================================================================
// �R���X�g���N�^
// ==================================================================
SpaceGo::SpaceGo()
{}

// ==================================================================
// �f�X�g���N�^
// ==================================================================
SpaceGo::~SpaceGo()
{
	releaseAll();  // ���ׂẴO���t�B�b�N�X�A�C�e���ɂ���onLostDevice()���Ăяo��
}

// ==================================================================
// �Q�[����������
// �G���[����GameError���X���[
// ==================================================================
void SpaceGo::initialize(HWND hwnd)
{
	Game::initialize(hwnd);  // GameError���X���[
	elapsedTime = 0.0f;


	// �w�i�̃e�N�X�`��
	if(!spaceTexture.initialize(graphics, BACK_GROUND))
		throw(GameError(gameErrorNS::WARNING, "Error initializing space texture"));

	// �w�i
	if(!space.initialize(graphics, 0, 0, 0, &spaceTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));

	space.setX(GAME_WIDTH * 0.5f - space.getWidth() * 0.5f * space.getScale());
	space.setFrameDelay(30);


	// �F���D�̃e�N�X�`��
	if(!shipTexture.initialize(graphics, CHARA_MAP))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));

	// �F���D
	if(!ship.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));

	// �F���D��z�u
	ship.setX(GAME_WIDTH * 0.5f - ship.getWidth() * 0.5f * ship.getScale());
	ship.setY(GAME_HEIGHT * 0.8f - ship.getWidth() * 0.5f * ship.getScale());
	ship.setCurrentFrame(ship.getCurrentFrame());
	ship.setFrameDelay(shipNS::SHIP_ANIMATION_DELAY);


	// ��Q���̃e�N�X�`��
	if(!obstacleTexture.initialize(graphics, OBSTACLE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing obstacle texture"));

	// ��Q��
	for(int i = 0; i < 8; i++)
	{
		if(!obstacle1[i].initialize(this, obstacleNS::WIDTH, obstacleNS::HEIGHT, obstacleNS::TEXTURE_COLS, &obstacleTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing obstacle"));
		else
		{
			obstacle1[i].setCurrentFrame(4 + 29 * i);
			obstacle1[i].setX(rand() % (int)(GAME_WIDTH - obstacle1[i].getWidth()));
			obstacle1[i].setY((rand() % (int)(GAME_HEIGHT / 3)) - (GAME_HEIGHT / 3));
			obstacle1[i].setCurrentFrame(obstacle1[i].getCurrentFrame());
			obstacle1[i].setFrameDelay(obstacleNS::OBSTACLE_ANIMATION_DELAY);
		}

		if(!obstacle2[i].initialize(this, obstacleNS::WIDTH, obstacleNS::HEIGHT, obstacleNS::TEXTURE_COLS, &obstacleTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing obstacle"));
		else
		{
			obstacle2[i].setCurrentFrame(18 + 29 * i);
			obstacle2[i].setX(rand() % (int)(GAME_WIDTH - obstacle2[i].getWidth()));
			obstacle2[i].setY((rand() % (int)(GAME_HEIGHT / 3)) - (GAME_HEIGHT / 3));
			obstacle2[i].setCurrentFrame(obstacle2[i].getCurrentFrame());
			obstacle2[i].setFrameDelay(obstacleNS::OBSTACLE_ANIMATION_DELAY);
		}

		if(!obstacle3[i].initialize(this, obstacleNS::WIDTH, obstacleNS::HEIGHT, obstacleNS::TEXTURE_COLS, &obstacleTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing obstacle"));
		else
		{
			obstacle3[i].setCurrentFrame(20 + 29 * i);
			obstacle3[i].setX(rand() % (int)(GAME_WIDTH - obstacle3[i].getWidth()));
			obstacle3[i].setY((rand() % (int)(GAME_HEIGHT / 3)) - (GAME_HEIGHT / 3));
			obstacle3[i].setCurrentFrame(obstacle3[i].getCurrentFrame());
			obstacle3[i].setFrameDelay(obstacleNS::OBSTACLE_ANIMATION_DELAY);
		}
	}


	// �����̃e�N�X�`��
	for(int i = 0; i < 10; i++)
	{
		if(!numberTexture[i].initialize(graphics, NUMBER[i]))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing number texture"));
	}

	// ����
	for(int i = 0; i < 10; i++)
	{
		if(!number[i].initialize(graphics, 128, 128, 10, &numberTexture[i]))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing number"));
	}

	return;
}

// ==================================================================
// ���ׂẴQ�[���A�C�e�����X�V
// ==================================================================
void SpaceGo::update()
{
	elapsedTime += frameTime;
	score += (elapsedTime * 1.2);

	space.setFrameDelay(space.getFrameDelay() + 0.01f);
	space.setY(space.getY() + frameTime * space.getFrameDelay());

	ship.update(frameTime);
	if(elapsedTime >= 0 && elapsedTime < 15)
	{
		for(int i = 0; i < 4; i++)
			obstacle1[i].update(frameTime, space.getFrameDelay());
	}
	else if(elapsedTime >= 15 && elapsedTime < 30)
	{
		for(int i = 0; i < 8; i++)
			obstacle1[i].update(frameTime, space.getFrameDelay());
	}
	else if(elapsedTime >= 30 && elapsedTime < 45)
	{
		for(int i = 0; i < 8; i++)
			obstacle1[i].update(frameTime, space.getFrameDelay());
		for(int i = 0; i < 4; i++)
			obstacle2[i].update(frameTime, space.getFrameDelay());
	}
	else if(elapsedTime >= 45 && elapsedTime < 60)
	{
		for(int i = 0; i < 8; i++)
		{
			obstacle1[i].update(frameTime, space.getFrameDelay());
			obstacle2[i].update(frameTime, space.getFrameDelay());
		}
	}
	else if(elapsedTime >= 60 && elapsedTime < 75)
	{
		for(int i = 0; i < 8; i++)
		{
			obstacle1[i].update(frameTime, space.getFrameDelay());
			obstacle2[i].update(frameTime, space.getFrameDelay());
		}
		for(int i = 0; i < 4; i++)
			obstacle3[i].update(frameTime, space.getFrameDelay());
	}
	else if(elapsedTime >= 75)
	{
		for(int i = 0; i < 8; i++)
		{
			obstacle1[i].update(frameTime, space.getFrameDelay());
			obstacle2[i].update(frameTime, space.getFrameDelay());
			obstacle3[i].update(frameTime, space.getFrameDelay());
		}
	}
}

// ==================================================================
// �l�H�m�\
// ==================================================================
void SpaceGo::ai()
{}

// ==================================================================
// �Փ˂�����
// ==================================================================
void SpaceGo::collisions()
{
	VECTOR2 collisionVector;

	for(int i = 0; i < 8; i++)
	{
		if(ship.collidesWith(obstacle1[i], collisionVector) ||
			ship.collidesWith(obstacle2[i], collisionVector) ||
			ship.collidesWith(obstacle3[i], collisionVector))
		{
			ship.setX(ship.getX());
			ship.setY(ship.getY());
			ship.setCurrentFrame(21);
			MessageBox(hwnd, "���@�͑�j���܂����B\n�Q�[�����I�����܂��B", "Exit", MB_OK);
			PostQuitMessage(0);
			return;
		}
	}
	
}

// ==================================================================
// �Q�[���A�C�e���������_�[
// ==================================================================
void SpaceGo::render()
{
	float y = space.getY();
	if(y > space.getHeight())
		y = 0;

	graphics->spriteBegin();  // �X�v���C�g�̕`����J�n

	space.draw();
	if(space.getY() + space.getHeight() < GAME_HEIGHT)
	{
		space.setY(space.getY() + space.getHeight());
		space.draw();
		space.setY(y);
	}
	if(space.getY() > 0)
	{
		space.setY(space.getY() - space.getHeight());
		space.draw();
		space.setY(y);
	}

	ship.draw();

	for(int i = 0; i < 8; i++)
	{
		obstacle1[i].draw();
		obstacle2[i].draw();
		obstacle3[i].draw();
	}

	// �X�R�A�̕\��
	int tmp = score;
	int num;
	int count = 0;
	while(tmp > 0)
	{
		num = tmp % 10;
		number[num].setX(GAME_WIDTH - (36 * count) );
		number[num].draw();
		count++;
		tmp /= 10;
	}
	
	
		
	

	graphics->spriteEnd();    // �X�v���C�g�̕`����I��
}

// ==================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
// ==================================================================
void SpaceGo::releaseAll()
{
	spaceTexture.onLostDevice();
	shipTexture.onLostDevice();
	obstacleTexture.onLostDevice();

	Game::releaseAll();
	return;
}

// ==================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬
// ==================================================================
void SpaceGo::resetAll()
{
	spaceTexture.onResetDevice();
	shipTexture.onResetDevice();
	obstacleTexture.onResetDevice();

	Game::resetAll();
	return;
}
