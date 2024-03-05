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
#include "constant.h"		//����萔�Q

//=================================
// �萔��`
//=================================
namespace mylib_useful
{
	float Point2Angle(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);	// 2�_�̊p�x�擾(xz����)
	D3DXVECTOR3 Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);	// 2�_�̊p�x�擾(3D)
	float VecLength(float X, float Y);	// �x�N�g���̑傫���擾
	float VecRot(float X, float Y);	// �x�N�g���̊p�x�擾
	bool CollisionCircle(const D3DXVECTOR3 pos0, const float fRad0, const D3DXVECTOR3 pos1, const float fRad1);	//�����m�̓����蔻��
	float NormalizeRot(float fRot);	//�p�x�C��(�΃��[�v�p)
	bool CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);
	bool CircleRange3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);
	bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 TargetPos, float fTargetRadius);
	float RandomRot();	//�����_������
}

#endif
