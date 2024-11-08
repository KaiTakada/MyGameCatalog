//=================================================
//
// �r���{�[�h(billboard.cpp)
// Author: Kai Takada
//
//=================================================
#include "target_mark.h"
#include "debugproc.h"
#include "texture.h"

//==============================
// �}�N����`
//==============================
namespace
{
	std::string TEX_PASS = "data\\TEXTURE\\target.png";
}

//==============================
// �ÓI�����o�ϐ��錾
//==============================

//==============================
// �R���X�g���N�^
//==============================
CTarget_Mark::CTarget_Mark(int nPriority) : CObjectBillboard(nPriority)
{
	m_nIdx = -1;
}

//==============================
// �f�X�g���N�^
//==============================
CTarget_Mark::~CTarget_Mark()
{

}

//==============================
// �r���{�[�h�̏���������
//==============================
HRESULT CTarget_Mark::Init(void)
{
	CObjectBillboard::Init();

	return S_OK;
}

//==============================
// �r���{�[�h�̏���������
//==============================
HRESULT CTarget_Mark::Init(const D3DXVECTOR3 pos)
{
	CObjectBillboard::Init(pos);

	return S_OK;
}

//==============================
// �r���{�[�h�̏I������
//==============================
void CTarget_Mark::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//==============================
// �r���{�[�h�̍X�V����
//==============================
void CTarget_Mark::Update(void)
{
	CObjectBillboard::Update();
}

//==============================
// �r���{�[�h�̕`�揈��
//==============================
void CTarget_Mark::Draw(void)
{
	if (m_nIdx  == CManager::GetInstance()->GetRenderer()->GetCamCtr() &&
		m_nIdx != -1)
	{
		CObjectBillboard::Draw();
	}
}

//============================
// 2D�|���S���̐�������
//============================
CTarget_Mark * CTarget_Mark::Create(void)
{
	CTarget_Mark *pThis;

	//�I�u�W�F�N�g����
	pThis = new CTarget_Mark;

	//������
	pThis->Init();
	pThis->SetLight(true);

	return pThis;
}

//============================
// 3D�|���S���̐�������
//============================
CTarget_Mark * CTarget_Mark::Create(const D3DXVECTOR3 pos)
{
	CTarget_Mark * pThis = nullptr;

	//�I�u�W�F�N�g����&������
	pThis = new CTarget_Mark;

	if (pThis != nullptr)
	{
		pThis->Init(pos);
		pThis->SetLight(true);
		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		pThis->SetIdxTexture(pTexture->Regist(TEX_PASS.c_str()));
	}
	else
	{
		return nullptr;
	}

	return pThis;
}
