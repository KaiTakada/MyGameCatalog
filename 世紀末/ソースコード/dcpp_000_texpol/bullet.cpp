//===============================================
//
// e(bullet.cpp) p³ : objectBillboard
// Author ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "manager.h"	//å³
#include "bullet.h"		//³
#include "texture.h"	//`æeNX`
#include "effect.h"		//OÕ
#include "particle.h"	//GtFNg
#include "explosion.h"	//GtFNg
#include "game.h"		//XRAÁZ
#include "score.h"		//XRAÁZ
#include "object.h"		//½è»è
#include "enemy.h"		//½è»è
#include "player.h"		//½è»è

//============================
// }Nè`
//============================
#define NUM_SPEED (20.0f)	//Ú®Ê
#define NUM_SIZE D3DXVECTOR3(50.0f,50.0f,50.0f)				//ftHgTCY
#define FILE_PASS "data\\TEXTURE\\bullet000.png"	//t@CpX

//============================
// ÃIoÏé¾
//============================
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = nullptr;				//¤ÊeNX`obt@
int CBullet::m_nNumAll = 0;		//

//============================================
//==================== e ====================
//============================
// RXgN^
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
// fXgN^
//============================
CBullet::~CBullet()
{
	m_nNumAll--;
}

//====================================
// 2DeÌú»(ftH)
//====================================
HRESULT CBullet::Init(void)
{
	//ú»
	CObjectBillboard::Init();

	CObjectBillboard::SetLight(true);

	SetType(TYPE_BULLET);

	return S_OK;
}

//====================================
// 2DeÌú»(ÊuIo)
//====================================
HRESULT CBullet::Init(const D3DXVECTOR3 pos)
{
	//ú»
	if(FAILED(CObjectBillboard::Init(pos)))
	{
		return E_FAIL;
	}

	CObjectBillboard::SetLight(true);

	//^CvÝè
	SetType(TYPE_BULLET);

	//TCYÝè
	SetSize(D3DXVECTOR3(NUM_SIZE.x, NUM_SIZE.y, NUM_SIZE.z));

	return S_OK;
}

//============================
// 2DeÌI¹
//============================
void CBullet::Uninit(void)
{
	//I¹
	CObjectBillboard::Uninit();
}

//============================
// 2DeÌXV
//============================
void CBullet::Update(void)
{
	//ÀWæ¾
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

	//æÊO»è


	//â½»è
	if (m_nLife <= 0)
	{
		CParticle::Create(pos, rot, D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(25.0f, 25.0f, 25.0f), CParticle::TYPE_EXPLODE);
		CExplosion::Create(pos, D3DXVECTOR3(50.0f, 50.0f, 0.0f));

		Release();
		return;
	}

	//GÆÌ½è»è
	if (CollisionCircle(pos))
	{//½Á½ç
		CScore *pScore = CGame::GetScore();
		
		if (pScore != nullptr)
		{
			pScore->CntValue(100);
		}

		return;
	}

	//ubNÆÌ»è
	//if (CollisionRect())
	//{
	//	return;
	//}

	//½ÌJEg_E
	m_nLife--;

	//JE^[ÁZ
	m_nEffCtr++;
}

//============================
// 2DeÌ`æ
//============================
void CBullet::Draw(void)
{
	//`æ
	CObjectBillboard::Draw();
}

