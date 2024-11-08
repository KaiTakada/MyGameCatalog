//===============================================
//
// 数字(number.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _NUMBER_H_		//このマクロ定義がされてなかったら
#define _NUMBER_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "obj_polygon2D.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================
//class CObject2D;

//=========================
// プレイヤークラス
//=========================
class CNumber : public CObject2D
{
public:
	CNumber();		//コンストラクタ
	~CNumber();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CNumber* Create(int nValue);		//生成
	static CNumber *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, int nValue);		//生成
	void SetNumber(int nNumber);		//数字設定
	int GetNumber() { return m_nValue; }		//数字取得

protected:

private:
	int m_nValue;		//表示する数字値
};

#endif
