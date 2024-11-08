//==================================================
//
// �u���b�N����(block.cpp)
// Author : Kai Takada
//
//==================================================
#include "block.h"
#include "manager.h"

//=======================
// �}�N����`
//=======================
#define PLAYER_EXCESSX (0.0f)						//�v���C���[�̃e�N�X�`���̕��̗]���ȕ�����

//=======================
// �o�C�i���t�@�C����
//=======================
const char *c_apFileBlock[CBlock::TYPE_MAX] =
{
	"data\\MODEL\\BLOCK\\brick.x",		//�ʏ�u���b�N
};

//=======================
// �ÓI�����o�ϐ��錾
//=======================
int CBlock::m_nNumAll = 0;											//���ݎg���Ă���u���b�N��

//=======================
// �R���X�g���N�^
//=======================
CBlock::CBlock(int nPriority) : CObjectX(nPriority)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_NORMAL;
}

//=======================
// �f�X�g���N�^
//=======================
CBlock::~CBlock()
{

}

//=======================
//�u���b�N�̏���������
//=======================
HRESULT CBlock::Init(void)
{
	m_type = TYPE_NORMAL;

	CObjectX::Init();
	
	m_posOld = GetPos();
	
	CXModel *pXmodel = CManager::GetInstance()->GetXModel();

	D3DXVECTOR3 vtxMin = pXmodel->GetAddress(GetIdxModel())->vtxMin;
	D3DXVECTOR3 vtxMax = pXmodel->GetAddress(GetIdxModel())->vtxMax;
	SetVtx(vtxMin, vtxMax);

	SetType(TYPE_BLOCK);

	return S_OK;
}

//=======================
//�u���b�N�̏���������
//=======================
HRESULT CBlock::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE_Blk type)
{
	CObjectX::Init(pos, rot);

	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = type;

	D3DXVECTOR3 vtxMin = DEF_VEC3;
	D3DXVECTOR3 vtxMax = DEF_VEC3;

	CXModel *pXmodel = CManager::GetInstance()->GetXModel();
	SetIdxModel(pXmodel->Regist(c_apFileBlock[type], &vtxMin, &vtxMax));		//���f�����蓖��

	vtxMin = pXmodel->GetAddress(GetIdxModel())->vtxMin;
	vtxMax = pXmodel->GetAddress(GetIdxModel())->vtxMax;
	SetVtx(vtxMin, vtxMax);

	SetType(TYPE_BLOCK);

	return S_OK;
}

//=======================
//�u���b�N�̏I������
//=======================
void CBlock::Uninit(void)
{
	CObjectX::Uninit();
}

//=======================
//�u���b�N�̍X�V����
//=======================
void CBlock::Update(void)
{
	m_posOld = GetPos();

	CollisionRect();

	CObjectX::Update();
}

//=======================
//�u���b�N�̕`�揈��
//=======================
void CBlock::Draw(void)
{
	CObjectX::Draw();
}

//=======================
//�u���b�N�̐ݒ菈��
//=======================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE_Blk type)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CBlock *pBlock = nullptr;

	//�I�u�W�F�N�g����&������
	switch (type)
	{
	case TYPE_NORMAL:
	default:
		pBlock = new CBlock;
		break;
	}


	if (pBlock != nullptr)
	{
		if (FAILED(pBlock->Init(pos, rot, type)))
		{
			return nullptr;
		}
		else
		{
			pBlock->m_posOld = pos;
			pBlock->m_type = type;
		}
	}
	else
	{
		return nullptr;
	}

	return pBlock;
}

//============================
//�u���b�N�̓����蔻��(����ŉ����߂�)
//============================
bool CBlock::CollisionRect(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 sizeMin = GetMinVtx();		//�ŏ��l
	D3DXVECTOR3 sizeMax = GetMaxVtx();		//�ő�l
	D3DXVECTOR3 move = mylib_const::DEFVEC3;

	D3DXVECTOR3 posC = mylib_const::DEFVEC3;
	D3DXVECTOR3 posOldC = mylib_const::DEFVEC3;
	D3DXVECTOR3 sizeC = mylib_const::DEFVEC3;
	D3DXVECTOR3 moveC = mylib_const::DEFVEC3;
	bool bJumpC = false;

	bool bCollision = false;			//�����蔻�肪���������ǂ���

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject* pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_PLAYER)
				{//�v���C���\��������
					posC = pObject->GetPos();
					posOldC = pObject->GetPosOld();
					sizeC = pObject->GetSize();
					moveC = pObject->GetMove();
					bJumpC = pObject->GetJump();

					bCollision = mylib_useful::CollisionBlock(pos, pos, move, sizeMin,
						&posC, posOldC, &moveC, sizeC, &bJumpC);
				
					pObject->SetPos(posC);
					pObject->SetMove(moveC);
					pObject->SetJump(bJumpC);
				}
			}

		}
	}

	return bCollision;
}
