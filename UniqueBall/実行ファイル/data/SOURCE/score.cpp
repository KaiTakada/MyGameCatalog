//===============================================
//
// スコア(score.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "score.h"
#include "number.h"

//============================
// マクロ定義
//============================

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
//============================
CScore::CScore()
{
	for (int nCntCrt = 0; nCntCrt < MAX_SCORE; nCntCrt++)
	{
		if (m_apNumber[nCntCrt] != nullptr)
		{
			m_apNumber[nCntCrt] = nullptr;
		}
	}

	m_nValue = 0;
	m_nNum = 0;
}

//============================
// デストラクタ
//============================
CScore::~CScore()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CScore::Init(void)
{
	SetType(TYPE_SCORE);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CScore::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	pos;
	rot;

	SetType(TYPE_SCORE);

	return S_FALSE;
	//return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CScore::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, int nNum)
{
	D3DXVECTOR3 posDif = pos;
	m_nNum = nNum;

	for (int nCntCrt = 0; nCntCrt < nNum; nCntCrt++)
	{
		if (m_apNumber[nCntCrt] == nullptr)
		{
			m_apNumber[nCntCrt] = CNumber::Create(posDif, rot, size, 0);		//自身を代入

			posDif.x += size.x * 2;
		}
	}

	SetType(TYPE_SCORE);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CScore::Uninit(void)
{
	for (int nCntCrt = 0; nCntCrt < MAX_SCORE; nCntCrt++)
	{
		if (m_apNumber[nCntCrt] != nullptr)
		{
			m_apNumber[nCntCrt]->Uninit();
			m_apNumber[nCntCrt] = nullptr;
		}
	}

	Release();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CScore::Update(void)
{

}

//============================
// 2Dポリゴンの描画処理
//============================
void CScore::Draw(void)
{

}

//============================
// 2Dポリゴンの生成処理
//============================
CScore * CScore::Create(void)
{
	CScore *pScore;

	//オブジェクト生成
	pScore = new CScore;

	//初期化
	pScore->Init();

	return pScore;
}

//============================
// 2Dポリゴンの生成処理
//============================
CScore * CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, int nNum)
{
	CScore *pScore = nullptr;

	//オブジェクト生成&初期化
	if (pScore == nullptr)
	{
		pScore = new CScore;

		if (pScore != nullptr)
		{
			pScore->Init(pos, rot, size, nNum);
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
// スコア値設定
//============================
void CScore::SetValue(int nValue)
{
	m_nValue = nValue; 

	int aTex[MAX_SCORE] = {};				//各桁の数字を格納
	int nCalc = nValue;

	//数値の設定

	//テクスチャ座標の計測
	for (int nCntScore = m_nNum - 1; nCntScore >= 0; nCntScore--)
	{
		aTex[nCntScore] = nCalc % 10;
		nCalc /= 10;
	}

	for (int nCntSet = 0; nCntSet < m_nNum; nCntSet++)
	{
		m_apNumber[nCntSet]->SetNumber(aTex[nCntSet]);
	}
}

//============================
// スコア値加算
//============================
void CScore::CntValue(int nValue)
{
	m_nValue += nValue;

	SetValue(m_nValue);
}


//============================
// 位置設定
//============================
void CScore::SetPos(const D3DXVECTOR3 pos)
{
	pos;
}

//============================
// 向き設定
//============================
void CScore::SetRot(const D3DXVECTOR3 rot)
{
	rot;
}

//============================
// 位置取得
//============================
D3DXVECTOR3 CScore::GetPos(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
// 2Dサイズ取得
//============================
D3DXVECTOR3 CScore::GetSize(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
