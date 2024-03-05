//============================
//
//�J�����̏���(camera.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "cursor.h"
#include "block.h"

//============================
// �萔��`
//============================
namespace
{
	const float CAMERA_SPEED(10.0f);				//�ړ��X�s�[�h
	const float CAMERA_ROTSPEED(0.01f);				//��]�X�s�[�h
	const float CAMERA_DISTANCE(350.0f);			//���_�E�����_�̋���
	const float CAMERA_INIT_POS_Y(100.0f);			//���_�E�����_��y�����l
	const float CAMERA_RADJT(400.0f);				//�����_�̂��炵�l
	const float CAMERA_VISDISTANCE(40000.0f);		//�������̒l
	const float CAMERA_LIMIT_Z(D3DX_PI * 0.48f);	//z����]�̌��E�l
	const float CAMERA_VR_ROT(0.85f);		//���_�ƒ����_�̊p�x
	const float CAMERA_Y_STALK(300.0f);		//�v���C���\��y�l�ɍ��킹�ď�������X�^�[�g�l
	const float POSR_MOVE = 0.005f;			//�}�E�X�̈ړ��ʂƊ|����l
}

//============================
// �R���X�g���N�^
//============================
CCamera::CCamera()
{
	m_mtxView = {};
	m_mtxProjection = {};
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = m_posV;
	m_posRDest = m_posR;
	m_posRAdjt = 0.0f;
	m_fDis = 0.0f;
	m_fAngle = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
// �f�X�g���N�^
//============================
CCamera::~CCamera()
{

}

//============================
//�J�����̏���������
//============================
HRESULT CCamera::Init()
{
	m_fDis = CAMERA_DISTANCE;
	m_posV = D3DXVECTOR3(0.0f, CAMERA_INIT_POS_Y, 0.0f - m_fDis);
	m_posR = D3DXVECTOR3(0.0f, CAMERA_INIT_POS_Y * 0.5f, 0.0f);
	m_posVDest = m_posV;
	m_posRDest = m_posR;
	m_posRAdjt = CAMERA_RADJT;
	m_fAngle = D3DX_PI * CAMERA_VR_ROT;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//============================
//�J�����̏I������
//============================
void CCamera::Uninit()
{

}

//============================
//�J�����̍X�V����
//============================
void CCamera::Update()
{
	PlayerStalk();
}

//============================
//�J�����̐ݒ菈��
//============================
void CCamera::SetCamera()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬[�������e]
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,
								CAMERA_VISDISTANCE);

	////�v���W�F�N�V�����}�g���b�N�X�̍쐬[���s���e]-2d���ۂ�
	//D3DXMatrixOrthoLH(&m_mtxProjection,
	//				(float)SCREEN_WIDTH,
	//				(float)SCREEN_HEIGHT,
	//				10.0f,
	//				CAMERA_VISDISTANCE);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION,&m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
						&m_posV,			//���_
						&m_posR,			//�����_
						&m_vecU);		//������x�N�g��

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//===========================
// Dest�ɒǂ���������
//===========================
void CCamera::ReviseRot()
{
	//����
	float fDiff = 0.0f;
	fDiff = m_rotDest.y - m_rot.y;

	//PI����͂ݏo�Ă����ꍇ�̕␳
	if (fDiff < -D3DX_PI)
	{
		fDiff += D3DX_PI * 2;
	}
	else if (fDiff > D3DX_PI)
	{
		fDiff += -D3DX_PI * 2;
	}
	
	//m_rot.y += fDiff * 0.01f;
	m_rot.y += fDiff * 1.0f;

	//PI����͂ݏo�Ă����ꍇ�̕␳
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y += -D3DX_PI * 2;
	}
}

//============================
// �G�f�B�b�g�J�[�\���Ǐ]����
//============================
void CCamera::EditStalk(void)
{
	CMap *pMap = CGame::GetMap();

	if (pMap == nullptr)
	{
		return;
	}

	CBlock *pBlock = pMap->GetCursor();

	if (pBlock == nullptr)
	{
		return;
	}

	D3DXVECTOR3 pos = pBlock->GetPos();
	m_posR.x = pos.x;
	m_posR.z = pos.z;
	m_posR.y = pos.y;

	m_posV.x = pos.x;
	m_posV.y = pos.y + (sinf(m_fAngle) * m_fDis);
	m_posV.z = pos.z + (cosf(m_fAngle) * m_fDis);
}

