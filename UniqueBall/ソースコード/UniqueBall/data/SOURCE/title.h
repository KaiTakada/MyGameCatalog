//===============================================
//
// タイトル(title.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _TITLE_H_		//このマクロ定義がされてなかったら
#define _TITLE_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"
#include "scene.h"

//=========================
// 前方宣言
//=========================
class CPress_Enter;

//=========================
// 画面(シーン)クラス
//=========================
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	virtual HRESULT Init();		//初期化
	virtual void Uninit();		//終了
	virtual void Update();		//更新
	virtual void Draw();		//描画
	static CTitle *Create();		//生成

protected:

private:
	CField *m_pField;							//地面

	CPress_Enter *m_pEnter;	//press enter
	int m_nAlphaCnter;						//PRESS ENTERの点滅間隔の判定
	int m_nTimeInterval;						//PRESS ENTERの点滅間隔
	float m_fAlphaColor;					//頂点カラー
	int m_nCntRanking;						//ランキング遷移
};

#endif