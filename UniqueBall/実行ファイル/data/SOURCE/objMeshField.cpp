//============================
//
// ���b�V���n��(objMeshField.cpp)
// Author: Kai Takada
//
//============================
//==================================
// �C���N���[�h�t�@�C��
//==================================
#include "objMeshField.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"

//==================================
// �萔��`
//==================================
//�ꖇ�ɂ�����ő咸�_��(64000)

namespace
{
	const float MESHFIELD_SIZE(600.0f);	//���a�̒���
	const int CUT_U(2);		//������x
	const int CUT_V(2);		//������z
	const char* TEX_FIELD = "data\\TEXTURE\\BG\\field.jpg";
	const int DEF_NUM_DIF(8);
}

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// �R���X�g���N�^
//==================================
CObjMeshField::CObjMeshField(int nPriority) : CObject(nPriority)
{
	m_pos = mylib_const::DEFVEC3;
	m_rot = mylib_const::DEFVEC3;
	m_size = mylib_const::DEFVEC2;
	m_mtxWorld = {};
	m_nNumCutV = 0;
	m_nNumCutU = 0;
	m_nNumVtxV = 0;
	m_nNumVtxU = 0;
	m_nNumVtxAll = 0;
	m_nNumIdx = 0;
	m_nNumPrim = 0;
	
	m_pIdxBuff= nullptr;
	m_pVtxBuff = nullptr;
	m_nIdxTexture = 0;

	m_bWireFrame = false;
	m_nCull = 0;

	m_bZtest = false;	
	m_bAtest = false;
	m_bAbrend = false;
	m_bLight = false;
}

//==================================
// �f�X�g���N�^
//==================================
CObjMeshField::~CObjMeshField()
{

}

