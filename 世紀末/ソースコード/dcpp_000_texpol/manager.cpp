//===============================================
//
// �}�l�[�W��(manager.cpp)
// Author ���c ����
//
//===============================================

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "fade.h"

#include "object.h"
#include "texture.h"
#include "Xmodel.h"
#include "object2D.h"
#include "camera.h"
#include "spawn.h"
#include "light.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "multibg.h"
#include "effect.h"
#include "particle.h"
#include "number.h"
#include "score.h"
#include "block.h"
#include "item.h"
#include "Field.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "ranking.h"

//#include "renderer.h"

//=========================
// �}�N����`
//=========================
#define DEBUG_SCENE CScene::MODE_TITLE//�f�o�b�O���̃X�^�[�g�V�[��

//=========================
// �O���錾
//=========================

//=========================
// �ÓI�����o�ϐ�
//=========================
//Manager
CRenderer *CManager::m_pRenderer = nullptr;					//�����_���[
CTexture *CManager::m_pTeture = nullptr;					//�e�N�X�`��
CXModel *CManager::m_pXModel = nullptr;						//X�t�@�C�����f��
CCamera *CManager::m_pCamera = nullptr;						//�J����
CLight *CManager::m_pLight = nullptr;						//���C�g
CSound *CManager::m_pSound = nullptr;						//�T�E���h
CSpawn *CManager::m_pSpawn = nullptr;						//�G�X�|�[��

CInputKeyboard *CManager::m_pCInputKeyboard = nullptr;		//�L�[�{�[�h����
CInputGamepad *CManager::m_pCInputGamepad = nullptr;		//����(Pad[X])
CInputMouse *CManager::m_pCInputMouse = nullptr;			//����(�}�E�X)

CDebugProc *CManager::m_pDebugProc = nullptr;				//�f�o�b�O
CScene *CManager::m_pScene = nullptr;						//�V�[��

bool CManager::m_bPause = false;		//�|�[�Y�t���O
bool CManager::m_bGrow = false;			//�i���V�[���t���O
CManager::RESULT_TYPE CManager::m_result = RT_NONE;			//���s���e

//Scene
CBg *CScene::m_pBg = nullptr;								//�w�i
CMultiBg *CScene::m_pMultiBg = nullptr;						//���d�w�i
CNumber *CScene::m_pNumber = nullptr;						//����
CScore *CScene::m_pScore = nullptr;							//�X�R�A
CFade *CScene::m_pFade = nullptr;							//�t�F�[�h

CObject3D *CScene::m_pObject3D = nullptr;					//3D�|���S��

CObjectBillboard *CScene::m_pBillBoard = nullptr;			//�r���{�[�h
CEffect *CScene::m_pEffect = nullptr;						//�G�t�F�N�g
CParticle *CScene::m_pParticle = nullptr;					//�p�[�e�B�N��
CBullet *CScene::m_pBullet = nullptr;						//�e
CExplosion *CScene::m_pExplosion = nullptr;				//����
CField *CScene::m_pField = nullptr;						//�n��
CItem *CScene::m_pItem = nullptr;							//�A�C�e��

CObjectX *CScene::m_pObjectX = nullptr;					//���f��
CEnemy *CScene::m_pEnemy = nullptr;						//�G
CBlock *CScene::m_pBlock = nullptr;						//�u���b�N

int CScene::m_nScore = 0;								//����v���C����

//============================
// �R���X�g���N�^
//============================
CManager::CManager()
{

}

//============================
// �f�X�g���N�^
//============================
CManager::~CManager()
{

}

