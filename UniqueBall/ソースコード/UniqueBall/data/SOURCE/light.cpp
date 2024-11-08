//============================
//
//���C�g(light.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "light.h"
#include "debugproc.h"
#include "input.h"

//============================
// �R���X�g���N�^
//============================
CLight::CLight()
{
	m_nDebugNum = 0;
}

//============================
// �f�X�g���N�^
//============================
CLight::~CLight()
{

}

//============================
//���C�g�̏���������
//============================
HRESULT CLight::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir;		//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{//���C�g�̊g�U�Z�ƕ�����ݒ�
		case 0:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(-1.87, -1.57, 2.65);
			break;

		case 1:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(-1.87, -1.57, -2.65);
			break;

		case 2:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(3.03f, -1.57f, -2.57f);
			break;

		default:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(0.0f, -0.0f, 0.0f);
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g���𐳋K������(1.0f�ɂ���)
		m_light[nCntLight].Direction = vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//============================
//���C�g�̏I������
//============================
void CLight::Uninit(void)
{

}

//============================
//���C�g�̍X�V����
//============================
void CLight::Update(void)
{
	//�f�o�b�O
#if _DEBUG
	D3DXVECTOR3 pos = m_light[m_nDebugNum].Direction;
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();

	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_RALT))
	{
		m_nDebugNum++;
		m_nDebugNum %= MAX_LIGHT;
	}
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		pos.z += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		pos.z -= 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		pos.x += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		pos.x -= 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_RSHIFT))
	{
		pos.y += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_RCONTROL))
	{
		pos.y -= 0.01f;
	}

	pos.x = mylib_useful::NormalizeRot(pos.x);
	pos.y = mylib_useful::NormalizeRot(pos.y);
	pos.z = mylib_useful::NormalizeRot(pos.z);

	m_light[m_nDebugNum].Direction = pos;

	//���C�g��ݒ肷��
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->SetLight(m_nDebugNum, &m_light[m_nDebugNum]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(m_nDebugNum, TRUE);

#endif
}
