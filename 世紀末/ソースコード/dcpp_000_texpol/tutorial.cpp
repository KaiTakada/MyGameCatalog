//===============================================
//
// チュートリアル(tutorial.cpp)
// Author: ��田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "object2D.h"
#include "texture.h"

//=========================
// マクロ定義
//=========================

//=========================
// 静的メンバ変数
//=========================

//============================
// コンストラクタ
//============================
CTutorial::CTutorial()
{

}

//============================
// デストラクタ
//============================
CTutorial::~CTutorial()
{

}

//============================
// 初期化
//============================
HRESULT CTutorial::Init()
{
	CScene::Init();

	float fWidth = SCREEN_WIDTH;
	float fHeight = SCREEN_HEIGHT;

	CTexture *pTexture = CManager::GetTexture();

	CObject2D *pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\tutorial.jpg"));

	CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_TUTORIAL);

	return S_OK;
}

//============================
// 終了
//============================
void CTutorial::Uninit()
{
	CScene::Uninit();

	CManager::GetSound()->Stop();
}

//============================
// 更新
//============================
void CTutorial::Update()
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInputGamepad();
	CFade *pFade = CScene::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputKeyboard->GetTrigger(DIK_SPACE) || 
		pInputPad->GetPress(CInputGamepad::BUTTON_A, 0) == true)
	{//[ Enter ]キーでポーズ
		if (pFade->SetState(CScene::MODE_GAME))
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BUTTON);
		}
	}

	CScene::Update();
}

//============================
// 描画
//============================
void CTutorial::Draw()
{
	CScene::Draw();
}

//============================
// 生成
//============================
CTutorial * CTutorial::Create()
{
	CTutorial *pResult = nullptr;

	//オブジェクト生成&初期化
	pResult = new CTutorial;

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
