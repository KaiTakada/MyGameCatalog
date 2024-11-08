//===============================================
//
// Q[(game.cpp)
// Author: ûüc ÀË
//
//===============================================

//=========================
// CN[ht@C
//=========================
#include "game.h"
#include "player.h"
#include "score.h"
#include "sound.h"
#include "field.h"
#include "input.h"
#include "spawn.h"
#include "pause.h"
#include "fade.h"
#include "timer.h"
#include "growselecter.h"
#include "enemy_boss.h"
#include "result.h"

//=========================
// }Nè`
//=========================

#if _DEBUG
#define START_TIME (10)		//X^[gb
#else
#define START_TIME (120)		//X^[gb
#endif

//=========================
// ÃIoÏ
//=========================
CPlayer *CGame::m_pPlayer = nullptr;					//vC[
CPause *CGame::m_pPause = nullptr;						//|[Y
CScore *CGame::m_pScore = nullptr;						//XRA
CTimer *CGame::m_pTimer = nullptr;						//^C}[
CGrowSelecter *CGame::m_pGrow = nullptr;				//i»¬·V[
CEnemy_Boss *CGame::m_pBoss = nullptr;						//{X

//============================
// RXgN^
//============================
CGame::CGame()
{
	m_pPlayer = nullptr;
	m_pScore = nullptr;
	m_pField = nullptr;
	m_pSpawn = nullptr;
	m_pPause = nullptr;
	m_pTimer = nullptr;
	m_pGrow = nullptr;
	m_pBoss = nullptr;
	m_pResult = nullptr;

	m_bResult = false;
}

//============================
// fXgN^
//============================
CGame::~CGame()
{

}

//============================
// ú»
//============================
HRESULT CGame::Init()
{
	m_bResult = false;

	CScene::Init();

	//IuWFNgÌ¶¬
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.74f, SCREEN_HEIGHT * 0.08f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(30.0f, 50.0f, 0.0f),
		6);

	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//============ X|[ ============
	//NULL`FbN
	if (m_pSpawn != nullptr)
	{
		m_pSpawn->Uninit();
		m_pSpawn = nullptr;
	}

	if (m_pSpawn == nullptr)
	{
		m_pSpawn = CSpawn::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		return -1;
	}
	
	if (m_pTimer != nullptr)
	{
		m_pTimer = nullptr;
	}

	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.45f, SCREEN_HEIGHT * 0.1f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 50.0f, 0.0f));
	m_pTimer->SetValue(START_TIME);

	m_pBoss = CEnemy_Boss::Create(D3DXVECTOR3(1000.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//============================
// I¹
//============================
void CGame::Uninit()
{
	if (m_pSpawn != nullptr)
	{
		m_pSpawn->Uninit();
		m_pSpawn = nullptr;
	}

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	if (m_pScore != nullptr)
	{
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	if (m_pField != nullptr)
	{
		m_pField->Uninit();
		m_pField = nullptr;
	}

	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		m_pPause = nullptr;
	}

	if (m_pTimer != nullptr)
	{
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	if (m_pGrow != nullptr)
	{
		m_pGrow->Uninit();
		m_pGrow = nullptr;
	}

	if (m_pResult != nullptr)
	{
		m_pResult->Uninit();
		m_pResult = nullptr;
	}

	CScene::Uninit();

	CManager::GetSound()->Stop();
}

//============================
// XV
//============================
void CGame::Update()
{
	//L[{[hæ¾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInputGamepad();
	bool bPause = CManager::GetPause();

	CScene::Update();

	if (m_bResult == true)
	{//UgÍUgÌÝXV
		if (bPause == true)
		{
			//Ug\¦
			if (m_pResult != nullptr)
			{
				m_pResult->Update();
			}
		}
		else
		{
			CManager::InvPause();
		}

		return;
	}

	if (pInputKeyboard->GetTrigger(DIK_P) || pInputPad->GetPress(CInputGamepad::BUTTON_START, 0) == true)
	{//[ P ]L[Å|[Y

		if (m_pPause == nullptr)
		{
			m_pPause = CPause::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		}
		else
		{
			m_pPause->Uninit();
			m_pPause = nullptr;
		}

		CManager::InvPause();
	}

	if (m_pGrow != nullptr)
	{
		if (CManager::GetGrow() == false)
		{
			CManager::InvGrow();
		}

		m_pGrow->Update();
	}

	if (m_pPause != nullptr && bPause == true)
	{
		m_pPause->Update();
	}

	if (m_pTimer != nullptr)
	{//ÔØê
		if (m_pTimer->GetValue() <= 0)
		{
			if (m_pBoss != nullptr)
			{
				m_pBoss->MoveCtrl();
			}
			m_pTimer->Uninit();
			m_pTimer = nullptr;
		}
	}

	if (CManager::GetResult() != CManager::RT_NONE && m_bResult == false)
	{//Êªmèµ½ç
		if (m_pScore != nullptr)
		{//Êð}l[WÉÛ¶
			SetNowScore(m_pScore->GetValue());
		}

		//Ug\¦
		if (m_pResult != nullptr)
		{
			m_pResult->Uninit();
			m_pResult = nullptr;
		}

		m_pResult = CResult::Create();

		if (m_pSpawn != nullptr)
		{
			m_pSpawn->Uninit();
			m_pSpawn = nullptr;
		}

		m_bResult = true;
	}

	int nNumCsr = 0;

	if (bPause == true)
	{
		nNumCsr = ShowCursor(true);
	}
	else
	{
		nNumCsr = ShowCursor(false);
	}
}

//============================
// `æ
//============================
void CGame::Draw()
{
	CScene::Draw();
}

//============================
// ¶¬
//============================
CGame * CGame::Create()
{
	CGame *pGame = nullptr;

	//IuWFNg¶¬&ú»
	pGame = new CGame;

	if (pGame != nullptr)
	{
		if (FAILED(pGame->Init()))
		{
			return nullptr;
		}
		else
		{
			//pGame->m_mode = CScene::MODE_GAME;
		}
	}
	else
	{
		return nullptr;
	}

	return pGame;
}

//============================
// ¬·V[Ýè
//============================
HRESULT CGame::SetGrow(CGrowSelecter * pGrow)
{
	if (m_pGrow == nullptr)
	{
		m_pGrow = pGrow;
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}
