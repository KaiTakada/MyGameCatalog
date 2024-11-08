//===============================================
//
// vC[(player.cpp)
// Author ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "player.h"
#include "manager.h"	//ÙÚmÅKv
#include "game.h"		//ÙÚmÅKv
#include "input.h"			//ì
#include "camera.h"			//Ú®ûü
#include "camera_player.h"		//Ú®ûü
#include "sound.h"			//oÉKv
#include "parts.h"			//eqÖW
#include "motion.h"			//[V
#include "enemy.h"			//^bNÌ½è»è
#include "Field.h"			//nÊÆÌ½è»è
#include "wall.h"			//ÇÆÌ½è»è
#include "wp_ball.h"		//
#include "gauge.h"			//HPQ[W
#include "Xmodel.h"			//f
#include "debugproc.h"		//fobO
#include "state_life.h"		//óÔÇ
#include "ef_smoke.h"		//o
#include "wp_ball.h"		//{[Ì½è»è
#include "ui_life.h"		//ÌÍUI
#include "objectBillboard.h"	//`[Wo
#include "texture.h"			//`[Wo
#include "Anim.h"				//`[Wo
#include "target_mark.h"		//`[Wo
#include "particle.h"			//`[Wo

#include "playerManager.h"		//ÅL}l[W

//fobOp(¨»ç­Á·)
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// èè`
//============================
namespace
{
	const float NUM_GETAREA(100.0f);		//æ¾ÌæÌ¼a
	const float NUM_HYGETAREA(100.0f);		//æ¾Á¬ÌæÌ¼a
	const float NUM_GETAREA_SPEED(8.0f);	//æ¾ÌæÌø«ñ¹¬x
	const float NUM_COLL(30.0f);			//½è»è¼a
	const float NUM_ATTACK(1.0f);			//UÍ
	const float NUM_SPEED(1.0f);			//Ú®Ê
	const float NUM_SPEED_STOP(0.0001f);	//Ú®Êð0É·éðl
	const float NUM_HEART(70.0f);			//SÊu
	const float NUM_JUMP(15.0f);			//WvÍ
	const float NUM_BOOST(40.0f);			//u[XgÍ
	const float NUM_GRAV(0.5f);				//dÍ
	const float MAX_THROW_CHARGE(1.0f);		//`[WãÀ
	const float ADD_THROW_CHARGE(0.01f);	//`[W
	const int HANDR_IDX = 12;			//EèIdx
	const int READ_PSIZE(256);			//Ç|C^TCY
	const int DAMAGE_CT(30);			//_[W³GÔ
	const int WAIT_JC(3);			//WXgLb`P\Ô
	const int CT_CATCH(30);			//Lb`CT
	const int JC_FRAME(CT_CATCH - WAIT_JC);			//WXgLb`P\
	const int CT_TACKLE(30);			//^bNCT
	const int CT_JUMP2(30);			//2iWvCT
	const int NUM_HP(5);			//HPÊ
	const D3DXCOLOR COL_SMOKE(D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.5f));
	const char* MOTION_FILE = "data\\SET\\MOTION\\motion_player.txt";		//[Vt@CpX

	const char* UV_FILE_BODY[mylib_const::MAX_PLAYER] =
	{
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
		"data\\TEXTURE\\PLAYER\\human_body_use1.png",
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
	};		//UVeNX`pX
}

namespace MotionBrend
{
	const std::map<CPlayer::MOTIONTYPE, int> NAME_MAP =
	{
		{CPlayer::MOTIONTYPE::MOTIONTYPE_NEUTRAL,		4},	// Ò@
		{CPlayer::MOTIONTYPE::MOTIONTYPE_WALK,			3},	// à«
		{CPlayer::MOTIONTYPE::MOTIONTYPE_DASH,			3},	// _bV
		{CPlayer::MOTIONTYPE::MOTIONTYPE_JUMP,			3},	// Wv
		{CPlayer::MOTIONTYPE::MOTIONTYPE_JUMP2,			2},	// 2iWv
		{CPlayer::MOTIONTYPE::MOTIONTYPE_GROUND,		3},	// n
		{CPlayer::MOTIONTYPE::MOTIONTYPE_THROW_RDY,		2},	// °p\¦é
		{CPlayer::MOTIONTYPE::MOTIONTYPE_THROW,			2},	// °é
		{CPlayer::MOTIONTYPE::MOTIONTYPE_CATCH_RDY,		2},	// Lb`p\¦é
		{CPlayer::MOTIONTYPE::MOTIONTYPE_CATCH,			2},	// Lb`
		{CPlayer::MOTIONTYPE::MOTIONTYPE_TACKLE,		2},	// ^bN
		{CPlayer::MOTIONTYPE::MOTIONTYPE_DAMAGE,		2},	// _[W
		{CPlayer::MOTIONTYPE::MOTIONTYPE_NEUTRAL_BALL,	2},	// Ò@(¿)
		{CPlayer::MOTIONTYPE::MOTIONTYPE_DASH_BALL,		2},	// _bV(¿)
		{CPlayer::MOTIONTYPE::MOTIONTYPE_JUMP_BALL,		2}	// Wv(¿)
	};
}

//============================
// èè`
//============================

//============================
// ÃIoÏé¾
//============================
int CPlayer::m_nControlIdx = 0;		//L[{[hÅì·évC[

//============================
// RXgN^
//============================
CPlayer::CPlayer(int nPriority) : CObjectCharacter(nPriority)
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
	m_nIdx = 0;		//©gÌÔ

	m_bDelete = false;
	m_bTarget = false;
	m_pTargetMark = nullptr;

	if (CPlayerManager* pPlayerManager = CPlayerManager::GetInstance())
	{
		pPlayerManager->Add(this);
	}
}

//============================
// fXgN^
//============================
CPlayer::~CPlayer()
{

}

//====================================
// vC[Ìú»(ftH)
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
	m_param.fLife = NUM_HP;
	m_param.fLifeMax = NUM_HP;
	m_param.fAttack = NUM_ATTACK;
	m_param.fSpeed = NUM_SPEED;
	m_param.fColl = NUM_COLL;

	//[VÌ¶¬Aú»
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//t@CÇp[c¶¬Eú»
	ReadFile();

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	//óÔÌ¶¬
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//ÌÍUIÌ¶¬
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->Uninit();
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(static_cast<int>(m_param.fLifeMax), m_nIdx);
	m_pLifeUI->SetColAll(mylib_const::PERSONAL_COLOR[m_nIdx]);

	//^CvÝè
	SetType(TYPE_PLAYER);

	return S_OK;
}

