//===============================================
//
// �G�t�F�N�g(effect.cpp)
// Author ���c ����
//
//===============================================
#ifndef _EFFECT_H_		//���̃}�N����`������ĂȂ�������
#define _EFFECT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "manager.h"
#include "objectBillboard.h"

//=========================
// �}�N����`
//=========================

//=========================
// �I�u�W�F�N�g2D�w�i�N���X
//=========================
class CEffect : public CObjectBillboard
{
public:
	enum TEX
	{
		TEX_000 = 0,
		TEX_MAX,
	};

	CEffect(int nPriority = 4);		//�R���X�g���N�^
	~CEffect();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CEffect *Create(void);		//����
	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const D3DXVECTOR3 size, int nLife, const TEX TexType);		//����

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_MAX];				//�e�N�X�`���o�b�t�@
	int m_nLife;		//�c�����
	int m_nLifeMax;		//����
};

#endif