//============================
//
// 地面処理(Field.cpp)
// Author: Kai Takada
//
//============================
//==================================
// インクルードファイル
//==================================
#include "Field.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"

//==================================
// マクロ定義
//==================================
#define TEX_FIELD "data\\TEXTURE\\BG\\field.jpg"
#define DEF_NUM_DIF (8)

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// コンストラクタ
//==================================
CField::CField(int nPriority) : CObject3D(nPriority)
{

}

//==================================
// デストラクタ
//==================================
CField::~CField()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CField::Init(void)
{
	CObject3D::Init();

	SetPos(DEF_VERTEX3D);	//初期化する
	SetRot(DEF_VERTEX3D);	//初期化する
	SetSize(D3DXVECTOR3(DEF_SIZE.x, DEF_SIZE.y, 0.0f));

	SetType(TYPE_FIELD);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CField::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject3D::Init(pos,rot);

	//サイズ設定
	SetSize(D3DXVECTOR3(DEF_SIZE.x, 0.0f, DEF_SIZE.z));

	SetType(TYPE_FIELD);

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CField::Uninit(void)
{
	CObject3D::Uninit();
}

//==================================
// 更新処理
//==================================
void CField::Update(void)
{
	CObject3D::Update();
}

//==================================
// 描画処理
//==================================
void CField::Draw(void)
{
	CObject3D::Draw();
}

//============================
// 生成処理
//============================
CField * CField::Create(void)
{
	CField *pField;

	//オブジェクト生成
	pField = new CField;

	//初期化
	pField->Init();

	return pField;
}

//============================
// 生成処理
//============================
CField * CField::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	CField *pField = nullptr;

	//オブジェクト生成&初期化
	pField = new CField;

	if (pField != nullptr)
	{
		pField->Init(pos, rot);

		pField->SetSize(size);

		int tex0 = int(size.x / 256.0f);
		int tex1 = int(size.z / 256.0f);
		pField->SetDivTex(tex0, tex1);

		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		pField->SetIdxTexture(pTexture->Regist(TEX_FIELD));
	}
	else
	{
		return nullptr;
	}

	return pField;
}
