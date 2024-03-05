//===============================================
//
// �v���C���[(player.cpp)
// Author ���c ����
//
//===============================================

//============================
// �C���N���[�h�t�@�C��
//============================
#include "player.h"
#include "manager.h"	//�قڊm�ŕK�v
#include "game.h"		//�قڊm�ŕK�v
#include "input.h"			//����
#include "camera.h"			//�ړ�����
#include "camera_game.h"		//�ړ�����
#include "sound.h"			//���o���ɕK�v
#include "parts.h"			//�e�q�֌W
#include "motion.h"			//���[�V����
#include "enemy.h"			//�^�b�N���̓����蔻��
#include "Field.h"			//�n�ʂƂ̓����蔻��
#include "wall.h"			//�ǂƂ̓����蔻��
#include "wp_ball.h"		//��
#include "gauge.h"			//HP�Q�[�W
#include "Xmodel.h"			//���f��
#include "debugproc.h"		//�f�o�b�O
#include "state_life.h"		//��ԊǗ�
#include "ef_smoke.h"		//�����o
#include "wp_ball.h"		//�{�[���̓����蔻��
#include "ui_life.h"		//�̗�UI
#include "objectBillboard.h"		//�`���[�W���o
#include "texture.h"		//�`���[�W���o
#include "Anim.h"		//�`���[�W���o

//�f�o�b�O�p(�����炭����)
#include "bullet.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// �}�N����`
//============================
namespace
{
	const float NUM_GETAREA(100.0f);		//�擾�̈�̔��a
	const float NUM_HYGETAREA(100.0f);		//�擾�����̈�̔��a
	const float NUM_GETAREA_SPEED(8.0f);	//�擾�̈�̈����񂹑��x
	const float NUM_COLL(30.0f);			//�����蔻�蔼�a
	const float NUM_ATTACK(1.0f);			//�U����
	const float NUM_SPEED(1.0f);			//�ړ���
	const float NUM_SPEED_STOP(0.0001f);	//�ړ��ʂ�0�ɂ�������l
	const float NUM_HEART(100.0f);			//�S���ʒu
	const float NUM_JUMP(15.0f);			//�W�����v��
	const float NUM_BOOST(40.0f);			//�u�[�X�g��
	const float NUM_GRAV(0.5f);				//�d��
	const float MAX_THROW_CHARGE(0.5f);			//�`���[�W���
	const float ADD_THROW_CHARGE(0.01f);		//�`���[�W
	const int HANDR_IDX = 12;			//�E��Idx
	const int READ_PSIZE(256);			//�Ǎ��|�C���^�T�C�Y
	const int DAMAGE_CT(30);			//�_���[�W���G����
	const int WAIT_JC(-3);			//�W���X�g�L���b�`�P�\����
	const int CT_CATCH(30);			//�L���b�`CT
	const int CT_TACKLE(30);			//�^�b�N��CT
	const int CT_JUMP2(30);			//2�i�W�����vCT
	const int NUM_HP(2);			//HP��
	const char* MOTION_FILE = "data\\SET\\MOTION\\motion_player.txt";		//���[�V�����t�@�C���p�X

	const char* UV_FILE_BODY[mylib_const::MAX_PLAYER] =
	{
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
		"data\\TEXTURE\\PLAYER\\human_body_use1.png",
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
	};		//UV�e�N�X�`���p�X
}

//============================
// �萔��`
//============================

//============================
// �ÓI�����o�ϐ��錾
//============================

//============================
// �R���X�g���N�^
//============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	m_nNumModel = 0;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = MAX_PLAYER_SIZE;
	m_posOld = m_pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = m_rot;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_fHeart = 0.0f;
	m_bJump = false;
	m_pMotion = nullptr;
	ZeroMemory(&m_param, sizeof(m_param));
	m_pStateLife = nullptr;

	m_pBall = nullptr;
	m_pLifeUI = nullptr;

	for (int nCntPart = 0; nCntPart < MAX_PARTS; nCntPart++)
	{
		m_apPart[nCntPart] = nullptr;
	}

	m_eMember = my_Identity::MEMBER_NONE;
	m_nCatchCtr = 0;
	m_nTackleCtr = 0;
	m_fThrowChargeCtr = 0.0f;
	m_nJump2Ctr = 0;
	m_nIdx = 0;		//���g�̔ԍ�

	m_bDelete = false;
	m_bTarget = false;
	m_pTargetMark = nullptr;
}

//============================
// �f�X�g���N�^
//============================
CPlayer::~CPlayer()
{

}

//====================================
// �v���C���[�̏�����(�f�t�H)
//====================================
HRESULT CPlayer::Init(void)
{
	m_pos = mylib_const::DEFVEC3;
	m_posOld = m_pos;
	m_size = MAX_PLAYER_SIZE;
	m_rot = mylib_const::DEFVEC3;
	m_rotDest = m_rot;
	m_nNumModel = 0;
	m_fHeart = NUM_HEART;
	m_param.nLife = NUM_HP;
	m_param.nLifeMax = NUM_HP;
	m_param.fAttack = NUM_ATTACK;
	m_param.fSpeed = NUM_SPEED;
	m_param.fColl = NUM_COLL;

	//���[�V�����̐����A������
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//�t�@�C���Ǎ����p�[�c�����E������
	ReadFile();

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	//��Ԃ̐���
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//�̗�UI�̐���
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->Uninit();
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(m_param.nLifeMax, m_nIdx);

	//�^�C�v�ݒ�
	SetType(TYPE_PLAYER);

	return S_OK;
}

//====================================
// �v���C���[�̏�����(�ʒu�I�o��)
//====================================
HRESULT CPlayer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nNumParts)
{
	m_pos = pos;
	m_posOld = pos;
	m_size = MAX_PLAYER_SIZE;
	//m_rot = rot;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_rotDest = rot;
	m_nNumModel = nNumParts;
	m_fHeart = NUM_HEART;
	m_param.nLife = NUM_HP;
	m_param.nLifeMax = NUM_HP;
	m_param.fAttack = NUM_ATTACK;
	m_param.fSpeed = NUM_SPEED;
	m_param.fColl = NUM_COLL;

	//���[�V�����̐����A������
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//�t�@�C���Ǎ����p�[�c�����E������
	ReadFile();

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	//��Ԃ̐���
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//�̗�UI�̐���
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->Uninit();
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(m_param.nLifeMax, m_nIdx);

	//�^�C�v�ݒ�
	SetType(TYPE_PLAYER);

	return S_OK;
}

