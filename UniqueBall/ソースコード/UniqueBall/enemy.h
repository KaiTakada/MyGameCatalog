//===============================================
//
// �G(enemy.h)
// Author ���c ����
//
//===============================================
#ifndef _ENEMY_H_		//���̃}�N����`������ĂȂ�������
#define _ENEMY_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

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
// �v���C���[�N���X
//=========================
class CEnemy : public CObject
{
public:

	//====================
	// ���[�V�����̎��
	//====================
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	//�ҋ@
		MOTIONTYPE_WALK,	//����
		MOTIONTYPE_DASH,	//�_�b�V��
		MOTIONTYPE_JUMP,	//�W�����v
		MOTIONTYPE_JUMP2,	//2�i�W�����v
		MOTIONTYPE_GROUND,	//���n
		MOTIONTYPE_THROW_RDY,	//�����p�\����
		MOTIONTYPE_THROW,	//������
		MOTIONTYPE_CATCH_RDY,	//�L���b�`�p�\����
		MOTIONTYPE_CATCH,	//�L���b�`
		MOTIONTYPE_TACKLE,	//�^�b�N��
		MOTIONTYPE_MAX,
	}MOTIONTYPE;

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

	CEnemy(int nPriority = 3);		//�R���X�g���N�^
	~CEnemy();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//������(�I�o��)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//����

	bool GetJump(void) { return m_bJump; }						//�W�����v����
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//�O��ʒu�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }					//���݈ʒu�擾
	D3DXVECTOR3 GetRot(void) { return m_rot; }					//�����擾
	D3DXVECTOR3 GetMove(void) { return m_move; }				//�ړ��ʎ擾
	D3DXVECTOR3 GetSize(void) { return m_size; }				//�T�C�Y�擾
	float GetHeartPos(void) { return m_fHeart; }				//�S���ʒu�擾
	Param GetParam(void) { return m_param; }					//���݃p�����[�^�擾
	void SetJump(const bool bJump) { m_bJump = bJump; }				//�W�����v����ݒ�
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//�ړ��ʐݒ�
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }				//���݈ʒu�ݒ�
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }				//�����ݒ�
	void SetHeartPos(const float fHeart) { m_fHeart = fHeart; }		//�S���ʒu�ݒ�
	void SetParam(const Param param) { m_param = param; }			//���݃p�����[�^�ݒ�
	void SetMember(const my_Identity::eMember member) { m_eMember = member; }		//�����ݒ�
	my_Identity::eMember GetMember() { return m_eMember; }		//�����ݒ�


	void ReadFile(void);	//�t�@�C���Ǎ�
	void Damage(float fDamege = 1.0f);

	CMotion *GetMotion(void);					//���݃p�����[�^�擾

protected:

private:
	void RotAdj(float fRotDiff);			//�����C��
	void RotLimit(float *pRot);				//�����C��

	void CollisionField(D3DXVECTOR3 pos);
	void CollisionBall(D3DXVECTOR3 pos);

	void Throw();

	CParts *m_apPart[MAX_PARTS];		//���f��(�p�[�c)�ւ̃|�C���^
	int m_nNumModel;							//���f��(�p�[�c)�̑���
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_posOld;		//�O��ʒu
	D3DXVECTOR3 m_pos;			//���݈ʒu
	D3DXVECTOR3 m_size;			//���݃T�C�Y
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�ڕW�̌���
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	float m_fHeart;			//�v���C���[�̒��Sy��(�S�����W)
	bool m_bJump;			//�W�����v����(false�̓W�����v�\���)
	CMotion *m_pMotion;		//���[�V�������
	Param m_param;			//�p�����[�^
	CBall *m_pBall;		//����(�{�[��)���
	CState_Life *m_pStateLife;		//���
	my_Identity::eMember m_eMember;		//����
	int m_nCatchCtr;		//�L���b�`�J�E���^�[
	int m_nThrowCtr;		//�����J�E���^�[
};

#endif