//====================================
// vC[Ìú»(ÊuIo)
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
	m_param.fLife = NUM_HP;
	m_param.fLifeMax = NUM_HP;
	m_param.fAttack = NUM_ATTACK;
	m_param.fSpeed = NUM_SPEED;
	m_param.fColl = NUM_COLL;

	//[VÌ¶¬Aú»
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//t@CÇp[c¶¬Eú»
	ReadFile();

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	//óÔÌ¶¬
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//ÌÍUIÌ¶¬
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->Uninit();
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(static_cast<int>(m_param.fLifeMax), m_nIdx);
	m_pLifeUI->SetColAll(mylib_const::PERSONAL_COLOR[m_nIdx]);

	//^CvÝè
	SetType(TYPE_PLAYER);

	return S_OK;
}

//============================
// vC[ÌI¹
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

	//IuWFNg
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->SetDeath(true);
		m_pLifeUI = nullptr;
	}

	//I¹
	Release();
}

//============================
// vC[ÌXV
//============================
void CPlayer::Update(void)
{
	//ÀWæ¾
	float fRotMove = m_rot.y;	//»ÝÌü«
	float fRotDest = m_rot.y;	//ÚWÌü«
	float fRotDiff = 0.0f;			//·ª

	D3DXVECTOR3 pos = GetPos();
	m_posOld = pos;

	D3DXVECTOR3 cameraRot = m_rot;
	CScene *pScene = CManager::GetInstance()->GetScene();
	if (pScene->GetMode() == CScene::MODE_GAME)
	{
		CGame *pGame = static_cast<CGame*>(pScene);
		CCameraPlayer *pCameraGame = pGame->GetCamera(m_nIdx);
		fRotDest = pCameraGame->GetRot().y;
	}

	//{^ANV
	ButtonAction();

	//Ú®üÍ
	MoveOperate(&fRotDest);
	RotOperate(&fRotDest);

	if (!m_pMotion->IsFinish())
	{
		m_pMotion->Update();
	}

	fRotDiff = fRotDest - fRotMove;
	RotAdj(fRotDiff);

	m_rotDest.y = fRotDiff;

	CollisionBall(pos);
	Debug();

	//#if _DEBUG
	//	DebugKey();
	//#endif

	CPlayer::MOTIONTYPE Mtype = MOTIONTYPE(m_pMotion->GetType());

	if (Mtype != MOTIONTYPE_TACKLE)
	{
		//TCYÉÊíÉC³
		m_size = MAX_PLAYER_SIZE;
	}
	else
	{//XCfBO¾Á½ç

	}

	MotionEffect();
	ReduceCounter();

	//dÍ
	float fGrav = 1.0f;
	if (Mtype == MOTIONTYPE_JUMP2)
	{
		fGrav = 0.5f;
	}

	m_move.y -= NUM_GRAV * fGrav;

	//Ú®ÊðKp
	pos += m_move;

	//x^Å¿Ç
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

	//³ðæ¾·é
	CollisionWall();
	CObjectCharacter::CollisionField(pos);
	Target();

	if (Mtype == MOTIONTYPE_TACKLE)
	{
		TackleCollision();
	}

	//º~Ï
	if (m_pos.y < -50.0f)
	{
		pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		SetPos(D3DXVECTOR3(pos));
		m_move.y = 0.0f;
		Damage(1);
	}
	else
	{
		SetPos(D3DXVECTOR3(pos.x, m_pos.y, pos.z));
	}

	//íÌXV
	if (m_pBall != nullptr)
	{
		D3DXVECTOR3 maxVtx = CManager::GetInstance()->GetXModel()->GetVtxMax(m_pBall->GetIdxModel());

		m_pBall->Update();
		m_pBall->SetPos(D3DXVECTOR3(0.0f, 0.0f, maxVtx.y));
	}

	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Update();
	}

	//Ú®ÊðXV(¸³¹é)µ«
	m_move.x += (0 - m_move.x) * 0.1f;
	m_move.z += (0 - m_move.z) * 0.1f;

	if (m_move.x <= NUM_SPEED_STOP && m_move.x >= -NUM_SPEED_STOP)
	{//KèlÈºÅÚ®Êð0É
		m_move.x = 0.0f;
	}
	if (m_move.z <= NUM_SPEED_STOP && m_move.z >= -NUM_SPEED_STOP)
	{//KèlÈºÅÚ®Êð0É
		m_move.z = 0.0f;
	}

	//Ò@`FbN
	SetNeutral();

	//ÊmF
	RankOK();

	//à«¹ÌÊuãº
	if ((Mtype == MOTIONTYPE_DASH || Mtype == MOTIONTYPE_WALK) && m_pMotion->GetTotalCtr() % 20 == 0)
	{
		//int nFrame = m_pMotion->GetTotalCtr() % 20;
		//m_fHeart += nFrame - 10;
	}
}

