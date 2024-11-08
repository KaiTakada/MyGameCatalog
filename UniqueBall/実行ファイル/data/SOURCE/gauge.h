//===============================================
//
// ゲージ(gauge.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _GAUGE_H_		//このマクロ定義がされてなかったら
#define _GAUGE_H_		//二重インクルード防止のマクロを定義する

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
class CObjectBillboard;

//=========================
// プレイヤークラス
//=========================
class CGauge : public CObject
{
public:

	//左右・上下
	//左・中央・右寄せ

	CGauge(int nPriority = 5);		//コンストラクタ
	~CGauge();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CGauge *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const float fRat = 1.0f);		//生成
	void SetRatio(float fRat);			//割合設定
	void SetPos(const D3DXVECTOR3 pos);			//座標設定
	void SetSizeMax(D3DXVECTOR3 sizeMax) { m_sizeMax = sizeMax; }						//割合設定
	void SetCol(D3DXCOLOR col);						//色設定
	float GetRatio() { return m_fRat; }						//割合取得
	D3DXVECTOR3 GetSizeMax() { return m_sizeMax; }						//割合取得

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;				//共通テクスチャバッファ
	CObjectBillboard *m_pBarBB;			//ビルボード(バー)
	CObjectBillboard *m_pFrameBB;		//ビルボード(枠)
	D3DXVECTOR3 m_size;		//サイズ
	D3DXVECTOR3 m_sizeMax;		//maxサイズ
	float m_fRat;			//割合
};

#endif
