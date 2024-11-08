//===============================================
//
// �e�N�X�`��(texture.cpp)
// Author: Kai Takada
//
//===============================================

//============================
// �C���N���[�h�t�@�C��
//============================
#include "texture.h"
#include "manager.h"

//============================
// �}�N����`
//============================

//============================
// �ÓI�����o�ϐ��錾
//============================
int CTexture::m_nNumAll = 0;				//���ʃe�N�X�`���o�b�t�@

//============================
// �萔
//============================
namespace
{
	const char* c_apTexFilepass[] = 	//�f�t�H���g�e�N�X�`���p�X
	{
		"data\\TEXTURE\\null.jpg" 
	};
}

//============================
// �R���X�g���N�^
//============================
CTexture::CTexture()
{
	m_apTexture.clear();
}

//============================
// �f�X�g���N�^
//============================
CTexture::~CTexture()
{
	m_apTexture.clear();
}

//============================
// ���L�e�N�X�`���擾
//============================
HRESULT CTexture::Load(void)
{
	for (int nCntFile = 0; nCntFile < sizeof(c_apTexFilepass) / sizeof(c_apTexFilepass[0]); nCntFile++)
	{
		Regist(c_apTexFilepass[nCntFile]);
	}

	return S_OK;
}

//============================
// ���L�e�N�X�`���j��
//============================
void CTexture::Unload(void)
{
	m_apTexture.clear();
}

//============================
// �e�N�X�`���ǉ��Ǎ�
//============================
int CTexture::Regist(const char *pFilename)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	LPDIRECT3DTEXTURE9 pObj = nullptr;

	// string�^�ɕϊ�
	std::string strFilename(pFilename);

	int i = 0;

	// ���X�g���[�v
	for (std::list<std::string>::const_iterator it = m_apFilename.cbegin(); it != m_apFilename.cend(); it++)
	{
		std::string filename = *it;

		if (strcmp(filename.c_str(), strFilename.c_str()) == 0)
		{//���łɓǂݍ���ł����

			return i;
		}

		i++;
	}

	//�e�N�X�`���̓ǂݍ���[0]
	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice,
		strFilename.c_str(),			//�e�N�X�`���̃t�@�C����
		&pObj)))
	{//���������ꍇ
		m_apFilename.push_back(strFilename);
		m_apTexture.push_back(pObj);
		m_nNumAll++;
		return i;
	}
	else
	{
		return 0;
	}

	return 0;
}

//============================
// �ԍ��ɍ������e�N�X�`����n��
//============================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx < 0 || nIdx > m_apTexture.size())
	{
		return nullptr;
	}
	return *std::next(m_apTexture.begin(), nIdx);
}
