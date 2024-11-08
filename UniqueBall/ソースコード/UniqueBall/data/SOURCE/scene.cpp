//===============================================
//
// シーン(scene.cpp)
// Author 髙田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
//#include "manager.h"
#include "scene.h"
#include "debugproc.h"
#include "input.h"
#include "fade.h"

#include "object.h"
#include "texture.h"
#include "Xmodel.h"
#include "obj_polygon2D.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "player.h"
#include "enemy.h"
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
#include "obj_polygon3D.h"
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
	//CScene::MODE MODE_DEBUG = CScene::MODE_GAME;//デバッグ時のスタートシーン
	CScene::MODE MODE_DEBUG = CScene::MODE_TITLE;//デバッグ時のスタートシーン
}

//=========================
// 前方宣言
//=========================

//=========================
// 静的メンバ変数
//=========================
//Scene
CNumber *CScene::m_pNumber = nullptr;						//数字
CScore *CScene::m_pScore = nullptr;							//スコア
CFade *CScene::m_pFade = nullptr;							//フェード

CObject3D *CScene::m_pObject3D = nullptr;					//3Dポリゴン

CObjectBillboard *CScene::m_pBillBoard = nullptr;			//ビルボード
CField *CScene::m_pField = nullptr;						//地面
CItem *CScene::m_pItem = nullptr;						//アイテム
CPlayer *CScene::m_pPlayer[mylib_const::MAX_PLAYER] = {nullptr};					//プレイヤー

CBlock *CScene::m_pBlock = nullptr;						//ブロック

int CScene::m_nTime = 0;								//今回プレイ結果
int CScene::m_nNumPlayer = 0;						//プレイヤー人数

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
