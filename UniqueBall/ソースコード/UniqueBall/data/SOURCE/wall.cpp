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
	const char* TEX_FIELD = "data\\TEXTURE\\BG\\bg_001.jpg";
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
	SetRot(DEF_VERTEX3D);	//����������
	SetSize(D3DXVECTOR3(DEF_SIZE.x, DEF_SIZE.y, 0.0f));

	D3DXVECTOR3 lRot = GetRot();	//����������
	SetRot({ lRot.x, D3DX_PI * 1.0f, lRot.z });	//����������

	SetType(TYPE_WALL);

	return S_OK;
}

//====================================
// ������(�ʒu�I�o��)
//====================================
HRESULT CWall::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject3D::Init(pos, rot);

	D3DXVECTOR3 lRot = GetRot();	//����������
	SetRot({ D3DX_PI * -0.5f, lRot.y, lRot.z });	//����������

	//�T�C�Y�ݒ�
	SetSize(D3DXVECTOR3(DEF_SIZE.x, 0.0f, DEF_SIZE.z));

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

		float fposZ = mylib_const::DEF_FIELD_SIZE.z;
		pWall->SetPos({ pos.x, pos.y + fposZ, pos.z });

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

	D3DXVECTOR3 posPoly0 = mylib_const::DEFVEC3;		//pos0
	D3DXVECTOR3 posPoly1 = mylib_const::DEFVEC3;		//pos1

	GetPolyPos(&posPoly0, &posPoly1, nullptr, nullptr);

	posPoly0 += GetPos();		//pos0
	posPoly1 += GetPos();		//pos1

	if (!mylib_useful::CollisionLine(posPoly0, posPoly1, posObj))
	{//�O��͓����炸�A���񂾂��������Ă���
		if (mylib_useful::CollisionLine(posPoly0, posPoly1, posOldObj))
		{
			bCol = true;
		}
	}

	return bCol;
}
