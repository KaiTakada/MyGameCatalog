//===============================================
//
// �̗�UI(ui_life.cpp)
// Author ���c ����
//
//===============================================
#ifndef _UI_LIFE_H_		//���̃}�N����`������ĂȂ�������
#define _UI_LIFE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "manager.h"
#include "object.h"

//=========================
// �}�N����`
//=========================
namespace myUI_life
{
	const int MAX_NUM_LIFE = 10;	//�z��ő吔
}

//=========================
// �O���錾
//=========================
class CScore;
class CObject2D;

//=========================
// �I�u�W�F�N�g2D�w�i�N���X
//=========================
class CUI_Life : public CObject
{
public:
	CUI_Life();		//�R���X�g���N�^
	~CUI_Life();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CUI_Life *Create(void);		//����
	static CUI_Life *Create(const int nNumMark, const int nIdx);		//����

	void SetPos(const D3DXVECTOR3 pos) override;			//�ʒu�ݒ�
	D3DXVECTOR3 GetPos() { return m_pos; }		//�ʒu�擾

	void SetNumMark(const int nNumMark) { m_nNumMark = nNumMark; }			//�ʒu�ݒ�
	int GetNumMark() { return m_nNumMark; }		//�ʒu�擾

	void SetIdx(const int nIdx) { m_nIdx = nIdx; }			//�ԍ��ݒ�
	int GetIdx() { return m_nIdx; }		//�ԍ��擾

	void SetMark(CObject2D* pMark, int nIdx) { m_apMark[nIdx] = pMark; }			//�ʒu�ݒ�
	CObject2D **GetMark() { return m_apMark; }		//�ʒu�擾
	CObject2D* GetMark(int nIdx) { return m_apMark[nIdx]; }		//�ʒu�擾

	void SetColAll(D3DXCOLOR col);		//�S�Ă̐F�ς�

	void AddLife(const int nIdx);			//����
	void SubLife(const int nIdx);			//����

protected:

private:
	CObject2D *m_apMark[myUI_life::MAX_NUM_LIFE];		//�X�R�A���
	D3DXVECTOR3 m_pos;	//�ʒu
	int m_nNumMark;	//�}�[�N��
	int m_nIdx;	//�f��J�����ԍ�
};

#endif