//==================================================
//
// ��(wp_ball.cpp) �p���FCWeapon
// Author : Kai Takada
//
//==================================================
#include "wp_ball.h"
#include "weapon.h"
#include "Xmodel.h"
#include "manager.h"
#include "Field.h"
#include "objectcharacter.h"

#include "sound.h"
#include "particle.h"
#include "obj_polygon3D.h"
#include "Anim.h"
#include "texture.h"

//=======================
// �萔��`
//=======================
namespace
{
	const float GRAVITY = 0.2f;		//�d��
	const float NUM_RESIST(0.02f);		//�ړ���R
	const float STOP_SPEED = 0.4f;		//��~���������x
	const float FALL_HEIGHT = 10000.0f;		//��Η����W��
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
	m_fSpeedMag = 0.0f;
	m_pTarget = nullptr;

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
	m_fSpeedMag = 1.0f;
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
	m_fSpeedMag = 1.0f;
	CWeapon::Init(pos, rot, CWeapon::WPNTYPE_BALL);

	SetType(TYPE_BALL);

	return S_OK;
}

//=======================
// �I������
//=======================
void CBall::Uninit(void)
{
	if (m_pTarget != nullptr)
	{
		m_pTarget = nullptr;
	}

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
	//Target();

	float fGravMul = 1.0f;
	if (pos.y > FALL_HEIGHT)
	{
		fGravMul = 1.3f;
	}

	//�d��
	m_move.y -= GRAVITY * fGravMul;

	//�������擾����
	CollisionField(pos);

	//�ړ��ʂ�K�p
	SetPos(pos += m_move); 

	//�x�^�ł���
	bool bCol = false;
	if (pos.x > mylib_const::DEF_FIELD_SIZE.x - COLLISION_RAD)
	{
		pos.x = mylib_const::DEF_FIELD_SIZE.x - COLLISION_RAD;
		bCol = true;
	}
	else if (pos.x < -mylib_const::DEF_FIELD_SIZE.x + COLLISION_RAD)
	{
		pos.x = -mylib_const::DEF_FIELD_SIZE.x + COLLISION_RAD;
		bCol = true;
	}
	if (pos.z > mylib_const::DEF_FIELD_SIZE.z - COLLISION_RAD)
	{
		pos.z = mylib_const::DEF_FIELD_SIZE.z - COLLISION_RAD;
		bCol = true;
	}
	else if (pos.z < -mylib_const::DEF_FIELD_SIZE.z + COLLISION_RAD)
	{
		pos.z = -mylib_const::DEF_FIELD_SIZE.z + COLLISION_RAD;
		bCol = true;
	}

	if (bCol == true)
	{
		m_move.x = 0.0f;
		m_move.y = -1.0f;
		m_move.z = 0.0f;
	}

	SetPos(pos);

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

	//�T�C�Y
	float fScale = 0.2f;
	fScale = 1.0f + ((m_fSpeedMag - 1.0f) * fScale);
	SetScale(fScale);

	//���ŃJ�E���g�_�E��
	if (m_eMember == my_Identity::MEMBER_NONE)
	{
		if (m_nDeathCtr <= 0)
		{
			SetDeath(true);
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
	//�����������ݒ�
	m_move.x = sinf(vec.y + 1.0f * D3DX_PI) * fPower * m_fSpeedMag;
	m_move.z = cosf(vec.y + 1.0f * D3DX_PI) * fPower * m_fSpeedMag;
	m_move.y = sinf(vec.z + 1.0f * D3DX_PI) * fPower * m_fSpeedMag;
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
	float fColl = m_fRad * GetScale().x + fRad;		//�����蔻��͈�

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

		if (GetParent() == nullptr && m_eMember != my_Identity::MEMBER_MAP)
		{//�e�����遁�������ʒu�Œ�
			CObject3DAnim *pAnim3D = CObject3DAnim::Create(D3DXVECTOR3(pos.x,pos.y + 10.0f,pos.z));
			CTexture* pTexture = CManager::GetInstance()->GetTexture();
			pAnim3D->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\ANIM\\death_explode.png"));
			pAnim3D->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
			pAnim3D->SetTexPos(D3DXVECTOR2(1.0f / 8, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			pAnim3D->GetAnim()->SetNumPtnXY(8, 1);
			pAnim3D->GetAnim()->SetNumLoop(0);
			pAnim3D->GetAnim()->SetNumFrame(5);
			pAnim3D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			pAnim3D->SetLight(true);

			m_eMember = my_Identity::MEMBER_NONE;
		}

		pos.y = fHeight;
		m_move.y *= -0.7f;
		m_fSpeedMag = 1.0f;
	}

	//���W�ݒ�(�X�V)
	SetPos(pos);
}

//============================
// �^�[�Q�b�g���ݎ��z����
//============================
void CBall::Target()
{
	D3DXVECTOR3 pos = mylib_const::DEFVEC3;
	D3DXVECTOR3 Mypos = GetWorldPos();

	if (m_pTarget != nullptr)
	{
		pos = m_pTarget->GetPosCol();

		if (m_pTarget->GetDelete())
		{
			m_pTarget = nullptr;
		}
	}
	else
	{
		return;
	}

	//�^�[�Q�b�g�̕���
	D3DXVECTOR3 move = GetMove();
	move = mylib_useful::Point2Rot(pos, Mypos);
	SetMove(move);

	return;
}
