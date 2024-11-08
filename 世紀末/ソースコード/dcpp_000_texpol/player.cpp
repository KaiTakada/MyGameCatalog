//===============================================
//
// vC[(player.cpp)
// Author ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "manager.h"	//ÙÚmÅKv
#include "game.h"		//ÙÚmÅKv
#include "player.h"
#include "input.h"		//ì
#include "camera.h"		//Ú®ûü
#include "sound.h"		//oÉKv
#include "parts.h"		//eqÖW
#include "motion.h"		//[V
#include "Field.h"		//nÊÆÌ½è»è
#include "weapon.h"		//í
#include "gauge.h"		//HPQ[W
#include "Xmodel.h"		//f
#include "growselecter.h"		//i»E¬·V[
#include "cursor.h"		//AimJ[\
#include "debugproc.h"	//fobO
#include "state_life.h"		//óÔÇ

//fobOp(¨»ç­Á·)
#include "bullet.h"
#include "block.h"
#include "enemy.h"
#include "effect.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "experience.h"

//============================
// }Nè`
//============================
#define NUM_GETAREA (300.0f)		//æ¾ÌæÌ¼a
#define NUM_HYGETAREA (100.0f)		//æ¾Á¬ÌæÌ¼a
#define NUM_GETAREA_SPEED (8.0f)	//æ¾ÌæÌø«ñ¹¬x
#define NUM_COLL (30.0f)			//½è»è¼a
#define NUM_HP (100.0f)				//HPÊ
#define NUM_ATTACK (1.0f)			//UÍ
#define NUM_SPEED (0.5f)			//Ú®Ê
#define NUM_SPEED_STOP (0.0001f)	//Ú®Êð0É·éðl
#define NUM_HEART (50.0f)			//SÊu
#define NUM_JUMP (15.0f)			//WvÍ
#define NUM_GRAV (0.5f)				//dÍ
#define MOTION_FILE "data\\SET\\MOTION\\motion_player.txt"		//[Vt@CpX
#define READ_PSIZE (256)			//Ç|C^TCY
#define ARMR_IDX (8)				//Eèp[c
#define HP_HEIGHT (170.0f)			//HPQ[WÌ³
#define EXP_MAX (5.0f)				//expQ[WÌMaxÌúl
#define EXP_MAX_MGNFC (1.2f)		//expQ[WÌMaxÉ|¯é{¦
#define DAMAGE_CT (1)				//_[W³GÔ

//============================
// ÃIoÏé¾
//============================

//============================
// RXgN^
//============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	m_nNumModel = 0;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = m_pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};
	m_fHeart = 0.0f;
	m_bJump = false;
	m_pMotion = nullptr;
	m_pWeapon = nullptr;
	m_pGaugeHP = nullptr;
	m_pGaugeExp = nullptr;
	m_pCursor = nullptr;
	ZeroMemory(&m_param, sizeof(m_param));
	m_fExp = 0;
	m_fExpMax = 0;
	m_pStateLife = nullptr;

	for (int nCntPart = 0; nCntPart < MAX_PLAYER_PARTS; nCntPart++)
	{
		m_apPart[nCntPart] = nullptr;
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
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeart = NUM_HEART;
	m_param.fLife = NUM_HP;
	m_param.fLifeMax = NUM_HP;
	m_param.fSpeed = NUM_SPEED;
	m_param.fGetArea = NUM_GETAREA;
	m_param.fColl = NUM_COLL;
	m_fExpMax = EXP_MAX;

	//t@CÇp[c¶¬Eú»
	ReadFile();

	//^CvÝè
	SetType(TYPE_PLAYER);

	return S_OK;
}

