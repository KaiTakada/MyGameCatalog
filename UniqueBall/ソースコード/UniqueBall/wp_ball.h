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
	bool CollisionCircle(const D3DXVECTOR3 pos, const float fRad);			//�����蔻��(�v���C���[)
	bool CollisionWall(const D3DXVECTOR3 pos, const D3DXVECTOR3* pVtx);		//�����蔻��(��)

	void SetMember(const my_Identity::eMember member);		//�����ݒ�
	my_Identity::eMember GetMember() { return m_eMember; }		//�����ݒ�
	void SetPosOld(const D3DXVECTOR3 pos) { m_posOld = pos; }		//�O��ʒu�ݒ�
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }				//�O��ʒu�擾
	void SetDeathCtr(const int nCtr) { m_nDeathCtr = nCtr; }		//���ŃJ�E���^�ݒ�
	int GetDeathCtr(void) { return m_nDeathCtr; }					//���ŃJ�E���^�擾
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

	static int s_nNumAll;
};

#endif
