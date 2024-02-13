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
#define BLOCK_WIDTH (80.0f)						//ブロックの幅
#define BLOCK_HEIGHT (80.0f)					//ブロックの高さ

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
	typedef enum
	{
		TYPE_NORMAL = 0,		//通常のブロック
		TYPE_MAX,
	}TYPE;

	CBlock(int nPriority = 1);
	~CBlock();

	HRESULT Init(void);			//初期化
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);			//終了
	void Update(void);			//更新
	void Draw(void);			//描画

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), TYPE type = TYPE_NORMAL);		//生成
	bool CollisionRect(void);		//当たり判定
	static int GetNumAll(void) { return m_nNumAll; }		//総数取得
	bool GetJump(void) { return false; }		//ジャンプ判定
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }		//前回位置取得

protected:

private:
	static int m_nNumAll;											//現在使っているブロック数

	D3DXVECTOR3 m_posOld;			//前回の位置
	CBlock::TYPE m_type;			//種類
};

#endif
