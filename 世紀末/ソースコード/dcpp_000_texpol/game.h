//===============================================
//
// ゲーム(game.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _GAME_H_		//このマクロ定義がされてなかったら
#define _GAME_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"
#include "manager.h"

//=========================
// 前方宣言
//=========================
//class CRenderer;
class CPause;
class CGrowSelecter;
class CTimer;
class CEnemy_Boss;
class CResult;

//=========================
// 画面(シーン)クラス
//=========================
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	virtual HRESULT Init();		//初期化
	virtual void Uninit();		//終了
	virtual void Update();		//更新
	virtual void Draw();		//描画
	static CGame *Create();		//生成

	static HRESULT SetGrow(CGrowSelecter *pGrow);					//成長シーン設定

	static CScore *GetScore() { return m_pScore; }								//スコアの取得
	static CPlayer *GetPlayer() { return m_pPlayer; }							//プレイヤーの取得
	static CTimer *GetTimer() { return m_pTimer; }								//タイマーの取得
	static CGrowSelecter *GetGrow() { return m_pGrow; }							//成長シーン取得
	static CEnemy_Boss *GetBoss() { return m_pBoss; }								//ボス取得

	static void NullPause() { m_pPause = nullptr; }								//ポーズポインタをnullに
	static void NullGrow() { m_pGrow = nullptr; }									//成長シーンポインタをnullに

protected:

private:
	static CPlayer *m_pPlayer;					//プレイヤー
	static CPause *m_pPause;					//ポーズ
	static CGrowSelecter *m_pGrow;				//成長進化シーン
	static CScore *m_pScore;					//スコア
	static CTimer *m_pTimer;					//タイマー
	static CEnemy_Boss *m_pBoss;				//ボス
	CField *m_pField;							//地面
	CSpawn *m_pSpawn;							//敵スポーン
	CResult *m_pResult;							//リザルト
	bool m_bResult;								//リザルト
};

#endif