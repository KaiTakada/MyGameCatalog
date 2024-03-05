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
#include "game.h"
#include "map.h"
#include "cursor.h"
#include "block.h"

//============================
// 定数定義
//============================
namespace
{
	const float CAMERA_SPEED(10.0f);				//移動スピード
	const float CAMERA_ROTSPEED(0.01f);				//回転スピード
	const float CAMERA_DISTANCE(350.0f);			//視点・注視点の距離
	const float CAMERA_INIT_POS_Y(100.0f);			//視点・注視点のy初期値
	const float CAMERA_RADJT(400.0f);				//注視点のずらし値
	const float CAMERA_VISDISTANCE(40000.0f);		//可視距離の値
	const float CAMERA_LIMIT_Z(D3DX_PI * 0.48f);	//z軸回転の限界値
	const float CAMERA_VR_ROT(0.85f);		//視点と注視点の角度
	const float CAMERA_Y_STALK(300.0f);		//プレイヤ―のy値に合わせて上を向くスタート値
	const float POSR_MOVE = 0.005f;			//マウスの移動量と掛ける値
}

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
	PlayerStalk();
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
// Destに追いつこう処理
//===========================
void CCamera::ReviseRot()
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
	
	//m_rot.y += fDiff * 0.01f;
	m_rot.y += fDiff * 1.0f;

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
	m_posV.y = pos.y + (sinf(m_fAngle) * m_fDis);
	m_posV.z = pos.z + (cosf(m_fAngle) * m_fDis);
}

//============================
// プレイヤー追従処理(向きも)
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

	//プレイヤーの向きを追従する
	m_rotDest.y = pPlayer->GetRot().y;

	//注視点
	m_posR.x = pos.x;
	m_posR.z = pos.z;
	m_posR.y = pos.y;
	//m_posR.x = pos.x + (sinf(m_rot.y) * m_fDis);
	//m_posR.z = pos.z + (cosf(m_rot.y) * m_fDis);
	//m_posR.y = pos.y;

	//視点
	m_posV.x = m_posR.x + (sinf(m_rot.y) * m_fDis);
	m_posV.y = m_posR.y + (sinf(m_fAngle) * m_fDis);
	m_posV.z = m_posR.z + (cosf(m_rot.y) * m_fDis);

	RotControll();

	////注視点水平旋回
	//RotHorPosR();

	////注視点垂直旋回
	//RotVerPosR();
	
	////補正
	//ReviseRot();

	//プレイヤーに向きを反映
	pPlayer->SetRot(m_rotDest);
}

//===========================
//注視点水平旋回
//===========================
void CCamera::RotHorPosR()
{
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	//ブーストorジャンプ
	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
	}

	D3DXVECTOR3 mouseMove = pInputMouse->GetChangePos();
	m_rot.y += mouseMove.x * POSR_MOVE;

	//視点の旋回
	m_posR.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.z) * -m_fDis;
	m_posR.y = m_posV.y + sinf(m_rot.z) * -m_fDis;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.z) * -m_fDis;


	if (m_rot.y < -D3DX_PI)
	{//上限下限
		m_rot.y = D3DX_PI;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}
}

//===========================
//注視点垂直旋回
//===========================
void CCamera::RotVerPosR()
{
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();

	//ブーストorジャンプ
	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
	}

	D3DXVECTOR3 mouseMove = pInputMouse->GetChangePos();
	m_rot.z += mouseMove.y * POSR_MOVE;

	//注視点の旋回
	m_posR.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.z) * -m_fDis;
	m_posR.y = m_posV.y + sinf(m_rot.z) * -m_fDis;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.z) * -m_fDis;

	//真上下ギリギリで反転前にストップ
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
//マウスで向きを調整
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
	{//左キーのみを押したとき
	}
	else if (pInputGamepad->GetGameStickLXPress(0) > 0)
	{//右キーのみを押したとき
	}

	D3DXVECTOR3 mouseMove = pInputMouse->GetChangePos();
	m_rot.y += mouseMove.x * POSR_MOVE;
	m_rot.z += mouseMove.y * POSR_MOVE;

	if (m_rot.y < -D3DX_PI)
	{//上限下限
		m_rot.y = D3DX_PI;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}
	//真上下ギリギリで反転前にストップ
	if (m_rot.z < -CAMERA_LIMIT_Z)
	{
		m_rot.z = -CAMERA_LIMIT_Z;
	}
	else if (m_rot.z > CAMERA_LIMIT_Z)
	{
		m_rot.z = CAMERA_LIMIT_Z;
	}
}
