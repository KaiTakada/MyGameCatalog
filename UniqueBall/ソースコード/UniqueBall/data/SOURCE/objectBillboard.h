//=================================================
//
// �r���{�[�h(ObjectBillboard.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _OBJECTBILLBOARD_H_								//���̃}�N����`������ĂȂ�������
#define _OBJECTBILLBOARD_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//===============================
// �C���N���[�h�t�@�C��
//===============================
#include "obj_polygon3D.h"

//============================
// �}�N����`
//============================
#define DEF_BBSIZE D3DXVECTOR3(20.0f,0.0f,20.0f)				//�f�t�H���g�T�C�Y
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//�f�t�H���g�T�C�Y

//===============================
// �O���錾
//===============================
class CAnim;

//===============================
// �r���{�[�h�N���X
//===============================
class CObjectBillboard : public CObject3D
{
public:
	CObjectBillboard(int nPriority = 3);
	~CObjectBillboard();
	
	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��
	static CObjectBillboard *Create(void);		//����
	static CObjectBillboard *Create(const D3DXVECTOR3 pos = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_BBSIZE, bool bZtest = false);		//����

	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//�ړ��ʐݒ�
	D3DXVECTOR3 GetMove(void) { return m_move; }		//2D�T�C�Y�擾
	void SetSize(const D3DXVECTOR3 size) override;			//�T�C�Y�ݒ�

protected:

private:
	LPDIRECT3DTEXTURE9 m_pTexture;				//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_move;							//�ړ���
};

//=========================
// �I�u�W�F�N�g2D�A�j���[�V�����N���X
//=========================
class CObjectBillboardAnim : public CObjectBillboard
{
public:
	CObjectBillboardAnim(int nPriority = 3);		//�R���X�g���N�^
	~CObjectBillboardAnim();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��
	static CObjectBillboardAnim *Create(const D3DXVECTOR3 pos = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_BBSIZE);		//����
	CAnim *GetAnim(void) { return m_anim; }		//�A�j�����擾
	void SetAnim(CAnim *anim) { m_anim = anim; }		//�A�j�����ݒ�

protected:

private:
	CAnim *m_anim;		//�A�j���[�V�����v�f
};

#endif