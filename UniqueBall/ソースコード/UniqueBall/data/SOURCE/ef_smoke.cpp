//===============================================
//
// GtFNg(ef_smoke.cpp) p³ : effect
// Author: ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "manager.h"	//å³
#include "ef_smoke.h"		//³
#include "texture.h"	//`æeNX`
#include "game.h"	//Q[
#include "player.h"	//vC[

//============================
// }Nè`
//============================
namespace
{
	const float NUM_SPEED = 20.0f;	//Ú®Ê
	const D3DXVECTOR3 NUM_SIZE = D3DXVECTOR3(50.0f, 50.0f, 50.0f);				//ftHgTCY
	const float NUM_SCALE = 1.07f;				//ftHgk¬x
}

//============================
// ÃIoÏéŸ
//============================

//============================
// t@CŒè`
//============================
static const char *c_pTexSmoke = "data\\TEXTURE\\EFFECT\\effect002.jpg";

//============================================
//==================== e ====================
//============================
// RXgN^
//============================
CEf_Smoke::CEf_Smoke(int nPriority) : CEffect(nPriority)
{
	m_fScaleFactor = 0.0f;
}

//============================
// fXgN^
//============================
CEf_Smoke::~CEf_Smoke()
{

}

//====================================
// Ìú»(ftH)
//====================================
HRESULT CEf_Smoke::Init(void)
{
	CEffect::Init();

	m_fScaleFactor = NUM_SCALE;

	//F
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	CObject::SetType(TYPE_EFFECT);

	return S_OK;
}

//====================================
// Ìú»(ÊuIo)
//====================================
HRESULT CEf_Smoke::Init(const D3DXVECTOR3 pos)
{
	CEffect::Init(pos);

	m_fScaleFactor = NUM_SCALE;

	//F
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//^CvÝè
	CObject::SetType(TYPE_EFFECT);

	return S_OK;
}

//============================
// ÌI¹
//============================
void CEf_Smoke::Uninit(void)
{
	CEffect::Uninit();
}

//============================
// ÌXV
//============================
void CEf_Smoke::Update(void)
{
	//ÊuÏŠ
	D3DXVECTOR3 pos = GetPos();
	pos += GetMove();
	SetPos(pos);

	//TCYÏŠ
	D3DXVECTOR3 size = GetSize();
	size *= m_fScaleFactor;
	SetSize(size);

	//Fåò¹
	D3DXCOLOR col = GetColor();
	col.r *= 0.95f;
	col.g *= 0.95f;
	col.b *= 0.95f;
	SetColor(col);

	//õœ
	int nLife = GetLife();
	if (nLife <= 0)
	{
		Uninit();
		return;
	}

	//TCY
	if (size.x <= 0.0f ||
		size.y <= 0.0f ||
		size.z <= 0.0f)
	{
		Uninit();
		return;
	}

	//õœíè
	nLife--;
	SetLife(nLife);
}

//============================
// Ì`æ
//============================
void CEf_Smoke::Draw(void)
{
	CEffect::Draw();
}

//============================
// Ì¶¬
//============================
CEf_Smoke * CEf_Smoke::Create( D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CEf_Smoke *pSmoke= nullptr;

	//IuWFNg¶¬&ú»
	pSmoke= new CEf_Smoke;

	if (pSmoke!= nullptr)
	{
		if (FAILED(pSmoke->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			pSmoke->SetSize(size);
			pSmoke->SetLight(true);

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pSmoke->SetIdxTexture(pTexture->Regist(c_pTexSmoke));
		}
	}
	else
	{
		return nullptr;
	}

	return pSmoke;
}
