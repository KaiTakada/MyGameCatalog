//===============================================
//
// マネージャ(manager.cpp)
// Author 髙田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "fade.h"

#include "object.h"
#include "texture.h"
#include "Xmodel.h"
#include "object2D.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "bg.h"
#include "multibg.h"
#include "effect.h"
#include "particle.h"
#include "number.h"
#include "score.h"
#include "block.h"
#include "item.h"
#include "Field.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"

//#include "renderer.h"

//=========================
// 定数定義
//=========================
namespace
{
	CScene::MODE MODE_DEBUG = CScene::MODE_GAME;//デバッグ時のスタートシーン
}

//=========================
// 前方宣言
//=========================

//=========================
// 静的メンバ変数
//=========================
//Manager
CManager *CManager::m_pManager = nullptr;								//背景

//Scene
CBg *CScene::m_pBg = nullptr;								//背景
CMultiBg *CScene::m_pMultiBg = nullptr;						//多重背景
CNumber *CScene::m_pNumber = nullptr;						//数字
CScore *CScene::m_pScore = nullptr;							//スコア
CFade *CScene::m_pFade = nullptr;							//フェード

CObject3D *CScene::m_pObject3D = nullptr;					//3Dポリゴン

CObjectBillboard *CScene::m_pBillBoard = nullptr;			//ビルボード
CEffect *CScene::m_pEffect = nullptr;						//エフェクト
CParticle *CScene::m_pParticle = nullptr;					//パーティクル
CField *CScene::m_pField = nullptr;						//地面
CItem *CScene::m_pItem = nullptr;						//アイテム
CPlayer *CScene::m_pPlayer[mylib_const::MAX_PLAYER] = {nullptr};					//プレイヤー

CObjectX *CScene::m_pObjectX = nullptr;					//モデル
CEnemy *CScene::m_pEnemy = nullptr;						//敵
CBlock *CScene::m_pBlock = nullptr;						//ブロック

int CScene::m_nTime = 0;								//今回プレイ結果
int CScene::m_nNumPlayer = 0;						//プレイヤー人数

//============================
// コンストラクタ
//============================
CManager::CManager()
{
	m_pRenderer = nullptr;					//レンダラー
	m_pTeture = nullptr;					//テクスチャ
	m_pXModel = nullptr;					//Xファイルモデル
	m_pCamera = nullptr;					//カメラ
	m_pLight = nullptr;						//ライト
	m_pSound = nullptr;						//サウンド

	m_pCInputKeyboard = nullptr;		//入力(キーボード)
	m_pCInputGamepad = nullptr;			//入力(Pad[X])
	m_pCInputMouse = nullptr;			//入力(マウス)

	m_pDebugProc = nullptr;				//デバッグ
	m_pScene = nullptr;					//シーン

	m_bPause = false;			//ポーズフラグ
	m_bGrow = false;			//進化シーンフラグ
	m_bEdit = false;			//エディットシーンフラグ
	m_result = RT_NONE;			//勝敗内容

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		m_anRank[i] = -1;
	}

	m_pManager = this;
}

//============================
// デストラクタ
//============================
CManager::~CManager()
{
	m_pManager = nullptr;
}

