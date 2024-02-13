//===============================================
//
// 煙エフェクト(ef_smoke.cpp) 継承 : effect
// Author: 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "manager.h"	//大元
#include "ef_smoke.h"		//元
#include "texture.h"	//描画テクスチャ
#include "game.h"	//ゲーム
#include "player.h"	//プレイヤー

//============================
// マクロ定義
//============================
#define NUM_SPEED (20.0f)	//移動量
#define NUM_SIZE D3DXVECTOR3(50.0f,50.0f,50.0f)				//デフォルトサイズ

//============================
// 静的メンバ変数宣言
//============================

//============================
// ファイル名定義
//============================
static const char *c_pTexSmoke = "data\\TEXTURE\\EFFECT\\effect002.jpg";

//============================================
//==================== 弾 ====================
//============================
// コンストラクタ
//============================
CEf_Smoke::CEf_Smoke(int nPriority) : CEffect(nPriority)
{

}

//============================
// デストラクタ
//============================
CEf_Smoke::~CEf_Smoke()
{

}

//====================================
// 煙の初期化(デフォ)
//====================================
HRESULT CEf_Smoke::Init(void)
{
	CEffect::Init();

	//色
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	CObject::SetType(TYPE_EFFECT);

	return S_OK;
}

//====================================
// 煙の初期化(位置オバロ)
//====================================
HRESULT CEf_Smoke::Init(const D3DXVECTOR3 pos)
{
	CEffect::Init(pos);

	//色
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//タイプ設定
	CObject::SetType(TYPE_EFFECT);

	return S_OK;
}

//============================
// 煙の終了
//============================
void CEf_Smoke::Uninit(void)
{
	CEffect::Uninit();
}

//============================
// 煙の更新
//============================
void CEf_Smoke::Update(void)
{
	//位置変え
	D3DXVECTOR3 pos = GetPos();
	pos += GetMove();
	SetPos(pos);

	//サイズ変え
	D3DXVECTOR3 size = GetSize();
	size *= 1.07f;
	SetSize(size);

	//色褪せ
	D3DXCOLOR col = GetColor();
	col.r *= 0.95f;
	col.g *= 0.95f;
	col.b *= 0.95f;
	SetColor(col);

	//寿命
	int nLife = GetLife();
	if (nLife <= 0)
	{
		Uninit();
		return;
	}

	//寿命削り
	nLife--;
	SetLife(nLife);
}

//============================
// 煙の描画
//============================
void CEf_Smoke::Draw(void)
{
	CEffect::Draw();
}

//============================
// 煙の生成
//============================
CEf_Smoke * CEf_Smoke::Create( D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CEf_Smoke *pSmoke= nullptr;

	//オブジェクト生成&初期化
	pSmoke= new CEf_Smoke;

	if (pSmoke!= nullptr)
	{
		if (FAILED(pSmoke->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			pSmoke->SetSize(size);
			pSmoke->SetLight(true);

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pSmoke->SetIdxTexture(pTexture->Regist(c_pTexSmoke));
		}
	}
	else
	{
		return nullptr;
	}

	return pSmoke;
}