//==================================
// ����������
//==================================
HRESULT CObjMeshField::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_nNumCutV = CUT_V;
	m_nNumCutU = CUT_U;
	m_nNumVtxV = m_nNumCutV + 1;
	m_nNumVtxU = m_nNumCutU + 1;
	m_nNumVtxAll = m_nNumVtxV * m_nNumVtxU;			//�S���_

	m_nNumIdx = m_nNumVtxV * m_nNumVtxU				//�S���_
		+ (m_nNumVtxV - 2) * m_nNumVtxU		//���Ԃ�
		+ (m_nNumVtxV - 2) * 2;									//�΂�	

	m_nNumPrim = (m_nNumCutV * m_nNumCutU) * 2	//�`��|���S��
		+ (m_nNumVtxV - 2) * 4;								//�k�ރ|���S��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxAll,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//���a
	float fRadiusU = MESHFIELD_SIZE;
	float fRadiusV = MESHFIELD_SIZE;

	//�X�^�[�g
	float fStartU = -fRadiusU;
	float fStartV = fRadiusV;

	//���_�Ԃ̋���
	float fLargeU = (fRadiusU * 2) / m_nNumCutU;	//��
	float fLargeV = -(fRadiusV * 2) / m_nNumCutV;	//�c

	//���_���̃|�C���^
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�E�@���x�N�g���E���_�J���[�̐ݒ�
	for (int nCntPosV = 0; nCntPosV < m_nNumVtxV; nCntPosV++)
	{//�c
		for (int nCntPosU = 0; nCntPosU < m_nNumVtxU; nCntPosU++)
		{//��
			if (nCntPosU == 355 && nCntPosV == 2)
			{
				pVtx[0] = pVtx[0];
			}

			//���_���W
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].pos = D3DXVECTOR3(fStartU + (nCntPosU * fLargeU),
				0.0f,
				fStartV + (nCntPosV * fLargeV));		//(x,y,z)

			//�e�N�X�`�����W
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].tex = D3DXVECTOR2(nCntPosU * 1.0f, nCntPosV * 1.0f);

			//�@���x�N�g��
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	WORD* pIdx;		//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCtrV = 0;

	for (int nCntIdx = 0; nCntIdx < m_nNumIdx / 2; nCntIdx++)
	{//�C���f�b�N�X����
		nCtrV = nCntIdx / (m_nNumVtxU + 1);	// ���i�ڂ�

		if (nCntIdx == (m_nNumVtxU + ((m_nNumVtxU * nCtrV) + nCtrV)) &&
			nCntIdx != 0)	//�΂߂̎�����������
		{
			pIdx[nCntIdx * 2 + 1] = (WORD)(nCntIdx - (nCtrV)+m_nNumVtxU);
			pIdx[nCntIdx * 2] = (WORD)(nCntIdx - (nCtrV)-1);
		}
		else
		{
			pIdx[nCntIdx * 2 + 1] = (WORD)(nCntIdx - (nCtrV));
			pIdx[nCntIdx * 2] = (WORD)(nCntIdx - (nCtrV)+m_nNumVtxU);
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	//�e�N�X�`���ԍ�
	SetIdxTexture(0);

	SetType(TYPE_MESHFIELD);

	return S_OK;
}

//====================================
// ������(�ʒu�I�o��)
//====================================
HRESULT CObjMeshField::Init(const int nNumCutU, const int nNumCutV)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_nNumCutV = nNumCutV;
	m_nNumCutU = nNumCutU;
	m_nNumVtxV = nNumCutV + 1;
	m_nNumVtxU = nNumCutU + 1;
	m_nNumVtxAll = m_nNumVtxV * m_nNumVtxU;			//�S���_

	m_nNumIdx = m_nNumVtxV * m_nNumVtxU				//�S���_
		+ (m_nNumVtxV - 2) * m_nNumVtxU		//���Ԃ�
		+ (m_nNumVtxV - 2) * 2;									//�΂�	

	m_nNumPrim = (m_nNumCutV * m_nNumCutU) * 2	//�`��|���S��
		+ (m_nNumVtxV - 2) * 4;								//�k�ރ|���S��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxAll,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//���a
	float fRadiusU = MESHFIELD_SIZE;
	float fRadiusV = MESHFIELD_SIZE;

	//�X�^�[�g
	float fStartU = -fRadiusU;
	float fStartV = fRadiusV;

	//���_�Ԃ̋���
	float fLargeU = (fRadiusU * 2) / m_nNumCutU;	//��
	float fLargeV = -(fRadiusV * 2) / m_nNumCutV;	//�c

	//���_���̃|�C���^
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�E�@���x�N�g���E���_�J���[�̐ݒ�
	for (int nCntPosV = 0; nCntPosV < m_nNumVtxV; nCntPosV++)
	{//�c
		for (int nCntPosU = 0; nCntPosU < m_nNumVtxU; nCntPosU++)
		{//��
			if (nCntPosU == 355 && nCntPosV == 2)
			{
				pVtx[0] = pVtx[0];
			}

			//���_���W
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].pos = D3DXVECTOR3(fStartU + (nCntPosU * fLargeU),
				0.0f,
				fStartV + (nCntPosV * fLargeV));		//(x,y,z)

			//�e�N�X�`�����W
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].tex = D3DXVECTOR2(nCntPosU * 1.0f, nCntPosV * 1.0f);

			//�@���x�N�g��
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD* pIdx;		//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCtrV = 0;

	for (int nCntIdx = 0; nCntIdx < m_nNumIdx / 2; nCntIdx++)
	{//�C���f�b�N�X����
		nCtrV = nCntIdx / (m_nNumVtxU + 1);	// ���i�ڂ�

		if (nCntIdx == (m_nNumVtxU + ((m_nNumVtxU * nCtrV) + nCtrV)) &&
			nCntIdx != 0)	//�΂߂̎�����������
		{
			pIdx[nCntIdx * 2 + 1] = (WORD)(nCntIdx - (nCtrV)+m_nNumVtxU);
			pIdx[nCntIdx * 2] = (WORD)(nCntIdx - (nCtrV)-1);
		}
		else
		{
			pIdx[nCntIdx * 2 + 1] = (WORD)(nCntIdx - (nCtrV));
			pIdx[nCntIdx * 2] = (WORD)(nCntIdx - (nCtrV)+m_nNumVtxU);
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	//�e�N�X�`���ԍ�
	SetIdxTexture(0);

	SetType(TYPE_MESHFIELD);

	return S_OK;
}

//==================================
// �I������
//==================================
void CObjMeshField::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;					//�o�b�t�@�̔j��
	}

	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;					//�o�b�t�@�̔j��
	}

	//���S�t���O�����Ă��Ⴄ
	SetDeath(true);
	Release();
}

//==================================
// �X�V����
//==================================
void CObjMeshField::Update(void)
{
	UpdateVtx();
}

//==================================
// �`�揈��
//==================================
void CObjMeshField::Draw(void)
{
	//�e�N�X�`���擾
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�����_���[�̐ݒ�
	DrawRenderSet(pDevice);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,
		0,													//�ŏ��C���f�b�N�X
		m_nNumVtxAll,										//�p�ӂ������_�̐�
		0,													//���ԃX�^�[�g��
		m_nNumPrim);										//�`�悷��v���~�e�B�u�̐�

	//�����_���[�f�t�H���g�ݒ�
	DrawRenderReset(pDevice);
}

//============================
// ��������
//============================
CObjMeshField* CObjMeshField::Create()
{
	CObjMeshField* pField = nullptr;

	//�I�u�W�F�N�g����
	pField = new CObjMeshField;

	//������
	pField->Init();

	return pField;
}

//============================
// ��������
//============================
CObjMeshField* CObjMeshField::Create(const int nNumCutU, const int nNumCutV)
{
	CObjMeshField* pField = nullptr;

	//�I�u�W�F�N�g����&������
	pField = new CObjMeshField;

	if (pField != nullptr)
	{
		pField->Init(nNumCutU, nNumCutV);

		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		pField->SetIdxTexture(pTexture->Regist(TEX_FIELD));
	}
	else
	{
		return nullptr;
	}

	return pField;
}

//============================
// ���_�ʒu�擾
//============================
void CObjMeshField::GetPolyPos(D3DXVECTOR3* Vtx0, D3DXVECTOR3* Vtx1, D3DXVECTOR3* Vtx2, D3DXVECTOR3* Vtx3)
{
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	if (Vtx0 != nullptr)*Vtx0 = pVtx[0].pos;		//(x,y,z)
	if (Vtx1 != nullptr)*Vtx1 = pVtx[m_nNumCutU].pos;
	if (Vtx2 != nullptr)*Vtx2 = pVtx[m_nNumVtxAll - m_nNumCutU].pos;
	if (Vtx3 != nullptr)*Vtx3 = pVtx[m_nNumVtxAll - 1].pos;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================
// ����Ă���I�u�W�F�N�g�̈ʒuy�擾
//====================================
float CObjMeshField::GetHeight(D3DXVECTOR3 posObj)
{
	D3DXVECTOR3 vecToPos0, vecToPos1, vecToPos2, vecToPos3;			//pos0��pos1�̍����i�ʒu�֌W�j
	D3DXVECTOR3 vecLine0, vecLine1, vecLine2, vecLine3;		//�ǂ̒���
	D3DXVECTOR3 vecLineMid, vecToPosMid;	//�^�񒆂̒����E����

	D3DXVECTOR3 Vtxpos0, Vtxpos1, Vtxpos2, Vtxpos3;

	GetPolyPos(&Vtxpos0, &Vtxpos1, &Vtxpos2, &Vtxpos3);

	vecLine0 = Vtxpos1 - Vtxpos0;
	vecToPos0 = posObj - Vtxpos0;

	vecLine1 = Vtxpos0 - Vtxpos2;
	vecToPos1 = posObj - Vtxpos2;

	vecLine2 = Vtxpos2 - Vtxpos3;
	vecToPos2 = posObj - Vtxpos3;

	vecLine3 = Vtxpos3 - Vtxpos1;
	vecToPos3 = posObj - Vtxpos1;

	//�^��
	vecLineMid = Vtxpos1 - Vtxpos2;
	vecToPosMid = posObj - Vtxpos2;

	//�v���C���[���|���S���̓����ɂ���
	if ((vecLine0.z * vecToPos0.x) - (vecLine0.x * vecToPos0.z) >= 0.0f &&
		(vecLine1.z * vecToPos1.x) - (vecLine1.x * vecToPos1.z) >= 0.0f &&
		(vecLine2.z * vecToPos2.x) - (vecLine2.x * vecToPos2.z) >= 0.0f &&
		(vecLine3.z * vecToPos3.x) - (vecLine3.x * vecToPos3.z) >= 0.0f)
	{
		D3DXVECTOR3 nor;	//�@��
		float fHeight;		//���߂鍂��

		//�ΏۃI�u�W�F�N�g�ւ̃x�N�g��
		D3DXVECTOR3 vec0;
		D3DXVECTOR3 vec1;
		D3DXVECTOR3 VtxposOrg;
		D3DXVECTOR3 vecP;

		if ((vecLineMid.z * vecToPosMid.x) - (vecLineMid.x * vecToPosMid.z) <= 0.0f)
		{
			VtxposOrg = Vtxpos0;
			vecP = posObj - Vtxpos0;
			vec0 = Vtxpos1 - Vtxpos0;
			vec1 = Vtxpos2 - Vtxpos0;
		}
		else
		{
			VtxposOrg = Vtxpos3;
			vecP = posObj - Vtxpos3;
			vec0 = Vtxpos2 - Vtxpos3;
			vec1 = Vtxpos1 - Vtxpos3;
		}

		//2�̃x�N�g������@�������߂�
		D3DXVec3Cross(&nor, &vec0, &vec1);

		//�@���𐳋K��
		D3DXVec3Normalize(&nor, &nor);

		if (nor.y != 0.0f)
		{//�N�����������ꍇ?

			fHeight = (-((vecP.x * nor.x) + (vecP.z * nor.z)) + (VtxposOrg.y * nor.y)) / nor.y;

			return fHeight;
		}
	}

	return posObj.y;
}

//==========================================
// �`�掞�����_���[�ݒ�
//==========================================
void CObjMeshField::DrawRenderSet(LPDIRECT3DDEVICE9 pDevice)
{
	//Z�e�X�g�𖳌��ɂ���
	if (m_bZtest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	//�A���t�@�e�X�g��L���ɂ���
	if (m_bAtest == true)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	}

	//a�u�����f�B���O�����Z�����ɐݒ�
	if (m_bAbrend == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//���C�e�B���O��TRUE�ɐݒ�
	if (m_bLight == true)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
}

//==========================================
// �`�掞�����_���[�ݒ����
//==========================================
void CObjMeshField::DrawRenderReset(LPDIRECT3DDEVICE9 pDevice)
{
	//���C�e�B���O��TRUE�ɐݒ�
	if (m_bLight)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	//Z�e�X�g�𖳌��ɂ���
	if (m_bZtest)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//�A���t�@�e�X�g��L���ɂ���
	if (m_bAtest)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
	}

	//a�u�����f�B���O�����Z�����ɐݒ�
	if (m_bAbrend)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//==========================================
// ���_���X�V
//==========================================
void CObjMeshField::UpdateVtx()
{
	//�X�^�[�g
	float fStartU = -m_size.x;
	float fStartV = m_size.y;
	
	//���_�Ԃ̋���
	float fLargeU = (m_size.x * 2) / m_nNumCutU;	//��
	float fLargeV = -(m_size.y * 2) / m_nNumCutV;	//�c

	//���_���̃|�C���^
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�E�@���x�N�g���E���_�J���[�̐ݒ�
	for (int nCntPosV = 0; nCntPosV < m_nNumVtxV; nCntPosV++)
	{//�c
		for (int nCntPosU = 0; nCntPosU < m_nNumVtxU; nCntPosU++)
		{//��
			if (nCntPosU == 355 && nCntPosV == 2)
			{
				pVtx[0] = pVtx[0];
			}

			//���_���W
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].pos = D3DXVECTOR3(fStartU + (nCntPosU * fLargeU),
				0.0f,
				fStartV + (nCntPosV * fLargeV));		//(x,y,z)

			//�e�N�X�`�����W
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].tex = D3DXVECTOR2(nCntPosU * 1.0f, nCntPosV * 1.0f);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
