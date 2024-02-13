//===============================================
//
// 弾(bullet.cpp) 継承 : objectBillboard
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "manager.h"	//大元
#include "bullet.h"		//元
#include "texture.h"	//描画テクスチャ
#include "effect.h"		//軌跡
#include "particle.h"	//エフェクト
#include "explosion.h"	//エフェクト
#include "game.h"		//スコア加算
#include "score.h"		//スコア加算
#include "object.h"		//当たり判定
#include "enemy.h"		//当たり判定
#include "player.h"		//当たり判定

//============================
// マクロ定義
//============================
#define NUM_SPEED (20.0f)	//移動量
#define NUM_SIZE D3DXVECTOR3(50.0f,50.0f,50.0f)				//デフォルトサイズ
#define FILE_PASS "data\\TEXTURE\\bullet000.png"	//ファイルパス

//============================
// 静的メンバ変数宣言
//============================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = nullptr;				//共通テクスチャバッファ
int CBullet::m_nNumAll = 0;		//総数

//============================================
//==================== 弾 ====================
//============================
// コンストラクタ
//============================
CBullet::CBullet(int nPriority) : CObjectBillboard(nPriority)
{
	m_nLife = 0;
	m_nEffCtr = 0;
	m_side = SIDE_NONE;
	m_nDamage = 0;

	m_nNumAll++;
}

//============================
// デストラクタ
//============================
CBullet::~CBullet()
{
	m_nNumAll--;
}

//====================================
// 2D弾の初期化(デフォ)
//====================================
HRESULT CBullet::Init(void)
{
	//初期化
	CObjectBillboard::Init();

	CObjectBillboard::SetLight(true);

	SetType(TYPE_BULLET);

	return S_OK;
}

//====================================
// 2D弾の初期化(位置オバロ)
//====================================
HRESULT CBullet::Init(const D3DXVECTOR3 pos)
{
	//初期化
	if(FAILED(CObjectBillboard::Init(pos)))
	{
		return E_FAIL;
	}

	CObjectBillboard::SetLight(true);

	//タイプ設定
	SetType(TYPE_BULLET);

	//サイズ設定
	SetSize(D3DXVECTOR3(NUM_SIZE.x, NUM_SIZE.y, NUM_SIZE.z));

	return S_OK;
}

//============================
// 2D弾の終了
//============================
void CBullet::Uninit(void)
{
	//終了
	CObjectBillboard::Uninit();
}

//============================
// 2D弾の更新
//============================
void CBullet::Update(void)
{
	//座標取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	pos += GetMove();

	SetPos(pos);

	CObjectBillboard::Update();

	if (m_nEffCtr >= 1)
	{
		//CEffect::Create(pos, GetRot(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 60, CEffect::TEX_000);
	
		m_nEffCtr = 0;
	}

	//画面外判定


	//絶命判定
	if (m_nLife <= 0)
	{
		CParticle::Create(pos, rot, D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(25.0f, 25.0f, 25.0f), CParticle::TYPE_EXPLODE);
		CExplosion::Create(pos, D3DXVECTOR3(50.0f, 50.0f, 0.0f));

		Release();
		return;
	}

	//敵との当たり判定
	if (CollisionCircle(pos))
	{//当たったら
		CScore *pScore = CGame::GetScore();
		
		if (pScore != nullptr)
		{
			pScore->CntValue(100);
		}

		return;
	}

	//ブロックとの判定
	//if (CollisionRect())
	//{
	//	return;
	//}

	//命のカウントダウン
	m_nLife--;

	//カウンター加算
	m_nEffCtr++;
}

//============================
// 2D弾の描画
//============================
void CBullet::Draw(void)
{
	//描画
	CObjectBillboard::Draw();
}

