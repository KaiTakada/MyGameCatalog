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
#include "game.h"
#include "timer.h"

//=========================
// マクロ定義
//=========================

//=========================
// 静的メンバ変数
//=========================

//============================
// コンストラクタ
//============================
CResult::CResult()
{
	m_pTimer = nullptr;
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

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\BG\\bg_002.jpg"));

	pPolygon->SetTexCol(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.7f));
	//--------------------------------


	//スコアポリゴン------------------
	pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.2f, fHeight * 0.7f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(200.0f, 60.0f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\score.png"));

	if (m_pTimer != nullptr)
	{
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	m_pTimer = CTimer::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.7f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(40.0f, 60.0f, 0.0f));

	m_pTimer->SetValue(GetNowTime());

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//============================
// 終了
//============================
void CResult::Uninit()
{
	if (m_pTimer != nullptr)
	{
		m_pTimer->Uninit();
		m_pTimer = nullptr;
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

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputPad->GetPress(CInputGamepad::BUTTON_A, 0) == true)
	{//[ Enter ]キーでポーズ
		if (pFade->SetState(CScene::MODE_RANKING))
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BUTTON);
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
