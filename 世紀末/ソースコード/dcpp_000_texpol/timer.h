//===============================================
//
// �X�R�A(score.cpp)
// Author ���c ����
//
//===============================================
#ifndef _TIMER_H_		//���̃}�N����`������ĂȂ�������
#define _TIMER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "manager.h"
#include "object.h"

//=========================
// �}�N����`
//=========================
#define MAX_TIME (3)	//�X�R�A����

//=========================
// �O���錾
//=========================
class CNumber;

//=========================
// �I�u�W�F�N�g2D�w�i�N���X
//=========================
class CTimer : public CObject
{
public:
	CTimer();		//�R���X�g���N�^
	~CTimer();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//������(�I�o��)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CTimer *Create(void);		//����
	static CTimer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size);		//����

	void SetValue(int nValue);		//�X�R�A�l�ݒ�
	void CntValue(int nValue);		//�X�R�A�l���Z
	int GetValue() { return m_nValue; }		//�X�R�A�l�擾

	//���������֐��Ŏd���Ȃ��ɍ�����B��ŏ�������
	void SetPos(const D3DXVECTOR3 pos);			//�ʒu�ݒ�
	void SetRot(const D3DXVECTOR3 rot);			//�����ݒ�
	D3DXVECTOR3 GetPos(void);		//�ʒu�擾
	D3DXVECTOR3 GetSize(void);		//2D�T�C�Y�擾
	bool GetJump(void) { return false; }		//�W�����v����
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//�O��ʒu�擾

protected:

private:
	CNumber *m_apNumber[MAX_TIME];		//�X�R�A���
	int m_nValue;		//�X�R�A�̒l
	int m_nSecond;		//1�b���萔�l
};

#endif