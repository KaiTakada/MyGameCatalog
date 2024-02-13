//===============================================
//
// プレイヤー(player.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "manager.h"	//ほぼ確で必要
#include "game.h"		//ほぼ確で必要
#include "player.h"
#include "input.h"		//操作
#include "camera.h"		//移動方向
#include "sound.h"		//演出等に必要
#include "parts.h"		//親子関係
#include "motion.h"		//モーション
#include "Field.h"		//地面との当たり判定
#include "weapon.h"		//武器
#include "gauge.h"		//HPゲージ
#include "Xmodel.h"		//モデル
#include "growselecter.h"		//進化・成長シーン
#include "cursor.h"		//Aimカーソル
#include "debugproc.h"	//デバッグ
#include "state_life.h"		//状態管理

//デバッグ用(おそらく消す)
#include "bullet.h"
#include "block.h"
#include "enemy.h"
#include "effect.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "experience.h"

//============================
// マクロ定義
//============================
#define NUM_GETAREA (300.0f)		//取得領域の半径
#define NUM_HYGETAREA (100.0f)		//取得加速領域の半径
#define NUM_GETAREA_SPEED (8.0f)	//取得領域の引き寄せ速度
#define NUM_COLL (30.0f)			//当たり判定半径
#define NUM_HP (100.0f)				//HP量
#define NUM_ATTACK (1.0f)			//攻撃力
#define NUM_SPEED (0.5f)			//移動量
#define NUM_SPEED_STOP (0.0001f)	//移動量を0にする条件値
#define NUM_HEART (50.0f)			//心臓位置
#define NUM_JUMP (15.0f)			//ジャンプ力
#define NUM_GRAV (0.5f)				//重力
#define MOTION_FILE "data\\SET\\MOTION\\motion_player.txt"		//モーションファイルパス
#define READ_PSIZE (256)			//読込ポインタサイズ
#define ARMR_IDX (8)				//右手パーツ
#define HP_HEIGHT (170.0f)			//HPゲージの高さ
#define EXP_MAX (5.0f)				//expゲージのMaxの初期値
#define EXP_MAX_MGNFC (1.2f)		//expゲージのMaxに掛ける倍率
#define DAMAGE_CT (1)				//ダメージ無敵時間

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
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
// デストラクタ
//============================
CPlayer::~CPlayer()
{

}

//====================================
// プレイヤーの初期化(デフォ)
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

	//ファイル読込＆パーツ生成・初期化
	ReadFile();

	//タイプ設定
	SetType(TYPE_PLAYER);

	return S_OK;
}

//====================================
// 2Dプレイヤーの初期化(位置オバロ)
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

	//モーションの生成、初期化
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//ファイル読込＆パーツ生成・初期化
	ReadFile();

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	//武器を持たせる
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

	//HPゲージの生成
	if (m_pGaugeHP != nullptr)
	{
		m_pGaugeHP->Uninit();
		m_pGaugeHP = nullptr;
	}

	m_pGaugeHP = CGauge::Create(D3DXVECTOR3(m_pos.x, m_pos.y + HP_HEIGHT, m_pos.z), m_apPart[0]->GetRot(), D3DXVECTOR3(60.0f, 10.0f, 0.0f));
	
	//expゲージの生成
	if (m_pGaugeExp != nullptr)
	{
		m_pGaugeExp->Uninit();
		m_pGaugeExp = nullptr;
	}

	m_pGaugeExp = CGauge::Create(D3DXVECTOR3(m_pos.x, m_pos.y + HP_HEIGHT - 15.0f, m_pos.z), m_apPart[0]->GetRot(), D3DXVECTOR3(60.0f, 5.0f, 0.0f));
	m_pGaugeExp->SetRatio(0.0f);
	m_pGaugeExp->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	//expゲージの生成
	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

	CInputMouse *pInputMouse = CManager::GetInputMouse();

	D3DXVECTOR3 posCsr = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	posCsr = pInputMouse->GetPos();

	m_pCursor = CCursor::Create(posCsr);

	//状態の生成
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//タイプ設定
	SetType(TYPE_PLAYER);

	return S_OK;
}

//============================
// プレイヤーの終了
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

	//終了
	Release();
}

