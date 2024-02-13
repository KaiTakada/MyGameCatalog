//===============================================
//
// リザルト(result.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _RESULT_H_		//このマクロ定義がされてなかったら
#define _RESULT_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"
#include "manager.h"
#include "object.h"

//=========================
// 前方宣言
//=========================
//class CRenderer;
class CTimer;

//=========================
// 画面(シーン)クラス
//=========================
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	virtual HRESULT Init();		//初期化
	virtual void Uninit();		//終了
	virtual void Update();		//更新
	virtual void Draw();		//描画
	static CResult *Create();		//生成

protected:

private:
	CTimer *m_pTimer;		//スコア
};

#endif