//===============================================
//
// 背景(bg.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "cursor.h"
#include "texture.h"
#include "input.h"

//============================
// マクロ定義
//============================
#define CSR_DIST (SCREEN_HEIGHT * 0.35f)		//カーソルの位置（半径）

//============================
// 静的メンバ変数宣言
//============================
LPDIRECT3DTEXTURE9 CCursor::m_pTexture[CCursor::TEX_MAX] = {};

//============================
// ファイル名定義
//============================
static const char *c_apTexpass[CCursor::TEX_MAX] =
{
	"data\\TEXTURE\\CURSOR\\Aim.png",
	"data\\TEXTURE\\CURSOR\\Aim.png",
	"data\\TEXTURE\\CURSOR\\Aim.png",
};

//============================
// コンストラクタ
//============================
CCursor::CCursor()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Defpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bMouse = true;
}

//============================
// デストラクタ
//============================
CCursor::~CCursor()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CCursor::Init(void)
{
	CObject2D::Init();

	SetType(TYPE_BG);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CCursor::Init(const D3DXVECTOR3 pos)
{
	CObject2D::Init();

	CObject2D::SetPos(pos);

	SetType(TYPE_BG);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CCursor::Uninit(void)
{
	//自身の破棄
	CObject2D::Uninit();

	//Release();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CCursor::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (m_bMouse)
	{//マウス操作
		pos += pInputMouse->GetChangePos();
	
		if (pos.x >= SCREEN_WIDTH - GetSize().x)
		{//右限
			pos.x = SCREEN_WIDTH - GetSize().x;
		}
		else if (pos.x <= 0.0f + GetSize().x)
		{//左限
			pos.x = 0.0f + GetSize().x;
		}

		if (pos.y >= SCREEN_HEIGHT - GetSize().y)
		{//下限
			pos.y = SCREEN_HEIGHT - GetSize().y;
		}
		else if (pos.y <= 0.0f + GetSize().y)
		{//上限
			pos.y = 0.0f + GetSize().x;
		}
	}
	else
	{//マウス以外
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
// 2Dポリゴンの描画処理
//============================
void CCursor::Draw(void)
{
	CObject2D::Draw();
}

//============================
// 2Dポリゴンの生成処理
//============================
CCursor * CCursor::Create(void)
{
	CCursor *pBg;

	//オブジェクト生成
	pBg = new CCursor;

	//初期化
	pBg->Init();

	return pBg;
}

//============================
// 2Dポリゴンの生成処理
//============================
CCursor * CCursor::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const TEX TexType)
{
	CCursor *pCursor = nullptr;

	//オブジェクト生成&初期化
	if (pCursor == nullptr)
	{
		pCursor = new CCursor;

		if (pCursor != nullptr)
		{
			pCursor->Init(pos);

			//中心点をウィンドウの中心にする
			pCursor->m_Defpos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
			pCursor->SetSize(size);

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
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
// ベクトルをRotとして渡す
//============================
D3DXVECTOR3 CCursor::GetRot()
{
	D3DXVECTOR3 pos = GetPos();

	D3DXVECTOR3 vecDiff = D3DXVECTOR3(pos.x - m_Defpos.x,
		pos.y - m_Defpos.y,
		0.0f);

	//D3DXVec3Normalize(&vecDiff, &vecDiff);		//ベクトルを正規化する(1.0fにする)

	float fAngle = atan2f(vecDiff.y, vecDiff.x) + D3DX_PI * -0.5f;

	return D3DXVECTOR3(0.0f, fAngle, 0.0f);
}
