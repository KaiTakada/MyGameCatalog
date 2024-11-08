//===============================================
//
// 便利関数ら(useful.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _USEFUL_H_		//このマクロ定義がされてなかったら
#define _USEFUL_H_		//二重インクルード防止のマクロを定義する

//=================================
// インクルードファイル
//=================================
#include "main.h"
#include "constant.h"		//自作定数群

//template <class T> T Add(const T data0, const T data1);
//template <class T> T operator(const T data);

//=================================
// 定数定義
//=================================
namespace mylib_useful
{
	float Point2Angle(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);	// 2点の角度取得(xz平面)
	D3DXVECTOR3 Point2Rot(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);	// 2点の角度取得(3D)
	float Vec2Length(float X, float Y);	// ベクトルの大きさ取得
	float VecRot(float X, float Y);	// ベクトルの角度取得
	bool CollisionCircle(const D3DXVECTOR3 pos0, const float fRad0, const D3DXVECTOR3 pos1, const float fRad1);	//球同士の当たり判定
	float NormalizeRot(float fRot);	//角度修正(πループ用)
	bool CollisionLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 MainPos);
	bool CircleRange3D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float fRadius1, float fRadius2);
	bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 TargetPos, float fTargetRadius);
	float RandomRot();	//ランダム方向

	float Dot(D3DXVECTOR3 vecU, D3DXVECTOR3 vecV);
	float LinePointLength(D3DXVECTOR3 Linepos0, D3DXVECTOR3 Linepos1, D3DXVECTOR3 Pointpos);

	bool CollisionBlock( D3DXVECTOR3 PosPair, D3DXVECTOR3 PosOldPair, D3DXVECTOR3 MovePair, D3DXVECTOR3 SizePair,
		D3DXVECTOR3* pPosMy, D3DXVECTOR3 pPosOldMy, D3DXVECTOR3* pMoveMy, D3DXVECTOR3 SizeMy, bool* bJump );

	//=============================================
	// 値の修正
	//=============================================
	template<class T>
	void Limit(T &main, T min, T max)
	{
		if (main < min)
		{
			main = min;
		}

		if (main > max)
		{
			main = max;
		}
	}

	template<class T>
	void Limit(T& main, T min)
	{
		if (main < min)
		{
			main = min;
		}
	}
}

#endif
