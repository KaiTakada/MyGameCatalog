//===============================================
//
// CPU�G(enemy_cpu.h)
// Author ���c ����
//
//===============================================
#ifndef _ENEMY_CPU_H_		//���̃}�N����`������ĂȂ�������
#define _ENEMY_CPU_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "enemy.h"
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
class CObjectCharacter;

//=========================
// CPU�G�N���X
//=========================
class CEnemy_CPU : public CEnemy
{
public:

	//�A�N�V�����񋓌^
	enum EAction
	{
		ACTION_NONE = 0,
		ACTION_CHASE,	// �v���C���[��ǂ�������
		ACTION_THROW,	// ������
		ACTION_PICKUP,	// �{�[�����E���ɍs��
		ACTION_CAUTION,	// �v���C���[���x������(�L���b�`���悤�Ƃ���)
		ACTION_MAX,
	};

	CEnemy_CPU(int nPriority = 3);		//�R���X�g���N�^
	~CEnemy_CPU();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//������(�I�o��)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CEnemy_CPU *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//����

	void SetAction(const EAction eAction) { m_eAction = eAction; }		//�A�N�V�����ݒ�
	EAction GetAction() { return m_eAction; }							//�A�N�V�����擾

	void ActionSelect();	//�A�N�V�����I��

protected:

private:
	void SerchTarget();		//�Ŋ��̓G��⑫

	EAction m_eAction;	//�A�N�V����

	CObjectCharacter* m_pTarget;	//�^�[�Q�b�g�̃L�����N�^�[
};

#endif