//====================================
// 2DvC[Ìú»(ÊuIo)
//====================================
HRESULT CPlayer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nNumParts)
{
	m_pos = pos;
	m_posOld = pos;
	m_rot = rot;
	m_nNumModel = nNumParts;
	m_fHeart = NUM_HEART;
	m_param.fLife = NUM_HP;
	m_param.fLifeMax = NUM_HP;
	m_param.fAttack = NUM_ATTACK;
	m_param.fSpeed = NUM_SPEED;
	m_param.fGetArea = NUM_GETAREA;
	m_param.fColl = NUM_COLL;
	m_fExpMax = EXP_MAX;

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

	//íð½¹é
	if (m_pWeapon != nullptr)
	{
		return E_FAIL;
	}
	
	m_pWeapon = CWeapon::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_pWeapon != nullptr)
	{
		if (m_apPart[ARMR_IDX] != nullptr)
		{
			float fposX = m_apPart[ARMR_IDX]->GetMaxVtx().x * 0.5f;
			m_pWeapon->SetParent(m_apPart[ARMR_IDX]);
			m_pWeapon->SetPos(D3DXVECTOR3(fposX, 0.0f, 0.0f));
		}
	
		m_pWeapon->SetWpnType(CWeapon::WPNTYPE_REVOLVER);
	}

	//HPQ[WÌ¶¬
	if (m_pGaugeHP != nullptr)
	{
		m_pGaugeHP->Uninit();
		m_pGaugeHP = nullptr;
	}

	m_pGaugeHP = CGauge::Create(D3DXVECTOR3(m_pos.x, m_pos.y + HP_HEIGHT, m_pos.z), m_apPart[0]->GetRot(), D3DXVECTOR3(60.0f, 10.0f, 0.0f));
	
	//expQ[WÌ¶¬
	if (m_pGaugeExp != nullptr)
	{
		m_pGaugeExp->Uninit();
		m_pGaugeExp = nullptr;
	}

	m_pGaugeExp = CGauge::Create(D3DXVECTOR3(m_pos.x, m_pos.y + HP_HEIGHT - 15.0f, m_pos.z), m_apPart[0]->GetRot(), D3DXVECTOR3(60.0f, 5.0f, 0.0f));
	m_pGaugeExp->SetRatio(0.0f);
	m_pGaugeExp->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	//expQ[WÌ¶¬
	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

	CInputMouse *pInputMouse = CManager::GetInputMouse();

	D3DXVECTOR3 posCsr = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	posCsr = pInputMouse->GetPos();

	m_pCursor = CCursor::Create(posCsr);

	//óÔÌ¶¬
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//^CvÝè
	SetType(TYPE_PLAYER);

	return S_OK;
}

