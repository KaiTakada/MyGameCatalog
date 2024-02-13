//==================================================
//
// �u���b�N����(block.h)
// Author : Kai Takada
//
//==================================================
#ifndef _BLOCK_H_								//���̃}�N����`������ĂȂ�������
#define _BLOCK_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//============================================
// �C���N���[�h�t�@�C��
//============================================
#include "objectX.h"

//============================================
//�}�N����`
//============================================
#define MAX_BLOCK (256)						//�u���b�N�̍ő吔
#define BLOCK_WIDTH (80.0f)						//�u���b�N�̕�
#define BLOCK_HEIGHT (80.0f)					//�u���b�N�̍���

//============================================
// �O���錾
//============================================

//============================================
// �u���b�N�N���X
//============================================
class CBlock : public CObjectX
{
public:
	//===================================
	// �u���b�N�񋓌^�̒�`
	//===================================
	typedef enum
	{
		TYPE_NORMAL = 0,		//�ʏ�̃u���b�N
		TYPE_MAX,
	}TYPE;

	CBlock(int nPriority = 1);
	~CBlock();

	HRESULT Init(void);			//������
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//������(�I�o��)
	void Uninit(void);			//�I��
	void Update(void);			//�X�V
	void Draw(void);			//�`��

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), TYPE type = TYPE_NORMAL);		//����
	bool CollisionRect(void);		//�����蔻��
	static int GetNumAll(void) { return m_nNumAll; }		//�����擾
	bool GetJump(void) { return false; }		//�W�����v����
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }		//�O��ʒu�擾

protected:

private:
	static int m_nNumAll;											//���ݎg���Ă���u���b�N��

	D3DXVECTOR3 m_posOld;			//�O��̈ʒu
	CBlock::TYPE m_type;			//���
};

#endif
