//===============================================
//
// ����(number.cpp)
// Author ���c ����
//
//===============================================

//============================
// �C���N���[�h�t�@�C��
//============================
#include "number.h"
#include "object.h"
#include "texture.h"

//============================
// �}�N����`
//============================
namespace
{
	const float NUM_SPEED = 20.0f;											//�ړ���
	const D3DXVECTOR3 NUM_SIZE = D3DXVECTOR3(125.0f, 200.0f, 0.0f);				//�f�t�H���g�T�C�Y
	const char* NUMBER_PASS = "data\\TEXTURE\\NUMBER\\number000.png";		//�f�t�H���g�T�C�Y
}

//============================
// �ÓI�����o�ϐ��錾
//============================

//============================================
//==================== �e ====================
//============================
// �R���X�g���N�^
//============================
CNumber::CNumber()
{
	m_nValue = 0;
}

//============================
// �f�X�g���N�^
//============================
CNumber::~CNumber()
{

}

//====================================
// �����̏�����(�f�t�H)
//====================================
HRESULT CNumber::Init(void)
{
	//������
	CObject2D::Init();
	SetSize(NUM_SIZE);

	SetType(TYPE_NUMBER);

	return S_OK;
}

//====================================
// �����̏�����(�ʒu�I�o��)
//====================================
HRESULT CNumber::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//������
	if(FAILED(CObject2D::Init(pos, rot)))
	{
		return E_FAIL;
	}

	//�^�C�v�ݒ�
	SetType(TYPE_NUMBER);

	return S_OK;
}

//============================
// �����̏I��
//============================
void CNumber::Uninit(void)
{
	//�I��
	CObject2D::Uninit();
}

//============================
// �����̍X�V
//============================
void CNumber::Update(void)
{

}

//============================
// �����̕`��
//============================
void CNumber::Draw(void)
{
	//�`��
	CObject2D::Draw();
}

//============================
// ����
//============================
CNumber* CNumber::Create(int nValue)
{
	CNumber* pNumber = nullptr;

	//�I�u�W�F�N�g����&������
	pNumber = new CNumber;

	if (pNumber != nullptr)
	{
		if (FAILED(pNumber->Init()))
		{
			return nullptr;
		}
		else
		{
			CTexture* pTexture = CManager::GetInstance()->GetTexture();
			pNumber->SetIdxTexture(pTexture->Regist(NUMBER_PASS));

			pNumber->SetNumber(nValue);		//���l���蓖��
		}
	}
	else
	{
		return nullptr;
	}

	return pNumber;
}

//============================
// ����
//============================
CNumber *CNumber::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, int nValue)
{
	CNumber *pNumber = nullptr;

	//�I�u�W�F�N�g����&������
	pNumber = new CNumber;

	if (pNumber != nullptr)
	{
		if (FAILED(pNumber->Init(pos, rot)))
		{
			return nullptr;
		}
		else
		{
			pNumber->SetSize(size);		//�T�C�Y�ݒ�

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pNumber->SetIdxTexture(pTexture->Regist(NUMBER_PASS));

			pNumber->SetNumber(nValue);		//���l���蓖��
		}
	}
	else
	{
		return nullptr;
	}

	return pNumber;
}

//============================
// �C�ӂ̐��l��`��ʒu�ݒ�
//============================
void CNumber::SetNumber(int nNumber)
{
	m_nValue = nNumber;

	D3DXVECTOR2 rightDown = { 0.1f * nNumber + 0.1f,1.0f };
	D3DXVECTOR2 leftUp = { 0.1f * nNumber, 0.0f };
	
	SetTexPos(rightDown, leftUp);
}
