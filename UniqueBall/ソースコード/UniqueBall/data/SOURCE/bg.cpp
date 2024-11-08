//===============================================
//
// 背景(bg.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "bg.h"
#include "multibg.h"
#include "texture.h"

//============================
// マクロ定義
//============================

//============================
// 静的メンバ変数宣言
//============================
LPDIRECT3DTEXTURE9 CBg::m_pTexture[CBg::TEX_MAX] = {};

//============================
// ファイル名定義
//============================
static const char *c_apTexpass[CBg::TEX_MAX] =
{
	"data\\TEXTURE\\bg100.png",
	"data\\TEXTURE\\bg101.png",
	"data\\TEXTURE\\bg102.png",
};

//============================
// コンストラクタ
//============================
CBg::CBg()
{
	m_texMove = D3DXVECTOR2(0.0f, 0.0f);
}

//============================
// デストラクタ
//============================
CBg::~CBg()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CBg::Init(void)
{
	CObject2D::Init();

	SetType(TYPE_BG);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CBg::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject2D::Init();

	CObject2D::SetPos(pos);
	CObject2D::SetRot(rot);
	CObject2D::SetSize(D3DXVECTOR3(DEF_SIZE_STATIC.x, DEF_SIZE_STATIC.y, 0.0f));

	SetType(TYPE_BG);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CBg::Uninit(void)
{
	//自身の破棄
	CObject2D::Uninit();

	//Release();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CBg::Update(void)
{
	D3DXVECTOR2 texPosleftUp = GetTexPosleftUp();
	D3DXVECTOR2 texPosrightDown = GetTexPosrightDown();

	D3DXVECTOR2 move = m_texMove * 0.01f;

	texPosleftUp += move;
	texPosrightDown += move;

	//texPosleftUp += m_texMove;
	//texPosrightDown += m_texMove;


	if (texPosleftUp.x >= 1.0f - GetTexPosDif().x)
	{
		texPosleftUp.x = 0.0f;
	}
	if (texPosleftUp.y >= 1.0f - GetTexPosDif().y)
	{
		texPosleftUp.y = 0.0f;
	}

	if (texPosrightDown.x >= 1.0f - GetTexPosDif().x)
	{
		texPosrightDown.x = 1.0f;
	}
	if (texPosrightDown.y >= 1.0f - GetTexPosDif().y)
	{
		texPosrightDown.y = 1.0f;
	}

	SetTexPos(texPosrightDown, texPosleftUp);
}

//============================
// 2Dポリゴンの描画処理
//============================
void CBg::Draw(void)
{
	CObject2D::Draw();
}

//============================
// 2Dポリゴンの生成処理
//============================
CBg * CBg::Create(void)
{
	CBg *pBg;

	//オブジェクト生成
	pBg = new CBg;

	//初期化
	pBg->Init();

	return pBg;
}

//============================
// 2Dポリゴンの生成処理
//============================
CBg * CBg::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 move, const D3DXVECTOR3 size, const TEX TexType)
{
	CBg *pBg = nullptr;

	//オブジェクト生成&初期化
	if (pBg == nullptr)
	{
		pBg = new CBg;

		if (pBg != nullptr)
		{
			pBg->Init(pos, rot);

			pBg->SetSize(size);

			pBg->SetTexPos(D3DXVECTOR2(1.0f,1.0f), D3DXVECTOR2(0.0f, 0.0f));

			pBg->m_texMove = move;

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pBg->SetIdxTexture(pTexture->Regist(c_apTexpass[TexType]));
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	return pBg;
}
