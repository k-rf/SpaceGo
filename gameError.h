#ifndef _GAMEERROR_H
#define _GAMEERROR_H

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <exception>

namespace gameErrorNS
{
	// �G���[�R�[�h
	// ���̐��́A�Q�[�����V���b�g�_�E������K�v�̂���v���I�ȃG���[��\���܂��B
	// ���̐��́A�Q�[�����V���b�g�_�E������K�v�̂Ȃ��x����\���܂��B
	const int FATAL_ERROR = -1;
	const int WARNING = 1;
}

// Game Error�N���X�B�Q�[���G���W���ɂ���ăG���[�����m���ꂽ�Ƃ��ɃX���[����܂��B
// std::exception���p��
class GameError : public std::exception
{
	int errorCode;
	std::string message;

public:
	// �f�t�H���g�R���X�g���N�^
	GameError()throw() : errorCode(gameErrorNS::FATAL_ERROR), message("Undefined Error in game.")
	{}

	// �R�s�[�R���X�g���N�^
	GameError(const GameError& e)throw() : std::exception(e), errorCode(e.errorCode), message(e.message)
	{}

	// ��������̃R���X�g���N�^
	GameError(int code, const std::string& s)throw() : errorCode(code), message(s)
	{}

	// ������Z�q
	GameError& operator=(const GameError& rhs)throw()
	{
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}

	// �f�X�g���N�^
	virtual ~GameError()throw() {}

	// ��{�N���X�ɑ΂���I�[�o�[���C�h
	virtual const char* what()const throw()
	{
		return this->getMessage();
	}

	const char* getMessage()const throw()
	{
		return message.c_str();
	}

	int getErrorCode()const throw()
	{
		return errorCode;
	}
};

#endif