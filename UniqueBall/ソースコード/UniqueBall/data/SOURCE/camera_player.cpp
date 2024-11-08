//============================
//
//�J�����̏���(camera.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "camera_player.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"

//============================
// �萔��`
//============================
namespace
{
	const DWORD HEIGHT = static_cast<DWORD>(SCREEN_HEIGHT * 0.5f);
	const DWORD WIDTH = static_cast<DWORD>(SCREEN_WIDTH * 0.5f);
	//const float CAMERA_LIMIT_Z(D3DX_PI * 0.48f);	//z����]�̌��E�l
	const float CAMERA_LIMIT_Z(D3DX_PI * 0.25f);	//z����]�̌��E�l

	const float KEYMOUSE_INPUT_VALUE(0.01f);	//�L�[�}�E�̃C���v�b�g�l
}

//============================
// �R���X�g���N�^
//============================
CCameraPlayer::CCameraPlayer()
{
	m_nIdx = -1;
	m_pTarget = nullptr;
}

//============================
// �f�X�g���N�^
//============================
CCameraPlayer::~CCameraPlayer()
{

}

//============================
//�J�����̏���������
//============================
HRESULT CCameraPlayer::Init()
{
	CCamera::Init();

	m_nIdx = -1;
	m_viewport.Height = HEIGHT;
	m_viewport.Width = WIDTH;
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	return S_OK;
}

//============================
//�J�����̏I������
//============================
void CCameraPlayer::Uninit()
{
	CCamera::Uninit();
}

//============================
//�J�����̍X�V����
//============================
void CCameraPlayer::Update()
{
	if (m_pTarget != nullptr)
	{
		if (m_pTarget->GetDeath())
		{//�폜�O�ɉ���
			m_pTarget = nullptr;
		}
	}

	//�ǂ����̃��[�h�H
	if (m_pTarget != nullptr)
	{
		FocusOnTarget();
	}
	else
	{
		PlayerStalk();
	}

	Debug();
}

//============================
//�J�����̐ݒ菈��
//============================
void CCameraPlayer::SetCamera()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxProjection = GetMtxProj(); //�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView = GetMtxView();	//�r���[�}�g���b�N�X
	D3DXVECTOR3 posV = GetPosV();	//���_
	D3DXVECTOR3 posR = GetPosR(); //�����_
	D3DXVECTOR3 vecU = GetVecU(); //������x�N�g��

	//�r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&m_viewport);

	//�o�b�t�@���N���A
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬[�������e]
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
								D3DXToRadian(45.0f),	//����
								(float)m_viewport.Width / (float)m_viewport.Height,	//�c����
								10.0f,	//�ŏ�����
								40000.0f);	//�ő勗��

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION,&mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView,
						&posV,			//���_
						&posR,			//�����_
						&vecU);		//������x�N�g��

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	////�o�b�t�@���N���A
	//pDevice->Clear(0, NULL,
	//	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
	//	D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
}

//============================
// �`���ʈʒu�ݒ�
//============================
void CCameraPlayer::SetViewPos()
{
	m_viewport.Height = static_cast<DWORD>(SCREEN_HEIGHT / 1.0f);
	m_viewport.Width = static_cast<DWORD>(SCREEN_WIDTH / 2);

	if (m_nIdx == 0)
	{
		m_viewport.X = 0;
		m_viewport.Y = 0;
	}
	else
	{
		m_viewport.X = static_cast<DWORD>(SCREEN_WIDTH / 2);
		m_viewport.Y = 0;
		//m_viewport.Y = static_cast<DWORD>(SCREEN_HEIGHT / 2);
	}
}

