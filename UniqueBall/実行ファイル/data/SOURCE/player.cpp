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
#include "camera_player.h"		//移動方向
#include "sound.h"			//演出等に必要
#include "parts.h"			//親子関係
#include "motion.h"			//モーション
#include "enemy.h"			//タックルの当たり判定
#include "Field.h"			//地面との当たり判定
#include "wall.h"			//壁との当たり判定
#include "wp_ball.h"		//球
#include "gauge.h"			//HPゲージ
#include "Xmodel.h"			//モデル
#include "debugproc.h"		//デバッグ
#include "state_life.h"		//状態管理
#include "ef_smoke.h"		//煙演出
#include "wp_ball.h"		//ボールの当たり判定
#include "ui_life.h"		//体力UI
#include "objectBillboard.h"	//チャージ演出
#include "texture.h"			//チャージ演出
#include "Anim.h"				//チャージ演出
#include "target_mark.h"		//チャージ演出
#include "particle.h"			//チャージ演出

#include "playerManager.h"		//固有マネージャ

//デバッグ用(おそらく消す)
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// 定数定義
//============================
namespace
{
	const float NUM_GETAREA(100.0f);		//取得領域の半径
	const float NUM_HYGETAREA(100.0f);		//取得加速領域の半径
	const float NUM_GETAREA_SPEED(8.0f);	//取得領域の引き寄せ速度
	const float NUM_COLL(30.0f);			//当たり判定半径
	const float NUM_ATTACK(1.0f);			//攻撃力
	const float NUM_SPEED(1.0f);			//移動量
	const float NUM_SPEED_STOP(0.0001f);	//移動量を0にする条件値
	const float NUM_HEART(70.0f);			//心臓位置
	const float NUM_JUMP(15.0f);			//ジャンプ力
	const float NUM_BOOST(40.0f);			//ブースト力
	const float NUM_GRAV(0.5f);				//重力
	const float MAX_THROW_CHARGE(1.0f);		//チャージ上限
	const float ADD_THROW_CHARGE(0.01f);	//チャージ
	const int HANDR_IDX = 12;			//右手Idx
	const int READ_PSIZE(256);			//読込ポインタサイズ
	const int DAMAGE_CT(30);			//ダメージ無敵時間
	const int WAIT_JC(3);			//ジャストキャッチ猶予時間
	const int CT_CATCH(30);			//キャッチCT
	const int JC_FRAME(CT_CATCH - WAIT_JC);			//ジャストキャッチ猶予
	const int CT_TACKLE(30);			//タックルCT
	const int CT_JUMP2(30);			//2段ジャンプCT
	const int NUM_HP(5);			//HP量
	const D3DXCOLOR COL_SMOKE(D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.5f));
	const char* MOTION_FILE = "data\\SET\\MOTION\\motion_player.txt";		//モーションファイルパス

	const char* UV_FILE_BODY[mylib_const::MAX_PLAYER] =
	{
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
		"data\\TEXTURE\\PLAYER\\human_body_use1.png",
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
		"data\\TEXTURE\\PLAYER\\human_body_use.png",
	};		//UVテクスチャパス
}

namespace MotionBrend
{
	const std::map<CPlayer::MOTIONTYPE, int> NAME_MAP =
	{
		{CPlayer::MOTIONTYPE::MOTIONTYPE_NEUTRAL,		4},	// 待機
		{CPlayer::MOTIONTYPE::MOTIONTYPE_WALK,			3},	// 歩き
		{CPlayer::MOTIONTYPE::MOTIONTYPE_DASH,			3},	// ダッシュ
		{CPlayer::MOTIONTYPE::MOTIONTYPE_JUMP,			3},	// ジャンプ
		{CPlayer::MOTIONTYPE::MOTIONTYPE_JUMP2,			2},	// 2段ジャンプ
		{CPlayer::MOTIONTYPE::MOTIONTYPE_GROUND,		3},	// 着地
		{CPlayer::MOTIONTYPE::MOTIONTYPE_THROW_RDY,		2},	// 投げ用構える
		{CPlayer::MOTIONTYPE::MOTIONTYPE_THROW,			2},	// 投げる
		{CPlayer::MOTIONTYPE::MOTIONTYPE_CATCH_RDY,		2},	// キャッチ用構える
		{CPlayer::MOTIONTYPE::MOTIONTYPE_CATCH,			2},	// キャッチ
		{CPlayer::MOTIONTYPE::MOTIONTYPE_TACKLE,		2},	// タックル
		{CPlayer::MOTIONTYPE::MOTIONTYPE_DAMAGE,		2},	// ダメージ
		{CPlayer::MOTIONTYPE::MOTIONTYPE_NEUTRAL_BALL,	2},	// 待機(球持ち)
		{CPlayer::MOTIONTYPE::MOTIONTYPE_DASH_BALL,		2},	// ダッシュ(球持ち)
		{CPlayer::MOTIONTYPE::MOTIONTYPE_JUMP_BALL,		2}	// ジャンプ(球持ち)
	};
}

