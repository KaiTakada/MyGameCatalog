//==================================================
//
// アイテム(item.h)
// Author : Kai Takada
//
//==================================================
#ifndef _ITEM_H_								//このマクロ定義がされてなかったら
#define _ITEM_H_								//二重インクルード防止のマクロを定義する

#include "manager.h"
#include "objectBillboard.h"

//マクロ定義
#define ITEM_SIZE D3DXVECTOR3(50.0f,50.0f,50.0f)					//デフォルトサイズ

//============================================
// 前方宣言
//============================================

//============================================
// ブロッククラス
//============================================
class CItem : public CObjectBillboard
{
public:
	//===================================
	// タイプ列挙型定義
	//===================================
	typedef enum
	{
		TYPE_EXP = 0,						//経験値クリスタル
		TYPE_MAX,
	}TYPE;

	CItem(int nPriority = 1);
	~CItem();

	HRESULT Init(void);			//初期化
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);			//終了
	void Update(void);			//更新
	void Draw(void);			//描画

	CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size = ITEM_SIZE, TYPE type = TYPE_EXP);		//生成
	bool CItem::CollisionBall(void);		//当たり判定
	int GetNumAll(void) { return m_nNumAll; }		//総数取得

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];			//テクスチャへのポインタ
	static int m_nNumAll;									//現在使っているブロック数

	CItem::TYPE m_type;					//種類
};

#endif
