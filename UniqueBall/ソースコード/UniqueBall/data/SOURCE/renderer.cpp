//===============================================
//
// レンダラー(renderer.cpp)
// Author ��田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "game.h"
#include "camera_player.h"

//=========================
// マクロ定義
//=========================

//============================
// コンストラクタ
//============================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_nCamCtr = -1;
}

//============================
// デストラクタ
//============================
CRenderer::~CRenderer()
{

}

//============================
// 初期化処理
//============================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;											//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;									//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										//ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

	//Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//ランダムの種生成
	srand((unsigned int)time(NULL));

	return S_OK;
}

//============================
// 終了処理
//============================
void CRenderer::Uninit(void)
{
	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//============================
// 更新処理
//============================
void CRenderer::Update(void)
{
	//ポリゴンの更新処理
	CObject::UpdateAll();
}

//============================
// 描画処理
//============================
void CRenderer::Draw(void)
{
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	
	//元ビューポート保管
	D3DVIEWPORT9 viewport;

	//画面クリア
	m_pD3DDevice->Clear(0,NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0,0,0,0),1.0f,0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		m_pD3DDevice->GetViewport(&viewport);

		CScene *pScene = CManager::GetInstance()->GetScene();
		if (pScene->GetMode() == CScene::MODE_GAME)
		{//ゲームなら画面分割する
			CGame *pGame = static_cast<CGame*>(pScene);
			CCameraPlayer **ppCamera = pGame->GetCamera();

			for (int i = 0; i < (mygame::NUMPLAYER); i++)
			{
				if (ppCamera[i] == nullptr)
				{
					continue;
				}

				m_nCamCtr = i;
				ppCamera[i]->SetCamera();
				CObject::DrawAll();
				pDebug->Draw();
			}
		}
		else
		{
			CCamera *pCamera = CManager::GetInstance()->GetCamera();
			if (pCamera != nullptr)
			{
				pCamera->SetCamera();
			}

			//各種オブジェクトの描画処理
			CObject::DrawAll();
			pDebug->Draw();
		}


		m_pD3DDevice->SetViewport(&viewport);

		//描画終了
		m_pD3DDevice->EndScene();
		m_nCamCtr = -1;
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL,NULL);
}

//============================
// デバイスの取得
//============================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}
