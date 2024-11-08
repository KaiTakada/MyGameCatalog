//=================================================
//
// �r���{�[�h(target_mark.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _TARGET_MARK_H_								//���̃}�N����`������ĂȂ�������
#define _TARGET_MARK_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//===============================
// �C���N���[�h�t�@�C��
//===============================
#include "objectBillboard.h"

//============================
// �}�N����`
//============================

//===============================
// �O���錾
//===============================
class CAnim;

//===============================
// �r���{�[�h�N���X
//===============================
class CTarget_Mark : public CObjectBillboard
{
public:
	CTarget_Mark(int nPriority = 3);
	~CTarget_Mark();
	
	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��
	static CTarget_Mark *Create(void);		//����
	static CTarget_Mark *Create(const D3DXVECTOR3 pos);		//����

	void SetIdx(int nIdx) { m_nIdx = nIdx; }

protected:

private:
	int m_nIdx;		//�g�p���Ă���v���C���[�ԍ�
};

#endif