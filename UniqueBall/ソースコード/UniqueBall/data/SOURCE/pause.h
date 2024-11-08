//===============================================
//
// ポーズ(pause.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//=========================
// インクルードファイル
//=========================
#include "object.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================
class CObject2D;

//=========================
// プレイヤークラス
//=========================
class CPause : public CObject
{
public:

	//ポーズメニューの列挙型
	enum PAUSE_MENU
	{
		PAUSE_MENU_CONTINUE = 0,	//ゲームに戻る
		PAUSE_MENU_RETRY,			//ゲームをやり直す
		PAUSE_MENU_QUIT,			//タイトル画面に戻る
		PAUSE_MENU_MAX
	};

	CPause(int nPriority = PRIORITY_PAUSE);		//コンストラクタ
	~CPause();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CPause *Create(const D3DXVECTOR3 pos);		//生成
	
	void SetDeath(bool bDeath = true) override;		//死亡フラグ設定

protected:

private:
	CObject2D *m_pItem[PAUSE_MENU_MAX];			//選択項目()
	CObject2D *m_pBg;			//背景()
	PAUSE_MENU m_pauseMenu;		//ポーズメニュー
	int m_nPauseSelect;			//現在の選択肢
	bool m_bEnter;				//決定フラグ
};

#endif