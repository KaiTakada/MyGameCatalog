//===============================================
//
// ランキング(ranking.cpp)
// Author: 髙田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "object2D.h"
#include "texture.h"
#include "timer.h"

//=========================
// マクロ定義
//=========================

#define RANK_READ_FILE "data\\SET\\RANKING\\rank.txt"

//=========================
// 静的メンバ変数
//=========================

//============================
// コンストラクタ
//============================
CRanking::CRanking()
{
	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		m_apTimer[nCnt] = nullptr;
	}

	m_nScore = 0;
	m_nCntTitle = 0;
}

//============================
// デストラクタ
//============================
CRanking::~CRanking()
{

}

//============================
// 初期化
//============================
HRESULT CRanking::Init()
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	float fWidth = SCREEN_WIDTH;
	float fHeight = SCREEN_HEIGHT;

	CScene::Init();

	CManager::RESULT_TYPE type = CManager::GetInstance()->GetResult();

	//今回の結果を取得
	m_nScore = GetNowTime();

	//背景
	CObject2D *pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(fWidth * 0.5f, fHeight * 0.5f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\BG\\bg_002.jpg"));
	pPolygon->SetTexCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.2f));

	pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.2f, fHeight * 0.1f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(200.0f, 80.0f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

	//新記録画像パス
	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\ranking_logo.png"));

	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		fHeight = SCREEN_HEIGHT * (nCnt + 1) * 0.15f;
		fHeight += 100.0f;

		//順位ポリゴン
		pPolygon = CObject2D::Create(D3DXVECTOR3(fWidth * 0.3f, fHeight, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(100.0f, 50.0f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

		pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\ranking.png"));
		pPolygon->SetTexPos(D3DXVECTOR2(nCnt * 0.2f + 0.2f, 1.0f), D3DXVECTOR2(nCnt * 0.2f, 0.0f));

		//スコア
		if (m_apTimer[nCnt] != nullptr)
		{
			m_apTimer[nCnt]->Uninit();
			m_apTimer[nCnt] = nullptr;
		}

		m_apTimer[nCnt] = CTimer::Create(D3DXVECTOR3(fWidth * 0.5f, fHeight, 0.0f));

		if (m_apTimer[nCnt] == nullptr)
		{
			return S_FALSE;
		}
	}

	//読込
	Read();

	//ソート
	Sort();

	//書き込み
	Write();

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_RANKING);

	return S_OK;
}

//============================
// 終了
//============================
void CRanking::Uninit()
{
	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		if (m_apTimer[nCnt] != nullptr)
		{
			m_apTimer[nCnt]->Uninit();
			m_apTimer[nCnt] = nullptr;
		}
	}

	CScene::Uninit();

	CManager::GetInstance()->GetSound()->Stop();
}

//============================
// 更新
//============================
void CRanking::Update()
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamepad *pInputPad = CManager::GetInstance()->GetInputGamepad();
	CFade *pFade = CScene::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputKeyboard->GetTrigger(DIK_SPACE) ||
		pInputPad->GetPress(CInputGamepad::BUTTON_A, 0) == true)
	{//[ Enter ]キーでポーズ	
		if (pFade->SetState(CScene::MODE_TITLE))
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BUTTON);
		}
	}
	else if (m_nCntTitle >= 1200)
	{
		pFade->SetState(CScene::MODE_TITLE);
	}

	CScene::Update();
}

//============================
// 描画
//============================
void CRanking::Draw()
{
	CScene::Draw();
}

//============================
// 生成
//============================
CRanking * CRanking::Create()
{
	CRanking *pResult = nullptr;

	//オブジェクト生成&初期化
	pResult = new CRanking;

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

//============================
// ランキングソート
//============================
void CRanking::Sort()
{
	int aScore[RK_NUM_SCORE] = {};
	int nIdxNow = -1;

	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{//データ代入
		if (m_apTimer[nCnt] != nullptr)
		{
			aScore[nCnt] = m_apTimer[nCnt]->GetValue();
		}
	}

	//降順に並べ替える(>)(小が上)
	//昇順に並べ替える(<)(大が上)
	if (m_nScore < aScore[RK_NUM_SCORE - 1])
	{
		aScore[RK_NUM_SCORE - 1] = m_nScore;

		for (int nCntMax = 0; nCntMax < RK_NUM_SCORE; nCntMax++)
		{
			int nHidata = nCntMax;

			for (int nCntSort = nCntMax + 1; nCntSort < RK_NUM_SCORE; nCntSort++)
			{
				if (aScore[nHidata] > aScore[nCntSort])
				{
					nHidata = nCntSort;
				}
			}

			if (nHidata != nCntMax)
			{//選択した場所が変わっていたら
				int nTemp = aScore[nCntMax];
				aScore[nCntMax] = aScore[nHidata];
				aScore[nHidata] = nTemp;

				nIdxNow = nCntMax;
			}
		}

		for (int nCntComp = 0; nCntComp < RK_NUM_SCORE; nCntComp++)
		{//データ再代入

			if (m_apTimer[nCntComp] != nullptr)
			{
				m_apTimer[nCntComp]->SetValue(aScore[nCntComp]);
			}
		}

		////新記録だったら追加表示
		//if (nIdxNow != -1)
		//{
		//	D3DXVECTOR3 pos = m_apTimer[nIdxNow]->GetPos();

		//	CTexture *pTexture = CManager::GetInstance()->GetTexture();

		//	CObject2D *pPolygon = CObject2D::Create(D3DXVECTOR3(pos.x + 450.0f, pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		//		D3DXVECTOR3(200.0f, 40.0f, 0.0f), CObject2D::UPDATE_TYPE_NONE);

		//	//新記録画像パス
		//	pPolygon->SetIdxTexture(pTexture->Regist("data\\TEXTURE\\SCENE\\now.png"));
		//}
	}
}

//==========================
//ランキングのリセット処理
//==========================
void CRanking::Read(char *pReadFile)
{
	int aRankScore[5] = { 0,0,0,0,0 };

	FILE *pFile;									//ファイルポイントを宣言

	pFile = fopen(pReadFile, "r");

	if (pFile == NULL)
	{//ファイル開けなかったら
		return;
	}

	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		fscanf(pFile, "%d", &aRankScore[nCnt]);
	}

	fclose(pFile);

	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		if (m_apTimer[nCnt] != nullptr)
		{
			m_apTimer[nCnt]->SetValue(aRankScore[nCnt]);
		}
	}
}
//==========================
//ランキングのリセット処理
//==========================
void CRanking::Write(char *pReadFile)
{
	FILE *pFile;									//ファイルポイントを宣言

	pFile = fopen(pReadFile, "w");

	if (pFile == NULL)
	{//ファイル開けなかったら
		return;
	}

	for (int nCnt = 0; nCnt < RK_NUM_SCORE; nCnt++)
	{
		if (m_apTimer[nCnt] != nullptr)
		{
			fprintf(pFile, "%d\n", m_apTimer[nCnt]->GetValue());
		}
	}

	fclose(pFile);
}
