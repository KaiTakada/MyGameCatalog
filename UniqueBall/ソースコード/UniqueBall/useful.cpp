//===============================================
//
// �֗��֐���(useful.cpp)
// Author: Kai Takada
//
//===============================================

//===========================
// �C���N���[�h�t�@�C��
//===========================
#include "useful.h"

using namespace mylib_useful;

//================
// 2�_�̊p�x�擾
//@param (pos0)��ƂȂ�ʒu
//@param (pos1)����̈ʒu
//@return �p�x
//================
float mylib_useful::Point2Angle(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3 vecDiff = D3DXVECTOR3(pos0.x - pos1.x,
		0.0f,
		pos0.z - pos1.z);

	float fAngle = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;		//�ڕW�̌���

	return fAngle;
}

//================
// �x�N�g���̑傫���擾
//@param (X)�x�N�g����X��
//@param (Y)�x�N�g����Y��
//@return ����
//================
float mylib_useful::VecLength(float X, float Y)
{
	return sqrtf(static_cast<float>(pow(X, 2)) + static_cast<float>(pow(Y, 2)));
}

//=======================
//@brief �����m�̓����蔻��
// �����F
// (pos0)�Ώ�0�̍��W
// (fRad0)�Ώ�0�̔��a
// (pos1)�Ώ�1�̍��W
// (fRad1)�Ώ�1�̔��a
// �Ԃ�l�F�����������ǂ���(true:��������,false:�������ĂȂ�)
//=======================
bool mylib_useful::CollisionCircle(const D3DXVECTOR3 pos0, const float fRad0, const D3DXVECTOR3 pos1, const float fRad1)
{
	bool bCol = false;

	//3���g�������̔���
	float fColl = fRad1 + fRad0;		//�����蔻��͈�

	float fLength0 = hypotf((pos1.x - pos0.x), (pos1.z - pos0.z));		//2�_�Ԃ̒���xz
	float fLength1 = hypotf((pos1.x - pos0.x), (pos1.y - pos0.y));		//2�_�Ԃ̒���xy
	float fLength2 = hypotf((pos1.z - pos0.z), (pos1.y - pos0.y));		//2�_�Ԃ̒���zy

	if (fLength0 <= fColl &&
		fLength1 <= fColl &&
		fLength2 <= fColl)
	{//�������Ă�����
		bCol = true;
	}

	return bCol;
}