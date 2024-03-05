//===============================================
//
// �X�R�A(score.cpp)
// Author ���c ����
//
//===============================================

//============================
// �C���N���[�h�t�@�C��
//============================
#include "ui_life.h"
#include "object2D.h"
#include "texture.h"
#include "camera_game.h"
#include "game.h"

//============================
// �}�N����`
//============================
namespace
{
	const D3DXVECTOR3 DEF_SIZE = { 50.0f, 50.0f ,0.0f };	//�f�t�H���g�̃}�[�N�̃T�C�Y
	const char* TIME_COLON_PASS = "data\\TEXTURE\\UI\\life00.png";	//life.png
	const D3DXVECTOR3 DEF_POS_DOUBLE[mygame::NUMPLAYER] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.0f + DEF_SIZE.x, SCREEN_HEIGHT - DEF_SIZE.y ,0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2.0f + DEF_SIZE.x, SCREEN_HEIGHT - DEF_SIZE.y ,0.0f) };	//�f�t�H���g�̃v���C���[2�l�p�ʒu
}

//============================
// �ÓI�����o�ϐ��錾
//============================

//============================
// �R���X�g���N�^
//============================
CUI_Life::CUI_Life()
{
	for (int i = 0; i < myUI_life::MAX_NUM_LIFE; i++)
	{
		if (m_apMark[i] != nullptr)
		{
			m_apMark[i] = nullptr;
		}
	}

	m_pos = mylib_const::DEFVEC3;
	m_nNumMark = 0;
	m_nIdx = -1;
}

//============================
// �f�X�g���N�^
//============================
CUI_Life::~CUI_Life()
{

}

//====================================
// 2D�|���S���̏���������(�f�t�H)
//====================================
HRESULT CUI_Life::Init()
{
	D3DXVECTOR3 posDif = DEF_POS_DOUBLE[m_nIdx];

	for (int nCntAll = 0; nCntAll < m_nNumMark; nCntAll++)
	{
		if (m_apMark[nCntAll] == nullptr)
		{
			m_apMark[nCntAll] = CObject2D::Create(posDif, mylib_const::DEFVEC3, DEF_SIZE, CObject2D::UPDATE_TYPE_NONE);
		
			CTexture *pTex = CManager::GetInstance()->GetTexture();
			
			//�e�N�X�`���t�^
			m_apMark[nCntAll]->SetIdxTexture(pTex->Regist(TIME_COLON_PASS));

			posDif.x += (DEF_SIZE.x * 1.5f);		//����1���~1��(:)
		}
	}

	SetType(TYPE_TIMER);

	return S_OK;
}

//============================
// 2D�|���S���̏I������
//============================
void CUI_Life::Uninit(void)
{
	for (int nCntDel = 0; nCntDel < m_nNumMark; nCntDel++)
	{
		if (m_apMark[nCntDel] != nullptr)
		{
			m_apMark[nCntDel]->Uninit();
			m_apMark[nCntDel] = nullptr;
		}
	}

	Release();
}

//============================
// 2D�|���S���̍X�V����
//============================
void CUI_Life::Update(void)
{
	DWORD dwTimeNow = timeGetTime();
	DWORD dwTimeOld = timeGetTime();
}

//============================
// 2D�|���S���̕`�揈��
//============================
void CUI_Life::Draw(void)
{

}

//============================
// 2D�|���S���̐�������
//============================
CUI_Life * CUI_Life::Create(void)
{
	CUI_Life *pScore;

	//�I�u�W�F�N�g����
	pScore = new CUI_Life;

	//������
	pScore->Init();

	return pScore;
}

//============================
// 2D�|���S���̐�������
//============================
CUI_Life * CUI_Life::Create(const int nNumMark, const int nIdx)
{
	CUI_Life *pScore = nullptr;

	//�I�u�W�F�N�g����&������
	if (pScore == nullptr)
	{
		pScore = new CUI_Life;

		if (pScore != nullptr)
		{
			pScore->SetNumMark(nNumMark);
			pScore->SetIdx(nIdx);
			pScore->Init();
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	return pScore;
}

//============================
// �ʒu�ݒ�
//============================
void CUI_Life::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	D3DXVECTOR3 posDif = pos;

	if (m_nIdx != -1)
	{
		CScene *pScene = CManager::GetInstance()->GetScene();
		if (pScene != nullptr)
		{
			if (pScene->GetMode() == CScene::MODE_GAME)
			{
				CGame *pGame = nullptr;
				pGame = static_cast<CGame*>(pScene);

				CCameraGame *pCamera = pGame->GetCamera(m_nIdx);

				if (pCamera != nullptr)
				{
					D3DVIEWPORT9 viewport = pCamera->GetViewPort();
					posDif.x += viewport.X;
					posDif.y += viewport.Y;
				}
			}
		}
	}

	for (int i = 0; i < myUI_life::MAX_NUM_LIFE; i++)
	{
		if (m_apMark[i] != nullptr)
		{
			m_apMark[i]->SetPos(posDif);
			posDif += m_apMark[i]->GetSize();
		}
	}
}

//============================
// �}�[�N����
//============================
void CUI_Life::AddLife(const int nIdx)
{
	D3DXVECTOR3 posDif = mylib_const::DEFVEC3;

	if (m_nNumMark <= 0)
	{
		posDif = DEF_POS_DOUBLE[m_nIdx];
	}
	else
	{
		posDif = m_apMark[m_nIdx - 1]->GetPos();
	}

	for (int i = m_nNumMark; i < m_nNumMark + nIdx; i++)
	{
		if (m_apMark[i] == nullptr)
		{
			m_apMark[i] = CObject2D::Create(posDif, mylib_const::DEFVEC3, DEF_SIZE, CObject2D::UPDATE_TYPE_NONE);

			CTexture *pTex = CManager::GetInstance()->GetTexture();

			//�e�N�X�`���t�^
			m_apMark[i]->SetIdxTexture(pTex->Regist(TIME_COLON_PASS));

			posDif.x += (DEF_SIZE.x * 1.5f);		//����1���~1��(:)
		}
	}

	m_nNumMark += nIdx;
}

//============================
// �}�[�N����
//============================
void CUI_Life::SubLife(const int nIdx)
{
	if (nIdx <= 0)
	{
		return;
	}

	int nNum = nIdx;
	if (m_nNumMark < nNum)
	{
		nNum = m_nNumMark;
	}

	for (int i = m_nNumMark - 1; i > m_nNumMark - nNum - 1; i--)
	{
		if (m_apMark[i] != nullptr)
		{
			m_apMark[i]->SetDeath(true);
		}
	}

	m_nNumMark -= nNum;
}
