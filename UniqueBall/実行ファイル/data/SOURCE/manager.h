//===============================================
//
// �}�l�[�W��(manager.h)
// Author ���c ����
//
//===============================================
#ifndef _MANAGER_H_		//���̃}�N����`������ĂȂ�������
#define _MANAGER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "main.h"
#include "renderer.h"
#include "scene.h"

//=========================
// �O���錾
//=========================
class CRenderer;
class CTexture;
class CXModel;
class CCamera;
class CLight;
class CSound;
class CFade;

class CInputKeyboard;
class CInputGamepad;
class CInputMouse;

class CBg;
class CMultiBg;
class CNumber;
class CScore;

class CObject3D;
class CField;

class CObjectBillboard;
class CItem;
class CBullet;
class CExplosion;
class CEffect;
class CParticle;

class CObjectX;
class CBlock;
class CEnemy;
class CPlayer;

class CDebugProc;

//=========================
// �}�l�[�W���N���X
//=========================
class CManager
{
public:

	//========================
	// ���U���g��
	//========================
	enum RESULT_TYPE
	{
		RT_NONE = 0,
		RT_WIN,
		RT_LOSE,
		RT_MAX,
	};

	CManager();			//�R���X�g���N�^
	~CManager();		//�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		//������
	void Uninit(void);												//�I��
	void Update(void);												//�X�V
	void Draw(void);												//�`��
	void SetMode(CScene::MODE mode);							//�V�[���ݒ�
	
	static CManager *GetInstance();						//�C���X�^���X�擾
	static void Release();						//�����[�X

	CRenderer *GetRenderer(void) { return m_pRenderer; }							//�����_���[�̎擾
	CTexture *GetTexture(void) { return m_pTeture; }								//�e�N�X�`���̎擾
	CXModel *GetXModel(void) { return m_pXModel; }								//X�t�@�C�����f���̎擾
	CCamera *GetCamera(void) { return m_pCamera; }								//�J�����̎擾
	CLight *GetLight(void) { return m_pLight; }									//���C�g�̎擾
	CSound *GetSound(void) { return m_pSound; }									//�T�E���h�̎擾

	CInputKeyboard *GetInputKeyboard(void) { return m_pCInputKeyboard; }			//���͏��̎擾(�L�[�{�[�h)
	CInputGamepad *GetInputGamepad(void) { return m_pCInputGamepad; }			//���͏��̎擾(Pad [X])
	CInputMouse *GetInputMouse(void) { return m_pCInputMouse; }					//���͏��̎擾(�}�E�X)

	CDebugProc *GetDebugProc(void) { return m_pDebugProc; }				//�f�o�b�O�̎擾
	CScene *GetScene(void) { return m_pScene; }							//�V�[���̎擾

	void SetPause(bool bPause) { m_bPause = bPause; }					//�|�[�Y��Ԑݒ�
	bool GetPause(void) { return m_bPause; }								//�|�[�Y��Ԏ擾
	void InvPause(void) { m_bPause = m_bPause ? false : true; }			//�|�[�Y�؂�ւ�

	void SetGrow(bool bGrow) { m_bGrow = bGrow; }						//�i���V�[����Ԑݒ�
	bool GetGrow(void) { return m_bGrow; }								//�i���V�[����Ԏ擾
	void InvGrow(void) { m_bGrow = m_bGrow ? false : true; }			//�i���V�[���؂�ւ�

	void SetEdit(bool bEdit) { m_bEdit = bEdit; }						//�i���V�[����Ԑݒ�
	bool GetEdit(void) { return m_bEdit; }								//�i���V�[����Ԏ擾
	void InvEdit(void) { m_bEdit = m_bEdit ? false : true; }			//�i���V�[���؂�ւ�

	void SetResult(RESULT_TYPE result) { m_result = result; }			//���s��Ԑݒ�
	RESULT_TYPE GetResult(void) { return m_result; }					//���s��Ԏ擾
	
	void SetRank(int nRank,int nIdx) { m_anRank[nIdx] = nRank; }			//���ʐݒ�
	int GetRank(int nIdx) { return m_anRank[nIdx]; }						//���ʎ擾(1��)
	int *GetRank() { return m_anRank; }									//���ʎ擾
	void ZeroRank();									//���ʎ擾

protected:

private:
	CRenderer *m_pRenderer;						//�����_���[
	CTexture *m_pTeture;						//�e�N�X�`��
	CXModel *m_pXModel;							//X�t�@�C�����f��
	CCamera *m_pCamera;							//�J����
	CLight *m_pLight;							//���C�g
	CSound *m_pSound;							//�T�E���h

	CInputKeyboard *m_pCInputKeyboard;			//����(�L�[�{�[�h)
	CInputGamepad *m_pCInputGamepad;			//����(Pad[X])
	CInputMouse *m_pCInputMouse;				//����(�}�E�X)

	CDebugProc *m_pDebugProc;					//�f�o�b�O
	CScene* m_pScene;							//�V�[��

	bool m_bPause;				//�|�[�Y�t���O(���Q�[���V�[���Ŏg�p��Renderer����)
	bool m_bGrow;				//�i���V�[���t���O(���Q�[���V�[���Ŏg�p��Renderer����)
	bool m_bEdit;				//�G�f�B�b�g�t���O(���Q�[���V�[���Ŏg�p��Renderer����)
	RESULT_TYPE m_result;		//���s�t���O(���Q�[�������U���g�V�[���Ŏg�p)
	int m_anRank[mylib_const::MAX_PLAYER];	//���ʕۑ�(�l�̓v���C���[�ԍ��A���ʂ͔��ԍ�

	static CManager *m_pManager;		//�C���X�^���X�ێ�
};

#endif
