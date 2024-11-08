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
float mylib_useful::Point2Angle(D3DXVECTOR3 posMy, D3DXVECTOR3 posTarget)
{
	D3DXVECTOR3 vecDiff = D3DXVECTOR3(posMy.x - posTarget.x,
		0.0f,
		posMy.z - posTarget.z);

	float fAngle = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;		//�ڕW�̌���
	mylib_useful::NormalizeRot(fAngle);

	return fAngle;
}

//================
// 2�_�Ԃ̒P�ʃx�N�g���擾(��])
//@param (pos0)��ƂȂ�ʒu
//@param (pos1)����̈ʒu
//@return �x�N�g��
//================
D3DXVECTOR3 mylib_useful::Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3 vecDiff = pos1 - pos0;
	D3DXVec3Normalize(&vecDiff, &vecDiff);		//�x�N�g���𐳋K������(1.0f �` -1.0f�ɂ���)

	return vecDiff;
}

//================
// �x�N�g���̑傫���擾
//@param (X)�x�N�g����X��
//@param (Y)�x�N�g����Y��
//@return ����
//================
float mylib_useful::Vec2Length(float X, float Y)
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
// (pos0)���̍��W(��)
// (pos1)���̍��W(��)
// (MainPos)����Ώۂ̍��W
// �Ԃ�l�F���̂ǂ��瑤�ɂ��邩����(true:�E,false:��)
//==================================================================================
bool mylib_useful::CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos)
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
	bLine1 = CollisionLine(posLeft, posChar, TargetPos);

	// �E�̓_�Ƃ̐���
	bLine2 = CollisionLine(posChar, posRight, TargetPos);

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

//==================================================================================
// �h�b�g��(����)
// �����F
// (vecU)��1�x�N�g��
// (vecV)��2�x�N�g��
// �Ԃ�l�F���ɂ��邩����(true:��������,false:�������ĂȂ�)
//==================================================================================
float mylib_useful::Dot(D3DXVECTOR3 vecU, D3DXVECTOR3 vecV)
{
	return vecU.x * vecV.x + vecU.y * vecV.y + vecU.z * vecV.z;
}

//==================================================================================
// �����Ɠ_�̋�������
// �����F
// (Linepos0)���̕Б����W
// (Linepos1)���̂����Б����W
// (Pointpos)����Ώۂ̍��W
// �Ԃ�l�F�ŒZ����
//==================================================================================
float mylib_useful::LinePointLength(D3DXVECTOR3 LineposA, D3DXVECTOR3 LineposB, D3DXVECTOR3 Pointpos)
{
	D3DXVECTOR3 ap = Pointpos - LineposA;
	D3DXVECTOR3 ab = LineposB - LineposA;
	D3DXVECTOR3 ba = LineposA - LineposB;
	D3DXVECTOR3 bp = Pointpos - LineposB;

	float distance;	//����
	D3DXVECTOR3 neighbor_point;	//�ړ_

	if (Dot(ap, ab) < 0)
	{
		distance = std::sqrt(Dot(ap,ap));
		neighbor_point = LineposA;
	}
	else if (Dot(bp, ba) < 0)
	{
		D3DXVECTOR3 pos = Pointpos - LineposB;
		distance = std::sqrt(Dot(pos,pos));
		neighbor_point = LineposB;
	}
	else
	{
		float ai_norm = Dot(ap, ab) / std::sqrt(Dot(ab, ab));
		neighbor_point = LineposA + (ab) / std::sqrt(Dot(ab, ab)) * ai_norm;
		D3DXVECTOR3 pos = Pointpos - neighbor_point;
		distance = std::sqrt(Dot(pos, pos));
	}

	return 0.0f;
}

