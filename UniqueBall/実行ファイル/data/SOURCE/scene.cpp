//===============================================
//
// �V�[��(scene.cpp)
// Author ���c ����
//
//===============================================

//=========================
// �C���N���[�h�t�@�C��
//=========================
//#include "manager.h"
#include "scene.h"
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
	//CScene::MODE MODE_DEBUG = CScene::MODE_GAME;//�f�o�b�O���̃X�^�[�g�V�[��
	CScene::MODE MODE_DEBUG = CScene::MODE_TITLE;//�f�o�b�O���̃X�^�[�g�V�[��
}

//=========================
// �O���錾
//=========================

//=========================
// �ÓI�����o�ϐ�
//=========================
//Scene
CNumber *CScene::m_pNumber = nullptr;						//����
CScore *CScene::m_pScore = nullptr;							//�X�R�A
CFade *CScene::m_pFade = nullptr;							//�t�F�[�h

CObject3D *CScene::m_pObject3D = nullptr;					//3D�|���S��

CObjectBillboard *CScene::m_pBillBoard = nullptr;			//�r���{�[�h
CField *CScene::m_pField = nullptr;						//�n��
CItem *CScene::m_pItem = nullptr;						//�A�C�e��
CPlayer *CScene::m_pPlayer[mylib_const::MAX_PLAYER] = {nullptr};					//�v���C���[

CBlock *CScene::m_pBlock = nullptr;						//�u���b�N

int CScene::m_nTime = 0;								//����v���C����
int CScene::m_nNumPlayer = 0;						//�v���C���[�l��

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

	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	//�J�����̏�����
	pCamera->Init();

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
	CInputKeyboard *pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (m_pFade != nullptr)
	{//�t�F�[�h
		m_pFade->Update();
	}

	////�����t�F�[�h(�������f�o�b�O�p)
	//int nMode = static_cast<int>(GetMode());
	//nMode += 1;
	//nMode %= static_cast<int>(CScene::MODE_MAX);
	//
	//if (nMode >= static_cast<int>(CScene::MODE_TITLE) && nMode < static_cast<int>(CScene::MODE_MAX))
	//{
	//	m_pFade->SetState(CScene::MODE(nMode));
	//}
	//else
	//{
	//	static_assert(true, "���[�h���");
	//}

#if _DEBUG
	if (pKeyboard->GetTrigger(DIK_ADD))
	{
		//�V�������[�h�̐���
		int nMode = GetMode() + 1;
		nMode %= CScene::MODE_MAX;

		m_pFade->SetState(CScene::MODE(nMode));
	}

	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("#------------------------\n");
	pDebug->Print("#���[�h\n");
	pDebug->Print("#���݂̃��[�h:%d\n", static_cast<int>(m_mode));
	pDebug->Print("#------------------------\n");
#endif
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

	case CScene::MODE_RESULT:
		pScene = new CResult;
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
				m_pFade = CFade::Create(mode);
			}
		}
	}
	else
	{
		return nullptr;
	}

	return pScene;
}
