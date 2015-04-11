#include "entity.h"

// ============================================================================
// �~�̏Փ˔��胁�\�b�h
// collision()�ɂ���ČĂяo�����A�f�t�H���g�̏Փ˔��胁�\�b�h
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
// ============================================================================
bool Entity::collideCircle(Entity& ent, VECTOR2& collisionVector)
{
	// ���S�ƒ��S�̊Ԃ̍�
	distSquared = *getCenter() - *ent.getCenter();
	distSquared.x = distSquared.x * distSquared.x;  // �����Q��
	distSquared.y = distSquared.y * distSquared.y;

	// ���a�̍��v���v�Z�i�g��k���̔{���𒲐��j
	sumRadiiSquared = (radius * getScale()) + (ent.radius * ent.getScale());
	sumRadiiSquared *= sumRadiiSquared;  // �Q�悷��

	// �G���e�B�e�B���Փ˂��Ă���ꍇ
	if(distSquared.x + distSquared.y <= sumRadiiSquared)
	{
		// �Փ˃x�N�g����ݒ�
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;  // �Փ˂��Ă��Ȃ��ꍇ
}

// ============================================================================
// �����s���E�{�b�N�X�̏Փ˔��胁�\�b�h
// collision()�ɂ���ČĂяo�����
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
// ============================================================================
bool Entity::collideBox(Entity& ent, VECTOR2& collisionVector)
{
	// �ǂ��炩�̃G���e�B�e�B���A�N�e�B�u�łȂ��ꍇ�A�Փ˂͋N����Ȃ�
	if(!active || !ent.getActive())
		return false;

	// �����s���E�{�b�N�X���g���ďՓ˂��`�F�b�N
	if((getCenterX() + edge.right * getScale() >=
		ent.getCenterX() + ent.getEdge().left * ent.getScale()) &&
		(getCenterX() + edge.left * getScale() <=
		ent.getCenterX() + ent.getEdge().right * ent.getScale()) &&
		(getCenterY() + edge.bottom * getScale() >=
		ent.getCenterY() + ent.getEdge().top * ent.getScale()) &&
		(getCenterY() + edge.top * getScale() <=
		ent.getCenterY() + ent.getEdge().bottom * ent.getScale()))
	{
		// �Փ˃x�N�g����ݒ�
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}

// ============================================================================
// ��]����{�b�N�X�̏Փ˔��胁�\�b�h
// collision()�ɂ���ČĂяo�����
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false
// �@�@�@�@�Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
// �������e�X�g���g�p���ďՓ˂𔻒�
// �������e�X�g�F
// �Q�̃{�b�N�X�́A����̒����ւ̓��e���d�Ȃ��Ă��Ȃ���ΏՓ˂��Ă��Ȃ�
// ============================================================================
bool Entity::collideRotatedBox(Entity& ent, VECTOR2& collisionVector)
{
	computeRotatedBox();     // ��]����{�b�N�X������
	ent.computeRotatedBox(); // ��]����{�b�N�X������

	if(projectionsOverlap(ent) && ent.projectionsOverlap(*this))
	{
		// �Փ˃x�N�g����ݒ�
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}

// ============================================================================
// ��]����{�b�N�X�Ɖ~�̏Փ˔��胁�\�b�h
// collision()�ɂ���ČĂяo�����
// �{�b�N�X�̕ӂƉ~�̔��a�ŕ������e�X�g���g�p
// �~�̒��S���Փ˃{�b�N�X�̊e�ӂ�L�΂��������̊O��
// �i�{���m�C�̈�ƌĂ΂��j�ɂ���ꍇ�A�{�b�N�X�̂����Ƃ��߂����_��
// �Փ˂��Ȃ����������̃`�F�b�N�Ŕ��肵�܂��B
// �ł��߂����_�́A�d�Ȃ�̃e�X�g���画�f�ł��܂��B
//
// Voronoi0 |   | Voronoi1
//       ---0---1---
//          |   |
//       ---3---2---
// Voronoi3 |   | Vronoi2
//
// ���s�O�F���̃G���e�B�e�B�͕K���{�b�N�X�A����̃G���e�B�e�B�ient�j�͕K���~
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
// ============================================================================
bool Entity::collideRotatedBoxCircle(Entity& ent, VECTOR2& collisionVector)
{
	float min01, min03, max01, max03, center01, center03;
	computeRotatedBox();  // ��]����{�b�N�X������

	// �~�̒��S��edge01�ɓ��e
	center01 = graphics->Vector2Dot(&edge01, ent.getCenter());
	// �ŏ��l�ƍő�l�͒��S����̔��a
	min01 = center01 - ent.getRadius() * ent.getScale();
	max01 = center01 + ent.getRadius() * ent.getScale();

	if(min01 > edge01Max || max01 < edge01Min)  // ���e���d�Ȃ�Ȃ��ꍇ
		return false;  // �Փ˂̉\���Ȃ�

	// �~�̒��S��edge03�ɓ��e
	center03 = graphics->Vector2Dot(&edge03, ent.getCenter());
	// �ŏ��l�ƍő�l�͒��S����̔��a
	min03 = center03 - ent.getRadius() * ent.getScale();
	max03 = center03 + ent.getRadius() * ent.getScale();
	if(min03 > edge03Max || max03 < edge03Min)  // ���e���d�Ȃ�Ȃ��ꍇ
		return false;  // �Փ˂̉\���Ȃ�

	// �~�̓��e���{�b�N�X�̓��e�ɏd�Ȃ�ꍇ
	// �~���Փ˃{�b�N�X�̃{���m�C�̈�ɂ��邩�ǂ������`�F�b�N

	// ���S��Voronoi0�ɂ���ꍇ
	if(center01 < edge01Min && center03 < edge03Min)
		return collideCornerCircle(corners[0], ent, collisionVector);
	// ���S��Voronoi1�ɂ���ꍇ
	if(center01 > edge01Max && center03 < edge03Min)
		return collideCornerCircle(corners[1], ent, collisionVector);
	// ���S��Voronoi2�ɂ���ꍇ
	if(center01 > edge01Max && center03 > edge03Max)
		return collideCornerCircle(corners[2], ent, collisionVector);
	// ���S��Voronoi3�ɂ���ꍇ
	if(center01 < edge01Max && center03 > edge03Max)
		return collideCornerCircle(corners[3], ent, collisionVector);

	// �~���{���m�C�̈�ɂȂ��ꍇ�A�~�̓{�b�N�X�̕ӂƏՓ˂��Ă���
	// �Փ˃x�N�g����ݒ�A�P���ɁA�~�̒��S����{�b�N�X�̒��S�܂ł��g�p
	collisionVector = *ent.getCenter() - *getCenter();

	return true;
}

// ============================================================================
// �{�b�N�X�̒��_���~�ƏՓ˂��Ă��Ȃ������A�����̃`�F�b�N���g�p���Ĕ���
// collideRotatedBoxCircle()�ɂ���ČĂяo�����
// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
// ============================================================================
bool Entity::collideCornerCircle(VECTOR2 corner, Entity& ent, VECTOR2& collisionVector)
{
	distSquared = corner - *ent.getCenter();               // ���_ - �~
	distSquared.x = distSquared.x * distSquared.x;         // �����Q��
	distSquared.y = distSquared.y * distSquared.y;

	// ���a�̍��v���v�Z���Ă���A������Q��
	sumRadiiSquared = ent.getRadius() * ent.getScale();    // �i�O + �~�̔��a�j
	sumRadiiSquared *= sumRadiiSquared;                    // �Q�悷��

	// ���_�Ɖ~���Փ˂��Ă���ꍇ
	if(distSquared.x + distSquared.y <= sumRadiiSquared)
	{
		// �Փ˃x�N�g����ݒ�
		collisionVector = *ent.getCenter() - corner;
		return true;
	}

	return false;
}

// ============================================================================
// ����̃{�b�N�X���A���̃G���e�B�e�B��edge01�����edge03�ɓ��e
// collideRotatedVox()�ɂ���ČĂяo�����
// ���s��F���e���d�Ȃ��Ă���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
// ============================================================================
bool Entity::projectionsOverlap(Entity& ent)
{
	float projection, min01, max01, min03, max03;

	// ����̃{�b�N�X��edge01�ɓ��e
	projection = graphics->Vector2Dot(&edge01, ent.getCorner(0));

	// ���_0�𓊉e
	min01 = projection;
	max01 = projection;

	// �c��̒��_���ꂼ�������
	for(int c = 1; c < 4; c++)
	{
		// ���_��edge01�ɓ��e
		projection = graphics->Vector2Dot(&edge01, ent.getCorner(c));
		if(projection < min01)
			min01 = projection;
		else if(projection > max01)
			max01 = projection;
	}

	// ���e���d�Ȃ�Ȃ��ꍇ
	if(min01 > edge01Max || max01 < edge01Min)
		return false;  // �Փ˂̉\���Ȃ�

	// ����̃{�b�N�X��edge03�ɓ��e
	projection = graphics->Vector2Dot(&edge03, ent.getCorner(0));

	// ���_0�𓊉e
	min03 = projection;
	max03 = projection;

	// �c��̒��_���ꂼ�������
	for(int c = 1; c < 4; c++)
	{
		// ���_��edge03�ɓ��e
		projection = graphics->Vector2Dot(&edge03, ent.getCorner(c));
		if(projection < min03)
			min03 = projection;
		else if(projection > max03)
			max03 = projection;
	}

	// ���e���d�Ȃ�Ȃ��ꍇ
	if(min03 > edge03Max || max03 < edge03Min)
		return false;  // �Փ˂̉\���Ȃ�
	return true;       // ���e���d�Ȃ��Ă���ꍇ
}

// ============================================================================
// ��]����{�b�N�X�̒��_�A���e���A���e�̍ŏ��l�ƍő�l���v�Z
// 0---1 ���_�̔ԍ�
// |   |
// 3---2
// ============================================================================
void Entity::computeRotatedBox()
{
	if(rotatedBoxReady)
		return;

	float projection;
	VECTOR2 rotatedX(cos(spriteData.angle), sin(spriteData.angle));
	VECTOR2 rotatedY(-sin(spriteData.angle), cos(spriteData.angle));
	const VECTOR2* center = getCenter();

	corners[0] = *center +
		rotatedX * ((float)edge.left * getScale()) +
		rotatedY * ((float)edge.top * getScale());
	corners[1] = *center +
		rotatedX * ((float)edge.right * getScale()) +
		rotatedY * ((float)edge.top * getScale());
	corners[2] = *center +
		rotatedX * ((float)edge.right * getScale()) +
		rotatedY * ((float)edge.bottom * getScale());
	corners[3] = *center +
		rotatedX * ((float)edge.left * getScale()) +
		rotatedY * ((float)edge.bottom * getScale());

	// corners[0]����_�Ƃ��Ďg�p
	// corners[0]�ɐڂ���Q�҂𓊉e���Ƃ��Ďg�p
	edge01 = VECTOR2(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
	graphics->Vector2Normalize(&edge01);
	edge03 = VECTOR2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
	graphics->Vector2Normalize(&edge03);

	// ���̃G���e�B�e�B�𓊉e����ɓ��e�����Ƃ��̍ŏ��l�ƍő�l
	projection = graphics->Vector2Dot(&edge01, &corners[0]);
	edge01Min = projection;
	edge01Max = projection;

	// edge01�ւ̓��e
	projection = graphics->Vector2Dot(&edge01, &corners[1]);
	if(projection < edge01Min)
		edge01Min = projection;
	else if(projection > edge01Max)
		edge01Max = projection;

	// edge03�ւ̓��e
	projection = graphics->Vector2Dot(&edge03, &corners[0]);
	edge03Min = projection;
	edge03Max = projection;
	projection = graphics->Vector2Dot(&edge03, &corners[3]);
	if(projection < edge03Min)
		edge03Min = projection;
	else if(projection > edge03Max)
		edge03Max = projection;

	rotatedBoxReady = true;
}

// ============================================================================
// �R���X�g���N�^
// ============================================================================
Entity::Entity() : Image()
{
	radius = 1.0;
	edge.left = -1;
	edge.top = -1;
	edge.right = 1;
	edge.bottom = 1;
	mass = 1.0;
	velocity.x = 0.0;
	velocity.y = 0.0;
	deltaV.x = 0.0;
	deltaV.y = 0.0;
	active = true;  // �G���e�B�e�B�̓A�N�e�B�u
	rotatedBoxReady = false;
	collisionType = entityNS::CIRCLE;
	health = 100;
	gravity = entityNS::GRAVITY;
}

// ============================================================================
// Entity��������
// ���s�O�F*gamePtr = Game�I�u�W�F�N�g�ւ̃|�C���^
// �@�@�@�@width = Image�̕��i�s�N�Z���P�ʁj�i�O = �e�N�X�`���S�̂̕����g�p�j
// �@�@�@�@height = Image�̍����i�s�N�Z���P�ʁj�i�O = �e�N�X�`���S�̂̍������g�p�j
// �@�@�@�@ncols = �e�N�X�`�����̗񐔁i�P����n�܂Łj�i�O�͂P�Ɠ����j
// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
// ============================================================================
bool Entity::initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM)
{
	input = gamePtr->getInput();  // ���̓V�X�e��
	return (Image::initialize(gamePtr->getGraphics(), width, height, ncols, textureM));
}

// ============================================================================
// Update
// �ʏ�A�t���[�����ƂɂP��Ăяo��
// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
// ============================================================================
void Entity::update(float frameTime)
{
	velocity += deltaV;
	deltaV.x = 0;
	deltaV.y = 0;
	Image::update(frameTime);
	rotatedBoxReady = false;  // rotatedBox�̏Փ˔���̂���
}

// ============================================================================
// AI�i�l�H�m�\�j
// �ʏ�A�t���[�����ƂɂP��Ăяo��
// �l�H�m�\�v�Z�����s�Aent�͑��ݍ�p�̂��߂ɓn�����
// ============================================================================
void Entity::ai(float frameTime, Entity& ent) {}

// ============================================================================
// �G���e�B�e�B���A�N�e�B�u��
// ============================================================================
void Entity::activate() { active = true; }

// ============================================================================
// ���̃G���e�B�e�B�Ƃ�������̃G���e�B�e�B�̏Փ˂𔻒�
// �e�G���e�B�e�B�͕K���P��ނ̏Փ˂��g�p���܂��B
// �Փ˂̎�ނ𕡐��K�v�Ƃ��镡�G�Ȑ}�`�́A
// �e������ʌ̃G���e�B�e�B�Ƃ��Ĉ������Ƃɂ�菈���ł��܂��B
// �ʏ�A�t���[�����ƂɂP��Ăяo��
// �Փ˂̎�ށFCIRCLE�ABOX�A�܂���ROTATED_BOX
// ���s��F�Փ˂��Ă���ꍇ�́Atrue�A���Ă��Ȃ��ꍇ��false��߂�
// �@�@�@�@�Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
// ============================================================================
bool Entity::collidesWith(Entity& ent, VECTOR2& collisionVector)
{
	// �ǂ��炩�̃G���e�B�e�B���A�N�e�B�u�łȂ��ꍇ�A�Փ˂͋N����Ȃ�
	if(!active || !ent.getActive()) 
		return false;

	// �����̃G���e�B�e�B��CIRCLE�Փ˂ł���ꍇ
	if(collisionType == entityNS::CIRCLE && ent.getCollisionType() == entityNS::CIRCLE)
		return collideCircle(ent, collisionVector);

	// �����̃G���e�B�e�B��BOX�Փ˂ł���ꍇ
	if(collisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX)
		return collideBox(ent, collisionVector);

	// ���̂��ׂĂ̑g�ݍ��킹���g�p���镪�����e�X�g
	// �ǂ���̃G���e�B�e�B��CIRCLE�Փ˂��g�p���Ȃ��ꍇ
	if(collisionType != entityNS::CIRCLE && ent.getCollisionType() != entityNS::CIRCLE)
		return collideRotatedBox(ent, collisionVector);
	// �ǂ��炩�P�̃G���e�B�e�B���~�̏ꍇ
	else
	{
		// ���̃G���e�B�e�B��CIRCLE�Փ˂��g�p����ꍇ
		if(collisionType == entityNS::CIRCLE)
			return ent.collideRotatedBoxCircle(*this, collisionVector);
		// ��������̃G���e�B�e�B��CIRCLE�Փ˂��g�p����ꍇ
		else
			return collideRotatedBoxCircle(ent, collisionVector);
	}
	return false;
}

// ============================================================================
// ���̃G���e�B�e�B���w�肳�ꂽ��`�̊O���ɂ��邩
// ���s��F��`�̊O���ɂ���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
// ============================================================================
bool Entity::outsideRect(RECT rect)
{
	if(spriteData.x + spriteData.width * getScale() < rect.left ||
		spriteData.x > rect.right ||
		spriteData.y + spriteData.height * getScale() < rect.top ||
		spriteData.y > rect.bottom)
		return true;
	return false;
}

// ============================================================================
// Damage
// ���̃G���e�B�e�B���A����ɂ���ă_���[�W���󂯂�
// �p�����鑤�̃N���X�ł��̊֐����I�[�o�[���C�h
// ============================================================================
void Entity::damage(int weapon) {}

// ============================================================================
// Bounce
// ���̃G���e�B�e�B�����̃G���e�B�e�B�ƏՓ˂������ɒ��˂�
// ============================================================================
void Entity::bounce(VECTOR2& collisionVector, Entity& ent) {}

// ============================================================================
// �d��
// ============================================================================
void Entity::gravityForce(Entity* other, float frameTime) {}