//====================================================================
// �����ʂɑ΂��铖���蔻��
//====================================================================
bool mylib_useful::CollisionBlock(
	D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 MovePair, D3DXVECTOR3 SizePair,
	D3DXVECTOR3* pPosMy, D3DXVECTOR3 pPosOldMy, D3DXVECTOR3* pMoveMy, D3DXVECTOR3 SizeMy, bool* bJump)
{
		//�{����|�̖�
		if (PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			(PosOldPair.x + SizePair.x <= pPosMy->x - SizeMy.x ||
				PosPair.x + SizePair.x <= pPosOldMy.x - SizeMy.x ||
				(PosPair.x + SizePair.x >= pPosOldMy.x - SizeMy.x &&
					PosOldPair.x + SizePair.x <= pPosOldMy.x - SizeMy.x &&
					PosOldPair.x + SizePair.x >= pPosMy->x - SizeMy.x)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.x >= 0.0f)
			{
				pPosMy->x = pPosOldMy.x - SizeMy.x + SizeMy.x + MovePair.x;
			}
			else
			{
				pPosMy->x = PosPair.x + SizePair.x + SizeMy.x + MovePair.x;
			}
			pMoveMy->x = 0.0f;
		}

		//�|����{�̖�
		if (PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			(PosOldPair.x - SizePair.x >= pPosMy->x + SizeMy.x ||
				PosPair.x - SizePair.x >= pPosOldMy.x + SizeMy.x ||
				(PosPair.x - SizePair.x <= pPosOldMy.x + SizeMy.x &&
					PosOldPair.x - SizePair.x >= pPosOldMy.x + SizeMy.x &&
					PosOldPair.x - SizePair.x <= pPosMy->x + SizeMy.x)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.y + SizePair.y >= pPosMy->y &&
			PosPair.y - SizePair.y <= pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.x <= 0.0f)
			{
				pPosMy->x = pPosOldMy.x + SizeMy.x - SizeMy.x + MovePair.x;
			}
			else
			{
				pPosMy->x = PosPair.x - SizePair.x - SizeMy.x + MovePair.x;
			}
			pMoveMy->x = 0.0f;
		}

		//�{����|�̖�
		if (PosPair.y + SizePair.y > pPosMy->y &&
			(PosOldPair.y + SizePair.y <= pPosMy->y ||
				PosPair.y + SizePair.y <= pPosOldMy.y ||
				(PosPair.y + SizePair.y >= pPosOldMy.y &&
					PosOldPair.y + SizePair.y <= pPosOldMy.y &&
					PosOldPair.y + SizePair.y >= pPosMy->y)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x
			)
		{
			if (MovePair.y >= 0.0f)
			{
				pPosMy->y = pPosOldMy.y + MovePair.y;
			}
			else
			{
				pPosMy->y = PosPair.y + SizePair.y + MovePair.y;
			}
			pMoveMy->y = 0.0f;
			*bJump = false;
		}

		//�|����{�̖�
		if (PosPair.y - SizePair.y < pPosMy->y + SizeMy.y &&
			(PosOldPair.y - SizePair.y >= pPosMy->y + SizeMy.y ||
				PosPair.y - SizePair.y >= pPosOldMy.y + SizeMy.y ||
				(PosPair.y - SizePair.y <= pPosOldMy.y + SizeMy.y &&
					PosOldPair.y - SizePair.y >= pPosOldMy.y + SizeMy.y &&
					PosOldPair.y - SizePair.y <= pPosMy->y + SizeMy.y)) &&
			PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x
			)
		{
			pPosMy->y = PosPair.y - SizePair.y - SizeMy.y + MovePair.y;
			pMoveMy->y = MovePair.y;
		}

		//�{����|�̖�
		if (PosPair.z + SizePair.z > pPosMy->z - SizeMy.z &&
			(PosOldPair.z + SizePair.z <= pPosMy->z - SizeMy.z ||
				PosPair.z + SizePair.z <= pPosOldMy.z - SizeMy.z ||
				(PosPair.z + SizePair.z >= pPosOldMy.z - SizeMy.z &&
					PosOldPair.z + SizePair.z <= pPosOldMy.z - SizeMy.z &&
					PosOldPair.z + SizePair.z >= pPosMy->z - SizeMy.z)) &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.z >= 0.0f)
			{
				pPosMy->z = pPosOldMy.z - SizeMy.z + SizeMy.z + MovePair.z;
			}
			else
			{
				pPosMy->z = PosPair.z + SizePair.z + SizeMy.z + MovePair.z;
			}
			pMoveMy->z = 0.0f;
		}

		//�|����{�̖�
		if (PosPair.z - SizePair.z < pPosMy->z + SizeMy.z &&
			(PosOldPair.z - SizePair.z >= pPosMy->z + SizeMy.z ||
				PosPair.z - SizePair.z >= pPosOldMy.z + SizeMy.z ||
				(PosPair.z - SizePair.z <= pPosOldMy.z + SizeMy.z &&
					PosOldPair.z - SizePair.z >= pPosOldMy.z + SizeMy.z &&
					PosOldPair.z - SizePair.z <= pPosMy->z + SizeMy.z)) &&
			PosPair.x + SizePair.x > pPosMy->x - SizeMy.x &&
			PosPair.x - SizePair.x < pPosMy->x + SizeMy.x &&
			PosPair.y + SizePair.y > pPosMy->y &&
			PosPair.y - SizePair.y < pPosMy->y + SizeMy.y
			)
		{
			if (MovePair.z <= 0.0f)
			{
				pPosMy->z = pPosOldMy.z + SizeMy.z - SizeMy.z + MovePair.z;
			}
			else
			{
				pPosMy->z = PosPair.z - SizePair.z - SizeMy.z + MovePair.z;
			}
			pMoveMy->z = 0.0f;
		}

	return false;
}
