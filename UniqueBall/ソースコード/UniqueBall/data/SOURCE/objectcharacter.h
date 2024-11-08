//===============================================
//
// �L�����N�^�[(objectcharacter.h)
// Author�F ���c ����
//
//===============================================
#ifndef _OBJECTCHARACTER_H_		//���̃}�N����`������ĂȂ�������
#define _OBJECTCHARACTER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "object.h"
#include "identity.h"

//=========================
// �}�N����`
//=========================
#define MAX_PARTS (32)		//�ő�p�[�c��
#define MAX_PLAYER_WEAPON (2)		//���퐔
#define MAX_PLAYER_SIZE D3DXVECTOR3(30.0f,60.0f,30.0f)		//�T�C�Y

//=========================
// �O���錾
//=========================
class CParts;
class CMotion;
class CBall;
class CGauge;
class CCursor;
class CState_Life;

//=========================
// �L�����N�^�[�N���X
//=========================
class CObjectCharacter : public CObject
{
public:

	//====================
	// ���[�V�����̎��
	//====================
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,	//�ҋ@
		MOTIONTYPE_WALK,	//����
		MOTIONTYPE_DASH,	//�_�b�V��
		MOTIONTYPE_JUMP,	//�W�����v
		MOTIONTYPE_JUMP2,	//�W�����v
		MOTIONTYPE_GROUND,	//���n
		MOTIONTYPE_MAX,
	};

	//====================
	// �L�����̃p�����[�^
	//====================
	typedef struct
	{
		float fLife;		//�̗�
		float fLifeMax;		//�ő�̗�
		float fSpeed;		//�ړ����x
		float fAttack;		//�U����
		float fColl;		//�����蔻��̔��a
	}Param;

	CObjectCharacter(int nPriority = 3);		//�R���X�g���N�^
	~CObjectCharacter();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//������(�I�o��)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CObjectCharacter *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//����

	void SetJump(const bool bJump) { m_bJump = bJump; }				//�W�����v����ݒ�
	bool GetJump(void) { return m_bJump; }							//�W�����v���菈��
	void SetJumpSec(const bool bJump) { m_bJumpSec = bJump; }		//2�i�W�����v����ݒ�
	bool GetJumpSec(void) { return m_bJumpSec; }					//2�i�W�����v���菈��
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }				//���݈ʒu�ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }						//���݈ʒu�擾
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }				//�����ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }						//�����擾
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//�ړ��ʐݒ�
	D3DXVECTOR3 GetMove(void) { return m_move; }					//�ړ��ʎ擾
	void SetHeartPos(const float fHeart) { m_fHeart = fHeart; }		//�S���ʒu�ݒ�
	float GetHeartPos(void) { return m_fHeart; }					//�S���ʒu�擾
	void SetParam(const Param param) { m_param = param; }			//���݃p�����[�^�ݒ�
	Param GetParam(void) { return m_param; }						//���݃p�����[�^�擾
	void SetMember(const my_Identity::eMember member) { m_eMember = member; }		//�����ݒ�
	my_Identity::eMember GetMember() { return m_eMember; }							//�����ݒ�
	D3DXVECTOR3 GetPosCol(void) { return D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeart, m_pos.z); }				//���݈ʒu�擾

	void SetMotion(CMotion* pMotion) { m_pMotion = pMotion; }		//���[�V�����ݒ�
	CMotion* GetMotion(void) { return m_pMotion; }						//���[�V�����擾

	void SetDelete(const bool bDelete) { m_bDelete = bDelete; }		//org���S�t���O�ݒ�
	bool GetDelete(void) { return m_bDelete; }					//org���S�t���O�擾

	void SetStateLife(CState_Life* pStateLife) { m_pStateLife = pStateLife; }		//������Ԑݒ�
	CState_Life *GetStateLife(void) { return m_pStateLife; }					//������Ԏ擾

	D3DXVECTOR3 GetSize(void) { return m_size; }					//�T�C�Y�擾
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }				//�O��ʒu�擾

	void ReadFile(void);	//�t�@�C���Ǎ�
	void Damage(float fDamege = 1.0f);
	void InitModel();	//���f��������

protected:
	void CollisionField(D3DXVECTOR3 pos);		//�n�ʔ���
	void RotAdj(float fRotDiff);			//�����C��
	virtual void MotionSet(int type, int brend = 0);

	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_posOld;		//�O��ʒu
	D3DXVECTOR3 m_pos;			//���݈ʒu
	D3DXVECTOR3 m_size;			//���݃T�C�Y
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�ڕW�̌���
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	float m_fHeart;			//�v���C���[�̒��Sy��(�S�����W)
	Param m_param;			//�p�����[�^
	CState_Life *m_pStateLife;		//���
	my_Identity::eMember m_eMember;		//����
	bool m_bDelete;		//���񂾂�Scene�ŎE���p�t���O[t:����]

	CParts* m_apPart[MAX_PARTS];		//���f��(�p�[�c)�ւ̃|�C���^
	int m_nNumModel;							//���f��(�p�[�c)�̑���
	CMotion* m_pMotion;		//���[�V�������

private:
	void RotLimit(float *pRot);				//�����C��

	bool m_bJump;				//�W�����v����(false�̓W�����v�\���)
	bool m_bJumpSec;			//2�i�W�����v����(false�̓W�����v�\���)
};

#endif