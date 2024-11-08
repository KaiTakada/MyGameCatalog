//===============================================
//
// スコア(score.cpp)
// Author 髙田 佳依
//
//===============================================
#ifndef _TIMER_H_		//このマクロ定義がされてなかったら
#define _TIMER_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "object.h"

//=========================
// マクロ定義
//=========================
#define MAX_TIME_SCORE (3)	//スコア桁数

//=========================
// 前方宣言
//=========================
class CScore;
class CObject2D;

//=========================
// オブジェクト2D背景クラス
//=========================
class CTimer : public CObject
{
public:
	struct STime
	{
		int nMinute;		//分
		int nSecond;		//秒
		int nMilliSecond;		//コンマ秒
		
		STime &operator+=(const STime &time);
	};

	CTimer();		//コンストラクタ
	~CTimer();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CTimer *Create(void);		//生成
	static CTimer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 size = D3DXVECTOR3(30.0f, 50.0f, 0.0f));		//生成

	void SetValue(int nValue);		//スコア値設定
	void CntValue(int nValue);		//スコア値加算
	int GetValue() { return m_nValue; }		//スコア値取得

	void SetTime(STime time);		//タイム設定
	void CntTime(STime time);		//タイム加算
	STime GetTime() { return m_time; }		//タイム取得

	//純粋仮装関数で仕方なしに作った。後で消したい
	void SetPos(const D3DXVECTOR3 pos);			//位置設定
	void SetRot(const D3DXVECTOR3 rot);			//向き設定
	void SetStop(const bool bStop) { m_bStop = bStop; }			//ストップ設定
	D3DXVECTOR3 GetPos() { return m_pos; }		//位置取得
	D3DXVECTOR3 GetRot() { return m_rot; }		//向き取得
	bool GetStop() { return m_bStop; }		//ストップ取得
	void CntStop() { m_bStop = m_bStop ? false : true; }		//ストップ取得

protected:

private:
	CScore *m_apScore[MAX_TIME_SCORE];		//スコア情報
	CObject2D *m_apColon[MAX_TIME_SCORE - 1];		//スコア情報
	STime m_time;		//時間型データ
	int m_nValue;		//スコアの値
	int m_nSecond;		//1秒判定数値
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	bool m_bStop;		//加算許さないマン
};

#endif