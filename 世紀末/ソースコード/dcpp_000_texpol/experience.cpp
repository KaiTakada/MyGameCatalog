//===============================================
//
// 経験値オーブ(experience.cpp) 継承 : effect
// Author: 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "manager.h"	//大元
#include "experience.h"		//元
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
LPDIRECT3DTEXTURE9 CExperience::m_pTexture = nullptr;				//共通テクスチャバッファ

//============================
// ファイル名定義
//============================
static const char *c_pTexExp = "data\\TEXTURE\\EXP\\exp.png";

//============================================
//==================== 弾 ====================
//============================
// コンストラクタ
//============================
CExperience::CExperience(int nPriority) : CEffect(nPriority)
{
	m_fValue = 0.0f;			//割合
}

//============================
// デストラクタ
//============================
CExperience::~CExperience()
{

}

//====================================
// 経験値の初期化(デフォ)
//====================================
HRESULT CExperience::Init(void)
{
	m_fValue = 0.0f;

	CEffect::Init();

	SetType(TYPE_EXP);

	return S_OK;
}

//====================================
// 経験値の初期化(位置オバロ)
//====================================
HRESULT CExperience::Init(const D3DXVECTOR3 pos)
{
	m_fValue = 0.0f;

	CEffect::Init(pos);

	//CObjectBillboard::SetAbrend(false);

	//タイプ設定
	SetType(TYPE_EXP);

	return S_OK;
}

//============================
// 経験値の終了
//============================
void CExperience::Uninit(void)
{
	CEffect::Uninit();
}

//============================
// 経験値の更新
//============================
void CExperience::Update(void)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 pos = GetPos();

	pos += move;

	SetPos(pos);

	CollisionBall();
}

//============================
// 経験値の描画
//============================
void CExperience::Draw(void)
{
	CEffect::Draw();
}

//============================
// 経験値の生成
//============================
CExperience * CExperience::Create( D3DXVECTOR3 pos, const D3DXVECTOR3 size, const float fValue)
{
	CExperience *pExp= nullptr;

	//オブジェクト生成&初期化
	pExp= new CExperience;

	if (pExp!= nullptr)
	{
		if (FAILED(pExp->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			pExp->m_fValue = fValue;		//割合
			pExp->SetSize(size);
			pExp->SetLight(true);

			CTexture *pTexture = CManager::GetTexture();
			pExp->SetIdxTexture(pTexture->Regist(c_pTexExp));
		}
	}
	else
	{
		return nullptr;
	}

	return pExp;
}

//============================
// 当たり判定 (球と球)
//============================
bool CExperience::CollisionBall(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject *pObj;

			//オブジェクトを取得
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULLチェック
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_PLAYER)
				{//プレイヤ―だったら
					Objpos = pObj->GetPos();
					Objsize = pObj->GetSize();

					CPlayer *pPlayer = CGame::GetPlayer();
					Objpos.y += pPlayer->GetHeartPos();

					float fSize0 = (size.x + size.z) * 0.5f;		//アイテムの半径xz
					float fSize1 = (size.x + size.y) * 0.5f;		//アイテムの半径xy
					float fSize2 = (size.z + size.y) * 0.5f;		//アイテムの半径zy

					float fObjsize0 = (Objsize.x + Objsize.z) * 0.5f;		//オブジェクトの半径xz
					float fObjsize1 = (Objsize.x + Objsize.y) * 0.5f;		//オブジェクトの半径xy
					float fObjsize2 = (Objsize.z + Objsize.y) * 0.5f;		//オブジェクトの半径zy

					float fColl0 = fSize0 + fObjsize0;		//当たり判定範囲
					float fColl1 = fSize1 + fObjsize1;		//当たり判定範囲
					float fColl2 = fSize2 + fObjsize2;		//当たり判定範囲

					float fLength0 = hypotf((pos.x - Objpos.x), (pos.z - Objpos.z));		//2点間の長さxz
					float fLength1 = hypotf((pos.x - Objpos.x), (pos.y - Objpos.y));		//2点間の長さxy
					float fLength2 = hypotf((pos.z - Objpos.z), (pos.y - Objpos.y));		//2点間の長さzy

					if (fLength0 <= fColl0 &&
						fLength1 <= fColl1 &&
						fLength2 <= fColl2)
					{
						pPlayer->CntExp(m_fValue);

						//弾終了
						Uninit();

						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}
