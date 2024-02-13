//===============================================
//
// 煙エフェクト(ef_smoke.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _EF_SMOKE_H_		//このマクロ定義がされてなかったら
#define _EF_SMOKE_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "effect.h"

//=========================
// マクロ定義
//=========================
#define SMOKE_SIZE D3DXVECTOR3(10.0f,10.0f,10.0f)	//経験値オーブのデフォルトサイズ

//=========================
// 前方宣言
//=========================

//=========================
// プレイヤークラス
//=========================
class CEf_Smoke : public CEffect
{
public:

	CEf_Smoke(int nPriority = 5);		//コンストラクタ
	~CEf_Smoke();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CEf_Smoke *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size = SMOKE_SIZE);		//生成

protected:

private:
};

#endif
