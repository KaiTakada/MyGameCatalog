//============================
//
// X���f���̏���(model.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "Xmodel.h"

//===============================
// �}�N����`
//===============================

//============================
// �ÓI�����o�ϐ��錾
//============================
int CXModel::m_nNumAll = 0;				//���ʃe�N�X�`���o�b�t�@

//============================
// ���f���p�X
//============================
const char *c_apXFilepass[] =
{
	"data\\MODEL\\null.x"
};

//=================================
// �R���X�g���N�^
//=================================
CXModel::CXModel()
{
	m_aModel.clear();
	m_apFilename.clear();
}

//=================================
// �f�X�g���N�^
//=================================
CXModel::~CXModel()
{
	m_aModel.clear();
	m_apFilename.clear();
}

//=================================
// �ǂݍ���
//=================================
HRESULT CXModel::Load(void)
{
	for (int nCntFile = 0; nCntFile < sizeof(c_apXFilepass) / sizeof(c_apXFilepass[0]); nCntFile++)
	{
		Regist(c_apXFilepass[nCntFile]);
	}

	return S_OK;
}

//=================================
// �j��
//=================================
void CXModel::Unload(void)
{
	// ���X�g���[�v
	for (std::list<Model>::const_iterator it = m_aModel.cbegin(); it != m_aModel.cend(); it++)
	{
		Model model = *it;

		if (model.pMesh != nullptr)
		{
			model.pMesh->Release();
			model.pMesh = nullptr;
		}

		if (model.pBuffMat != nullptr)
		{
			model.pBuffMat->Release();
			model.pBuffMat = nullptr;
		}

		if (model.pTexture != nullptr)
		{
			delete model.pTexture;
			model.pTexture = nullptr;
		}
	}

	m_aModel.clear();
	m_apFilename.clear();
}

//============================
// �ǉ��Ǎ�
//============================
int CXModel::Regist(const char *pFilename, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	int nNumVtx = 0;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	int nCntReg = 0;

	for (nCntReg = 0; nCntReg < MAX_XMODEL; nCntReg++)
	{
		Model* model = GetAddress(nCntReg);

		if (model == nullptr)
		{//��񂪓����Ă��Ȃ�
			break;
		}

		std::string str = GetFilename(nCntReg);
		if (strcmp(str.c_str(), (pFilename)) == 0)
		{//��񂪓����Ă���
			
			if (vtxMin != nullptr) vtxMin = &model->vtxMin;
			if (vtxMax != nullptr) vtxMax = &model->vtxMax;

			return nCntReg;
		}
	}

	Model* pModel = new Model;

	//X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModel->pBuffMat,
		NULL,
		&pModel->dwNumMat,
		&pModel->pMesh)))
	{
		return 0;
	}

	//�}�e���A�����ɑ΂���|�C���^���擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	if (pModel->pTexture != nullptr)
	{
		pModel->pTexture = nullptr;
	}

	//TODO: ��������ēx�u���b�V���A�b�v
	pModel->pTexture = new LPDIRECT3DTEXTURE9[(int)pModel->dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,				//�e�N�X�`���̃t�@�C����
				&pModel->pTexture[nCntMat])))	//�e�N�X�`���p�X���Ԉ���Ă���ƕ`�掞�ɉ���(breakpoint)
			{
				pModel->pTexture[nCntMat] = nullptr;
			}
		}
		else
		{
			pModel->pTexture[nCntMat] = nullptr;
		}
	}

	//���_�����擾
	nNumVtx = pModel->pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y�̎擾
	dwSizeFVF = D3DXGetFVFVertexSize(pModel->pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

		//���ׂĂ̒��_���r���ēG�̍ŏ��l�E�ő�l�𔲂��o��
		if (pModel->vtxMax.x < vtx.x)
		{
			pModel->vtxMax.x = vtx.x;
		}
		if (pModel->vtxMax.y < vtx.y)
		{
			pModel->vtxMax.y = vtx.y;
		}
		if (pModel->vtxMax.z < vtx.z)
		{
			pModel->vtxMax.z = vtx.z;
		}

		if (pModel->vtxMin.x > vtx.x)
		{
			pModel->vtxMin.x = vtx.x;
		}
		if (pModel->vtxMin.y > vtx.y)
		{
			pModel->vtxMin.y = vtx.y;
		}
		if (pModel->vtxMin.z > vtx.z)
		{
			pModel->vtxMin.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	pModel->pMesh->UnlockVertexBuffer();

	vtxMin = &pModel->vtxMin;
	vtxMax = &pModel->vtxMax;

	//���X�g�ɒǉ�
	m_aModel.push_back(*pModel);
	m_apFilename.push_back(pFilename);

	//�|�C���^�폜
	delete pModel;

	m_nNumAll++;
	return nCntReg;
}

//============================
// ���擾
//============================
CXModel::Model* CXModel::GetAddress(int nIdx)
{
	if (nIdx < 0 || nIdx >= static_cast<int>(m_aModel.size()))
	{
		return nullptr;
	}
	return &(*std::next(m_aModel.begin(), nIdx));
}

//============================
// �p�X�擾
//============================
std::string CXModel::GetFilename(int nIdx)
{
	if (nIdx < 0 || nIdx >= static_cast<int>(m_apFilename.size()))
	{
		return nullptr;
	}
	std::string str = (*std::next(m_apFilename.begin(), nIdx));
	return str;
}

//============================
// �ŏ���`�擾
//============================
D3DXVECTOR3 CXModel::GetVtxMin(int nIdx)
{
	if (nIdx < 0 || nIdx >= static_cast<int>(m_aModel.size()))
	{
		return mylib_const::DEFVEC3;
	}

	Model* model = &(*std::next(m_aModel.begin(), nIdx));
	
	return model->vtxMax;
}

//============================
// �ő��`�擾
//============================
D3DXVECTOR3 CXModel::GetVtxMax(int nIdx)
{
	if (nIdx < 0 || nIdx >= static_cast<int>(m_aModel.size()))
	{
		return mylib_const::DEFVEC3;
	}

	Model* model = &(*std::next(m_aModel.begin(), nIdx));

	return model->vtxMin;
}
