//===============================================
//
// �Q�[��(game.cpp)
// Author: ���c ����
//
//===============================================

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "gameManager.h"
#include "game.h"
#include "obj_polygon2D.h"
#include "number.h"
#include "Texture.h"
#include "fade.h"
#include "sound.h"

//=========================
// �}�N����`
//=========================
namespace
{
	const int COUNTDOWN_START(3);		//���R�R����J�E���g�_�E��
	const int STAYTIME_START(60);		//�Ƃǂ܂鎞��
	const int ROTATETIME_START(10);		//��]�ɗv���鎞��
	const char* GO_PASS("data\\TEXTURE\\SCENE//GAME//Go.png");
	const char* GAMESET_PASS("data\\TEXTURE\\SCENE//GAME//GameSet.png");
	const D3DXVECTOR3 GO_SIZE(D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, 0.0f));
}

//=========================
// �ÓI�����o�ϐ�
//=========================


//==========================================================================
// �֐��|�C���^
//==========================================================================
CGameManager::UPD_FUNC CGameManager::m_UpdateFuncList[] =
{
	nullptr,
	&CGameManager::UpdateStart,		// �X�^�[�g
	nullptr,
	&CGameManager::UpdateGameset,	// �Q�[���Z�b�g
};

CGameManager::CRT_FUNC CGameManager::m_CreateFuncList[] =
{
	nullptr,
	&CGameManager::CreateStart,		// �X�^�[�g
	nullptr,
	&CGameManager::CreateGameset,	// �Q�[���Z�b�g
};

//============================
// �R���X�g���N�^
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
// �f�X�g���N�^
//============================
CGameManager::~CGameManager()
{

}

//============================
// ������
//============================
HRESULT CGameManager::Init()
{
	m_bEnd = true;

	return S_OK;
}

//============================
// �I��
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
// �X�V
//============================
void CGameManager::Update()
{
	if (m_UpdateFuncList[m_FlowStage] != nullptr)
	{
		(this->*(m_UpdateFuncList[m_FlowStage]))();
	}
}

//============================
// �`��
//============================
void CGameManager::Draw()
{

}

//============================
// ����
//============================
CGameManager * CGameManager::Create()
{
	CGameManager *pManager = nullptr;

	//�I�u�W�F�N�g����&������
	pManager = new CGameManager;

	if (pManager != nullptr)
	{
		if (FAILED(pManager->Init()))
		{//���s������
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
// �󋵐ݒ�
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
// �X�^�[�g������
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
// �Q�[���Z�b�g������
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
// �X�^�[�g���X�V
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
	{//�t�F�[�h���͊J�n���Ȃ�
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
		{//�\���J�E���g��0�ɂȂ����u��

			if (m_pCountdown == nullptr)
			{//��̏����̓J�E���g�_�E�����Ă����ꍇ
			
				if (m_OnePoint != nullptr)
				{//Go�̃J�E���g���I�����
					m_OnePoint->SetDeath();
					m_OnePoint = nullptr;
					SetFlowStage(INGAME);
					m_bEnd = true;
				}

				return;
			}

			if (m_pCountdown->GetNumber() > 1)
			{//�J�E���g�_�E���̗]�n������ꍇ
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
// �Q�[���Z�b�g���X�V
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
