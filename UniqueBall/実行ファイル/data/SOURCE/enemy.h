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
class CCursor;
class CState_Life;

//=========================
// �G�N���X
//=========================
class CEnemy : public CObjectCharacter
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
		MOTIONTYPE_JUMP2,	//2�i�W�����v
		MOTIONTYPE_GROUND,	//���n
		MOTIONTYPE_THROW_RDY,	//�����p�\����
		MOTIONTYPE_THROW,	//������
		MOTIONTYPE_CATCH_RDY,	//�L���b�`�p�\����
		MOTIONTYPE_CATCH,	//�L���b�`
		MOTIONTYPE_TACKLE,	//�^�b�N��
		MOTIONTYPE_MAX,
	};

	CEnemy(int nPriority = 3);		//�R���X�g���N�^
	~CEnemy();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//������(�I�o��)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//����

	void SetDelete(const bool bDelete) { m_bDelete = bDelete; }		//org���S�t���O�ݒ�
	bool GetDelete(void) { return m_bDelete; }						//org���S�t���O�擾

	void SetBall(CBall *pBall) { m_pBall = pBall; }		//�{�[���ݒ�
	CBall *GetBall(void) { return m_pBall; }			//�{�[���擾

	void ReadFile(void);	//�t�@�C���Ǎ�
	void InitModel();	//���f�������ݒ�

protected:

private:
	void CollisionBall(D3DXVECTOR3 pos);

	void Throw();

	CBall *m_pBall;		//����(�{�[��)���
	int m_nCatchCtr;		//�L���b�`�J�E���^�[
	int m_nThrowCtr;		//�����J�E���^�[
	bool m_bDelete;		//���񂾂�Scene�ŎE���p�t���O[t:����]
};

#endif