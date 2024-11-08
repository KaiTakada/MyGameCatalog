//===============================================
//
// �v���C���[�}�l�[�W��(playerManager.h)
// Author: Kai Takada
//
//===============================================
#ifndef _PLAYER_MANAGER_H_		//���̃}�N����`������ĂȂ�������
#define _PLAYER_MANAGER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================

//=========================
// �O���錾
//=========================
class CPlayer;

//=========================
// �}�l�[�W���N���X
//=========================
class CPlayerManager
{
public:

	CPlayerManager();			//�R���X�g���N�^
	~CPlayerManager();		//�f�X�g���N�^

	HRESULT Init();		//������
	void Uninit();												//�I��
	void Update();												//�X�V

	static CPlayerManager* GetInstance();						//�C���X�^���X�擾
	static void Release();						//�����[�X
	void Add(CPlayer* pPlayer);						//�v�f�ǉ�

	CPlayer* GetElem(int nIdx);

protected:

private:
	static CPlayerManager* m_pManager;		//�C���X�^���X�ێ�

	std::list<CPlayer*> m_pPlayer;		//�v���C���[
	//CPlayer* m_pPlayer;					//�v���C���[
};

#endif