//============================
// 初期化処理
//============================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//============ レンダラー ============
	//NULLチェック
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		m_pRenderer = nullptr;
	}

	//レンダラーの生成
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;
	}
	else
	{
		return -1;
	}

	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//============ キーボード ============
	//NULLチェック
	if (m_pCInputKeyboard != nullptr)
	{
		m_pCInputKeyboard->Uninit();
		m_pCInputKeyboard = nullptr;
	}

	//キーボードの生成
	if (m_pCInputKeyboard == nullptr)
	{
		m_pCInputKeyboard = new CInputKeyboard;

		if (FAILED(m_pCInputKeyboard->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			return -1;
		}
	}
	else
	{
		return -1;
	}
	
	//============ Pad[X] ============
	//NULLチェック
	if (m_pCInputGamepad != nullptr)
	{
		m_pCInputGamepad->Uninit();
		m_pCInputGamepad = nullptr;
	}

	//Pad[X]の生成
	if (m_pCInputGamepad == nullptr)
	{
		m_pCInputGamepad = new CInputGamepad;

		if (FAILED(m_pCInputGamepad->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ マウス ============
	//NULLチェック
	if (m_pCInputMouse != nullptr)
	{
		m_pCInputMouse->Uninit();
		m_pCInputMouse = nullptr;
	}

	//マウスの生成
	if (m_pCInputMouse == nullptr)
	{
		m_pCInputMouse = new CInputMouse;

		if (FAILED(m_pCInputMouse->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ デバッグ ============
	//NULLチェック
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		m_pDebugProc = nullptr;
	}

	if (m_pDebugProc == nullptr)
	{
		m_pDebugProc = new CDebugProc;

		if (m_pDebugProc != nullptr)
		{
			m_pDebugProc->Init();
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ カメラ ============
	//NULLチェック
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		m_pCamera = nullptr;
	}

	if (m_pCamera == nullptr)
	{
		m_pCamera = new CCamera;
	}
	else
	{
		return -1;
	}

	if (FAILED(m_pCamera->Init()))
	{//初期化処理が失敗した場合
		return -1;
	}

	//============ ライト ============
	//NULLチェック
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	if (m_pLight == nullptr)
	{
		m_pLight = new CLight;
	}
	else
	{
		return -1;
	}

	if (FAILED(m_pLight->Init()))
	{//初期化処理が失敗した場合
		return -1;
	}

	//============ サウンド ============
	//NULLチェック
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		m_pSound = nullptr;
	}

	if (m_pSound == nullptr)
	{
		m_pSound = new CSound;
	}
	else
	{
		return -1;
	}

	if (FAILED(m_pSound->Init(hWnd)))
	{//初期化処理が失敗した場合
		return -1;
	}
	else
	{
#ifdef _RELEASE
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
#else
#endif
	}

	//============ テクスチャ ============
	//NULLチェック
	if (m_pTeture != nullptr)
	{
		m_pTeture = nullptr;
	}

	//生成
	if (m_pTeture == nullptr)
	{
		m_pTeture = new CTexture;

		if (FAILED(m_pTeture->Load()))
		{//初期化処理が失敗した場合
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ モデル ============
	//NULLチェック
	if (m_pXModel != nullptr)
	{
		m_pXModel = nullptr;
	}

	//生成
	if (m_pXModel == nullptr)
	{
		m_pXModel = new CXModel;

		if (FAILED(m_pXModel->Load()))
		{//初期化処理が失敗した場合
			return -1;
		}
	}
	else
	{
		return -1;
	}

	//============ オブジェクト ============
	//オブジェクトの初期化処理
	CObject::ReleaseAll();

	//============ シーン ============
	//NULLチェック
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	if (m_pScene == nullptr)
	{

#if _DEBUG
		m_pScene = CScene::Create(MODE_DEBUG);
#else
		m_pScene = CScene::Create(CScene::MODE_TITLE);
#endif

	}
	else
	{
		return -1;
	}

	return S_OK;
}

//============================
// 終了処理
//============================
void CManager::Uninit(void)
{
	if (m_pScene != nullptr)
	{//シーン破棄
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//全てのオブジェクト破棄
	CObject::ReleaseAll();

	if (m_pTeture != nullptr)
	{//テクスチャ破棄
		m_pTeture->Unload();
		delete m_pTeture;
		m_pTeture = nullptr;
	}

	if (m_pXModel != nullptr)
	{//Xモデル破棄
		m_pXModel->Unload();
		delete m_pXModel;
		m_pXModel = nullptr;
	}

	if (m_pCamera != nullptr)
	{//カメラ破棄
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pLight != nullptr)
	{//ライト破棄
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pSound != nullptr)
	{//サウンド破棄
		m_pSound->Stop();
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
	
	if (m_pDebugProc != nullptr)
	{//デバッグ破棄
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	if (m_pTeture != nullptr)
	{//テクスチャ破棄
		m_pTeture->Unload();
		delete m_pTeture;
		m_pTeture = nullptr;
	}
	
	if (m_pXModel != nullptr)
	{//Xモデル破棄
		m_pXModel->Unload();
		delete m_pXModel;
		m_pXModel = nullptr;
	}

	if (m_pCInputMouse != nullptr)
	{//マウス破棄
		m_pCInputMouse->Uninit();
		delete m_pCInputMouse;
		m_pCInputMouse = nullptr;
	}

	if (m_pCInputGamepad != nullptr)
	{//Pad[X]破棄
		m_pCInputGamepad->Uninit();
		delete m_pCInputGamepad;
		m_pCInputGamepad = nullptr;
	}

	if (m_pCInputKeyboard != nullptr)
	{//キーボード破棄
		m_pCInputKeyboard->Uninit();
		delete m_pCInputKeyboard;
		m_pCInputKeyboard = nullptr;
	}

	if (m_pRenderer != nullptr)
	{//レンダラー破棄
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

//============================
// 更新処理
//============================
void CManager::Update(void)
{
	//更新処理
	if (m_pCInputGamepad != nullptr)
	{
		m_pCInputGamepad->Update();
	}
	if (m_pCInputKeyboard != nullptr)
	{
		m_pCInputKeyboard->Update();
	}
	if (m_pCInputMouse != nullptr)
	{
		m_pCInputMouse->Update();
	}
	if (m_pRenderer != nullptr && m_bPause == false && m_bGrow == false && m_bEdit == false)
	{//全オブジェクト
		m_pRenderer->Update();
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}
	if (m_pLight != nullptr)
	{
		m_pLight->Update();
	}

	if (m_pDebugProc != nullptr)
	{//デバッグ破棄
		m_pDebugProc->Update();
	}
}

//============================
// 描画処理
//============================
void CManager::Draw(void)
{
	m_pScene->Draw();
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
}

//======================================================
//============================
// コンストラクタ
//============================
CScene::CScene()
{
	m_mode = MODE_TITLE;
}

//============================
// デストラクタ
//============================
CScene::~CScene()
{

}

//============================
// 初期化
//============================
HRESULT CScene::Init()
{
	//全てのオブジェクト破棄
	CObject::ReleaseAll();

	m_mode = MODE_TITLE;

	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	//カメラの初期化
	pCamera->Init();

	return S_OK;
}

//============================
// 終了
//============================
void CScene::Uninit()
{
	if (m_pFade != nullptr)
	{//フェード破棄
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	//全てのオブジェクト破棄
	CObject::ReleaseAll();
}

//============================
// 更新
//============================
void CScene::Update()
{
	CInputKeyboard *pKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (m_pFade != nullptr)
	{//フェード
		m_pFade->Update();
	}

	////無限フェード(メモリデバッグ用)
	//int nMode = static_cast<int>(GetMode());
	//nMode += 1;
	//nMode %= static_cast<int>(CScene::MODE_MAX);
	//
	//if (nMode >= static_cast<int>(CScene::MODE_TITLE) && nMode < static_cast<int>(CScene::MODE_MAX))
	//{
	//	m_pFade->SetState(CScene::MODE(nMode));
	//}
	//else
	//{
	//	static_assert(true, "モード壊れ");
	//}

#if _DEBUG
	if (pKeyboard->GetTrigger(DIK_ADD))
	{
		//新しいモードの生成
		int nMode = GetMode() + 1;
		nMode %= CScene::MODE_MAX;

		m_pFade->SetState(CScene::MODE(nMode));
	}

	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("#------------------------\n");
	pDebug->Print("#モード\n");
	pDebug->Print("#現在のモード:%d\n", static_cast<int>(m_mode));
	pDebug->Print("#------------------------\n");
#endif
}

//============================
// 描画
//============================
void CScene::Draw()
{
	m_pFade->Draw();
}

//============================
// 生成
//============================
CScene * CScene::Create(MODE mode)
{
	CScene *pScene = nullptr;

	//オブジェクト生成&初期化
	//pScene = new CScene;

	switch (mode)
	{
	case CScene::MODE_TITLE:
		pScene = new CTitle;
		break;

	case CScene::MODE_TUTORIAL:
		pScene = new CTutorial;
		break;

	case CScene::MODE_GAME:
		pScene = new CGame;
		break;

	case CScene::MODE_RESULT:
		pScene = new CResult;
		break;

	default:
		pScene = new CScene;
		break;
	}

	if (pScene != nullptr)
	{
		if (FAILED(pScene->Init()))
		{
			return nullptr;
		}
		else
		{
			pScene->m_mode = mode;
		
			//============ フェード ============
			if (m_pFade == nullptr)
			{
				m_pFade = CFade::Create(mode);
			}
		}
	}
	else
	{
		return nullptr;
	}

	return pScene;
}

//============================
// シーン設定
//============================
void CManager::SetMode(const CScene::MODE mode)
{
	if (m_pSound != nullptr)
	{
		m_pSound->Stop();		//サウンドの停止
	}

	//現在のモードの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//ポーズ状態をOFFに
	SetPause(false);

	m_bPause = false;		//ポーズフラグ
	m_bGrow = false;		//進化シーンフラグ

	//新しいモードの生成
	m_pScene = CScene::Create(mode);
}

//======================================
// インスタンス取得
//======================================
CManager * CManager::GetInstance()
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManager;
	}

	return m_pManager;
}

//======================================
// リリース
//======================================
void CManager::Release()
{
	if (m_pManager != nullptr)
	{
		m_pManager->Uninit();
		delete m_pManager;
		m_pManager = nullptr;
	}
}

void CManager::ZeroRank()
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		m_anRank[i] = -1;
	}
}
