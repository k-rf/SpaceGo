#include "game.h"

// ==================================================================
// �R���X�g���N�^
// ==================================================================
Game::Game()
{
	input = new Input();  // �L�[�{�[�h���͂𑦎��ɏ�����

	// ���̑��̏������́A���
	// input->initialize()���Ăяo���ď���
	paused = false;  // �Q�[���͈ꎞ��~���ł͂Ȃ�
	graphics = NULL;
	initialized = false;
}

// ==================================================================
// �f�X�g���N�^
// ==================================================================
Game::~Game()
{
	deleteAll();      // �\�񂳂�Ă��������������ׂĉ��
	ShowCursor(true); // �J�[�\����\��
}

// ==================================================================
// Windows���b�Z�[�W�n���h��
// ==================================================================
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(initialized)  // ����������Ă��Ȃ��ꍇ�̓��b�Z�[�W���������Ȃ�
	{
		switch(msg)
		{
		case WM_DESTROY:
			// Windows�ɂ��̃v���O�������I������悤�ɓ`����
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:        // �L�[�������ꂽ
			input->keyDown(wParam);
			return 0;

		case WM_KEYUP:
		case WM_SYSKEYUP:          // �L�[�������ꂽ
			input->keyUp(wParam);
			return 0;

		case WM_CHAR:              // ���������͂��ꂽ
			input->keyIn(wParam);
			return 0;

		case WM_MOUSEMOVE:         // �}�E�X���ړ����ꂽ
			input->mouseIn(lParam);
			return 0;

		case WM_INPUT:             // �}�E�X����̃��[�f�[�^����
			input->mouseRawIn(lParam);
			return 0;

		case WM_LBUTTONDOWN:       // ���}�E�X�{�^���������ꂽ
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_LBUTTONUP:         // ���}�E�X�{�^���������ꂽ
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_MBUTTONDOWN:       // �����}�E�X�{�^���������ꂽ
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_MBUTTONUP:         // �����}�E�X�{�^���������ꂽ
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_RBUTTONDOWN:       // �E�}�E�X�{�^���������ꂽ
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_RBUTTONUP:         // �E�}�E�X�{�^���������ꂽ
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_XBUTTONDOWN:       // �}�E�X��X�{�^���������ꂽ
		case WM_XBUTTONUP:         //                �����ꂽ
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return 0;

		case WM_DEVICECHANGE:      // �R���g���[�����`�F�b�N
			input->checkControllers();
			return 0;
		}
	}

	// Windows�ɏ�����C����
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// ==================================================================
// �Q�[����������
// �G���[����GameError���X���[
// ==================================================================
void Game::initialize(HWND hw)
{
	hwnd = hw;  // �E�B���h�E�n���h����ۑ�

	// �O���t�B�b�N��������
	graphics = new Graphics();

	// GameError���X���[
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	// ���͂��������A�}�E�X�̓L���v�`�����Ȃ�
	input->initialize(hwnd, false);  // GameError���X���[

	// ������\�^�C�}�[�̏��������݂�
	if(QueryPerformanceFrequency(&timeFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
	QueryPerformanceCounter(&timeStart);  // �J�n���Ԃ��擾
	initialized = true;
}

// ==================================================================
// ���������O���t�B�b�N�X�f�o�C�X������
// ==================================================================
void Game::handleLostGraphicsDevice()
{
	// �f�o�C�X�̏������e�X�g���A����ɉ����ď��������s
	hr = graphics->getDeviceState();
	if(FAILED(hr))                   // �O���t�B�b�N�X�f�o�C�X���L���ȏ�ԂłȂ��ꍇ
	{
		// �f�o�C�X���������Ă���A���Z�b�g�ł����ԂɂȂ��ꍇ
		if(hr == D3DERR_DEVICELOST)
		{
			Sleep(100);              // CPU���Ԃ𖾂��n��(100�~���b)
			return;
		}

		// �f�o�C�X���������Ă��邪�A���Z�b�g�ł����Ԃɂ���ꍇ
		else if(hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();  // �O���t�B�b�N�X�f�o�C�X�̃��Z�b�g�����݂�
			if(FAILED(hr))           // ���Z�b�g�����s�����ꍇ
				return;
			resetAll();
		}
		else
			return;                  // ���̃f�o�C�X�G���[
	}
}

// ==================================================================
// �Q�[���A�C�e���������_�[
// ==================================================================
void Game::renderGame()
{
	// �����_�����O���J�n
	if(SUCCEEDED(graphics->beginScene()))
	{
		// render�́A�p�������N���X���ŋL�q����K�v�̂��鏃�����z�֐��ł�
		render();  // �h���N���X��render()���Ăяo��

		// �����_�����O���I��
		graphics->endScene();
	}
	handleLostGraphicsDevice();

	// �o�b�N�o�b�t�@����ʂɕ\��
	graphics->showBackbuffer();
}

// ==================================================================
// WinMain���̃��C���̃��b�Z�[�W���[�v�ŌJ��Ԃ��Ăяo�����
// ==================================================================
void Game::run(HWND hwnd)
{
	if(graphics == NULL)  // �O���t�B�b�N�X������������Ă��Ȃ��ꍇ
		return;

	// �G�X�P�[�v�L�[�ŏI��
	if(input->isKeyDown(ESC_KEY))
	{
		PostQuitMessage(0);
		return;
	}

	// �Ō�̃t���[������̌o�ߎ��Ԃ��v�Z�AframeTime�ɕۑ�
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timeFreq.QuadPart;

	// �ȓd�̓R�[�h(winmm.lib���K�v)
	// ��]����t���[�����[�g�ɑ΂��Čo�ߎ��Ԃ��Z���ꍇ
	if(frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);  // 1�~���b�̕���\��Windows�^�C�}�[�ɗv��
		Sleep(sleepTime);    // sleepTime�̊ԁACPU�����
		timeEndPeriod(1);    // 1�~���b�̃^�C�}�[����\���I��
		return;
	}

	if(frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime);  // ����fps

	if(frameTime > MAX_FRAME_TIME)        // �t���[�����[�g�����ɒx���ꍇ
		frameTime = MAX_FRAME_TIME;       // �ő�frameTime�𐧌�

	timeStart = timeEnd;

	input->readControllers();             // �R���g���[���̏�Ԃ�ǂݎ��

	// update()�Aai()�Acollisions()�͏������z�֐��ł�
	// �����̊֐��́AGame���p�����Ă���N���X���ŋL�q����K�v������܂�
	if(!paused)                           // �ꎞ��~���łȂ�
	{
		update();                         // ���ׂẴQ�[���A�C�e�����X�V
		ai();                             // �l�H�m�\
		collisions();                     // �Փ˂�����
		input->vibrateControllers(frameTime);  //�R���g���[���̐U��������
	}
	renderGame();                         // ���ׂẴQ�[���A�C�e����`��

	// ���͂��N���A
	// ���ׂẴL�[�`�F�b�N���s��ꂽ�ケ����Ăяo��
	input->clear(inputNS::KEYS_PRESSED);
}

// ============================================================================
// �O���t�B�b�N�X�f�o�C�X�̏���
// ���ׂẴr�f�I��������������ăO���t�B�b�N�X�f�o�C�X�����Z�b�g����
// ============================================================================
void Game::releaseAll()
{}

// ============================================================================
// ���ׂă��Z�b�g����
// ============================================================================
void Game::resetAll()
{}

// ============================================================================
// ���ׂẴ��������폜����
// ============================================================================
void Game::deleteAll()
{
	releaseAll();
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	initialized = false;
}
