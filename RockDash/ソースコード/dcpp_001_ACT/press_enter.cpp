//===============================================
//
// 2Dポリゴン(object2D.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "press_enter.h"
#include "texture.h"

//============================
// マクロ定義
//============================

//============================
// 前方宣言
//============================
//class CPlayer;

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ(デフォ)
//============================
CPress_Enter::CPress_Enter(int nPriority) : CObject2D(nPriority)
{
	m_nAlphaCnter = 0;						//PRESS ENTERの点滅間隔の判定
	m_nTimeInterval = 0;					//PRESS ENTERの点滅間隔
	m_fAlphaColor = 0.0f;					//頂点カラー
}

//============================
// デストラクタ
//============================
CPress_Enter::~CPress_Enter()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CPress_Enter::Init(void)
{
	m_nTimeInterval = 25;						//点滅間隔を小さくする
	m_fAlphaColor = 1.0f;
	m_nAlphaCnter = 0;

	CObject2D::Init();

	SetType(TYPE_ENTER);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CPress_Enter::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_nTimeInterval = 25;						//点滅間隔を小さくする
	m_fAlphaColor = 1.0f;
	m_nAlphaCnter = 0;

	CObject2D::Init(pos, rot);

	SetType(TYPE_ENTER);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CPress_Enter::Uninit(void)
{
	CObject2D::Uninit();

	//死亡フラグを建てちゃう
	SetDeath(true);
}

//============================
// 2Dポリゴンの更新処理
//============================
void CPress_Enter::Update(void)
{
	CObject2D::Update();

	m_nAlphaCnter++;

	if (m_nAlphaCnter % m_nTimeInterval == 0)
	{//0.5秒消したら

		m_nAlphaCnter = 0;
		D3DXCOLOR col = GetTexCol();

		//col.a = col.a ? 1.0f : 0.0f;
		if (col.a == 0.0f)
		{
			col.a = 1.0f;
		}
		else if (col.a == 1.0f)
		{
			col.a = 0.0f;
		}

		m_fAlphaColor = col.a;

		//col.a = m_fAlphaColor;
		SetTexCol(col);
	}
}

//============================
// 2Dポリゴンの描画処理
//============================
void CPress_Enter::Draw(void)
{
	CObject2D::Draw();
}

//============================
// 2Dポリゴンの生成処理
//============================
CPress_Enter * CPress_Enter::Create(void)
{
	CPress_Enter *pEnter;

	//オブジェクト生成
	pEnter = new CPress_Enter;

	//初期化
	pEnter->Init();

	return pEnter;
}

//============================
// 2Dポリゴンの生成処理
//============================
CPress_Enter * CPress_Enter::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 rot)
{
	CPress_Enter *pEnter = new CPress_Enter;

	if (pEnter != nullptr)
	{
		pEnter->Init(pos, rot);
		
		pEnter->SetSize(size);

		//テクスチャ付与
		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		pEnter->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\PressEnter00.png"));
	}
	else
	{
		return nullptr;
	}

	return pEnter;
}
