//===============================================
//
// リザルトUI(ui_result.cpp)
// Author : 髙田 佳依
//
//===============================================
#ifndef _UI_RESULT_H_		//このマクロ定義がされてなかったら
#define _UI_RESULT_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "object.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================
class CScore;
class CObject2D;

//=========================
// オブジェクト2D背景クラス
//=========================
class CUI_Result : public CObject
{
public:
	CUI_Result();		//コンストラクタ
	~CUI_Result();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CUI_Result *Create(void);		//生成

	void SetMark(CObject2D* pMark, int nIdx) { m_apFrame[nIdx] = pMark; }			//位置設定
	CObject2D **GetMark() { return m_apFrame; }		//位置取得
	CObject2D *GetMark(int nIdx) { return m_apFrame[nIdx]; }		//位置取得

	bool GetTrans() { return m_bTrans; }	//遷移フラグ取得

protected:

private:
	CObject2D *m_apFrame[mylib_const::MAX_PLAYER];		//スコア情報
	bool m_bOK[mylib_const::MAX_PLAYER];	//遷移同意フラグ
	bool m_bTrans;	//遷移フラグ
};

#endif