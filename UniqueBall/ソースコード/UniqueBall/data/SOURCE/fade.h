//===============================================
//
// ゲーム(game.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _FADE_H_		//このマクロ定義がされてなかったら
#define _FADE_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "obj_polygon2D.h"

//=========================
// 前方宣言
//=========================
//class CRenderer;
//class CObject2D;

//=========================
// 画面(シーン)クラス
//=========================
class CFade : public CObject2D
{
public:

	//フェードの状態
	typedef enum
	{
		STATE_NONE = 0,		//何もしてない状態
		STATE_IN,			//フェードイン状態
		STATE_OUT,			//フェードアウト状態
		STATE_MAX
	}STATE;

	CFade(int nPriority = PRIORITY_FADE);
	~CFade();

	HRESULT Init(CScene::MODE modeNext);		//初期化
	virtual void Uninit();		//終了
	virtual void Update();		//更新
	virtual void Draw();		//描画
	static CFade *Create(CScene::MODE modeNext);		//生成

	bool SetState(CScene::MODE modeNext);
	STATE GetState(void) { return m_state; }	//フェード状態取得

protected:

private:
	STATE m_state;												//フェードの状態
	CScene::MODE m_modeNext;									//次の画面(モード)
	D3DXCOLOR m_colorFade;										//ポリゴン(フェード)の色
	bool m_bExc;												//実行中フラグ
};

#endif