//============================
// �v���C���[�̏I��
//============================
void CPlayer::Uninit(void)
{
	for (int nCntPrt = 0; nCntPrt < MAX_PARTS; nCntPrt++)
	{
		if (m_apPart[nCntPrt] != nullptr)
		{
			m_apPart[nCntPrt]->SetDeath(true);
			m_apPart[nCntPrt] = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pBall != nullptr)
	{
		m_pBall->Uninit();
		m_pBall = nullptr;
	}

	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	//�I�u�W�F�N�g
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->SetDeath(true);
		m_pLifeUI = nullptr;
	}

	//�I��
	Release();
}

//============================
// �v���C���[�̍X�V
//============================
void CPlayer::Update(void)
{
	//���W�擾
	float fRotMove = m_rot.y;	//���݂̌���
	float fRotDest = m_rot.y;	//�ڕW�̌���
	float fRotDiff = 0.0f;			//����

	D3DXVECTOR3 pos = GetPos();
	m_posOld = pos;

	//�C���v�b�g�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	D3DXVECTOR3 cameraRot = m_rot;
	CScene *pScene = CManager::GetInstance()->GetScene();
	if (pScene->GetMode() == CScene::MODE_GAME)
	{
		CGame *pGame = static_cast<CGame*>(pScene);
		CCameraGame *pCameraGame = pGame->GetCamera(m_nIdx);
		cameraRot = pCameraGame->GetRot();
	}

	//�ړ�����
	MoveOperate(&fRotDest);
	RotOperate(&fRotDest);

	if (!m_pMotion->IsFinish())
	{
		m_pMotion->Update();
	}

	fRotDiff = fRotDest - fRotMove;
	RotAdj(fRotDiff);

	m_rotDest.y = fRotDiff;

	//2�i�W�����vor�W�����v
	if (pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, m_nIdx))
	{//[ - ]�L�[

		if (m_bJump == true
			&& m_pMotion->GetType() != MOTIONTYPE_JUMP2
			&& m_pMotion->GetType() != MOTIONTYPE_TACKLE
			&& m_nJump2Ctr == 0)
		{//�W�����v�g�p�ς�
			m_bJump = true;
			m_move.y = NUM_JUMP * 0.5f;
			m_pMotion->Set(MOTIONTYPE_JUMP2);

			//���o
		}
		else if (m_bJump == false)
		{//�W�����v���g�p
			m_bJump = true;
			m_move.y += NUM_JUMP;
			m_pMotion->Set(MOTIONTYPE_JUMP);

			//�����o
			CEf_Smoke * pSmoke = CEf_Smoke::Create(m_pos);
			pSmoke->SetLife(10);
			pSmoke->SetLifeMax(10);
		}
	}

	//�^�b�N��
	if ((pInputKeyboard->GetTrigger(DIK_LSHIFT) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON_Y, m_nIdx))
		 && m_pMotion->GetType() != MOTIONTYPE_TACKLE
		&& m_nTackleCtr == 0)
	{//[]�L�[�������W�����v���Ă��Ȃ�
		float fStickL = pInputGamepad->GetStickLRot(m_nIdx);
		m_move.x = sinf(GetRot().y + fStickL + D3DX_PI * 1.0f) * NUM_BOOST;		//x
		m_move.z = cosf(GetRot().y + fStickL + D3DX_PI * 1.0f) * NUM_BOOST;		//z
		m_pMotion->Set(MOTIONTYPE_TACKLE);
	
		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_TACKLE);
	}

	//�����\��
	if ((pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_RB, m_nIdx))
		&& (m_pMotion->GetType() != MOTIONTYPE_THROW
			&& m_pMotion->GetType() != MOTIONTYPE_TACKLE
			&& m_pMotion->GetType() != MOTIONTYPE_CATCH_RDY))
	{//[Rctrl]�L�[�����������Ă��Ȃ�

		if (m_pBall != nullptr)
		{
			if (m_pMotion->GetType() == MOTIONTYPE_THROW_RDY &&
				m_fThrowChargeCtr < MAX_THROW_CHARGE)
			{//�`���[�W
				m_pBall->AddSpeedMag(ADD_THROW_CHARGE);
				m_fThrowChargeCtr += ADD_THROW_CHARGE;
			}

			m_pMotion->Set(MOTIONTYPE_THROW_RDY);
		}
	}

	//����
	if ((pInputMouse->GetRelease(CInputMouse::BUTTON_LEFT) ||
		pInputGamepad->GetRelease(CInputGamepad::BUTTON_RB, m_nIdx)
		&& m_pMotion->GetType() == MOTIONTYPE_THROW_RDY))
	{
		if (m_pBall != nullptr)
		{
			Throw();
		}
	}

	//�L���b�`�\��
	if ((pInputMouse->GetTrigger(CInputMouse::BUTTON_RIGHT) ||
		pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LB, m_nIdx))
		&& (m_pMotion->GetType() != MOTIONTYPE_CATCH_RDY)
		&& (m_pMotion->GetType() != MOTIONTYPE_TACKLE)
		&& m_nCatchCtr == 0)
	{
		//�o���A����
		m_pMotion->Set(MOTIONTYPE_CATCH_RDY);
		m_nCatchCtr = WAIT_JC;
	}

	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_X, m_nIdx))
	{
		m_bTarget = !m_bTarget;
	}

	CollisionBall(pos);
	DebugKey(pInputKeyboard);

	//#if _DEBUG
	//	DebugKey(pInputKeyboard);
	//#endif

	CPlayer::MOTIONTYPE Mtype = MOTIONTYPE(m_pMotion->GetType());

	if (Mtype != MOTIONTYPE_TACKLE)
	{
		//�T�C�Y�ɒʏ�ɏC��
		m_size = MAX_PLAYER_SIZE;
	}
	else
	{//�X���C�f�B���O����������

	}

	MotionEffect();
	ReduceCounter();

	//�d��
	float fGrav = 1.0f;
	if (Mtype == MOTIONTYPE_JUMP2)
	{
		fGrav = 0.5f;
	}

	m_move.y -= NUM_GRAV * fGrav;

	//�ړ��ʂ�K�p
	pos += m_move;

	//�������擾����
	CollisionField(pos);
	CollisionWall();
	Target();

	if (Mtype == MOTIONTYPE_TACKLE)
	{
		TackleCollision();
	}

	//�x�^�ł���
	if (pos.x > mylib_const::DEF_FIELD_SIZE.x - NUM_COLL)
	{
		pos.x = mylib_const::DEF_FIELD_SIZE.x - NUM_COLL;
	}
	else if (pos.x < -mylib_const::DEF_FIELD_SIZE.x + NUM_COLL)
	{
		pos.x = -mylib_const::DEF_FIELD_SIZE.x + NUM_COLL;
	}
	if (pos.z > mylib_const::DEF_FIELD_SIZE.z - NUM_COLL)
	{
		pos.z = mylib_const::DEF_FIELD_SIZE.z - NUM_COLL;
	}
	else if (pos.z < -mylib_const::DEF_FIELD_SIZE.z + NUM_COLL)
	{
		pos.z = -mylib_const::DEF_FIELD_SIZE.z + NUM_COLL;
	}

	//�������~��
	if (pos.y < -50.0f)
	{
		pos.y = -50.0f;
		m_move.y = 0.0f;
		m_bJump = false;
		//static_assert(true, "��");
	}

	SetPos(D3DXVECTOR3(pos.x, m_pos.y, pos.z));

	//����̍X�V
	if (m_pBall != nullptr)
	{
		m_pBall->Update();
		m_pBall->SetPos(mylib_const::DEFVEC3);
	}

	if (m_pStateLife != nullptr)
	{
		CState_Life::STATE state = m_pStateLife->GetState();

		m_pStateLife->Update();

		if (state == CState_Life::STATE_DAMAGE &&
			m_pStateLife->GetState() == CState_Life::STATE_NORMAL)
		{//DAMAGE����NORMAL�ɖ߂�����

			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(D3DXMATERIAL));

			//�p�[�c����h������
			for (int nCntPrt = 0; nCntPrt < m_nNumModel; nCntPrt++)
			{
				if (m_apPart[nCntPrt] != nullptr)
				{
					m_apPart[nCntPrt]->SetMat(mat);
					m_apPart[nCntPrt]->Update();
				}
			}
		}
	}

	//�ړ��ʂ��X�V(����������)����
	m_move.x += (0 - m_move.x) * 0.1f;
	m_move.z += (0 - m_move.z) * 0.1f;

	if (m_move.x <= NUM_SPEED_STOP && m_move.x >= -NUM_SPEED_STOP)
	{//�K��l�ȉ��ňړ��ʂ�0��
		m_move.x = 0.0f;
	}
	if (m_move.z <= NUM_SPEED_STOP && m_move.z >= -NUM_SPEED_STOP)
	{//�K��l�ȉ��ňړ��ʂ�0��
		m_move.z = 0.0f;
	}

	int nRank = -1;
	if (pScene->GetMode() == CScene::MODE_GAME &&
		CManager::GetInstance()->GetResult() != CManager::RT_NONE)
	{
		bool bRank = false;

		//���ʐݒ�
		for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
		{
			nRank = CManager::GetInstance()->GetRank(i);

			if (nRank == m_nIdx)
			{
				bRank = true;
			}
		}

		if (bRank == false)
		{
			CManager::GetInstance()->SetRank(m_nIdx, pScene->GetNumPlayer() - 1);
		}
	}
}

