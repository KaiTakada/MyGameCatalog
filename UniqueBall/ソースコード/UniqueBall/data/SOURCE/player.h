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
#include "objectcharacter.h"
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
class CState_Life;
class CUI_Life;
class CTarget_Mark;

//=========================
// �v���C���[�N���X
//=========================
class CPlayer : public CObjectCharacter
{
public:

	//====================
	// ���[�V�����̎��
	//====================
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		//�ҋ@
		MOTIONTYPE_WALK,			//����
		MOTIONTYPE_DASH,			//�_�b�V��
		MOTIONTYPE_JUMP,			//�W�����v
		MOTIONTYPE_JUMP2,			//2�i�W�����v
		MOTIONTYPE_GROUND,			//���n
		MOTIONTYPE_THROW_RDY,		//�����p�\����
		MOTIONTYPE_THROW,			//������
		MOTIONTYPE_CATCH_RDY,		//�L���b�`�p�\����
		MOTIONTYPE_CATCH,			//�L���b�`
		MOTIONTYPE_TACKLE,			//�^�b�N��
		MOTIONTYPE_DAMAGE,			//�_���[�W
		MOTIONTYPE_NEUTRAL_BALL,	//�ҋ@(������)
		MOTIONTYPE_DASH_BALL,		//�_�b�V��(������)
		MOTIONTYPE_JUMP_BALL,		//�W�����v(������)
		MOTIONTYPE_MAX,
	};

	//====================
	// �{�^���Ǘ�
	//====================
	enum CONTROL_MANAGE
	{
		CONTROL_FRONT= 0,
		CONTROL_BACK,
		CONTROL_RIGHT,
		CONTROL_LEFT,
		CONTROL_JUMP,
		CONTROL_TACKLE,
		CONTROL_THROW,
		CONTROL_THROW_RELEASE,
		CONTROL_CATCH,
		CONTROL_TARGET,
		CONTROL_MAX,
	};

	CPlayer(int nPriority = 3);		//�R���X�g���N�^
	~CPlayer();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//������(�I�o��)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//����
	
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//�O��ʒu�擾
	D3DXVECTOR3 GetSize(void) { return m_size; }				//�T�C�Y�擾
	CMotion* GetMotion(void);					//���[�V�������擾
	
	static int GetControlPlayer() { return m_nControlIdx; }					//����v���C���[

	void SetIdx(int nIdx) { m_nIdx = nIdx; }		//�ԍ��ݒ�
	int GetIdx(void) { return m_nIdx; }				//�ԍ��擾
	void SetUI_Life(CUI_Life* pLifeUI) { m_pLifeUI = pLifeUI; }		//�̗�UI�ݒ�
	CUI_Life *GetUI_Life(void) { return m_pLifeUI; }				//�̗�UI�擾
	
	void ReadFile(void);	//�t�@�C���Ǎ�
	void InitUI();	//UI������
	void InitModel();	//���f��������
	void Damage(int nDamege = 1);
	void Knockback(float fRot);		//�m�b�N�o�b�N

protected:
	void MotionSet(int type, int brend = 0) override;

private:
	void MoveOperate(float *pfRotDest);		//�ړ�����
	void RotOperate(float *pfRotDest);		//��������

	void CollisionWall();
	void CollisionBall(D3DXVECTOR3 pos);
	void TackleCollision();
	void MotionEffect();
	void ReduceCounter();
	void ButtonAction();	//�{�^���A�N�V����
	void Debug();		//�f�o�b�O�L�[
	void RankOK();
	bool ControllManage(CONTROL_MANAGE key);

	void Throw();	//�{�[������
	void Target();	//�^�[�Q�e�B���O
	void SetNeutral();	//�j���[�g������Ԃɂ���

	CBall *m_pBall;		//����(�{�[��)���
	CUI_Life *m_pLifeUI;		//�̗�UI
	int m_nCatchCtr;		//�L���b�`�J�E���^�[
	int m_nTackleCtr;		//�^�b�N���J�E���^�[
	float m_fThrowChargeCtr;		//���ߓ����J�E���^�[
	int m_nJump2Ctr;		//2�i�W�����v�J�E���^�[
	int m_nIdx;		//���g�̔ԍ�
	bool m_bTarget;		//�^�[�Q�b�g�t���O[t:����]
	CTarget_Mark *m_pTargetMark;		//�^�[�Q�b�g�}�[�N

	static int m_nControlIdx;		//�L�[�{�[�h�ő��삷��v���C���[
};

#endif
