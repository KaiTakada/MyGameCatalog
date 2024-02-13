//===============================================
//
// 身元識別(identity.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _IDENTITY_H_		//このマクロ定義がされてなかったら
#define _IDENTITY_H_		//二重インクルード防止のマクロを定義する

//=================================
// マクロ定義
//=================================

namespace my_Identity
{
	enum eMember	//所属
	{
		MEMBER_NONE = 0,
		MEMBER_PLAYER0,		//4人分
		MEMBER_PLAYER1,
		MEMBER_PLAYER2,
		MEMBER_PLAYER3,
		MEMBER_ENEMY,
		MEMBER_MAX
	};

	const float THROW_SPEED = 100.0f;		//ボールを投げる時の付与スピード
	const float THROW_PARABOLA = -0.02f;		//ボールを投げる時の角度(放物線もどき)
}

#endif
