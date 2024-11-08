//===============================================
//
// �e�N�X�`��(texture.h)
// Author: Kai Takada
//
//===============================================
#ifndef _TEXTURE_H_		//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "renderer.h"

//=========================
// �}�N����`
//=========================

//=========================
// �O���錾
//=========================

//=========================
// �v���C���[�N���X
//=========================
class CTexture
{
public:
	CTexture();		//�R���X�g���N�^
	~CTexture();	//�f�X�g���N�^

	HRESULT Load(void);								// �e�N�X�`���ǂݍ���
	void Unload(void);								// �e�N�X�`���j��
	int Regist(const char *pFilename);				// �e�N�X�`���ǉ��Ǎ�
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);		// �e�N�X�`�����擾
	int GetAddress() { return m_nNumAll; }			// �����擾

protected:

private:
	std::list<LPDIRECT3DTEXTURE9> m_apTexture;				//���ʃe�N�X�`���o�b�t�@
	std::list<std::string> m_apFilename;		//�t�@�C��������
	static int m_nNumAll;	//���e�N�X�`����
};

#endif