//============================
// vC[Ì`æ
//============================
void CPlayer::Draw(void)
{
	//`æ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//foCXÌæ¾
	D3DXMATRIX mtxRot, mtxTrans;		//vZp}gbNX

	//[h}gbNXÌú»
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxTrans);

	//ü«ð½f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//Êuð½f
	D3DXMatrixTranslation(&mtxTrans,m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//[h}gbNXÌÝè
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//============================
// vC[Ì¶¬
//============================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CPlayer *pPlayer = nullptr;

	//IuWFNg¶¬&ú»
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
// Ú®ì
//============================
void CPlayer::MoveOperate(float *pRotDest)
{
	//L[{[hæ¾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	CScene *pScene = CManager::GetInstance()->GetScene();
	if (pScene->GetMode() == CScene::MODE_GAME)
	{
		CGame *pGame = static_cast<CGame*>(pScene);
		CCameraPlayer *pCameraGame = pGame->GetCamera(m_nIdx);
		cameraRot = pCameraGame->GetRot();
	}

	bool bInput = false;

	if (ControllManage(CONTROL_BACK))
	{
		if (ControllManage(CONTROL_LEFT))
		{//ã©Â¶L[ðµ½Æ«
			m_move.x += sinf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		else if (ControllManage(CONTROL_RIGHT))
		{//ã©ÂEL[ðµ½Æ«
			m_move.x += sinf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		else
		{//ãL[ÌÝðµ½Æ«
			m_move.x += sinf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		
		*pRotDest = cameraRot.y + 0.0f * D3DX_PI;
	}
	else if (ControllManage(CONTROL_FRONT))
	{
		if (ControllManage(CONTROL_LEFT))
		{//º©Â¶L[ðµ½Æ«
			m_move.x += sinf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + -0.25f * D3DX_PI;
			bInput = true;
		}
		else if (ControllManage(CONTROL_RIGHT))
		{//º©ÂEL[ðµ½Æ«
			m_move.x += sinf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.25f * D3DX_PI;
			bInput = true;
		}
		else
		{//ºL[ÌÝðµ½Æ«
			m_move.x += sinf(cameraRot.y + 1.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 1.0f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.0f * D3DX_PI;
			bInput = true;
		}
	}
	else if (ControllManage(CONTROL_LEFT))
	{//¶L[ÌÝðµ½Æ«
		m_move.x += sinf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + -0.5f * D3DX_PI;
		bInput = true;
	}
	else if (ControllManage(CONTROL_RIGHT))
	{//EL[ÌÝðµ½Æ«
		m_move.x += sinf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + 0.5f * D3DX_PI;
		bInput = true;
	}

	if (m_pMotion->IsFinish() ||
		m_pMotion->GetType() == MOTIONTYPE_NEUTRAL ||
		m_pMotion->GetType() == MOTIONTYPE_NEUTRAL_BALL)
	{//[VÝè
	
		if (m_pMotion->GetType() == MOTIONTYPE_JUMP2)
		{
			m_nJump2Ctr = CT_JUMP2;
		}

		if (bInput == true && !GetJump())
		{
			if (m_pBall != nullptr &&
				m_pMotion->GetType() != MOTIONTYPE_DASH_BALL)
			{
				MotionSet(MOTIONTYPE_DASH_BALL, MotionBrend::NAME_MAP.at(MOTIONTYPE_DASH_BALL));
			}
			else if(m_pMotion->GetType() != MOTIONTYPE_WALK)
			{
				MotionSet(MOTIONTYPE_WALK, MotionBrend::NAME_MAP.at(MOTIONTYPE_WALK));
			}
		}
	}

	//Ò@óÔÉÈé
	if (mylib_useful::Vec2Length(m_move.x, m_move.z) == 0.0f &&
		!GetJump()&&
		!bInput &&
		m_pMotion->GetType() != MOTIONTYPE_NEUTRAL &&
		m_pMotion->GetType() != MOTIONTYPE_NEUTRAL_BALL &&
		(m_pMotion->GetType() != MOTIONTYPE_CATCH_RDY && m_nCatchCtr > 0) &&
		m_pMotion->GetType() != MOTIONTYPE_THROW_RDY)
	{
		if (m_pBall != nullptr &&
			m_pMotion->GetType() != MOTIONTYPE_NEUTRAL_BALL)
		{
			MotionSet(MOTIONTYPE_NEUTRAL_BALL, MotionBrend::NAME_MAP.at(MOTIONTYPE_NEUTRAL_BALL));
		}
		else if(m_pBall == nullptr && 
			m_pMotion->GetType() != MOTIONTYPE_NEUTRAL)
		{
			MotionSet(MOTIONTYPE_NEUTRAL, MotionBrend::NAME_MAP.at(MOTIONTYPE_NEUTRAL));
		}
	}
}

//============================
// ûüì
//============================
void CPlayer::RotOperate(float * /*pfRotDest*/)
{
	//}EXÆPADæ¾
	CInputMouse *pMouse = CManager::GetInstance()->GetInputMouse();

	//}EXÆpadÌXeBbNÌÚ®ÊÅm_rotDestðXV
	if (pMouse == nullptr)
	{//Jª êÎ
		return;
	}
}

//============================
// t@CÇÝÝ(motion)
//============================
void CPlayer::ReadFile(void)
{
	char *pComp = new char[READ_PSIZE];				//S~
	char *pFilepass[MAX_PARTS] = {};		//t@CpX
	D3DXVECTOR3 pos[MAX_PARTS];			//vZbgÊu
	D3DXVECTOR3 rot[MAX_PARTS];			//vZbgü«
	int aParent[MAX_PARTS];				//efÌL³
	int nNumParts = 0;					//p[c
	D3DXVECTOR3 **ppPos = nullptr;		//Êu
	D3DXVECTOR3 **ppRot = nullptr;		//ü«
	int nNumKey = 0;					//L[
	int nFrame = 0;						//t[
	int nLoop = 0;						//[v [ 0:µÈ¢ / 1:·é ]

	int nKeyCtr = 0;			//[VÇÌL[JE^[
	int nKeySetCtr = 0;			//[VÇÌL[ZbgJE^[
	int nMotionCtr = 0;			//[V
	CMotion::INFO *info = new CMotion::INFO[16];		//[Vîñ

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
			{// ±êÌ ÆRg
				fgets(pComp, READ_PSIZE, pFile);
				continue;
			}

			if (strcmp(pComp, "NUM_MODEL") == 0)
			{//æ¾
				fscanf(pFile, "%s %d", pComp, &nNumParts);
				m_nNumModel = nNumParts;
			}
			else if (strcmp(pComp, "MODEL_FILENAME") == 0)
			{//t@CÇ
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
			{//ItZbgîñæ¾Jn
				int nCntSet = 0;

				do
				{
					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "PARTSSET") == 0)
					{//p[cîñæ¾Jn
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

						//æ¾I¹ÅÁZ
						nCntSet++;
					}

				} while (strcmp(pComp, "END_CHARACTERSET") != 0);
			}
			else if (strcmp(pComp, "MOTIONSET") == 0)
			{
				do
				{//[VîñðÇÞ

					fscanf(pFile, "%s", pComp);

					if (strncmp(pComp, "#", 1) == 0)
					{// ±êÌ ÆRg
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
					{//[vÝèæ¾
						fscanf(pFile, "%s %d", pComp, &nLoop);

						info[nMotionCtr].bLoop = (nLoop == 0 ? false : true);
					}
					else if (strcmp(pComp, "NUM_KEY") == 0)
					{//L[æ¾
						fscanf(pFile, "%s %d", pComp, &nNumKey);

						info[nMotionCtr].nNumKey = nNumKey;

						//[VÉKvÈ¾¯Êuîñð¶¬
						//¶¬àe : ppPos[L[][p[c]
						//¶¬àe : ppRot[L[][p[c]
						ppPos = new D3DXVECTOR3*[nNumKey];
						ppRot = new D3DXVECTOR3*[nNumKey];

						for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
						{
							ppPos[nCntMotKey] = new D3DXVECTOR3[nNumParts];
							ppRot[nCntMotKey] = new D3DXVECTOR3[nNumParts];
						}
					}
					else if (strcmp(pComp, "KEYSET") == 0)
					{//L[Zbgîñæ¾Jn
						do
						{
							fscanf(pFile, "%s", pComp);

							if (strncmp(pComp, "#", 1) == 0)
							{// ±êÌ ÆRg
								fgets(pComp, READ_PSIZE, pFile);
								continue;
							}
							else if (strcmp(pComp, "END_KEYSET") == 0)
							{
								//æ¾I¹ÅÁZ
								nKeySetCtr++;
								nKeyCtr = 0;

								break;
							}
							else if (strcmp(pComp, "FRAME") == 0)
							{//t[æ¾
								fscanf(pFile, "%s %d", pComp, &nFrame);

								info[nMotionCtr].aKeyInfo[nKeySetCtr].nFrame = nFrame;
							}
							else if (strcmp(pComp, "KEY") == 0)
							{//p[cîñæ¾Jn
								while (TRUE)
								{
									fscanf(pFile, "%s", pComp);

									if (strncmp(pComp, "#", 1) == 0)
									{// ±êÌ ÆRg
										fgets(pComp, READ_PSIZE, pFile);
										continue;
									}
									else if (strcmp(pComp, "END_KEY") == 0)
									{
										//æ¾I¹ÅÁZ
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
	{//t@CÇÉ¸s
		return;
	}

	//fÌ¶¬(Sp[cª)
	for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
	{
		m_apPart[nCntCrt] = CParts::Create(pFilepass[nCntCrt], pos[nCntCrt], rot[nCntCrt]);
	}

	//efÌÝè(Sp[cª)
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

	//[VîñÝè
	m_pMotion->SetInfo(info, nMotionCtr);

	delete[] pComp;		//S~
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
// _[W
//============================
void CPlayer::Damage(int nDamege)
{
	D3DXMATERIAL mat;
	ZeroMemory(&mat, sizeof(D3DXMATERIAL));

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->GetState() != CState_Life::STATE_NORMAL)
		{//_[Wðó¯çêÈ¢óÔÈç
			return;
		}
	}

	MotionSet(MOTIONTYPE_DAMAGE, MotionBrend::NAME_MAP.at(MOTIONTYPE_DAMAGE));

	m_param.fLife -= nDamege;

	if (m_param.fLife > m_param.fLifeMax)
	{
		m_param.fLife = m_param.fLifeMax;
	}

	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->SubLife(nDamege);
	}

	if (m_pStateLife != nullptr)
	{
		mat = m_pStateLife->GetMat();
	}

	//p[cêÂ¸Âh·é
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

	if (m_param.fLife <= 0)
	{
		int nRank = -1;

		//ñ¾ç
		CPlayer *pPlayer = nullptr;
		CScene *pScene = CManager::GetInstance()->GetScene();
		switch (pScene->GetMode())
		{
		case CScene::MODE_TUTORIAL:
			//¿ÅÍ±±Å©®X|[·é
			CObject::SetDeath(true);	//StO
		
			pPlayer = pScene->GetElem(m_nIdx);
			pPlayer = CPlayer::Create(mylib_const::DEFVEC3, mylib_const::DEFVEC3);
			pPlayer->SetIdx(m_nIdx);
			pPlayer->SetMember(static_cast<my_Identity::eMember>(m_nIdx + 1));
			pScene->SetPlayer(pPlayer,m_nIdx);
			break;

		case CScene::MODE_GAME:
			//¿ÅÍ±±Å©®X|[·é
			m_bDelete = true;	//StO

			//ÊÝè
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
// mbNobN
//============================
void CPlayer::Knockback(float fRot)
{
	//^¦çê½ü«ÌtÉòÎ·
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
// [Vîñæ¾ (.hÅÍOûé¾ÌÝÌ½ßAgÍ.cppÉ­)
//============================
CMotion *CPlayer::GetMotion(void)
{
	return m_pMotion;
}

//============================
// fobOL[
//============================
void CPlayer::Debug()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_K))
	{//KÅ{[X|[
		CBall::Create(D3DXVECTOR3(0.0f, 150.0f, 0.0f));
	}

	if (pInputKeyboard->GetTrigger(DIK_E) && m_nControlIdx == m_nIdx)
	{//ì·évC[ÏX
		m_nControlIdx++;
		if (m_nControlIdx > mylib_const::MAX_PLAYER) m_nControlIdx = 0;
		//if (m_nControlIdx > CPlayerManager::GetPlayer(m_nControlIdx)) m_nControlIdx = 0;
	}

	// fobO\¦
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- vC[îñ ---\n");
	pDebug->Print("»ÝÌÊu:%f %f %f\n", m_pos.x, m_pos.y, m_pos.z);
	pDebug->Print("»ÝÌûü:%f %f %f\n", m_rot.x, m_rot.y, m_rot.z);

	if (m_pBall != nullptr)
	{
		pDebug->Print("»ÝÌ{[Ì¬x{¦:%f\n", m_pBall->GetSpeedMag());
	}

	pDebug->Print("[V^Cv:%d\n", m_pMotion->GetType());
	pDebug->Print("[VJE^:%d\n", m_pMotion->GetCounter());
	pDebug->Print("[VSÌJE^:%d\n", m_pMotion->GetTotalCtr());
	pDebug->Print("[V»ÝL[:%d\n", m_pMotion->GetKey());
	pDebug->Print("[VOñL[:%d\n", m_pMotion->GetKeyOld());
	pDebug->Print("^[QeBOóÔ:%d\n", m_bTarget);
}

//============================
//ubNÆÌ½è»è(é`)
//============================
void CPlayer::CollisionWall()
{
	D3DXVECTOR3 Cross = mylib_const::DEFVEC3;
	bool bCol = false;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//¡ñÌIuWFNgÌ^Cv

				if (type == CObject::TYPE_WALL)
				{//ACe¾Á½ç

					CWall *pWall= (CWall*)pObject;
					bCol = pWall->CollisionChara(m_pos, m_posOld, &Cross);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == EndÜÅsÁ½ÁÄ±ÆÅ±ÌDæxÍI¹
				break;
			}
		}
	}

	if (bCol)
	{//½ÁÄ¢½ç
		m_pos.x = Cross.x;
		m_pos.z = Cross.z;
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}
}

//============================
//{[ÆÌ½è»è()
//============================
void CPlayer::CollisionBall(D3DXVECTOR3 pos)
{
	bool bHave = false;
	CParticle* pParticle = nullptr;

	if (m_pStateLife->GetState() != CState_Life::STATE_NORMAL)
	{
		return;
	}

	if (m_pBall != nullptr)
	{
		bHave = true;
	}

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//¡ñÌIuWFNgÌ^Cv

				if (type == CObject::TYPE_BALL)
				{//{[¾Á½ç

					//LXg
					CBall *pBall = static_cast<CBall*>(pObject);

					if (pBall != nullptr)
					{
						if (pBall->GetParent() != nullptr)
						{
							pObject = pObject->GetNext();
							continue;
						}

						if (!pBall->CollisionCircle(pos, NUM_GETAREA))
						{//{[É½ÁÄ¢È©Á½ç
							pObject = pObject->GetNext();
							continue;
						}

						my_Identity::eMember member = pBall->GetMember();

						if (member == m_eMember)		//©R
						{
						}
						else if (member == my_Identity::MEMBER_NONE || member == my_Identity::MEMBER_MAP)	//³®
						{
							if (m_pBall == nullptr)
							{
								m_pBall = pBall;
							}
						}
						else	//GR
						{
							if (m_pMotion == nullptr)
							{
								pObject = pObject->GetNext();
								continue;
							}

							//Lb`
							if (m_pMotion->GetType() == MOTIONTYPE_CATCH_RDY)
							{
								if (m_pBall != nullptr)
								{//ÁÄ¢½çúÁÄ
									D3DXMATRIX mtx = m_pBall->GetMtxWorld();
									m_pBall->Throw(D3DXVECTOR3(5.0f, 20.0f, 5.0f), 10.0f);
									m_pBall->SetParent(nullptr);
									m_pBall->SetMember(my_Identity::MEMBER_NONE);
									m_pBall->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
									m_pBall->SetRot(D3DXVECTOR3(mylib_const::DEFVEC3));
									m_pBall = nullptr;
								}

								m_pBall = pBall;

								//o
								CEffect *pEffect = CEffect::Create(m_pBall->GetWorldPos());
								pEffect->SetLifeMax(10);
								pEffect->SetLife(10);
								pEffect->SetLight(true);

								//WXg»è
								if (m_nCatchCtr > JC_FRAME)
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

								MotionSet(MOTIONTYPE_CATCH, MotionBrend::NAME_MAP.at(MOTIONTYPE_CATCH));

							}
							else
							{//Lb`Å«¸_[W
								Damage();
								Knockback(mylib_useful::Point2Angle(m_pos, pBall->GetPos()));
								pParticle = CParticle::Create(m_pos, mylib_const::DEFVEC3, D3DXVECTOR3(100.0f, 100.0f, 100.0f),
									mylib_const::DEFVEC3, CParticle::TYPE::TYPE_EXPLODE);
								pParticle->SetSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
								pParticle->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							}
						}
					}
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == EndÜÅsÁ½ÁÄ±ÆÅ±ÌDæxÍI¹
				break;
			}
		}
	}

	if (m_pBall != nullptr && bHave == false)
	{//{[ðüèµ½ê

		m_pBall->SetPosOld(mylib_const::DEFVEC3);
		m_pBall->SetPos(mylib_const::DEFVEC3);
		m_pBall->SetMove(mylib_const::DEFVEC3);
		m_pBall->SetParent(m_apPart[HANDR_IDX]);
		m_pBall->SetMember(m_eMember);
	}
}

//============================
//^bNÌ½è»è()
//============================
void CPlayer::TackleCollision()
{
	//Oñ`FbN
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
				CObject::TYPE type = pObject->GetType();	//¡ñÌIuWFNgÌ^Cv

				if (this == pObject)
				{
					pObject = pObject->GetNext();
					continue;
				}

				if (type == CObject::TYPE_ENEMY)
				{
					CEnemy *pEnemy = static_cast<CEnemy*>(pObject);

					if (pEnemy != nullptr)
					{
						if (mylib_useful::CollisionCircle(m_pos, NUM_COLL, pEnemy->GetPos(), pEnemy->GetParam().fColl))
						{
							if (m_pStateLife != nullptr)
							{
								pEnemy->Damage(0.0f);
							}
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
							if (pPlayer->GetStateLife()->GetState() == CState_Life::STATE_NORMAL)
							{//_[Wðó¯çêÈ¢óÔÈç
								pPlayer->Damage(0);
								pPlayer->Knockback(m_rot.y);
							}
						}
					}
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == EndÜÅsÁ½ÁÄ±ÆÅ±ÌDæxÍI¹
				break;
			}
		}
	}
}

//============================
// [VÝè
//============================
void CPlayer::MotionSet(int type, int brend)
{
	if (brend > 0)
	{
		m_pMotion->Set(type, brend);
	}
	else
	{
		m_pMotion->Set(type);
	}
}

//============================
// [Vo
//============================
void CPlayer::MotionEffect()
{
	if (m_pMotion == nullptr)
	{
		return;
	}

	CEffect *pEffect = nullptr;
	D3DXMATRIX mtx = {};

	switch (m_pMotion->GetType())
	{
	case MOTIONTYPE_WALK:	//Ú®
	case MOTIONTYPE_DASH_BALL:
		if (GetJump() || mylib_useful::Vec2Length(GetMove().x, GetMove().z) <= 5.0f)
		{
			break;
		}

		if (m_pMotion->GetTotalCtr() % 6 == 0)
		{//GtFNg
			D3DXVECTOR3 pos = GetPos();
			pos.x += sinf(GetRot().y + D3DX_PI) * 1.0f;		//x
			pos.z += cosf(GetRot().y + D3DX_PI) * 1.0f;		//z
			D3DXVECTOR2 posTex = D3DXVECTOR2(0.0f, 0.0f);
			D3DXVECTOR2 posTex1 = D3DXVECTOR2(1.0f, 1.0f);
			D3DXVECTOR2 posD = D3DXVECTOR2(1.0f, 1.0f);
			if (rand() % 2)
			{
				posD = posTex;
				posTex = posTex1;
				posTex1 = posD;
			}

			//o
			CEf_Smoke* pSmoke = CEf_Smoke::Create(pos);
			if (pSmoke == nullptr) break;
			pSmoke->SetLife(60);
			pSmoke->SetLifeMax(60);
			pSmoke->SetTexPos(posTex, posTex1);
			pSmoke->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
			pSmoke->SetColor(COL_SMOKE);
			pSmoke->SetScaleFactor(0.93f);
		}
		break;

	case MOTIONTYPE_THROW_RDY:	//°p\¦é
		pEffect = CEffect::Create(m_pBall->GetWorldPos());
		if (pEffect == nullptr) break;
		pEffect->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f) * m_pBall->GetSpeedMag());
		pEffect->SetLifeMax(10);
		pEffect->SetLife(10);
		pEffect->SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.2f));
		pEffect->SetLight(true);
		break;

	case MOTIONTYPE_TACKLE:		//^bN
		if (m_pMotion->GetTotalCtr() % 3 == 0)
		{//cIÈÂGtFNg
			pEffect = CEffect::Create(GetPosCol());
			if (pEffect == nullptr) break;
			pEffect->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
			pEffect->SetLifeMax(60);
			pEffect->SetLife(60);
			pEffect->SetColor(D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f));
			pEffect->SetLight(true);
		}
		break;

	case MOTIONTYPE_JUMP2:		//2iWv
		if (m_pMotion->GetTotalCtr() % 2 == 0)
		{//cIÈÂGtFNg

			D3DXVECTOR3 posWorld = GetPos();
			D3DXVECTOR3 posDest = GetPos() - m_apPart[HANDR_IDX]->GetWorldPos();

			posWorld.y += 100.0f;
			posWorld.x += posDest.x * 2.0f;
			posWorld.z += posDest.z * 2.0f;

			pEffect = CEffect::Create(posWorld);
			if (pEffect == nullptr) break;
			pEffect->SetSize(D3DXVECTOR3(20.0f, 20.0f, 20.0f));
			pEffect->SetLifeMax(5);
			pEffect->SetLife(5);
			pEffect->SetColor(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
			pEffect->SetLight(true);
		}
		break;

	default:
		break;
	}
}

//============================
// JE^[¸çµ
//============================
void CPlayer::ReduceCounter()
{
	if (m_nCatchCtr > 0)
	{//N[^C
		m_nCatchCtr--;
	}
	else if (m_nCatchCtr < 0)
	{//WXg
		m_nCatchCtr++;
	}

	if (m_nTackleCtr > 0)
	{//^bNCT
		m_nTackleCtr--;
	}

	if (m_nJump2Ctr > 0)
	{//2iWvCT
		m_nJump2Ctr--;
	}
}

//============================
// {[°
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
		CCameraPlayer *pCameraGame = pGame->GetCamera(m_nIdx);
		if (pCameraGame != nullptr)
		{
			rot = pCameraGame->GetRot();
			rot.z += D3DX_PI;
		}
	}

	if (m_pBall != nullptr && m_bTarget)
	{
		if (m_pBall->GetTarget() != nullptr)
		{
			m_pBall->Target();
			rot = m_pBall->GetMove();
		}
	}

	//­µãü«É·é
	rot.z += my_Identity::THROW_PARABOLA;

	m_pBall->Throw(rot, my_Identity::THROW_SPEED);
	m_pBall->SetParent(nullptr);
	m_pBall->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	m_pBall->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pBall = nullptr;

	m_fThrowChargeCtr = 0.0f;
	MotionSet(MOTIONTYPE_THROW, MotionBrend::NAME_MAP.at(MOTIONTYPE_THROW));
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_THROW);
}

