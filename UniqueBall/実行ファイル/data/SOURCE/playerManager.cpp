//===============================================
//
// プレイヤーマネージャ(playerManager.h)
// Author: Kai Takada
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "playerManager.h"
#include "player.h"

#include "debugproc.h"

//=========================
// 静的メンバ変数
//=========================
CPlayerManager* CPlayerManager::m_pManager = nullptr;								//背景

//============================
// コンストラクタ
//============================
CPlayerManager::CPlayerManager()
{
	m_pManager = this;
}

//============================
// デストラクタ
//============================
CPlayerManager::~CPlayerManager()
{
	m_pManager = nullptr;
}

//============================
// 初期化処理
//============================
HRESULT CPlayerManager::Init()
{
	return S_OK;
}

//============================
// 終了処理
//============================
void CPlayerManager::Uninit()
{
	m_pPlayer.clear();

	delete m_pManager;
}

//============================
// 更新処理
//============================
void CPlayerManager::Update()
{
	int i = 0;

	// 各要素に対して操作を行う
	for (auto itr = m_pPlayer.begin(); itr != m_pPlayer.end();)
	{
		CPlayer* player = *std::next(m_pPlayer.begin(), i);

		if (player->GetDeath())
		{
			itr = m_pPlayer.erase(std::next(m_pPlayer.begin(), i));
		}
		else
		{
			itr++;
			i++;
		}
	}

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("キーマウで操作するプレイヤー:%d\n", CPlayer::GetControlPlayer());
}

//======================================
// インスタンス取得
//======================================
CPlayerManager* CPlayerManager::GetInstance()
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CPlayerManager;
	}

	return m_pManager;
}

//======================================
// リリース
//======================================
void CPlayerManager::Release()
{
	if (m_pManager != nullptr)
	{
		m_pManager->Uninit();
		delete m_pManager;
		m_pManager = nullptr;
	}
}

//======================================
// 要素追加
//======================================
void CPlayerManager::Add(CPlayer* pPlayer)
{
	m_pPlayer.push_back(pPlayer);              // 末尾に追加
}


//======================================
// 要素取得
//======================================
CPlayer* CPlayerManager::GetElem(int nIdx)
{
	CPlayer* player = nullptr;

	if (nIdx < static_cast<int>(m_pPlayer.size()))
	{
		player = *std::next(m_pPlayer.begin(), nIdx);
	}

	return player;
}