//============================
// vC[ÌI¹
//============================
void CPlayer::Uninit(void)
{
	for (int nCntPrt = 0; nCntPrt < MAX_PLAYER_PARTS; nCntPrt++)
	{
		if (m_apPart[nCntPrt] != nullptr)
		{
			m_apPart[nCntPrt]->Uninit();
			//delete m_apPart[nCntPrt];
			m_apPart[nCntPrt] = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pWeapon != nullptr)
	{
		m_pWeapon->Uninit();
		m_pWeapon = nullptr;
	}

	if (m_pGaugeHP != nullptr)
	{
		m_pGaugeHP->Uninit();
		m_pGaugeHP = nullptr;
	}
	
	if (m_pGaugeExp != nullptr)
	{
		m_pGaugeExp->Uninit();
		m_pGaugeExp = nullptr;
	}

	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
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
	float fRotMove = GetRot().y;	//»ÝÌü«
	float fRotDest = GetRot().y;	//ÚWÌü«
	float fRotDiff = 0.0f;			//·ª

	D3DXVECTOR3 pos = GetPos();
	m_posOld = pos;

	//L[{[hæ¾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInputGamepad();

	//Ú®üÍ
	MoveOperate(&fRotDest);
	RotOperate(&fRotDest);

	fRotDiff = fRotDest - fRotMove;
	RotAdj(fRotDiff);

	if (pInputKeyboard->GetPress(DIK_Q))
	{//[ Q ]L[ÅWv
		m_rot.y += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_E))
	{//[ E ]L[ÅWv
		m_rot.y -= 0.01f;
	}

	if ((pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_A,0))
		&& m_bJump == false)
	{//[ - ]L[ÅWv
		m_move.y += NUM_JUMP;
		m_bJump = true;
		m_pMotion->Set(MOTIONTYPE_JUMP);
	}

	if (pInputKeyboard->GetPress(DIK_RETURN) ||
		pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) ||
		pInputGamepad->GetGameStickRXPress(0) != 0 ||
		pInputGamepad->GetGameStickRYPress(0) != 0)
	{//ENTERL[Åe­Ë

		m_pWeapon->Attack(m_rot,int(m_param.fAttack));
		
		if (m_pWeapon->GetWpnType() == CWeapon::WPNTYPE_REVOLVER)
		{
			m_pMotion->Set(MOTIONTYPE_FIRE_REV);
		}
		else if (m_pWeapon->GetWpnType() == CWeapon::WPNTYPE_AK)
		{
			m_pMotion->Set(MOTIONTYPE_FIRE_AK);
		}
	}
	
	DebugKey(pInputKeyboard);

	//dÍ
	m_move.y -= NUM_GRAV;

	//Ú®ÊðKp
	pos += m_move;

	//³ðæ¾·é
	CollisionField(pos);

	//º~Ï
	if (pos.y <= -50.0f)
	{
		pos.y = -50.0f;
		m_move.y = 0.0f;
		m_bJump = false;
	}

	//íÌÀWXV
	if (m_pWeapon != nullptr)
	{
		if (m_apPart[ARMR_IDX] != nullptr)
		{
			//íÌ¿èðEèÉ[ÜéÊuÉ·é
			CXModel *pXmodel = CManager::GetXModel();
			D3DXVECTOR3 vtx = pXmodel->GetVtxMax(m_apPart[ARMR_IDX]->GetIdxModel());
			m_pWeapon->SetPos(D3DXVECTOR3(vtx.x * 0.7f, 0.0f, 0.0f));
		}
		
		m_pWeapon->Update();

		if (m_param.fSpeedFire != 0.0f)
		{//N[_ELv.Åíi»
			if (m_param.fSpeedFire >= 5.0f && m_pWeapon->GetWpnType() == CWeapon::WPNTYPE_REVOLVER)
			{
				m_pWeapon->SetWpnType(CWeapon::WPNTYPE_AK);
				m_param.fSpeedFire = 0.0f;
			}
			m_pWeapon->SetCooltimeMin(int(m_param.fSpeedFire));
		}
	}

	//HPQ[WÌÀWXV
	if (m_pGaugeHP != nullptr)
	{
		m_pGaugeHP->SetPos(D3DXVECTOR3(pos.x, pos.y + HP_HEIGHT, pos.z));
	
		m_pGaugeHP->Update();
	}
	
	//expQ[WÌÀWXV
	if (m_pGaugeExp != nullptr)
	{
		m_pGaugeExp->SetPos(D3DXVECTOR3(pos.x, pos.y + HP_HEIGHT - 15.0f, pos.z));

		m_pGaugeExp->Update();
	}

	if (m_pStateLife != nullptr)
	{
		CState_Life::STATE state = m_pStateLife->Get();
	
		m_pStateLife->Update();

		if (state == CState_Life::STATE_DAMAGE &&
			m_pStateLife->Get() == CState_Life::STATE_NORMAL)
		{//DAMAGE©çNORMALÉßÁ½ç

			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(D3DXMATERIAL));

			//p[cêÂ¸Âh·é
			for (int nCntPrt = 0; nCntPrt < MAX_PLAYER_PARTS; nCntPrt++)
			{
				if (m_apPart[nCntPrt] != nullptr)
				{
					m_apPart[nCntPrt]->SetMat(mat);
				}
			}
		}
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

	//ø«ñ¹
	PullItem();

	if (!m_pMotion->IsFinish())
	{
		m_pMotion->Update();
	}
}