//============================
// ^[QeBO
//============================
void CPlayer::Target()
{
	CScene* pScene = CManager::GetInstance()->GetScene();
	CCameraPlayer* pCameraGame = nullptr;
	CGame* pGame = static_cast<CGame*>(pScene);

	if (pScene->GetMode() != CScene::MODE_GAME)
	{
		return;
	}

	pCameraGame = pGame->GetCamera(m_nIdx);

	if (!m_bTarget)
	{
		if (m_pTargetMark != nullptr)
		{
			m_pTargetMark->SetDeath(true);
			m_pTargetMark = nullptr;
		}

		if (pCameraGame != nullptr)
		{
			pCameraGame->SetTarget(nullptr);
		}

		return;
	}

	D3DXVECTOR3 myPos = GetPos();
	D3DXVECTOR3 pos = myPos;
	bool bTag = false;
	CPlayer* pPlayer = nullptr;

	//G(vC[orBot)ª¢é©^[QeBOÍÍpxÉ¢é©
	int i = 0;

	while (CPlayerManager::GetInstance()->GetElem(i) != nullptr)
	{
		pPlayer = CPlayerManager::GetInstance()->GetElem(i);

		if (pPlayer == nullptr)
		{//k`FbN
			i++;
			continue;
		}

		if (pPlayer->GetMember() == m_eMember)
		{//©g
			pPlayer = nullptr;
			i++;
			continue;
		}

		pos = pPlayer->GetPos();
		pos.y += pPlayer->GetHeartPos();
		bTag = true;

		break;
	}

	// ^[Qbg}[N
	if (m_pTargetMark != nullptr)
	{
		m_pTargetMark->SetDeath(true);
		m_pTargetMark = nullptr;
	}

	if (bTag)
	{
		if (pCameraGame != nullptr)
		{
			pCameraGame->SetTarget(pPlayer);
		}

		// ^[QbgvC[ð©é
		D3DXVECTOR3 rot = GetRot();
		rot.y = mylib_useful::Point2Angle(pos, myPos);
		SetRot(rot);

		// }[Nð¶¬
		m_pTargetMark = CTarget_Mark::Create(pos);
		if (m_pTargetMark == nullptr) return;
		m_pTargetMark->SetSize(D3DXVECTOR3(150.0f, 150.0f, 150.0f));
		m_pTargetMark->SetColor(mylib_const::PERSONAL_COLOR[pPlayer->GetIdx()]);
		m_pTargetMark->SetIdx(m_nIdx);
	}
}