//============================
// 定数定義
//============================

//============================
// 静的メンバ変数宣言
//============================
int CPlayer::m_nControlIdx = 0;		//キーボードで操作するプレイヤー

//============================
// コンストラクタ
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
	m_nIdx = 0;		//自身の番号

	m_bDelete = false;
	m_bTarget = false;
	m_pTargetMark = nullptr;

	if (CPlayerManager* pPlayerManager = CPlayerManager::GetInstance())
	{
		pPlayerManager->Add(this);
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

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	//状態の生成
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//体力UIの生成
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->Uninit();
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(static_cast<int>(m_param.fLifeMax), m_nIdx);
	m_pLifeUI->SetColAll(mylib_const::PERSONAL_COLOR[m_nIdx]);

	//タイプ設定
	SetType(TYPE_PLAYER);

	return S_OK;
}

//====================================
// プレイヤーの初期化(位置オバロ)
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

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	//状態の生成
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//体力UIの生成
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->Uninit();
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(static_cast<int>(m_param.fLifeMax), m_nIdx);
	m_pLifeUI->SetColAll(mylib_const::PERSONAL_COLOR[m_nIdx]);

	//タイプ設定
	SetType(TYPE_PLAYER);

	return S_OK;
}

//============================
// プレイヤーの終了
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

	//オブジェクト
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->SetDeath(true);
		m_pLifeUI = nullptr;
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
	float fRotMove = m_rot.y;	//現在の向き
	float fRotDest = m_rot.y;	//目標の向き
	float fRotDiff = 0.0f;			//差分

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

	//ボタンアクション
	ButtonAction();

	//移動入力
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
		//サイズに通常に修正
		m_size = MAX_PLAYER_SIZE;
	}
	else
	{//スライディング中だったら

	}

	MotionEffect();
	ReduceCounter();

	//重力
	float fGrav = 1.0f;
	if (Mtype == MOTIONTYPE_JUMP2)
	{
		fGrav = 0.5f;
	}

	m_move.y -= NUM_GRAV * fGrav;

	//移動量を適用
	pos += m_move;

	//ベタ打ち壁
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

	//高さを取得する
	CollisionWall();
	CObjectCharacter::CollisionField(pos);
	Target();

	if (Mtype == MOTIONTYPE_TACKLE)
	{
		TackleCollision();
	}

	//落下死救済
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

	//武器の更新
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

	//待機チェック
	SetNeutral();

	//順位確認
	RankOK();

	//歩き時胸の位置上下
	if ((Mtype == MOTIONTYPE_DASH || Mtype == MOTIONTYPE_WALK) && m_pMotion->GetTotalCtr() % 20 == 0)
	{
		//int nFrame = m_pMotion->GetTotalCtr() % 20;
		//m_fHeart += nFrame - 10;
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
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxTrans);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
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
		{//上かつ左キーを押したとき
			m_move.x += sinf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.25f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		else if (ControllManage(CONTROL_RIGHT))
		{//上かつ右キーを押したとき
			m_move.x += sinf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		else
		{//上キーのみを押したとき
			m_move.x += sinf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.0f * D3DX_PI) * m_param.fSpeed;		//y
			bInput = true;
		}
		
		*pRotDest = cameraRot.y + 0.0f * D3DX_PI;
	}
	else if (ControllManage(CONTROL_FRONT))
	{
		if (ControllManage(CONTROL_LEFT))
		{//下かつ左キーを押したとき
			m_move.x += sinf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + -0.25f * D3DX_PI;
			bInput = true;
		}
		else if (ControllManage(CONTROL_RIGHT))
		{//下かつ右キーを押したとき
			m_move.x += sinf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + -0.75f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.25f * D3DX_PI;
			bInput = true;
		}
		else
		{//下キーのみを押したとき
			m_move.x += sinf(cameraRot.y + 1.0f * D3DX_PI) * m_param.fSpeed;		//x
			m_move.z += cosf(cameraRot.y + 1.0f * D3DX_PI) * m_param.fSpeed;		//y
			*pRotDest = cameraRot.y + 0.0f * D3DX_PI;
			bInput = true;
		}
	}
	else if (ControllManage(CONTROL_LEFT))
	{//左キーのみを押したとき
		m_move.x += sinf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + 0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + -0.5f * D3DX_PI;
		bInput = true;
	}
	else if (ControllManage(CONTROL_RIGHT))
	{//右キーのみを押したとき
		m_move.x += sinf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		m_move.z += cosf(cameraRot.y + -0.5f * D3DX_PI) * m_param.fSpeed;
		*pRotDest = cameraRot.y + 0.5f * D3DX_PI;
		bInput = true;
	}

	if (m_pMotion->IsFinish() ||
		m_pMotion->GetType() == MOTIONTYPE_NEUTRAL ||
		m_pMotion->GetType() == MOTIONTYPE_NEUTRAL_BALL)
	{//モーション設定
	
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

	//待機状態になる
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
// 方向操作
//============================
void CPlayer::RotOperate(float * /*pfRotDest*/)
{
	//マウスとPAD取得
	CInputMouse *pMouse = CManager::GetInstance()->GetInputMouse();

	//マウスとpadのスティックの移動量でm_rotDestを更新
	if (pMouse == nullptr)
	{//カメラがあれば
		return;
	}
}

//============================
// ファイル読み込み(motion)
//============================
void CPlayer::ReadFile(void)
{
	char *pComp = new char[READ_PSIZE];				//ゴミ
	char *pFilepass[MAX_PARTS] = {};		//ファイルパス
	D3DXVECTOR3 pos[MAX_PARTS];			//プリセット位置
	D3DXVECTOR3 rot[MAX_PARTS];			//プリセット向き
	int aParent[MAX_PARTS];				//親モデルの有無
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
			if (m_apPart[aParent[nCntPrt]] != nullptr)
			{
				m_apPart[nCntPrt]->SetParent(m_apPart[aParent[nCntPrt]]);
			}
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
void CPlayer::Damage(int nDamege)
{
	D3DXMATERIAL mat;
	ZeroMemory(&mat, sizeof(D3DXMATERIAL));

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->GetState() != CState_Life::STATE_NORMAL)
		{//ダメージを受けられない状態なら
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

	//パーツ一つずつ塗装する
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

		//死んだら
		CPlayer *pPlayer = nullptr;
		CScene *pScene = CManager::GetInstance()->GetScene();
		switch (pScene->GetMode())
		{
		case CScene::MODE_TUTORIAL:
			//α版はここで自動リスポーンする
			CObject::SetDeath(true);	//死亡フラグ
		
			pPlayer = pScene->GetElem(m_nIdx);
			pPlayer = CPlayer::Create(mylib_const::DEFVEC3, mylib_const::DEFVEC3);
			pPlayer->SetIdx(m_nIdx);
			pPlayer->SetMember(static_cast<my_Identity::eMember>(m_nIdx + 1));
			pScene->SetPlayer(pPlayer,m_nIdx);
			break;

		case CScene::MODE_GAME:
			//α版はここで自動リスポーンする
			m_bDelete = true;	//死亡フラグ

			//順位設定
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
// ノックバック
//============================
void CPlayer::Knockback(float fRot)
{
	//与えられた向きの逆に飛ばす
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
// モーション情報取得 (.hでは前方宣言のみのため、中身は.cppに書く)
//============================
CMotion *CPlayer::GetMotion(void)
{
	return m_pMotion;
}

//============================
// デバッグキー
//============================
void CPlayer::Debug()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_K))
	{//Kでボールスポーン
		CBall::Create(D3DXVECTOR3(0.0f, 150.0f, 0.0f));
	}

	if (pInputKeyboard->GetTrigger(DIK_E) && m_nControlIdx == m_nIdx)
	{//操作するプレイヤー変更
		m_nControlIdx++;
		if (m_nControlIdx > mylib_const::MAX_PLAYER) m_nControlIdx = 0;
		//if (m_nControlIdx > CPlayerManager::GetPlayer(m_nControlIdx)) m_nControlIdx = 0;
	}

	// デバッグ表示
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- プレイヤー情報 ---\n");
	pDebug->Print("現在の位置:%f %f %f\n", m_pos.x, m_pos.y, m_pos.z);
	pDebug->Print("現在の方向:%f %f %f\n", m_rot.x, m_rot.y, m_rot.z);

	if (m_pBall != nullptr)
	{
		pDebug->Print("現在のボールの速度倍率:%f\n", m_pBall->GetSpeedMag());
	}

	pDebug->Print("モーションタイプ:%d\n", m_pMotion->GetType());
	pDebug->Print("モーションカウンタ:%d\n", m_pMotion->GetCounter());
	pDebug->Print("モーション全体カウンタ:%d\n", m_pMotion->GetTotalCtr());
	pDebug->Print("モーション現在キー:%d\n", m_pMotion->GetKey());
	pDebug->Print("モーション前回キー:%d\n", m_pMotion->GetKeyOld());
	pDebug->Print("ターゲティング状態:%d\n", m_bTarget);
}

//============================
//ブロックとの当たり判定(矩形)
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
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_WALL)
				{//アイテムだったら

					CWall *pWall= (CWall*)pObject;
					bCol = pWall->CollisionChara(m_pos, m_posOld, &Cross);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	if (bCol)
	{//当たっていたら
		m_pos.x = Cross.x;
		m_pos.z = Cross.z;
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}
}

//============================
//ボールとの当たり判定(球)
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
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_BALL)
				{//ボールだったら

					//キャスト
					CBall *pBall = static_cast<CBall*>(pObject);

					if (pBall != nullptr)
					{
						if (pBall->GetParent() != nullptr)
						{
							pObject = pObject->GetNext();
							continue;
						}

						if (!pBall->CollisionCircle(pos, NUM_GETAREA))
						{//ボールに当たっていなかったら
							pObject = pObject->GetNext();
							continue;
						}

						my_Identity::eMember member = pBall->GetMember();

						if (member == m_eMember)		//自軍
						{
						}
						else if (member == my_Identity::MEMBER_NONE || member == my_Identity::MEMBER_MAP)	//無所属
						{
							if (m_pBall == nullptr)
							{
								m_pBall = pBall;
							}
						}
						else	//敵軍
						{
							if (m_pMotion == nullptr)
							{
								pObject = pObject->GetNext();
								continue;
							}

							//キャッチ
							if (m_pMotion->GetType() == MOTIONTYPE_CATCH_RDY)
							{
								if (m_pBall != nullptr)
								{//持っていたら放って
									D3DXMATRIX mtx = m_pBall->GetMtxWorld();
									m_pBall->Throw(D3DXVECTOR3(5.0f, 20.0f, 5.0f), 10.0f);
									m_pBall->SetParent(nullptr);
									m_pBall->SetMember(my_Identity::MEMBER_NONE);
									m_pBall->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
									m_pBall->SetRot(D3DXVECTOR3(mylib_const::DEFVEC3));
									m_pBall = nullptr;
								}

								m_pBall = pBall;

								//演出
								CEffect *pEffect = CEffect::Create(m_pBall->GetWorldPos());
								pEffect->SetLifeMax(10);
								pEffect->SetLife(10);
								pEffect->SetLight(true);

								//ジャスト判定
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
							{//キャッチできずダメージ
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
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	if (m_pBall != nullptr && bHave == false)
	{//ボールを入手した場合

		m_pBall->SetPosOld(mylib_const::DEFVEC3);
		m_pBall->SetPos(mylib_const::DEFVEC3);
		m_pBall->SetMove(mylib_const::DEFVEC3);
		m_pBall->SetParent(m_apPart[HANDR_IDX]);
		m_pBall->SetMember(m_eMember);
	}
}

//============================
//タックル時の当たり判定()
//============================
void CPlayer::TackleCollision()
{
	//前提チェック
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
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

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
							{//ダメージを受けられない状態なら
								pPlayer->Damage(0);
								pPlayer->Knockback(m_rot.y);
							}
						}
					}
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}
}

//============================
// モーション設定
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
// モーション時演出
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
	case MOTIONTYPE_WALK:	//移動
	case MOTIONTYPE_DASH_BALL:
		if (GetJump() || mylib_useful::Vec2Length(GetMove().x, GetMove().z) <= 5.0f)
		{
			break;
		}

		if (m_pMotion->GetTotalCtr() % 6 == 0)
		{//煙エフェクト
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

			//煙演出
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

	case MOTIONTYPE_THROW_RDY:	//投げ用構える
		pEffect = CEffect::Create(m_pBall->GetWorldPos());
		if (pEffect == nullptr) break;
		pEffect->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f) * m_pBall->GetSpeedMag());
		pEffect->SetLifeMax(10);
		pEffect->SetLife(10);
		pEffect->SetColor(D3DXCOLOR(1.0f, 0.3f, 0.3f, 0.2f));
		pEffect->SetLight(true);
		break;

	case MOTIONTYPE_TACKLE:		//タックル
		if (m_pMotion->GetTotalCtr() % 3 == 0)
		{//残像的な青エフェクト
			pEffect = CEffect::Create(GetPosCol());
			if (pEffect == nullptr) break;
			pEffect->SetSize(D3DXVECTOR3(50.0f, 50.0f, 50.0f));
			pEffect->SetLifeMax(60);
			pEffect->SetLife(60);
			pEffect->SetColor(D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f));
			pEffect->SetLight(true);
		}
		break;

	case MOTIONTYPE_JUMP2:		//2段ジャンプ
		if (m_pMotion->GetTotalCtr() % 2 == 0)
		{//残像的な青エフェクト

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
// カウンター減らし
//============================
void CPlayer::ReduceCounter()
{
	if (m_nCatchCtr > 0)
	{//クールタイム中
		m_nCatchCtr--;
	}
	else if (m_nCatchCtr < 0)
	{//ジャスト時
		m_nCatchCtr++;
	}

	if (m_nTackleCtr > 0)
	{//タックルCT中
		m_nTackleCtr--;
	}

	if (m_nJump2Ctr > 0)
	{//2段ジャンプCT中
		m_nJump2Ctr--;
	}
}

//============================
// ボール投げ
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

	//少し上向きにする
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
// ターゲティング
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

	//敵(プレイヤーorBot)がいるかターゲティング範囲角度にいるか
	int i = 0;

	while (CPlayerManager::GetInstance()->GetElem(i) != nullptr)
	{
		pPlayer = CPlayerManager::GetInstance()->GetElem(i);

		if (pPlayer == nullptr)
		{//ヌルチェック
			i++;
			continue;
		}

		if (pPlayer->GetMember() == m_eMember)
		{//自身
			pPlayer = nullptr;
			i++;
			continue;
		}

		pos = pPlayer->GetPos();
		pos.y += pPlayer->GetHeartPos();
		bTag = true;

		break;
	}

	// ターゲットマーク
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

		// ターゲットプレイヤーを見る
		D3DXVECTOR3 rot = GetRot();
		rot.y = mylib_useful::Point2Angle(pos, myPos);
		SetRot(rot);

		// マークを生成
		m_pTargetMark = CTarget_Mark::Create(pos);
		if (m_pTargetMark == nullptr) return;
		m_pTargetMark->SetSize(D3DXVECTOR3(150.0f, 150.0f, 150.0f));
		m_pTargetMark->SetColor(mylib_const::PERSONAL_COLOR[pPlayer->GetIdx()]);
		m_pTargetMark->SetIdx(m_nIdx);
	}
}

