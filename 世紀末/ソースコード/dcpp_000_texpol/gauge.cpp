//===============================================
//
// ゲージ(gauge.cpp) 継承 : objectBillboard
// Author: 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "manager.h"	//大元
#include "gauge.h"		//元
#include "texture.h"	//描画テクスチャ
#include "effect.h"		//軌跡
#include "particle.h"	//エフェクト

//============================
// マクロ定義
//============================
#define NUM_SPEED (20.0f)	//移動量
#define NUM_SIZE D3DXVECTOR3(50.0f,50.0f,50.0f)				//デフォルトサイズ
#define FILE_PASS_BAR "data\\TEXTURE\\GAUGE\\bar.png"	//ファイルパス
#define FILE_PASS_FRAME "data\\TEXTURE\\GAUGE\\frame.png"	//ファイルパス

//============================
// 静的メンバ変数宣言
//============================
LPDIRECT3DTEXTURE9 CGauge::m_pTexture = nullptr;				//共通テクスチャバッファ

//============================================
//==================== 弾 ====================
//============================
// コンストラクタ
//============================
CGauge::CGauge(int nPriority) : CObject(nPriority)
{
	m_pFrameBB = nullptr;			//ビルボード(枠)
	m_pBarBB = nullptr;			//ビルボード(棒)
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_sizeMax = m_size;
	m_fRat = 0.0f;			//割合
}

//============================
// デストラクタ
//============================
CGauge::~CGauge()
{

}

//====================================
// 2D弾の初期化(デフォ)
//====================================
HRESULT CGauge::Init(void)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRat = 0.0f;

	m_pFrameBB = CObjectBillboard::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f, 0.0f));
	m_pBarBB = CObjectBillboard::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f, 0.0f));

	m_pBarBB->SetLight(true);
	m_pFrameBB->SetLight(true);

	SetType(TYPE_GAUGE);

	return S_OK;
}

//====================================
// 2D弾の初期化(位置オバロ)
//====================================
HRESULT CGauge::Init(const D3DXVECTOR3 pos)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRat = 0.0f;

	m_pBarBB = CObjectBillboard::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f, 0.0f), D3DXVECTOR3(NUM_SIZE.x, NUM_SIZE.y, NUM_SIZE.z), true);
	m_pFrameBB = CObjectBillboard::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f, 0.0f), D3DXVECTOR3(NUM_SIZE.x, NUM_SIZE.y, NUM_SIZE.z), true);

	m_pBarBB->SetLight(true);
	m_pFrameBB->SetLight(true);

	//タイプ設定
	SetType(TYPE_GAUGE);

	return S_OK;
}

//============================
// 2D弾の終了
//============================
void CGauge::Uninit(void)
{
	if (m_pFrameBB != nullptr)
	{
		m_pFrameBB->Uninit();
	}

	if (m_pBarBB != nullptr)
	{
		m_pBarBB->Uninit();
	}

	Release();
}

//============================
// 2D弾の更新
//============================
void CGauge::Update(void)
{
	//if (m_pFrameBB != nullptr)
	//{
	//	m_pFrameBB->Update();
	//}

	//if (m_pBarBB != nullptr)
	//{
	//	m_pBarBB->Update();
	//}
}

//============================
// 2D弾の描画
//============================
void CGauge::Draw(void)
{
	//Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//Zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//============================
// 弾の生成
//============================
CGauge * CGauge::Create( D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const float fRat)
{
	CGauge *pGauge= nullptr;

	//オブジェクト生成&初期化
	pGauge= new CGauge;

	if (pGauge!= nullptr)
	{
		if (FAILED(pGauge->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			pGauge->m_fRat = fRat;		//割合
			pGauge->m_pBarBB->SetSize(size);		//サイズ設定
			pGauge->m_pFrameBB->SetSize(size);		//サイズ設定
			pGauge->SetSizeMax(pGauge->m_pFrameBB->GetSize());

			CTexture *pTexture = CManager::GetTexture();
			pGauge->m_pBarBB->SetIdxTexture(pTexture->Regist(FILE_PASS_BAR));
			pGauge->m_pFrameBB->SetIdxTexture(pTexture->Regist(FILE_PASS_FRAME));
		
			pGauge->m_pBarBB->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
	}
	else
	{
		return nullptr;
	}

	return pGauge;
}

//============================
// 割合設定
//============================
void CGauge::SetRatio(float fRat)
{
	if (m_pBarBB != nullptr)
	{
		D3DXVECTOR3 size = m_sizeMax;
		D3DXVECTOR3 pos = m_pBarBB->GetPos();

		size.x *= fRat;
		float fPosX = size.x - m_sizeMax.x;

		//pos.x = fPosX;

		m_fRat = fRat;
		m_pBarBB->SetSize(size);
		//m_pBarBB->SetPos(pos);
	}
}

//============================
// 座標設定
//============================
void CGauge::SetPos(D3DXVECTOR3 pos)
{
	if (m_pBarBB != nullptr)
	{
		m_pBarBB->SetPos(pos);
	}


	if (m_pFrameBB != nullptr)
	{
		m_pFrameBB->SetPos(pos);
	}
}

//============================
// バーの色設定
//============================
void CGauge::SetCol(D3DXCOLOR col)
{
	m_pBarBB->SetColor(col);
}
