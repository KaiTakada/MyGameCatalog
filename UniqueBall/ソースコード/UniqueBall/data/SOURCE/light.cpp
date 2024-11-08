//============================
//
//ライト(light.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "light.h"
#include "debugproc.h"
#include "input.h"

//============================
// コンストラクタ
//============================
CLight::CLight()
{
	m_nDebugNum = 0;
}

//============================
// デストラクタ
//============================
CLight::~CLight()
{

}

//============================
//ライトの初期化処理
//============================
HRESULT CLight::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir;		//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{//ライトの拡散校と方向を設定
		case 0:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(-1.87, -1.57, 2.65);
			break;

		case 1:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(-1.87, -1.57, -2.65);
			break;

		case 2:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(3.03f, -1.57f, -2.57f);
			break;

		default:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(0.0f, -0.0f, 0.0f);
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトルを正規化する(1.0fにする)
		m_light[nCntLight].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//============================
//ライトの終了処理
//============================
void CLight::Uninit(void)
{

}

//============================
//ライトの更新処理
//============================
void CLight::Update(void)
{
	//デバッグ
#if _DEBUG
	D3DXVECTOR3 pos = m_light[m_nDebugNum].Direction;
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();

	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_RALT))
	{
		m_nDebugNum++;
		m_nDebugNum %= MAX_LIGHT;
	}
	if (pInputKeyboard->GetPress(DIK_UP))
	{
		pos.z += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		pos.z -= 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		pos.x += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		pos.x -= 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_RSHIFT))
	{
		pos.y += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_RCONTROL))
	{
		pos.y -= 0.01f;
	}

	pos.x = mylib_useful::NormalizeRot(pos.x);
	pos.y = mylib_useful::NormalizeRot(pos.y);
	pos.z = mylib_useful::NormalizeRot(pos.z);

	m_light[m_nDebugNum].Direction = pos;

	//ライトを設定する
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->SetLight(m_nDebugNum, &m_light[m_nDebugNum]);

	//ライトを有効にする
	pDevice->LightEnable(m_nDebugNum, TRUE);

#endif
}
