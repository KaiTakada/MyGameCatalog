//==============================================
//
// �Q�[���p�J�����̏���(camera_game.h)
// Author: Kai Takada
//
//==============================================
#ifndef _CAMERA_GAME_H_								//���̃}�N����`������ĂȂ�������
#define _CAMERA_GAME_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//===============================
// �C���N���[�h�t�@�C��
//================================
#include "camera.h"

//===============================
// �J�����N���X
//================================
class CCameraGame : public CCamera
{
public:
	CCameraGame();
	~CCameraGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
	void SetIdx(int nIdx) { m_nIdx = nIdx; }	//�ԍ��ݒ�
	void SetViewPos();	//�ʒu�ݒ�
	void PlayerStalk();
	void RotControll();

protected:

private:
	D3DVIEWPORT9 m_viewport;	//�r���[�|�[�g
	int m_nIdx;	//�ԍ�
};

#endif