//============================
// �v���C���[�̕`��
//============================
void CPlayer::Draw(void)
{
	//�`��
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxTrans);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�f�o�b�O
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- �v���C���[��� ---\n");
	pDebug->Print("���݂̕���:%f %f %f\n", m_rot.x, m_rot.y, m_rot.z);

	if (m_pBall != nullptr)
	{
		pDebug->Print("���݂̃{�[���̑��x�{��:%f\n", m_pBall->GetSpeedMag());
	}

	pDebug->Print("�L���b�`�J�E���g:%d\n", m_nCatchCtr);
}

//============================
// �v���C���[�̐���
//============================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CPlayer *pPlayer = nullptr;

	//�I�u�W�F�N�g����&������
	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->Init(pos, rot, 1);
	}
	else
	{
		return nullptr;
	}

	return pPlayer;
}

//============================
// �ړ�����
//============================
void CPlayer::MoveOperate(float *pRotDest)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	CScene *pScene = CManager::GetInstance()->GetScene();
	if (pScene->GetMode() == CScene::MODE_GAME)
	{
		CGame *pGame = static_cast<CGame*>(pScene);
		CCameraGame *pCameraGame = pGame->GetCamera(m_nIdx);
		cameraRot = pCameraGame->GetRot();
	}

	bool bInput = false;

	if (pInputKeyboard->GetPress(DIK_S) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_DOWN, m_nIdx) || pInputGamepad->GetGameStickLYPress(m_nIdx) < 0)
	{
		if (pInputKeyboard->GetPress(DIK_A) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, m_nIdx) || pInputGamepad->GetGameStickLXPress(m_nIdx) < 0)
		{//�ォ���L�[���������Ƃ�
			m_move.x += sinf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, m_nIdx) || pInputGamepad->GetGameStickLXPress(m_nIdx) > 0)
		{//�ォ�E�L�[���������Ƃ�
			m_move.x += sinf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		else
		{//��L�[�݂̂��������Ƃ�
			m_move.x += sinf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		
		*pRotDest = cameraRot.y + 0.0f * D3DX_PI;
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_UP, m_nIdx) || pInputGamepad->GetGameStickLYPress(m_nIdx) > 0)
	{
		if (pInputKeyboard->GetPress(DIK_A) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, m_nIdx) || pInputGamepad->GetGameStickLXPress(m_nIdx) < 0)
		{//�������L�[���������Ƃ�
			m_move.x += sinf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + -0.25f * D3DX_PI;
			bInput = true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, m_nIdx) || pInputGamepad->GetGameStickLXPress(m_nIdx) > 0)
		{//�����E�L�[���������Ƃ�
			m_move.x += sinf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.25f * D3DX_PI;
			bInput = true;
		}
		else
		{//���L�[�݂̂��������Ƃ�
			m_move.x += sinf(cameraRot.y + 1.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 1.0f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.0f * D3DX_PI;
			bInput = true;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, m_nIdx) || pInputGamepad->GetGameStickLXPress(m_nIdx) < 0)
	{//���L�[�݂̂��������Ƃ�
		m_move.x += sinf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + -0.5f * D3DX_PI;
		bInput = true;
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, m_nIdx) || pInputGamepad->GetGameStickLXPress(m_nIdx) > 0)
	{//�E�L�[�݂̂��������Ƃ�
		m_move.x += sinf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + 0.5f * D3DX_PI;
		bInput = true;
	}

	if (m_pMotion->IsFinish())
	{//���[�V�����ݒ�
		if (m_pMotion->GetType() == MOTIONTYPE_CATCH_RDY)
		{
			m_nCatchCtr = CT_CATCH;
		}
		else if (m_pMotion->GetType() == MOTIONTYPE_TACKLE)
		{
			m_nTackleCtr = CT_TACKLE;
		}
		else if (m_pMotion->GetType() == MOTIONTYPE_JUMP2)
		{
			m_nJump2Ctr = CT_JUMP2;
		}

		if (bInput == true)
		{

			if (NUM_SPEED > m_param.fSpeed)
			{
				m_pMotion->Set(MOTIONTYPE_DASH);
			}
			else
			{
				m_pMotion->Set(MOTIONTYPE_WALK);
			}
		}
		else if(m_move.x == 0.0f && m_move.z == 0.0f)
		{

			m_pMotion->Set(MOTIONTYPE_NEUTRAL);
		}
	}
}

