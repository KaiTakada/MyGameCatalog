//===============================================
//
// �X�R�A(score.cpp)
// Author ���c ����
//
//===============================================

//============================
// �C���N���[�h�t�@�C��
//============================
#include "timer.h"
#include "number.h"

//============================
// �}�N����`
//============================

//============================
// �ÓI�����o�ϐ��錾
//============================

//============================
// �R���X�g���N�^
//============================
CTimer::CTimer()
{
	for (int nCntCrt = 0; nCntCrt < MAX_TIME; nCntCrt++)
	{
		if (m_apNumber[nCntCrt] != nullptr)
		{
			m_apNumber[nCntCrt] = nullptr;
		}
	}

	m_nValue = 0;
	m_nSecond = 0;
}

//============================
// �f�X�g���N�^
//============================
CTimer::~CTimer()
{

}

//====================================
// 2D�|���S���̏���������(�f�t�H)
//====================================
HRESULT CTimer::Init(void)
{
	SetType(TYPE_SCORE);

	return S_OK;
}

//====================================
// 2D�|���S���̏�����(�ʒu�I�o��)
//====================================
HRESULT CTimer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	pos;
	rot;

	SetType(TYPE_SCORE);

	return S_OK;
}

//====================================
// 2D�|���S���̏�����(�ʒu�I�o��)
//====================================
HRESULT CTimer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	D3DXVECTOR3 posDif = pos;

	for (int nCntAll = 0; nCntAll < MAX_TIME; nCntAll++)
	{
		if (m_apNumber[nCntAll] == nullptr)
		{
			m_apNumber[nCntAll] = CNumber::Create(posDif, rot, size, 0);		//���g����

			posDif.x += size.x * 2;
		}
	}

	SetType(TYPE_SCORE);

	return S_OK;
}

//============================
// 2D�|���S���̏I������
//============================
void CTimer::Uninit(void)
{
	for (int nCntCrt = 0; nCntCrt < MAX_TIME; nCntCrt++)
	{
		if (m_apNumber[nCntCrt] != nullptr)
		{
			m_apNumber[nCntCrt]->Uninit();
			m_apNumber[nCntCrt] = nullptr;
		}
	}

	Release();
}

//============================
// 2D�|���S���̍X�V����
//============================
void CTimer::Update(void)
{
	m_nSecond++;

	if (m_nSecond >= 60)
	{
		CntValue(-1);
	
		m_nSecond = 0;
	}
}

//============================
// 2D�|���S���̕`�揈��
//============================
void CTimer::Draw(void)
{

}

//============================
// 2D�|���S���̐�������
//============================
CTimer * CTimer::Create(void)
{
	CTimer *pScore;

	//�I�u�W�F�N�g����
	pScore = new CTimer;

	//������
	pScore->Init();

	return pScore;
}

//============================
// 2D�|���S���̐�������
//============================
CTimer * CTimer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	CTimer *pScore = nullptr;

	//�I�u�W�F�N�g����&������
	if (pScore == nullptr)
	{
		pScore = new CTimer;

		if (pScore != nullptr)
		{
			pScore->Init(pos, rot, size);
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
// �X�R�A�l�ݒ�
//============================
void CTimer::SetValue(int nValue)
{
	m_nValue = nValue; 

	int aTex[MAX_TIME] = {};				//�e���̐������i�[
	int nCalc = nValue;

	//���l�̐ݒ�

	//�e�N�X�`�����W�̌v��
	for (int nCntScore = MAX_TIME - 1; nCntScore >= 0; nCntScore--)
	{
		aTex[nCntScore] = nCalc % 10;
		nCalc /= 10;
	}

	for (int nCntSet = 0; nCntSet < MAX_TIME; nCntSet++)
	{
		m_apNumber[nCntSet]->SetNumber(aTex[nCntSet]);
	}
}

//============================
// �X�R�A�l���Z
//============================
void CTimer::CntValue(int nValue)
{
	m_nValue += nValue;

	SetValue(m_nValue);
}


//============================
// �ʒu�ݒ�
//============================
void CTimer::SetPos(const D3DXVECTOR3 pos)
{
	pos;
}

//============================
// �����ݒ�
//============================
void CTimer::SetRot(const D3DXVECTOR3 rot)
{
	rot;
}

//============================
// �ʒu�擾
//============================
D3DXVECTOR3 CTimer::GetPos(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
// 2D�T�C�Y�擾
//============================
D3DXVECTOR3 CTimer::GetSize(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
