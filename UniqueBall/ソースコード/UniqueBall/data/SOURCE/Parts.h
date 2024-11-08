//===============================================
//
// X���f���I�u�W�F�N�g�p�[�c(Parts.cpp)	//�e���f���������f��
// Author: ���c ����
//
//===============================================
#ifndef _PARTS_H_								//���̃}�N����`������ĂȂ�������
#define _PARTS_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//================================
// �C���N���[�h�t�@�C��
//================================
#include "manager.h"
#include "object.h"

//================================
// �}�N����`
//================================
namespace my_parts
{
	const int NUM_TEX_PASS = 4;		//�����ւ��e�N�X�`����
}

//================================
// ���f���N���X
//================================
class CParts : public CObject
{
public:
	//=============================
	// �p�[�c���\���̂̒�`
	//=============================
	typedef struct
	{
		char *pFilepass;		//�t�@�C���p�X
		int nIdxParent;			//�e�̃C���f�b�N�X
	}PassParent;

	CParts(int nPriority = 3);
	~CParts();

	HRESULT Init(void);							//������(�f�t�H)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);			//������(�I�o��)
	void Uninit(void);							//�I��
	void Update(void);							//�X�V
	void Draw(void);							//�`��

	static CParts *Create(const char *pFilename, const D3DXVECTOR3 pos = mylib_const::DEFVEC3, const D3DXVECTOR3 rot = mylib_const::DEFVEC3);		//����
	void SetParent(CParts *pParts);
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }						//���݈ʒu�ݒ�
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }						//���݌����ݒ�
	void SetScale(const float fScale) { m_scale = D3DXVECTOR3(fScale, fScale, fScale); }						//�T�C�Y�ݒ�
	D3DXVECTOR3 GetScale() { return m_scale; }						//�T�C�Y�擾
	void SetMultiply(const bool bMultipy) { m_bMultiply = bMultipy; }		//�|�����킹�t���O�ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }				//���݈ʒu�擾
	D3DXVECTOR3 GetRot(void) { return m_rot; }				//���݌����擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }		//���[���h�}�g���b�N�X�擾
	bool GetMultiply(void) { return m_bMultiply; }			//�|�����킹�t���O�擾
	D3DXVECTOR3 GetWorldPos(void) { return D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43); }		//���[���h���W�擾

	void SetIdxModel(int nIdx) { m_nIdxModel = nIdx; }		//���f���ԍ��̊��蓖��
	int GetIdxModel() { return m_nIdxModel; }				//���f���ԍ��̎擾
	D3DXVECTOR3 GetOutcomePos(CParts *pParts);				//���v���W
	CParts *GetParent() { return m_pParent; }				//�e���f���|�C���^�擾
	void SetMat(D3DXMATERIAL mat) { m_mat = mat; }			//�F�ݒ�
	std::string *GetTexPass() { return m_sTexPass; }				//�����ւ��e�N�X�`���擾
	void SetTexPass(std::string sTexPass, int nIdx);			//�����ւ��e�N�X�`���ݒ�
	int GetIdxParent() { return m_nIdxParent; }				//�����ւ��ԍ��擾
	void SetIdxParent(int nIdx) { m_nIdxParent = nIdx; }			//�����ւ��ԍ��ݒ�

protected:

private:
	D3DXMATRIX m_mtxWorld;			//���݈ʒu
	D3DXVECTOR3 m_pos;			//���݈ʒu
	D3DXVECTOR3 m_rot;			//���݌���
	D3DXVECTOR3 m_scale;		//���݃X�P�[��(�f�t�H���g1.0f)
	CParts *m_pParent;			//�e���f���ւ̃|�C���^
	int m_nIdxModel;			//���f���ԍ�
	D3DXMATERIAL m_mat;			//�w��F
	bool m_bMultiply;		//�eor�ŐV�Ɗ|�����킹��t���O(false:���킹��)
	int m_nIdxParent;		//�e���ԍ�������������
	std::string m_sTexPass[my_parts::NUM_TEX_PASS];		//�����ւ��e�N�X�`��
	LPDIRECT3DTEXTURE9 m_pTexture[my_parts::NUM_TEX_PASS];		//�����ւ��e�N�X�`��
};

#endif