//============================
// 2D�ړ�����
//============================
void CPlayer::MoveOperate2D(float *pRotDest)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	bool bInput = false;

	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	if (m_pMotion->GetType() != MOTIONTYPE_TACKLE)
	{
		if (pInputKeyboard->GetPress(DIK_A) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, m_nIdx) || pInputGamepad->GetGameStickLXPress(m_nIdx) < 0)
		{//���L�[�݂̂��������Ƃ�
			m_move.x += sinf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
			m_move.z += cosf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
			*pRotDest = cameraRot.y + 0.5f * D3DX_PI;
			bInput = true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, m_nIdx) || pInputGamepad->GetGameStickLXPress(m_nIdx) > 0)
		{//�E�L�[�݂̂��������Ƃ�
			m_move.x += sinf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
			m_move.z += cosf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
			*pRotDest = cameraRot.y + -0.5f * D3DX_PI;
			bInput = true;
		}
	}
	if (bInput == true)
	{
		m_rotDest.y = *pRotDest;
		if (m_pMotion->IsFinish() ||
			m_pMotion->GetType() == MOTIONTYPE_NEUTRAL ||
			m_pMotion->GetType() == MOTIONTYPE_GROUND)
		{
			if (NUM_SPEED > m_param.fSpeed)
			{
				m_pMotion->Set(MOTIONTYPE_DASH);
			}
			else
			{
				m_pMotion->Set(MOTIONTYPE_WALK);
			}
		}
	}
	else
	{
		if (m_pMotion->IsFinish() ||
			m_pMotion->GetType() == MOTIONTYPE_DASH ||
			m_pMotion->GetType() == MOTIONTYPE_WALK)
		{
			m_pMotion->Set(MOTIONTYPE_NEUTRAL);
		}
	}
}

//============================
// ��������
//============================
void CPlayer::RotOperate(float * /*pfRotDest*/)
{
	//�}�E�X��PAD�擾
	CInputMouse *pMouse = CManager::GetInstance()->GetInputMouse();

	//�}�E�X��pad�̃X�e�B�b�N�̈ړ��ʂ�m_rotDest���X�V
	if (pMouse == nullptr)
	{//�J�����������
		return;
	}
}

//============================
// �����̏C��
//============================
void CPlayer::RotAdj(float fRotDiff)
{
	//�p�x�̒l���C������
	RotLimit(&fRotDiff);

	m_rot.y += fRotDiff * 0.1f;		//�ړ�����(�p�x)�̕␳

	//�p�x�̒l���C������
	RotLimit(&m_rotDest.x);
	RotLimit(&m_rotDest.y);
	RotLimit(&m_rotDest.z);
	RotLimit(&m_rot.x);
	RotLimit(&m_rot.y);
	RotLimit(&m_rot.z);
}

