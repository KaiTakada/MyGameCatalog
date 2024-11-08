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
#include "objMeshField.h"
#include "input.h"
#include "pause.h"
#include "fade.h"
#include "timer.h"
#include "map.h"
#include "ball_ganarator.h"
#include "camera_player.h"
#include "wall.h"
#include "texture.h"
#include "gameManager.h"

#include "block.h"

//=========================
// マクロ定義
//=========================
namespace
{
	//プレイヤー
	const D3DXVECTOR3 PLAYER_POS[mylib_const::MAX_PLAYER] =
	{
		D3DXVECTOR3(2000.0f, 0.0f, -2000.0f),
		D3DXVECTOR3(-2000.0f, 0.0f, 2000.0f),
		D3DXVECTOR3(2000.0f, 0.0f, 2000.0f),
		D3DXVECTOR3(-2000.0f, 0.0f, -2000.0f),
	};

	//壁
	const D3DXVECTOR3 WALL_POS[mygame::NUM_WALL] =
	{
		D3DXVECTOR3(0.0f, 0.0f, -mylib_const::DEF_FIELD_SIZE.z),
		D3DXVECTOR3(mylib_const::DEF_FIELD_SIZE.x, 0.0f, 0.0f),
		D3DXVECTOR3(-mylib_const::DEF_FIELD_SIZE.x, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, mylib_const::DEF_FIELD_SIZE.z),
};

	//壁
	const D3DXVECTOR3 WALL_ROT[mygame::NUM_WALL] =
	{
		D3DXVECTOR3(0.0f, D3DX_PI,  0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI,  0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI,  0.0f),
		D3DXVECTOR3(0.0f, D3DX_PI,  0.0f),
	};
}

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

	m_pGameManager = nullptr;
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

	int nNumPlayer = GetNumPlayer();
	nNumPlayer = mygame::NUMPLAYER;		//とりま2人
	SetNumPlayer(nNumPlayer);

	for (int i = 0; i < nNumPlayer; i++)
	{
		m_ppCamera[i] = new CCameraPlayer;

		if (m_ppCamera[i] != nullptr)
		{
			D3DXVECTOR3 rot = mylib_const::DEFVEC3;
			rot.y = mylib_useful::Point2Angle(mylib_const::DEFVEC3, PLAYER_POS[i]);

			m_ppCamera[i]->Init();
			m_ppCamera[i]->SetIdx(i);
			m_ppCamera[i]->SetViewPos();
			m_ppCamera[i]->SetRot(rot);
		}
	}

	//オブジェクトの生成
	for (int i = 0; i < nNumPlayer; i++)
	{//CSceneのプレイヤーを作る
		D3DXVECTOR3 rot = mylib_const::DEFVEC3;
		rot.y = mylib_useful::Point2Angle(mylib_const::DEFVEC3, PLAYER_POS[i]);
		CPlayer *pPlayer = CPlayer::Create(PLAYER_POS[i], rot);
		
		if (pPlayer != nullptr)
		{
			//pPlayer->SetRot(rot);
			pPlayer->SetIdx(i);
			pPlayer->SetMember(static_cast<my_Identity::eMember>(i + 1));
			pPlayer->InitUI();
			pPlayer->InitModel();
			CScene::SetPlayer(pPlayer, i);
		}
	}

	//床
	m_pField = CObjMeshField::Create(10, 10);
	m_pField->SetSize2D(mylib_const::DEF_FIELD_SIZE2);
	CTexture* pTex = CManager::GetInstance()->GetTexture();
	m_pField->SetIdxTexture(pTex->Regist("data\\TEXTURE\\BG\\field_02.jpg"));

#if 1
	float fRotBG = D3DX_PI;
	for (int i = 0; i < mygame::NUM_WALL; i++)
	{
		fRotBG -= i * 0.5f * D3DX_PI;
		CWall*pBG = CWall::Create(WALL_POS[i], D3DXVECTOR3(0.0f, fRotBG, 0.0f));
		pBG->SetDivTex(10, 10);
		pBG->SetIdxTexture(pTex->Regist("data\\TEXTURE\\BG\\field_02.jpg"));
	}
