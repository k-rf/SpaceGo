#ifndef _ENTITY_H
#define _ENTITY_H

#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "game.h"

namespace entityNS
{
	enum COLLISION_TYPE { NONE, CIRCLE, BOX, ROTATED_BOX, PIXEL_PERFECT };
	const float GRAVITY = 6.67428e-11f;
}

class Entity : public Image
{
protected:
	entityNS::COLLISION_TYPE collisionType;      // �Փ˂̎��
	VECTOR2 center;          // �G���e�B�e�B�̒��S
	float radius;            // �~�̏Փ˂̔���Ɏg�p����A�~�̔��a
	VECTOR2 distSquared;
	float sumRadiiSquared;
	RECT edge;               // BOX�܂���ROTATED_BOX�̏Փ˔��肪�g�p�����`�̈�
	VECTOR2 corners[4];      // ROTATED_BOX�̏Փ˔��肪�g�p�����`�̈�̒��_
	VECTOR2 edge01, edge03;  // ���e�ɗp������
	float edge01Min, edge01Max, edge03Min, edge03Max;
	VECTOR2 velocity;        // ���x
	VECTOR2 deltaV;
	float mass;              // �G���e�B�e�B�̎���
	float health;            // �G���e�B�e�B�̗̑�
	float rr;
	float force;
	float gravity;
	Input* input;
	HRESULT hr;
	bool active;
	bool rotatedBoxReady;
	DWORD pixelsColliding;

	// �~�̏Փ˔��胁�\�b�h
	// collision()�ɂ���ČĂяo�����A�f�t�H���g�̏Փ˔��胁�\�b�h
	// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
	// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	virtual bool collideCircle(Entity& ent, VECTOR2& collisionVector);

	// �����s���E�{�b�N�X�̏Փ˔��胁�\�b�h
	// collision()�ɂ���ČĂяo�����
	// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
	// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	virtual bool collideBox(Entity& ent, VECTOR2& collisionVector);

	// ��]����{�b�N�X�̏Փ˔��胁�\�b�h
	// collision()�ɂ���ČĂяo�����
	// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false
	// �@�@�@�@�Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	// �������e�X�g���g�p���ďՓ˂𔻒�
	// �������e�X�g�F
	// �Q�̃{�b�N�X�́A����̒����ւ̓��e���d�Ȃ��Ă��Ȃ���ΏՓ˂��Ă��Ȃ�
	virtual bool collideRotatedBox(Entity& ent, VECTOR2& collisionVector);

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
	virtual bool collideRotatedBoxCircle(Entity& ent, VECTOR2& collisionVector);

	// ��]����{�b�N�X�̒��_�A���e���A���e�̍ŏ��l�ƍő�l���v�Z
	// 0---1 ���_�̔ԍ�
	// |   |
	// 3---2
	void computeRotatedBox();

	// ����̃{�b�N�X���A���̃G���e�B�e�B��edge01�����edge03�ɓ��e
	// collideRotatedVox()�ɂ���ČĂяo�����
	// ���s��F���e���d�Ȃ��Ă���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
	bool projectionsOverlap(Entity& ent);

	// �{�b�N�X�̒��_���~�ƏՓ˂��Ă��Ȃ������A�����̃`�F�b�N���g�p���Ĕ���
	// collideRotatedBoxCircle()�ɂ���ČĂяo�����
	// ���s��F�Փ˂��Ă���ꍇ��true�A���Ă��Ȃ��ꍇ��false��߂�
	// �Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	bool collideCornerCircle(VECTOR2 corner, Entity& ent, VECTOR2& collisionVector);

public:
	// �R���X�g���N�^
	Entity();

	// �f�X�g���N�^
	virtual ~Entity(){}

	// ------------------------------------------------------------------------
	// �Q�b�^
	// ------------------------------------------------------------------------

	// �G���e�B�e�B�̒��S��Ԃ�
	virtual const VECTOR2* getCenter()
	{
		center = VECTOR2(getCenterX(), getCenterY());
		return &center;
	}

	// �Փ˔���̉~�̔��a��Ԃ�
	virtual float getRadius()const { return radius; }

	// BOX��ROTATED_BOX�̏Փ˔���Ɏg���Ă���RECT�\���̂�Ԃ�
	virtual const RECT& getEdge()const { return edge; }

	// ROTATED_BOX�̒��_c��Ԃ�
	virtual const VECTOR2* getCorner(UINT c)const
	{
		if(c >= 4)
			c = 0;
		return &corners[c];
	}

	// ���x�x�N�g����Ԃ�
	virtual const VECTOR2 getVelocity()const { return velocity; }

	// active��Ԃ�
	virtual bool getActive()const { return active; }

	// ���ʂ�Ԃ�
	virtual float getMass()const { return mass; }

