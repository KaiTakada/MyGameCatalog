//===============================================
//
// ���[�V����(motion.h)
// Author ���c ����
//
//===============================================
#ifndef _MOTION_H_		//���̃}�N����`������ĂȂ�������
#define _MOTION_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================
//#include "objectX.h"

//=========================
// �}�N����`
//=========================
#define MAX_PARTS (32)		//�ő�p�[�c��
#define NUM_MOTION (32)				//���[�V������
#define NUM_KEY (32)					//�L�[��

//=========================
// �O���錾
//=========================
class CParts;

//=========================
// �v���C���[�N���X
//=========================
class CMotion
{
public:
	//====================
	// �L�[�̍\����
	//====================
	struct KEY
	{
		float fPosX;		//�ʒuX
		float fPosY;		//�ʒuY
		float fPosZ;		//�ʒuZ
		float fRotX;		//�ʒuX
		float fRotY;		//�ʒuY
		float fRotZ;		//�ʒuZ

		KEY(): fPosX(0.0f), fPosY(0.0f) , fPosZ(0.0f) , fRotX(0.0f) , fRotY(0.0f) , fRotZ(0.0f) {};
	};

	//====================
	// �L�[���̍\����
	//====================
	struct KEY_INFO
	{
		int nFrame;			//�Đ��t���[��
		KEY aKey[MAX_PARTS];		//�e���f���̃L�[�v�f
	};

	//====================
	// ���[�V�������̍\����
	//====================
	struct INFO
	{
		bool bLoop;				//���[�v���邩�ǂ���
		int nNumKey;			//�L�[�̑���
		KEY_INFO aKeyInfo[NUM_KEY];	//�L�[���
	};

	CMotion();		//�R���X�g���N�^
	~CMotion();		//�f�X�g���N�^

	HRESULT Init(void);						// ������(�f�t�H)
	void Uninit(void);						// �I��
	void Update(void);						// �X�V
	void Set(int nType);															// ���[�V�����̋N��
	void Set(int nType, int nBrendCtr) { m_nBrendCtr = nBrendCtr; Set(nType); }		// ���[�V�����̋N��(�u�����h)

	// �X�V
	void UpdateParts(int nNextKey);		// �p�[�c�̍X�V

	int GetType(void);						// ���݂̃��[�V�����̎�ގ擾
	bool IsFinish(void);					// �I������擾
	void SetInfo(INFO *info, int nNum);				// ���[�V�������ݒ�

	void SetModel(CParts **ppParts, int nNumModel);		// ���f�����ݒ�
	//void SetOrg(CParts **ppParts, int nNumModel);		// �I�t�Z�b�g���ݒ�

	char **ReadParts(const char *pReadFile, int *pnNumFile);
	void ReadSetUp(const char *pReadFile, CParts **ppParts);
	void ReadMotions(const char *pReadFile, CMotion::INFO *pInfo);

	int GetKey(void) { return m_nKey; }				// ���݂̃L�[�擾
	int GetKeyOld(void) { return m_nKeyOld; }		// �O��̃L�[�擾
	INFO *GetInfo(void) { return m_aInfo; }			// ���݂̃L�[���擾

	int GetCounter() { return m_nCounter; }			// ���݃J�E���g�擾
	int GetTotalCtr() { return m_nTotalCtr; }		// �g�[�^���J�E���g�擾
	
protected:

private:
	void ReadFile(char* pFilePass);	// �t�@�C���Ǎ�
	void KeySetBrend();			// �L�[�ݒ�(�u�����h)
	void KeySetNormal();		// �L�[�ݒ�(�L�[�X�V)

	KEY m_aKeyOrg[MAX_PARTS];	// �e���f���̃v���Z�b�g���
	KEY m_aKeyOld[MAX_PARTS];	// �e���f���̃v���Z�b�g���(�ߋ�)
	INFO m_aInfo[NUM_MOTION];	// ���[�V�������
	int m_nNumAll;				// ���[�V�����̑���
	int m_nType;				// ���[�V�����̎��
	int m_nTypeOld;				// ���[�V�����̎��(�O��)
	int m_nCounter;				// ���[�V�����J�E���^�[
	int m_nNumKey;				// �L�[�̑���
	int m_nKey;					// ���݂̃L�[No.
	int m_nKeyOld;				// �O��̃L�[No.
	bool m_bLoop;				// ���[�v���邩�ǂ���
	bool m_bFinish;				// �I���������ǂ���
	int m_nTotalCtr;			// 1�̃g�[�^���J�E���^�[
	int m_nBrendCtr;			// �u�����h�J�E���^�[

	CParts **m_ppParts;	// ���f���ւ̃|�C���^
	int m_nNumModel;	// ���f���̑���
};

#endif
