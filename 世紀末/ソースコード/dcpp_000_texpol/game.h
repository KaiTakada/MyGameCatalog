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
#include "manager.h"

//=========================
// �O���錾
//=========================
//class CRenderer;
class CPause;
class CGrowSelecter;
class CTimer;
class CEnemy_Boss;
class CResult;

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

	static HRESULT SetGrow(CGrowSelecter *pGrow);					//�����V�[���ݒ�

	static CScore *GetScore() { return m_pScore; }								//�X�R�A�̎擾
	static CPlayer *GetPlayer() { return m_pPlayer; }							//�v���C���[�̎擾
	static CTimer *GetTimer() { return m_pTimer; }								//�^�C�}�[�̎擾
	static CGrowSelecter *GetGrow() { return m_pGrow; }							//�����V�[���擾
	static CEnemy_Boss *GetBoss() { return m_pBoss; }								//�{�X�擾

	static void NullPause() { m_pPause = nullptr; }								//�|�[�Y�|�C���^��null��
	static void NullGrow() { m_pGrow = nullptr; }									//�����V�[���|�C���^��null��

protected:

private:
	static CPlayer *m_pPlayer;					//�v���C���[
	static CPause *m_pPause;					//�|�[�Y
	static CGrowSelecter *m_pGrow;				//�����i���V�[��
	static CScore *m_pScore;					//�X�R�A
	static CTimer *m_pTimer;					//�^�C�}�[
	static CEnemy_Boss *m_pBoss;				//�{�X
	CField *m_pField;							//�n��
	CSpawn *m_pSpawn;							//�G�X�|�[��
	CResult *m_pResult;							//���U���g
	bool m_bResult;								//���U���g
};

#endif