//============================
// j[góÔÉ·é
//============================
void CPlayer::SetNeutral()
{
	bool bNeutral = false;
	int type = m_pMotion->GetType();

	switch (type)
	{

	case MOTIONTYPE_WALK:
	case MOTIONTYPE_DASH:
	case MOTIONTYPE_DASH_BALL:
	case MOTIONTYPE_GROUND:
		if (mylib_useful::Vec2Length(m_move.x, m_move.z) == 0.0f) bNeutral = true;
		break;

	case MOTIONTYPE_CATCH_RDY:
		if (m_nCatchCtr == 0) bNeutral = true;
		break;

	case MOTIONTYPE_CATCH:
		if (m_pMotion->IsFinish()) bNeutral = true;
		break;

	case MOTIONTYPE_TACKLE:
		if (m_nTackleCtr == 0) bNeutral = true;
		break;

	case MOTIONTYPE_THROW:
		if (m_pMotion->IsFinish()) bNeutral = true;
		break;

	case MOTIONTYPE_DAMAGE:
		if (m_pMotion->IsFinish()) bNeutral = true;
		break;

	default:
		break;
	}

	if (m_pStateLife != nullptr)
	{
		if (type == MOTIONTYPE_DAMAGE && bNeutral)
		{//DAMAGE©çNORMALÉßÁ½ç(F¼µ)

			//p[cêÂ¸Âh·é
			for (int nCntPrt = 0; nCntPrt < m_nNumModel; nCntPrt++)
			{
				if (m_apPart[nCntPrt] != nullptr)
				{
					m_apPart[nCntPrt]->SetMat(D3DXMATERIAL());
					m_apPart[nCntPrt]->Update();
				}
			}
		}
	}

	if (bNeutral)
	{
		if (m_pBall != nullptr &&
			m_pMotion->GetType() != MOTIONTYPE_NEUTRAL_BALL)
		{
			MotionSet(MOTIONTYPE_NEUTRAL_BALL, MotionBrend::NAME_MAP.at(MOTIONTYPE_NEUTRAL_BALL));
		}
		else if (m_pBall == nullptr &&
			m_pMotion->GetType() != MOTIONTYPE_NEUTRAL)
		{
			MotionSet(MOTIONTYPE_NEUTRAL, MotionBrend::NAME_MAP.at(MOTIONTYPE_NEUTRAL));
		}
	}
}

