//==================================================
//
// ブロック処理(block.h)
// Author : Kai Takada
//
//==================================================
#ifndef _BLOCK_H_								//このマクロ定義がされてなかったら
#define _BLOCK_H_								//二重インクルード防止のマクロを定義する

//============================================
// インクルードファイル
//============================================
#include "objectX.h"

//============================================
//マクロ定義
//============================================
#define MAX_BLOCK (256)						//ブロックの最大数

//============================================
// 前方宣言
//============================================

//============================================
// ブロッククラス
//============================================
class CBlock : public CObjectX
{
public:
	//===================================
	// ブロック列挙型の定義
	//===================================
	enum TYPE_Blk
	{
		TYPE_NORMAL = 0,		//通常のブロック
		TYPE_BREAK,		//壊せるブロック
		TYPE_GOAL,		//ゴールブロック
		TYPE_MAX,
	};

	CBlock(int nPriority = 1);
	~CBlock();

	virtual HRESULT Init(void);			//初期化
	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE_Blk type = TYPE_NORMAL);		//初期化(オバロ)
	virtual void Uninit(void);			//終了
	virtual void Update(void);			//更新
	virtual void Draw(void);			//描画

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), TYPE_Blk type = TYPE_NORMAL);		//生成
	virtual bool CollisionRect(void);		//当たり判定
	static int GetNumAll(void) { return m_nNumAll; }		//総数取得
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }		//前回位置取得
	void SetType_Blk(CBlock::TYPE_Blk type) { m_type = type; }		//種類
	CBlock::TYPE_Blk GetType_Blk() { return m_type; }		//種類
	virtual int GetLocalType() { return (int)GetType_Blk(); }	//種類取得(基底※objectから継承)

protected:

private:
	static int m_nNumAll;											//現在使っているブロック数

	D3DXVECTOR3 m_posOld;			//前回の位置
	CBlock::TYPE_Blk m_type;			//種類
};

#endif