//============================
// プレイヤーの更新
//============================
void CPlayer::Update(void)
{
	//座標取得
	float fRotMove = GetRot().y;	//現在の向き
	float fRotDest = GetRot().y;	//目標の向き
	float fRotDiff = 0.0f;			//差分

	D3DXVECTOR3 pos = GetPos();
	m_posOld = pos;

	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInputGamepad();

	//移動入力
	MoveOperate(&fRotDest);
	RotOperate(&fRotDest);

	fRotDiff = fRotDest - fRotMove;
	RotAdj(fRotDiff);

	if (pInputKeyboard->GetPress(DIK_Q))
	{//[ Q ]キーでジャンプ
		m_rot.y += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_E))
	{//[ E ]キーでジャンプ
		m_rot.y -= 0.01f;
	}

	if ((pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_A,0))
		&& m_bJump == false)
	{//[ - ]キーでジャンプ
		m_move.y += NUM_JUMP;
		m_bJump = true;
		m_pMotion->Set(MOTIONTYPE_JUMP);
	}

	if (pInputKeyboard->GetPress(DIK_RETURN) ||
		pInputMouse->GetPress(CInputMouse::BUTTON_LEFT) ||
		pInputGamepad->GetGameStickRXPress(0) != 0 ||
		pInputGamepad->GetGameStickRYPress(0) != 0)
	{//ENTERキーで弾発射

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

	//重力
	m_move.y -= NUM_GRAV;

	//移動量を適用
	pos += m_move;

	//高さを取得する
	CollisionField(pos);

	//落下死救済
	if (pos.y <= -50.0f)
	{
		pos.y = -50.0f;
		m_move.y = 0.0f;
		m_bJump = false;
	}

	//武器の座標更新
	if (m_pWeapon != nullptr)
	{
		if (m_apPart[ARMR_IDX] != nullptr)
		{
			//武器の持ち手を右手に納まる位置にする
			CXModel *pXmodel = CManager::GetXModel();
			D3DXVECTOR3 vtx = pXmodel->GetVtxMax(m_apPart[ARMR_IDX]->GetIdxModel());
			m_pWeapon->SetPos(D3DXVECTOR3(vtx.x * 0.7f, 0.0f, 0.0f));
		}
		
		m_pWeapon->Update();

		if (m_param.fSpeedFire != 0.0f)
		{//クールダウンLv.で武器進化
			if (m_param.fSpeedFire >= 5.0f && m_pWeapon->GetWpnType() == CWeapon::WPNTYPE_REVOLVER)
			{
				m_pWeapon->SetWpnType(CWeapon::WPNTYPE_AK);
				m_param.fSpeedFire = 0.0f;
			}
			m_pWeapon->SetCooltimeMin(int(m_param.fSpeedFire));
		}
	}

	//HPゲージの座標更新
	if (m_pGaugeHP != nullptr)
	{
		m_pGaugeHP->SetPos(D3DXVECTOR3(pos.x, pos.y + HP_HEIGHT, pos.z));
	
		m_pGaugeHP->Update();
	}
	
	//expゲージの座標更新
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
		{//DAMAGEからNORMALに戻ったら

			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(D3DXMATERIAL));

			//パーツ一つずつ塗装する
			for (int nCntPrt = 0; nCntPrt < MAX_PLAYER_PARTS; nCntPrt++)
			{
				if (m_apPart[nCntPrt] != nullptr)
				{
					m_apPart[nCntPrt]->SetMat(mat);
				}
			}
		}
	}
	
	//移動量を更新(減衰させる)慣性
	m_move.x += (0 - m_move.x) * 0.1f;
	m_move.z += (0 - m_move.z) * 0.1f;

	if (m_move.x <= NUM_SPEED_STOP && m_move.x >= -NUM_SPEED_STOP)
	{//規定値以下で移動量を0に
		m_move.x = 0.0f;
	}
	if (m_move.z <= NUM_SPEED_STOP && m_move.z >= -NUM_SPEED_STOP)
	{//規定値以下で移動量を0に
		m_move.z = 0.0f;
	}

	//引き寄せ
	PullItem();

	if (!m_pMotion->IsFinish())
	{
		m_pMotion->Update();
	}
}

//============================
// プレイヤーの描画
//============================
void CPlayer::Draw(void)
{
	//描画
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//デバッグ
	CDebugProc *pDebug = CManager::GetDebugProc();
	pDebug->Print("プレイヤー情報\n");
	pDebug->Print("現在の方向:%f,%f,%f\n", m_rot.x, m_rot.y, m_rot.z);
}