//============================
// �v���C���[�Ǐ]����(������)
//============================
void CCameraPlayer::PlayerStalk(void)
{
	CPlayer *pPlayer = CScene::GetElem(m_nIdx);

	if (pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 pos = pPlayer->GetPos();
	pos.y += pPlayer->GetHeartPos();

	D3DXVECTOR3 posR = GetPosR();
	D3DXVECTOR3 posV = GetPosV();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 rotDest = GetRotDest();
	float fDis = GetDis();

	//�v���C���[�̌�����Ǐ]����
	rotDest.y = pPlayer->GetRot().y;

	//�����_
	posR.x = pos.x;
	posR.z = pos.z;
	posR.y = pos.y;

	//���_
	posV.x = posR.x + sinf(rot.y + D3DX_PI) * cosf(rot.z) * -fDis;
	posV.y = posR.y + sinf(rot.z) * -fDis;
	posV.z = posR.z + cosf(rot.y + D3DX_PI) * cosf(rot.z) * -fDis;

	//�J����
	RotControll();

	SetPosR(posR);
	SetPosV(posV);
	pPlayer->SetRot(D3DXVECTOR3(rot.x, rot.y,0.0f));
}

//===========================
// �I�𒍎��_�ɑ��삷��
//===========================
void CCameraPlayer::FocusOnTarget()
{
	if (m_pTarget == nullptr)
	{
		return;
	}

	//���g
	CPlayer* pPlayer = CScene::GetElem(m_nIdx);

	if (pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posP = pPlayer->GetPosCol();
	D3DXVECTOR3 pos = m_pTarget->GetPos();
	posP.y += pPlayer->GetHeartPos();
	pos.y += m_pTarget->GetHeartPos();
	D3DXVECTOR3 rotP = pPlayer->GetRot();

	D3DXVECTOR3 posR = GetPosR();
	D3DXVECTOR3 posV = GetPosV();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 rotDest = GetRotDest();
	float fDis = GetDis();
	float fAngle = GetAngle();

	//�v���C���[�̌�����Ǐ]����
	rotDest.y = pPlayer->GetRot().y;

	//�����_
	posR.x = pos.x;
	posR.z = pos.z;
	posR.y = pos.y;

	//���_
	posV.x = posP.x + sinf(rotP.y + D3DX_PI) * cosf(rotP.z) * -fDis;
	posV.y = posP.y + sinf(rotP.z) * -fDis;
	posV.z = posP.z + cosf(rotP.y + D3DX_PI) * cosf(rotP.z) * -fDis;

	SetPosR(posR);
	SetPosV(posV);
	SetRotDest(rotDest);
	pPlayer->SetRot(D3DXVECTOR3(rot.x, rot.y, 0.0f));
}

//===========================
//PAD�Ō����𒲐�
//===========================
void CCameraPlayer::RotControll()
{
	//�E�X�e�B�b�N
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	bool bInput = false;

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = mylib_const::DEFVEC3;

	if (CPlayer::GetControlPlayer() == m_nIdx)
	{
		D3DXVECTOR3 MousePos = pInputMouse->GetChangePos();
		rot.y += MousePos.x * 0.01f;
		rot.z += MousePos.y * -0.01f;

		if (pInputKeyboard->GetPress(DIK_UP))		rot.z += KEYMOUSE_INPUT_VALUE;
		if (pInputKeyboard->GetPress(DIK_DOWN))		rot.z -= KEYMOUSE_INPUT_VALUE;
		if (pInputKeyboard->GetPress(DIK_RIGHT))	rot.y += KEYMOUSE_INPUT_VALUE;
		if (pInputKeyboard->GetPress(DIK_LEFT))		rot.y -= KEYMOUSE_INPUT_VALUE;
	}

	rot.y += float(float(pInputGamepad->GetGameStickRXPress(m_nIdx)) / float(SHRT_MAX)) * 0.05f;
	rot.z += float(float(pInputGamepad->GetGameStickRYPress(m_nIdx)) / float(SHRT_MAX)) * 0.05f;
	
	if (rot.y < -D3DX_PI)
	{//�������
		rot.y = D3DX_PI;
	}
	else if (rot.y > D3DX_PI)
	{
		rot.y = -D3DX_PI;
	}
	//�^�㉺�M���M���Ŕ��]�O�ɃX�g�b�v
	if (rot.z < -CAMERA_LIMIT_Z)
	{
		rot.z = -CAMERA_LIMIT_Z;
	}
	else if (rot.z > CAMERA_LIMIT_Z)
	{
		rot.z = CAMERA_LIMIT_Z;
	}

	SetRot(rot);
}

//===========================
// �f�o�b�O
//===========================
void CCameraPlayer::Debug()
{
	//�f�o�b�O
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- �J������� ---\n");
	pDebug->Print("���݂̕���:%f %f %f\n", GetRot().x, GetRot().y, GetRot().z);
	pDebug->Print("�^�[�Q�b�g:%d\n", (m_pTarget != nullptr));
}
