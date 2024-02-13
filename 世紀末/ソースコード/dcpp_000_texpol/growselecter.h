//===============================================
//
// 成長シーン(growselecter.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _GROWSELECTER_H_
#define _GROWSELECTER_H_

//=========================
// インクルードファイル
//=========================
#include "object.h"

//=========================
// マクロ定義
//=========================
#define MAX_ITEM (3)		//アイテム数

//=========================
// 前方宣言
//=========================
class CItem2D;
class CObject2D;

//=========================
// プレイヤークラス
//=========================
class CGrowSelecter : public CObject
{
public:
	CGrowSelecter(int nPriority = PRIORITY_UI);		//コンストラクタ
	~CGrowSelecter();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CGrowSelecter *Create();		//生成

protected:

private:
	CItem2D *m_pItem[MAX_ITEM];			//選択項目()
	CObject2D *m_pBg;			//背景()
	int m_nPauseSelect;			//現在の選択肢
	bool m_bEnter;				//決定フラグ
};

#endif