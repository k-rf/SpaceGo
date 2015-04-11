#include "graphics.h"

// ==================================================================
// �R���X�g���N�^
// ==================================================================
Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
	backColor = graphicsNS::BACK_COLOR;
}

// ==================================================================
// �f�X�g���N�^
// ==================================================================
Graphics::~Graphics()
{
	releaseAll();
}

// ==================================================================
// ���ׂĂ����
// ==================================================================
void Graphics::releaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

// ==================================================================
// DirectX�O���t�B�b�N�X��������
// �G���[����GameError���X���[
// ==================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	// DirectX��������
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

	initD3Dpp(); // D3D�v���[���e�[�V�����p�����[�^��������

	if(fullscreen) // �S��ʕ\�����[�h�̏ꍇ
	{
		if(isAdapterCompatible()) // �A�_�v�^�[���Ή����Ă��邩
			// �Ή����Ă��郊�t���b�V�����[�g��ݒ�
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else
			throw(GameError(gameErrorNS::FATAL_ERROR, "The graphics device does not support the specified resolution and/or format."));
	}

	// �O���t�B�b�N�X�J�[�h���n�[�h�E�F�A�e�N�X�`�������O�ƃ��C�e�B���O�A
	// ���_�V�F�[�_�[���T�|�[�g���Ă��邩�ǂ����𔻒�
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	// �f�o�C�X��HW�e�N�X�`�������O�ƃ��C�e�B���O���T�|�[�g���Ă��Ȃ��ꍇ�A
	// �܂���1.1���_�V�F�[�_�[���n�[�h�E�F�A�ŃT�|�[�g���Ă��Ȃ��ꍇ�A
	// �\�t�g�E�F�A���_�����ɐ؂�ւ��܂��B
	if((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		// �\�t�g�E�F�A�݂̂̏������g�p
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		// �n�[�h�E�F�A�݂̂̏������g�p
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// Direct3D�f�o�C�X���쐬
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&d3dpp,
		&device3d
		);

	if(FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR,
		"Error creating Direct3D device"));

	result = D3DXCreateSprite(device3d, &sprite);
	if(FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));
}

// ==================================================================
// D3D�v���[���e�[�V�����p�����[�^��������
// ==================================================================
void Graphics::initD3Dpp()
{
	try
	{
		ZeroMemory(&d3dpp, sizeof(d3dpp));             // �\���̂�0��ݒ�

		// �K�v�ȃp�����[�^��ݒ�
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if(fullscreen)                                 // �S��ʕ\���̏ꍇ
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;  // 24�r�b�g�J���[
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;   // �f�X�N�g�b�v�ݒ���g�p
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing D3D presentation parameters"));
	}
}

