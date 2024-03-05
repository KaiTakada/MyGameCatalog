//===============================================
//
// ���U���g(result.cpp)
// Author: ���c ����
//
//===============================================

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "object2D.h"
#include "texture.h"
#include "timer.h"
#include "ui_result.h"

//=========================
// �}�N����`
//=========================
namespace
{
	const char* RANK_PASS = "data\\TEXTURE\\SCENE\\ranking.png";
}

//=========================
// �ÓI�����o�ϐ�
//=========================

//============================
// �R���X�g���N�^
//============================
CResult::CResult()
{
	m_pResultUI = nullptr;		//�v���C���\�̃t���[��
}

//============================
// �f�X�g���N�^
//============================
CResult::~CResult()
{

}

//============================
// ������
//============================
HRESULT CResult::Init()
{
	float fWidth = SCREEN_WIDTH;
	float fHeight = SCREEN_HEIGHT;

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	CScene::Init();

	//�w�i----------------------------
	CObject2D *pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\BG\\field.jpg"));

	pPolygon->SetTexCol(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.7f));
	//--------------------------------

	//�v���C���\ ---------------------
	//--------------------------------

	//�t���[��------------------------
	m_pResultUI = CUI_Result::Create();
	//--------------------------------
	
	//���� ---------------------
	int nNumPlayer = CManager::GetInstance()->GetScene()->GetNumPlayer();
	for (int i = 0; i < nNumPlayer; i++)
	{
		if (m_pResultUI != nullptr)
		{
			int nPlayer = CManager::GetInstance()->GetRank(i);	//���̏���(i)�̃v���C���[�ԍ�
			if (nPlayer == -1)
			{
				continue;
			}

			CObject2D *p2D = m_pResultUI->GetMark(nPlayer);
			float fPos = (nNumPlayer - 1) * 0.2f - i * 0.2f;
			D3DXVECTOR3 pos = D3DXVECTOR3(p2D->GetPos().x - 180.0f, p2D->GetPos().y - 200.0f, 0.0f);

			if (p2D != nullptr)
			{
				CObject2D *pRank = CObject2D::Create(pos, mylib_const::DEFVEC3, D3DXVECTOR3(50.0f, 50.0f, 50.0f), CObject2D::UPDATE_TYPE_NONE);
				pRank->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\ranking.png"));
				pRank->SetTexPos(D3DXVECTOR2(fPos + 0.2f, 1.0f), D3DXVECTOR2(fPos, 0.0f));
			}
		}
	}
	//--------------------------------

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//============================
// �I��
//============================
void CResult::Uninit()
{
	CManager::GetInstance()->SetResult(CManager::RT_NONE);

	if (m_pResultUI != nullptr)
	{
		m_pResultUI->SetDeath(true);
		m_pResultUI = nullptr;
	}

	CScene::Uninit();
}

//============================
// �X�V
//============================
void CResult::Update()
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInstance()->GetInputGamepad();
	CFade *pFade = CScene::GetFade();

	if (m_pResultUI->GetTrans())
	{//[ Enter ]�L�[�Ń|�[�Y
		if (pFade->SetState(CScene::MODE_TUTORIAL))
		{
			//CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BUTTON);
			CManager::GetInstance()->ZeroRank();	//���ʏ���
		}
	}

	CScene::Update();
}

//============================
// �`��
//============================
void CResult::Draw()
{
	CScene::Draw();
}

//============================
// ����
//============================
CResult * CResult::Create()
{
	CResult *pResult = nullptr;

	//�I�u�W�F�N�g����&������
	pResult = new CResult;

	if (pResult != nullptr)
	{
		if (FAILED(pResult->Init()))
		{
			return nullptr;
		}
		else
		{
			//pResult->m_mode = mode;
		}
	}
	else
	{
		return nullptr;
	}


	return pResult;
}
