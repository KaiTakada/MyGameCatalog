//===============================================
//
// �����L���O(ranking.cpp)
// Author: ���c ����
//
//===============================================

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "object2D.h"
#include "texture.h"
#include "score.h"

//=========================
// �}�N����`
//=========================

#define RANK_WIN_READ_FILE "data\\SET\\RANKING\\rank_win.txt"
#define RANK_LOSE_READ_FILE "data\\SET\\RANKING\\rank_lose.txt"

//=========================
// �ÓI�����o�ϐ�
//=========================

//============================
// �R���X�g���N�^
//============================
CRanking::CRanking()
{
	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		m_apScore[nCnt] = nullptr;
	}

	m_nScore = 0;
	m_nCntTitle = 0;
}

//============================
// �f�X�g���N�^
//============================
CRanking::~CRanking()
{

}

//============================
// ������
//============================
HRESULT CRanking::Init()
{
	CTexture *pTexture = CManager::GetTexture();
	float fWidth = SCREEN_WIDTH;
	float fHeight = SCREEN_HEIGHT;

	CScene::Init();

	CManager::RESULT_TYPE type = CManager::GetResult();

	m_nScore = GetNowScore();

	//�w�i
	CObject2D *pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\null.jpg"));
	pPolygon->SetTexCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.2f));

	//���s�ǂ��炩
	pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.2f, fHeight * 0.1f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(200.0f, 40.0f, 0.0f), CObject2D::UPDATE_TYPE_NONE);
	
	if (type == CManager::RT_WIN)
	{
		pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\SOURCE\\RANKING\\result_win.png"));
	}
	else if (type == CManager::RT_LOSE)
	{
		pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\SOURCE\\RANKING\\result_lose.png"));
	}

	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		fHeight = SCREEN_HEIGHT * (nCnt + 1) * 0.15f;
		fHeight += 100.0f;

		//���ʃ|���S��
		pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.3f, fHeight, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(100.0f, 50.0f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

		pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\ranking.png"));
		pPolygon->SetTexPos(D3DXVECTOR2(nCnt * 0.2f + 0.2f, 1.0f), D3DXVECTOR2(nCnt * 0.2f, 0.0f));

		//�X�R�A
		if (m_apScore[nCnt] != nullptr)
		{
			m_apScore[nCnt]->Uninit();
			m_apScore[nCnt] = nullptr;
		}

		m_apScore[nCnt] = CScore::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight, 0.0f));

		if (m_apScore[nCnt] == nullptr)
		{
			return S_FALSE;
		}
	}

	//�Ǎ�
	if (type == CManager::RT_WIN)
	{
		Read(RANK_WIN_READ_FILE);
	}
	else if (type == CManager::RT_LOSE)
	{
		Read(RANK_LOSE_READ_FILE);
	}
	else
	{
		Read();
	}

	//�\�[�g
	Sort();

	//��������
	if (type == CManager::RT_WIN)
	{
		Write(RANK_WIN_READ_FILE);
	}
	else if (type == CManager::RT_LOSE)
	{
		Write(RANK_LOSE_READ_FILE);
	}
	else
	{
		Write();
	}

	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_RANKING);

	return S_OK;
}

//============================
// �I��
//============================
void CRanking::Uninit()
{
	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		if (m_apScore[nCnt] != nullptr)
		{
			m_apScore[nCnt]->Uninit();
			m_apScore[nCnt] = nullptr;
		}
	}

	CScene::Uninit();

	CManager::GetSound()->Stop();
}

//============================
// �X�V
//============================
void CRanking::Update()
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInputGamepad();
	CFade *pFade = CScene::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputPad->GetPress(CInputGamepad::BUTTON_A, 0) == true)
	{//[ Enter ]�L�[�Ń|�[�Y	
		if (pFade->SetState(CScene::MODE_TITLE))
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BUTTON);
		}
	}
	else if (m_nCntTitle >= 1200)
	{
		pFade->SetState(CScene::MODE_TITLE);
	}

	CScene::Update();
}

//============================
// �`��
//============================
void CRanking::Draw()
{
	CScene::Draw();
}

//============================
// ����
//============================
CRanking * CRanking::Create()
{
	CRanking *pResult = nullptr;

	//�I�u�W�F�N�g����&������
	pResult = new CRanking;

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

//============================
// �����L���O�\�[�g
//============================
void CRanking::Sort()
{
	int aScore[RK_NUM_SCORE] = {};

	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{//�f�[�^���
		if (m_apScore[nCnt] != nullptr)
		{
			aScore[nCnt] = m_apScore[nCnt]->GetValue();
		}
	}

	if (m_nScore > aScore[RK_NUM_SCORE - 1])
	{
		aScore[RK_NUM_SCORE - 1] = m_nScore;

		for (int nCntMax = 0; nCntMax < RK_NUM_SCORE; nCntMax++)
		{
			int nHidata = nCntMax;

			for (int nCntSort = nCntMax + 1; nCntSort < RK_NUM_SCORE; nCntSort++)
			{
				if (aScore[nHidata] < aScore[nCntSort])
				{
					nHidata = nCntSort;
				}
			}

			if (nHidata != nCntMax)
			{//�I�������ꏊ���ς���Ă�����
				int nTemp = aScore[nCntMax];
				aScore[nCntMax] = aScore[nHidata];
				aScore[nHidata] = nTemp;
			}
		}

		for (int nCntComp = 0; nCntComp < RK_NUM_SCORE; nCntComp++)
		{//�f�[�^�đ��

			if (m_apScore[nCntComp] != nullptr)
			{
				m_apScore[nCntComp]->SetValue(aScore[nCntComp]);
			}
		}
	}
}

//==========================
//�����L���O�̃��Z�b�g����
//==========================
void CRanking::Read(char *pReadFile)
{
	int aRankScore[5] = { 0,0,0,0,0 };

	FILE *pFile;									//�t�@�C���|�C���g��錾

	pFile = fopen(pReadFile, "r");

	if (pFile != NULL)
	{//�t�@�C���J������
		for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
		{
			fscanf(pFile, "%d", &aRankScore[nCnt]);
		}

		fclose(pFile);
	}

	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		if (m_apScore[nCnt] != nullptr)
		{
			m_apScore[nCnt]->SetValue(aRankScore[nCnt]);
		}
	}
}

//==========================
//�����L���O�̃��Z�b�g����
//==========================
void CRanking::Write(char *pReadFile)
{
	FILE *pFile;									//�t�@�C���|�C���g��錾

	pFile = fopen(pReadFile, "w");

	if (pFile != NULL)
	{//�t�@�C���J������
	
		for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
		{
			if (m_apScore[nCnt] != nullptr)
			{
				fprintf(pFile, "%d\n", m_apScore[nCnt]->GetValue());
			}
		}

		fclose(pFile);
	}
}
