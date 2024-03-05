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
#include "game.h"
#include "pause.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//=======================
//マクロ定義
//=======================
#define MAX_PAUSE (PAUSE_MENU_MAX)			//ポーズの選択肢数
#define PAUSE_MID (100)						//ポーズの選択肢どうしの間（縦）
#define PAUSE_SIZE D3DXVECTOR3(250.0f,30.0f,0.0f)			//タイトル名のサイズ
#define PAUSE_SIZE_BG D3DXVECTOR3(350.0f,400.0f,0.0f)			//タイトル名のサイズ

//=======================
// ファイルパス
//=======================
const char *c_apFilenamePause[] =
{
	"data\\TEXTURE\\PAUSE\\continue.png",
	"data\\TEXTURE\\PAUSE\\retry.png",
	"data\\TEXTURE\\PAUSE\\quit.png",
	"data\\TEXTURE\\PAUSE\\Bg.png",
};

//=======================
// 静的メンバ変数
//=======================

//============================
// コンストラクタ
//============================
CPause::CPause(int nPriority) : CObject(nPriority)
{
	for (int nCntItem = 0; nCntItem < PAUSE_MENU_MAX; nCntItem++)
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
CPause::~CPause()
{

}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CPause::Init()
{
	m_pBg = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXVECTOR3(PAUSE_SIZE_BG.x, PAUSE_SIZE_BG.y, PAUSE_SIZE_BG.z), CObject2D::UPDATE_TYPE_NONE);			//選択肢(quit)

	for (int nCntItem = 0; nCntItem < PAUSE_MENU_MAX; nCntItem++)
	{
		m_pItem[nCntItem] = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_WIDTH * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(PAUSE_SIZE.x, PAUSE_SIZE.y, PAUSE_SIZE.z), CObject2D::UPDATE_TYPE_NONE);			//選択肢(quit)
	}

	m_nPauseSelect = 0;

	//タイプ設定
	SetType(TYPE_GAUGE);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CPause::Init(const D3DXVECTOR3 pos)
{
	m_pBg = CObject2D::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(PAUSE_SIZE_BG.x, PAUSE_SIZE_BG.y, PAUSE_SIZE_BG.z), CObject2D::UPDATE_TYPE_NONE);			//選択肢(bg)

	//間隔を個数分用意
	float fMid = -PAUSE_MID * (PAUSE_MENU_MAX / 2);

	for (int nCntItem = 0; nCntItem < PAUSE_MENU_MAX; nCntItem++)
	{
		m_pItem[nCntItem] = CObject2D::Create(D3DXVECTOR3(pos.x, pos.y + fMid, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(PAUSE_SIZE.x, PAUSE_SIZE.y, PAUSE_SIZE.z), CObject2D::UPDATE_TYPE_NONE);			//選択肢(quit)

		fMid += PAUSE_MID;
	}

	m_nPauseSelect = 0;

	//タイプ設定
	SetType(TYPE_GAUGE);

	return S_OK;
}

//============================
// 終了
//============================
void CPause::Uninit(void)
{
	for (int nCntItem = 0; nCntItem < PAUSE_MENU_MAX; nCntItem++)
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
void CPause::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputGamepad = CManager::GetInstance()->GetInputGamepad();
	CFade *pFade = CScene::GetFade();
	int nNum[2] = {};

	if (pInputKeyboard->GetTrigger(DIK_W) ||
		pInputGamepad->GetTriggerOR(CInputGamepad::BUTTON_UP))
	{
		nNum[0] = m_nPauseSelect;

		m_nPauseSelect--;

		if (m_nPauseSelect < 0)
		{//下限突破時
			m_nPauseSelect = PAUSE_MENU_MAX - 1;
		}
	
		nNum[1] = m_nPauseSelect;
	}
	else if (pInputKeyboard->GetTrigger(DIK_S) ||
		pInputGamepad->GetTriggerOR(CInputGamepad::BUTTON_DOWN))
	{
		nNum[0] = m_nPauseSelect;

		m_nPauseSelect++;

		m_nPauseSelect %= PAUSE_MENU_MAX;

		nNum[1] = m_nPauseSelect;
	}

	if (nNum[0] != nNum[1])
	{//選択先が変わったら、選択肢の色変え
		m_pItem[nNum[0]]->SetTexCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		m_pItem[nNum[1]]->SetTexCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) ||
		pInputGamepad->GetTriggerOR(CInputGamepad::BUTTON_A)
		&& m_bEnter == false)
	{
		m_bEnter = true;

		switch (m_nPauseSelect)
		{
			case PAUSE_MENU_CONTINUE:
				break;

			case PAUSE_MENU_RETRY:
				pFade->SetState(CScene::MODE_GAME);
				break;

			case PAUSE_MENU_QUIT:
				pFade->SetState(CScene::MODE_TITLE);
				break;

			default:
			break;
		}

		CManager::GetInstance()->SetPause(false);
		CGame::NullPause();
		Uninit();
	}
}

//============================
// 描画
//============================
void CPause::Draw(void)
{

}

//============================
// ポーズの生成
//============================
CPause * CPause::Create(D3DXVECTOR3 pos)
{
	CPause *pPause = nullptr;

	//オブジェクト生成&初期化
	pPause = new CPause;

	if (pPause != nullptr)
	{
		if (FAILED(pPause->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			CTexture *pTexture = CManager::GetInstance()->GetTexture();

			for (int nCntItem = 0; nCntItem < PAUSE_MENU_MAX; nCntItem++)
			{
				if (pPause->m_pItem[nCntItem] != nullptr)
				{
					if (pPause->m_nPauseSelect != nCntItem)
					{
						pPause->m_pItem[nCntItem]->SetTexCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
					}
					
					pPause->m_pItem[nCntItem]->SetIdxTexture(pTexture->Regist(c_apFilenamePause[nCntItem]));
				}
			}
			
			pPause->m_pBg->SetTexCol(D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f));
			pPause->m_pBg->SetIdxTexture(-1);
			//pPause->m_pBg->SetIdxTexture(pTexture->Regist(c_apFilenamePause[PAUSE_MENU_MAX]));
		}
	}
	else
	{
		return nullptr;
	}

	return pPause;
}
