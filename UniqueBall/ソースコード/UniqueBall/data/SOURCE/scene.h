//===============================================
//
// �V�[��(scene.h)
// Author ���c ����
//
//===============================================
#ifndef _SCENE_H_		//���̃}�N����`������ĂȂ�������
#define _SCENE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "main.h"
#include "renderer.h"
#include "constant.h"

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
// ���(�V�[��)�N���X
//=========================
class CScene
{
public:
	enum MODE
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	};

	CScene();
	~CScene();

	virtual HRESULT Init();		//������
	virtual void Uninit();		//�I��
	virtual void Update();		//�X�V
	virtual void Draw();		//�`��
	static CScene *Create(MODE mode);		//����
	CScene::MODE GetMode() { return m_mode; }		//���݂̃��[�h�擾
	static CFade *GetFade(void) { return m_pFade; }									//�t�F�[�h�̓�

	static CNumber *GetNumber(void) { return m_pNumber; }								//�����̎擾
	static CScore *GetScore(void) { return m_pScore; }									//�X�R�A�̎擾

	static CObject3D *GetObject3D(void) { return m_pObject3D; }							//3D�|���S���̎擾
	static CField *GetField(void) { return m_pField; }									//�n�ʂ̎擾

	static CObjectBillboard *GetBillBoard(void) { return m_pBillBoard; }				//�r���{�[�h�̎擾
	static CItem *GetItem(void) { return m_pItem; }										//�A�C�e���̎擾
	static CPlayer **GetElem(void) { return &m_pPlayer[0]; }						//�v���C���[�S�̂̎擾
	static CPlayer *GetElem(int nNum) { return m_pPlayer[nNum]; }						//����̃v���C���[�̎擾
	static void SetPlayer(CPlayer *pPlayer, int nIdx) { m_pPlayer[nIdx] = pPlayer; }						//����̃v���C���[�̐ݒ�

	static CBlock *GetBlock(void) { return m_pBlock; }									//�u���b�N�̎擾

	static int GetNowTime(void) { return m_nTime; }									//����v���C���ʂ̎擾
	static void SetNowTime(int nScore) { m_nTime = nScore; }							//����v���C���ʂ̎擾
	static int GetNumPlayer(void) { return m_nNumPlayer; }									//�v���C���[�l���̎擾
	static void SetNumPlayer(int nNumPlayer) { m_nNumPlayer = nNumPlayer; }					//�v���C���[�l���̎擾

protected:

private:
	MODE m_mode;		//���݂̃��[�h

	static CNumber *m_pNumber;							//����
	static CScore *m_pScore;							//�X�R�A
	static CFade *m_pFade;								//�t�F�[�h

	static CObject3D *m_pObject3D;						//3D�|���S��
	static CField *m_pField;							//�n��

	static CObjectBillboard *m_pBillBoard;				//�r���{�[�h
	static CItem *m_pItem;								//�A�C�e��
	static CPlayer *m_pPlayer[mylib_const::MAX_PLAYER];					//�v���C���[

	static CBlock *m_pBlock;							//�u���b�N
	static int m_nTime;								//����v���C����
	static int m_nNumPlayer;						//�v���C���[�l��
};

#endif