//============================
// �t�@�C���ǂݍ���(motion)
//============================
void CPlayer::ReadFile(void)
{
	char *pComp = new char[READ_PSIZE];				//�S�~
	char *pFilepass[MAX_PARTS] = {};		//�t�@�C���p�X
	D3DXVECTOR3 pos[MAX_PARTS];			//�v���Z�b�g�ʒu
	D3DXVECTOR3 rot[MAX_PARTS];			//�v���Z�b�g����
	int aParent[MAX_PARTS];				//�e���f���̗L��
	int nNumParts = 0;					//�p�[�c����
	D3DXVECTOR3 **ppPos = nullptr;		//�ʒu
	D3DXVECTOR3 **ppRot = nullptr;		//����
	int nNumKey = 0;					//�L�[��
	int nFrame = 0;						//�t���[����
	int nLoop = 0;						//���[�v [ 0:���Ȃ� / 1:���� ]

	int nKeyCtr = 0;			//���[�V�����Ǎ����̃L�[�J�E���^�[
	int nKeySetCtr = 0;			//���[�V�����Ǎ����̃L�[�Z�b�g�J�E���^�[
	int nMotionCtr = 0;			//���[�V������
	CMotion::INFO *info = new CMotion::INFO[16];		//���[�V�������

	for (int nCntNull = 0; nCntNull < MAX_PARTS; nCntNull++)
	{
		pos[nCntNull] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot[nCntNull] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		aParent[nCntNull] = -1;
	}

	FILE *pFile;

	pFile = fopen(MOTION_FILE, "r");

	if (pFile != nullptr)
	{
		do
		{
			fscanf(pFile, "%s", pComp);

			if (strncmp(pComp, "#", 1) == 0)
			{// ����̂��ƃR�����g
				fgets(pComp, READ_PSIZE, pFile);
				continue;
			}

			if (strcmp(pComp, "NUM_MODEL") == 0)
			{//�����擾
				fscanf(pFile, "%s %d", pComp, &nNumParts);
				m_nNumModel = nNumParts;
			}
			else if (strcmp(pComp, "MODEL_FILENAME") == 0)
			{//�t�@�C���Ǎ�
				for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
				{
					if (pFilepass[nCntCrt] == nullptr)
					{
						pFilepass[nCntCrt] = new char[128];
						fscanf(pFile, "%s %s", pComp, pFilepass[nCntCrt]);
						break;
					}
				}
			}
			else if (strcmp(pComp, "CHARACTERSET") == 0)
			{//�I�t�Z�b�g���擾�J�n
				int nCntSet = 0;

				do
				{
					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "PARTSSET") == 0)
					{//�p�[�c���擾�J�n
						while (TRUE)
						{
							fscanf(pFile, "%s", pComp);

							if (strcmp(pComp, "END_PARTSSET") == 0)
							{
								break;
							}
							else if (strcmp(pComp, "PARENT") == 0)
							{
								fscanf(pFile, "%s %d", pComp, &aParent[nCntSet]);
								//aParent[nCntSet] = -1;
							}
							else if (strcmp(pComp, "POS") == 0)
							{
								fscanf(pFile, "%s %f %f %f", pComp, &pos[nCntSet].x, &pos[nCntSet].y, &pos[nCntSet].z);
							}
							else if (strcmp(pComp, "ROT") == 0)
							{
								fscanf(pFile, "%s %f %f %f", pComp, &rot[nCntSet].x, &rot[nCntSet].y, &rot[nCntSet].z);
							}
						}

						//�擾�I���ŉ��Z
						nCntSet++;
					}

				} while (strcmp(pComp, "END_CHARACTERSET") != 0);
			}
			else if (strcmp(pComp, "MOTIONSET") == 0)
			{
				do
				{//���[�V��������ǂ�

					fscanf(pFile, "%s", pComp);

					if (strncmp(pComp, "#", 1) == 0)
					{// ����̂��ƃR�����g
						fgets(pComp, READ_PSIZE, pFile);
						continue;
					}
					else if (strcmp(pComp, "END_MOTIONSET") == 0)
					{
						nMotionCtr++;
						nKeyCtr = 0;
						nKeySetCtr = 0;

						if (ppPos != nullptr)
						{
							for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
							{
								delete[] ppPos[nCntMotKey];
							}

							delete[] ppPos;
						}

						if (ppRot != nullptr)
						{
							for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
							{
								delete[] ppRot[nCntMotKey];
							}

							delete[] ppRot;
						}

						break;
					}
					else if (strcmp(pComp, "LOOP") == 0)
					{//���[�v�ݒ�擾
						fscanf(pFile, "%s %d", pComp, &nLoop);

						info[nMotionCtr].bLoop = (nLoop == 0 ? false : true);
					}
					else if (strcmp(pComp, "NUM_KEY") == 0)
					{//�L�[���擾
						fscanf(pFile, "%s %d", pComp, &nNumKey);

						info[nMotionCtr].nNumKey = nNumKey;

						//���[�V�������ɕK�v�Ȑ������ʒu���𐶐�
						//�������e : ppPos[�L�[��][�p�[�c��]
						//�������e : ppRot[�L�[��][�p�[�c��]
						ppPos = new D3DXVECTOR3*[nNumKey];
						ppRot = new D3DXVECTOR3*[nNumKey];

						for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
						{
							ppPos[nCntMotKey] = new D3DXVECTOR3[nNumParts];
							ppRot[nCntMotKey] = new D3DXVECTOR3[nNumParts];
						}
					}
					else if (strcmp(pComp, "KEYSET") == 0)
					{//�L�[�Z�b�g���擾�J�n
						do
						{
							fscanf(pFile, "%s", pComp);

							if (strncmp(pComp, "#", 1) == 0)
							{// ����̂��ƃR�����g
								fgets(pComp, READ_PSIZE, pFile);
								continue;
							}
							else if (strcmp(pComp, "END_KEYSET") == 0)
							{
								//�擾�I���ŉ��Z
								nKeySetCtr++;
								nKeyCtr = 0;

								break;
							}
							else if (strcmp(pComp, "FRAME") == 0)
							{//�t���[�����擾
								fscanf(pFile, "%s %d", pComp, &nFrame);

								info[nMotionCtr].aKeyInfo[nKeySetCtr].nFrame = nFrame;
							}
							else if (strcmp(pComp, "KEY") == 0)
							{//�p�[�c���擾�J�n
								while (TRUE)
								{
									fscanf(pFile, "%s", pComp);

									if (strncmp(pComp, "#", 1) == 0)
									{// ����̂��ƃR�����g
										fgets(pComp, READ_PSIZE, pFile);
										continue;
									}
									else if (strcmp(pComp, "END_KEY") == 0)
									{
										//�擾�I���ŉ��Z
										nKeyCtr++;
										break;
									}
									else if (strcmp(pComp, "POS") == 0)
									{
										fscanf(pFile, "%s %f %f %f", pComp, &ppPos[nKeySetCtr][nKeyCtr].x, &ppPos[nKeySetCtr][nKeyCtr].y, &ppPos[nKeySetCtr][nKeyCtr].z);

										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosX = ppPos[nKeySetCtr][nKeyCtr].x;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosY = ppPos[nKeySetCtr][nKeyCtr].y;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosZ = ppPos[nKeySetCtr][nKeyCtr].z;
									}
									else if (strcmp(pComp, "ROT") == 0)
									{
										fscanf(pFile, "%s %f %f %f", pComp, &ppRot[nKeySetCtr][nKeyCtr].x, &ppRot[nKeySetCtr][nKeyCtr].y, &ppRot[nKeySetCtr][nKeyCtr].z);

										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotX = ppRot[nKeySetCtr][nKeyCtr].x;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotY = ppRot[nKeySetCtr][nKeyCtr].y;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotZ = ppRot[nKeySetCtr][nKeyCtr].z;
									}
								}

							}

						} while (strcmp(pComp, "END_KEYSET") != 0);
					}
				} while (strcmp(pComp, "END_MOTIONSET") != 0);
			}

		} while (strcmp(pComp, "END_SCRIPT") != 0);

		fclose(pFile);
	}
	else
	{//�t�@�C���Ǎ��Ɏ��s
		return;
	}

	//���f���̐���(�S�p�[�c��)
	for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
	{
		m_apPart[nCntCrt] = CParts::Create(pFilepass[nCntCrt], pos[nCntCrt], rot[nCntCrt]);
	}

	//�e���f���̐ݒ�(�S�p�[�c��)
	for (int nCntPrt = 0; nCntPrt < nNumParts; nCntPrt++)
	{
		if (aParent[nCntPrt] <= -1)
		{
			m_apPart[nCntPrt]->SetParent(nullptr);
		}
		else
		{
			if (m_apPart[aParent[nCntPrt]] != nullptr)
			{
				m_apPart[nCntPrt]->SetParent(m_apPart[aParent[nCntPrt]]);
			}
		}
	}

	//���[�V�������ݒ�
	m_pMotion->SetInfo(info, nMotionCtr);

	delete[] pComp;		//�S�~
	delete[] info;

	for (int nCntPass = 0; nCntPass < 32; nCntPass++)
	{
		if (pFilepass[nCntPass] != nullptr)
		{
			delete pFilepass[nCntPass];
		}
	}
}

