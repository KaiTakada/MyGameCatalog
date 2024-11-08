//===============================================
//
// o±lI[u(experience.cpp) p³ : effect
// Author: ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "manager.h"	//å³
#include "experience.h"		//³
#include "texture.h"	//`æeNX`
#include "game.h"	//Q[
#include "player.h"	//vC[

//============================
// }Nè`
//============================
#define NUM_SPEED (20.0f)	//Ú®Ê
#define NUM_SIZE D3DXVECTOR3(50.0f,50.0f,50.0f)				//ftHgTCY

//============================
// ÃIoÏé¾
//============================
LPDIRECT3DTEXTURE9 CExperience::m_pTexture = nullptr;				//¤ÊeNX`obt@

//============================
// t@C¼è`
//============================
static const char *c_pTexExp = "data\\TEXTURE\\EXP\\exp.png";

//============================================
//==================== e ====================
//============================
// RXgN^
//============================
CExperience::CExperience(int nPriority) : CEffect(nPriority)
{
	m_fValue = 0.0f;			//
}

//============================
// fXgN^
//============================
CExperience::~CExperience()
{

}

//====================================
// o±lÌú»(ftH)
//====================================
HRESULT CExperience::Init(void)
{
	m_fValue = 0.0f;

	CEffect::Init();

	SetType(TYPE_EXP);

	return S_OK;
}

//====================================
// o±lÌú»(ÊuIo)
//====================================
HRESULT CExperience::Init(const D3DXVECTOR3 pos)
{
	m_fValue = 0.0f;

	CEffect::Init(pos);

	//CObjectBillboard::SetAbrend(false);

	//^CvÝè
	SetType(TYPE_EXP);

	return S_OK;
}

//============================
// o±lÌI¹
//============================
void CExperience::Uninit(void)
{
	CEffect::Uninit();
}

//============================
// o±lÌXV
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
// o±lÌ`æ
//============================
void CExperience::Draw(void)
{
	CEffect::Draw();
}

//============================
// o±lÌ¶¬
//============================
CExperience * CExperience::Create( D3DXVECTOR3 pos, const D3DXVECTOR3 size, const float fValue)
{
	CExperience *pExp= nullptr;

	//IuWFNg¶¬&ú»
	pExp= new CExperience;

	if (pExp!= nullptr)
	{
		if (FAILED(pExp->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			pExp->m_fValue = fValue;		//
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
// ½è»è (Æ)
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

			//IuWFNgðæ¾
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULL`FbN
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_PLAYER)
				{//vC\¾Á½ç
					Objpos = pObj->GetPos();
					Objsize = pObj->GetSize();

					CPlayer *pPlayer = CGame::GetPlayer();
					Objpos.y += pPlayer->GetHeartPos();

					float fSize0 = (size.x + size.z) * 0.5f;		//ACeÌ¼axz
					float fSize1 = (size.x + size.y) * 0.5f;		//ACeÌ¼axy
					float fSize2 = (size.z + size.y) * 0.5f;		//ACeÌ¼azy

					float fObjsize0 = (Objsize.x + Objsize.z) * 0.5f;		//IuWFNgÌ¼axz
					float fObjsize1 = (Objsize.x + Objsize.y) * 0.5f;		//IuWFNgÌ¼axy
					float fObjsize2 = (Objsize.z + Objsize.y) * 0.5f;		//IuWFNgÌ¼azy

					float fColl0 = fSize0 + fObjsize0;		//½è»èÍÍ
					float fColl1 = fSize1 + fObjsize1;		//½è»èÍÍ
					float fColl2 = fSize2 + fObjsize2;		//½è»èÍÍ

					float fLength0 = hypotf((pos.x - Objpos.x), (pos.z - Objpos.z));		//2_ÔÌ·³xz
					float fLength1 = hypotf((pos.x - Objpos.x), (pos.y - Objpos.y));		//2_ÔÌ·³xy
					float fLength2 = hypotf((pos.z - Objpos.z), (pos.y - Objpos.y));		//2_ÔÌ·³zy

					if (fLength0 <= fColl0 &&
						fLength1 <= fColl1 &&
						fLength2 <= fColl2)
					{
						pPlayer->CntExp(m_fValue);

						//eI¹
						Uninit();

						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}
