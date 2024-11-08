//===============================================
//
// 爆発(explosion.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "explosion.h"
#include "texture.h"
#include "Anim.h"

//============================
// マクロ定義
//============================

//============================
// 静的メンバ変数宣言
//============================
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = nullptr;				//共通テクスチャバッファ
int CExplosion::m_nNumAll = 0;		//総数

//============================================
//==================== 弾 ====================
//============================
// コンストラクタ
//============================
CExplosion::CExplosion(int nPriority) : CObjectBillboardAnim(nPriority)
{
	m_nNumAll++;
}

//============================
// デストラクタ
//============================
CExplosion::~CExplosion()
{
	m_nNumAll--;
}

//====================================
// 2D弾の初期化(デフォ)
//====================================
HRESULT CExplosion::Init(void)
{
	//初期化
	CObjectBillboardAnim::Init();

	//タイプ設定
	SetType(TYPE_EXPLOSION);

	return S_OK;
}

//====================================
// 2D弾の初期化(位置オバロ)
//====================================
HRESULT CExplosion::Init(const D3DXVECTOR3 pos)
{
	//初期化
	if(FAILED(CObjectBillboardAnim::Init(pos)))
	{
		return E_FAIL;
	}

	//サイズ設定
	SetSize(D3DXVECTOR3(DEF_SIZE_ANIM.x, DEF_SIZE_ANIM.y, 0.0f));



	//タイプ設定
	SetType(TYPE_EXPLOSION);

	return S_OK;
}

//============================
// 2D弾の終了
//============================
void CExplosion::Uninit(void)
{
	//終了
	CObjectBillboardAnim::Uninit();
}

//============================
// 2D弾の更新
//============================
void CExplosion::Update(void)
{
	CObjectBillboardAnim::Update();
	CAnim *pAnim = CObjectBillboardAnim::GetAnim();

	//ループ判定
	if (pAnim->GetNumLoop() == 0)
	{
		Uninit();
	}
}

//============================
// 2D弾の描画
//============================
void CExplosion::Draw(void)
{
	//描画
	CObjectBillboardAnim::Draw();
}

//============================
// 弾の生成
//============================
CExplosion *CExplosion::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nFrame, const int nLoop, const int nNumPtnX, const int nNumPtnY)
{
	CExplosion *pExplosion = nullptr;

	//オブジェクト生成&初期化
	pExplosion = new CExplosion;

	if (pExplosion != nullptr)
	{
		if (FAILED(pExplosion->Init(pos)))
		{
			pExplosion->Uninit();
			return nullptr;
		}
		else
		{

			CAnim *pAnim = pExplosion->GetAnim();

			pAnim->SetNumPtnX(nNumPtnX);		//パターン数設定
			pAnim->SetNumPtnY(nNumPtnY);
			pAnim->SetNumFrame(nFrame);		//フレーム数設定
			pAnim->SetNumLoop(nLoop);		//ループ数設定

			pExplosion->SetLight(true);

			pExplosion->SetTexPos(D3DXVECTOR2(1.0f / nNumPtnX, 1.0f / nNumPtnY), D3DXVECTOR2(0.0f, 0.0f));	//テクスチャサイズ設定

			pExplosion->SetSize(size);		//サイズ設定

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pExplosion->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\explosion000.png"));
		}
	}
	else
	{
		return nullptr;
	}

	return pExplosion;
}