// ==================================================================
// �e�N�X�`�����f�t�H���g��D3D�������ɓǂݍ��ށi�ʏ�̃e�N�X�`���Ŏg�p�j
// �G���W���̎g�p���ɓ����I�ɂ̂ݎg�p���܂�
// �Q�[���e�N�X�`���̓ǂݍ��݂ɂ́ATextureManager�N���X���g�p���܂�
// ���s�O�Ffilename�́A�e�N�X�`���t�@�C���̖��O
//        transcolor�́A�����Ƃ��Ĉ����F
// ���s��Fwidth��height = �e�N�X�`���̐��@
//        texture�́A�e�N�X�`�����w��
// HRESULT��߂�
// ==================================================================
HRESULT Graphics::loadTexture(const char* filename, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture)
{
	// �t�@�C������ǂݎ�邽�߂̍\��
	D3DXIMAGE_INFO info;
	result = E_FAIL;
	try
	{
		if(filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// ���ƍ������t�@�C������擾
		result = D3DXGetImageInfoFromFile(filename, &info);
		if(result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		// �t�@�C����ǂݍ���ŁA�V�����e�N�X�`�����쐬
		result = D3DXCreateTextureFromFileEx(
			device3d,        // 3D�f�o�C�X
			filename,        // �摜�t�@�C���̖��O
			info.Width,      // �e�N�X�`���̕�
			info.Height,     // �e�N�X�`���̍���
			1,               // �~�b�v�}�b�v�̃��x���i�`�F�[���Ȃ��̏ꍇ��1�j
			0,               // �g�p���@�iUsage�j
			D3DFMT_UNKNOWN,  // �T�[�t�F�C�X�t�H�[�}�b�g�i�f�t�H���g�j
			D3DPOOL_DEFAULT, // �e�N�X�`���p�̃������̎��
			D3DX_DEFAULT,    // �摜�t�B���^
			D3DX_DEFAULT,    // �~�b�v�t�B���^
			transcolor,      // �����p�̐F�L�[
			&info,           // �r�b�g�}�b�v�t�@�C�����i�ǂݍ��񂾃t�@�C������j
			NULL,            // �J���[�p���b�g
			&texture         // �ړI�̃e�N�X�`��
			);
	}
	catch(...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return result;
}

// ==================================================================
// SpriteData�\���̂ɋL�q���ꂽ�X�v���C�g��`��
// color�i�I�v�V�����j�́A�t�B���^�̂悤�ɓK�p�����
// �f�t�H���g�͔��i�ω��Ȃ��j
// ���s�O�Fsprite->Begin()���Ăяo��
// ���s��Fsprite->End()���Ăяo��
//        spriteData.rect�́A�`�悷��spriteData.texture�̕������`
//        spriteData.rect.right�́A�E�[ + 1�ɂ���K�v������
//        spriteData.rect.bottom�́A���[ + 1�ɂ���K�v������
// ==================================================================
void Graphics::drawSprite(const SpriteData& spriteData, COLOR_ARGB color)
{
	if(spriteData.texture == NULL)  // �e�N�X�`�����Ȃ��ꍇ
		return;

	// �X�v���C�g�̒��S�����
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2(
		(float)(spriteData.width / 2 * spriteData.scale),
		(float)(spriteData.height / 2 * spriteData.scale)
		);

	// �X�v���C�g�̉�ʈʒu
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);

	// X������Y�����̊g��k��
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	if(spriteData.flipHorizontal)      // ���������ɔ��]����ꍇ
	{
		scaling.x *= -1;               // X�X�P�[���𕉂ɂ��Ĕ��]

		// ���]���ꂽ�摜�̒��S���擾
		spriteCenter.x -= (float)(spriteData.width * spriteData.scale);

		// ���[�𒆐S�ɔ��]���N����̂ŁA���]���ꂽ�摜��
		// ���Ɠ����ʒu�ɔz�u���邽�߂ɁA�E�ֈړ����܂�
		translate.x += (float)(spriteData.width * spriteData.scale);
	}

	if(spriteData.flipVertical)        // ���������ɔ��]����ꍇ
	{
		scaling.y *= -1;               // Y�X�P�[���𕉂ɂ��Ĕ��]

		// ���]���ꂽ�摜�̒��S���擾
		spriteCenter.y -= (float)(spriteData.height * spriteData.scale);

		// ��[�𒆐S�ɔ��]���N����̂ŁA���]���ꂽ�摜��
		// ���Ɠ����ʒu�ɔz�u���邽�߂ɁA���ֈړ����܂�
		translate.y += (float)(spriteData.height * spriteData.scale);
	}

	// �X�v���C�g�̉�]�A�g��k���A�z�u���s�����߂̍s����쐬
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,                  // �s��
		NULL,                     // �g��k�����s���Ƃ��A���_�͍���̂܂�
		0.0f,                     // �g��k�����̉�]�Ȃ�
		&scaling,                 // �g��k���̔{��
		&spriteCenter,            // ��]�̒��S
		(float)(spriteData.angle),// ��]�̊p�x
		&translate                // X,Y�̈ʒu
		);

	// �s����X�v���C�g�ɓK�p
	sprite->SetTransform(&matrix);

	// �X�v���C�g��`��
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

// ==================================================================
// �o�b�N�o�b�t�@��\��
// ==================================================================
HRESULT Graphics::showBackbuffer()
{
	result = E_FAIL; // ���s���f�t�H���g�Ƃ��A�������ɒu������

	// �o�b�N�o�b�t�@����ʂɕ\��
	result = device3d->Present(NULL, NULL, NULL, NULL);
	return result;
}

// ==================================================================
// �A�_�v�^�[���`�F�b�N���āAd3dpp�Ŏw�肳�ꂽBackBuffer�̍����A���A
// ���t���b�V�����[�g�ɑΉ����Ă��邩�ǂ������m�F���܂��B
// �Ή�������̂����������ꍇ�́ApMode�\���̂ɁA
// �Ή����Ă��郂�[�h�̃t�H�[�}�b�g��ݒ肵�܂��B
// ���s�O�Fd3dpp������������Ă���
// ���s��F�Ή����Ă��郂�[�h��������ApMode�\���̂Ƀf�[�^��
// �ݒ肳�ꂽ�ꍇ�Atrue��߂��܂��B
// �Ή����Ă��郂�[�h��������Ȃ������ꍇ�Afalse��߂��܂��B
// ==================================================================
bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat);

	for(UINT i = 0; i < modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &pMode);
		if(pMode.Height == d3dpp.BackBufferHeight && pMode.Width == d3dpp.BackBufferWidth && pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}

// ==================================================================
// �f�o�C�X���������Ă��Ȃ������`�F�b�N
// ==================================================================
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;  // ���s���f�t�H���g�Ƃ��A�������ɒu������
	if(device3d == NULL)
		return result;
	result = device3d->TestCooperativeLevel();
	return result;
}

// ==================================================================
// �O���t�B�b�N�X�f�o�C�X�����Z�b�g
// ==================================================================
HRESULT Graphics::reset()
{
	result = E_FAIL;  // ���s���f�t�H���g�Ƃ��A�������ɒu������
	initD3Dpp();      // D3D�v���[���e�[�V�����p�����[�^��������

	// �O���t�B�b�N�X�f�o�C�X�̃��Z�b�g�����݂�
	result = device3d->Reset(&d3dpp);
	return result;
}

// ==================================================================
// �o�b�N�o�b�t�@���N���A���āADirectX��BeginScene()���Ăяo��
// ==================================================================
HRESULT Graphics::beginScene()
{
	result = E_FAIL;
	if(device3d == NULL)
		return result;

	// �o�b�N�o�b�t�@��backColor�ŃN���A����
	device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0f, 0);
	result = device3d->BeginScene();  // �`��̂��߂̃V�[�����J�n����
	return result;
}

// ==================================================================
// DirectX��EndScene()���Ăяo��
// ==================================================================
HRESULT Graphics::endScene()
{
	result = E_FAIL;
	if(device3d)
		result = device3d->EndScene();
	return result;
}

