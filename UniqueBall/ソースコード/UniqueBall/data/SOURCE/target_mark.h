//=================================================
//
// ビルボード(target_mark.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _TARGET_MARK_H_								//このマクロ定義がされてなかったら
#define _TARGET_MARK_H_								//二重インクルード防止のマクロを定義する

//===============================
// インクルードファイル
//===============================
#include "objectBillboard.h"

//============================
// マクロ定義
//============================

//===============================
// 前方宣言
//===============================
class CAnim;

//===============================
// ビルボードクラス
//===============================
class CTarget_Mark : public CObjectBillboard
{
public:
	CTarget_Mark(int nPriority = 3);
	~CTarget_Mark();
	
	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CTarget_Mark *Create(void);		//生成
	static CTarget_Mark *Create(const D3DXVECTOR3 pos);		//生成

	void SetIdx(int nIdx) { m_nIdx = nIdx; }

protected:

private:
	int m_nIdx;		//使用しているプレイヤー番号
};

#endif