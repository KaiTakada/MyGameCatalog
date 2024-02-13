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
	
	SetType(TYPE_BLOCK);

	return S_OK;
}

//=======================
//�u���b�N�̏���������
//=======================
HRESULT CBlock::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_NORMAL;

	CObjectX::Init(pos, rot);

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
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CBlock *pBlock = nullptr;

	//�I�u�W�F�N�g����&������
	pBlock = new CBlock;

	if (pBlock != nullptr)
	{
		if (FAILED(pBlock->Init(pos, rot)))
		{
			return nullptr;
		}
		else
		{
			pBlock->m_posOld = pos;

			CXModel *pXmodel = CManager::GetXModel();
			pBlock->SetIdxModel(pXmodel->Regist(c_apFileBlock[type], &vtxMin, &vtxMax));		//���f�����蓖��
		
			pBlock->SetVtx(vtxMin, vtxMax);
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
	D3DXVECTOR3 sizeMin, sizeMax;
	sizeMin = GetMinVtx();
	sizeMax = GetMaxVtx();

	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjposOld = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Objmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	bool bLand = false;			//���n�������ǂ���
	bool Collision = false;			//�����蔻�肪���������ǂ���

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			bLand = false;
			Collision = false;

			CObject *pObj;

			//�I�u�W�F�N�g���擾
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULL�`�F�b�N
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_PLAYER)
				{//�v���C���\��������
					Objpos = pObj->GetPos();
					ObjposOld = pObj->GetPosOld();
					//Objsize = pObj->GetSize();
					Objsize = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
					Objmove = pObj->GetMove();

					if (pObj->GetJump() == false)
					{
						if (ObjposOld.z + Objsize.z <= m_posOld.z - sizeMin.z
							&& Objpos.z + Objsize.z >= pos.z - sizeMin.z
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x + sizeMin.x + 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x - sizeMax.x - 0.1f)
						{//�u���b�N��
							//bLand = true;
							Objpos.z = pos.z - sizeMin.z - Objsize.z - 0.1f;		//�u���b�N�̏�ɗ�������
							pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));			//�ړ��ʂ�0��
							Collision = true;
						}
						else if (ObjposOld.z - Objsize.z >= m_posOld.z + sizeMax.z
							&& Objpos.z - Objsize.z <= pos.z + sizeMax.z
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f)
						{//�u���b�N��
							Objpos.z = pos.z + sizeMax.z + Objsize.z;		//�u���b�N�̉��ɗ�������
							pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));			//�ړ��ʂ�0��
							Collision = true;
						}

						if (ObjposOld.x + Objsize.x - PLAYER_EXCESSX <= m_posOld.x - sizeMin.x + 0.1f
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.z + Objsize.z > pos.z - sizeMin.z
							&& Objpos.z - Objsize.z < pos.z + sizeMax.z)
						{//�u���b�N��
							Objpos.x = pos.x - sizeMin.z - Objsize.x + PLAYER_EXCESSX;			//�u���b�N�̍��ɗ�������
							pObj->SetMove(D3DXVECTOR3(0.0f, Objmove.z, Objmove.z));			//�ړ��ʂ�0��
							Collision = true;
						}
						else if (ObjposOld.x - Objsize.x + PLAYER_EXCESSX >= m_posOld.x + sizeMax.x - 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f
							&& Objpos.z + Objsize.z > pos.z - sizeMin.z
							&& Objpos.z - Objsize.z < pos.z + sizeMax.z)
						{//�u���b�N�E
							Objpos.x = pos.x + sizeMax.x + Objsize.x - PLAYER_EXCESSX;		//�u���b�N�̉E�ɗ�������
							pObj->SetMove(D3DXVECTOR3(0.0f, Objmove.z, Objmove.z));			//�ړ��ʂ�0��
							Collision = true;
						}
					}
					else
					{
						if (ObjposOld.x + Objsize.x - PLAYER_EXCESSX <= m_posOld.x - sizeMin.x + 0.1f
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.z + Objsize.z > pos.z - sizeMin.z
							&& Objpos.z - Objsize.z < pos.z + sizeMax.z)
						{//�u���b�N��
							Objpos.x = pos.x - sizeMin.z - Objsize.x + PLAYER_EXCESSX;			//�u���b�N�̍��ɗ�������
							pObj->SetMove(D3DXVECTOR3(0.0f, Objmove.z, Objmove.z));			//�ړ��ʂ�0��
							Collision = true;
						}
						else if (ObjposOld.x - Objsize.x + PLAYER_EXCESSX >= m_posOld.x + sizeMax.x - 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f
							&& Objpos.z + Objsize.z > pos.z - sizeMin.z
							&& Objpos.z - Objsize.z < pos.z + sizeMax.z)
						{//�u���b�N�E
							Objpos.x = pos.x + sizeMax.x + Objsize.x - PLAYER_EXCESSX;		//�u���b�N�̉E�ɗ�������
							pObj->SetMove(D3DXVECTOR3(0.0f, Objmove.z, Objmove.z));			//�ړ��ʂ�0��
							Collision = true;
						}

						if (ObjposOld.z + Objsize.z <= m_posOld.z - sizeMin.z
							&& Objpos.z + Objsize.z >= pos.z - sizeMin.z
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f)
						{//�u���b�N��
							//bLand = true;
							Objpos.z = pos.z - sizeMin.z - Objsize.z - 0.1f;		//�u���b�N�̏�ɗ�������
							pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));			//�ړ��ʂ�0��
							Collision = true;
						}
						else if (ObjposOld.z - Objsize.z >= m_posOld.z + sizeMax.z
							&& Objpos.z - Objsize.z <= pos.z + sizeMax.z
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f)
						{//�u���b�N��
							Objpos.z = pos.z + sizeMax.z + Objsize.z;		//�u���b�N�̉��ɗ�������
							pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));			//�ړ��ʂ�0��
							Collision = true;
						}
					}
				}

				if (Collision)
				{
					pObj->SetPos(Objpos);
				}

				if (bLand)
				{
					pObj->SetJump(!bLand);
				}
			}
		}
	}

	return bLand;
}
