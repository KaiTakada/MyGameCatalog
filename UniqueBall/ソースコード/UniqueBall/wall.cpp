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
	const char* TEX_FIELD = "data\\TEXTURE\\BG\\wall.jpg";
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
// �����F
// [In](posObj)����Ώۂ̍��W
// [In](posOldObj)����Ώۂ̉ߋ����W
// [Out](posCross)��_
// �Ԃ�l�F�����������ǂ���(true:��������,false:�������ĂȂ�)
//====================================
bool CWall::CollisionChara(D3DXVECTOR3 posObj, D3DXVECTOR3 posOldObj, D3DXVECTOR3 *posCross)
{
	bool bCol = false;

	D3DXVECTOR3 posPoly0;		//pos0
	D3DXVECTOR3 posPoly1;		//pos1
	D3DXVECTOR3 posPoly2;		//pos2
	D3DXVECTOR3 posPoly3;		//pos3

	GetPolyPos(&posPoly0, &posPoly1, &posPoly2, &posPoly3);

	D3DXVECTOR3 vecToPos;		//pos0��pos1�̍����i�ʒu�֌W�j
	D3DXVECTOR3 vecLine;		//�ǂ̒���
	D3DXVECTOR3 vecMove;		//�e�̃x�N�g��
	float fRate;		//��_�̊���

	vecLine = posPoly1 - posPoly0;
	vecToPos = posObj - posPoly0;
	vecMove = posObj - posOldObj;

	fRate = (((vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z))	//A
		/ ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z)));	//B

	if ((vecLine.z * vecToPos.x)
		- (vecLine.x * vecToPos.z) <= 0.0f
		&& fRate >= 0.0f && fRate <= 1.0f)
	{//��������
		if (posCross != nullptr)
		{
			*posCross = vecLine * fRate;	//��_
		}

		bCol = true;
	}

	return bCol;
}
