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
#define MAX_TIME (3)	//スコア桁数

//=========================
// 前方宣言
//=========================
class CNumber;

//=========================
// オブジェクト2D背景クラス
//=========================
class CTimer : public CObject
{
public:
	CTimer();		//コンストラクタ
	~CTimer();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CTimer *Create(void);		//生成
	static CTimer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size);		//生成

	void SetValue(int nValue);		//スコア値設定
	void CntValue(int nValue);		//スコア値加算
	int GetValue() { return m_nValue; }		//スコア値取得

	//純粋仮装関数で仕方なしに作った。後で消したい
	void SetPos(const D3DXVECTOR3 pos);			//位置設定
	void SetRot(const D3DXVECTOR3 rot);			//向き設定
	D3DXVECTOR3 GetPos(void);		//位置取得
	D3DXVECTOR3 GetSize(void);		//2Dサイズ取得
	bool GetJump(void) { return false; }		//ジャンプ判定
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//前回位置取得

protected:

private:
	CNumber *m_apNumber[MAX_TIME];		//スコア情報
	int m_nValue;		//スコアの値
	int m_nSecond;		//1秒判定数値
};

#endif