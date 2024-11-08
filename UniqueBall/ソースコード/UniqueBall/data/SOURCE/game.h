//===============================================
//
// �Q�[��(game.h)
// Author: ���c ����
//
//===============================================
#ifndef _GAME_H_		//���̃}�N����`������ĂȂ�������
#define _GAME_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "main.h"
#include "scene.h"

//=========================
// �萔��`
//=========================
namespace mygame
{
	const int NUMPLAYER = 2;	//�v���C���[�̌Œ�l��
	const int NUM_WALL = 4;		//�ǂ̌Œ萔
}

//=========================
// �O���錾
//=========================
//class CRenderer;
class CPause;
class CGrowSelecter;
class CTimer;
class CMap;
class CCameraPlayer;
class CWall;
class CGameManager;
class CObjMeshField;

//=========================
// ���(�V�[��)�N���X
//=========================
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	virtual HRESULT Init();		//������
	virtual void Uninit();		//�I��
	virtual void Update();		//�X�V
	virtual void Draw();		//�`��
	static CGame *Create();		//����

	static CPause *GetPause() { return m_pPause; }								//�|�[�Y�̎擾
	static CTimer *GetTimer() { return m_pTimer; }								//�^�C�}�[�̎擾
	static CMap *GetMap() { return m_pMap; }								//�}�b�v�̎擾

	static void NullPause() { m_pPause = nullptr; }								//�|�[�Y�|�C���^��null��
	static bool MapFlag() { return m_pMap != nullptr; }								//�|�[�Y�|�C���^��null��
	CCameraPlayer *GetCamera(int nIdx) { return m_ppCamera[nIdx]; }					//�����J������1���擾
	CCameraPlayer **GetCamera() { return m_ppCamera; }								//�����J�����̎擾
	CObjMeshField* GetField() { return m_pField; }						//�����擾
	CWall *GetWall(int nIdx) { return m_ppWall[nIdx]; }					//�ǂ�1���擾
	CWall **GetWall() { return m_ppWall; }								//�ǂ̎擾

protected:

private:
	static CPause *m_pPause;					//�|�[�Y
	static CTimer *m_pTimer;					//�^�C�}�[
	static CMap *m_pMap;						//�}�b�v
	CObjMeshField *m_pField;							//�n��
	CWall *m_ppWall[mygame::NUM_WALL];							//�n��
	CEnemy *m_pBot;								//�������{�b�g
	CCameraPlayer *m_ppCamera[mylib_const::MAX_PLAYER];		//�v���C���[�Ǐ]�J����
	CGameManager* m_pGameManager;	//�Q�[���}�l�[�W���[
};

#endif