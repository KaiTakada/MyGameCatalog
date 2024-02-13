//==================================================
//
// �u���b�N����(block.cpp)
// Author : Kai Takada
//
//==================================================
#include "block.h"
#include "manager.h"

#include "ball_ganarator.h"
#include "wp_ball.h"

//=======================
// �萔��`
//=======================
namespace
{
	const char *FILEPASS = "data\\MODEL\\BLOCK\\ball_generator.x";
	const int GENERATECNT = 300;
}

//=======================
// �R���X�g���N�^
//=======================
CBallGenerator::CBallGenerator(int nPriority) : CObjectX(nPriority)
{
	m_nGenerateCtr = 0;
	m_pBall = nullptr;
}

//=======================
// �f�X�g���N�^
//=======================
CBallGenerator::~CBallGenerator()
{

}

//=======================
//�u���b�N�̏���������
//=======================
HRESULT CBallGenerator::Init(void)
{
	CObjectX::Init();
	m_pBall = nullptr;

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
HRESULT CBallGenerator::Init(const D3DXVECTOR3 pos)
{
	CObjectX::Init(pos, mylib_const::DEFVEC3);
	m_pBall = nullptr;

	D3DXVECTOR3 vtxMin = mylib_const::DEFVEC3;
	D3DXVECTOR3 vtxMax = mylib_const::DEFVEC3;

	CXModel *pXmodel = CManager::GetInstance()->GetXModel();
	SetIdxModel(pXmodel->Regist(FILEPASS, &vtxMin, &vtxMax));		//���f�����蓖��

	vtxMin = pXmodel->GetAddress(GetIdxModel())->vtxMin;
	vtxMax = pXmodel->GetAddress(GetIdxModel())->vtxMax;
	SetVtx(vtxMin, vtxMax);

	SetType(TYPE_BALL_GENERATOR);

	return S_OK;
}

//=======================
//�u���b�N�̏I������
//=======================
void CBallGenerator::Uninit(void)
{
	CObjectX::Uninit();
}

//=======================
//�u���b�N�̍X�V����
//=======================
void CBallGenerator::Update(void)
{
	if (mylib_const::MAX_PLAYER * 2 > CBall::GetNumAll() &&
		m_pBall == nullptr)
	{
		if (m_nGenerateCtr >= GENERATECNT)
		{
			//����
			CBall *pBall = CBall::Create(GetPos());
			m_pBall = pBall;
		}
		else
		{
			m_nGenerateCtr++;
		}
	}
	else
	{
		m_nGenerateCtr = 0;
	}

	if (m_pBall != nullptr)
	{
		if (m_pBall->GetMember() != my_Identity::MEMBER_NONE)
		{//���̏����ɂȂ���������
			m_pBall = nullptr;
		}
		else
		{//
			D3DXVECTOR3 pos = GetPos();
			pos.y += m_pBall->GetMaxVtx().y;
			m_pBall->SetPos(pos);
			m_pBall->SetDeathCtr(100);
		}
	}

	//CollisionRect();

	CObjectX::Update();
}

//=======================
//�u���b�N�̕`�揈��
//=======================
void CBallGenerator::Draw(void)
{
	CObjectX::Draw();
}

//=======================
//�u���b�N�̐ݒ菈��
//=======================
CBallGenerator *CBallGenerator::Create(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CBallGenerator *pBlock = nullptr;

	pBlock = new CBallGenerator;

	if (pBlock != nullptr)
	{
		if (FAILED(pBlock->Init(pos)))
		{
			return nullptr;
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
bool CBallGenerator::CollisionRect(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 sizeMin = GetMinVtx();		//�ŏ��l
	D3DXVECTOR3 sizeMax = GetMaxVtx();		//�ő�l

	D3DXVECTOR3 posC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 posOldC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 sizeC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 moveC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����߂���

	bool bLand = false;			//���n�������ǂ���
	bool bCollision = false;			//�����蔻�肪���������ǂ���

	//x
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

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

					D3DXVECTOR3 sizeOldMinC = D3DXVECTOR3(posOldC.x - sizeC.x, posOldC.y, posOldC.z - sizeC.z);		//�L�����ŏ��l
					D3DXVECTOR3 sizeOldMaxC = D3DXVECTOR3(posOldC.x + sizeC.x, posOldC.y + (sizeC.y * 2), posOldC.z + sizeC.z);		//�L�����ő�l

					D3DXVECTOR3 sizeMinC = D3DXVECTOR3(posC.x - sizeC.x, posC.y, posC.z - sizeC.z);		//�L�����ŏ��l
					D3DXVECTOR3 sizeMaxC = D3DXVECTOR3(posC.x + sizeC.x, posC.y + (sizeC.y * 2), posC.z + sizeC.z);		//�L�����ő�l

					if (pObject->GetJump() == false)
					{
						if (sizeOldMaxC.x <= pos.x + sizeMin.x
							&& sizeMaxC.x > pos.x + sizeMin.x
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//�u���b�N��
							move.x = (pos.x + sizeMin.x) - (sizeMaxC.x) - 0.1f;
							bCollision = true;
						}
						else if (sizeOldMinC.x >= pos.x + sizeMax.x
							&& sizeMinC.x <= pos.x + sizeMax.x
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//�u���b�N��
							move.x = (pos.x + sizeMax.x) - (sizeMinC.x) + 0.1f;
							bCollision = true;
						}
					}
					else
					{
						if (sizeOldMaxC.x <= pos.x + sizeMin.x
							&& sizeMaxC.x > pos.x + sizeMin.x
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//�u���b�N��
							move.x = (pos.x + sizeMin.x) - (sizeMaxC.x) - 0.1f;
							bCollision = true;
						}
						else if (sizeOldMinC.x >= pos.x + sizeMax.x
							&& sizeMinC.x <= pos.x + sizeMax.x
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//�u���b�N��
							move.x = (pos.x + sizeMax.x) - (sizeMinC.x) + 0.1f;
							bCollision = true;
						}
					}
				}

				if (bCollision)
				{
					pObject->SetPos(posC + move);
					bCollision = false;
					break;
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	//y
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

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

					D3DXVECTOR3 sizeOldMinC = D3DXVECTOR3(posOldC.x - sizeC.x, posOldC.y, posOldC.z - sizeC.z);		//�L�����ŏ��l
					D3DXVECTOR3 sizeOldMaxC = D3DXVECTOR3(posOldC.x + sizeC.x, posOldC.y + (sizeC.y * 2), posOldC.z + sizeC.z);		//�L�����ő�l

					D3DXVECTOR3 sizeMinC = D3DXVECTOR3(posC.x - sizeC.x, posC.y, posC.z - sizeC.z);		//�L�����ŏ��l
					D3DXVECTOR3 sizeMaxC = D3DXVECTOR3(posC.x + sizeC.x, posC.y + (sizeC.y * 2), posC.z + sizeC.z);		//�L�����ő�l

					if (pObject->GetJump() == false)
					{
						if (sizeOldMinC.y >= pos.y + sizeMax.y
							&& sizeMinC.y <= pos.y + sizeMax.y
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f)
						{//�u���b�N��
							D3DXVECTOR3 Objmove = pObject->GetMove();
							pObject->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

							move.y = (pos.y + sizeMax.y) - (sizeMinC.y) + 0.1f;
							pObject->SetJump(false);
							pObject->SetBoost(false);
							bCollision = true;
							bLand = true;
						}
						else if (sizeOldMaxC.y <= pos.y + sizeMin.y
							&& sizeMaxC.y >= pos.y + sizeMin.y
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f)
						{//�u���b�N��
							D3DXVECTOR3 Objmove = pObject->GetMove();
							pObject->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

							move.y = (pos.y + sizeMin.y) - (sizeMaxC.y) - 0.1f;
							bCollision = true;
						}
					}
					else
					{
						if (sizeOldMinC.y >= pos.y + sizeMax.y
							&& sizeMinC.y <= pos.y + sizeMax.y
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f)
						{//�u���b�N��
							D3DXVECTOR3 Objmove = pObject->GetMove();
							pObject->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

							move.y = (pos.y + sizeMax.y) - (sizeMinC.y) + 0.1f;
							pObject->SetJump(false);
							pObject->SetBoost(false);
							bCollision = true;
							bLand = true;
						}
						else if (sizeOldMaxC.y <= pos.y + sizeMin.y
							&& sizeMaxC.y >= pos.y + sizeMin.y
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f)
						{//�u���b�N��
							D3DXVECTOR3 Objmove = pObject->GetMove();
							pObject->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

							move.y = (pos.y + sizeMin.y) - (sizeMaxC.y) - 0.1f;
							bCollision = true;
						}
					}
				}

				if (bCollision)
				{
					pObject->SetPos(posC + move);
					bCollision = false;
					break;
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	//z
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

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

					D3DXVECTOR3 sizeOldMinC = D3DXVECTOR3(posOldC.x - sizeC.x, posOldC.y, posOldC.z - sizeC.z);		//�L�����ŏ��l
					D3DXVECTOR3 sizeOldMaxC = D3DXVECTOR3(posOldC.x + sizeC.x, posOldC.y + (sizeC.y * 2), posOldC.z + sizeC.z);		//�L�����ő�l

					D3DXVECTOR3 sizeMinC = D3DXVECTOR3(posC.x - sizeC.x, posC.y, posC.z - sizeC.z);		//�L�����ŏ��l
					D3DXVECTOR3 sizeMaxC = D3DXVECTOR3(posC.x + sizeC.x, posC.y + (sizeC.y * 2), posC.z + sizeC.z);		//�L�����ő�l

					if (pObject->GetJump() == false)
					{
						if (sizeOldMaxC.z <= pos.z + sizeMin.z
							&& sizeMaxC.z > pos.z + sizeMin.z
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//�u���b�N�k
							move.z = (pos.z + sizeMin.z) - (sizeMaxC.z) - 0.1f;
							bCollision = true;
						}
						else if (sizeOldMinC.z >= pos.z + sizeMax.z
							&& sizeMinC.z <= pos.z + sizeMax.z
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//�u���b�N��
							move.z = (pos.z + sizeMax.z) - (sizeMinC.z) + 0.1f;
							bCollision = true;
						}
					}
					else
					{
						if (sizeOldMaxC.z <= pos.z + sizeMin.z
							&& sizeMaxC.z > pos.z + sizeMin.z
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//�u���b�N�k
							move.z = (pos.z + sizeMin.z) - (sizeMaxC.z) - 0.1f;
							bCollision = true;
						}
						else if (sizeOldMinC.z >= pos.z + sizeMax.z
							&& sizeMinC.z <= pos.z + sizeMax.z
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//�u���b�N��
							move.z = (pos.z + sizeMax.z) - (sizeMinC.z) + 0.1f;
							bCollision = true;
						}
					}
				}

				if (bCollision)
				{
					pObject->SetPos(posC + move);
					bCollision = false;
					break;
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	return bLand;
}
