//===============================================
//
// �����_���[(renderer.h)
// Author ���c ����
//
//===============================================
#ifndef _RENDERER_H_		//���̃}�N����`������ĂȂ�������
#define _RENDERER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "main.h"

//=========================
// �����_���[�N���X
//=========================
class CRenderer
{
public:
	CRenderer();		//�R���X�g���N�^
	~CRenderer();		//�f�X�g���N�^

	HRESULT Init(HWND hWnd, BOOL bWindow);		//������
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��
	LPDIRECT3DDEVICE9 GetDevice(void);			//�f�o�C�X�擾
	int GetCamCtr() { return m_nCamCtr; }		//���ݕ`�揈�����J�����ԍ�

protected:

private:
	LPDIRECT3D9 m_pD3D;						//Dierct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Dierct3D�f�o�C�X�ւ̃|�C���^
	int m_nCamCtr;		//�J�����`�挻��
};

#endif