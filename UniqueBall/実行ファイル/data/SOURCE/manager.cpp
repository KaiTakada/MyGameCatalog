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
#include "obj_polygon2D.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
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
#include "obj_polygon3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"

//#include "renderer.h"

//=========================
// �萔��`
//=========================
namespace
{
	CScene::MODE MODE_DEBUG = CScene::MODE_GAME;//�f�o�b�O���̃X�^�[�g�V�[��
	//CScene::MODE MODE_DEBUG = CScene::MODE_TITLE;//�f�o�b�O���̃X�^�[�g�V�[��
}

//=========================
// �O���錾
//=========================

//=========================
// �ÓI�����o�ϐ�
//=========================
//Manager
CManager *CManager::m_pManager = nullptr;								//�w�i

//============================
// �R���X�g���N�^
//============================
CManager::CManager()
{
	m_pRenderer = nullptr;					//�����_���[
	m_pTeture = nullptr;					//�e�N�X�`��
	m_pXModel = nullptr;					//X�t�@�C�����f��
	m_pCamera = nullptr;					//�J����
	m_pLight = nullptr;						//���C�g
	m_pSound = nullptr;						//�T�E���h

	m_pCInputKeyboard = nullptr;		//����(�L�[�{�[�h)
	m_pCInputGamepad = nullptr;			//����(Pad[X])
	m_pCInputMouse = nullptr;			//����(�}�E�X)

	m_pDebugProc = nullptr;				//�f�o�b�O
	m_pScene = nullptr;					//�V�[��

	m_bPause = false;			//�|�[�Y�t���O
	m_bGrow = false;			//�i���V�[���t���O
	m_bEdit = false;			//�G�f�B�b�g�V�[���t���O
	m_result = RT_NONE;			//���s���e

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		m_anRank[i] = -1;
	}

	m_pManager = this;
}

//============================
// �f�X�g���N�^
//============================
CManager::~CManager()
{
	m_pManager = nullptr;
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
		m_pScene = CScene::Create(MODE_DEBUG);
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
	if (m_pRenderer != nullptr && m_bPause == false && m_bGrow == false && m_bEdit == false)
	{//�S�I�u�W�F�N�g
		m_pRenderer->Update();
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}
	if (m_pLight != nullptr)
	{
		m_pLight->Update();
	}

	if (m_pDebugProc != nullptr)
	{//�f�o�b�O�j��
		m_pDebugProc->Update();
	}
}

//============================
// �`�揈��
//============================
void CManager::Draw(void)
{
	m_pScene->Draw();
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
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

	//�V�������[�h�̐���
	m_pScene = CScene::Create(mode);
}

//======================================
// �C���X�^���X�擾
//======================================
CManager * CManager::GetInstance()
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManager;
	}

	return m_pManager;
}

//======================================
// �����[�X
//======================================
void CManager::Release()
{
	if (m_pManager != nullptr)
	{
		m_pManager->Uninit();
		delete m_pManager;
		m_pManager = nullptr;
	}
}

void CManager::ZeroRank()
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		m_anRank[i] = -1;
	}
}
