//============================
//
//カメラの処理(camera.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "debugproc.h"
#include "game.h"
#include "map.h"
#include "cursor.h"
#include "block.h"

//============================
//マクロ定義
//============================
#define CAMERA_SPEED (10.0f)				//移動スピード
#define CAMERA_ROTSPEED (0.01f)				//回転スピード
#define CAMERA_DISTANCE (1000.0f)			//視点・注視点の距離
#define CAMERA_INIT_POS_Y (100.0f)			//視点・注視点のy初期値
#define CAMERA_RADJT (400.0f)				//注視点のずらし値
#define CAMERA_VISDISTANCE (40000.0f)		//可視距離の値
#define CAMERA_LIMIT_Z (D3DX_PI * 0.48f)	//z軸回転の限界値
#define CAMERA_VR_ROT (0.9f)	//視点と注視点の角度
#define CAMERA_Y_STALK (300.0f)	//プレイヤ―のy値に合わせて上を向くスタート値

//============================
// コンストラクタ
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
// デストラクタ
//============================
CCamera::~CCamera()
{

}

//============================
//カメラの初期化処理
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
//カメラの終了処理
//============================
void CCamera::Uninit()
{

}

//============================
//カメラの更新処理
//============================
void CCamera::Update()
{
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	CInputMouse *pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse != nullptr)
	{//カメラがあれば

		if (pMouse->GetChangePos().z >= 10.0f)
		{//マウス左ボタン
			m_fAngle += 0.1f;
		}
		else if (pMouse->GetChangePos().z <= -10.0f)
		{//マウス左ボタン
			m_fAngle -= 0.1f;
		}

		D3DXVECTOR3 mousepos = pMouse->GetPos();

		pDebug->Print("マウスの位置 = X:%f \n", mousepos.x);
		pDebug->Print("マウスの位置 = Y:%f \n", mousepos.y);
		pDebug->Print("マウスの位置 = Z:%f \n", mousepos.z);
		pDebug->Print("カメラのアングル：%f \n", m_fAngle);
	}

	//プレイヤーに追従する
	if (CGame::MapFlag())
	{
		EditStalk();
	}
	else
	{
		PlayerStalk();
	}
}

//============================
//カメラの設定処理
//============================
void CCamera::SetCamera()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成[透視投影]
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,
								CAMERA_VISDISTANCE);

	////プロジェクションマトリックスの作成[平行投影]-2dっぽい
	//D3DXMatrixOrthoLH(&m_mtxProjection,
	//				(float)SCREEN_WIDTH,
	//				(float)SCREEN_HEIGHT,
	//				10.0f,
	//				CAMERA_VISDISTANCE);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION,&m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
						&m_posV,			//視点
						&m_posR,			//注視点
						&m_vecU);		//上方向ベクトル

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//===========================
// 回り込み処理
//===========================
void CCamera::CameraStalk()
{
	//差分
	float fDiff = 0.0f;
	fDiff = m_rotDest.y - m_rot.y;

	//PIからはみ出ていた場合の補正
	if (fDiff < -D3DX_PI)
	{
		fDiff += D3DX_PI * 2;
	}
	else if (fDiff > D3DX_PI)
	{
		fDiff += -D3DX_PI * 2;
	}
	
	m_rot.y += fDiff * 0.01f;

	//PIからはみ出ていた場合の補正
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
// エディットカーソル追従処理
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
	//m_posV.y = pos.y + CAMERA_Y_STALK;
	m_posV.y = pos.y + (sinf(m_fAngle) * m_fDis);
	m_posV.z = pos.z + (cosf(m_fAngle) * m_fDis);
}

//============================
// プレイヤー追従処理
//============================
void CCamera::PlayerStalk(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 pos = pPlayer->GetPosOld();
	m_posR.x = pos.x;
	m_posR.z = pos.z;
	m_posR.y = pos.y;

	m_posV.x = pos.x;
	//m_posV.y = pos.y + CAMERA_Y_STALK;
	m_posV.y = pos.y + (sinf(m_fAngle) * m_fDis);
	m_posV.z = pos.z + (cosf(m_fAngle) * m_fDis);
}
