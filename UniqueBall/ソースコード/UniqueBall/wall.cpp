//============================
//
// �n�ʏ���(Field.cpp)
// Author: Kai Takada
//
//============================
//==================================
// �C���N���[�h�t�@�C��
//==================================
#include "wall.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"

//==================================
// �}�N����`
//==================================
namespace
{
	const char* TEX_FIELD = "data\\TEXTURE\\FIELD\\rock.jpg";
	const int DEF_NUM_DIF = 8;
}

//==================================
// �ÓI�����o�ϐ��錾
//==================================

//==================================
// �R���X�g���N�^
//==================================
CWall::CWall(int nPriority) : CObject3D(nPriority)
{

}

//==================================
// �f�X�g���N�^
//==================================
CWall::~CWall()
{

}

//==================================
// ����������
//==================================
HRESULT CWall::Init(void)
{
	CObject3D::Init();

	SetPos(DEF_VERTEX3D);	//����������
	SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));	//����������
	SetSize(D3DXVECTOR3(DEF_SIZE.x, DEF_SIZE.y, 0.0f));

	SetType(TYPE_WALL);

	return S_OK;
}

//====================================
// ������(�ʒu�I�o��)
//====================================
HRESULT CWall::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject3D::Init(pos,rot);

	//�T�C�Y�ݒ�
	SetSize(D3DXVECTOR3(DEF_SIZE.x, DEF_SIZE.z, 0.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));	//����������

	SetType(TYPE_WALL);

	return S_OK;
}

//==================================
// �I������
//==================================
void CWall::Uninit(void)
{
	CObject3D::Uninit();
}

//==================================
// �X�V����
//==================================
void CWall::Update(void)
{
	CObject3D::Update();
}

//==================================
// �`�揈��
//==================================
void CWall::Draw(void)
{
	CObject3D::Draw();
}

//============================
// ��������
//============================
CWall * CWall::Create(void)
{
	CWall *pWall;

	//�I�u�W�F�N�g����
	pWall = new CWall;

	//������
	pWall->Init();

	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	pWall->SetIdxTexture(pTexture->Regist(TEX_FIELD));

	return pWall;
}

//============================
// ��������
//============================
CWall * CWall::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	CWall *pWall = nullptr;

	//�I�u�W�F�N�g����&������
	pWall = new CWall;

	if (pWall != nullptr)
	{
		pWall->Init(pos, rot);

		pWall->SetSize(size);

		int tex0 = int(size.x / 256.0f);
		int tex1 = int(size.z / 256.0f);
		pWall->SetDivTex(tex0, tex1);

		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		pWall->SetIdxTexture(pTexture->Regist(TEX_FIELD));
	}
	else
	{
		return nullptr;
	}

	return pWall;
}

//====================================
// ����Ă���I�u�W�F�N�g�̈ʒuy�擾
//====================================
float CWall::CollisionChara(D3DXVECTOR3 posObj)
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
		D3DXVECTOR3 nor;
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
		{//�N�����Ȃ������ꍇ

			fHeight = (-((vecP.x * nor.x) + (vecP.z * nor.z)) + (VtxposOrg.y * nor.y)) / nor.y;

			return fHeight;
		}
	}

	return posObj.y;
}
