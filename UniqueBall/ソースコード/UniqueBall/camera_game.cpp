//============================
//
//カメラの処理(camera.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "camera_game.h"
#include "player.h"
#include "input.h"

//============================
// 定数定義
//============================
namespace
{
	const DWORD HEIGHT = static_cast<DWORD>(SCREEN_HEIGHT * 0.5f);
	const DWORD WIDTH = static_cast<DWORD>(SCREEN_WIDTH * 0.5f);
	const float CAMERA_LIMIT_Z(D3DX_PI * 0.48f);	//z軸回転の限界値
}

//============================
// コンストラクタ
//============================
CCameraGame::CCameraGame()
{
	m_nIdx = -1;

}

//============================
// デストラクタ
//============================
CCameraGame::~CCameraGame()
{

}

//============================
//カメラの初期化処理
//============================
HRESULT CCameraGame::Init()
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
//カメラの終了処理
//============================
void CCameraGame::Uninit()
{
	CCamera::Uninit();
}

//============================
//カメラの更新処理
//============================
void CCameraGame::Update()
{
	PlayerStalk();
}

//============================
//カメラの設定処理
//============================
void CCameraGame::SetCamera()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxProjection = GetMtxProj(); //プロジェクションマトリックス
	D3DXMATRIX mtxView = GetMtxView();	//ビューマトリックス
	D3DXVECTOR3 posV = GetPosV();	//視点
	D3DXVECTOR3 posR = GetPosR(); //注視点
	D3DXVECTOR3 vecU = GetVecU(); //上方向ベクトル

	//ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	//バッファをクリア
	pDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	//プロジェクションマトリックスの作成[透視投影]
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
								D3DXToRadian(45.0f),
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,
								40000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION,&mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView,
						&posV,			//視点
						&posR,			//注視点
						&vecU);		//上方向ベクトル

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	////バッファをクリア
	//pDevice->Clear(0, NULL,
	//	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
	//	D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
}

//============================
// 描画画面位置設定
//============================
void CCameraGame::SetViewPos()
{
	if (m_nIdx == 0)
	{
		m_viewport.X = 0;
		m_viewport.Y = 0;
	}
	else
	{
		m_viewport.X = static_cast<DWORD>(SCREEN_WIDTH / 2);
		m_viewport.Y = static_cast<DWORD>(SCREEN_HEIGHT / 2);
	}
}

//============================
// プレイヤー追従処理(向きも)
//============================
void CCameraGame::PlayerStalk(void)
{
	CPlayer *pPlayer = CScene::GetPlayer(m_nIdx);

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
	float fAngle = GetAngle();

	//プレイヤーの向きを追従する
	rotDest.y = pPlayer->GetRot().y;

	//注視点
	posR.x = pos.x;
	posR.z = pos.z;
	posR.y = pos.y;
	//m_posR.x = pos.x + (sinf(m_rot.y) * m_fDis);
	//m_posR.z = pos.z + (cosf(m_rot.y) * m_fDis);
	//m_posR.y = pos.y;

	//視点
	posV.x = posR.x + (sinf(rot.y) * fDis);
	posV.y = posR.y + (sinf(fAngle) * fDis);
	posV.z = posR.z + (cosf(rot.y) * fDis);

	RotControll();

	////注視点水平旋回
	//RotHorPosR();

	////注視点垂直旋回
	//RotVerPosR();

	////補正
	//ReviseRot();

	SetPosR(posR);
	SetPosV(posV);
	SetRotDest(rotDest);

	//プレイヤーに向きを反映
	pPlayer->SetRot(rotDest);
}

//===========================
//マウスで向きを調整
//===========================
void CCameraGame::RotControll()
{
	//右スティック
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	bool bInput = false;

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = mylib_const::DEFVEC3;

	rot.y += float(float(pInputGamepad->GetGameStickRXPress(m_nIdx)) / float(SHRT_MAX)) * 0.05f;
	rot.z += float(float(pInputGamepad->GetGameStickRYPress(m_nIdx)) / float(SHRT_MAX)) * 0.05f;
	
	if (rot.y < -D3DX_PI)
	{//上限下限
		rot.y = D3DX_PI;
	}
	else if (rot.y > D3DX_PI)
	{
		rot.y = -D3DX_PI;
	}
	//真上下ギリギリで反転前にストップ
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
