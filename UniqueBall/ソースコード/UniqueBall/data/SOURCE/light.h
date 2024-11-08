//==============================================
//
//���C�g(light.h)
// Author: Kai Takada
//
//==============================================
#ifndef _LIGHT_H_								//���̃}�N����`������ĂȂ�������
#define _LIGHT_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//============================
// �}�N����`
//============================
const int MAX_LIGHT = 3;

//============================
// ���C�g�N���X
//============================
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_light[MAX_LIGHT];		//���C�g�̏��
	int m_nDebugNum;	//�f�o�b�O�Ώ�
};

#endif
