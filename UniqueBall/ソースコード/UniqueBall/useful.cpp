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
// 2�_�̊p�x�擾(xz����)
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
	mylib_useful::NormalizeRot(fAngle);

	return fAngle;
}

//================
// 2�_�̊p�x�擾(xz����)
//@param (pos0)��ƂȂ�ʒu
//@param (pos1)����̈ʒu
//@return �p�x
//================
D3DXVECTOR3 mylib_useful::Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3 vecDiff = pos0 - pos1;
	D3DXVECTOR3 rot = mylib_const::DEFVEC3;
	rot.y = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;		//�ڕW�̌���(y)
	mylib_useful::NormalizeRot(rot.y);

	rot.z = VecRot(VecLength(vecDiff.x, vecDiff.z), vecDiff.y) + D3DX_PI;
	mylib_useful::NormalizeRot(rot.z);

	return rot;
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

//================
// �x�N�g���̊p�x�擾
//@param (X)�x�N�g����X��
//@param (Y)�x�N�g����Y��
//@return �p�x
//================
float mylib_useful::VecRot(float vecX, float vecY)
{
	return atan2f(vecY, vecX);
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

//=======================
//@brief 3.14�̔����������
// �����F
// (fRot)�C���Ώ�
// �Ԃ�l�F�C������
//=======================
float mylib_useful::NormalizeRot(float fRot)
{
	if (fRot < D3DX_PI && fRot > -D3DX_PI)
	{
		return fRot;
	}

	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}
	else if (fRot < D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}

	return fRot;
}

//==================================================================================
// ���̓����蔻��  
// �����F
// (pos0)��������߂Ă�����̂̍��W(��)
// (pos1)��������߂Ă�����̂̍��W(��)
// (MainPos)����Ώۂ̍��W
// (MainPosOld)����Ώۂ̉ߋ����W
// �Ԃ�l�F���̂ǂ��瑤�ɂ��邩����(true:�E,false:��)
//==================================================================================
bool mylib_useful::CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld)
{
	// �����������̔���
	bool bHit = false;

	// ���E���̃x�N�g��
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.z = pos1.z - pos0.z;

	// �e�ƕǂ̃x�N�g��
	D3DXVECTOR3 vecToPos;
	vecToPos.x = MainPos.x - pos0.x;
	vecToPos.z = MainPos.z - pos0.z;

	if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) <= 0)
	{// ���𒴂���

	 // ��������
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// �~�̓����蔻��(3D��Ԃ�xz����)
// �����F
// (pos1)��������߂Ă�����̂̍��W
// (pos2)����Ώۂ̍��W
// (fRadius1)���a
// (fRadius2)��������߂�p�x�̊�_
// �Ԃ�l�F���ɂ��邩����(true:��������,false:�������ĂȂ�)
//==================================================================================
bool mylib_useful::CircleRange3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength =
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
	{// �~�̒��ɓ�������

	 // ������������Ԃ�
		return true;
	}

	return false;
}

//==================================================================================
// ��̓����蔻��
// �����F
// (posChar)��������߂Ă�����̂̍��W
// (posLeft)��̍������W
// (posRight)��̉E�����W
// (Targetpos)����Ώۂ̍��W
// (fTargetRadius)��������߂�p�x�̊�_
// �Ԃ�l�F���ɂ��邩����(true:��������,false:�������ĂȂ�)
//==================================================================================
bool mylib_useful::CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 TargetPos, float fTargetRadius)
{
	// �����������̔���
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bCircle = false;

	// ���̓_�Ƃ̐���
	bLine1 = CollisionLine(posLeft, posChar, TargetPos, TargetPos);

	// �E�̓_�Ƃ̐���
	bLine2 = CollisionLine(posChar, posRight, TargetPos, TargetPos);

	// ��̔��a
	float fLen =
		sqrtf((posLeft.x - posChar.x) * (posLeft.x - posChar.x)
			+ (posLeft.z - posChar.z) * (posLeft.z - posChar.z));

	// �~�̓����蔻��
	if (CircleRange3D(posChar, TargetPos, fLen, fTargetRadius) == true)
	{// �������Ă�����
		bCircle = true;
	}

	// �S�Ă͈̔͂ɓ����Ă�����
	if (bLine1 == true && bLine2 == true && bCircle == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// �����_���ɕ����𐶐�����
// �����F����
// �Ԃ�l�F�������ꂽ����
//==================================================================================
float mylib_useful::RandomRot()
{
	 float fRot = static_cast<float>(rand() % 629 - 314) / 100.0f;

	return fRot;
}
