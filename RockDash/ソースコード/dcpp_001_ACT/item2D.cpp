//===============================================
//
// 数字(number.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "item2D.h"
#include "object.h"
#include "texture.h"
#include "player.h"
#include "weapon.h"
#include "game.h"

//============================
// マクロ定義
//============================
#define NUM_SPEED (0.0f)	//移動量
#define NUM_SIZE D3DXVECTOR2(100.0f,100.0f)				//デフォルトサイズ

//============================
// テクスチャパス
//============================
static const char *c_apTexpassItem2D[CItem2D::TYPE_MAX] =
{
	"data\\TEXTURE\\ITEM\\000_heal.png",
	"data\\TEXTURE\\ITEM\\001_powerup.png",
	"data\\TEXTURE\\ITEM\\002_speedup.png",
	"data\\TEXTURE\\ITEM\\004_Cooltime.png",
};

//============================
// 静的メンバ変数宣言
//============================
LPDIRECT3DTEXTURE9 CItem2D::m_pTexture = nullptr;				//共通テクスチャバッファ

//============================================
//==================== 弾 ====================
//============================
// コンストラクタ
//============================
CItem2D::CItem2D()
{
	m_type = CItem2D::TYPE_000_HEAL;
}

//============================
// デストラクタ
//============================
CItem2D::~CItem2D()
{

}

//====================================
// 初期化(デフォ)
//====================================
HRESULT CItem2D::Init(void)
{
	//初期化
	CObject2D::Init();

	CObject::SetType(TYPE_ITEM2D);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CItem2D::Init(const D3DXVECTOR3 pos)
{
	//初期化
	if(FAILED(CObject2D::Init(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f))))
	{
		return E_FAIL;
	}

	//タイプ設定
	CObject::SetType(TYPE_ITEM2D);

	//サイズ設定
	SetSize(D3DXVECTOR3(NUM_SIZE.x, NUM_SIZE.y, 0.0f));

	return S_OK;
}

//============================
// 終了
//============================
void CItem2D::Uninit(void)
{
	//終了
	CObject2D::Uninit();
}

//============================
// 更新
//============================
void CItem2D::Update(void)
{
	//更新
	CObject2D::Update();
}

//============================
// 描画
//============================
void CItem2D::Draw(void)
{
	//描画
	CObject2D::Draw();
}

//============================
// 生成
//============================
CItem2D *CItem2D::Create(const D3DXVECTOR3 pos, const TYPE type, const D3DXVECTOR3 size)
{
	CItem2D *pNumber = nullptr;

	//オブジェクト生成&初期化
	pNumber = new CItem2D;

	if (pNumber != nullptr)
	{
		if (FAILED(pNumber->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			pNumber->SetSize(size);		//サイズ設定

			pNumber->m_type = type;

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pNumber->SetIdxTexture(pTexture->Regist(c_apTexpassItem2D[type]));
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
void CItem2D::SetType(TYPE type)
{
	m_type = type;

	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CObject2D::SetIdxTexture(pTexture->Regist(c_apTexpassItem2D[type]));
}

//============================
// プレイヤーに対象を与える
//============================
void CItem2D::PlayerAdd()
{
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer == nullptr)
	{
		return;
	}

	CPlayer::Param param = pPlayer->GetParam();

	switch (m_type)
	{
	case CItem2D::TYPE_000_HEAL:
		param.fLife += 30.0f;
		pPlayer->Damage(-30.0f);
		break;
	case CItem2D::TYPE_001_ATTACK:
		param.fAttack += 1.0f;
		break;
	case CItem2D::TYPE_002_SPEED:
		param.fSpeed *= 1.1f;
		break;
	default:
		assert(false);
		break;
	}

	pPlayer->SetParam(param);
}
