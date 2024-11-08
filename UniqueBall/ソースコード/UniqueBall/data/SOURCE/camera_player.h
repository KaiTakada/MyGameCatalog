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
// �O���錾
//===============================
class CObjectCharacter;

//===============================
// �J�����N���X
//================================
class CCameraPlayer : public CCamera
{
public:
	CCameraPlayer();
	~CCameraPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
	void SetIdx(int nIdx) { m_nIdx = nIdx; }	//�ԍ��ݒ�
	void SetViewPos();	//�ʒu�ݒ�
	void PlayerStalk();
	void RotControll();
	D3DVIEWPORT9 GetViewPort() { return m_viewport; }	//�r���[�|�[�g�擾
	void FocusOnTarget();
	void SetTarget(CObjectCharacter *pChara) { m_pTarget = pChara; }

private:
	void Debug();
		
	D3DVIEWPORT9 m_viewport;	//�r���[�|�[�g
	int m_nIdx;	//�ԍ�
	CObjectCharacter* m_pTarget;
};

#endif
