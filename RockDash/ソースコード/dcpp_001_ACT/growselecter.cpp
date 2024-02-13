//===============================================
//
// ポーズ画面の処理(pause.cpp)
// Author:Kai Takada
//
//===============================================

//============================
// インクルードファイル
//============================
#include "manager.h"	//大元
#include "texture.h"	//描画テクスチャ
#include "object2D.h"
#include "item2D.h"
#include "game.h"
#include "growselecter.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "player.h"

//=======================
//マクロ定義
//=======================
#define MAX_PAUSE (MAX_ITEM)			//ポーズの選択肢数
#define EVO_MID (200)						//ポーズの選択肢どうしの間（横）
#define EVO_SIZE D3DXVECTOR3(250.0f,30.0f,0.0f)			//タイトル名のサイズ
#define EVO_SIZE_BG D3DXVECTOR3(350.0f,400.0f,0.0f)			//タイトル名のサイズ
#define EVO_BG_FILE "data\\TEXTURE\\ITEM\\bg.png"		//Bgファイルパス

//=======================
// 静的メンバ変数
//=======================

//============================
// コンストラクタ
//============================
CGrowSelecter::CGrowSelecter(int nPriority) : CObject(nPriority)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		m_pItem[nCntItem] = nullptr;		//選択肢(continue)
	}

	m_pBg = nullptr;			//背景()
	m_nPauseSelect = 0;			//現在の選択肢
	m_bEnter = false;
}

//============================
// デストラクタ
//============================
CGrowSelecter::~CGrowSelecter()
{

}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CGrowSelecter::Init()
{
	float fMid = -EVO_MID * (MAX_ITEM / 2);

	m_pBg = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXVECTOR3(EVO_SIZE_BG.x, EVO_SIZE_BG.y, EVO_SIZE_BG.z), CObject2D::UPDATE_TYPE_NONE);			//選択肢(quit)

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		m_pItem[nCntItem] = CItem2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + fMid, SCREEN_HEIGHT * 0.5f, 0.0f), CItem2D::TYPE(rand() % CItem2D::TYPE_MAX));			//選択肢(quit)
	
		fMid += EVO_MID;
	}

	m_nPauseSelect = 0;

	//タイプ設定
	SetType(TYPE_GAUGE);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CGrowSelecter::Init(const D3DXVECTOR3 pos)
{
	m_pBg = CObject2D::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(EVO_SIZE_BG.x, EVO_SIZE_BG.y, EVO_SIZE_BG.z), CObject2D::UPDATE_TYPE_NONE);			//選択肢(bg)

	//間隔を個数分用意
	float fMid = -EVO_MID * (MAX_ITEM / 2);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		m_pItem[nCntItem] = CItem2D::Create(D3DXVECTOR3(pos.x, pos.y + fMid, 0.0f), CItem2D::TYPE_000_HEAL);			//選択肢(quit)

		fMid += EVO_MID;
	}

	m_nPauseSelect = 0;

	//タイプ設定
	SetType(TYPE_GAUGE);

	return S_OK;
}

//============================
// 終了
//============================
void CGrowSelecter::Uninit(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}

	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	Release();
}

//============================
// 更新
//============================
void CGrowSelecter::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInstance()->GetInputGamepad();
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CFade *pFade = CScene::GetFade();
	int nNum[2] = {};

	if (pInputKeyboard->GetTrigger(DIK_A) || pInputPad->GetPress(CInputGamepad::BUTTON_LEFT, 0) == true)
	{
		nNum[0] = m_nPauseSelect;

		m_nPauseSelect--;

		if (m_nPauseSelect < 0)
		{//下限突破時
			m_nPauseSelect = MAX_ITEM - 1;
		}
	
		nNum[1] = m_nPauseSelect;
	}
	else if (pInputKeyboard->GetTrigger(DIK_D) || pInputPad->GetPress(CInputGamepad::BUTTON_RIGHT, 0) == true)
	{
		nNum[0] = m_nPauseSelect;

		m_nPauseSelect++;

		m_nPauseSelect %= MAX_ITEM;

		nNum[1] = m_nPauseSelect;
	}

	if (nNum[0] != nNum[1])
	{//選択先が変わったら、選択肢の色変え
		m_pItem[nNum[0]]->SetTexCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		m_pItem[nNum[1]]->SetTexCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if ((pInputKeyboard->GetTrigger(DIK_RETURN) || pInputKeyboard->GetTrigger(DIK_SPACE) || 
		pInputPad->GetPress(CInputGamepad::BUTTON_A, 0) == true)
		&& m_bEnter == false)
	{
		m_bEnter = true;

		//Playerに効果を付与するのはどっちの方がいい？evo?item2D?
		m_pItem[m_nPauseSelect]->PlayerAdd();

		CPlayer *pPlayer = CGame::GetPlayer();
		
		if (pPlayer != nullptr)
		{
		
		}
		else
		{
			pPlayer = nullptr;
		}

		CManager::GetInstance()->SetGrow(false);
		Uninit();
		return;
	}
}

//============================
// 描画
//============================
void CGrowSelecter::Draw(void)
{

}

//============================
// ポーズの生成
//============================
CGrowSelecter * CGrowSelecter::Create()
{
	CGrowSelecter *pPause = nullptr;

	//オブジェクト生成&初期化
	pPause = new CGrowSelecter;

	if (pPause != nullptr)
	{
		if (FAILED(pPause->Init()))
		{
			return nullptr;
		}
		else
		{
			CTexture *pTexture = CManager::GetInstance()->GetTexture();

			for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
			{
				if (pPause->m_pItem[nCntItem] != nullptr)
				{
					if (pPause->m_nPauseSelect != nCntItem)
					{
						pPause->m_pItem[nCntItem]->SetTexCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
					}
				}
			}
			
			pPause->m_pBg->SetTexCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f));
			pPause->m_pBg->SetIdxTexture(-1);
			//pPause->m_pBg->SetIdxTexture(pTexture->Regist(EVO_BG_FILE));
		}
	}
	else
	{
		return nullptr;
	}

	return pPause;
}
