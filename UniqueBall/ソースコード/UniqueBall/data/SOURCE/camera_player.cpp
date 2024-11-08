//============================
//
//カメラの処理(camera.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "camera_player.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"

//============================
// 定数定義
//============================
namespace
{
	const DWORD HEIGHT = static_cast<DWORD>(SCREEN_HEIGHT * 0.5f);
	const DWORD WIDTH = static_cast<DWORD>(SCREEN_WIDTH * 0.5f);
	//const float CAMERA_LIMIT_Z(D3DX_PI * 0.48f);	//z軸回転の限界値
	const float CAMERA_LIMIT_Z(D3DX_PI * 0.25f);	//z軸回転の限界値

	const float KEYMOUSE_INPUT_VALUE(0.01f);	//キーマウのインプット値
}

//============================
// コンストラクタ
//============================
CCameraPlayer::CCameraPlayer()
{
	m_nIdx = -1;
	m_pTarget = nullptr;
}

//============================
// デストラクタ
//============================
CCameraPlayer::~CCameraPlayer()
{

}

//============================
//カメラの初期化処理
//============================
HRESULT CCameraPlayer::Init()
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
void CCameraPlayer::Uninit()
{
	CCamera::Uninit();
}

//============================
//カメラの更新処理
//============================
void CCameraPlayer::Update()
{
	if (m_pTarget != nullptr)
	{
		if (m_pTarget->GetDeath())
		{//削除前に解除
			m_pTarget = nullptr;
		}
	}

	//どっちのモード？
	if (m_pTarget != nullptr)
	{
		FocusOnTarget();
	}
	else
	{
		PlayerStalk();
	}

	Debug();
}

//============================
//カメラの設定処理
//============================
void CCameraPlayer::SetCamera()
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
								D3DXToRadian(45.0f),	//視野
								(float)m_viewport.Width / (float)m_viewport.Height,	//縦横比
								10.0f,	//最小距離
								40000.0f);	//最大距離

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
void CCameraPlayer::SetViewPos()
{
	m_viewport.Height = static_cast<DWORD>(SCREEN_HEIGHT / 1.0f);
	m_viewport.Width = static_cast<DWORD>(SCREEN_WIDTH / 2);

	if (m_nIdx == 0)
	{
		m_viewport.X = 0;
		m_viewport.Y = 0;
	}
	else
	{
		m_viewport.X = static_cast<DWORD>(SCREEN_WIDTH / 2);
		m_viewport.Y = 0;
		//m_viewport.Y = static_cast<DWORD>(SCREEN_HEIGHT / 2);
	}
}

//============================
// プレイヤー追従処理(向きも)
//============================
void CCameraPlayer::PlayerStalk(void)
{
	CPlayer *pPlayer = CScene::GetElem(m_nIdx);

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

	//プレイヤーの向きを追従する
	rotDest.y = pPlayer->GetRot().y;

	//注視点
	posR.x = pos.x;
	posR.z = pos.z;
	posR.y = pos.y;

	//視点
	posV.x = posR.x + sinf(rot.y + D3DX_PI) * cosf(rot.z) * -fDis;
	posV.y = posR.y + sinf(rot.z) * -fDis;
	posV.z = posR.z + cosf(rot.y + D3DX_PI) * cosf(rot.z) * -fDis;

	//カメラ
	RotControll();

	SetPosR(posR);
	SetPosV(posV);
	pPlayer->SetRot(D3DXVECTOR3(rot.x, rot.y,0.0f));
}

//===========================
// 的を注視点に操作する
//===========================
void CCameraPlayer::FocusOnTarget()
{
	if (m_pTarget == nullptr)
	{
		return;
	}

	//自身
	CPlayer* pPlayer = CScene::GetElem(m_nIdx);

	if (pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posP = pPlayer->GetPosCol();
	D3DXVECTOR3 pos = m_pTarget->GetPos();
	posP.y += pPlayer->GetHeartPos();
	pos.y += m_pTarget->GetHeartPos();
	D3DXVECTOR3 rotP = pPlayer->GetRot();

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

	//視点
	posV.x = posP.x + sinf(rotP.y + D3DX_PI) * cosf(rotP.z) * -fDis;
	posV.y = posP.y + sinf(rotP.z) * -fDis;
	posV.z = posP.z + cosf(rotP.y + D3DX_PI) * cosf(rotP.z) * -fDis;

	SetPosR(posR);
	SetPosV(posV);
	SetRotDest(rotDest);
	pPlayer->SetRot(D3DXVECTOR3(rot.x, rot.y, 0.0f));
}

//===========================
//PADで向きを調整
//===========================
void CCameraPlayer::RotControll()
{
	//右スティック
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	bool bInput = false;

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = mylib_const::DEFVEC3;

	if (CPlayer::GetControlPlayer() == m_nIdx)
	{
		D3DXVECTOR3 MousePos = pInputMouse->GetChangePos();
		rot.y += MousePos.x * 0.01f;
		rot.z += MousePos.y * -0.01f;

		if (pInputKeyboard->GetPress(DIK_UP))		rot.z += KEYMOUSE_INPUT_VALUE;
		if (pInputKeyboard->GetPress(DIK_DOWN))		rot.z -= KEYMOUSE_INPUT_VALUE;
		if (pInputKeyboard->GetPress(DIK_RIGHT))	rot.y += KEYMOUSE_INPUT_VALUE;
		if (pInputKeyboard->GetPress(DIK_LEFT))		rot.y -= KEYMOUSE_INPUT_VALUE;
	}

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

//===========================
// デバッグ
//===========================
void CCameraPlayer::Debug()
{
	//デバッグ
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- カメラ情報 ---\n");
	pDebug->Print("現在の方向:%f %f %f\n", GetRot().x, GetRot().y, GetRot().z);
	pDebug->Print("ターゲット:%d\n", (m_pTarget != nullptr));
}
