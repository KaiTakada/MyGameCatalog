//===============================================
//
// ゲーム(game.cpp)
// Author: 髙田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "field.h"
#include "input.h"
#include "pause.h"
#include "fade.h"
#include "timer.h"
#include "map.h"
#include "bg3D.h"
#include "ball_ganarator.h"
#include "camera_game.h"
#include "wall.h"

#include "block.h"
#include "blk_goal.h"

//=========================
// マクロ定義
//=========================

#if _DEBUG
#define START_TIME (10)		//スタート時秒数
#else
#define START_TIME (120)		//スタート時秒数
#endif

//=========================
// 静的メンバ変数
//=========================
CPause *CGame::m_pPause = nullptr;						//ポーズ
CTimer *CGame::m_pTimer = nullptr;						//タイマー
CMap *CGame::m_pMap = nullptr;						//マップ

//============================
// コンストラクタ
//============================
CGame::CGame()
{
	m_pField = nullptr;
	m_pPause = nullptr;
	m_pTimer = nullptr;
	m_pMap = nullptr;
	m_pBot = nullptr;

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++) 
	{
		m_ppCamera[i] = nullptr;
	}

	for (int i = 0; i < mygame::NUM_WALL; i++)
	{
		m_ppWall[i] = nullptr;
	}
}

//============================
// デストラクタ
//============================
CGame::~CGame()
{

}

//============================
// 初期化
//============================
HRESULT CGame::Init()
{
	CScene::Init();

	//オブジェクトの生成
	for (int i = 0; i < mygame::NUMPLAYER; i++)
	{//CSceneのプレイヤーを作る
	
		CPlayer *pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		
		if (pPlayer != nullptr)
		{
			pPlayer->SetIdx(i);
			pPlayer->SetMember(static_cast<my_Identity::eMember>(i + 1));
			CScene::SetPlayer(pPlayer, i);
		}
	}

	for (int i = 0; i < mygame::NUMPLAYER; i++)
	{
		m_ppCamera[i] = new CCameraGame;

		if (m_ppCamera[i] != nullptr)
		{
			m_ppCamera[i]->Init();
			m_ppCamera[i]->SetIdx(i);
			m_ppCamera[i]->SetViewPos();
		}
	}

	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	//float fRot = 0.0f;
	//for (int i = 0; i < mygame::NUM_WALL; i++)
	//{
	//	m_ppWall[i] = CWall::Create(mylib_const::DEFVEC3);
	//	D3DXVECTOR3 rot = m_ppWall[0]->GetRot();
	//	m_ppWall[0]->SetRot(D3DXVECTOR3(rot.x, fRot, rot.z));
	//	//m_ppWall[i]->SetSize(D3DXVECTOR3(mylib_const::DEF_FIELD_SIZE.x, mylib_const::DEF_FIELD_SIZE.z * 0.5f, 0.0f));
	//	fRot += 0.25f * D3DX_PI;
	//}

	//m_ppWall[0]->SetPos(D3DXVECTOR3(mylib_const::DEF_FIELD_SIZE.x, 0.0f, 0.0f));
	//m_ppWall[1]->SetPos(D3DXVECTOR3(-mylib_const::DEF_FIELD_SIZE.x, 0.0f, 0.0f));
	//m_ppWall[2]->SetPos(D3DXVECTOR3(0.0f, 0.0f, mylib_const::DEF_FIELD_SIZE.z));
	//m_ppWall[3]->SetPos(D3DXVECTOR3(0.0f, 0.0f, -mylib_const::DEF_FIELD_SIZE.z));
	D3DXVECTOR3 aPos[mygame::NUM_WALL] =
	{
		D3DXVECTOR3(0.0f, 0.0f, -mylib_const::DEF_FIELD_SIZE.z),
		D3DXVECTOR3(mylib_const::DEF_FIELD_SIZE.x, 0.0f, 0.0f),
		D3DXVECTOR3(-mylib_const::DEF_FIELD_SIZE.x, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, mylib_const::DEF_FIELD_SIZE.z),
	};


	float fRotBG = D3DX_PI;
	for (int i = 0; i < mygame::NUM_WALL; i++)
	{
		fRotBG -= i * 0.5f * D3DX_PI;
		CBg3D::Create(aPos[i], D3DXVECTOR3(0.0f, fRotBG, 0.0f));
	}

	//if (m_pTimer != nullptr)
	//{
	//	m_pTimer->Uninit();
	//	m_pTimer = nullptr;
	//}

	//m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.45f, SCREEN_HEIGHT * 0.1f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 50.0f, 0.0f));
	//m_pTimer->SetValue(START_TIME);

	if (m_pBot != nullptr)
	{
		m_pBot->Uninit();
		m_pBot = nullptr;
	}

	m_pBot = CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, 0.0f), mylib_const::DEFVEC3);

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

	//マップ生成
	m_pMap = CMap::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	m_pMap->Load("data\\SET\\MAP\\load.txt");
	m_pMap->Uninit();
	delete m_pMap;
	m_pMap = nullptr;

	CBallGenerator::Create(D3DXVECTOR3(-1500.0f, 0.0f, 0.0f));
	CBallGenerator::Create(D3DXVECTOR3(-500.0f, 0.0f, 0.0f));
	CBallGenerator::Create(D3DXVECTOR3(1000.0f, 0.0f, 0.0f));

	return S_OK;
}