//============================
// ニュートラル状態にする
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
		{//DAMAGEからNORMALに戻ったら(色直し)

			//パーツ一つずつ塗装する
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
// UI設定
//============================
void CPlayer::InitUI()
{
	//体力UIの生成
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->SetDeath(true);
		m_pLifeUI = nullptr;
	}

	m_pLifeUI = CUI_Life::Create(static_cast<int>(m_param.fLifeMax), m_nIdx);
	m_pLifeUI->SetColAll(mylib_const::PERSONAL_COLOR[m_nIdx]);
}

//============================
// モデル設定
//============================
void CPlayer::InitModel()
{
	//モデルの生成(全パーツ分)
	for (int nCntCrt = 0; nCntCrt < m_nNumModel; nCntCrt++)
	{
		if (m_apPart[nCntCrt] == nullptr)
		{
			continue;
		}

		m_apPart[nCntCrt]->SetTexPass(UV_FILE_BODY[m_nIdx], m_nIdx);	//テクスチャ差し替え

		std::string file = CManager::GetInstance()->GetXModel()->GetFilename(m_apPart[nCntCrt]->GetIdxModel());
		if (strstr(const_cast<char*>(file.c_str()), "head") == nullptr)
		{//""がなかったら
			m_apPart[nCntCrt]->SetIdxParent(m_nIdx);	//テクスチャ差し替え番号
		}
	}

	//初期モーション
	MotionSet(MOTIONTYPE_NEUTRAL, MotionBrend::NAME_MAP.at(MOTIONTYPE_NEUTRAL));
}