//============================
// UIÝè
//============================
void CPlayer::InitUI()
{
	//ÌÍUIÌ¶¬
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->SetDeath(true);
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(static_cast<int>(m_param.fLifeMax), m_nIdx);
	m_pLifeUI->SetColAll(mylib_const::PERSONAL_COLOR[m_nIdx]);
}

//============================
// fÝè
//============================
void CPlayer::InitModel()
{
	//fÌ¶¬(Sp[cª)
	for (int nCntCrt = 0; nCntCrt < m_nNumModel; nCntCrt++)
	{
		if (m_apPart[nCntCrt] == nullptr)
		{
			continue;
		}

		m_apPart[nCntCrt]->SetTexPass(UV_FILE_BODY[m_nIdx], m_nIdx);	//eNX`·µÖ¦

		std::string file = CManager::GetInstance()->GetXModel()->GetFilename(m_apPart[nCntCrt]->GetIdxModel());
		if (strstr(const_cast<char*>(file.c_str()), "head") == nullptr)
		{//""ªÈ©Á½ç
			m_apPart[nCntCrt]->SetIdxParent(m_nIdx);	//eNX`·µÖ¦Ô
		}
	}

	//ú[V
	MotionSet(MOTIONTYPE_NEUTRAL, MotionBrend::NAME_MAP.at(MOTIONTYPE_NEUTRAL));
}

