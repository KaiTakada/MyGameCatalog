//===============================================
//
// エフェクト(effect.cpp)
// Author 髙田 佳依
//
//===============================================
#ifndef _EFFECT_H_		//このマクロ定義がされてなかったら
#define _EFFECT_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "objectBillboard.h"

//=========================
// マクロ定義
//=========================

//=========================
// オブジェクト2D背景クラス
//=========================
class CEffect : public CObjectBillboard
{
public:
	enum TEX
	{
		TEX_000 = 0,
		TEX_SMOKE,
		TEX_MAX,
	};

	CEffect(int nPriority = 4);		//コンストラクタ
	~CEffect();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	void SetLife(int nLife) { m_nLife = nLife; }				//残り寿命
	int GetLife() { return m_nLife; }							//残り寿命

	void SetLifeMax(int nLife) { m_nLifeMax = nLife; }				//寿命
	int GetLifeMax() { return m_nLifeMax; }							//寿命

	void SetType(TEX type) { m_type = type; }				//寿命
	TEX GetType() { return m_type; }						//寿命

	static CEffect *Create(void);		//生成
	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	const D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f), int nLife = 10, const TEX TexType = TEX_000);		//生成

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_MAX];				//テクスチャバッファ
	int m_nLife;		//残り寿命
	int m_nLifeMax;		//寿命
	TEX m_type;			//種類
};

#endif