//============================
//
// 生命状態管理(state_life.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "state_life.h"

//===============================
// マクロ定義
//===============================

//=================================
// コンストラクタ
//=================================
CState_Life::CState_Life()
{
	m_nCtrState = 0;
	m_state = STATE_NORMAL;
	m_pAdress = nullptr;

	ZeroMemory(&m_MatDmg, sizeof(D3DXMATERIAL));
}

//=================================
// デストラクタ
//=================================
CState_Life::~CState_Life()
{

}

//=================================
// プレイヤーの初期化処理
//=================================
HRESULT CState_Life::Init(void)
{
	m_MatDmg.MatD3D.Diffuse = D3DXCOLOR(0.7f, 0.3f, 0.3f, 1.0f);
	m_MatDmg.MatD3D.Ambient = D3DXCOLOR(0.7f, 0.3f, 0.3f, 1.0f);
	m_MatDmg.MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	return S_OK;
}

//=================================
// プレイヤーの終了処理
//=================================
void CState_Life::Uninit(void)
{
	if (m_pAdress != nullptr)
	{
		delete m_pAdress;
		m_pAdress = nullptr;
	}
}

//=================================
// プレイヤーの更新処理
//=================================
void CState_Life::Update(void)
{
	if (m_nCtrState == 0)
	{
		switch (m_state)
		{
		case STATE_DAMAGE:
			m_state = STATE_NORMAL;
			break;

		default:
			break;
		}
	}
	else if (m_nCtrState > 0)
	{
		m_nCtrState--;
	}
}

//=================================
// 生成
//=================================
CState_Life *CState_Life::Create(void)
{
	//オブジェクト生成
	CState_Life *pState= new CState_Life;

	//初期化
	pState->Init();

	pState->m_pAdress = pState;

	return pState;
}
