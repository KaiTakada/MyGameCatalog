//===============================================
//
// リザルトUI(ui_result.cpp)
// Author : 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "ui_result.h"
#include "object2D.h"
#include "texture.h"
#include "camera_game.h"
#include "game.h"
#include "input.h"
#include "move2D.h"
#include "sound.h"

//============================
// マクロ定義
//============================
namespace
{
	const D3DXVECTOR3 DEF_SIZE = { SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT* 0.5f, 0.0f };	//デフォルトのマークのサイズ
	const char* FRAME_PASS = "data\\TEXTURE\\UI\\RESULT\\frame.png";	//frame.png
	const char* FRAMECOVER_PASS = "data\\TEXTURE\\UI\\RESULT\\frame_cover.png";	//frame_cover.png
	const D3DXVECTOR3 DEF_POS_DOUBLE[mygame::NUMPLAYER] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.5f, 0.0f) };	//デフォルトのプレイヤー2人用位置
}

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
//============================
CUI_Result::CUI_Result()
{
	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		if (m_apFrame[i] != nullptr)
		{
			m_apFrame[i] = nullptr;
		}

		m_bOK[i] = false;
	}

	m_bTrans = false;
}

//============================
// デストラクタ
//============================
CUI_Result::~CUI_Result()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CUI_Result::Init()
{
	D3DXVECTOR3 posDif = mylib_const::DEFVEC3;

	int nNumPlayer = CManager::GetInstance()->GetScene()->GetNumPlayer();

	for (int i = 0; i < nNumPlayer; i++)
	{
		posDif = DEF_POS_DOUBLE[i];

		if (m_apFrame[i] == nullptr)
		{
			m_apFrame[i] = CObject2D::Create(posDif, mylib_const::DEFVEC3, DEF_SIZE, CObject2D::UPDATE_TYPE_NONE);
		
			CTexture *pTex = CManager::GetInstance()->GetTexture();
			
			//テクスチャ付与
			m_apFrame[i]->SetIdxTexture(pTex->Regist(FRAME_PASS));

			//posDif.x += (DEF_SIZE.x * 1.5f);		//数字1つ分×1つ分(:)
		}
	}

	SetType(TYPE_TIMER);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CUI_Result::Uninit(void)
{
	for (int nCntDel = 0; nCntDel < mylib_const::MAX_PLAYER; nCntDel++)
	{
		if (m_apFrame[nCntDel] != nullptr)
		{
			m_apFrame[nCntDel]->Uninit();
			m_apFrame[nCntDel] = nullptr;
		}
	}

	Release();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CUI_Result::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInstance()->GetInputGamepad();
	int nNumPlayer = CManager::GetInstance()->GetScene()->GetNumPlayer();
	bool bOK[mylib_const::MAX_PLAYER] = { false };
	if (m_bTrans == true)
	{
		return;
	}

	for (int i = 0; i < nNumPlayer; i++)
	{
		if (pInputPad->GetPress(CInputGamepad::BUTTON_A, i) == true)
		{
			if (m_bOK[i])
			{
				continue;
			}
			m_bOK[i] = true;
			bOK[i] = true;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN))
	{
		for (int i = 0; i < nNumPlayer; i++)
		{
			if (m_bOK[i])
			{
				continue;
			}
			m_bOK[i] = true;
			bOK[i] = true;
		}
	}

	int nOK = 0;
	for (int i = 0; i < nNumPlayer; i++)
	{//遷移判定
		if (m_bOK[i] == false)
		{
			continue;
		}

		nOK++;

		if (m_apFrame[i] != nullptr && bOK[i])
		{//蓋
			//CMove2D *pMove2D = CMove2D::Create(
			//	D3DXVECTOR3(m_apFrame[i]->GetPos().x, -SCREEN_HEIGHT, 0.0f),
			//	m_apFrame[i]->GetPos(),
			//	100.0f);
			CMove2D *pMove2D = CMove2D::Create(
				D3DXVECTOR3(m_apFrame[i]->GetPos()),
				m_apFrame[i]->GetPos(),
				0.0f);
			pMove2D->SetSize(DEF_SIZE);

			CTexture *pTex = CManager::GetInstance()->GetTexture();
			pMove2D->SetIdxTexture(pTex->Regist(FRAMECOVER_PASS));
		
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DOOR);
		}
	}

	if (nOK == nNumPlayer)
	{
		m_bTrans = true;
	}
}

//============================
// 2Dポリゴンの描画処理
//============================
void CUI_Result::Draw(void)
{

}

//============================
// 2Dポリゴンの生成処理
//============================
CUI_Result * CUI_Result::Create(void)
{
	CUI_Result *pScore;

	//オブジェクト生成
	pScore = new CUI_Result;

	//初期化
	pScore->Init();

	return pScore;
}
