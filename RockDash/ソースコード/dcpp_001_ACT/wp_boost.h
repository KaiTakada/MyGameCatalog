//==================================================
//
// ブースター(wp_boost.h)
// Author : Kai Takada
//
//==================================================
#ifndef _WP_BOOST_H_								//このマクロ定義がされてなかったら
#define _WP_BOOST_H_								//二重インクルード防止のマクロを定義する

#include "weapon.h"

//============================================
// マクロ定義
//============================================

//============================================
// 前方宣言
//============================================

//============================================
// ブロッククラス
//============================================
class CBoost : public CWeapon
{
public:
	CBoost(int nPriority = 3);
	~CBoost();

	HRESULT Init(void);			//初期化
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);			//終了
	void Update(void);			//更新
	void Draw(void);			//描画
	virtual void Attack(const D3DXVECTOR3 rot, const int nDamage = 1);			//攻撃

	static CBoost *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//生成

protected:

private:

};

#endif
