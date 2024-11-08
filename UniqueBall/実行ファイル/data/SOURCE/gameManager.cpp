//===============================================
//
// ゲーム(game.cpp)
// Author: 髙田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "gameManager.h"
#include "game.h"
#include "obj_polygon2D.h"
#include "number.h"
#include "Texture.h"
#include "fade.h"
#include "sound.h"

//=========================
// マクロ定義
//=========================
namespace
{
	const int COUNTDOWN_START(3);		//←ココからカウントダウン
	const int STAYTIME_START(60);		//とどまる時間
	const int ROTATETIME_START(10);		//回転に要する時間
	const char* GO_PASS("data\\TEXTURE\\SCENE//GAME//Go.png");
	const char* GAMESET_PASS("data\\TEXTURE\\SCENE//GAME//GameSet.png");
	const D3DXVECTOR3 GO_SIZE(D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, 0.0f));
}

//=========================
// 静的メンバ変数
//=========================


//==========================================================================
// 関数ポインタ
//==========================================================================
CGameManager::UPD_FUNC CGameManager::m_UpdateFuncList[] =
{
	nullptr,
	&CGameManager::UpdateStart,		// スタート
	nullptr,
	&CGameManager::UpdateGameset,	// ゲームセット
};

CGameManager::CRT_FUNC CGameManager::m_CreateFuncList[] =
{
	nullptr,
	&CGameManager::CreateStart,		// スタート
	nullptr,
	&CGameManager::CreateGameset,	// ゲームセット
};

//============================
// コンストラクタ
//============================
CGameManager::CGameManager()
{
	m_OnePoint = nullptr;
	m_pCountdown = nullptr;
	m_FlowStage = FlowStage::NONE;
	m_nStayTime = 0;
	m_nRotateTime = 0;
	m_bEnd = false;
}

//============================
// デストラクタ
//============================
CGameManager::~CGameManager()
{

}

//============================
// 初期化
//============================
HRESULT CGameManager::Init()
{
	m_bEnd = true;

	return S_OK;
}

//============================
// 終了
//============================
void CGameManager::Uninit()
{
	if (m_OnePoint != nullptr)
	{
		m_OnePoint->SetDeath();
		m_OnePoint = nullptr;
	}

	if (m_pCountdown != nullptr)
	{
		m_pCountdown->SetDeath();
		m_pCountdown = nullptr;
	}

	delete this;
}

//============================
// 更新
//============================
void CGameManager::Update()
{
	if (m_UpdateFuncList[m_FlowStage] != nullptr)
	{
		(this->*(m_UpdateFuncList[m_FlowStage]))();
	}
}

//============================
// 描画
//============================
void CGameManager::Draw()
{

}

//============================
// 生成
//============================
CGameManager * CGameManager::Create()
{
	CGameManager *pManager = nullptr;

	//オブジェクト生成&初期化
	pManager = new CGameManager;

	if (pManager != nullptr)
	{
		if (FAILED(pManager->Init()))
		{//失敗したら
			return nullptr;
		}
		else
		{

		}
	}
	else
	{
		return nullptr;
	}

	return pManager;
}

//============================
// 状況設定
//============================
void CGameManager::SetFlowStage(FlowStage set) 
{
	if (m_FlowStage == set)
	{
		return;
	}

	m_FlowStage = set; 

	if (m_CreateFuncList[m_FlowStage] != nullptr)
	{
		(this->*(m_CreateFuncList[m_FlowStage]))();
	}
}

//============================
// スタート時生成
//============================
void CGameManager::CreateStart()
{
	if (m_OnePoint != nullptr)
	{
		m_OnePoint->SetDeath();
		m_OnePoint = nullptr;
	}

	if (m_pCountdown != nullptr)
	{
		m_pCountdown->SetDeath();
		m_pCountdown = nullptr;
	}

	m_pCountdown = CNumber::Create(COUNTDOWN_START);
	m_pCountdown->SetPos(mylib_const::MID2D);
	m_nRotateTime = ROTATETIME_START;

	m_bEnd = false;
}

//============================
// ゲームセット時生成
//============================
void CGameManager::CreateGameset()
{
	if (m_OnePoint != nullptr)
	{
		m_OnePoint->SetDeath();
		m_OnePoint = nullptr;
	}

	if (m_pCountdown != nullptr)
	{
		m_pCountdown->SetDeath();
		m_pCountdown = nullptr;
	}

	m_OnePoint = CObject2D::Create();
	m_OnePoint->SetPos(mylib_const::MID2D);
	m_OnePoint->SetSize(GO_SIZE);

	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	m_OnePoint->SetIdxTexture(pTexture->Regist(GAMESET_PASS));
	m_nStayTime = STAYTIME_START;

	m_bEnd = false;
}

//============================
// スタート時更新
//============================
void CGameManager::UpdateStart()
{
	CScene* pScene = CManager::GetInstance()->GetScene();
	CGame* pGame = nullptr;
	if (pScene->GetMode() == CScene::MODE_GAME)
	{
		pGame = static_cast<CGame*>(pScene);
	}

	if (pGame->GetFade()->GetState() != CFade::STATE_NONE)
	{//フェード中は開始しない
		return;
	}

	if (m_nStayTime <= 0)
	{
		m_nStayTime = STAYTIME_START;
	
		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_COUNTDOWN);
	}
	else
	{
		m_nStayTime--;

		if (m_nStayTime <= 0)
		{//表示カウントが0になった瞬間

			if (m_pCountdown == nullptr)
			{//後の処理はカウントダウンしていた場合
			
				if (m_OnePoint != nullptr)
				{//Goのカウントが終わった
					m_OnePoint->SetDeath();
					m_OnePoint = nullptr;
					SetFlowStage(INGAME);
					m_bEnd = true;
				}

				return;
			}

			if (m_pCountdown->GetNumber() > 1)
			{//カウントダウンの余地がある場合
				m_pCountdown->SetNumber(m_pCountdown->GetNumber() - 1);
				m_nStayTime = STAYTIME_START;
			
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_COUNTDOWN);
			}
			else
			{
				if (m_pCountdown != nullptr)
				{
					m_pCountdown->SetDeath();
					m_pCountdown = nullptr;
				}

				m_OnePoint = CObject2D::Create();
				m_OnePoint->SetPos(mylib_const::MID2D);
				m_OnePoint->SetSize(GO_SIZE);

				CTexture* pTexture = CManager::GetInstance()->GetTexture();
				m_OnePoint->SetIdxTexture(pTexture->Regist(GO_PASS));
				m_nStayTime = STAYTIME_START;
		
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GO);
			}
		}
	}

	if (m_pCountdown != nullptr)
	{
		m_pCountdown->Update();
	}

	if (m_OnePoint != nullptr)
	{
		m_OnePoint->Update();
	}
}

//============================
// ゲームセット時更新
//============================
void CGameManager::UpdateGameset()
{
	if (m_nStayTime <= 0 && m_OnePoint != nullptr)
	{
		m_OnePoint->SetDeath();
		m_OnePoint = nullptr;
		m_bEnd = true;
	}

	m_nStayTime--;
}
