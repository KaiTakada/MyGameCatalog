//===============================================
//
// �g������(identity.h)
// Author: ���c ����
//
//===============================================
#ifndef _IDENTITY_H_		//���̃}�N����`������ĂȂ�������
#define _IDENTITY_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=================================
// �}�N����`
//=================================

namespace my_Identity
{
	enum eMember	//����
	{
		MEMBER_NONE = 0,
		MEMBER_PLAYER0,		//4�l��
		MEMBER_PLAYER1,
		MEMBER_PLAYER2,
		MEMBER_PLAYER3,
		MEMBER_ENEMY,
		MEMBER_MAX
	};

	const float THROW_SPEED = 100.0f;		//�{�[���𓊂��鎞�̕t�^�X�s�[�h
	const float THROW_PARABOLA = -0.02f;		//�{�[���𓊂��鎞�̊p�x(���������ǂ�)
}

#endif