//============================
// �_���[�W����
//============================
void CPlayer::Damage(int nDamege)
{
	D3DXMATERIAL mat;
	ZeroMemory(&mat, sizeof(D3DXMATERIAL));

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->GetState() != CState_Life::STATE_NORMAL)
		{//�_���[�W���󂯂��Ȃ���ԂȂ�
			return;
		}
	}

	m_param.nLife -= nDamege;

	if (m_param.nLife > m_param.nLifeMax)
	{
		m_param.nLife = m_param.nLifeMax;
	}

	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->SubLife(nDamege);
	}

	if (m_pStateLife != nullptr)
	{
		mat = m_pStateLife->GetMat();
	}

	//�p�[�c����h������
	if (nDamege > 0)
	{
		for (int nCntPrt = 0; nCntPrt < MAX_PARTS; nCntPrt++)
		{
			if (m_apPart[nCntPrt] != nullptr)
			{
				m_apPart[nCntPrt]->SetMat(mat);
			}
		}
	}

	if (m_param.nLife <= 0)
	{
		int nRank = -1;

		//���񂾂�
		CPlayer *pPlayer = nullptr;
		CScene *pScene = CManager::GetInstance()->GetScene();
		switch (pScene->GetMode())
		{
		case CScene::MODE_TUTORIAL:
			//���ł͂����Ŏ������X�|�[������
			CObject::SetDeath(true);	//���S�t���O
		
			pPlayer = pScene->GetPlayer(m_nIdx);
			pPlayer = CPlayer::Create(mylib_const::DEFVEC3, mylib_const::DEFVEC3);
			pPlayer->SetIdx(m_nIdx);
			pPlayer->SetMember(static_cast<my_Identity::eMember>(m_nIdx + 1));
			pScene->SetPlayer(pPlayer,m_nIdx);
			break;

		case CScene::MODE_GAME:
			//���ł͂����Ŏ������X�|�[������
			m_bDelete = true;	//���S�t���O

			//���ʐݒ�
			for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
			{
				nRank = CManager::GetInstance()->GetRank(i);
			
				if (nRank == -1)
				{
					nRank = m_nIdx;
					CManager::GetInstance()->SetRank(nRank, i);
					break;
				}
			}

			break;
		default:
			break;
		}
	}

	m_pStateLife->SetState(CState_Life::STATE_DAMAGE, DAMAGE_CT);
}

//============================
// �m�b�N�o�b�N
//============================
void CPlayer::Knockback(float fRot)
{
	//�v���C���[�̌����̋t�ɔ�΂�
	m_move.x = sinf(fRot + D3DX_PI) * 30.0f;
	m_move.z = cosf(fRot + D3DX_PI) * 30.0f;
	m_move.y = 5.0f;

	if (m_pBall != nullptr)
	{
		D3DXMATRIX mtx = m_pBall->GetMtxWorld();
		m_pBall->Throw(D3DXVECTOR3(0.0f, 20.0f, 0.0f), 10.0f);
		m_pBall->SetParent(nullptr);
		m_pBall->SetMember(my_Identity::MEMBER_NONE);
		m_pBall->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
		m_pBall->SetRot(D3DXVECTOR3(mylib_const::DEFVEC3));
		m_pBall = nullptr;
	}

}

//============================
// ���[�V�������擾 (.h�ł͑O���錾�݂̂̂��߁A���g��.cpp�ɏ���)
//============================
CMotion *CPlayer::GetMotion(void)
{
	return m_pMotion;
}

//============================
// �f�o�b�O�L�[
//============================
void CPlayer::DebugKey(CInputKeyboard *pInputKeyboard)
{
	//if (pInputKeyboard->GetPress(DIK_B) || (pInputKeyboard->GetTrigger(DIK_N)))
	//{//B�L�[�Ńu���b�N����
	//	CBlock::Create(GetPos(), GetRot());
	//}

	if (pInputKeyboard->GetTrigger(DIK_K))
	{//K�Ń{�[���X�|�[��
			 CBall::Create(D3DXVECTOR3(0.0f, 150.0f, 0.0f));
	}

	if (pInputKeyboard->GetTrigger(DIK_E))
	{//��Shift��HP����

	}
}

//============================
// �p�x�̒l���C������
//============================
void CPlayer::RotLimit(float *pRot)
{
	if (*pRot > D3DX_PI)
	{
		*pRot -= D3DX_PI * 2;
	}
	else if (*pRot < -D3DX_PI)
	{
		*pRot += D3DX_PI * 2;
	}
}

//============================
//�u���b�N�Ƃ̓����蔻��(��`)
//============================
void CPlayer::CollisionField(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_FIELD)
				{//�A�C�e����������

					CField *pField = (CField*)pObject;
					fHeight = pField->GetHeight(pos);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	if (pos.y < fHeight && m_posOld.y >= fHeight)
	{//���n��

		if (m_bJump)
		{
			D3DXVECTOR3 move = { 3.0f, 0.0f, 0.0f };

			//�����o
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				CEf_Smoke * pSmoke = CEf_Smoke::Create(m_pos);
				pSmoke->SetMove(move);
				pSmoke->SetLife(15);
				pSmoke->SetLifeMax(15);
			
				move.x *= -1.0f;
			}

			m_pMotion->Set(MOTIONTYPE_GROUND);
		}
	
		pos.y = fHeight;
		m_move.y = 0.0f;
		m_bJump = false;
	}

	//���W�ݒ�(�X�V)
	SetPos(pos);
}

