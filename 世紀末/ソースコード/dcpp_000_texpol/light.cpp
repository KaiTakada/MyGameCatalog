//============================
//
//ライト(light.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "light.h"

//============================
// コンストラクタ
//============================
CLight::CLight()
{

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
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
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;

		case 1:
			m_light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			break;

		case 2:
			m_light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			break;

		default:
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
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

}
