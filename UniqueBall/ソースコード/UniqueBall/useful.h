//===============================================
//
// �֗��֐���(useful.h)
// Author: ���c ����
//
//===============================================
#ifndef _USEFUL_H_		//���̃}�N����`������ĂȂ�������
#define _USEFUL_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=================================
// �C���N���[�h�t�@�C��
//=================================
#include "main.h"

//=================================
// �萔��`
//=================================
namespace mylib_useful
{
	float Point2Angle(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);
	float VecLength(float X, float Y);
	bool CollisionCircle(const D3DXVECTOR3 pos0, const float fRad0, const D3DXVECTOR3 pos1, const float fRad1);
}

#endif
