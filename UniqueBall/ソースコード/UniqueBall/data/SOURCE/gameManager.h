//===============================================
//
// ゲーム(game.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _GAMEMANAGER_H_		//このマクロ定義がされてなかったら
#define _GAMEMANAGER_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"

//=========================
// 定数定義
//=========================

//=========================
// 前方宣言
//=========================
class CObject2D;
class CNumber;

//=========================
// 画面(シーン)クラス
//=========================
class CGameManager
{
public:
	CGameManager();
	~CGameManager();

	enum FlowStage
	{
		NONE = 0,
		START,
		INGAME,
		GAMESET,
		MAX
	};

	HRESULT Init();		//初期化
	void Uninit();		//終了
	void Update();		//更新
	void Draw();		//描画
	static CGameManager *Create();		//生成

	void SetFlowStage(FlowStage set);						//状況設定
	FlowStage GetFlowStage() { return m_FlowStage; }		//状況取得
	void SetEnd(bool set) { m_bEnd = set; }	//終了フラグ設定
	bool GetEnd() { return m_bEnd; }		//終了フラグ取得

protected:

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CGameManager::* UPD_FUNC)(void);
	typedef void(CGameManager::* CRT_FUNC)(void);

	static UPD_FUNC m_UpdateFuncList[];
	static CRT_FUNC m_CreateFuncList[];

	void CreateStart();		//スタート時生成
	void CreateGameset();		//エンド時時生成

	void UpdateStart();		//スタート時更新
	void UpdateGameset();		//エンド時更新

	FlowStage m_FlowStage;		//状況管理
	CObject2D* m_OnePoint;		//スタート＆エンドのポリゴン
	CNumber* m_pCountdown;		//カウントダウンのポリゴン
	int m_nStayTime;			//とどまる時間
	int m_nRotateTime;			//まわる時間
	bool m_bEnd;				//処理終了のお知らせ
};

#endif