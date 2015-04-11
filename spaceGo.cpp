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

	// �{�[���̃e�N�X�`��
	if(!ballTexture.initialize(graphics, BALL_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball texture"));

	// �{�[��
	if(!ball.initialize(graphics, 0, 0, 0, &ballTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball"));

	// �{�[����z�u
	ball.setX(GAME_WIDTH * 0.5f - ball.getWidth() * 0.5f * ball.getScale());
	ball.setY(GAME_HEIGHT * 0.5f - ball.getWidth() * 0.5f * ball.getScale());
	ball.setframes(0, 3);
	ball.setCurrentFrame(0);
	ball.setFrameDelay(0.2f);

	return;
}

// ==================================================================
// ���ׂẴQ�[���A�C�e�����X�V
// ==================================================================
void SpaceGo::update()
{
	static float t;
	t += frameTime;

	ball.setY(t * t * GRAVITY / 2);
	ball.setRadians(10 * t);
	if(ball.getY() > GAME_HEIGHT - ball.getHeight() * ball.getScale())
	{
		ball.setY((float)-ball.getHeight());
		t = 0;
	}

	if(input->isKeyDown(LEFT_KEY))
	{
		ball.setX(ball.getX() - frameTime * BALL_SPEED);
		if(ball.getX() < -ball.getWidth() * ball.getScale())
			ball.setX((float)GAME_WIDTH);
	}

	if(input->isKeyDown(RIGHT_KEY))
	{
		ball.setX(ball.getX() + frameTime * BALL_SPEED);
		if(ball.getX() > GAME_WIDTH)
			ball.setX((float)-ball.getWidth() * ball.getScale());
	}

	ball.update(frameTime);
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
{}

// ==================================================================
// �Q�[���A�C�e���������_�[
// ==================================================================
void SpaceGo::render()
{
	graphics->spriteBegin();  // �X�v���C�g�̕`����J�n
	ball.draw();
	graphics->spriteEnd();    // �X�v���C�g�̕`����I��
}

// ==================================================================
// �O���t�B�b�N�X�f�o�C�X�����������ꍇ
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽�߁A
// �\�񂳂�Ă����r�f�I�����������ׂĉ��
// ==================================================================
void SpaceGo::releaseAll()
{
	ballTexture.onLostDevice();
	Game::releaseAll();
	return;
}

// ==================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�ꍇ
// ���ׂẴT�[�t�F�C�X���č쐬
// ==================================================================
void SpaceGo::resetAll()
{
	ballTexture.onResetDevice();
	Game::resetAll();
	return;
}
