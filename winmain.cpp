#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <ctime>
#include "spaceGo.h"

// �֐��v���g�^�C�v
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND&, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

// �Q�[���|�C���^
SpaceGo* game = NULL;
HWND hwnd = NULL;

// ============================================================================
// Windows�A�v���P�[�V�����̊J�n�_
// �p�����[�^
//     hInstance    �F�A�v���P�[�V�����̌��݂̃C���X�^���X�ւ̃n���h��
//     hPrevInstance�F���NULL�i�p�~���ꂽ�p�����[�^�j
//     lpCmdLine    �F�R�}���h���C��������NULL�I�[������ւ̃|�C���^
//     nCmdShow     �F�E�B���h�E���ǂ̂悤�ɕ\�����邩���w��
// ============================================================================
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// �f�o�b�O�r���h�̏ꍇ�A���������[�N���`�F�b�N
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	srand((unsigned)time(NULL));

	MSG msg;

	// �Q�[���̍쐬�A���b�Z�[�W�n���h���̐ݒ�
	game = new SpaceGo;

	// �E�B���h�E���쐬
	if(!CreateMainWindow(hwnd, hInstance, nCmdShow))
		return 1;

	try
	{
		game->initialize(hwnd);

		// ���C���̃��b�Z�[�W���[�v
		int done = 0;
		while(!done)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
					done = 1;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				game->run(hwnd);  // �Q�[�����[�v
		}
		SAFE_DELETE(game);  // �I���̑O�Ƀ����������
		return msg.wParam;
	}
	catch(const GameError& err)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, err.getMessage(), "Error", MB_OK);
	}
	catch(...)
	{
		game->deleteAll();
		DestroyWindow(hwnd);
		MessageBox(NULL, "Unknown error occured in game.", "Error", MB_OK);
	}

	SAFE_DELETE(game);  // �I���̑O�Ƀ����������
	return 0;
}

// ============================================================================
// �E�B���h�E�C�x���g�R�[���o�b�N�֐�
// ============================================================================
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->messageHandler(hWnd, msg, wParam, lParam);
}

// ============================================================================
// �E�B���h�E���쐬
// �߂�l�F�G���[�̏ꍇ�Afalse
// ============================================================================
bool CreateMainWindow(HWND& hwnd, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcx;

	// �E�B���h�E�N���X�̍\���̂����C���E�B���h�E���L�q����p�����[�^�Őݒ肵�܂�
	wcx.cbSize = sizeof(wcx);                    // �\���̂̃T�C�Y
	wcx.style = CS_HREDRAW | CS_VREDRAW;         // �E�B���h�E�T�C�Y�ύX���ɍĕ`��
	wcx.lpfnWndProc = WinProc;                   // �E�B���h�E�v���V�[�W�����w��
	wcx.cbClsExtra = 0;                          // �g���N���X�������Ȃ�
	wcx.cbWndExtra = 0;                          // �g���E�B���h�E�������Ȃ�
	wcx.hInstance = hInstance;                   // �C���X�^���X�ւ̃n���h��
	wcx.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);   // ���O��`����Ă�����J�[�\��
	wcx.hbrBackground =
		(HBRUSH)GetStockObject(BLACK_BRUSH);     // �w�i�u���V
	wcx.lpszMenuName = NULL;                     // ���j���[���\�[�X�̖��O
	wcx.lpszClassName = CLASS_NAME;              // �E�B���h�E�̃N���X�̖��O
	wcx.hIconSm = NULL;                          // �������A�C�R��

	// �E�B���h�E�N���X��o�^
	// �G���[���ARegisterClassEx��0��߂�
	if(RegisterClassEx(&wcx) == 0)
		return false;

	// �E�B���h�E���쐬
	hwnd = CreateWindow(
		CLASS_NAME,                    // �E�B���h�E�N���X�̖��O
		APP_TITLE,                     // �^�C�g���o�[�̃e�L�X�g
		WS_OVERLAPPEDWINDOW,           // �E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,                 // �E�B���h�E�̐����ʒu�̃f�t�H���g
		CW_USEDEFAULT,                 // �E�B���h�E�̐����ʒu�̃f�t�H���g
		GAME_WIDTH,                    // �E�B���h�E�̕�
		GAME_HEIGHT,                   // �E�B���h�E�̍���
		(HWND)NULL,                    // �e�E�B���h�E�Ȃ�
		(HMENU)NULL,                   // ���j���[�Ȃ�
		hInstance,                     // �A�v���P�[�V�����C���X�^���X�ւ̃n���h��
		(LPVOID)NULL                   // �E�B���h�E�p�����[�^�Ȃ�
		);

	// �E�B���h�E�쐬�ŃG���[�����������ꍇ
	if(!hwnd)
		return false;

	// �E�B���h�E��\��
	ShowWindow(hwnd, nCmdShow);

	// �E�B���h�E�v���V�[�W����WM_PAINT���b�Z�[�W�𑗂�
	UpdateWindow(hwnd);
	return true;
}