//============================
// ����������
//============================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//============ �����_���[ ============
	//NULL�`�F�b�N
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		m_pRenderer = nullptr;
	}

	//�����_���[�̐���
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;
	}
	else
	{
		return -1;
	}

	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//���������������s�����ꍇ
		return -1;
	}

	//============ �L�[�{�[�h ============
	//NULL�`�F�b�N
	if (m_pCInputKeyboard != nullptr)
	{
		m_pCInputKeyboard->Uninit();
		m_pCInputKeyboard = nullptr;
	}

	//�L�[�{�[�h�̐���
	if (m_pCInputKeyboard == nullptr)
	{
		m_pCInputKeyboard = new CInputKeyboard;

		if (FAILED(m_pCInputKeyboard->Init(hInstance, hWnd)))
		{//���������������s�����ꍇ
			return -1;
		}
	}
	else
	{
		return -1;
	}
	
	//============ Pad[X] ============
	//NULL�`�F�b�N
	if (m_pCInputGamepad != nullptr)
	{
		m_pCInputGamepad->Uninit();
		m_pCInputGamepad = nullptr;
	}

	//Pad[X]�̐���
	if (m_pCInputGamepad == nullptr)
	{
		m_pCInputGamepad = new CInputGamepad;

		if (FAILED(m_pCInputGamepad->Init(hInstance, hWnd)))
		{//���������������s�����ꍇ
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ �}�E�X ============
	//NULL�`�F�b�N
	if (m_pCInputMouse != nullptr)
	{
		m_pCInputMouse->Uninit();
		m_pCInputMouse = nullptr;
	}

	//�}�E�X�̐���
	if (m_pCInputMouse == nullptr)
	{
		m_pCInputMouse = new CInputMouse;

		if (FAILED(m_pCInputMouse->Init(hInstance, hWnd)))
		{//���������������s�����ꍇ
			return -1;
		}
	}
	else
	{
		return -1;
	}

	////============ �e�N�X�`�� ============
	////NULL�`�F�b�N
	//if (m_pTeture != nullptr)
	//{
	//	m_pTeture = nullptr;
	//}

	////����
	//if (m_pTeture == nullptr)
	//{
	//	m_pTeture = new CTexture;

	//	if (FAILED(m_pTeture->Load()))
	//	{//���������������s�����ꍇ
	//		return -1;
	//	}
	//}
	//else
	//{
	//	return -1;
	//}

	////============ ���f�� ============
	////NULL�`�F�b�N
	//if (m_pXModel != nullptr)
	//{
	//	m_pXModel = nullptr;
	//}

	////����
	//if (m_pXModel == nullptr)
	//{
	//	m_pXModel = new CXModel;

	//	if (FAILED(m_pXModel->Load()))
	//	{//���������������s�����ꍇ
	//		return -1;
	//	}
	//}
	//else
	//{
	//	return -1;
	//}

	//============ �f�o�b�O ============
	//NULL�`�F�b�N
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		m_pDebugProc = nullptr;
	}

	if (m_pDebugProc == nullptr)
	{
		m_pDebugProc = new CDebugProc;

		if (m_pDebugProc != nullptr)
		{
			m_pDebugProc->Init();
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ �J���� ============
	//NULL�`�F�b�N
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		m_pCamera = nullptr;
	}

	if (m_pCamera == nullptr)
	{
		m_pCamera = new CCamera;
	}
	else
	{
		return -1;
	}

	if (FAILED(m_pCamera->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	//============ ���C�g ============
	//NULL�`�F�b�N
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	if (m_pLight == nullptr)
	{
		m_pLight = new CLight;
	}
	else
	{
		return -1;
	}

	if (FAILED(m_pLight->Init()))
	{//���������������s�����ꍇ
		return -1;
	}

	//============ �T�E���h ============
	//NULL�`�F�b�N
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		m_pSound = nullptr;
	}

	if (m_pSound == nullptr)
	{
		m_pSound = new CSound;
	}
	else
	{
		return -1;
	}

	if (FAILED(m_pSound->Init(hWnd)))
	{//���������������s�����ꍇ
		return -1;
	}
	else
	{
#ifdef _RELEASE
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
#else
#endif
	}

	//============ �e�N�X�`�� ============
	//NULL�`�F�b�N
	if (m_pTeture != nullptr)
	{
		m_pTeture = nullptr;
	}

	//����
	if (m_pTeture == nullptr)
	{
		m_pTeture = new CTexture;

		if (FAILED(m_pTeture->Load()))
		{//���������������s�����ꍇ
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ ���f�� ============
	//NULL�`�F�b�N
	if (m_pXModel != nullptr)
	{
		m_pXModel = nullptr;
	}

	//����
	if (m_pXModel == nullptr)
	{
		m_pXModel = new CXModel;

		if (FAILED(m_pXModel->Load()))
		{//���������������s�����ꍇ
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ �I�u�W�F�N�g ============
	//�I�u�W�F�N�g�̏���������
	CObject::ReleaseAll();

	//============ �V�[�� ============
	//NULL�`�F�b�N
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	if (m_pScene == nullptr)
	{

#if _DEBUG
		m_pScene = CScene::Create(DEBUG_SCENE);
#else
		m_pScene = CScene::Create(CScene::MODE_TITLE);
#endif

	}
	else
	{
		return -1;
	}

	return S_OK;
}

//============================
// �I������
//============================
void CManager::Uninit(void)
{
	////�I�u�W�F�N�g�j��
	//if (m_pField != nullptr)
	//{//�n�ʔj��
	//	m_pField->Uninit();
	//	m_pField = nullptr;
	//}

	////�I�u�W�F�N�g�j��
	//if (m_pPlayer != nullptr)
	//{//�v���C���[�j��
	//	m_pPlayer->Uninit();
	//	m_pPlayer = nullptr;
	//}

	if (m_pScene != nullptr)
	{//�V�[���j��
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//�S�ẴI�u�W�F�N�g�j��
	CObject::ReleaseAll();

	if (m_pTeture != nullptr)
	{//�e�N�X�`���j��
		m_pTeture->Unload();
		delete m_pTeture;
		m_pTeture = nullptr;
	}

	if (m_pXModel != nullptr)
	{//X���f���j��
		m_pXModel->Unload();
		delete m_pXModel;
		m_pXModel = nullptr;
	}

	if (m_pSpawn != nullptr)
	{//�X�|�[���j��
		m_pSpawn->Uninit();
		delete m_pSpawn;
		m_pSpawn = nullptr;
	}

	if (m_pCamera != nullptr)
	{//�J�����j��
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pLight != nullptr)
	{//���C�g�j��
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pSound != nullptr)
	{//�T�E���h�j��
		m_pSound->Stop();
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
	
	if (m_pDebugProc != nullptr)
	{//�f�o�b�O�j��
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	if (m_pTeture != nullptr)
	{//�e�N�X�`���j��
		m_pTeture->Unload();
		delete m_pTeture;
		m_pTeture = nullptr;
	}
	
	if (m_pXModel != nullptr)
	{//X���f���j��
		m_pXModel->Unload();
		delete m_pXModel;
		m_pXModel = nullptr;
	}

	if (m_pCInputMouse != nullptr)
	{//�}�E�X�j��
		m_pCInputMouse->Uninit();
		delete m_pCInputMouse;
		m_pCInputMouse = nullptr;
	}

	if (m_pCInputGamepad != nullptr)
	{//Pad[X]�j��
		m_pCInputGamepad->Uninit();
		delete m_pCInputGamepad;
		m_pCInputGamepad = nullptr;
	}

	if (m_pCInputKeyboard != nullptr)
	{//�L�[�{�[�h�j��
		m_pCInputKeyboard->Uninit();
		delete m_pCInputKeyboard;
		m_pCInputKeyboard = nullptr;
	}

	if (m_pRenderer != nullptr)
	{//�����_���[�j��
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

//============================
// �X�V����
//============================
void CManager::Update(void)
{
	//�X�V����
	if (m_pCInputGamepad != nullptr)
	{
		m_pCInputGamepad->Update();
	}
	if (m_pCInputKeyboard != nullptr)
	{
		m_pCInputKeyboard->Update();
	}
	if (m_pCInputMouse != nullptr)
	{
		m_pCInputMouse->Update();
	}
	if (m_pRenderer != nullptr && m_bPause == false && m_bGrow == false)
	{
		m_pRenderer->Update();
	}
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}
}

//============================
// �`�揈��
//============================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}

	m_pScene->Draw();
}

//======================================================
//============================
// �R���X�g���N�^
//============================
CScene::CScene()
{
	m_mode = MODE_TITLE;
}

//============================
// �f�X�g���N�^
//============================
CScene::~CScene()
{

}

//============================
// ������
//============================
HRESULT CScene::Init()
{
	//�S�ẴI�u�W�F�N�g�j��
	CObject::ReleaseAll();

	m_mode = MODE_TITLE;

	return S_OK;
}

//============================
// �I��
//============================
void CScene::Uninit()
{
	if (m_pFade != nullptr)
	{//�t�F�[�h�j��
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	//�S�ẴI�u�W�F�N�g�j��
	CObject::ReleaseAll();
}

//============================
// �X�V
//============================
void CScene::Update()
{
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (m_pFade != nullptr)
	{//�t�F�[�h
		m_pFade->Update();
	}

	////�����t�F�[�h(�������f�o�b�O�p)
	//int nMode = GetMode() + 1;
	//nMode %= CScene::MODE_MAX;
	//m_pFade->SetState(CScene::MODE(nMode));

	if (pKeyboard->GetTrigger(DIK_ADD))
	{
		//�V�������[�h�̐���
		int nMode = GetMode() + 1;
		nMode %= CScene::MODE_MAX;

		m_pFade->SetState(CScene::MODE(nMode));
	}
}

//============================
// �`��
//============================
void CScene::Draw()
{
	m_pFade->Draw();
}

//============================
// ����
//============================
CScene * CScene::Create(MODE mode)
{
	CScene *pScene = nullptr;

	//�I�u�W�F�N�g����&������
	//pScene = new CScene;

	switch (mode)
	{
	case CScene::MODE_TITLE:
		pScene = new CTitle;
		break;

	case CScene::MODE_TUTORIAL:
		pScene = new CTutorial;
		break;

	case CScene::MODE_GAME:
		pScene = new CGame;
		break;

	case CScene::MODE_RANKING:
		pScene = new CRanking;
		break;

	default:
		pScene = new CScene;
		break;
	}

	if (pScene != nullptr)
	{
		if (FAILED(pScene->Init()))
		{
			return nullptr;
		}
		else
		{
			pScene->m_mode = mode;
		
			//============ �t�F�[�h ============
			if (m_pFade == nullptr)
			{
				m_pFade = CFade::Create(CScene::MODE_GAME);
			}
		}
	}
	else
	{
		return nullptr;
	}

	return pScene;
}

//============================
// �V�[���ݒ�
//============================
void CManager::SetMode(const CScene::MODE mode)
{
	if (m_pSound != nullptr)
	{
		m_pSound->Stop();		//�T�E���h�̒�~
	}

	//���݂̃��[�h�̔j��
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//�|�[�Y��Ԃ�OFF��
	SetPause(false);

	m_bPause = false;		//�|�[�Y�t���O
	m_bGrow = false;		//�i���V�[���t���O

	if (mode != CScene::MODE_RANKING)
	{
		m_result = RT_NONE;
	}

	//�V�������[�h�̐���
	m_pScene = CScene::Create(mode);
}
