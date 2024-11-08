//===============================================
//
// wi(bg.cpp)
// Author ϋόc ΐΛ
//
//===============================================

//============================
// CN[ht@C
//============================
#include "cursor.h"
#include "texture.h"
#include "input.h"

//============================
// }Nθ`
//============================
#define CSR_DIST (SCREEN_HEIGHT * 0.35f)		//J[\ΜΚuiΌaj

//============================
// ΓIoΟιΎ
//============================
LPDIRECT3DTEXTURE9 CCursor::m_pTexture[CCursor::TEX_MAX] = {};

//============================
// t@CΌθ`
//============================
static const char *c_apTexpass[CCursor::TEX_MAX] =
{
	"data\\TEXTURE\\CURSOR\\Aim.png",
	"data\\TEXTURE\\CURSOR\\Aim.png",
	"data\\TEXTURE\\CURSOR\\Aim.png",
};

//============================
// RXgN^
//============================
CCursor::CCursor()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Defpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bMouse = true;
}

//============================
// fXgN^
//============================
CCursor::~CCursor()
{

}

//====================================
// 2D|SΜϊ»(ftH)
//====================================
HRESULT CCursor::Init(void)
{
	CObject2D::Init();

	SetType(TYPE_BG);

	return S_OK;
}

//====================================
// 2D|SΜϊ»(ΚuIo)
//====================================
HRESULT CCursor::Init(const D3DXVECTOR3 pos)
{
	CObject2D::Init();

	CObject2D::SetPos(pos);

	SetType(TYPE_BG);

	return S_OK;
}

//============================
// 2D|SΜIΉ
//============================
void CCursor::Uninit(void)
{
	//©gΜjό
	CObject2D::Uninit();

	//Release();
}

//============================
// 2D|SΜXV
//============================
void CCursor::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CInputGamepad *pInputGamepad = CManager::GetInputGamepad();
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputMouse *pInputMouse = CManager::GetInputMouse();

	if (m_bMouse)
	{//}EXμ
		pos += pInputMouse->GetChangePos();
	
		if (pos.x >= SCREEN_WIDTH - GetSize().x)
		{//Eΐ
			pos.x = SCREEN_WIDTH - GetSize().x;
		}
		else if (pos.x <= 0.0f + GetSize().x)
		{//Άΐ
			pos.x = 0.0f + GetSize().x;
		}

		if (pos.y >= SCREEN_HEIGHT - GetSize().y)
		{//Ίΐ
			pos.y = SCREEN_HEIGHT - GetSize().y;
		}
		else if (pos.y <= 0.0f + GetSize().y)
		{//γΐ
			pos.y = 0.0f + GetSize().x;
		}
	}
	else
	{//}EXΘO
		if (pInputKeyboard->GetPress(DIK_DOWN) ||
			pInputGamepad->GetGameStickRYPress(0) < 0)
		{
			move.y += 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_UP) ||
			pInputGamepad->GetGameStickRYPress(0) > 0)
		{
			move.y -= 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_LEFT) ||
			pInputGamepad->GetGameStickRXPress(0) < 0)
		{
			move.x -= 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_RIGHT) ||
			pInputGamepad->GetGameStickRXPress(0) > 0)
		{
			move.x += 1.0f;
		}

		if (move == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			move = m_move;
		}
		else
		{
			m_move = move;
		}

		pos = m_Defpos;
	
		float fAngle = -atan2f(move.y, move.x) + D3DX_PI * 0.5f;
		pos.x += sinf(fAngle) * CSR_DIST;
		pos.y += cosf(fAngle) * CSR_DIST;
	}

	if (pInputMouse->GetPress(CInputMouse::BUTTON_RIGHT))
	{
		m_bMouse = !m_bMouse;
	}


	SetPos(pos);

	m_vec = pos - m_Defpos;
}

//============================
// 2D|SΜ`ζ
//============================
void CCursor::Draw(void)
{
	CObject2D::Draw();
}

//============================
// 2D|SΜΆ¬
//============================
CCursor * CCursor::Create(void)
{
	CCursor *pBg;

	//IuWFNgΆ¬
	pBg = new CCursor;

	//ϊ»
	pBg->Init();

	return pBg;
}

//============================
// 2D|SΜΆ¬
//============================
CCursor * CCursor::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX TexType)
{
	CCursor *pCursor = nullptr;

	//IuWFNgΆ¬&ϊ»
	if (pCursor == nullptr)
	{
		pCursor = new CCursor;

		if (pCursor != nullptr)
		{
			pCursor->Init(pos);

			//S_πEBhEΜSΙ·ι
			pCursor->m_Defpos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
			pCursor->SetSize(size);

			CTexture *pTexture = CManager::GetTexture();
			pCursor->SetIdxTexture(pTexture->Regist(c_apTexpass[TexType]));
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	return pCursor;
}

//============================
// xNgπRotΖ΅Δn·
//============================
D3DXVECTOR3 CCursor::GetRot()
{
	D3DXVECTOR3 pos = GetPos();

	D3DXVECTOR3 vecDiff = D3DXVECTOR3(pos.x - m_Defpos.x,
		pos.y - m_Defpos.y,
		0.0f);

	//D3DXVec3Normalize(&vecDiff, &vecDiff);		//xNgπ³K»·ι(1.0fΙ·ι)

	float fAngle = atan2f(vecDiff.y, vecDiff.x) + D3DX_PI * -0.5f;

	return D3DXVECTOR3(0.0f, fAngle, 0.0f);
}
