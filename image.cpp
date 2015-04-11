#include "image.h"

// ==================================================================
// �f�t�H���g�R���X�g���N�^
// ==================================================================
Image::Image()
{
	initialized = false;
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0;
	animTimer = 0.0;
	visible = true;
	loop = true;
	animComplete = false;
	graphics = NULL;
	colorFilter = graphicsNS::WHITE;
}

// ==================================================================
// �f�X�g���N�^
// ==================================================================
Image::~Image()
{}

// ==================================================================
// Image��������
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
// Graphics�ւ̃|�C���^
// Image�̕��i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̕����g�p�j
// Image�̍����i�s�N�Z���P�ʁj�i0 = �e�N�X�`���S�̂̍������g�p�j
// �e�N�X�`�����̗񐔁i1����n�܂Łj�i0��1�Ɠ����j
// TextureManager�ւ̃|�C���^
// ==================================================================
bool Image::initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM)
{
	try
	{
		graphics = g;                  // grahpics�I�u�W�F�N�g
		textureManager = textureM;     // �e�N�X�`���I�u�W�F�N�g�ւ̃|�C���^

		spriteData.texture = textureManager->getTexture();

		if(width == 0)
			width = textureManager->getWidth();  // �S�̂̕����g�p
		spriteData.width = width;

		if(height == 0)
			height = textureManager->getHeight();  // �S�̂̍������g�p
		spriteData.height = height;

		cols = ncols;
		if(cols == 0)
			cols = 1;  // cols��0�̏ꍇ�́A1���g�p

		// currentFrame��`�悷��spriteData.rect���\��
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		// �E�[�@+ 1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame % cols) * spriteData.height;
		// ���[ + 1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch(...)
	{
		return false;
	}

	initialized = true;  // ����ɏ��������ꂽ�ꍇ

	return true;
}

// ==================================================================
// color���t�B���^�Ƃ��Ďg���ĉ摜��`��
// color�p�����[�^�̓I�v�V����
// �iimage.h�ŁA�f�t�H���g�Ƃ��Ĕ������蓖�Ă��Ă���j
// ���s�O�FspriteBegin()���Ăяo��
// ���s��FspriteEnd()���Ăяo��
// ==================================================================
void Image::draw(COLOR_ARGB color)
{
	if(!visible || graphics == NULL)
		return;

	// onReset()���Ăяo���ꂽ�Ƃ��ɐV�����e�N�X�`�����擾
	spriteData.texture = textureManager->getTexture();

	if(color == graphicsNS::FILTER)  // �t�B���^���g���ĕ`�悷��ꍇ
		// colorFilter���g�p
		graphics->drawSprite(spriteData, colorFilter);
	else
		// �t�B���^�Ƃ���color���g�p
		graphics->drawSprite(spriteData, color);
}

// ==================================================================
// �w�肳�ꂽSpriteData���g���Ă��̉摜��`��
// ���݂�SpriteData.rect���g���āA�e�N�X�`����I��
// ���s�O�FspriteBegin()���Ăяo��
// ���s��FspriteEnd()���Ăяo��
// ==================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
	if(!visible || graphics == NULL)
		return;

	// ���̉摜�̋�`���g���ăe�N�X�`����I��
	sd.rect = spriteData.rect;

	// onReset()���Ăяo���ꂽ�Ƃ��ɐV�����e�N�X�`�����擾
	sd.texture = textureManager->getTexture();

	if(color == graphicsNS::FILTER)              // �t�B���^���g���ĕ`�悷��ꍇ
		graphics->drawSprite(sd, colorFilter);   // colorFilter���g�p
	else
		graphics->drawSprite(sd, color);         // �t�B���^�Ƃ���color���g�p
}

// ============================================================================
// �X�V
// �ʏ�1�t���[����1��Ăяo�����
// frameTime�͓���ƃA�j���[�V�����̑��x���Ǘ�����̂ɗp������
// ============================================================================
void Image::update(float frameTime)
{
	if(endFrame - startFrame > 0)
	{
		animTimer += frameTime;
		if(animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if(currentFrame < startFrame || currentFrame > endFrame)
			{
				if(loop == true)
					currentFrame = startFrame;
				else
				{
					currentFrame = endFrame;
					animComplete = true;
				}
			}
			setRect();
		}
	}
}

// ============================================================================
// �e�N�X�`���̌��݂̃t���[����ݒ�
// ============================================================================
void Image::setCurrentFrame(int c)
{
	if(c >= 0)
	{
		currentFrame = c;
		animComplete = false;
		setRect();
	}
}

// ============================================================================
// ���݂̃t���[���ɕ`�悷�邽�߂�spriteData.rect��ݒ�
// ============================================================================
inline void Image::setRect()
{
	// currentFrame��`�悷��spriteData.rect���\��
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	// �E�[�@+ 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame % cols) * spriteData.height;
	// ���[ + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}