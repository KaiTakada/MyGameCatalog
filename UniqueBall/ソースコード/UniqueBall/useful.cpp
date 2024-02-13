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
// 2点の角度取得
//@param (pos0)主となる位置
//@param (pos1)相手の位置
//@return 角度
//================
float mylib_useful::Point2Angle(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3 vecDiff = D3DXVECTOR3(pos0.x - pos1.x,
		0.0f,
		pos0.z - pos1.z);

	float fAngle = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;		//目標の向き

	return fAngle;
}

//================
// ベクトルの大きさ取得
//@param (X)ベクトルのX軸
//@param (Y)ベクトルのY軸
//@return 距離
//================
float mylib_useful::VecLength(float X, float Y)
{
	return sqrtf(static_cast<float>(pow(X, 2)) + static_cast<float>(pow(Y, 2)));
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