//============================
//�u���b�N�Ƃ̓����蔻��(��`)
//============================
void CPlayer::CollisionWall()
{
	D3DXVECTOR3 Cross = mylib_const::DEFVEC3;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_WALL)
				{//�A�C�e����������

					CWall *pWall= (CWall*)pObject;
					pWall->CollisionChara(m_pos, m_posOld, &Cross);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	if (Cross != mylib_const::DEFVEC3)
	{//���莞
		m_pos.x = Cross.x;
		m_pos.z = Cross.z;
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}
}

//============================
//�{�[���Ƃ̓����蔻��(��)
//============================
void CPlayer::CollisionBall(D3DXVECTOR3 pos)
{
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_BALL)
				{//�{�[����������

					//�L���X�g
					CBall *pBall = static_cast<CBall*>(pObject);

					if (pBall != nullptr)
					{
						if (pBall->GetParent() != nullptr)
						{
							pObject = pObject->GetNext();
							continue;
						}

						if (!pBall->CollisionCircle(pos, NUM_GETAREA))
						{//�{�[���ɓ������Ă��Ȃ�������
							pObject = pObject->GetNext();
							continue;
						}

						my_Identity::eMember member = pBall->GetMember();

						if (member == m_eMember)		//���R
						{
						}
						else if (member == my_Identity::MEMBER_NONE)	//������
						{
							if (m_pBall == nullptr)
							{
								m_pBall = pBall;
							}
						}
						else	//�G�R
						{
							if (m_pMotion == nullptr)
							{
								pObject = pObject->GetNext();
								continue;
							}

							//�L���b�`
							if (m_pMotion->GetType() == MOTIONTYPE_CATCH_RDY)
							{
								if (m_pBall != nullptr)
								{//�����Ă����������
									D3DXMATRIX mtx = m_pBall->GetMtxWorld();
									m_pBall->Throw(D3DXVECTOR3(5.0f, 20.0f, 5.0f), 10.0f);
									m_pBall->SetParent(nullptr);
									m_pBall->SetMember(my_Identity::MEMBER_NONE);
									m_pBall->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
									m_pBall->SetRot(D3DXVECTOR3(mylib_const::DEFVEC3));
									m_pBall = nullptr;
								}

								m_pBall = pBall;

								//���o
								CEffect *pEffect = CEffect::Create(m_pBall->GetWorldPos());
								pEffect->SetLifeMax(10);
								pEffect->SetLife(10);
								pEffect->SetLight(true);

								//�W���X�g����
								if (m_nCatchCtr < 0)
								{
									m_pBall->AddSpeedMag(MAX_THROW_CHARGE);
								
									pEffect->SetSize(D3DXVECTOR3(200.0f, 200.0f, 200.0f));
									pEffect->SetColor(D3DXCOLOR(0.9f, 0.0f, 0.9f, 0.9f));
								
									CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CATCH);
								}
								else
								{
									pEffect->SetSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
									pEffect->SetColor(D3DXCOLOR(0.9f, 0.0f, 0.9f, 0.9f));
								
									CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CATCH);
								}

								m_fThrowChargeCtr = MAX_THROW_CHARGE;

								m_pMotion->Set(MOTIONTYPE_CATCH);

							}
							else
							{//�L���b�`�ł����_���[�W
								Damage();
							}
						}
					}
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	if (m_pBall != nullptr)
	{//�{�[������肵���ꍇ

		m_pBall->SetPosOld(mylib_const::DEFVEC3);
		m_pBall->SetPos(mylib_const::DEFVEC3);
		m_pBall->SetMove(mylib_const::DEFVEC3);
		m_pBall->SetParent(m_apPart[HANDR_IDX]);
		m_pBall->SetMember(m_eMember);
	}
}

//============================
//�^�b�N�����̓����蔻��()
//============================
void CPlayer::TackleCollision()
{
	//�O��`�F�b�N
	if (m_pMotion == nullptr)
	{
		return;
	}
	if (m_pMotion->GetType() != MOTIONTYPE_TACKLE)
	{
		return;
	}

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (this == pObject)
				{
					pObject = pObject->GetNext();
					continue;
				}

				if (type == CObject::TYPE_ENEMY)
				{
					CEnemy *pBall = static_cast<CEnemy*>(pObject);

					if (pBall != nullptr)
					{
						if (mylib_useful::CollisionCircle(m_pos, NUM_COLL, pBall->GetPos(), pBall->GetParam().fColl))
						{
							//�m�b�N�o�b�N
							pBall->Damage(0.0f);
						}

					}
				}
				else if(type == CObject::TYPE_PLAYER)
				{
					CPlayer *pPlayer = static_cast<CPlayer*>(pObject);

					if (pPlayer != nullptr)
					{
						if (mylib_useful::CollisionCircle(m_pos, NUM_COLL, pPlayer->GetPos(), pPlayer->GetParam().fColl))
						{
							//�m�b�N�o�b�N
							pPlayer->Damage(0);
							pPlayer->Knockback(m_rot.y);
						}
					}
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}
}

//============================
// ���[�V���������o
//============================
void CPlayer::MotionEffect()
{
	if (m_pMotion == nullptr)
	{
		return;
	}

	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CObjectBillboardAnim *pAnimBB = nullptr;
	CEffect *pEffect = nullptr;
	CAnim *pAnim = nullptr;
	D3DXMATRIX mtx = {};

	switch (m_pMotion->GetType())
	{
	case MOTIONTYPE_NEUTRAL:	//�ҋ@
	case MOTIONTYPE_WALK:		//����
	case MOTIONTYPE_DASH:		//�_�b�V��
	case MOTIONTYPE_JUMP:		//�W�����v
	case MOTIONTYPE_JUMP2:		//2�i�W�����v
	case MOTIONTYPE_GROUND:		//���n
	case MOTIONTYPE_THROW:		//������
	case MOTIONTYPE_CATCH_RDY:	//�L���b�`�p�\����
	case MOTIONTYPE_CATCH:		//�L���b�`
		break;

	case MOTIONTYPE_THROW_RDY:	//�����p�\����
		pEffect = CEffect::Create(m_pBall->GetWorldPos());
		pEffect->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f) * m_pBall->GetSpeedMag());
		pEffect->SetLifeMax(10);
		pEffect->SetLife(10);
		pEffect->SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.2f));
		pEffect->SetLight(true);
		break;

	case MOTIONTYPE_TACKLE:		//�^�b�N��
		if (m_pMotion->GetTotalCtr() % 3 == 0)
		{//�c���I�ȐG�t�F�N�g
			pEffect = CEffect::Create(GetPosCol());
			pEffect->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
			pEffect->SetLifeMax(60);
			pEffect->SetLife(60);
			pEffect->SetColor(D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f));
			pEffect->SetLight(true);
		}
		break;

	default:
		break;
	}
}

