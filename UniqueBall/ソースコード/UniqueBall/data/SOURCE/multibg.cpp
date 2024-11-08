//===============================================
//
// マルチ背景(multibg.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "multibg.h"
#include "bg.h"
#include "obj_polygon2D.h"

//============================
// マクロ定義
//============================

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ(デフォ)
//============================
CMultiBg::CMultiBg()
{
	for (int nCntCrt = 0; nCntCrt < MAX_MULTIBG; nCntCrt++)
	{
		if (m_apObject[nCntCrt] != nullptr)
		{
			m_apObject[nCntCrt] = nullptr;
		}
	}
}

//============================
// デストラクタ
//============================
CMultiBg::~CMultiBg()
{

}
//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CMultiBg::Init(void)
{
	for (int nCntCrt = 0; nCntCrt < MAX_MULTIBG; nCntCrt++)
	{
		if (m_apObject[nCntCrt] == nullptr)
		{
			m_apObject[nCntCrt] = CObject2D::Create();		//自身を代入

			if (m_apObject[nCntCrt] != nullptr)
			{
				m_apObject[nCntCrt]->Init();
			}

			break;
		}
	}

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CMultiBg::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR2 move, const D3DXVECTOR3 size, int nNum)
{
	int nNumTex = 0;

	if (nNum > MAX_MULTIBG)
	{
		return E_FAIL;
	}

	for (int nCntCrt = 0; nCntCrt < nNum; nCntCrt++)
	{
		for (int nCntAll = 0; nCntAll < MAX_MULTIBG; nCntAll++)
		{
			if (m_apObject[nCntAll] == nullptr)
			{
				m_apObject[nCntAll] = CBg::Create(pos, rot, move, size, CBg::TEX(nNumTex));		//自身を代入
				
				move *= 2.0f;

				if (move.x >= 1.0f)
				{
					move.x = 0.7f;
				}
				if (move.y >= 1.0f)
				{
					move.y = 0.7f;
				}

				nNumTex++;

				if (nNumTex >= CBg::TEX_MAX)
				{
					nNumTex = 0;
				}

				break;
			}
		}
	}

	SetType(TYPE_MULTIBG);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化処理(オバロ)
//====================================
HRESULT CMultiBg::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	pos;
	rot;

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CMultiBg::Uninit(void)
{
	for (int nCntCrt = 0; nCntCrt < MAX_MULTIBG; nCntCrt++)
	{
		if (m_apObject[nCntCrt] != nullptr)
		{
			m_apObject[nCntCrt]->Uninit();
			m_apObject[nCntCrt] = nullptr;
		}
	}

	CObject::Release();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CMultiBg::Update(void)
{

}

//============================
// 2Dポリゴンの描画処理
//============================
void CMultiBg::Draw(void)
{

}

//============================
// 2Dポリゴンの生成処理
//============================
CMultiBg *CMultiBg::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR2 move, const D3DXVECTOR3 size, int nNum)
{
	CMultiBg *pMultiBg = nullptr;

	if (pMultiBg != nullptr)
	{
		pMultiBg = nullptr;
	}

	pMultiBg = new CMultiBg;

	if (pMultiBg != nullptr)
	{
		pMultiBg->Init(pos,rot,move,size,nNum);
	}
	else
	{
		return nullptr;
	}

	return pMultiBg;
}

//============================
// 破棄
//============================
void CMultiBg::Release(int nID)
{
	nID;
}

//============================
// 位置設定
//============================
void CMultiBg::SetPos(const D3DXVECTOR3 pos)
{
	pos;
}

//============================
// 向き設定
//============================
void CMultiBg::SetRot(const D3DXVECTOR3 rot)
{
	rot;
}

//============================
// 位置取得
//============================
D3DXVECTOR3 CMultiBg::GetPos(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
// 2Dサイズ取得
//============================
D3DXVECTOR3 CMultiBg::GetSize(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
