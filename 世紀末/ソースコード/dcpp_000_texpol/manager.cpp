//===============================================
//
// マネージャ(manager.cpp)
// Author ��田 佳依
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
#include "spawn.h"
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
#include "ranking.h"

//#include "renderer.h"

//=========================
// マクロ定義
//=========================
#define DEBUG_SCENE CScene::MODE_TITLE//デバッグ時のスタートシーン

//=========================
// 前方宣言
//=========================

//=========================
// 静的メンバ変数
//=========================
//Manager
CRenderer *CManager::m_pRenderer = nullptr;					//レンダラー
CTexture *CManager::m_pTeture = nullptr;					//テクスチャ
CXModel *CManager::m_pXModel = nullptr;						//Xファイルモデル
CCamera *CManager::m_pCamera = nullptr;						//カメラ
CLight *CManager::m_pLight = nullptr;						//ライト
CSound *CManager::m_pSound = nullptr;						//サウンド
CSpawn *CManager::m_pSpawn = nullptr;						//敵スポーン

CInputKeyboard *CManager::m_pCInputKeyboard = nullptr;		//キーボード入力
CInputGamepad *CManager::m_pCInputGamepad = nullptr;		//入力(Pad[X])
CInputMouse *CManager::m_pCInputMouse = nullptr;			//入力(マウス)

CDebugProc *CManager::m_pDebugProc = nullptr;				//デバッグ
CScene *CManager::m_pScene = nullptr;						//シーン

bool CManager::m_bPause = false;		//ポーズフラグ
bool CManager::m_bGrow = false;			//進化シーンフラグ
CManager::RESULT_TYPE CManager::m_result = RT_NONE;			//勝敗内容

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
CBullet *CScene::m_pBullet = nullptr;						//弾
CExplosion *CScene::m_pExplosion = nullptr;				//爆発
CField *CScene::m_pField = nullptr;						//地面
CItem *CScene::m_pItem = nullptr;							//アイテム

CObjectX *CScene::m_pObjectX = nullptr;					//モデル
CEnemy *CScene::m_pEnemy = nullptr;						//敵
CBlock *CScene::m_pBlock = nullptr;						//ブロック

int CScene::m_nScore = 0;								//今回プレイ結果

//============================
// コンストラクタ
//============================
CManager::CManager()
{

}

//============================
// デストラクタ
//============================
CManager::~CManager()
{

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

	////============ テクスチャ ============
	////NULLチェック
	//if (m_pTeture != nullptr)
	//{
	//	m_pTeture = nullptr;
	//}

	////生成
	//if (m_pTeture == nullptr)
	//{
	//	m_pTeture = new CTexture;

	//	if (FAILED(m_pTeture->Load()))
	//	{//初期化処理が失敗した場合
	//		return -1;
	//	}
	//}
	//else
	//{
	//	return -1;
	//}

	////============ モデル ============
	////NULLチェック
	//if (m_pXModel != nullptr)
	//{
	//	m_pXModel = nullptr;
	//}

	////生成
	//if (m_pXModel == nullptr)
	//{
	//	m_pXModel = new CXModel;

	//	if (FAILED(m_pXModel->Load()))
	//	{//初期化処理が失敗した場合
	//		return -1;
	//	}
	//}
	//else
	//{
	//	return -1;
	//}

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
		m_pScene = CScene::Create(DEBUG_SCENE);
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
	////オブジェクト破棄
	//if (m_pField != nullptr)
	//{//地面破棄
	//	m_pField->Uninit();
	//	m_pField = nullptr;
	//}

	////オブジェクト破棄
	//if (m_pPlayer != nullptr)
	//{//プレイヤー破棄
	//	m_pPlayer->Uninit();
	//	m_pPlayer = nullptr;
	//}

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

	if (m_pSpawn != nullptr)
	{//スポーン破棄
		m_pSpawn->Uninit();
		delete m_pSpawn;
		m_pSpawn = nullptr;
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
	if (m_pRenderer != nullptr && m_bPause == false && m_bGrow == false)
	{
		m_pRenderer->Update();
	}
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}
}

//============================
// 描画処理
//============================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}

	m_pScene->Draw();
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
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (m_pFade != nullptr)
	{//フェード
		m_pFade->Update();
	}

	////無限フェード(メモリデバッグ用)
	//int nMode = GetMode() + 1;
	//nMode %= CScene::MODE_MAX;
	//m_pFade->SetState(CScene::MODE(nMode));

	if (pKeyboard->GetTrigger(DIK_ADD))
	{
		//新しいモードの生成
		int nMode = GetMode() + 1;
		nMode %= CScene::MODE_MAX;

		m_pFade->SetState(CScene::MODE(nMode));
	}
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

	case CScene::MODE_RANKING:
		pScene = new CRanking;
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
				m_pFade = CFade::Create(CScene::MODE_GAME);
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

	if (mode != CScene::MODE_RANKING)
	{
		m_result = RT_NONE;
	}

	//新しいモードの生成
	m_pScene = CScene::Create(mode);
}
