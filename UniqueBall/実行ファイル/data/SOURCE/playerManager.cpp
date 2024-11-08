//===============================================
//
// �v���C���[�}�l�[�W��(playerManager.h)
// Author: Kai Takada
//
//===============================================

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "playerManager.h"
#include "player.h"

#include "debugproc.h"

//=========================
// �ÓI�����o�ϐ�
//=========================
CPlayerManager* CPlayerManager::m_pManager = nullptr;								//�w�i

//============================
// �R���X�g���N�^
//============================
CPlayerManager::CPlayerManager()
{
	m_pManager = this;
}

//============================
// �f�X�g���N�^
//============================
CPlayerManager::~CPlayerManager()
{
	m_pManager = nullptr;
}

//============================
// ����������
//============================
HRESULT CPlayerManager::Init()
{
	return S_OK;
}

//============================
// �I������
//============================
void CPlayerManager::Uninit()
{
	m_pPlayer.clear();

	delete m_pManager;
}

//============================
// �X�V����
//============================
void CPlayerManager::Update()
{
	int i = 0;

	// �e�v�f�ɑ΂��đ�����s��
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
	pDebug->Print("�L�[�}�E�ő��삷��v���C���[:%d\n", CPlayer::GetControlPlayer());
}

//======================================
// �C���X�^���X�擾
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
// �����[�X
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
// �v�f�ǉ�
//======================================
void CPlayerManager::Add(CPlayer* pPlayer)
{
	m_pPlayer.push_back(pPlayer);              // �����ɒǉ�
}


//======================================
// �v�f�擾
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
