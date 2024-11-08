//===============================================
//
// �ړ�2D(move2D.h)
// Author ���c ����
//
//===============================================
#ifndef _MOVE2D_H_		//���̃}�N����`������ĂȂ�������
#define _MOVE2D_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "manager.h"
#include "obj_polygon2D.h"

//=========================
// �}�N����`
//=========================

//=========================
// �O���錾
//=========================

//=========================
// �I�u�W�F�N�g2D�N���X
//=========================
class CMove2D : public CObject2D
{
public:

	//=========================
	// �^�C�v�񋓌^
	//=========================
	enum UPDATE_TYPE
	{
		UPDATE_TYPE_NONE = 0,
		UPDATE_TYPE_ANIM,
		UPDATE_TYPE_MAX
	};

	CMove2D(int nPriority = CObject::PRIORITY_UI);		//�R���X�g���N�^
	~CMove2D();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CMove2D *Create(void);		//����
	static CMove2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posDest,const float fSpeed);		//����

	void SetPosDest(const D3DXVECTOR3 posDest) { m_posDest = posDest; }			//�ڕW���W�ݒ�
	D3DXVECTOR3 GetPosDest(void) { return m_posDest; }							//�ڕW���W�擾
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }			//�ړ��ʐݒ�
	float GetSpeed(void) { return m_fSpeed; }					//�ړ��ʎ擾

protected:

private:
	D3DXVECTOR3 m_posDest;		//�ڕW���W
	float m_fSpeed;		//�ړ��X�s�[�h
};

#endif