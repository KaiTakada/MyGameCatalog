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

//=================================
// 定数定義
//=================================
namespace mylib_useful
{
	float Point2Angle(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);
	float VecLength(float X, float Y);
	bool CollisionCircle(const D3DXVECTOR3 pos0, const float fRad0, const D3DXVECTOR3 pos1, const float fRad1);
}

#endif
