//==================================================
//
// �u�[�X�^�[(wp_ball.cpp) �p���FCWeapon
// Author : Kai Takada
//
//==================================================
#include "wp_ball.h"
#include "weapon.h"
#include "Xmodel.h"
#include "manager.h"
#include "bullet.h"
#include "Field.h"

#include "sound.h"
#include "particle.h"

//=======================
// �萔��`
//=======================
namespace
{
	const float GRAVITY = 0.5f;		//�d��
	const float NUM_RESIST(0.02f);		//�ړ���R
	const float STOP_SPEED = 0.4f;		//��~���������x
	const float COLLISION_RAD = 50.0f;		//�����蔼�a
	const int DEATHCTR = 300;		//���ŗP�\����
}

//=======================
// �ÓI�����o�ϐ��錾
//=======================
int CBall::s_nNumAll = 0;

//=======================
// �R���X�g���N�^
//=======================
CBall::CBall(int nPriority) : CWeapon(nPriority)
{
	m_fRad = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SetMultiply(true);		//�|�����킹�t���O�ݒ�
	m_eMember = my_Identity::MEMBER_NONE;
	m_bLand = false;
	m_nDeathCtr = 0;

	s_nNumAll++;
}

//=======================
// �f�X�g���N�^
//=======================
CBall::~CBall()
{
	s_nNumAll--;
}

//=======================
// ����������
//=======================
HRESULT CBall::Init(void)
{
	m_fRad = COLLISION_RAD;
	m_nDeathCtr = DEATHCTR;
	CWeapon::Init();
	
	SetType(TYPE_BALL);

	return S_OK;
}

//=======================
// ����������
//=======================
HRESULT CBall::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_fRad = COLLISION_RAD;
	m_nDeathCtr = DEATHCTR;
	CWeapon::Init(pos, rot, CWeapon::WPNTYPE_BALL);

	SetType(TYPE_BALL);

	return S_OK;
}

//=======================
// �I������
//=======================
void CBall::Uninit(void)
{
	CWeapon::Uninit();
}

//=======================
// �X�V����
//=======================
void CBall::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	m_posOld = pos;

	CWeapon::Update();

	CParts *pParts = GetParent();

	if (pParts != nullptr)
	{//�e�����遁�������ʒu�Œ�
		return;
	}

	//@�ړ�-------------

	//�d��
	m_move.y -= GRAVITY;

	//�������擾����
	CollisionField(pos);

	//�ړ��ʂ�K�p
	SetPos(pos += m_move); 

	//�ړ��ʂ��X�V(����������)����
	m_move.x += (0 - m_move.x) * NUM_RESIST;
	m_move.z += (0 - m_move.z) * NUM_RESIST;

	if (m_move.x <= STOP_SPEED && m_move.x >= -STOP_SPEED)
	{//�K��l�ȉ��ňړ��ʂ�0��
		m_move.x = 0.0f;
	}
	if (m_move.z <= STOP_SPEED && m_move.z >= -STOP_SPEED)
	{//�K��l�ȉ��ňړ��ʂ�0��
		m_move.z = 0.0f;
	}

	//���ŃJ�E���g�_�E��
	if (m_eMember == my_Identity::MEMBER_NONE)
	{
		if (m_nDeathCtr <= 0)
		{
			Uninit();
			return;
		}

		m_nDeathCtr--;
	}
}

//=======================
// �`�揈��
//=======================
void CBall::Draw(void)
{
	CWeapon::Draw();
}

//=======================
// �U������
//=======================
void CBall::Attack(const D3DXVECTOR3 rot, const int nDamage)
{
	//�����������ݒ�
	SetRot(rot);
}

//=======================
// �U������
//=======================
void CBall::Throw(const D3DXVECTOR3 vec, float fPower)
{
	//m_move.x += sinf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//x

	//�����������ݒ�
	m_move.x = sinf(vec.y + 1.0f * D3DX_PI) * fPower;
	m_move.z = cosf(vec.y + 1.0f * D3DX_PI) * fPower;
	m_move.y = sinf(vec.x + 1.0f * D3DX_PI) * fPower;

}

//=======================
//@brief �v���C���[�Ƃ̓����蔻��
// �����F
// (pos)����Ώۂ̍��W
// (fRad)����Ώۂ̔��a
// �Ԃ�l�F�����������ǂ���(true:��������,false:�������ĂȂ�)
//=======================
bool CBall::CollisionCircle(const D3DXVECTOR3 pos, const float fRad)
{
	bool bCol = false;
	D3DXVECTOR3 myPos = GetPos();

	//3���g�������̔���
	float fColl = m_fRad + fRad;		//�����蔻��͈�

	float fLength0 = hypotf((myPos.x - pos.x), (myPos.z - pos.z));		//2�_�Ԃ̒���xz
	float fLength1 = hypotf((myPos.x - pos.x), (myPos.y - pos.y));		//2�_�Ԃ̒���xy
	float fLength2 = hypotf((myPos.z - pos.z), (myPos.y - pos.y));		//2�_�Ԃ̒���zy

	if (fLength0 <= fColl &&
		fLength1 <= fColl &&
		fLength2 <= fColl)
	{//�������Ă�����
		bCol = true;
	}

	return bCol;
}

//=======================
//@brief �ǂƂ̓����蔻��
// �����F
// (pos)����Ώۂ̍��W
// (pVtx)����Ώۂ̒��_�|�C���^
// �Ԃ�l�F�����������ǂ���(true:��������,false:�������ĂȂ�)
//=======================
bool CBall::CollisionWall(const D3DXVECTOR3 pos, const D3DXVECTOR3* pVtx)
{
	//pos�ƊeVtx���|�����킹�āA�����Ɠ������Ă��邩

	return false;
}

//=======================
// �����ݒ�
//=======================
void CBall::SetMember(const my_Identity::eMember member)
{
	if (m_eMember == my_Identity::MEMBER_NONE &&
		member != my_Identity::MEMBER_NONE)
	{
		m_nDeathCtr = DEATHCTR;
	}

	m_eMember = member;
}

//=======================
// ��������
//=======================
CBall *CBall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	CBall *pWeapon = nullptr;

	//�I�u�W�F�N�g����&������
	pWeapon = new CBall;

	if (pWeapon != nullptr)
	{
		if (FAILED(pWeapon->Init(pos, rot)))
		{
			return nullptr;
		}
		else
		{
			//���f���̊��蓖�Ă͐e�ňꊇ�ōs��
		}
	}
	else
	{
		return nullptr;
	}

	return pWeapon;
}

//============================
//�u���b�N�Ƃ̓����蔻��(��`)
//============================
void CBall::CollisionField(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;
	pos.y -= m_fRad;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_FIELD)
				{//�A�C�e����������

					CField *pField = (CField*)pObject;
					fHeight = pField->GetHeight(pos);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	if (pos.y < fHeight && m_posOld.y >= fHeight)
	{//���n��

		pos.y = fHeight;
		m_move.y *= -1.0f;
		m_eMember = my_Identity::MEMBER_NONE;
	}

	//���W�ݒ�(�X�V)
	SetPos(pos);
}
