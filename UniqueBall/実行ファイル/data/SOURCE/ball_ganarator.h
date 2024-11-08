//==================================================
//
// ブロック処理(block.h)
// Author : Kai Takada
//
//==================================================
#ifndef _BALL_GENERATOR_H_								//このマクロ定義がされてなかったら
#define _BALL_GENERATOR_H_								//二重インクルード防止のマクロを定義する

//============================================
// インクルードファイル
//============================================
#include "objectX.h"

//============================================
// 前方宣言
//============================================
class CBall;

//============================================
// ボール生成器クラス
//============================================
class CBallGenerator : public CObjectX
{
public:
	CBallGenerator(int nPriority = 1);
	~CBallGenerator();

	virtual HRESULT Init(void);			//初期化
	virtual HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	virtual void Uninit(void);			//終了
	virtual void Update(void);			//更新
	virtual void Draw(void);			//描画

	static CBallGenerator *Create(D3DXVECTOR3 pos = mylib_const::DEFVEC3);		//生成
	//壁&ブロックとの当たり判定
protected:

private:
	CBall *m_pBall;		//生成したボールを保持
	int m_nGenerateCtr;		//ボール生成カウンタ
};

#endif
