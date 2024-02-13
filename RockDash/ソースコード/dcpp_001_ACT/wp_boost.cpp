//==================================================
//
// ブースター(wp_boost.cpp) 継承：CWeapon
// Author : Kai Takada
//
//==================================================
#include "wp_boost.h"
#include "weapon.h"
#include "Xmodel.h"
#include "manager.h"
#include "bullet.h"

#include "sound.h"
#include "particle.h"

//=======================
// マクロ定義
//=======================
#define READ_SIZE (128)		//読込ポインタサイズ

//=======================
// 静的メンバ変数宣言
//=======================

//=======================
// コンストラクタ
//=======================
CBoost::CBoost(int nPriority) : CWeapon(nPriority)
{

}

//=======================
// デストラクタ
//=======================
CBoost::~CBoost()
{

}

//=======================
// 初期化処理
//=======================
HRESULT CBoost::Init(void)
{
	CWeapon::Init();
	
	SetType(TYPE_WEAPON);

	return S_OK;
}

//=======================
// 初期化処理
//=======================
HRESULT CBoost::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CWeapon::Init(pos, rot, CWeapon::WPNTYPE_BOOST);

	SetType(TYPE_WEAPON);

	return S_OK;
}

//=======================
// 終了処理
//=======================
void CBoost::Uninit(void)
{
	CWeapon::Uninit();
}

//=======================
// 更新処理
//=======================
void CBoost::Update(void)
{
	CWeapon::Update();
}

//=======================
// 描画処理
//=======================
void CBoost::Draw(void)
{
	CWeapon::Draw();
}

//=======================
// 攻撃処理
//=======================
void CBoost::Attack(const D3DXVECTOR3 rot, const int nDamage)
{
	D3DXMATRIX mtxworld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 mtxrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos.x = mtxworld._41;
	pos.y = mtxworld._42;
	pos.z = mtxworld._43;

	mtxrot.x = mtxworld._11 * mtxworld._21 * mtxworld._31;
	mtxrot.y = mtxworld._12 * mtxworld._22 * mtxworld._32;
	mtxrot.z = mtxworld._13 * mtxworld._23 * mtxworld._33;

	//ブーストエフェクト
	CParticle::Create(pos, rot, D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 30.0f), CParticle::TYPE_BOOST);
}

//=======================
// 生成処理
//=======================
CBoost *CBoost::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	CBoost *pWeapon = nullptr;

	//オブジェクト生成&初期化
	pWeapon = new CBoost;

	if (pWeapon != nullptr)
	{
		if (FAILED(pWeapon->Init(pos, rot)))
		{
			return nullptr;
		}
		else
		{
			//モデルの割り当ては親で一括で行う
		}
	}
	else
	{
		return nullptr;
	}

	return pWeapon;
}
