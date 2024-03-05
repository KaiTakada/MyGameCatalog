//==================================================
//
// �u�[�X�^�[(wp_ball.h)
// Author : Kai Takada
//
//==================================================
#ifndef _WP_BALL_H_								//���̃}�N����`������ĂȂ�������
#define _WP_BALL_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

#include "weapon.h"
#include "identity.h"

//============================================
// �}�N����`
//============================================

//============================================
// �O���錾
//============================================
class CPlayer;	//C�L�����N�^�[�ł܂Ƃ߂���
class CEnemy;

//============================================
// �{�[���N���X
//============================================
class CBall : public CWeapon
{
public:
	CBall(int nPriority = 3);
	~CBall();

	HRESULT Init(void);			//������
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//������(�I�o��)
	void Uninit(void);			//�I��
	void Update(void);			//�X�V
	void Draw(void) override;			//�`��
	void Attack(const D3DXVECTOR3 rot, const int nDamage = 1) override;			//�U��
	void Throw(const D3DXVECTOR3 vec, float fPower) ;			//�U��
	void Target();
	bool CollisionCircle(const D3DXVECTOR3 pos, const float fRad);			//�����蔻��(�v���C���[)
	bool CollisionWall(const D3DXVECTOR3 pos, const D3DXVECTOR3* pVtx);		//�����蔻��(��)

	void SetMove(const D3DXVECTOR3 move) { m_move = move; }		//�ړ��ʐݒ�
	D3DXVECTOR3 GetMove(void) { return m_move; }				//�ړ��ʎ擾
	void SetMember(const my_Identity::eMember member);		//�����ݒ�
	my_Identity::eMember GetMember() { return m_eMember; }		//�����ݒ�
	void SetPosOld(const D3DXVECTOR3 pos) { m_posOld = pos; }		//�O��ʒu�ݒ�
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }				//�O��ʒu�擾
	void SetDeathCtr(const int nCtr) { m_nDeathCtr = nCtr; }		//���ŃJ�E���^�ݒ�
	int GetDeathCtr(void) { return m_nDeathCtr; }					//���ŃJ�E���^�擾
	void AddSpeedMag(const float fSpeedMag) { m_fSpeedMag += fSpeedMag; }		//�{�����Z
	void SetSpeedMag(const float fSpeedMag) { m_fSpeedMag = fSpeedMag; }		//�{���ݒ�
	float GetSpeedMag(void) { return m_fSpeedMag; }								//�{���擾
	void SetTargetPlayer(CPlayer *pTargetPlayer) { m_pTargetPlayer = pTargetPlayer; }		//�^�[�Q�b�g�ݒ�
	CPlayer *GetTargetPlayer(void) { return m_pTargetPlayer; }								//�^�[�Q�b�g�擾
	void SetTargetEnemy(CEnemy *pTargetEnemy) { m_pTargetEnemy = pTargetEnemy; }		//�^�[�Q�b�g�ݒ�
	CEnemy *GetTargetEnemy(void) { return m_pTargetEnemy; }								//�^�[�Q�b�g�擾

	D3DXVECTOR3 GetWorldPos(void) { return D3DXVECTOR3(GetMtxWorld()._41, GetMtxWorld()._42, GetMtxWorld()._43); }								//�{���擾

	static void SetNumAll(const int nCtr) { s_nNumAll = nCtr; }		//�{�[�������ݒ�
	static int GetNumAll(void) { return s_nNumAll; }					//�{�[�������擾
	static CBall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//����

protected:

private:
	void CollisionField(D3DXVECTOR3 pos);

	float m_fRad;	//���a
	D3DXVECTOR3 m_posOld;			//�O��̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	my_Identity::eMember m_eMember;		//����
	bool m_bLand;		//�n�ʂɗ����Ă�t���O(false:�����ĂȂ�)
	int m_nDeathCtr;	//���R���ŃJ�E���^
	float m_fSpeedMag;		//���x�{��
	CPlayer *m_pTargetPlayer;		//�^�[�Q�b�g
	CEnemy *m_pTargetEnemy;		//�^�[�Q�b�g
	static int s_nNumAll;
};

#endif
