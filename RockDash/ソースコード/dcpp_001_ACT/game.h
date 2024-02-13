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
class CMap;

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

	static CPlayer *GetPlayer() { return m_pPlayer; }							//プレイヤーの取得
	static CPause *GetPause() { return m_pPause; }								//ポーズの取得
	static CTimer *GetTimer() { return m_pTimer; }								//タイマーの取得
	static CMap *GetMap() { return m_pMap; }								//マップの取得

	static void NullPause() { m_pPause = nullptr; }								//ポーズポインタをnullに
	static bool MapFlag() { return m_pMap != nullptr; }								//ポーズポインタをnullに

protected:

private:
	static CPlayer *m_pPlayer;					//プレイヤー
	static CPause *m_pPause;					//ポーズ
	static CTimer *m_pTimer;					//タイマー
	static CMap *m_pMap;						//マップ
	CField *m_pField;							//地面
};

#endif