//============================
// 弾の生成
//============================
CBullet * CBullet::Create( D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const SIDE side, const int nDamage, const D3DXVECTOR3 size, const float fSpeed, const int nLife)
{
	CBullet *pBullet = nullptr;

	//オブジェクト生成&初期化
	pBullet = new CBullet;

	if (pBullet != nullptr)
	{
		if (FAILED(pBullet->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			pBullet->m_nDamage = nDamage;

			pBullet->m_side = side;		//所属

			pBullet->m_nLife = nLife;		//体力代入

			pBullet->SetSize(size);		//サイズ設定

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pBullet->SetIdxTexture(pTexture->Regist(FILE_PASS));

			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			move.x = sinf(rot.y) * -fSpeed;
			move.z = cosf(rot.y) * -fSpeed;
			//pBullet->m_move.y = cosf(rot.y) * fSpeed;
			pBullet->SetMove(move);
		}
	}
	else
	{
		return nullptr;
	}

	return pBullet;
}

//============================
// 敵との当たり判定(円柱)
//============================
bool CBullet::CollisionCircle(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 size = GetSize();
	bool bCol = false;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS || type == CObject::TYPE_PLAYER)
				{//敵だったら

					D3DXVECTOR3 Objpos = pObject->GetPos();

					D3DXVECTOR3 Objsize;

					type == CObject::TYPE_PLAYER ?
						Objsize = pObject->GetSize() :
						Objsize = pObject->GetMaxVtx();

					//3軸使った球の判定から1軸の円の判定に
					float fSize0 = (size.x + size.z) * 0.5f;		//アイテムの半径xz
					//float fSize1 = (size.x + size.y) * 0.5f;		//アイテムの半径xy
					//float fSize2 = (size.z + size.y) * 0.5f;		//アイテムの半径zy

					float fObjsize0 = (Objsize.x + Objsize.z) * 0.5f;		//オブジェクトの半径xz
					//float fObjsize1 = (Objsize.x + Objsize.y) * 0.5f;		//オブジェクトの半径xy
					//float fObjsize2 = (Objsize.z + Objsize.y) * 0.5f;		//オブジェクトの半径zy

					float fColl0 = fSize0 + fObjsize0;		//当たり判定範囲
					//float fColl1 = fSize1 + fObjsize1;		//当たり判定範囲
					//float fColl2 = fSize2 + fObjsize2;		//当たり判定範囲

					float fLength0 = hypotf((pos.x - Objpos.x), (pos.z - Objpos.z));					//2点間の長さxz
					//float fLength1 = hypotf((pos.x - Objpos.x), (pos.y - Objpos.y));		//2点間の長さxy
					//float fLength2 = hypotf((pos.z - Objpos.z), (pos.y - Objpos.y));		//2点間の長さzy

					//if (fLength0 <= fColl0 &&
					//	fLength1 <= fColl1 &&
					//	fLength2 <= fColl2)

					if (fLength0 <= fColl0 && pos.y <= 110.0f)
					{
						//ダイナミックキャスト
						if (type == CObject::TYPE_PLAYER && m_side != SIDE_PLAYER)
						{
							CPlayer *pPlayer = dynamic_cast<CPlayer*>(pObject);
							pPlayer->Damage(float(m_nDamage));
							bCol = true;
						}
						else if (type != CObject::TYPE_PLAYER && m_side != SIDE_ENEMY)
						{
							CEnemy *pEnemy = dynamic_cast<CEnemy*>(pObject);
							pEnemy->Damage(m_nDamage);
							bCol = true;
						}

						if (bCol)
						{
							//爆発生成
							CExplosion::Create(pos, D3DXVECTOR3(10.0f, 10.0f, 10.0f));

							//弾終了
							Uninit();

							return bCol;
						}
					}
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	return bCol;
}

//============================
//ブロックとの当たり判定(矩形)
//============================
bool CBullet::CollisionRect(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 posOld = pos - GetMove();

	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	bool bCollision = false;			//当たり判定があったかどうか

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_BLOCK)
				{//プレイヤ―だったら
					Objpos = pObject->GetPos();
					Objsize = pObject->GetSize();

					if (Objpos.y + Objsize.y <= posOld.y - size.y
						&& Objpos.y + Objsize.y >= pos.y - size.y
						&& Objpos.x + Objsize.x >= pos.x - size.x + 0.1f
						&& Objpos.x - Objsize.x <= pos.x + size.x - 0.1f)
					{//ブロック上
						bCollision = true;
					}
					else if (Objpos.y - Objsize.y >= posOld.y + size.y
						&& Objpos.y - Objsize.y <= pos.y + size.y
						&& Objpos.x + Objsize.x >= pos.x - size.x + 0.1f
						&& Objpos.x - Objsize.x <= pos.x + size.x - 0.1f)
					{//ブロック下
						bCollision = true;
					}

					if (Objpos.x + Objsize.x <= posOld.x - size.x + 0.1f
						&& Objpos.x + Objsize.x >= pos.x - size.x + 0.1f
						&& Objpos.y + Objsize.y > pos.y - size.y
						&& Objpos.y - Objsize.y < pos.y + size.y)
					{//ブロック左
						bCollision = true;
					}
					else if (Objpos.x - Objsize.x >= posOld.x + size.x - 0.1f
						&& Objpos.x - Objsize.x <= pos.x + size.x - 0.1f
						&& Objpos.y + Objsize.y > pos.y - size.y
						&& Objpos.y - Objsize.y < pos.y + size.y)
					{//ブロック右
						bCollision = true;
					}

					if (bCollision)
					{
						//爆発生成
						CExplosion::Create(pos, D3DXVECTOR3(50.0f, 50.0f, 0.0f));

						Release();

						//ブロックにダメージ？
						return bCollision;
					}
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	return bCollision;
}
