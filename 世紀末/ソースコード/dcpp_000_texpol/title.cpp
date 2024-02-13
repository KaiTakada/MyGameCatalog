//===============================================
//
// �^�C�g��(title.cpp)
// Author: ���c ����
//
//===============================================

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "field.h"
#include "object2D.h"
#include "texture.h"

//=========================
// �}�N����`
//=========================

//=========================
// �ÓI�����o�ϐ�
//=========================

//============================
// �R���X�g���N�^
//============================
CTitle::CTitle()
{
	m_pField = nullptr;
	m_pEnter = nullptr;

	m_nCntRanking = 0;
	m_nAlphaCnter = 0;
	m_nTimeInterval = 0;
	m_fAlphaColor = 0.0f;
}

//============================
// �f�X�g���N�^
//============================
CTitle::~CTitle()
{

}

//============================
// ������
//============================
HRESULT CTitle::Init()
{
	m_nTimeInterval = 25;						//�_�ŊԊu������������
	m_fAlphaColor = 1.0f;
	m_nAlphaCnter = 0;

	CScene::Init();

	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	float fWidth = SCREEN_WIDTH;
	float fHeight = SCREEN_HEIGHT;

	CTexture *pTexture = CManager::GetTexture();

	CObject2D *pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\title.png"));

	m_pEnter = CObject2D::Create(D3DXVECTOR3(fWidth * 0.75f, fHeight * 0.8f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.2f, fHeight * 0.1f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	m_pEnter->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\PressEnter00.png"));

	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//============================
// �I��
//============================
void CTitle::Uninit()
{
	if (m_pField != nullptr)
	{
		m_pField->Uninit();
		m_pField = nullptr;
	}

	if (m_pEnter != nullptr)
	{
		m_pEnter->Uninit();
		m_pEnter = nullptr;
	}

	CScene::Uninit();

	CManager::GetSound()->Stop();
}

//============================
// �X�V
//============================
void CTitle::Update()
{
	//m_pField->Update();

	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInputGamepad();
	CFade *pFade = CScene::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputKeyboard->GetTrigger(DIK_SPACE) || 
		pInputPad->GetPress(CInputGamepad::BUTTON_A, 0) == true)
	{//[ Enter ]�L�[�Ń|�[�Y
		if (pFade->SetState(CScene::MODE_TUTORIAL))
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BUTTON);
		}
		m_nTimeInterval = 4;							//�_�ŊԊu������������
	}
	else if (m_nCntRanking >= 600)
	{
		pFade->SetState(CScene::MODE_RANKING);
	}

	m_nCntRanking++;
	m_nAlphaCnter++;
	
	if (m_nAlphaCnter % m_nTimeInterval == 0)
	{//0.5�b��������

		if (m_pEnter != nullptr)
		{
			D3DXCOLOR col = m_pEnter->GetTexCol();

			col.a = col.a ? 0.0f : 1.0f;

			//col.a = m_fAlphaColor;
			m_pEnter->SetTexCol(col);
		}
	}

	CScene::Update();
}

//============================
// �`��
//============================
void CTitle::Draw()
{
	//if (m_pField != nullptr)
	//{
	//	m_pField->Draw();
	//}

	CScene::Draw();
}

//============================
// ����
//============================
CTitle * CTitle::Create()
{
	CTitle *pTitle = nullptr;

	//�I�u�W�F�N�g����&������
	pTitle = new CTitle;

	if (pTitle != nullptr)
	{
		if (FAILED(pTitle->Init()))
		{
			return nullptr;
		}
		else
		{
			//pTitle->m_mode = mode;
		}
	}
	else
	{
		return nullptr;
	}


	return pTitle;
}
