//===============================================
//
// プレイヤー(player.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "player.h"
#include "manager.h"	//ほぼ確で必要
#include "game.h"		//ほぼ確で必要
#include "input.h"			//操作
#include "camera.h"			//移動方向
#include "sound.h"			//演出等に必要
#include "parts.h"			//親子関係
#include "motion.h"			//モーション
#include "Field.h"			//地面との当たり判定
#include "wp_boost.h"		//ブースト
#include "gauge.h"			//HPゲージ
#include "Xmodel.h"			//モデル
#include "growselecter.h"		//進化・成長シーン
#include "debugproc.h"		//デバッグ
#include "state_life.h"		//状態管理
#include "ef_smoke.h"		//煙演出

//デバッグ用(おそらく消す)
#include "bullet.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "experience.h"
#include "effect.h"

//============================
// マクロ定義
//============================
#define NUM_GETAREA (300.0f)		//取得領域の半径
#define NUM_HYGETAREA (100.0f)		//取得加速領域の半径
#define NUM_GETAREA_SPEED (8.0f)	//取得領域の引き寄せ速度
#define NUM_COLL (30.0f)			//当たり判定半径
#define NUM_HP (1.0f)				//HP量
#define NUM_ATTACK (1.0f)			//攻撃力
#define NUM_SPEED (1.0f)			//移動量
#define NUM_SPEED_STOP (0.0001f)	//移動量を0にする条件値
#define NUM_HEART (50.0f)			//心臓位置
#define NUM_JUMP (15.0f)			//ジャンプ力
#define NUM_BOOST (40.0f)			//ブースト力
#define NUM_GRAV (0.5f)				//重力
#define MOTION_FILE "data\\SET\\MOTION\\motion_player.txt"		//モーションファイルパス
#define READ_PSIZE (256)			//読込ポインタサイズ
#define ARMR_IDX (17)				//右手パーツ
#define HP_HEIGHT (170.0f)			//HPゲージの高さ
#define EXP_MAX (5.0f)				//expゲージのMaxの初期値
#define EXP_MAX_MGNFC (1.2f)		//expゲージのMaxに掛ける倍率
#define DAMAGE_CT (1)				//ダメージ無敵時間

