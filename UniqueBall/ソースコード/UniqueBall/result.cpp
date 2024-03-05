//===============================================
//
// リザルト(result.cpp)
// Author: 髙田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "object2D.h"
#include "texture.h"
#include "timer.h"
#include "ui_result.h"

//=========================
// マクロ定義
//=========================
namespace
{
	const char* RANK_PASS = "data\\TEXTURE\\SCENE\\ranking.png";
}

//=========================
// 静的メンバ変数
//=========================

//============================
// コンストラクタ
//============================
CResult::CResult()
{
	m_pResultUI = nullptr;		//プレイヤ―のフレーム
}

//============================
// デストラクタ
//============================
CResult::~CResult()
{

}

//============================
// 初期化
//============================
HRESULT CResult::Init()
{
	float fWidth = SCREEN_WIDTH;
	float fHeight = SCREEN_HEIGHT;

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	CScene::Init();

	//背景----------------------------
	CObject2D *pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\BG\\field.jpg"));

	pPolygon->SetTexCol(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.7f));
	//--------------------------------

	//プレイヤ― ---------------------
	//--------------------------------

	//フレーム------------------------
	m_pResultUI = CUI_Result::Create();
	//--------------------------------
	
	//順位 ---------------------
	int nNumPlayer = CManager::GetInstance()->GetScene()->GetNumPlayer();
	for (int i = 0; i < nNumPlayer; i++)
	{
		if (m_pResultUI != nullptr)
		{
			int nPlayer = CManager::GetInstance()->GetRank(i);	//この順位(i)のプレイヤー番号
			if (nPlayer == -1)
			{
				continue;
			}

			CObject2D *p2D = m_pResultUI->GetMark(nPlayer);
			float fPos = (nNumPlayer - 1) * 0.2f - i * 0.2f;
			D3DXVECTOR3 pos = D3DXVECTOR3(p2D->GetPos().x - 180.0f, p2D->GetPos().y - 200.0f, 0.0f);

			if (p2D != nullptr)
			{
				CObject2D *pRank = CObject2D::Create(pos, mylib_const::DEFVEC3, D3DXVECTOR3(50.0f, 50.0f, 50.0f), CObject2D::UPDATE_TYPE_NONE);
				pRank->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\ranking.png"));
				pRank->SetTexPos(D3DXVECTOR2(fPos + 0.2f, 1.0f), D3DXVECTOR2(fPos, 0.0f));
			}
		}
	}
	//--------------------------------

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//============================
// 終了
//============================
void CResult::Uninit()
{
	CManager::GetInstance()->SetResult(CManager::RT_NONE);

	if (m_pResultUI != nullptr)
	{
		m_pResultUI->SetDeath(true);
		m_pResultUI = nullptr;
	}

	CScene::Uninit();
}

//============================
// 更新
//============================
void CResult::Update()
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInstance()->GetInputGamepad();
	CFade *pFade = CScene::GetFade();

	if (m_pResultUI->GetTrans())
	{//[ Enter ]キーでポーズ
		if (pFade->SetState(CScene::MODE_TUTORIAL))
		{
			//CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BUTTON);
			CManager::GetInstance()->ZeroRank();	//順位消去
		}
	}

	CScene::Update();
}

//============================
// 描画
//============================
void CResult::Draw()
{
	CScene::Draw();
}

//============================
// 生成
//============================
CResult * CResult::Create()
{
	CResult *pResult = nullptr;

	//オブジェクト生成&初期化
	pResult = new CResult;

	if (pResult != nullptr)
	{
		if (FAILED(pResult->Init()))
		{
			return nullptr;
		}
		else
		{
			//pResult->m_mode = mode;
		}
	}
	else
	{
		return nullptr;
	}


	return pResult;
}
