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
#include "scene.h"

//=========================
// 定数定義
//=========================
namespace mygame
{
	const int NUMPLAYER = 2;	//プレイヤーの固定人数
	const int NUM_WALL = 4;		//壁の固定数
}

//=========================
// 前方宣言
//=========================
//class CRenderer;
class CPause;
class CGrowSelecter;
class CTimer;
class CMap;
class CCameraPlayer;
class CWall;
class CGameManager;
class CObjMeshField;

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

	static CPause *GetPause() { return m_pPause; }								//ポーズの取得
	static CTimer *GetTimer() { return m_pTimer; }								//タイマーの取得
	static CMap *GetMap() { return m_pMap; }								//マップの取得

	static void NullPause() { m_pPause = nullptr; }								//ポーズポインタをnullに
	static bool MapFlag() { return m_pMap != nullptr; }								//ポーズポインタをnullに
	CCameraPlayer *GetCamera(int nIdx) { return m_ppCamera[nIdx]; }					//分割カメラの1つを取得
	CCameraPlayer **GetCamera() { return m_ppCamera; }								//分割カメラの取得
	CObjMeshField* GetField() { return m_pField; }						//床を取得
	CWall *GetWall(int nIdx) { return m_ppWall[nIdx]; }					//壁の1つを取得
	CWall **GetWall() { return m_ppWall; }								//壁の取得

protected:

private:
	static CPause *m_pPause;					//ポーズ
	static CTimer *m_pTimer;					//タイマー
	static CMap *m_pMap;						//マップ
	CObjMeshField *m_pField;							//地面
	CWall *m_ppWall[mygame::NUM_WALL];							//地面
	CEnemy *m_pBot;								//球投げボット
	CCameraPlayer *m_ppCamera[mylib_const::MAX_PLAYER];		//プレイヤー追従カメラ
	CGameManager* m_pGameManager;	//ゲームマネージャー
};

#endif