	// �d�͒萔��Ԃ�
	virtual float getGravity()const { return gravity; }

	// �̗͂�Ԃ�
	virtual float getHealth()const { return health; }

	virtual entityNS::COLLISION_TYPE getCollisionType() { return collisionType; }


	// ------------------------------------------------------------------------
	// �Z�b�^
	// ------------------------------------------------------------------------

	// ���x��ݒ�
	virtual void setVelocity(VECTOR2 v) { velocity = v; }

	// ���x�ω���ݒ�
	virtual void setDeltaV(VECTOR2 dv) { deltaV = dv; }

	// active��ݒ�
	virtual void setActive(bool a) { active = a; }

	// �̗͂�ݒ�
	virtual void setHealth(float h) { health = h; }

	// ���ʂ�ݒ�
	virtual void setMass(float m) { mass = m; }

	// �d�͒萔��ݒ�
	virtual void setGravity(float g) { gravity = g; }

	// �Փ˔���̉~�̔��a��ݒ�
	virtual void setCollisionRadius(float r) { radius = r; }

	// �Փ˂̎�ނ�ݒ�iNONE, CIRCLE, BOX, ROTATED_BOX)
	virtual void setCollisionType(entityNS::COLLISION_TYPE ctype) { collisionType = ctype; }

	// BOX, ROTATD_BOX�̏Փ˔���Ɏg����RECT�\���̂�ݒ�
	virtual void setEdge(RECT e) { edge = e; }

	// rotatedBoxReady��ݒ�
	virtual void setRotaryBoxReady(bool r) { rotatedBoxReady = r; }

	// ------------------------------------------------------------------------
	// ���̑��̊֐�
	// ------------------------------------------------------------------------

	// Entity��������
	// ���s�O�F*gamePtr = Game�I�u�W�F�N�g�ւ̃|�C���^
	// �@�@�@�@width = Image�̕��i�s�N�Z���P�ʁj�i�O = �e�N�X�`���S�̂̕����g�p�j
	// �@�@�@�@height = Image�̍����i�s�N�Z���P�ʁj�i�O = �e�N�X�`���S�̂̍������g�p�j
	// �@�@�@�@ncols = �e�N�X�`�����̗񐔁i�P����n�܂Łj�i�O�͂P�Ɠ����j
	// ���s��F���������ꍇ��true�A���s�����ꍇ��false��߂�
	virtual bool initialize(Game* gamePtr, int width, int height, int ncols, TextureManager* textureM);
	
	// Update
	// �ʏ�A�t���[�����ƂɂP��Ăяo��
	// frameTime�́A�ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
	virtual void update(float frameTime);

	// AI�i�l�H�m�\�j
	// �ʏ�A�t���[�����ƂɂP��Ăяo��
	// �l�H�m�\�v�Z�����s�Aent�͑��ݍ�p�̂��߂ɓn�����
	virtual void ai(float frameTime, Entity& ent);

	// �G���e�B�e�B���A�N�e�B�u��
	virtual void activate();

	// ���̃G���e�B�e�B�Ƃ�������̃G���e�B�e�B�̏Փ˂𔻒�
	// �e�G���e�B�e�B�͕K���P��ނ̏Փ˂��g�p���܂��B
	// �Փ˂̎�ނ𕡐��K�v�Ƃ��镡�G�Ȑ}�`�́A
	// �e������ʌ̃G���e�B�e�B�Ƃ��Ĉ������Ƃɂ�菈���ł��܂��B
	// �ʏ�A�t���[�����ƂɂP��Ăяo��
	// �Փ˂̎�ށFCIRCLE�ABOX�A�܂���ROTATED_BOX
	// ���s��F�Փ˂��Ă���ꍇ�́Atrue�A���Ă��Ȃ��ꍇ��false��߂�
	// �@�@�@�@�Փ˂��Ă���ꍇ�́AcollisionVector��ݒ�
	virtual bool collidesWith(Entity& ent, VECTOR2& collisionVector);

	// ���̃G���e�B�e�B���w�肳�ꂽ��`�̊O���ɂ��邩
	// ���s��F��`�̊O���ɂ���ꍇ��true�A����ȊO�̏ꍇ��false��߂�
	virtual bool outsideRect(RECT rect);

	// Damage
	// ���̃G���e�B�e�B���A����ɂ���ă_���[�W���󂯂�
	// �p�����鑤�̃N���X�ł��̊֐����I�[�o�[���C�h
	virtual void damage(int weapon);

	// Bounce
	// ���̃G���e�B�e�B���ق��̃G���e�B�e�B�ƏՓ˂������ɒ��˂�
	void bounce(VECTOR2& collisionVector, Entity& ent);

	// �d��
	void gravityForce(Entity* other, float frameTime);

};

#endif