//============================
// ボタンアクション
//============================
void CPlayer::ButtonAction()
{
	//インプット取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad* pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	bool bJump = GetJump();

	// 2段ジャンプorジャンプ
	if (ControllManage(CONTROL_JUMP))
	{// [ - ]キー

		if (bJump == true
			&& !GetJumpSec()
			&& m_pMotion->GetType() != MOTIONTYPE_JUMP2
			&& m_pMotion->GetType() != MOTIONTYPE_TACKLE
			&& m_nJump2Ctr == 0)
		{// ジャンプ使用済み
			SetJumpSec(true);
			m_move.y = NUM_JUMP * 0.5f;
			CPlayer::MOTIONTYPE type = MOTIONTYPE_JUMP2;
			MotionSet(type, MotionBrend::NAME_MAP.at(type));

			// 演出
		}
		else if (bJump == false)
		{// ジャンプ未使用
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

			// 煙演出
			CEf_Smoke* pSmoke = CEf_Smoke::Create(m_pos);
			if (pSmoke != nullptr)
			{
				pSmoke->SetLife(10);
				pSmoke->SetLifeMax(10);
				pSmoke->SetColor(COL_SMOKE);
			}
		}
	}

	// タックル
	if (ControllManage(CONTROL_TACKLE)
		&& m_pMotion->GetType() != MOTIONTYPE_TACKLE
		&& m_nTackleCtr == 0)
	{// [RShift]キー押下＆ジャンプしていない
		float fStickL = pInputGamepad->GetStickLRot(m_nIdx);
		m_move.x = sinf(GetRot().y + fStickL + D3DX_PI * 1.0f) * NUM_BOOST;		//x
		m_move.z = cosf(GetRot().y + fStickL + D3DX_PI * 1.0f) * NUM_BOOST;		//z
		m_nTackleCtr = CT_TACKLE;

		CPlayer::MOTIONTYPE type = MOTIONTYPE_TACKLE;
		MotionSet(type, MotionBrend::NAME_MAP.at(type));

		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_TACKLE);
	}

	// 投げ構え
	if (ControllManage(CONTROL_THROW)
		&& m_pBall != nullptr
		&& m_pMotion->GetType() != MOTIONTYPE_TACKLE)
	{// 右クリック押下＆投げていない

		if (m_pMotion->GetType() == MOTIONTYPE_THROW_RDY &&
			m_fThrowChargeCtr < MAX_THROW_CHARGE)
		{// チャージ
			m_pBall->AddSpeedMag(ADD_THROW_CHARGE);
			m_fThrowChargeCtr += ADD_THROW_CHARGE;
		}

		CPlayer::MOTIONTYPE type = MOTIONTYPE_THROW_RDY;
		MotionSet(type, MotionBrend::NAME_MAP.at(type));
	}

	// 投げ
	if (ControllManage(CONTROL_THROW_RELEASE)
		&& m_pMotion->GetType() == MOTIONTYPE_THROW_RDY)
	{
		if (m_pBall != nullptr)
		{
			Throw();
		}
	}

	// キャッチ構え
	if (ControllManage(CONTROL_CATCH)
		&& (m_pMotion->GetType() != MOTIONTYPE_CATCH_RDY)
		&& (m_pMotion->GetType() != MOTIONTYPE_TACKLE)
		&& m_nCatchCtr == 0)
	{
		// バリア張る
		CPlayer::MOTIONTYPE type = MOTIONTYPE_CATCH_RDY;
		MotionSet(type, MotionBrend::NAME_MAP.at(type));
		m_nCatchCtr = CT_CATCH;
	}

	// ターゲティング
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

	// 設定
	SetJump(bJump);
}

//============================
// 順位決まりきったか確認
//============================
void CPlayer::RankOK()
{
	CScene* pScene = CManager::GetInstance()->GetScene();
	int nRank = -1;

	if (pScene->GetMode() == CScene::MODE_GAME &&
		CManager::GetInstance()->GetResult() != CManager::RT_NONE)
	{
		bool bRank = false;

		//順位設定
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
// 方向キー管理
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
