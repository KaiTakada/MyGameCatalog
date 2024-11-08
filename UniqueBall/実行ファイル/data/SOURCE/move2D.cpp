//===============================================
//
// 移動2D(move2D.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "move2D.h"
#include "texture.h"

//============================
// マクロ定義
//============================
namespace
{
	const D3DXVECTOR3 DEF_SIZE = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT, 0.0f };	//デフォルトサイズ
}

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ(デフォ)
//============================
CMove2D::CMove2D(int nPriority) : CObject2D(nPriority)
{
	m_posDest = mylib_const::DEFVEC3;
	m_fSpeed = 0.0f;
}

//============================
// デストラクタ
//============================
CMove2D::~CMove2D()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CMove2D::Init(void)
{
	CObject2D::Init();

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CMove2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject2D::Init(pos,rot);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CMove2D::Uninit(void)
{
	CObject2D::Uninit();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CMove2D::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = mylib_const::DEFVEC3;
	float fRot = mylib_useful::Point2Angle(m_posDest, pos);

	move.x = sinf(fRot + D3DX_PI) * m_fSpeed;
	move.y = cosf(fRot + D3DX_PI) * m_fSpeed;
	
	pos += move;

	SetPos(pos);

	CObject2D::Update();
}

//============================
// 2Dポリゴンの描画処理
//============================
void CMove2D::Draw(void)
{
	CObject2D::Draw();
}

//============================
// 2Dポリゴンの生成処理
//============================
CMove2D * CMove2D::Create(void)
{
	CMove2D *pObject2D;

	//オブジェクト生成
	pObject2D = new CMove2D;

	//初期化
	pObject2D->Init();

	return pObject2D;
}

//============================
// 2Dポリゴンの生成処理
//============================
CMove2D * CMove2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posDest, const float fSpeed)
{
	CMove2D *pObject2D = nullptr;

	pObject2D = new CMove2D;

	if (pObject2D != nullptr)
	{
		pObject2D->Init(pos, mylib_const::DEFVEC3);

		pObject2D->SetSize(DEF_SIZE);
		pObject2D->SetPosDest(posDest);
		pObject2D->SetSpeed(fSpeed);

		pObject2D->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pObject2D;
}