//============================
// 終了
//============================
void CGame::Uninit()
{
	for (int i = 0; i < mygame::NUMPLAYER; i++)
	{//CSceneのプレイヤーを殺す
		CPlayer *pPlayer = CScene::GetPlayer(i);
		if (pPlayer != nullptr)
		{
			pPlayer->SetDeath(true);
			pPlayer = nullptr;
		}

		SetPlayer(pPlayer, i);
	}

	for (int i = 0; i < mygame::NUM_WALL; i++)
	{
		if (m_ppWall[i] != nullptr)
		{
			m_ppWall[i]->Uninit();
			m_ppWall[i] = nullptr;
		}
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

	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();
		delete m_pMap;
		m_pMap = nullptr;
	}
	
	//通常メンバ変数
	if (m_pBot != nullptr)
	{
		m_pBot->Uninit();
		m_pBot = nullptr;
	}

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		if (m_ppCamera[i] != nullptr)
		{
			m_ppCamera[i]->Uninit();
			delete m_ppCamera[i];
		}
	}

	CScene::Uninit();

	CManager::GetInstance()->GetSound()->Stop();
}

//============================
// 更新
//============================
void CGame::Update()
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInstance()->GetInputGamepad();
	bool bPause = CManager::GetInstance()->GetPause();
	bool bEdit = CManager::GetInstance()->GetEdit();

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		if (m_ppCamera[i] != nullptr)
		{
			m_ppCamera[i]->Update();
		}
	}

	CScene::Update();

#if _DEBUG
	//エディット
	if (pInputKeyboard->GetTrigger(DIK_M))
	{//[ M ]キーでエディット

		//if (m_pMap == nullptr)
		//{
		//	m_pMap = CMap::Create(m_pPlayer->GetPos());
		//}
		//else
		//{
		//	m_pMap->Uninit();
		//	delete m_pMap;
		//	m_pMap = nullptr;
		//}

		CManager::GetInstance()->InvEdit();
	}

	if (m_pMap != nullptr && bEdit == true)
	{
		m_pMap->Update();
		return;
	}

#endif

	//ポーズ
	if (pInputKeyboard->GetTrigger(DIK_P) || pInputPad->GetPress(CInputGamepad::BUTTON_START, 0) == true)
	{//[ P ]キーでポーズ

		if (m_pPause == nullptr)
		{
			m_pPause = CPause::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		}
		else
		{
			m_pPause->Uninit();
			m_pPause = nullptr;
		}

		CManager::GetInstance()->InvPause();

		if (m_pTimer != nullptr)
		{
			m_pTimer->CntStop();
		}
	}

	if (m_pPause != nullptr && bPause == true)
	{
		m_pPause->Update();
	}

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_RETURN))
	{
		CManager::GetInstance()->SetResult(CManager::RT_WIN);
	}
#endif

	CFade *pFade = CScene::GetFade();

	if (CManager::GetInstance()->GetResult() != CManager::RT_NONE)
	{//結果が確定したら
		
		pFade->SetState(CScene::MODE_RESULT);

		if (m_pTimer != nullptr)
		{//タイマー保存

			m_pTimer->SetStop(true);
			SetNowTime(m_pTimer->GetValue());
		}
	}

	if (m_pTimer != nullptr)
	{//タイマー更新

	 //加算
		m_pTimer->CntValue(1);
	}

	//死んでたらリスポーン
	for (int i = 0; i < mygame::NUMPLAYER; i++)
	{//CSceneのプレイヤーを殺す
		CPlayer *pPlayer = CScene::GetPlayer(i);
		if (pPlayer == nullptr)
		{
			continue;
		}
		if (pPlayer->GetDeath())
		{
			pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			
			if (pPlayer == nullptr)
			{
				continue;
			}
			
			pPlayer->SetIdx(i);
			pPlayer->SetMember(static_cast<my_Identity::eMember>(i + 1));
		}

		SetPlayer(pPlayer, i);
	}
}

//============================
// 描画
//============================
void CGame::Draw()
{
	if (m_pMap != nullptr)
	{
		m_pMap->Draw();
		return;
	}

	CScene::Draw();
}

//============================
// 生成
//============================
CGame * CGame::Create()
{
	CGame *pGame = nullptr;

	//オブジェクト生成&初期化
	pGame = new CGame;

	if (pGame != nullptr)
	{
		if (FAILED(pGame->Init()))
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

	return pGame;
}