//============================
// {^ANV
//============================
void CPlayer::ButtonAction()
{
	//Cvbgæ¾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad* pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	bool bJump = GetJump();

	// 2iWvorWv
	if (ControllManage(CONTROL_JUMP))
	{// [ - ]L[

		if (bJump == true
			&& !GetJumpSec()
			&& m_pMotion->GetType() != MOTIONTYPE_JUMP2
			&& m_pMotion->GetType() != MOTIONTYPE_TACKLE
			&& m_nJump2Ctr == 0)
		{// WvgpÏÝ
			SetJumpSec(true);
			m_move.y = NUM_JUMP * 0.5f;
			CPlayer::MOTIONTYPE type = MOTIONTYPE_JUMP2;
			MotionSet(type, MotionBrend::NAME_MAP.at(type));

			// o
		}
		else if (bJump == false)
		{// Wv¢gp
			bJump = true;
			m_move.y += NUM_JUMP;

			CPlayer::MOTIONTYPE type = MOTIONTYPE_JUMP;

			if (m_pBall != nullptr)
			{
				type = MOTIONTYPE_JUMP_BALL;
				MotionSet(type, MotionBrend::NAME_MAP.at(type));
			}
			else
			{
				type = MOTIONTYPE_JUMP;
				MotionSet(type, MotionBrend::NAME_MAP.at(type));
			}

			// o
			CEf_Smoke* pSmoke = CEf_Smoke::Create(m_pos);
			if (pSmoke != nullptr)
			{
				pSmoke->SetLife(10);
				pSmoke->SetLifeMax(10);
				pSmoke->SetColor(COL_SMOKE);
			}
		}
	}

	// ^bN
	if (ControllManage(CONTROL_TACKLE)
		&& m_pMotion->GetType() != MOTIONTYPE_TACKLE
		&& m_nTackleCtr == 0)
	{// [RShift]L[ºWvµÄ¢È¢
		float fStickL = pInputGamepad->GetStickLRot(m_nIdx);
		m_move.x = sinf(GetRot().y + fStickL + D3DX_PI * 1.0f) * NUM_BOOST;		//x
		m_move.z = cosf(GetRot().y + fStickL + D3DX_PI * 1.0f) * NUM_BOOST;		//z
		m_nTackleCtr = CT_TACKLE;

		CPlayer::MOTIONTYPE type = MOTIONTYPE_TACKLE;
		MotionSet(type, MotionBrend::NAME_MAP.at(type));

		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_TACKLE);
	}

	// °\¦
	if (ControllManage(CONTROL_THROW)
		&& m_pBall != nullptr
		&& m_pMotion->GetType() != MOTIONTYPE_TACKLE)
	{// ENbNº°Ä¢È¢

		if (m_pMotion->GetType() == MOTIONTYPE_THROW_RDY &&
			m_fThrowChargeCtr < MAX_THROW_CHARGE)
		{// `[W
			m_pBall->AddSpeedMag(ADD_THROW_CHARGE);
			m_fThrowChargeCtr += ADD_THROW_CHARGE;
		}

		CPlayer::MOTIONTYPE type = MOTIONTYPE_THROW_RDY;
		MotionSet(type, MotionBrend::NAME_MAP.at(type));
	}

	// °
	if (ControllManage(CONTROL_THROW_RELEASE)
		&& m_pMotion->GetType() == MOTIONTYPE_THROW_RDY)
	{
		if (m_pBall != nullptr)
		{
			Throw();
		}
	}

	// Lb`\¦
	if (ControllManage(CONTROL_CATCH)
		&& (m_pMotion->GetType() != MOTIONTYPE_CATCH_RDY)
		&& (m_pMotion->GetType() != MOTIONTYPE_TACKLE)
		&& m_nCatchCtr == 0)
	{
		// oA£é
		CPlayer::MOTIONTYPE type = MOTIONTYPE_CATCH_RDY;
		MotionSet(type, MotionBrend::NAME_MAP.at(type));
		m_nCatchCtr = CT_CATCH;
	}

	// ^[QeBO
	if (ControllManage(CONTROL_TARGET))
	{
		m_bTarget = !m_bTarget;
		Target();

		if (m_pBall != nullptr)
		{
			if (m_pBall->GetTarget() != nullptr)
			{
				m_pBall->Target();
			}
		}
	}

	// Ýè
	SetJump(bJump);
}

