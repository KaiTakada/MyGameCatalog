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
#include "score.h"
#include "object2D.h"
#include "texture.h"

//============================
// マクロ定義
//============================
#define MAX_TIME_SCORE_DIGIT (2)	//スコア桁数
#define MAX_TIME_SCORE_DIGIT_LIMIT (60)	//スコア桁数の上昇値
#define TIME_COLON_PASS "data\\TEXTURE\\NUMBER\\colon.png"	//コロン.png

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
//============================
CTimer::CTimer()
{
	for (int nCntCrt = 0; nCntCrt < MAX_TIME_SCORE; nCntCrt++)
	{
		if (m_apScore[nCntCrt] != nullptr)
		{
			m_apScore[nCntCrt] = nullptr;
		}

		if (m_apColon[nCntCrt] != nullptr && nCntCrt < MAX_TIME_SCORE - 1)
		{
			m_apColon[nCntCrt] = nullptr;
		}
	}

	m_nValue = 0;
	m_nSecond = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3();
	m_bStop = false;

	ZeroMemory(&m_time, sizeof(m_time));
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
	SetType(TYPE_TIMER);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CTimer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	pos;
	rot;

	SetType(TYPE_TIMER);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CTimer::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	D3DXVECTOR3 posDif = pos;
	m_bStop = false;

	for (int nCntAll = 0; nCntAll < MAX_TIME_SCORE; nCntAll++)
	{
		if (m_apScore[nCntAll] == nullptr)
		{
			m_apScore[nCntAll] = CScore::Create(posDif, rot, size, 2);		//自身を代入

			posDif.x += (size.x * 2) * 2;		//数字1つ分×2つ分(00)
		}

		if (m_apColon[nCntAll] == nullptr && nCntAll < MAX_TIME_SCORE - 1)
		{
			m_apColon[nCntAll] = CObject2D::Create(posDif, rot, size, CObject2D::UPDATE_TYPE_NONE);
		
			CTexture *pTex = CManager::GetInstance()->GetTexture();
			
			//テクスチャ付与
			m_apColon[nCntAll]->SetIdxTexture(pTex->Regist(TIME_COLON_PASS));

			posDif.x += (size.x * 2);		//数字1つ分×1つ分(:)
		}
	}

	SetType(TYPE_TIMER);

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CTimer::Uninit(void)
{
	for (int nCntDel = 0; nCntDel < MAX_TIME_SCORE; nCntDel++)
	{
		if (m_apScore[nCntDel] != nullptr)
		{
			m_apScore[nCntDel]->Uninit();
			m_apScore[nCntDel] = nullptr;
		}
	
		if (m_apColon[nCntDel] != nullptr && nCntDel < MAX_TIME_SCORE - 1)
		{
			m_apColon[nCntDel]->Uninit();
			m_apColon[nCntDel] = nullptr;
		}
	}

	Release();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CTimer::Update(void)
{
	DWORD dwTimeNow = timeGetTime();
	DWORD dwTimeOld = timeGetTime();
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
	if (m_bStop != false)
	{//すでに停止されていた場合
		return;
	}

	m_nValue = nValue;

	STime lTime =
	{
	0,
	0,
	nValue
	};

	SetTime(lTime);
}

//============================
// スコア値加算
//============================
void CTimer::CntValue(int nValue)
{
	if (m_bStop != false)
	{//すでに停止されていた場合
		return;
	}

	m_nValue += nValue;
	m_time.nMilliSecond += nValue;

	SetValue(m_nValue);
}

//============================
// タイム設定
//============================
void CTimer::SetTime(STime time)
{
	if (m_bStop != false)
	{//すでに停止されていた場合
		return;
	}

	m_time = time;
	STime lTime = time;

	//各桁の数字を格納
	int aTex[MAX_TIME_SCORE] = 
	{
		lTime.nMinute,
		lTime.nSecond,
		lTime.nMilliSecond,
	};

	//数値の設定

	//テクスチャ座標の計測
	for (int nCntScore = MAX_TIME_SCORE - 1; nCntScore > 0; nCntScore--)
	{
		int nCntNext = nCntScore - 1;
		aTex[nCntNext] = aTex[nCntScore] / MAX_TIME_SCORE_DIGIT_LIMIT;
		aTex[nCntScore] %= MAX_TIME_SCORE_DIGIT_LIMIT;
	}

	lTime.nMinute = aTex[0];
	lTime.nSecond = aTex[1];
	lTime.nMilliSecond = aTex[2];

	for (int nCntSet = 0; nCntSet < MAX_TIME_SCORE; nCntSet++)
	{
		m_apScore[nCntSet]->SetValue(aTex[nCntSet]);
	}
}

//============================
// タイム加算
//============================
void CTimer::CntTime(STime time)
{
	if(m_bStop != false)
	{//すでに停止されていた場合
		return;
	}

	//オペレーターを使用した加算入
	m_time += time;

	SetTime(m_time);
}

//============================
// 位置設定
//============================
void CTimer::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	//スコア群にも与える
}

//============================
// 向き設定
//============================
void CTimer::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;

	//スコア群にも与える
}

//============================
// 時間型の加算入のオーバーロード[演算]
//============================
CTimer::STime & CTimer::STime::operator+=(const STime &time)
{
	nMinute += time.nMinute;
	nSecond += time.nSecond;
	nMilliSecond += time.nMilliSecond;

	return *this;
}
