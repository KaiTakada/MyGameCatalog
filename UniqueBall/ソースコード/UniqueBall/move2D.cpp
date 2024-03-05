//===============================================
//
// �ړ�2D(move2D.cpp)
// Author ���c ����
//
//===============================================

//============================
// �C���N���[�h�t�@�C��
//============================
#include "move2D.h"
#include "texture.h"

//============================
// �}�N����`
//============================
namespace
{
	const D3DXVECTOR3 DEF_SIZE = { SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT, 0.0f };	//�f�t�H���g�T�C�Y
}

//============================
// �ÓI�����o�ϐ��錾
//============================

//============================
// �R���X�g���N�^(�f�t�H)
//============================
CMove2D::CMove2D(int nPriority) : CObject2D(nPriority)
{
	m_posDest = mylib_const::DEFVEC3;
	m_fSpeed = 0.0f;
}

//============================
// �f�X�g���N�^
//============================
CMove2D::~CMove2D()
{

}

//====================================
// 2D�|���S���̏���������(�f�t�H)
//====================================
HRESULT CMove2D::Init(void)
{
	CObject2D::Init();

	return S_OK;
}

//====================================
// 2D�|���S���̏�����(�ʒu�I�o��)
//====================================
HRESULT CMove2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject2D::Init(pos,rot);

	return S_OK;
}

//============================
// 2D�|���S���̏I������
//============================
void CMove2D::Uninit(void)
{
	CObject2D::Uninit();
}

//============================
// 2D�|���S���̍X�V����
//============================
void CMove2D::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = mylib_const::DEFVEC3;
	float fRot = mylib_useful::Point2Angle(m_posDest, pos);

	move.x = sinf(fRot + D3DX_PI) * m_fSpeed;
	move.y = cosf(fRot + D3DX_PI) * m_fSpeed;
	
	pos += move;

	SetPos(pos);

	CObject2D::Update();
}

//============================
// 2D�|���S���̕`�揈��
//============================
void CMove2D::Draw(void)
{
	CObject2D::Draw();
}

//============================
// 2D�|���S���̐�������
//============================
CMove2D * CMove2D::Create(void)
{
	CMove2D *pObject2D;

	//�I�u�W�F�N�g����
	pObject2D = new CMove2D;

	//������
	pObject2D->Init();

	return pObject2D;
}

//============================
// 2D�|���S���̐�������
//============================
CMove2D * CMove2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posDest, const float fSpeed)
{
	CMove2D *pObject2D = nullptr;

	pObject2D = new CMove2D;

	if (pObject2D != nullptr)
	{
		pObject2D->Init(pos, mylib_const::DEFVEC3);

		pObject2D->SetSize(DEF_SIZE);
		pObject2D->SetPosDest(posDest);
		pObject2D->SetSpeed(fSpeed);

		pObject2D->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pObject2D;
}
