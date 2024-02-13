//============================
//
// 3D背景処理(Bg3D.cpp)
// Author: Kai Takada
//
//============================
//==================================
// インクルードファイル
//==================================
#include "bg3D.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"
#include "Field.h"

//==================================
// マクロ定義
//==================================
#define TEX_FIELD "data\\TEXTURE\\BG\\bg_001.jpg"
#define DEF_NUM_DIF (8)		//テクスチャの分割数

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// コンストラクタ
//==================================
CBg3D::CBg3D(int nPriority) : CObject3D(nPriority)
{

}

//==================================
// デストラクタ
//==================================
CBg3D::~CBg3D()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CBg3D::Init(void)
{
	CObject3D::Init();

	SetPos(DEF_VERTEX3D);	//初期化する
	SetRot(DEF_VERTEX3D);	//初期化する
	SetSize(D3DXVECTOR3(DEF_SIZE.x, DEF_SIZE.y, 0.0f));

	D3DXVECTOR3 lRot = GetRot();	//初期化する
	SetRot({ lRot.x, D3DX_PI * 1.0f, lRot.z });	//初期化する

	SetType(TYPE_FIELD);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CBg3D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject3D::Init(pos,rot);

	D3DXVECTOR3 lRot = GetRot();	//初期化する
	SetRot({ D3DX_PI * -0.5f, lRot.y, lRot.z });	//初期化する
												
	//サイズ設定
	SetSize(D3DXVECTOR3(DEF_SIZE.x, 0.0f, DEF_SIZE.z));

	SetType(TYPE_FIELD);

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CBg3D::Uninit(void)
{
	CObject3D::Uninit();
}

//==================================
// 更新処理
//==================================
void CBg3D::Update(void)
{
	CObject3D::Update();
}

//==================================
// 描画処理
//==================================
void CBg3D::Draw(void)
{
	CObject3D::Draw();
}

//============================
// 生成処理
//============================
CBg3D * CBg3D::Create(void)
{
	CBg3D *pField;

	//オブジェクト生成
	pField = new CBg3D;

	//初期化
	pField->Init();

	return pField;
}

//============================
// 生成処理
//============================
CBg3D * CBg3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	CBg3D *pBg = nullptr;

	//オブジェクト生成&初期化
	pBg = new CBg3D;

	if (pBg != nullptr)
	{
		pBg->Init(pos, rot);

		pBg->SetSize(size);

		float fposZ = DEF_FIELD_SIZE.z;

		pBg->SetPos({ pos.x, pos.y + fposZ, pos.z + fposZ });

		int tex0 = int(size.x / 256.0f);
		int tex1 = int(size.z / 256.0f);
		pBg->SetDivTex(tex0, tex1);

		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		pBg->SetIdxTexture(pTexture->Regist(TEX_FIELD));
	}
	else
	{
		return nullptr;
	}

	return pBg;
}
