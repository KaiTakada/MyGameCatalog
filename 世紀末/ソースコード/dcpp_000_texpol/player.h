//===============================================
//
// �v���C���[(player.h)
// Author ���c ����
//
//===============================================
#ifndef _PLAYER_H_		//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
//#include "objectX.h"
#include "object.h"

//=========================
// �}�N����`
//=========================
#define MAX_PLAYER_PARTS (32)		//�ő�p�[�c��
#define MAX_PLAYER_SIZE D3DXVECTOR3(30.0f,50.0f,30.0f)		//�T�C�Y

//=========================
// �O���錾
//=========================
class CParts;
class CMotion;
class CWeapon;
class CGauge;
class CCursor;
class CState_Life;

//=========================
// �v���C���[�N���X
//=========================
class CPlayer : public CObject
{
public:

	//====================
	// ���[�V�����̎��
	//====================
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	//�ҋ@
		MOTIONTYPE_WALK,	//�_�b�V��
		MOTIONTYPE_DASH,	//����
		MOTIONTYPE_JUMP,	//�W�����v
		MOTIONTYPE_GROUND,	//���n
		MOTIONTYPE_FIRE_REV,	//����(REVOLVER)
		MOTIONTYPE_FIRE_AK,	//����(AK)
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
		float fSpeedFire;	//�A�ˑ��x
		float fAttack;		//�U����
		float fGetArea;		//�����񂹔���̔��a
		float fColl;		//�����蔻��̔��a
	}Param;

	CPlayer(int nPriority = 3);		//�R���X�g���N�^
	~CPlayer();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//������(�I�o��)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), int nNumParts = 1);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//����
	
	bool GetJump(void) { return m_bJump; }						//�W�����v����
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//�O��ʒu�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }					//���݈ʒu�擾
	D3DXVECTOR3 GetRot(void) { return m_rot; }					//�����擾
	D3DXVECTOR3 GetMove(void) { return m_move; }				//�ړ��ʎ擾
	D3DXVECTOR3 GetSize(void) { return MAX_PLAYER_SIZE; }				//�T�C�Y�擾
	float GetHeartPos(void) { return m_fHeart; }				//�S���ʒu�擾
	float GetExp(void) { return m_fExp; }						//���݌o���l�擾
	Param GetParam(void) { return m_param; }					//���݃p�����[�^�擾
	void SetJump(const bool bJump) { m_bJump = bJump; }				//�W�����v����ݒ�
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//�ړ��ʐݒ�
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }				//���݈ʒu�ݒ�
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }				//�����ݒ�
	void SetHeartPos(const float fHeart) { m_fHeart = fHeart; }		//�S���ʒu�ݒ�
	void SetExp(const float fExp);				//���݌o���l�ݒ�
	void CntExp(const float fExp);				//���݌o���l���Z
	void SetParam(const Param param) { m_param = param; }			//���݃p�����[�^�ݒ�
	void ReadFile(void);	//�t�@�C���Ǎ�
	void Damage(float fDamege = 1.0f);

protected:

private:
	void MoveOperate(float *pfRotDest);		//�ړ�����
	void RotOperate(float *pfRotDest);		//��������
	void RotAdj(float fRotDiff);			//�����C��
	void PullItem(void);					//�A�C�e��������
	void RotLimit(float *pRot);				//�����C��

	void DebugKey(CInputKeyboard *pInputKeyboard);		//�f�o�b�O�L�[
	void CollisionField(D3DXVECTOR3 pos);

	CParts *m_apPart[MAX_PLAYER_PARTS];		//���f��(�p�[�c)�ւ̃|�C���^
	int m_nNumModel;							//���f��(�p�[�c)�̑���
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_posOld;		//�O��ʒu
	D3DXVECTOR3 m_pos;			//���݈ʒu
	D3DXVECTOR3 m_rot;			//����
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	float m_fHeart;			//�v���C���[�̒��Sy��(�S�����W)
	bool m_bJump;			//�W�����v����(false�̓W�����v�\���)
	CMotion *m_pMotion;		//���[�V�������
	Param m_param;			//�p�����[�^
	CWeapon *m_pWeapon;		//������
	CGauge *m_pGaugeHP;		//HP���
	CGauge *m_pGaugeExp;	//exp���
	CCursor *m_pCursor;		//Aim�p�J�[�\��
	float m_fExp;			//���݌o���l
	float m_fExpMax;		//�o���l�Q�[�WMax�l
	CState_Life *m_pStateLife;		//���
};

#endif
