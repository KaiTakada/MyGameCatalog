//============================
//
// X���f���I�u�W�F�N�g�p�[�c(Parts.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "Parts.h"
#include "Xmodel.h"

//===============================
// �}�N����`
//===============================

//============================
// �ÓI�����o�ϐ��錾
//============================

//=================================
// �R���X�g���N�^
//=================================
CParts::CParts(int nPriority) : CObject(nPriority)
{
	D3DXMatrixIdentity(&m_mtxWorld);				// ���[���h�}�g���b�N�X
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_pParent = nullptr;		//�e���f���ւ̃|�C���^
	m_nIdxModel = 0;
	m_bMultiply = false;
	
	for (int i = 0; i < my_parts::NUM_TEX_PASS; i++)
	{
		m_sTexPass[i] = "";
		m_pTexture[i] = nullptr;
	}

	m_nIdxParent = -1;

	ZeroMemory(&m_mat.MatD3D, sizeof(D3DMATERIAL9));
}

//=================================
// �f�X�g���N�^
//=================================
CParts::~CParts()
{

}

//=================================
// ������
//=================================
HRESULT CParts::Init(void)
{
	SetType(TYPE_PARTS);

	return E_FAIL;
}

//=================================
// ������
//=================================
HRESULT CParts::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;

	SetType(TYPE_PARTS);

	return S_OK;
}

//=================================
// �I��
//=================================
void CParts::Uninit(void)
{
	m_pParent = nullptr;

	for (int nCntTex = 0; nCntTex < my_parts::NUM_TEX_PASS; nCntTex++)
	{
		if (m_pTexture[nCntTex] != nullptr)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = nullptr;
		}
	}

	Release();
}

//=================================
// �X�V
//=================================
void CParts::Update(void)
{
}

//=================================
// �`��
//=================================
void CParts::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;					//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;						//�e�̃}�g���b�N�X

	CXModel *pXModel = CManager::GetInstance()->GetXModel();
	CXModel::Model *pModel = pXModel->GetAddress(m_nIdxModel);

	//�e�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxParent);
	D3DXMatrixIdentity(&pModel->mtxWorld);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.y, m_scale.x, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�p�[�c�́u�e�̃}�g���b�N�X�v��ݒ�
	if (m_pParent != nullptr)
	{//�e���f��������ꍇ
		mtxParent = m_pParent->GetMtxWorld();		//�e���f���̃}�g���b�N�X���w��
	}
	else
	{//�e���f�����Ȃ��ꍇ
		if (!m_bMultiply)
		{//�|�����킹��ݒ�̏ꍇ
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);			// �ŐV�̃}�g���b�N�X��n��(DirectX�̋L�^)
		}
	}

	//�Z�o�����u�p�[�c�̃}�g���b�N�X�v�Ɓu�e�̃}�g���b�N�X�v���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		if (m_mat.MatD3D.Ambient.a == 0.0f)
		{
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		}
		else
		{
			pDevice->SetMaterial(&m_mat.MatD3D);
		}
		
		//�e�N�X�`���̐ݒ�
		if (m_sTexPass[m_nIdxParent] != "" && m_nIdxParent != -1)
		{//�����ւ�
			pDevice->SetTexture(0, m_pTexture[m_nIdxParent]);
		}
		else
		{
			pDevice->SetTexture(0, pModel->pTexture[nCntMat]);
		}

		//���f��(�p�[�c)�̕`��
		pModel->pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=================================
// ����
//=================================
CParts * CParts::Create(const char *pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CParts *pParts = nullptr;

	//�I�u�W�F�N�g����
	pParts = new CParts;

	if (pParts != nullptr)
	{
		//������
		if (FAILED(pParts->Init(pos, rot)))
		{
			pParts = nullptr;
			return nullptr;
		}

		CXModel *pXmodel = CManager::GetInstance()->GetXModel();
		pParts->SetIdxModel(pXmodel->Regist(pFilename));		//�e�N�X�`���̊��蓖��

	}
	else
	{
		return nullptr;
	}

	return pParts;
}

//=================================
// ���v���W�擾
//=================================
D3DXVECTOR3 CParts::GetOutcomePos(CParts * pParts)
{
	D3DXVECTOR3 answer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CParts * pNow = pParts;
	
	while (true)
	{
		if (pNow == nullptr)
		{
			break;
		}

		answer += pNow->GetPos();

		pNow = pNow->GetParent();
	}

	return answer;
}

//=================================
// �����ւ��e�N�X�`���ݒ�
//=================================
void CParts::SetTexPass(std::string sTexPass, int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	m_sTexPass[nIdx] = sTexPass;

	D3DXCreateTextureFromFile(
		pDevice,
		const_cast<char*>(sTexPass.c_str()),
		&m_pTexture[nIdx]);
}

//=================================
// �e���f���̐ݒ�
//=================================
void CParts::SetParent(CParts * pParts)
{
	m_pParent = pParts;
}
