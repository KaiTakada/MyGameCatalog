//===============================================
//
// ���U���gUI(ui_result.cpp)
// Author : ���c ����
//
//===============================================
#ifndef _UI_RESULT_H_		//���̃}�N����`������ĂȂ�������
#define _UI_RESULT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "manager.h"
#include "object.h"

//=========================
// �}�N����`
//=========================

//=========================
// �O���錾
//=========================
class CScore;
class CObject2D;

//=========================
// �I�u�W�F�N�g2D�w�i�N���X
//=========================
class CUI_Result : public CObject
{
public:
	CUI_Result();		//�R���X�g���N�^
	~CUI_Result();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CUI_Result *Create(void);		//����

	void SetMark(CObject2D* pMark, int nIdx) { m_apFrame[nIdx] = pMark; }			//�ʒu�ݒ�
	CObject2D **GetMark() { return m_apFrame; }		//�ʒu�擾
	CObject2D *GetMark(int nIdx) { return m_apFrame[nIdx]; }		//�ʒu�擾

	bool GetTrans() { return m_bTrans; }	//�J�ڃt���O�擾

protected:

private:
	CObject2D *m_apFrame[mylib_const::MAX_PLAYER];		//�X�R�A���
	bool m_bOK[mylib_const::MAX_PLAYER];	//�J�ړ��Ӄt���O
	bool m_bTrans;	//�J�ڃt���O
};

#endif