//============================
// eÌ¶¬
//============================
CBullet * CBullet::Create( D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const SIDE side, const int nDamage, const D3DXVECTOR3 size, const float fSpeed, const int nLife)
{
	CBullet *pBullet = nullptr;

	//IuWFNg¶¬&ú»
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

			pBullet->m_side = side;		//®

			pBullet->m_nLife = nLife;		//ÌÍãü

			pBullet->SetSize(size);		//TCYÝè

			CTexture *pTexture = CManager::GetTexture();
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
// GÆÌ½è»è(~)
//============================
bool CBullet::CollisionCircle(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 size = GetSize();
	bool bCol = false;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject *pObj;

			//IuWFNgðæ¾
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULL`FbN
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_BOSS || type == CObject::TYPE_PLAYER)
				{//G¾Á½ç
				
					 D3DXVECTOR3 Objpos = pObj->GetPos();

					 D3DXVECTOR3 Objsize;
					 
					 type == CObject::TYPE_PLAYER ?
						 Objsize = pObj->GetSize() :
						 Objsize = pObj->GetMaxVtx();

					 //3²gÁ½Ì»è©ç1²Ì~Ì»èÉ
					float fSize0 = (size.x + size.z) * 0.5f;		//ACeÌ¼axz
					//float fSize1 = (size.x + size.y) * 0.5f;		//ACeÌ¼axy
					//float fSize2 = (size.z + size.y) * 0.5f;		//ACeÌ¼azy

					float fObjsize0 = (Objsize.x + Objsize.z) * 0.5f;		//IuWFNgÌ¼axz
					//float fObjsize1 = (Objsize.x + Objsize.y) * 0.5f;		//IuWFNgÌ¼axy
					//float fObjsize2 = (Objsize.z + Objsize.y) * 0.5f;		//IuWFNgÌ¼azy

					float fColl0 = fSize0 + fObjsize0;		//½è»èÍÍ
					//float fColl1 = fSize1 + fObjsize1;		//½è»èÍÍ
					//float fColl2 = fSize2 + fObjsize2;		//½è»èÍÍ

					float fLength0 = hypotf((pos.x - Objpos.x), (pos.z - Objpos.z));		//2_ÔÌ·³xz
					//float fLength1 = hypotf((pos.x - Objpos.x), (pos.y - Objpos.y));		//2_ÔÌ·³xy
					//float fLength2 = hypotf((pos.z - Objpos.z), (pos.y - Objpos.y));		//2_ÔÌ·³zy

					//if (fLength0 <= fColl0 &&
					//	fLength1 <= fColl1 &&
					//	fLength2 <= fColl2)

					if (fLength0 <= fColl0 && pos.y <= 110.0f)
					{
						//_Ci~bNLXg
						if (type == CObject::TYPE_PLAYER && m_side != SIDE_PLAYER)
						{
							CPlayer *pPlayer= dynamic_cast<CPlayer*>(pObj);
							pPlayer->Damage(float(m_nDamage));
							pPlayer->CntExp(0.1f);
							bCol = true;
						}
						else if (type != CObject::TYPE_PLAYER && m_side != SIDE_ENEMY)
						{
							CEnemy *pEnemy = dynamic_cast<CEnemy*>(pObj);
							pEnemy->Damage(m_nDamage);
							bCol = true;
						}

						if (bCol)
						{
							//­¶¬
							CExplosion::Create(pos, D3DXVECTOR3(10.0f, 10.0f, 10.0f));

							//eI¹
							Uninit();

							return bCol;
						}
					}
				}
			}
		}
	}

	return bCol;
}

//============================
//ubNÆÌ½è»è(é`)
//============================
bool CBullet::CollisionRect(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();
	D3DXVECTOR3 posOld = pos - GetMove();

	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	bool bCollision = false;			//½è»èª Á½©Ç¤©

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			bCollision = false;

			CObject *pObj;

			//IuWFNgðæ¾
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULL`FbN
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_BLOCK)
				{//vC\¾Á½ç
					Objpos = pObj->GetPos();
					Objsize = pObj->GetSize();

					if (Objpos.y + Objsize.y <= posOld.y - size.y
						&& Objpos.y + Objsize.y >= pos.y - size.y
						&& Objpos.x + Objsize.x >= pos.x - size.x + 0.1f
						&& Objpos.x - Objsize.x <= pos.x + size.x - 0.1f)
					{//ubNã
						bCollision = true;
					}
					else if (Objpos.y - Objsize.y >= posOld.y + size.y
						&& Objpos.y - Objsize.y <= pos.y + size.y
						&& Objpos.x + Objsize.x >= pos.x - size.x + 0.1f
						&& Objpos.x - Objsize.x <= pos.x + size.x - 0.1f)
					{//ubNº
						bCollision = true;
					}

					if (Objpos.x + Objsize.x <= posOld.x - size.x + 0.1f
						&& Objpos.x + Objsize.x >= pos.x - size.x + 0.1f
						&& Objpos.y + Objsize.y > pos.y - size.y
						&& Objpos.y - Objsize.y < pos.y + size.y)
					{//ubN¶
						bCollision = true;
					}
					else if (Objpos.x - Objsize.x >= posOld.x + size.x - 0.1f
						&& Objpos.x - Objsize.x <= pos.x + size.x - 0.1f
						&& Objpos.y + Objsize.y > pos.y - size.y
						&& Objpos.y - Objsize.y < pos.y + size.y)
					{//ubNE
						bCollision = true;
					}

					if (bCollision)
					{
						//­¶¬
						CExplosion::Create(pos, D3DXVECTOR3(50.0f, 50.0f, 0.0f));

						Release();

						//ubNÉ_[WH
						return bCollision;
					}
				}
			}
		}
	}

	return bCollision;
}
