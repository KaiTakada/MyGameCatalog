//===============================================
//
// 定数定義(constant.h)
// Author: Kai Takada
//
//===============================================
#ifndef _CONSTANT_H_		//このマクロ定義がされてなかったら
#define _CONSTANT_H_		//二重インクルード防止のマクロを定義する

//=================================
// 定数定義
//=================================
namespace mylib_const
{
	const int MAX_PLAYER = 4;	//maxプレイヤー人数
	const D3DXVECTOR3 DEFVEC3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//デフォルトvec3
	const D3DXVECTOR3 DEF_FIELD_SIZE = D3DXVECTOR3(2048.0f, 0.0f, 2048.0f);		//マップデフォルトサイズ
}

#endif
