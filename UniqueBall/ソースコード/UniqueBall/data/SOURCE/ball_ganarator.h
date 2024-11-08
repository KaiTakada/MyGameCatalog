//==================================================
//
// �u���b�N����(block.h)
// Author : Kai Takada
//
//==================================================
#ifndef _BALL_GENERATOR_H_								//���̃}�N����`������ĂȂ�������
#define _BALL_GENERATOR_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//============================================
// �C���N���[�h�t�@�C��
//============================================
#include "objectX.h"

//============================================
// �O���錾
//============================================
class CBall;

//============================================
// �{�[��������N���X
//============================================
class CBallGenerator : public CObjectX
{
public:
	CBallGenerator(int nPriority = 1);
	~CBallGenerator();

	virtual HRESULT Init(void);			//������
	virtual HRESULT Init(const D3DXVECTOR3 pos);		//������(�I�o��)
	virtual void Uninit(void);			//�I��
	virtual void Update(void);			//�X�V
	virtual void Draw(void);			//�`��

	static CBallGenerator *Create(D3DXVECTOR3 pos = mylib_const::DEFVEC3);		//����
	//��&�u���b�N�Ƃ̓����蔻��
protected:

private:
	CBall *m_pBall;		//���������{�[����ێ�
	int m_nGenerateCtr;		//�{�[�������J�E���^
};

#endif
