//===============================================
//
// 数字(number.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "number.h"
#include "object.h"
#include "texture.h"

//============================
// マクロ定義
//============================
namespace
{
	const float NUM_SPEED = 20.0f;											//移動量
	const D3DXVECTOR3 NUM_SIZE = D3DXVECTOR3(125.0f, 200.0f, 0.0f);				//デフォルトサイズ
	const char* NUMBER_PASS = "data\\TEXTURE\\NUMBER\\number000.png";		//デフォルトサイズ
}

//============================
// 静的メンバ変数宣言
//============================

//============================================
//==================== 弾 ====================
//============================
// コンストラクタ
//============================
CNumber::CNumber()
{
	m_nValue = 0;
}

//============================
// デストラクタ
//============================
CNumber::~CNumber()
{

}

//====================================
// 数字の初期化(デフォ)
//====================================
HRESULT CNumber::Init(void)
{
	//初期化
	CObject2D::Init();
	SetSize(NUM_SIZE);

	SetType(TYPE_NUMBER);

	return S_OK;
}

//====================================
// 数字の初期化(位置オバロ)
//====================================
HRESULT CNumber::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//初期化
	if(FAILED(CObject2D::Init(pos, rot)))
	{
		return E_FAIL;
	}

	//タイプ設定
	SetType(TYPE_NUMBER);

	return S_OK;
}

//============================
// 数字の終了
//============================
void CNumber::Uninit(void)
{
	//終了
	CObject2D::Uninit();
}

//============================
// 数字の更新
//============================
void CNumber::Update(void)
{

}

//============================
// 数字の描画
//============================
void CNumber::Draw(void)
{
	//描画
	CObject2D::Draw();
}

//============================
// 生成
//============================
CNumber* CNumber::Create(int nValue)
{
	CNumber* pNumber = nullptr;

	//オブジェクト生成&初期化
	pNumber = new CNumber;

	if (pNumber != nullptr)
	{
		if (FAILED(pNumber->Init()))
		{
			return nullptr;
		}
		else
		{
			CTexture* pTexture = CManager::GetInstance()->GetTexture();
			pNumber->SetIdxTexture(pTexture->Regist(NUMBER_PASS));

			pNumber->SetNumber(nValue);		//数値割り当て
		}
	}
	else
	{
		return nullptr;
	}

	return pNumber;
}

//============================
// 生成
//============================
CNumber *CNumber::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, int nValue)
{
	CNumber *pNumber = nullptr;

	//オブジェクト生成&初期化
	pNumber = new CNumber;

	if (pNumber != nullptr)
	{
		if (FAILED(pNumber->Init(pos, rot)))
		{
			return nullptr;
		}
		else
		{
			pNumber->SetSize(size);		//サイズ設定

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pNumber->SetIdxTexture(pTexture->Regist(NUMBER_PASS));

			pNumber->SetNumber(nValue);		//数値割り当て
		}
	}
	else
	{
		return nullptr;
	}

	return pNumber;
}

//============================
// 任意の数値を描画位置設定
//============================
void CNumber::SetNumber(int nNumber)
{
	m_nValue = nNumber;

	D3DXVECTOR2 rightDown = { 0.1f * nNumber + 0.1f,1.0f };
	D3DXVECTOR2 leftUp = { 0.1f * nNumber, 0.0f };
	
	SetTexPos(rightDown, leftUp);
}
