//===============================================
//
// アニメーション(Anim.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "Anim.h"
#include "texture.h"

//============================
// マクロ定義
//============================

//============================
// 前方宣言
//============================

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
//============================
CAnim::CAnim()
{
	m_nCtrAnim = 0;							//アニメーションカウンター
	m_nPtnAnimX = 0;						//アニメーションパターンx No.
	m_nPtnAnimY = 0;						//アニメーションパターンy No.
	m_nNumPtnAnimX = 0;						//アニメーションパターン数x
	m_nNumPtnAnimY = 0;						//アニメーションパターン数y
	m_nFrame = 0;		//更新頻度フレーム
	m_nLoop = 0;		//ループ数(-1で無限)
	m_bEnd = false;

	for (int nCntTex = 0; nCntTex < MAX_VTX; nCntTex++)
	{
		m_aTexpos[nCntTex] = {};
	}
}

//============================
// デストラクタ
//============================
CAnim::~CAnim()
{

}

//============================
// 初期化
//============================
HRESULT CAnim::Init(void)
{
	m_nCtrAnim = 0;
	m_nPtnAnimX = 0;
	m_nPtnAnimY = 0;
	m_nNumPtnAnimX = 2;
	m_nNumPtnAnimY = 5;
	m_nFrame = 10;
	m_nLoop = 0;
	
	for (int nCntTex = 0; nCntTex < MAX_VTX; nCntTex++)
	{
		m_aTexpos[nCntTex] = D3DXVECTOR2(0.0f, 0.0f);
	}

	return S_OK;
}

//============================
// 終了
//============================
void CAnim::Uninit(void)
{

}

//============================
// 更新
//============================
void CAnim::Update(void)
{
	float fTexX = 1.0f / m_nNumPtnAnimX;
	float fTexY = 1.0f / m_nNumPtnAnimY;

	m_nCtrAnim++;							//カウンターを加算

	if ((m_nCtrAnim % m_nFrame) == 0)
	{
		m_nCtrAnim = 0;					//カウンターを初期値に戻す
		m_nPtnAnimX++;						//カウンターを更新する

		if (m_nPtnAnimX > m_nNumPtnAnimX * m_nNumPtnAnimY - 1)
		{//分割軸X
			m_nPtnAnimY++;		//分割軸Yを進める

			if (m_nPtnAnimY >= m_nNumPtnAnimY)
			{//1ループ
				m_nPtnAnimY = 0;

				if (m_nLoop == 0)
				{
					m_bEnd = true;
				}
				m_nLoop--;
			}

			m_nPtnAnimX = 0;				//カウンターを初期値に戻す
		}
	}

	float fTexposX = m_nPtnAnimX * fTexX;
	float fTexposY = m_nPtnAnimY * fTexY;

	//テクスチャ座標の設定
	m_aTexpos[0] = D3DXVECTOR2(fTexposX, fTexposY);				//(x,y)
	m_aTexpos[1] = D3DXVECTOR2(fTexposX + fTexX, fTexposY);
	m_aTexpos[2] = D3DXVECTOR2(fTexposX, fTexposY + fTexY);
	m_aTexpos[3] = D3DXVECTOR2(fTexposX + fTexX, fTexposY + fTexY);
}

//============================
// テクスチャ座標設定
//============================
void CAnim::SetTexpos(const D3DXVECTOR2 * pTexpos)
{
	for (int nCntTex = 0; nCntTex < MAX_VTX; nCntTex++)
	{
		m_aTexpos[nCntTex] = pTexpos[nCntTex];
	}
}