//============================
// vC[Ì`æ
//============================
void CPlayer::Draw(void)
{
	//`æ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//foCXÌæ¾
	D3DXMATRIX mtxRot, mtxTrans;		//vZp}gbNX

	//[h}gbNXÌú»
	D3DXMatrixIdentity(&m_mtxWorld);

	//ü«ð½f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//Êuð½f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//[h}gbNXÌÝè
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//fobO
	CDebugProc *pDebug = CManager::GetDebugProc();
	pDebug->Print("vC[îñ\n");
	pDebug->Print("»ÝÌûü:%f,%f,%f\n", m_rot.x, m_rot.y, m_rot.z);
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
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputGamepad *pInputGamepad = CManager::GetInputGamepad();
	CCamera *pCamera= CManager::GetCamera();
	bool bInput = false;

	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	if (pInputKeyboard->GetPress(DIK_S) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_DOWN, 0) || pInputGamepad->GetGameStickLYPress(0) < 0)
	{
		if (pInputKeyboard->GetPress(DIK_A) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, 0) || pInputGamepad->GetGameStickLXPress(0) < 0)
		{//ã©Â¶L[ðµ½Æ«
			m_move.x += sinf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.25f * D3DX_PI;
			bInput = true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, 0) || pInputGamepad->GetGameStickLXPress(0) > 0)
		{//ã©ÂEL[ðµ½Æ«
			m_move.x += sinf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + -0.25f * D3DX_PI;
			bInput = true;
		}
		else
		{//ãL[ÌÝðµ½Æ«
			m_move.x += sinf(cameraRot.y + 1.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 1.0f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.0f * D3DX_PI;
			bInput = true;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_UP, 0) || pInputGamepad->GetGameStickLYPress(0) > 0)
	{
		if (pInputKeyboard->GetPress(DIK_A) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, 0) || pInputGamepad->GetGameStickLXPress(0) < 0)
		{//º©Â¶L[ðµ½Æ«
			m_move.x += sinf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.75f * D3DX_PI;
			bInput = true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, 0) || pInputGamepad->GetGameStickLXPress(0) > 0)
		{//º©ÂEL[ðµ½Æ«
			m_move.x += sinf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + -0.75f * D3DX_PI;
			bInput = true;
		}
		else
		{//ºL[ÌÝðµ½Æ«
			m_move.x += sinf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 1.0f * D3DX_PI;
			bInput = true;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, 0) || pInputGamepad->GetGameStickLXPress(0) < 0)
	{//¶L[ÌÝðµ½Æ«
		m_move.x += sinf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + 0.5f * D3DX_PI;
		bInput = true;
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, 0) || pInputGamepad->GetGameStickLXPress(0) > 0)
	{//EL[ÌÝðµ½Æ«
		m_move.x += sinf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + -0.5f * D3DX_PI;
		bInput = true;
	}

	if (bInput == true)
	{
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
// Ú®ì
//============================
void CPlayer::RotOperate(float *pfRotDest)
{
	//L[{[hæ¾
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInputGamepad();
	bool bInput = false;

	if (m_pCursor == nullptr || pInputMouse == nullptr)
	{
		return;
	}

	D3DXVECTOR3 pos = m_pCursor->GetPos();
	pos += pInputMouse->GetChangePos();
	m_pCursor->SetPos(pos);
	m_rot.y = m_pCursor->GetRot().y;
}

//============================
// ü«ÌC³
//============================
void CPlayer::RotAdj(float fRotDiff)
{
	//pxÌlðC³·é
	RotLimit(&fRotDiff);

	m_rot.y += fRotDiff * 0.1f;		//Ú®ûü(px)Ìâ³

	//pxÌlðC³·é
	RotLimit(&m_rot.y);
}

//============================
// ACeø«ñ¹
//============================
void CPlayer::PullItem(void)
{
	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fSize = m_param.fGetArea;		//vC[Ì¼a

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject *pObj;

			//IuWFNgðæ¾
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULL`FbN
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_ITEM ||
					type == CObject::TYPE_EXP)
				{//ACe¾Á½ç
					Objpos = pObj->GetPos();
					Objsize = pObj->GetSize();

					float fObjsize = (Objsize.x + Objsize.z) * 0.5f;		//IuWFNgÌ¼a

					float fLength = fSize + fObjsize;		//2_ÔÌ·³
					float fColl = 0.0f;						//½è»èÍÍ

					//»è
					fColl = hypotf((m_pos.x - Objpos.x), (m_pos.z - Objpos.z));

					if (fColl <= fLength)
					{
						D3DXVECTOR3 ObjMove = pObj->GetMove();

						float fRotMove = atan2f(ObjMove.x, ObjMove.y);							//»ÝÌÚ®ûü(px)
						float fRotDest = atan2f(Objpos.x - m_pos.x, Objpos.z - m_pos.z);		//ÚWÌÚ®ûü(px)
						float fRotDiff = fRotDest - fRotMove;									//ÚWÌÚ®ûüÜÅÌ·ª

						RotLimit(&fRotDiff);

						fRotMove += fRotDiff * 1.0f;

						RotLimit(&fRotMove);

						float fSpeed = NUM_GETAREA_SPEED;

						if ((fLength - fColl) <= NUM_HYGETAREA)
						{//ß©Á½ç¬­
							fSpeed *= 2;
						}

						if ((fLength - fColl) <= NUM_COLL && type == CObject::TYPE_EXP)
						{//GêÄ¢½çæ¾
							//CExperience *pExp = pObj;
						}

						ObjMove.x = sinf(fRotMove + 1.0f * D3DX_PI) * fSpeed;		//x
						ObjMove.z = cosf(fRotMove + 1.0f * D3DX_PI) * fSpeed;		//y
						(Objpos.y > 50.0f) ? ObjMove.y = -fSpeed : ObjMove.y = fSpeed;	//y
						pObj->SetMove(ObjMove);
					}
				}
			}
		}
	}
}

//============================
// o±lQ[WÝè
//============================
void CPlayer::SetExp(const float fExp)
{
	m_fExp = fExp;

	CntExp(0);
}

//============================
// o±lÁZ
//============================
void CPlayer::CntExp(const float fExp)
{
	m_fExp += fExp;

	float fRatio = m_fExp / m_fExpMax;

	if (fRatio >= 1.0f)
	{
		CGrowSelecter *pGrow = CGrowSelecter::Create();

		if (FAILED(CGame::SetGrow(pGrow)))
		{
			pGrow->Uninit();
		}

		m_fExpMax *= EXP_MAX_MGNFC;
	}

	//ÁZÊÌÅÝè
	m_pGaugeExp->SetRatio(fRatio);
}

//============================
// t@CÇÝÝ
//============================
void CPlayer::ReadFile(void)
{
	char *pComp = new char[READ_PSIZE];				//S~
	char *pFilepass[MAX_PLAYER_PARTS] = {};		//t@CpX
	D3DXVECTOR3 pos[MAX_PLAYER_PARTS];			//vZbgÊu
	D3DXVECTOR3 rot[MAX_PLAYER_PARTS];			//vZbgü«
	int aParent[MAX_PLAYER_PARTS];				//efÌL³
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

	for (int nCntNull = 0; nCntNull < MAX_PLAYER_PARTS; nCntNull++)
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
			m_apPart[nCntPrt]->SetParent(m_apPart[aParent[nCntPrt]]);
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
void CPlayer::Damage(float fDamege)
{
	D3DXMATERIAL mat;
	ZeroMemory(&mat, sizeof(D3DXMATERIAL));

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->Get() != CState_Life::STATE_NORMAL)
		{//_[Wðó¯çêÈ¢óÔÈç
			return;
		}
	}

	m_param.fLife -= fDamege;

	if (m_param.fLife > m_param.fLifeMax)
	{
		m_param.fLife = m_param.fLifeMax;
	}

	if (m_pStateLife != nullptr)
	{
		mat = m_pStateLife->GetMat();
	}

	//p[cêÂ¸Âh·é
	for (int nCntPrt = 0; nCntPrt < MAX_PLAYER_PARTS; nCntPrt++)
	{
		if (m_apPart[nCntPrt] != nullptr)
		{
			m_apPart[nCntPrt]->SetMat(mat);
		}
	}

	float fRatio = m_param.fLife / m_param.fLifeMax;

	m_pGaugeHP->SetRatio(fRatio);

	if (m_param.fLife <= 0.0f)
	{
		//ñ¾çÇ¤µæ¤©
		CManager::SetResult(CManager::RT_LOSE);
	}

	m_pStateLife->Set(CState_Life::STATE_DAMAGE, DAMAGE_CT);
}

//============================
// fobOL[
//============================
void CPlayer::DebugKey(CInputKeyboard *pInputKeyboard)
{
	//if (pInputKeyboard->GetPress(DIK_B) || (pInputKeyboard->GetTrigger(DIK_N)))
	//{//BL[ÅubN¶¬
	//	CBlock::Create(GetPos(), GetRot());
	//}

	if (pInputKeyboard->GetTrigger(DIK_K))
	{//KÅí`F
		int nType = (m_pWeapon->GetWpnType() + 1) % CWeapon::WPNTYPE_MAX;
		m_pWeapon->SetWpnType(CWeapon::WPNTYPE(nType));
	}

	if (pInputKeyboard->GetTrigger(DIK_I))
	{//¶ShiftÅACe¶¬
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_E))
	{//¶ShiftÅHP¸­

		float fRatio = m_pGaugeHP->GetRatio();

		fRatio -= 0.01f;

		m_pGaugeHP->SetRatio(fRatio);
	}

	if (pInputKeyboard->GetPress(DIK_I))
	{
		CntExp(1.0f);
	}
}

//============================
// pxÌlðC³·é
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
//ubNÆÌ½è»è(é`)
//============================
void CPlayer::CollisionField(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject *pObj;

			//IuWFNgðæ¾
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULL`FbN
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_FIELD)
				{//nÊ¾Á½ç
					
					CField *pField = (CField*)pObj;
					fHeight = pField->GetHeight(pos);
				}
			}
		}
	}

	if (pos.y < fHeight && m_posOld.y >= fHeight)
	{
		pos.y = fHeight;
		m_move.y = 0.0f;
		m_bJump = false;

		if (m_pMotion->GetType() == MOTIONTYPE_JUMP)
		{
			m_pMotion->Set(MOTIONTYPE_GROUND);
		}
	}

	//ÀWÝè(XV)
	SetPos(pos);
}
