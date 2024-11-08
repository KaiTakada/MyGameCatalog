//=================================================
//
// ビルボード(billboard.cpp)
// Author: Kai Takada
//
//=================================================
#include "target_mark.h"
#include "debugproc.h"
#include "texture.h"

//==============================
// マクロ定義
//==============================
namespace
{
	std::string TEX_PASS = "data\\TEXTURE\\target.png";
}

//==============================
// 静的メンバ変数宣言
//==============================

//==============================
// コンストラクタ
//==============================
CTarget_Mark::CTarget_Mark(int nPriority) : CObjectBillboard(nPriority)
{
	m_nIdx = -1;
}

//==============================
// デストラクタ
//==============================
CTarget_Mark::~CTarget_Mark()
{

}

//==============================
// ビルボードの初期化処理
//==============================
HRESULT CTarget_Mark::Init(void)
{
	CObjectBillboard::Init();

	return S_OK;
}

//==============================
// ビルボードの初期化処理
//==============================
HRESULT CTarget_Mark::Init(const D3DXVECTOR3 pos)
{
	CObjectBillboard::Init(pos);

	return S_OK;
}

//==============================
// ビルボードの終了処理
//==============================
void CTarget_Mark::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//==============================
// ビルボードの更新処理
//==============================
void CTarget_Mark::Update(void)
{
	CObjectBillboard::Update();
}

//==============================
// ビルボードの描画処理
//==============================
void CTarget_Mark::Draw(void)
{
	if (m_nIdx  == CManager::GetInstance()->GetRenderer()->GetCamCtr() &&
		m_nIdx != -1)
	{
		CObjectBillboard::Draw();
	}
}

//============================
// 2Dポリゴンの生成処理
//============================
CTarget_Mark * CTarget_Mark::Create(void)
{
	CTarget_Mark *pThis;

	//オブジェクト生成
	pThis = new CTarget_Mark;

	//初期化
	pThis->Init();
	pThis->SetLight(true);

	return pThis;
}

//============================
// 3Dポリゴンの生成処理
//============================
CTarget_Mark * CTarget_Mark::Create(const D3DXVECTOR3 pos)
{
	CTarget_Mark * pThis = nullptr;

	//オブジェクト生成&初期化
	pThis = new CTarget_Mark;

	if (pThis != nullptr)
	{
		pThis->Init(pos);
		pThis->SetLight(true);
		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		pThis->SetIdxTexture(pTexture->Regist(TEX_PASS.c_str()));
	}
	else
	{
		return nullptr;
	}

	return pThis;
}
