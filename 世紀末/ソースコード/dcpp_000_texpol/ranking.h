//===============================================
//
// ランキング(ranking.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _RANKING_H_		//このマクロ定義がされてなかったら
#define _RANKING_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"
#include "manager.h"

//=========================
// マクロ定義
//=========================
#define RK_NUM_SCORE (5)	//表示するスコア数
#define RANK_READ_FILE "data\\SET\\RANKING\\rank.txt"

//=========================
// 前方宣言
//=========================

//=========================
// 画面(シーン)クラス
//=========================
class CRanking : public CScene
{
public:
	CRanking();
	~CRanking();

	virtual HRESULT Init();		//初期化
	virtual void Uninit();		//終了
	virtual void Update();		//更新
	virtual void Draw();		//描画
	static CRanking *Create();		//生成
	void SetScore(int nScore) { m_nScore = nScore; }		//生成
	int GetScore() { return m_nScore; }		//生成

protected:

private:
	void Sort();			//ランキングソート
	void Read(char *pReadFile = RANK_READ_FILE);			//ファイル読込
	void Write(char *pReadFile = RANK_READ_FILE);			//ファイル書込

	CScore *m_apScore[RK_NUM_SCORE];
	int m_nScore;		//今回のスコア
	int m_nCntTitle;	//タイトル遷移
};

#endif