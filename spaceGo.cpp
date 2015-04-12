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
	ship.setY(GAME_HEIGHT * 0.5f - ship.getWidth() * 0.5f * ship.getScale());
	//ship.setFrames(shipNS::SHIP_START_FRAME, shipNS::SHIP_END_FRAME);  
	ship.setCurrentFrame(ship.getCurrentFrame());
	ship.setFrameDelay(shipNS::SHIP_ANIMATION_DELAY);

	return;
}

// ==================================================================
// ���ׂẴQ�[���A�C�e�����X�V
// ==================================================================
void SpaceGo::update()
{
	space.setFrameDelay(space.getFrameDelay() + 0.01f);
	space.setY(space.getY() + frameTime * space.getFrameDelay());
	
	ship.update(frameTime);
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
	Game::resetAll();
	return;
}
