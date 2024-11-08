//=================================================
//
// 3D�|���S��(object3D.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _OBJECT3D_H_								//���̃}�N����`������ĂȂ�������
#define _OBJECT3D_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//============================
// �C���N���[�h�t�@�C��
//============================
#include "manager.h"
#include "object.h"

//============================
// �}�N����`
//============================
#define DEF_SIZE D3DXVECTOR3(100.0f,0.0f,100.0f)				//�f�t�H���g�T�C�Y
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//�f�t�H���g�T�C�Y

//============================
// �O���錾
//============================
class CAnim;

//============================
// 3D�|���S���N���X
//============================
class CObject3D : public CObject
{
public:
	CObject3D(int nPriority = 1);
	~CObject3D();

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//������(�I�o��)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject3D *Create(void);		//����
	static CObject3D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_SIZE);		//����

	void SetPos(const D3DXVECTOR3 pos);				//�ʒu�ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }		//���݈ʒu�擾
	void SetRot(const D3DXVECTOR3 rot);				//�����ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }		//�����擾
	virtual void SetSize(const D3DXVECTOR3 size);		//�|���S���T�C�Y�ݒ�
	D3DXVECTOR3 GetSize(void) { return m_size; }		//�|���S���T�C�Y�擾
	void SetColor(const D3DXCOLOR col);				//�e�N�X�`���F�ݒ�
	D3DXCOLOR GetColor();							//�e�N�X�`���F�擾
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }		//�e�N�X�`���ԍ��̊��蓖��
	int GetIdxTexture() { return m_nIdxTexture; }				//�e�N�X�`���ԍ��̎擾
	void SetMtxWorld(D3DXMATRIX mtx) { m_mtxWorld = mtx; }		//���[���h�}�g���b�N�X�ݒ�
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }			//���[���h�}�g���b�N�X�擾

	void SetSizeValue(const D3DXVECTOR3 size) { m_size = size; }			//�T�C�Y�l�ݒ�
	void SetNormal(const D3DXVECTOR3 nor);			//�@���ݒ�
	void SetDivTex(int nDiv0, int nDiv1);											//�e�N�X�`���̕������ݒ�
	void SetTexPos(const D3DXVECTOR2 rightDown, const D3DXVECTOR2 leftUp);			//�e�N�X�`�����W�ݒ�
	void GetPolyPos(D3DXVECTOR3* Vtx0, D3DXVECTOR3* Vtx1, D3DXVECTOR3* Vtx2, D3DXVECTOR3* Vtx3);	//4�_�擾
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }		//�o�b�t�@�擾

	float GetHeight(D3DXVECTOR3 posObj);				//����Ă���I�u�W�F�N�g�̈ʒuy�擾

	void SetZtest(const bool bZtest) { m_bZtest = bZtest; }			//Z�e�X�g�ݒ�
	void SetAtest(const bool bAtest) { m_bAtest = bAtest; }			//���e�X�g�ݒ�
	void SetAbrend(const bool bAbrend) { m_bAbrend = bAbrend; }		//���u�����h�ݒ�
	void SetLight(const bool bLight) { m_bLight = bLight; }			//���C�e�B���O�ݒ�
	bool GetZtest(void) { return m_bZtest; }		//Z�e�X�g�擾
	bool GetAtest(void) { return m_bAtest; }		//���e�X�g�擾
	bool GetAbrend(void) { return m_bAbrend; }		//���u�����h�擾
	bool GetLight(void) { return m_bLight; }		//���C�e�B���O�擾

protected:
	void DrawRenderSet(LPDIRECT3DDEVICE9 pDevice);
	void DrawRenderReset(LPDIRECT3DDEVICE9 pDevice);

private:
	D3DXVECTOR3 vec(D3DXVECTOR3 vtxpos0, D3DXVECTOR3 vtxpos1, D3DXVECTOR3 vtxpos2);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_pos;					//�ʒu
	D3DXVECTOR3 m_size;					//�T�C�Y
	D3DXVECTOR3 m_rot;					//�ړ���
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g���b�N�X
	int m_nIdxTexture;		//�e�N�X�`���̃C���f�b�N�X
	bool m_bZtest;			//Z�e�X�g[t:on,f:off]
	bool m_bAtest;			//���e�X�g[t:on,f:off]
	bool m_bAbrend;			//���u�����h[t:on,f:off]
	bool m_bLight;			//���C�e�B���O[t:on,f:off]
};

//=========================
// �I�u�W�F�N�g2D�A�j���[�V�����N���X
//=========================
class CObject3DAnim : public CObject3D
{
public:
	CObject3DAnim(int nPriority = 3);		//�R���X�g���N�^
	~CObject3DAnim();		//�f�X�g���N�^

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��
	CAnim *GetAnim(void) { return m_anim; }		//�A�j�����擾
	static CObject3DAnim* Create(void);		//����
	static CObject3DAnim* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_SIZE);		//����
	static int GetNumAll() { return s_nNumAll; }		//����

protected:

private:
	CAnim *m_anim;		//�A�j���[�V�����v�f
	static int s_nNumAll;
};

#endif
