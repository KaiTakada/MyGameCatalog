//===============================================
//
// タイトル(title.cpp)
// Author: 髙田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "field.h"
#include "object2D.h"
#include "press_enter.h"
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
CTitle::CTitle()
{
	m_pField = nullptr;
	m_pEnter = nullptr;

	m_nCntRanking = 0;
	m_nAlphaCnter = 0;
	m_nTimeInterval = 0;
	m_fAlphaColor = 0.0f;
}

//============================
// デストラクタ
//============================
CTitle::~CTitle()
{

}

//============================
// 初期化
//============================
HRESULT CTitle::Init()
{
	m_nTimeInterval = 25;						//点滅間隔を小さくする
	m_fAlphaColor = 1.0f;
	m_nAlphaCnter = 0;

	CScene::Init();

	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	float fWidth = SCREEN_WIDTH;
	float fHeight = SCREEN_HEIGHT;

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	CObject2D *pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.3f, fHeight * 0.3f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.3f, fHeight * 0.3f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\title.png"));

	m_pEnter = CPress_Enter::Create(D3DXVECTOR3(fWidth * 0.75f, fHeight * 0.8f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.15f, fHeight * 0.08f, 0.0f));

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);

	return S_OK;
}

//============================
// 終了
//============================
void CTitle::Uninit()
{
	if (m_pField != nullptr)
	{
		m_pField->Uninit();
		m_pField = nullptr;
	}

	if (m_pEnter != nullptr)
	{
		m_pEnter->Uninit();
		m_pEnter = nullptr;
	}

	CScene::Uninit();

	CManager::GetInstance()->GetSound()->Stop();
}

//============================
// 更新
//============================
void CTitle::Update()
{
	//m_pField->Update();

	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInstance()->GetInputGamepad();
	CFade *pFade = CScene::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputKeyboard->GetTrigger(DIK_SPACE) || 
		pInputPad->GetPress(CInputGamepad::BUTTON_A, 0) == true)
	{//[ Enter ]キーでポーズ
		if (pFade->SetState(CScene::MODE_TUTORIAL))
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BUTTON);
		}
		m_nTimeInterval = 4;							//点滅間隔を小さくする
	}
	else if (m_nCntRanking >= 600)
	{
		pFade->SetState(CScene::MODE_RANKING);
	}

	if (m_pField != nullptr)
	{
		m_pField->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pEnter != nullptr)
	{
		m_pEnter->Update();
	}

	CScene::Update();
}

//============================
// 描画
//============================
void CTitle::Draw()
{
	//if (m_pField != nullptr)
	//{
	//	m_pField->Draw();
	//}

	CScene::Draw();
}

//============================
// 生成
//============================
CTitle * CTitle::Create()
{
	CTitle *pTitle = nullptr;

	//オブジェクト生成&初期化
	pTitle = new CTitle;

	if (pTitle != nullptr)
	{
		if (FAILED(pTitle->Init()))
		{
			return nullptr;
		}
		else
		{
			//pTitle->m_mode = mode;
		}
	}
	else
	{
		return nullptr;
	}


	return pTitle;
}