//============================
// �J�E���^�[���炵
//============================
void CPlayer::ReduceCounter()
{
	if (m_nCatchCtr > 0)
	{//�N�[���^�C����
		m_nCatchCtr--;
	}
	else if (m_nCatchCtr < 0)
	{//�W���X�g��
		m_nCatchCtr++;
	}

	if (m_nTackleCtr > 0)
	{//�^�b�N��CT��
		m_nTackleCtr--;
	}

	if (m_nJump2Ctr > 0)
	{//2�i�W�����vCT��
		m_nJump2Ctr--;
	}
}

//============================
// �{�[������
//============================
void CPlayer::Throw()
{
	if (m_pBall == nullptr)
	{
		return;
	}

	D3DXMATRIX mtx = m_pBall->GetMtxWorld();
	D3DXVECTOR3 rot = m_rot;

	CScene *pScene = CManager::GetInstance()->GetScene();
	if (pScene->GetMode() == CScene::MODE_GAME)
	{
		CGame *pGame = static_cast<CGame*>(pScene);
		CCameraGame *pCameraGame = pGame->GetCamera(m_nIdx);
		if (pCameraGame != nullptr)
		{
			rot = pCameraGame->GetRot();
			rot.z += D3DX_PI;
		}
	}

	if (m_pBall != nullptr && m_bTarget)
	{
		if (m_pBall->GetTargetEnemy() != nullptr || m_pBall->GetTargetPlayer() != nullptr)
		{
			m_pBall->Target();
			rot = m_pBall->GetMove();
		}
	}

	//����������ɂ���
	rot.z += my_Identity::THROW_PARABOLA;

	m_pBall->Throw(rot, my_Identity::THROW_SPEED);
	m_pBall->SetParent(nullptr);
	m_pBall->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	m_pBall->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pBall = nullptr;

	m_fThrowChargeCtr = 0.0f;
	m_pMotion->Set(MOTIONTYPE_THROW);
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_THROW);
}

//============================
// �^�[�Q�e�B���O
//============================
void CPlayer::Target()
{
	if (!m_bTarget)
	{
		if (m_pTargetMark != nullptr)
		{
			m_pTargetMark->SetDeath(true);
			m_pTargetMark = nullptr;
		}

		return;
	}

	D3DXVECTOR3 rot = m_rot;
	D3DXVECTOR3 pos = m_pos;
	float fHeight = 0.0f;
	bool bTag = false;

	CScene *pScene = CManager::GetInstance()->GetScene();
	if (pScene->GetMode() == CScene::MODE_GAME)
	{
		CGame *pGame = static_cast<CGame*>(pScene);
		CCameraGame *pCameraGame = pGame->GetCamera(m_nIdx);
		if (pCameraGame != nullptr)
		{
			rot = pCameraGame->GetRot();
		}
	}

	//�G(�v���C���[orBot)�����邩�^�[�Q�e�B���O�͈͊p�x�ɂ��邩
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_PLAYER)
				{
					CPlayer *pPlayer = static_cast<CPlayer*>(pObject);

					if (pPlayer == nullptr)
					{
						pObject = pObject->GetNext();
						continue;
					}

					if (pPlayer->GetMember() == m_eMember)
					{
						pObject = pObject->GetNext();
						continue;
					}

					pos = pPlayer->GetPos();
					fHeight = pPlayer->GetHeartPos();

					if (m_pBall != nullptr)
					{
						m_pBall->SetTargetPlayer(pPlayer);
						m_pBall->SetTargetEnemy(nullptr);
						bTag = true;
					}
				}
				//else if (type == CObject::TYPE_ENEMY)
				//{
				//	CEnemy *pEnemy= static_cast<CEnemy*>(pObject);

				//	if (pEnemy != nullptr)
				//	{
				//		pos = pEnemy->GetPos();

				//		if (m_pBall != nullptr)
				//		{
				//			m_pBall->SetTargetEnemy(pEnemy);
				//			m_pBall->SetTargetPlayer(nullptr);
				//			bTag = true;
				//		}
				//	}
				//}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	//�^�[�Q�b�g�}�[�N
	if (bTag == false)
	{
		if (m_pTargetMark != nullptr)
		{
			m_pTargetMark->SetDeath(true);
			m_pTargetMark = nullptr;
		}
		return;
	}

	if (m_pTargetMark == nullptr)
	{
		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		m_pTargetMark = CObjectBillboard::Create(pos);
		m_pTargetMark->SetSize(D3DXVECTOR3(150.0f, 150.0f, 150.0f));
		m_pTargetMark->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\target.png"));

	}

	if (m_pTargetMark != nullptr)
	{
		m_pTargetMark->SetPos(D3DXVECTOR3(pos.x, pos.y + fHeight, pos.z));
	}
}

//============================
// UI�ݒ�
//============================
void CPlayer::InitUI()
{
	//�̗�UI�̐���
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->SetDeath(true);
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(m_param.nLifeMax, m_nIdx);
}

//============================
// ���f���ݒ�
//============================
void CPlayer::InitModel()
{
	//���f���̐���(�S�p�[�c��)
	for (int nCntCrt = 0; nCntCrt < m_nNumModel; nCntCrt++)
	{
		if (m_apPart[nCntCrt] == nullptr)
		{
			continue;
		}

		m_apPart[nCntCrt]->SetTexPass(UV_FILE_BODY[m_nIdx], m_nIdx);	//�e�N�X�`�������ւ�

		std::string file = CManager::GetInstance()->GetXModel()->GetFilename(m_apPart[nCntCrt]->GetIdxModel());
		if (strstr(const_cast<char*>(file.c_str()), "head") == nullptr)
		{//""���Ȃ�������
			m_apPart[nCntCrt]->SetIdxParent(m_nIdx);	//�e�N�X�`�������ւ��ԍ�
		}
	}
}
