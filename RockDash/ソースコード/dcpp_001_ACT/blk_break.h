//==================================================
//
// ゴール処理(blk_break.h)
// Author : Kai Takada
//
//==================================================
#ifndef _BLK_BREAK_H_								//このマクロ定義がされてなかったら
#define _BLK_BREAK_H_								//二重インクルード防止のマクロを定義する

//============================================
// インクルードファイル
//============================================
#include "block.h"

//============================================
//マクロ定義
//============================================

//============================================
// 前方宣言
//============================================

//============================================
// ブロッククラス
//============================================
class CBlk_Break : public CBlock
{
public:
	CBlk_Break(int nPriority = 1);
	~CBlk_Break();

	HRESULT Init(void);			//初期化
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE_Blk type = TYPE_BREAK);		//初期化(オバロ)
	void Uninit(void);			//終了
	void Update(void);			//更新
	void Draw(void);			//描画

	virtual bool CollisionRect(void);		//当たり判定(継承)

	static CBlk_Break *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//生成

protected:

private:
};

#endif
