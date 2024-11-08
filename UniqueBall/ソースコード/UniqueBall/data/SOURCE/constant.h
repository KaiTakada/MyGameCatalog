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
	const D3DXVECTOR2 DEFVEC2 = D3DXVECTOR2(0.0f, 0.0f);	//デフォルトvec3
	const D3DXVECTOR3 MID2D = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);	//2Dの真ん中
	const D3DXVECTOR3 DEF_FIELD_SIZE = D3DXVECTOR3(2048.0f, 0.0f, 2048.0f);		//マップデフォルトサイズ
	const D3DXVECTOR2 DEF_FIELD_SIZE2 = D3DXVECTOR2(2048.0f, 2048.0f);		//マップデフォルトサイズ

	const D3DXCOLOR PERSONAL_COLOR[] =
	{// 各プレイヤー色
		D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
		D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
		D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),
	};
}

#endif