//============================
// 定数定義
//============================
int g_anParts[MAX_PLAYER_WEAPON] = { 14,17 };		//両足の番号設定

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
	m_size = MAX_PLAYER_SIZE;
	m_posOld = m_pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = m_rot;
	m_mtxWorld = {};
	m_fHeart = 0.0f;
	m_bJump = false;
	m_bBoost = false;
	m_pMotion = nullptr;
	m_pGaugeBoost = nullptr;
	ZeroMemory(&m_param, sizeof(m_param));
	m_pStateLife = nullptr;
	
	for (int nCntBst = 0; nCntBst < sizeof(m_pBoost) / sizeof(m_pBoost[0]); nCntBst++)
	{
		m_pBoost[nCntBst] = nullptr;
	}

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
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_size = MAX_PLAYER_SIZE;
	m_rotDest = m_rot;
	m_fHeart = NUM_HEART;
	m_param.fLife = NUM_HP;
	m_param.fLifeMax = NUM_HP;
	m_param.fSpeed = NUM_SPEED;
	m_param.fColl = NUM_COLL;

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

	//モーションの生成、初期化
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//ファイル読込＆パーツ生成・初期化
	ReadFile();

	//int nNumParts = 0;
	//CMotion::INFO *pInfo;

	////パーツモデルファイル読込
	//char **ppParts = m_pMotion->ReadParts(MOTION_FILE, &nNumParts);

	////モデルの生成(全パーツ分)
	//for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
	//{
	//	m_apPart[nCntCrt] = CParts::Create(ppParts[nCntCrt]);
	//}

	//m_pMotion->ReadSetUp(MOTION_FILE, m_apPart);
	//m_pMotion->ReadMotions(MOTION_FILE, pInfo);

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	//武器を持たせる	
	for (int nCntBst = 0; nCntBst < sizeof(m_pBoost) / sizeof(m_pBoost[0]); nCntBst++)
	{
		if (m_pBoost[nCntBst] != nullptr)
		{
			m_pBoost[nCntBst] = nullptr;
		}

		m_pBoost[nCntBst] = CBoost::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (m_pBoost[nCntBst] != nullptr)
		{
			if (m_apPart[g_anParts[nCntBst]] != nullptr)
			{//定義位置のパーツが存在することを確認

				float fposX = m_apPart[g_anParts[nCntBst]]->GetMaxVtx().x * 0.5f;
				m_pBoost[nCntBst]->SetParent(m_apPart[g_anParts[nCntBst]]);
				m_pBoost[nCntBst]->SetPos(D3DXVECTOR3(fposX, 0.0f, 0.0f));
			}
		}
	}

	//HPゲージの生成
	if (m_pGaugeBoost != nullptr)
	{
		m_pGaugeBoost->Uninit();
		m_pGaugeBoost = nullptr;
	}

	m_pGaugeBoost = CGauge::Create(D3DXVECTOR3(m_pos.x, m_pos.y + HP_HEIGHT, m_pos.z), m_apPart[0]->GetRot(), D3DXVECTOR3(60.0f, 10.0f, 0.0f));
	
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

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

	for (int nCntBst = 0; nCntBst < sizeof(m_pBoost) / sizeof(m_pBoost[0]); nCntBst++)
	{
		if (m_pBoost[nCntBst] != nullptr)
		{
			m_pBoost[nCntBst]->Uninit();
			m_pBoost[nCntBst] = nullptr;
		}
	}

	if (m_pGaugeBoost != nullptr)
	{
		m_pGaugeBoost->Uninit();
		m_pGaugeBoost = nullptr;
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
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	//移動入力
	fRotDest = m_rotDest.y;

	MoveOperate2D(&fRotDest);
	RotOperate(&fRotDest);

	fRotDiff = fRotDest - fRotMove;
	RotAdj(fRotDiff);

	//ブーストorジャンプ
	if (pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputGamepad->GetPadTrigger(CInputGamepad::BUTTON_A,0))
	{//[ - ]キー

		if (((m_bJump == true) || m_pMotion->GetType() == MOTIONTYPE_SLIDING) && (m_bBoost == false))
		{//ジャンプ使用済み  orスライディング中なら
			//かつブーストゲージが残っていれば
			 //ブースト
			m_bJump = true;
			m_bBoost = true;
			m_move.y = NUM_JUMP * 1.5f;
			m_move.x *= 1.2f;
			m_pMotion->Set(MOTIONTYPE_BOOST);
			m_pBoost[0]->Attack(m_move, 0);
			m_pBoost[1]->Attack(m_move, 0);

			//煙演出
			CEf_Smoke * pSmoke = CEf_Smoke::Create(m_pos);
			pSmoke->SetLife(10);
			pSmoke->SetLifeMax(10);
		}
		else if (m_bJump == false)
		{//ジャンプ未使用
			m_bJump = true;
			m_move.y += NUM_JUMP;
			m_pMotion->Set(MOTIONTYPE_JUMP);

			//煙演出
			CEf_Smoke * pSmoke = CEf_Smoke::Create(m_pos);
			pSmoke->SetLife(10);
			pSmoke->SetLifeMax(10);
		}
	}
	
	//スライディング
	if ((pInputKeyboard->GetPress(DIK_S) ||
		pInputGamepad->GetPress(CInputGamepad::BUTTON_Y, 0))
		&& (m_bJump == false) && (m_pMotion->GetType() != MOTIONTYPE_SLIDING))
	{
		{//下キー押下＆ジャンプしていない＆スライディング中ではない
			m_move.x = sinf(fRotDest * D3DX_PI) * NUM_BOOST;		//x
			m_pMotion->Set(MOTIONTYPE_SLIDING);

			//しゃがみサイズに変更
			m_size = D3DXVECTOR3(MAX_PLAYER_SIZE.x, MAX_PLAYER_SIZE.y * 0.5f, MAX_PLAYER_SIZE.z);
		}
	}

#if _DEBUG
	DebugKey(pInputKeyboard);
#endif

	CPlayer::MOTIONTYPE Mtype = MOTIONTYPE(m_pMotion->GetType());

	if (Mtype != MOTIONTYPE_SLIDING && Mtype != MOTIONTYPE_BOOST)
	{
		//サイズに通常に修正
		m_size = MAX_PLAYER_SIZE;
	}
	else
	{//スライディング中だったら
	
		if (Mtype == MOTIONTYPE_SLIDING && m_pMotion->GetTotalCtr() % 8 == 0)
		{//8カウント毎に煙生成

		 //煙演出
			CEf_Smoke * pSmoke = CEf_Smoke::Create(m_pos);
			pSmoke->SetLife(30);
			pSmoke->SetLifeMax(30);
		}

		if (m_pMotion->GetTotalCtr() % 3 == 0)
		{//残像的な青エフェクト

			CEffect *pEffect = CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeart, m_pos.z));
			pEffect->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
			pEffect->SetLifeMax(60);
			pEffect->SetLife(60);

			if (Mtype == MOTIONTYPE_SLIDING)
			{//スライディングは青
				pEffect->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			}
			if (Mtype == MOTIONTYPE_BOOST)
			{//ブーストは赤
				pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}
	}

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
		m_bBoost = false;
		//static_assert(true, "沼");
	}

	//武器の更新
	for (int nCntBst = 0; nCntBst < sizeof(m_pBoost) / sizeof(m_pBoost[0]); nCntBst++)
	{
		if (m_pBoost[nCntBst] != nullptr)
		{
			m_pBoost[nCntBst]->Update();
		}
	}

	//HPゲージの座標更新
	if (m_pGaugeBoost != nullptr)
	{
		m_pGaugeBoost->SetPos(D3DXVECTOR3(pos.x, pos.y + HP_HEIGHT, pos.z));
	
		m_pGaugeBoost->Update();
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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
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
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- プレイヤー情報 ---\n");
	pDebug->Print("現在の方向:%f\n", m_rot.y);
	pDebug->Print("現在の方向:%f %f %f\n", m_pos.x, m_pos.y, m_pos.z);
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
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CCamera *pCamera= CManager::GetInstance()->GetCamera();
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
// 2D移動操作
//============================
void CPlayer::MoveOperate2D(float *pRotDest)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	bool bInput = false;

	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	if (m_pMotion->GetType() != MOTIONTYPE_SLIDING)
	{
		if (pInputKeyboard->GetPress(DIK_A) == true ||
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
// 移動操作
//============================
void CPlayer::RotOperate(float *pfRotDest)
{
	//キーボード取得
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	bool bInput = false;
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

	m_pGaugeBoost->SetRatio(fRatio);

	if (m_param.fLife <= 0.0f)
	{
		//死んだらどうしようか
		CManager::GetInstance()->SetResult(CManager::RT_LOSE);
	}

	m_pStateLife->Set(CState_Life::STATE_DAMAGE, DAMAGE_CT);
}

//============================
// モーション情報取得 (.hでは前方宣言のみのため、中身は.cppに書く)
//============================
CMotion *CPlayer::GetMotion(void)
{
	return m_pMotion;
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
		int nType = (m_pBoost[0]->GetWpnType() + 1) % CBoost::WPNTYPE_MAX;
		m_pBoost[0]->SetWpnType(CWeapon::WPNTYPE(nType));
	}

	if (pInputKeyboard->GetTrigger(DIK_I))
	{//左Shiftでアイテム生成
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_E))
	{//左ShiftでHP減少

		float fRatio = m_pGaugeBoost->GetRatio();

		fRatio -= 0.01f;

		m_pGaugeBoost->SetRatio(fRatio);
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
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_FIELD)
				{//アイテムだったら

					CField *pField = (CField*)pObject;
					fHeight = pField->GetHeight(pos);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	if (pos.y < fHeight && m_posOld.y >= fHeight)
	{//着地時

		if (m_bJump || m_bBoost)
		{
			D3DXVECTOR3 move = { 3.0f, 0.0f, 0.0f };

			//煙演出
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
		m_bBoost = false;
	}

	//座標設定(更新)
	SetPos(pos);
}
