//===============================================
//
// CPU�G(enemy_cpu.cpp)
// Author ���c ����
//
//===============================================

//============================
// �C���N���[�h�t�@�C��
//============================
#include "enemy_cpu.h"
#include "manager.h"	//�قڊm�ŕK�v
#include "game.h"		//�قڊm�ŕK�v
#include "camera.h"			//�ړ�����
#include "sound.h"			//���o���ɕK�v
#include "parts.h"			//�e�q�֌W
#include "motion.h"			//���[�V����
#include "Field.h"			//�n�ʂƂ̓����蔻��
#include "wp_ball.h"		//�u�[�X�g
#include "Xmodel.h"			//���f��
#include "debugproc.h"		//�f�o�b�O
#include "state_life.h"		//��ԊǗ�
#include "ef_smoke.h"		//�����o
#include "wp_ball.h"		//�{�[���̓����蔻��
#include "player.h"		//�v���C���[
#include "objectcharacter.h"		//���S

//�f�o�b�O�p(�����炭����)
#include "block.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// �萔��`
//============================
namespace
{
	//// �s�����I�̍\����
	//struct sProbability
	//{
	//	CEnemy_CPU::EAction action;	// �s��
	//	float fProbability;			// �m��
	//};

	const float THROW_LENGTH(200.0f);		//�����̔��苗��
	const float CAUTION_LENGTH(300.0f);		//�x���̔��苗��
	//std::vector<sProbability> ACT_PROBABILITY_HAVE =	// �s���̒��I�m��(�{�[���������Ă���Ƃ�)
	//{
	//	{ CEnemy_CPU::ACTION_CHASE, 0.7f },		// �����U��
	//	{ CEnemy_CPU::ACTION_THROW, 0.3f },		// �r�[���U��
	//};
	//std::vector<sProbability> ACT_PROBABILITY_NOT =	// �s���̒��I�m��(�{�[���������Ă��Ȃ��Ƃ�)
	//{
	//	{ CEnemy_CPU::ACTION_PICKUP, 0.7f },
	//	{ CEnemy_CPU::ACTION_CAUTION, 0.3f },
	//};

}

//============================
// �ÓI�����o�ϐ��錾
//============================

//============================
// �R���X�g���N�^
//============================
CEnemy_CPU::CEnemy_CPU(int nPriority) : CEnemy(nPriority)
{
	m_eAction = EAction::ACTION_NONE;
	m_pTarget = nullptr;
}

//============================
// �f�X�g���N�^
//============================
CEnemy_CPU::~CEnemy_CPU()
{

}

//====================================
// �v���C���[�̏�����(�f�t�H)
//====================================
HRESULT CEnemy_CPU::Init(void)
{
	CEnemy::Init();

	m_eAction = EAction::ACTION_NONE;

	return S_OK;
}

//====================================
// �v���C���[�̏�����(�ʒu�I�o��)
//====================================
HRESULT CEnemy_CPU::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nNumParts)
{
	CEnemy::Init(pos, rot, nNumParts);

	return S_OK;
}

//============================
// �v���C���[�̏I��
//============================
void CEnemy_CPU::Uninit(void)
{
	CEnemy::Uninit();
}

//============================
// �v���C���[�̍X�V
//============================
void CEnemy_CPU::Update(void)
{
	CEnemy::Update();
}

//============================
// �v���C���[�̕`��
//============================
void CEnemy_CPU::Draw(void)
{
	CEnemy::Draw();
}

//============================
// �v���C���[�̐���
//============================
CEnemy_CPU *CEnemy_CPU::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy_CPU *pEnemy = nullptr;

	//�I�u�W�F�N�g����&������
	pEnemy = new CEnemy_CPU;

	if (pEnemy != nullptr)
	{
		pEnemy->Init(pos, rot, 1);
	}
	else
	{
		return nullptr;
	}

	return pEnemy;
}

//====================================
// �A�N�V�����I��
//====================================
void CEnemy_CPU::ActionSelect()
{
	if (m_eAction == EAction::ACTION_NONE)
	{
		CBall *pBall = CEnemy::GetBall();

		//�G��T��
		SerchTarget();
		D3DXVECTOR3 pos = GetPos();
		float fLength = mylib_useful::Point2Angle(pos, m_pTarget->GetPos());

		//�{�[���������Ă��邩
		if (pBall != nullptr)
		{
			if (fLength < THROW_LENGTH)
			{//�G���߂������瓊����
			
			}
			else
			{//�ǂ�������

			}
		}
		else
		{
			
			if (fLength > CAUTION_LENGTH)
			{//�{�[�����E���ɍs��

			}
			else
			{//�L���b�`���悤�ƌ�ނ��Ȃ���x������

			}
			

		}
	}

	m_eAction = EAction::ACTION_NONE;

}

//============================
// �G��T������(xz����)
//============================
void CEnemy_CPU::SerchTarget()
{
	my_Identity::eMember myMember = GetMember();
	D3DXVECTOR3 myPos = GetPos();
	float fLength = 0.0f;
	bool bFirst = false;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY)
				{

				 //�L���X�g
					CPlayer * pChara = static_cast<CPlayer*>(pObject);

					if (pChara != nullptr)
					{
						my_Identity::eMember member = pChara->GetMember();

						if (member != myMember)	// ���R
						{
							D3DXVECTOR3 pos = pChara->GetPos();

							D3DXVECTOR3 vec = mylib_useful::Point2Rot(myPos, pos);
							float fLengthNow = mylib_useful::Vec2Length(vec.x, vec.z);
							if (!bFirst)
							{
								bFirst = !bFirst;
								fLength = fLengthNow;
							}
							else if (fLength > fLengthNow)
							{
								fLength = fLengthNow;
								m_pTarget = static_cast<CObjectCharacter*>(pChara);
							}
						}
					}
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == nullptr) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}
}