//============================
// プレイヤーの生成
//============================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CPlayer *pPlayer = nullptr;

	//オブジェクト生成&初期化
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
// 移動操作
//============================
void CPlayer::MoveOperate(float *pRotDest)
{
	//キーボード取得
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
		{//上かつ左キーを押したとき
			m_move.x += sinf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.25f * D3DX_PI;
			bInput = true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, 0) || pInputGamepad->GetGameStickLXPress(0) > 0)
		{//上かつ右キーを押したとき
			m_move.x += sinf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + -0.25f * D3DX_PI;
			bInput = true;
		}
		else
		{//上キーのみを押したとき
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
		{//下かつ左キーを押したとき
			m_move.x += sinf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.75f * D3DX_PI;
			bInput = true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true ||
			pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, 0) || pInputGamepad->GetGameStickLXPress(0) > 0)
		{//下かつ右キーを押したとき
			m_move.x += sinf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + -0.75f * D3DX_PI;
			bInput = true;
		}
		else
		{//下キーのみを押したとき
			m_move.x += sinf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 1.0f * D3DX_PI;
			bInput = true;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_LEFT, 0) || pInputGamepad->GetGameStickLXPress(0) < 0)
	{//左キーのみを押したとき
		m_move.x += sinf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + 0.5f * D3DX_PI;
		bInput = true;
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_RIGHT, 0) || pInputGamepad->GetGameStickLXPress(0) > 0)
	{//右キーのみを押したとき
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
// 移動操作
//============================
void CPlayer::RotOperate(float *pfRotDest)
{
	//キーボード取得
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
// 向きの修正
//============================
void CPlayer::RotAdj(float fRotDiff)
{
	//角度の値を修正する
	RotLimit(&fRotDiff);

	m_rot.y += fRotDiff * 0.1f;		//移動方向(角度)の補正

	//角度の値を修正する
	RotLimit(&m_rot.y);
}

//============================
// アイテム引き寄せ
//============================
void CPlayer::PullItem(void)
{
	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float fSize = m_param.fGetArea;		//プレイヤーの半径

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject *pObj;

			//オブジェクトを取得
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULLチェック
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_ITEM ||
					type == CObject::TYPE_EXP)
				{//アイテムだったら
					Objpos = pObj->GetPos();
					Objsize = pObj->GetSize();

					float fObjsize = (Objsize.x + Objsize.z) * 0.5f;		//オブジェクトの半径

					float fLength = fSize + fObjsize;		//2点間の長さ
					float fColl = 0.0f;						//当たり判定範囲

					//判定
					fColl = hypotf((m_pos.x - Objpos.x), (m_pos.z - Objpos.z));

					if (fColl <= fLength)
					{
						D3DXVECTOR3 ObjMove = pObj->GetMove();

						float fRotMove = atan2f(ObjMove.x, ObjMove.y);							//現在の移動方向(角度)
						float fRotDest = atan2f(Objpos.x - m_pos.x, Objpos.z - m_pos.z);		//目標の移動方向(角度)
						float fRotDiff = fRotDest - fRotMove;									//目標の移動方向までの差分

						RotLimit(&fRotDiff);

						fRotMove += fRotDiff * 1.0f;

						RotLimit(&fRotMove);

						float fSpeed = NUM_GETAREA_SPEED;

						if ((fLength - fColl) <= NUM_HYGETAREA)
						{//近かったら速く
							fSpeed *= 2;
						}

						if ((fLength - fColl) <= NUM_COLL && type == CObject::TYPE_EXP)
						{//触れていたら取得
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
// 経験値ゲージ設定
//============================
void CPlayer::SetExp(const float fExp)
{
	m_fExp = fExp;

	CntExp(0);
}

//============================
// 経験値加算
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

	//加算結果の割合で設定
	m_pGaugeExp->SetRatio(fRatio);
}

//============================
// ファイル読み込み
//============================
void CPlayer::ReadFile(void)
{
	char *pComp = new char[READ_PSIZE];				//ゴミ
	char *pFilepass[MAX_PLAYER_PARTS] = {};		//ファイルパス
	D3DXVECTOR3 pos[MAX_PLAYER_PARTS];			//プリセット位置
	D3DXVECTOR3 rot[MAX_PLAYER_PARTS];			//プリセット向き
	int aParent[MAX_PLAYER_PARTS];				//親モデルの有無
	int nNumParts = 0;					//パーツ総数
	D3DXVECTOR3 **ppPos = nullptr;		//位置
	D3DXVECTOR3 **ppRot = nullptr;		//向き
	int nNumKey = 0;					//キー数
	int nFrame = 0;						//フレーム数
	int nLoop = 0;						//ループ [ 0:しない / 1:する ]

	int nKeyCtr = 0;			//モーション読込時のキーカウンター
	int nKeySetCtr = 0;			//モーション読込時のキーセットカウンター
	int nMotionCtr = 0;			//モーション数
	CMotion::INFO *info = new CMotion::INFO[16];		//モーション情報

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
			{// これのあとコメント
				fgets(pComp, READ_PSIZE, pFile);
				continue;
			}

			if (strcmp(pComp, "NUM_MODEL") == 0)
			{//総数取得
				fscanf(pFile, "%s %d", pComp, &nNumParts);
				m_nNumModel = nNumParts;
			}
			else if (strcmp(pComp, "MODEL_FILENAME") == 0)
			{//ファイル読込
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
			{//オフセット情報取得開始
				int nCntSet = 0;

				do
				{
					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "PARTSSET") == 0)
					{//パーツ情報取得開始
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

						//取得終了で加算
						nCntSet++;
					}

				} while (strcmp(pComp, "END_CHARACTERSET") != 0);
			}
			else if (strcmp(pComp, "MOTIONSET") == 0)
			{
				do
				{//モーション情報を読む

					fscanf(pFile, "%s", pComp);

					if (strncmp(pComp, "#", 1) == 0)
					{// これのあとコメント
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
					{//ループ設定取得
						fscanf(pFile, "%s %d", pComp, &nLoop);

						info[nMotionCtr].bLoop = (nLoop == 0 ? false : true);
					}
					else if (strcmp(pComp, "NUM_KEY") == 0)
					{//キー数取得
						fscanf(pFile, "%s %d", pComp, &nNumKey);

						info[nMotionCtr].nNumKey = nNumKey;

						//モーション時に必要な数だけ位置情報を生成
						//生成内容 : ppPos[キー数][パーツ数]
						//生成内容 : ppRot[キー数][パーツ数]
						ppPos = new D3DXVECTOR3*[nNumKey];
						ppRot = new D3DXVECTOR3*[nNumKey];

						for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
						{
							ppPos[nCntMotKey] = new D3DXVECTOR3[nNumParts];
							ppRot[nCntMotKey] = new D3DXVECTOR3[nNumParts];
						}
					}
					else if (strcmp(pComp, "KEYSET") == 0)
					{//キーセット情報取得開始
						do
						{
							fscanf(pFile, "%s", pComp);

							if (strncmp(pComp, "#", 1) == 0)
							{// これのあとコメント
								fgets(pComp, READ_PSIZE, pFile);
								continue;
							}
							else if (strcmp(pComp, "END_KEYSET") == 0)
							{
								//取得終了で加算
								nKeySetCtr++;
								nKeyCtr = 0;

								break;
							}
							else if (strcmp(pComp, "FRAME") == 0)
							{//フレーム数取得
								fscanf(pFile, "%s %d", pComp, &nFrame);

								info[nMotionCtr].aKeyInfo[nKeySetCtr].nFrame = nFrame;
							}
							else if (strcmp(pComp, "KEY") == 0)
							{//パーツ情報取得開始
								while (TRUE)
								{
									fscanf(pFile, "%s", pComp);

									if (strncmp(pComp, "#", 1) == 0)
									{// これのあとコメント
										fgets(pComp, READ_PSIZE, pFile);
										continue;
									}
									else if (strcmp(pComp, "END_KEY") == 0)
									{
										//取得終了で加算
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
	{//ファイル読込に失敗
		return;
	}

	//モデルの生成(全パーツ分)
	for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
	{
		m_apPart[nCntCrt] = CParts::Create(pFilepass[nCntCrt], pos[nCntCrt], rot[nCntCrt]);
	}

	//親モデルの設定(全パーツ分)
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

	//モーション情報設定
	m_pMotion->SetInfo(info, nMotionCtr);

	delete[] pComp;		//ゴミ
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
// ダメージ処理
//============================
void CPlayer::Damage(float fDamege)
{
	D3DXMATERIAL mat;
	ZeroMemory(&mat, sizeof(D3DXMATERIAL));

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->Get() != CState_Life::STATE_NORMAL)
		{//ダメージを受けられない状態なら
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

	//パーツ一つずつ塗装する
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
		//死んだらどうしようか
		CManager::SetResult(CManager::RT_LOSE);
	}

	m_pStateLife->Set(CState_Life::STATE_DAMAGE, DAMAGE_CT);
}

//============================
// デバッグキー
//============================
void CPlayer::DebugKey(CInputKeyboard *pInputKeyboard)
{
	//if (pInputKeyboard->GetPress(DIK_B) || (pInputKeyboard->GetTrigger(DIK_N)))
	//{//Bキーでブロック生成
	//	CBlock::Create(GetPos(), GetRot());
	//}

	if (pInputKeyboard->GetTrigger(DIK_K))
	{//Kで武器チェン
		int nType = (m_pWeapon->GetWpnType() + 1) % CWeapon::WPNTYPE_MAX;
		m_pWeapon->SetWpnType(CWeapon::WPNTYPE(nType));
	}

	if (pInputKeyboard->GetTrigger(DIK_I))
	{//左Shiftでアイテム生成
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_E))
	{//左ShiftでHP減少

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
// 角度の値を修正する
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
//ブロックとの当たり判定(矩形)
//============================
void CPlayer::CollisionField(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject *pObj;

			//オブジェクトを取得
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULLチェック
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_FIELD)
				{//地面だったら
					
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

	//座標設定(更新)
	SetPos(pos);
}