#endif // 0

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
	
#if 0
	m_pBot = CEnemy::Create(D3DXVECTOR3(mylib_const::DEF_FIELD_SIZE.x - 100.0f, 0.0f, mylib_const::DEF_FIELD_SIZE.z - 100.0f), mylib_const::DEFVEC3);
#endif // 0

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_GAME);

	//マップ生成
	//m_pMap = CMap::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	//m_pMap->Load("data\\SET\\MAP\\load.txt");
	//m_pMap->Uninit();
	//delete m_pMap;
	m_pMap = nullptr;

	CBallGenerator::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CBallGenerator::Create(D3DXVECTOR3(1500.0f, 0.0f, 0.0f));
	CBallGenerator::Create(D3DXVECTOR3(-1500.0f, 0.0f, 0.0f));
	CBallGenerator::Create(D3DXVECTOR3(0.0f, 0.0f, 1500.0f));
	CBallGenerator::Create(D3DXVECTOR3(0.0f, 0.0f, -1500.0f));

	m_pGameManager = CGameManager::Create();
	m_pGameManager->SetFlowStage(CGameManager::START);

	return S_OK;
}

//============================
// 終了
//============================
void CGame::Uninit()
{
	for (int i = 0; i < mygame::NUMPLAYER; i++)
	{//CSceneのプレイヤーを殺す
		CPlayer *pPlayer = CScene::GetElem(i);
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
			m_ppWall[i]->SetDeath(true);
			m_ppWall[i] = nullptr;
		}
	}

	if (m_pField != nullptr)
	{
		m_pField->SetDeath(true);
		m_pField = nullptr;
	}

	if (m_pPause != nullptr)
	{
		m_pPause->SetDeath(true);
		m_pPause = nullptr;
	}

	if (m_pTimer != nullptr)
	{
		m_pTimer->SetDeath(true);
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
			m_ppCamera[i] = nullptr;
		}
	}

	if (m_pGameManager != nullptr)
	{
		m_pGameManager->Uninit();
		m_pGameManager = nullptr;
	}

	CScene::Uninit();

	CManager::GetInstance()->GetSound()->Stop();
}

//============================
// 更新
//============================
void CGame::Update()
{
	CScene::Update();

	if (CManager::GetInstance()->GetScene()->GetMode() != CScene::MODE_GAME)
	{
		return;
	}
	else if (CManager::GetInstance()->GetScene() != this)
	{
		return;
	}

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
	if (pInputKeyboard->GetTrigger(DIK_P) || pInputPad->GetTriggerOR(CInputGamepad::BUTTON_START) == true)
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

	if (m_pGameManager != nullptr)
	{
		m_pGameManager->Update();
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
		
		if (m_pGameManager != nullptr)
		{
			m_pGameManager->SetFlowStage(CGameManager::GAMESET);
		}

		if (m_pGameManager->GetEnd())
		{
			pFade->SetState(CScene::MODE_RESULT);
		}

		if (m_pTimer != nullptr)
		{//タイマー保存

			m_pTimer->SetStop(true);
			SetNowTime(m_pTimer->GetValue());
		}

		return;
	}

	if (m_pTimer != nullptr)
	{//タイマー更新

	 //加算
		m_pTimer->CntValue(1);
	}

	int nNumPlayer = 0;
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{//CSceneのプレイヤーを数える
		CPlayer *pPlayer = CScene::GetElem(i);
		if (pPlayer != nullptr)
		{
			if (pPlayer->GetDelete())
			{
				pPlayer->SetDeath(true);
				pPlayer = nullptr;
				CScene::SetPlayer(nullptr, i);
			}

			nNumPlayer++;
		}
	}

	if (nNumPlayer == 1 && CManager::GetInstance()->GetScene()->GetNumPlayer() >= 2)
	{//複数人居たのにプレイヤー1人だけ
		CManager::GetInstance()->SetResult(CManager::RT_WIN);
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
