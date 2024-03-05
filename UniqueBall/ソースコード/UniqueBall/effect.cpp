//===============================================
//
// 効果ポリゴン(effect.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "effect.h"
#include "texture.h"

//============================
// マクロ定義
//============================

//============================
// 静的メンバ変数宣言
//============================
LPDIRECT3DTEXTURE9 CEffect::m_pTexture[CEffect::TEX_MAX] = {};

//============================
// ファイル名定義
//============================
static const char *c_apTexpass[CEffect::TEX_MAX] =
{
	"data\\TEXTURE\\EFFECT\\effect000.jpg",
	"data\\TEXTURE\\EFFECT\\effect002.jpg",
};

//============================
// コンストラクタ
//============================
CEffect::CEffect(int nPriority) : CObjectBillboard(nPriority)
{
	m_nLife = 0;
	m_nLifeMax = 0;
	m_type = TEX_000;
}

//============================
// デストラクタ
//============================
CEffect::~CEffect()
{

}

//====================================
// エフェクトの初期化処理(デフォ)
//====================================
HRESULT CEffect::Init(void)
{
	CObjectBillboard::Init();

	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//移動量
	m_nLife = 0;		//残り寿命
	m_nLifeMax = 0;		//寿命

	CObjectBillboard::SetZtest(true);
	CObjectBillboard::SetAbrend(true);

	CObject::SetType(TYPE_EFFECT);

	return S_OK;
}

//====================================
// エフェクトの初期化(位置オバロ)
//====================================
HRESULT CEffect::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	if (FAILED(CObjectBillboard::Init()))
	{
		return E_FAIL;
	}

	SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//移動量
	m_nLife = 0;		//残り寿命
	m_nLifeMax = 0;		//寿命

	CObjectBillboard::SetPos(pos);
	CObjectBillboard::SetRot(rot);
	CObjectBillboard::SetSize(D3DXVECTOR3(DEF_BBSIZE.x, DEF_BBSIZE.y, 0.0f));
	CObjectBillboard::SetZtest(true);
	CObjectBillboard::SetAbrend(true);

	CObject::SetType(TYPE_EFFECT);

	return S_OK;
}

//============================
// エフェクトの終了処理
//============================
void CEffect::Uninit(void)
{
	//自身の破棄
	CObjectBillboard::Uninit();
}

//============================
// エフェクトの更新処理
//============================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	pos += GetMove();

	//位置変え
	SetPos(pos);

	//サイズ変え
	size.x *= (float)m_nLife / (float)m_nLifeMax;
	size.y *= (float)m_nLife / (float)m_nLifeMax;
	SetSize(size);

	//画面外判定


	if (size.x <= 1.1f ||
		size.y <= 1.1f)
	{
		Uninit();
		return;
	}

	//寿命
	if (m_nLife == 0)
	{
		Uninit();
		return;
	}

	if (m_nLife > 0)
	{
		m_nLife--;
	}
}

//============================
// エフェクトの描画処理
//============================
void CEffect::Draw(void)
{
	CObjectBillboard::Draw();
}

//============================
// エフェクトの生成処理
//============================
CEffect * CEffect::Create(void)
{
	CEffect *pEffect;

	//オブジェクト生成
	pEffect = new CEffect;

	//初期化
	pEffect->Init();

	return pEffect;
}

//============================
// エフェクトの生成処理
//============================
CEffect * CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const D3DXVECTOR3 size, int nLife, const TEX TexType)
{
	CEffect *pEffect = nullptr;

	//オブジェクト生成&初期化
	if (pEffect == nullptr)
	{
		pEffect = new CEffect;

		if (pEffect != nullptr)
		{//生成出来たら

			if (FAILED(pEffect->Init(pos, rot)))
			{
				pEffect->Uninit();
				return nullptr;
			}

			pEffect->SetSize(size);

			pEffect->SetMove(move);
			pEffect->m_nLife = nLife;
			pEffect->m_nLifeMax = nLife;
			pEffect->m_type = TexType;

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pEffect->SetIdxTexture(pTexture->Regist(c_apTexpass[TexType]));
		}
		else
		{//生成出来ず
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	return pEffect;
}
