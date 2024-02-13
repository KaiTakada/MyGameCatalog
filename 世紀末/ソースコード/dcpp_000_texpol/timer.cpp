//===============================================
//
// スコア(score.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "timer.h"
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
CTimer::CTimer()
{
	for (int nCntCrt = 0; nCntCrt < MAX_TIME; nCntCrt++)
	{
		if (m_apNumber[nCntCrt] != nullptr)
		{
			m_apNumber[nCntCrt] = nullptr;
		}
	}

	m_nValue = 0;
	m_nSecond = 0;
}

//============================
// デストラクタ
//============================
CTimer::~CTimer()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CTimer::Init(void)
{
	SetType(TYPE_SCORE);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CTimer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	pos;
	rot;

	SetType(TYPE_SCORE);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CTimer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	D3DXVECTOR3 posDif = pos;

	for (int nCntAll = 0; nCntAll < MAX_TIME; nCntAll++)
	{
		if (m_apNumber[nCntAll] == nullptr)
		{
			m_apNumber[nCntAll] = CNumber::Create(posDif, rot, size, 0);		//自身を代入

			posDif.x += size.x * 2;
		}
	}

	SetType(TYPE_SCORE);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CTimer::Uninit(void)
{
	for (int nCntCrt = 0; nCntCrt < MAX_TIME; nCntCrt++)
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
void CTimer::Update(void)
{
	m_nSecond++;

	if (m_nSecond >= 60)
	{
		CntValue(-1);
	
		m_nSecond = 0;
	}
}

//============================
// 2Dポリゴンの描画処理
//============================
void CTimer::Draw(void)
{

}

//============================
// 2Dポリゴンの生成処理
//============================
CTimer * CTimer::Create(void)
{
	CTimer *pScore;

	//オブジェクト生成
	pScore = new CTimer;

	//初期化
	pScore->Init();

	return pScore;
}

//============================
// 2Dポリゴンの生成処理
//============================
CTimer * CTimer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	CTimer *pScore = nullptr;

	//オブジェクト生成&初期化
	if (pScore == nullptr)
	{
		pScore = new CTimer;

		if (pScore != nullptr)
		{
			pScore->Init(pos, rot, size);
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
void CTimer::SetValue(int nValue)
{
	m_nValue = nValue; 

	int aTex[MAX_TIME] = {};				//各桁の数字を格納
	int nCalc = nValue;

	//数値の設定

	//テクスチャ座標の計測
	for (int nCntScore = MAX_TIME - 1; nCntScore >= 0; nCntScore--)
	{
		aTex[nCntScore] = nCalc % 10;
		nCalc /= 10;
	}

	for (int nCntSet = 0; nCntSet < MAX_TIME; nCntSet++)
	{
		m_apNumber[nCntSet]->SetNumber(aTex[nCntSet]);
	}
}

//============================
// スコア値加算
//============================
void CTimer::CntValue(int nValue)
{
	m_nValue += nValue;

	SetValue(m_nValue);
}


//============================
// 位置設定
//============================
void CTimer::SetPos(const D3DXVECTOR3 pos)
{
	pos;
}

//============================
// 向き設定
//============================
void CTimer::SetRot(const D3DXVECTOR3 rot)
{
	rot;
}

//============================
// 位置取得
//============================
D3DXVECTOR3 CTimer::GetPos(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
// 2Dサイズ取得
//============================
D3DXVECTOR3 CTimer::GetSize(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
