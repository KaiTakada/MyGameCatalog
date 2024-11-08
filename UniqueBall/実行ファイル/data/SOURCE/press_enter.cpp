//===============================================
//
// 2D|S(object2D.cpp)
// Author ϋόc ΐΛ
//
//===============================================

//============================
// CN[ht@C
//============================
#include "press_enter.h"
#include "texture.h"

//============================
// }Nθ`
//============================

//============================
// OϋιΎ
//============================
//class CPlayer;

//============================
// ΓIoΟιΎ
//============================

//============================
// RXgN^(ftH)
//============================
CPress_Enter::CPress_Enter(int nPriority) : CObject2D(nPriority)
{
	m_nAlphaCnter = 0;						//PRESS ENTERΜ_ΕΤuΜ»θ
	m_nTimeInterval = 0;					//PRESS ENTERΜ_ΕΤu
	m_fAlphaColor = 0.0f;					//Έ_J[
}

//============================
// fXgN^
//============================
CPress_Enter::~CPress_Enter()
{

}

//====================================
// 2D|SΜϊ»(ftH)
//====================================
HRESULT CPress_Enter::Init(void)
{
	m_nTimeInterval = 25;						//_ΕΤuπ¬³­·ι
	m_fAlphaColor = 1.0f;
	m_nAlphaCnter = 0;

	CObject2D::Init();
	SetAbrend(true);

	SetType(TYPE_ENTER);

	return S_OK;
}

//====================================
// 2D|SΜϊ»(ΚuIo)
//====================================
HRESULT CPress_Enter::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_nTimeInterval = 50;						//_ΕΤuπ¬³­·ι
	m_fAlphaColor = 1.0f;
	m_nAlphaCnter = 0;

	CObject2D::Init(pos, rot);
	SetAbrend(true);

	SetType(TYPE_ENTER);

	return S_OK;
}

//============================
// 2D|SΜIΉ
//============================
void CPress_Enter::Uninit(void)
{
	CObject2D::Uninit();
}

//============================
// 2D|SΜXV
//============================
void CPress_Enter::Update(void)
{
	CObject2D::Update();

	m_nAlphaCnter++;

	if (m_nAlphaCnter % m_nTimeInterval == 0)
	{//0.5bΑ΅½η

		m_nAlphaCnter = 0;
		D3DXCOLOR col = GetTexCol();

		if (col.a < 1.0f)
		{
			col.a = 1.0f;
			col.r = 1.0f;
			col.b = 1.0f;
			col.g = 1.0f;
		}
		else if (col.a >= 1.0f)
		{
			col.a = 0.0f;
			col.r = 0.0f;
			col.b = 0.0f;
			col.g = 0.0f;
		}

		m_fAlphaColor = col.a;
		SetTexCol(col);
	}
}

//============================
// 2D|SΜ`ζ
//============================
void CPress_Enter::Draw(void)
{
	CObject2D::Draw();
}

//============================
// 2D|SΜΆ¬
//============================
CPress_Enter * CPress_Enter::Create(void)
{
	CPress_Enter *pEnter;

	//IuWFNgΆ¬
	pEnter = new CPress_Enter;

	//ϊ»
	pEnter->Init();

	return pEnter;
}

//============================
// 2D|SΜΆ¬
//============================
CPress_Enter * CPress_Enter::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 rot)
{
	CPress_Enter *pEnter = new CPress_Enter;

	if (pEnter != nullptr)
	{
		pEnter->Init(pos, rot);
		
		pEnter->SetSize(size);

		//eNX`t^
		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		pEnter->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\PressEnter00.png"));
	}
	else
	{
		return nullptr;
	}

	return pEnter;
}