//============================
// ÊÜè«Á½©mF
//============================
void CPlayer::RankOK()
{
	CScene* pScene = CManager::GetInstance()->GetScene();
	int nRank = -1;

	if (pScene->GetMode() == CScene::MODE_GAME &&
		CManager::GetInstance()->GetResult() != CManager::RT_NONE)
	{
		bool bRank = false;

		//ÊÝè
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
// ûüL[Ç
//============================
bool CPlayer::ControllManage(CONTROL_MANAGE key)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad* pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	switch (key)
	{
	case CONTROL_FRONT:
		if ((pInputKeyboard->GetPress(DIK_W) == true && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_UP, m_nIdx) ||
			pInputGamepad->GetGameStickLYPress(m_nIdx) > 0)
		{
			return true;
		}
		break;

	case CONTROL_BACK:
		if ((pInputKeyboard->GetPress(DIK_S) == true && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_DOWN, m_nIdx) ||
			pInputGamepad->GetGameStickLYPress(m_nIdx) < 0)
		{
			return true;
		}
		break;

	case CONTROL_RIGHT:
		if ((pInputKeyboard->GetPress(DIK_D) == true && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, m_nIdx) ||
			pInputGamepad->GetGameStickLXPress(m_nIdx) < 0)
		{
			return true;
		}
		break;

	case CONTROL_LEFT:
		if ((pInputKeyboard->GetPress(DIK_A) == true && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, m_nIdx) ||
			pInputGamepad->GetGameStickLXPress(m_nIdx) > 0)
		{
			return true;
		}
		break;

	case CONTROL_JUMP:
		if ((pInputKeyboard->GetTrigger(DIK_SPACE) == true && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, m_nIdx))
		{
			return true;
		}
		break;

	case CONTROL_TACKLE:
		if ((pInputKeyboard->GetTrigger(DIK_LSHIFT) == true && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetTrigger(CInputGamepad::BUTTON_Y, m_nIdx))
		{
			return true;
		}
		break;

	case CONTROL_THROW:
		if ((pInputKeyboard->GetPress(DIK_RCONTROL) == true && m_nControlIdx == m_nIdx) ||
			(pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) == true && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RB, m_nIdx))
		{
			return true;
		}
		break;

	case CONTROL_THROW_RELEASE:
		if ((pInputKeyboard->GetRelease(DIK_RCONTROL) == true && m_nControlIdx == m_nIdx) ||
			(pInputMouse->GetRelease(CInputMouse::BUTTON_LEFT) == true && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetRelease(CInputGamepad::BUTTON_RB, m_nIdx))
		{
			return true;
		}
		break;

	case CONTROL_CATCH:
		if ((pInputKeyboard->GetPress(DIK_RSHIFT) == true && m_nControlIdx == m_nIdx) || 
			(pInputMouse->GetTrigger(CInputMouse::BUTTON_RIGHT) && m_nControlIdx == m_nIdx) ||
			pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LB, m_nIdx))
		{
			return true;
		}
		break;

	case CONTROL_TARGET:
		if ((pInputGamepad->GetTrigger(CInputGamepad::BUTTON_X, m_nIdx) && m_nControlIdx == m_nIdx) ||
			pInputKeyboard->GetTrigger(DIK_V))
		{
			return true;
		}
		break;

	default:
		break;
	}

	return false;
}
