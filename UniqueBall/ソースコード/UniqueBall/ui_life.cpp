//===============================================
//
// スコア(score.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "ui_life.h"
#include "object2D.h"
#include "texture.h"
#include "camera_game.h"
#include "game.h"

//============================
// マクロ定義
//============================
namespace
{
	const D3DXVECTOR3 DEF_SIZE = { 50.0f, 50.0f ,0.0f };	//デフォルトのマークのサイズ
	const char* TIME_COLON_PASS = "data\\TEXTURE\\UI\\life00.png";	//life.png
	const D3DXVECTOR3 DEF_POS_DOUBLE[mygame::NUMPLAYER] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.0f + DEF_SIZE.x, SCREEN_HEIGHT - DEF_SIZE.y ,0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2.0f + DEF_SIZE.x, SCREEN_HEIGHT - DEF_SIZE.y ,0.0f) };	//デフォルトのプレイヤー2人用位置
}

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
//============================
CUI_Life::CUI_Life()
{
	for (int i = 0; i < myUI_life::MAX_NUM_LIFE; i++)
	{
		if (m_apMark[i] != nullptr)
		{
			m_apMark[i] = nullptr;
		}
	}

	m_pos = mylib_const::DEFVEC3;
	m_nNumMark = 0;
	m_nIdx = -1;
}

//============================
// デストラクタ
//============================
CUI_Life::~CUI_Life()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CUI_Life::Init()
{
	D3DXVECTOR3 posDif = DEF_POS_DOUBLE[m_nIdx];

	for (int nCntAll = 0; nCntAll < m_nNumMark; nCntAll++)
	{
		if (m_apMark[nCntAll] == nullptr)
		{
			m_apMark[nCntAll] = CObject2D::Create(posDif, mylib_const::DEFVEC3, DEF_SIZE, CObject2D::UPDATE_TYPE_NONE);
		
			CTexture *pTex = CManager::GetInstance()->GetTexture();
			
			//テクスチャ付与
			m_apMark[nCntAll]->SetIdxTexture(pTex->Regist(TIME_COLON_PASS));

			posDif.x += (DEF_SIZE.x * 1.5f);		//数字1つ分×1つ分(:)
		}
	}

	SetType(TYPE_TIMER);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CUI_Life::Uninit(void)
{
	for (int nCntDel = 0; nCntDel < m_nNumMark; nCntDel++)
	{
		if (m_apMark[nCntDel] != nullptr)
		{
			m_apMark[nCntDel]->Uninit();
			m_apMark[nCntDel] = nullptr;
		}
	}

	Release();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CUI_Life::Update(void)
{
	DWORD dwTimeNow = timeGetTime();
	DWORD dwTimeOld = timeGetTime();
}

//============================
// 2Dポリゴンの描画処理
//============================
void CUI_Life::Draw(void)
{

}

//============================
// 2Dポリゴンの生成処理
//============================
CUI_Life * CUI_Life::Create(void)
{
	CUI_Life *pScore;

	//オブジェクト生成
	pScore = new CUI_Life;

	//初期化
	pScore->Init();

	return pScore;
}

//============================
// 2Dポリゴンの生成処理
//============================
CUI_Life * CUI_Life::Create(const int nNumMark, const int nIdx)
{
	CUI_Life *pScore = nullptr;

	//オブジェクト生成&初期化
	if (pScore == nullptr)
	{
		pScore = new CUI_Life;

		if (pScore != nullptr)
		{
			pScore->SetNumMark(nNumMark);
			pScore->SetIdx(nIdx);
			pScore->Init();
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	return pScore;
}

//============================
// 位置設定
//============================
void CUI_Life::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	D3DXVECTOR3 posDif = pos;

	if (m_nIdx != -1)
	{
		CScene *pScene = CManager::GetInstance()->GetScene();
		if (pScene != nullptr)
		{
			if (pScene->GetMode() == CScene::MODE_GAME)
			{
				CGame *pGame = nullptr;
				pGame = static_cast<CGame*>(pScene);

				CCameraGame *pCamera = pGame->GetCamera(m_nIdx);

				if (pCamera != nullptr)
				{
					D3DVIEWPORT9 viewport = pCamera->GetViewPort();
					posDif.x += viewport.X;
					posDif.y += viewport.Y;
				}
			}
		}
	}

	for (int i = 0; i < myUI_life::MAX_NUM_LIFE; i++)
	{
		if (m_apMark[i] != nullptr)
		{
			m_apMark[i]->SetPos(posDif);
			posDif += m_apMark[i]->GetSize();
		}
	}
}

//============================
// マーク増加
//============================
void CUI_Life::AddLife(const int nIdx)
{
	D3DXVECTOR3 posDif = mylib_const::DEFVEC3;

	if (m_nNumMark <= 0)
	{
		posDif = DEF_POS_DOUBLE[m_nIdx];
	}
	else
	{
		posDif = m_apMark[m_nIdx - 1]->GetPos();
	}

	for (int i = m_nNumMark; i < m_nNumMark + nIdx; i++)
	{
		if (m_apMark[i] == nullptr)
		{
			m_apMark[i] = CObject2D::Create(posDif, mylib_const::DEFVEC3, DEF_SIZE, CObject2D::UPDATE_TYPE_NONE);

			CTexture *pTex = CManager::GetInstance()->GetTexture();

			//テクスチャ付与
			m_apMark[i]->SetIdxTexture(pTex->Regist(TIME_COLON_PASS));

			posDif.x += (DEF_SIZE.x * 1.5f);		//数字1つ分×1つ分(:)
		}
	}

	m_nNumMark += nIdx;
}

//============================
// マーク減少
//============================
void CUI_Life::SubLife(const int nIdx)
{
	if (nIdx <= 0)
	{
		return;
	}

	int nNum = nIdx;
	if (m_nNumMark < nNum)
	{
		nNum = m_nNumMark;
	}

	for (int i = m_nNumMark - 1; i > m_nNumMark - nNum - 1; i--)
	{
		if (m_apMark[i] != nullptr)
		{
			m_apMark[i]->SetDeath(true);
		}
	}

	m_nNumMark -= nNum;
}
