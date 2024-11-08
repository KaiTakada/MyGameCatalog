//===============================================
//
// �Q�[��(game.h)
// Author: ���c ����
//
//===============================================
#ifndef _GAMEMANAGER_H_		//���̃}�N����`������ĂȂ�������
#define _GAMEMANAGER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
#include "main.h"

//=========================
// �萔��`
//=========================

//=========================
// �O���錾
//=========================
class CObject2D;
class CNumber;

//=========================
// ���(�V�[��)�N���X
//=========================
class CGameManager
{
public:
	CGameManager();
	~CGameManager();

	enum FlowStage
	{
		NONE = 0,
		START,
		INGAME,
		GAMESET,
		MAX
	};

	HRESULT Init();		//������
	void Uninit();		//�I��
	void Update();		//�X�V
	void Draw();		//�`��
	static CGameManager *Create();		//����

	void SetFlowStage(FlowStage set);						//�󋵐ݒ�
	FlowStage GetFlowStage() { return m_FlowStage; }		//�󋵎擾
	void SetEnd(bool set) { m_bEnd = set; }	//�I���t���O�ݒ�
	bool GetEnd() { return m_bEnd; }		//�I���t���O�擾

protected:

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CGameManager::* UPD_FUNC)(void);
	typedef void(CGameManager::* CRT_FUNC)(void);

	static UPD_FUNC m_UpdateFuncList[];
	static CRT_FUNC m_CreateFuncList[];

	void CreateStart();		//�X�^�[�g������
	void CreateGameset();		//�G���h��������

	void UpdateStart();		//�X�^�[�g���X�V
	void UpdateGameset();		//�G���h���X�V

	FlowStage m_FlowStage;		//�󋵊Ǘ�
	CObject2D* m_OnePoint;		//�X�^�[�g���G���h�̃|���S��
	CNumber* m_pCountdown;		//�J�E���g�_�E���̃|���S��
	int m_nStayTime;			//�Ƃǂ܂鎞��
	int m_nRotateTime;			//�܂�鎞��
	bool m_bEnd;				//�����I���̂��m�点
};

#endif