//============================
// �v���C���[�Ǐ]����(������)
//============================
void CCamera::PlayerStalk(void)
{
	CPlayer *pPlayer = CScene::GetPlayer(0);

	if (pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 pos = pPlayer->GetPos();
	pos.y += pPlayer->GetHeartPos();
	D3DXVECTOR3 rot = pPlayer->GetRot();

	//�v���C���[�̌�����Ǐ]����
	m_rotDest.y = pPlayer->GetRot().y;

	//�����_
	m_posR.x = pos.x;
	m_posR.z = pos.z;
	m_posR.y = pos.y;
	//m_posR.x = pos.x + (sinf(m_rot.y) * m_fDis);
	//m_posR.z = pos.z + (cosf(m_rot.y) * m_fDis);
	//m_posR.y = pos.y;

	//���_
	m_posV.x = m_posR.x + (sinf(m_rot.y) * m_fDis);
	m_posV.y = m_posR.y + (sinf(m_fAngle) * m_fDis);
	m_posV.z = m_posR.z + (cosf(m_rot.y) * m_fDis);

	RotControll();

	////�����_��������
	//RotHorPosR();

	////�����_��������
	//RotVerPosR();
	
	////�␳
	//ReviseRot();

	//�v���C���[�Ɍ����𔽉f
	pPlayer->SetRot(m_rotDest);
}

//===========================
//�����_��������
//===========================
void CCamera::RotHorPosR()
{
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	//�u�[�X�gor�W�����v
	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
	}

	D3DXVECTOR3 mouseMove = pInputMouse->GetChangePos();
	m_rot.y += mouseMove.x * POSR_MOVE;

	//���_�̐���
	m_posR.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.z) * -m_fDis;
	m_posR.y = m_posV.y + sinf(m_rot.z) * -m_fDis;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.z) * -m_fDis;


	if (m_rot.y < -D3DX_PI)
	{//�������
		m_rot.y = D3DX_PI;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}
}

//===========================
//�����_��������
//===========================
void CCamera::RotVerPosR()
{
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	//�u�[�X�gor�W�����v
	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
	}

	D3DXVECTOR3 mouseMove = pInputMouse->GetChangePos();
	m_rot.z += mouseMove.y * POSR_MOVE;

	//�����_�̐���
	m_posR.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.z) * -m_fDis;
	m_posR.y = m_posV.y + sinf(m_rot.z) * -m_fDis;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.z) * -m_fDis;

	//�^�㉺�M���M���Ŕ��]�O�ɃX�g�b�v
	if (m_rot.z < -CAMERA_LIMIT_Z)
	{
		m_rot.z = -CAMERA_LIMIT_Z;
	}
	else if (m_rot.z > CAMERA_LIMIT_Z)
	{
		m_rot.z = CAMERA_LIMIT_Z;
	}
}

//===========================
//�}�E�X�Ō����𒲐�
//===========================
void CCamera::RotControll()
{
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	bool bInput = false;

	if (pInputGamepad->GetGameStickLYPress(0) < 0)
	{
	}
	else if (pInputGamepad->GetGameStickLYPress(0) > 0)
	{
	}
	else if (pInputGamepad->GetGameStickLXPress(0) < 0)
	{//���L�[�݂̂��������Ƃ�
	}
	else if (pInputGamepad->GetGameStickLXPress(0) > 0)
	{//�E�L�[�݂̂��������Ƃ�
	}

	D3DXVECTOR3 mouseMove = pInputMouse->GetChangePos();
	m_rot.y += mouseMove.x * POSR_MOVE;
	m_rot.z += mouseMove.y * POSR_MOVE;

	if (m_rot.y < -D3DX_PI)
	{//�������
		m_rot.y = D3DX_PI;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}
	//�^�㉺�M���M���Ŕ��]�O�ɃX�g�b�v
	if (m_rot.z < -CAMERA_LIMIT_Z)
	{
		m_rot.z = -CAMERA_LIMIT_Z;
	}
	else if (m_rot.z > CAMERA_LIMIT_Z)
	{
		m_rot.z = CAMERA_LIMIT_Z;
	}
}
