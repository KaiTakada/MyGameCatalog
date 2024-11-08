//===============================================
//
// 便利関数ら(useful.cpp)
// Author: Kai Takada
//
//===============================================

//===========================
// インクルードファイル
//===========================
#include "useful.h"

using namespace mylib_useful;

//================
// 2点の角度取得(xz平面)
//@param (pos0)主となる位置
//@param (pos1)相手の位置
//@return 角度
//================
float mylib_useful::Point2Angle(D3DXVECTOR3 posMy, D3DXVECTOR3 posTarget)
{
	D3DXVECTOR3 vecDiff = D3DXVECTOR3(posMy.x - posTarget.x,
		0.0f,
		posMy.z - posTarget.z);

	float fAngle = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;		//目標の向き
	mylib_useful::NormalizeRot(fAngle);

	return fAngle;
}

//================
// 2点間の単位ベクトル取得(回転)
//@param (pos0)主となる位置
//@param (pos1)相手の位置
//@return ベクトル
//================
D3DXVECTOR3 mylib_useful::Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3 vecDiff = pos1 - pos0;
	D3DXVec3Normalize(&vecDiff, &vecDiff);		//ベクトルを正規化する(1.0f 〜 -1.0fにする)

	return vecDiff;
}

//================
// ベクトルの大きさ取得
//@param (X)ベクトルのX軸
//@param (Y)ベクトルのY軸
//@return 距離
//================
float mylib_useful::Vec2Length(float X, float Y)
{
	return sqrtf(static_cast<float>(pow(X, 2)) + static_cast<float>(pow(Y, 2)));
}

//================
// ベクトルの角度取得
//@param (X)ベクトルのX軸
//@param (Y)ベクトルのY軸
//@return 角度
//================
float mylib_useful::VecRot(float vecX, float vecY)
{
	return atan2f(vecY, vecX);
}

//=======================
//@brief 球同士の当たり判定
// 引数：
// (pos0)対象0の座標
// (fRad0)対象0の半径
// (pos1)対象1の座標
// (fRad1)対象1の半径
// 返り値：当たったかどうか(true:当たった,false:当たってない)
//=======================
bool mylib_useful::CollisionCircle(const D3DXVECTOR3 pos0, const float fRad0, const D3DXVECTOR3 pos1, const float fRad1)
{
	bool bCol = false;

	//3軸使った球の判定
	float fColl = fRad1 + fRad0;		//当たり判定範囲

	float fLength0 = hypotf((pos1.x - pos0.x), (pos1.z - pos0.z));		//2点間の長さxz
	float fLength1 = hypotf((pos1.x - pos0.x), (pos1.y - pos0.y));		//2点間の長さxy
	float fLength2 = hypotf((pos1.z - pos0.z), (pos1.y - pos0.y));		//2点間の長さzy

	if (fLength0 <= fColl &&
		fLength1 <= fColl &&
		fLength2 <= fColl)
	{//当たっていたら
		bCol = true;
	}

	return bCol;
}

//=======================
//@brief 3.14の縛りをかける
// 引数：
// (fRot)修正対象
// 返り値：修正結果
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
// 線の当たり判定  
// 引数：
// (pos0)線の座標(元)
// (pos1)線の座標(先)
// (MainPos)判定対象の座標
// 返り値：線のどちら側にいるか判定(true:右,false:左)
//==================================================================================
bool mylib_useful::CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos)
{
	// 当たったかの判定
	bool bHit = false;

	// 境界線のベクトル
	D3DXVECTOR3 vecLine;
	vecLine.x = pos1.x - pos0.x;
	vecLine.z = pos1.z - pos0.z;

	// 弾と壁のベクトル
	D3DXVECTOR3 vecToPos;
	vecToPos.x = MainPos.x - pos0.x;
	vecToPos.z = MainPos.z - pos0.z;

	if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) <= 0)
	{// 線を超えた

	 // 当たった
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// 円の当たり判定(3D空間のxz平面)
// 引数：
// (pos1)判定を求めているものの座標
// (pos2)判定対象の座標
// (fRadius1)半径
// (fRadius2)判定を求める角度の基点
// 返り値：中にいるか判定(true:当たった,false:当たってない)
//==================================================================================
bool mylib_useful::CircleRange3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2)
{
	float fLength =
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z);

	if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
	{// 円の中に入ったら

	 // 当たった判定返す
		return true;
	}

	return false;
}

//==================================================================================
// 扇の当たり判定
// 引数：
// (posChar)判定を求めているものの座標
// (posLeft)扇の左側座標
// (posRight)扇の右側座標
// (Targetpos)判定対象の座標
// (fTargetRadius)判定を求める角度の基点
// 返り値：中にいるか判定(true:当たった,false:当たってない)
//==================================================================================
bool mylib_useful::CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 TargetPos, float fTargetRadius)
{
	// 当たったかの判定
	bool bHit = false;
	bool bLine1 = false, bLine2 = false, bCircle = false;

	// 左の点との線分
	bLine1 = CollisionLine(posLeft, posChar, TargetPos);

	// 右の点との線分
	bLine2 = CollisionLine(posChar, posRight, TargetPos);

	// 扇の半径
	float fLen =
		sqrtf((posLeft.x - posChar.x) * (posLeft.x - posChar.x)
			+ (posLeft.z - posChar.z) * (posLeft.z - posChar.z));

	// 円の当たり判定
	if (CircleRange3D(posChar, TargetPos, fLen, fTargetRadius) == true)
	{// 当たっていたら
		bCircle = true;
	}

	// 全ての範囲に入っていたら
	if (bLine1 == true && bLine2 == true && bCircle == true)
	{
		bHit = true;
	}

	return bHit;
}

//==================================================================================
// ランダムに方向を生成する
// 引数：無し
// 返り値：生成された方向
//==================================================================================
float mylib_useful::RandomRot()
{
	 float fRot = static_cast<float>(rand() % 629 - 314) / 100.0f;

	return fRot;
}

//==================================================================================
// ドット積(内積)
// 引数：
// (vecU)第1ベクトル
// (vecV)第2ベクトル
// 返り値：中にいるか判定(true:当たった,false:当たってない)
//==================================================================================
float mylib_useful::Dot(D3DXVECTOR3 vecU, D3DXVECTOR3 vecV)
{
	return vecU.x * vecV.x + vecU.y * vecV.y + vecU.z * vecV.z;
}

//==================================================================================
// 線分と点の距離判定
// 引数：
// (Linepos0)線の片側座標
// (Linepos1)線のもう片側座標
// (Pointpos)判定対象の座標
// 返り値：最短距離
//==================================================================================
float mylib_useful::LinePointLength(D3DXVECTOR3 LineposA, D3DXVECTOR3 LineposB, D3DXVECTOR3 Pointpos)
{
	D3DXVECTOR3 ap = Pointpos - LineposA;
	D3DXVECTOR3 ab = LineposB - LineposA;
	D3DXVECTOR3 ba = LineposA - LineposB;
	D3DXVECTOR3 bp = Pointpos - LineposB;

	float distance;	//距離
	D3DXVECTOR3 neighbor_point;	//接点

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
// 動く面に対する当たり判定
//====================================================================
bool mylib_useful::CollisionBlock(
	D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 MovePair, D3DXVECTOR3 SizePair,
	D3DXVECTOR3* pPosMy, D3DXVECTOR3 pPosOldMy, D3DXVECTOR3* pMoveMy, D3DXVECTOR3 SizeMy, bool* bJump)
{
		//＋から−の面
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

		//−から＋の面
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

		//＋から−の面
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

		//−から＋の面
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

		//＋から−の面
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

		//−から＋の面
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
