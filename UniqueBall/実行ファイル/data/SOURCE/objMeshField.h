//=================================================
//
// ���b�V���n��(objMeshField.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _OBJMESHFIELD_H_								//���̃}�N����`������ĂȂ�������
#define _OBJMESHFIELD_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//============================
// �C���N���[�h�t�@�C��
//============================
#include "manager.h"
#include "object.h"

//============================
// 3D�|���S���N���X
//============================
class CObjMeshField : public CObject
{
public:

	//===================================================
	// �J�����O�̗񋓌^�錾
	//===================================================
	enum
	{
		CULLMODE_NONE = 0,	//�J�����O����
		CULLMODE_CW,		//���v���
		CULLMODE_CCW,		//�����v���
		CULLMODE_MAX,
	}CULLMODE;

	CObjMeshField(int nPriority = 1);
	~CObjMeshField();

	HRESULT Init(void);
	HRESULT Init(const int nNumCutU, const int nNumCutV);		//������(�I�o��)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObjMeshField*Create(void);		//����
	static CObjMeshField*Create(const int nNumCutU, const int nNumCutV);		//����

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }		//�ʒu�ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }				//�ʒu�擾
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }		//�����ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }				//�����擾
	void SetSize2D(const D3DXVECTOR2 size) { m_size = size; }		//�|���S���T�C�Y�ݒ�
	D3DXVECTOR2 GetSize2D(void) { return m_size; }					//�|���S���T�C�Y�擾
	void SetColor(const D3DXCOLOR col) { m_col = col; }		//�e�N�X�`���F�ݒ�
	D3DXCOLOR GetColor() { return m_col; }					//�e�N�X�`���F�擾
	void SetMtxWorld(D3DXMATRIX mtx) { m_mtxWorld = mtx; }		//���[���h�}�g���b�N�X�ݒ�
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }			//���[���h�}�g���b�N�X�擾

	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }		//�e�N�X�`���ԍ��̊��蓖��
	int GetIdxTexture() { return m_nIdxTexture; }				//�e�N�X�`���ԍ��̎擾
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void) { return m_pIdxBuff; }		//�C���f�b�N�X�o�b�t�@�擾
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }		//���_�o�b�t�@�擾

	void GetPolyPos(D3DXVECTOR3* Vtx0, D3DXVECTOR3* Vtx1, D3DXVECTOR3* Vtx2, D3DXVECTOR3* Vtx3);	//4�_�擾
	float GetHeight(D3DXVECTOR3 posObj);				//����Ă���I�u�W�F�N�g�̈ʒuy�擾

	void SetZtest(const bool bZtest) { m_bZtest = bZtest; }			//Z�e�X�g�ݒ�
	void SetAtest(const bool bAtest) { m_bAtest = bAtest; }			//���e�X�g�ݒ�
	void SetAbrend(const bool bAbrend) { m_bAbrend = bAbrend; }		//���u�����h�ݒ�
	void SetLight(const bool bLight) { m_bLight = bLight; }			//���C�e�B���O�ݒ�
	bool GetZtest(void) { return m_bZtest; }		//Z�e�X�g�擾
	bool GetAtest(void) { return m_bAtest; }		//���e�X�g�擾
	bool GetAbrend(void) { return m_bAbrend; }		//���u�����h�擾
	bool GetLight(void) { return m_bLight; }		//���C�e�B���O�擾
	void DrawRenderSet(LPDIRECT3DDEVICE9 pDevice);
	void DrawRenderReset(LPDIRECT3DDEVICE9 pDevice);

private:
	void UpdateVtx();	//���_���̍X�V

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_rot;		//�ړ���
	D3DXVECTOR2 m_size;		//�T�C�Y�E����
	D3DXCOLOR m_col;		//�F
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	int m_nNumCutV;		//������z
	int m_nNumCutU;		//������x
	int m_nNumVtxV;		//���_��z
	int m_nNumVtxU;		//���_��x
	int m_nNumVtxAll;		//�S�Ă̒��_��
	int m_nNumIdx;		//�C���f�b�N�X��
	int m_nNumPrim;		//�v���~�e�B�u��
	bool m_bWireFrame;	//���C���[�t���[��
	int m_nCull;	//�J�����O���[�h
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	int m_nIdxTexture;		//�e�N�X�`���̃C���f�b�N�X
	bool m_bZtest;			//Z�e�X�g[t:on,f:off]
	bool m_bAtest;			//���e�X�g[t:on,f:off]
	bool m_bAbrend;			//���u�����h[t:on,f:off]
	bool m_bLight;			//���C�e�B���O[t:on,f:off]
};
#endif
