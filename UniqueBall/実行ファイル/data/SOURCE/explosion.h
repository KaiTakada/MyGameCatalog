//===============================================
//
// 爆発(explosion.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _EXPLOSION_H_		//このマクロ定義がされてなかったら
#define _EXPLOSION_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "objectBillboard.h"

//=========================
// マクロ定義
//=========================
#define DEF_EXPLOSION_SIZE D3DXVECTOR3(100.0f, 100.0f, 20.0f)		//デフォルトサイズ
#define DEF_EXPLOSION_FRAME (8)		//デフォルトフレーム数

//=========================
// 前方宣言
//=========================
//class CObject2D;

//=========================
// プレイヤークラス
//=========================
class CExplosion : public CObjectBillboardAnim
{
public:
	CExplosion(int nPriority = 4);		//コンストラクタ
	~CExplosion();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CExplosion *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size = DEF_EXPLOSION_SIZE, const int nFrame = DEF_EXPLOSION_FRAME, const int nLoop = 1, const int nNumPtnX = 8, const int nNumPtnY = 1);		//生成
	static int GetNumAll(void) { return m_nNumAll; }		//総数取得

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;				//共通テクスチャバッファ
	static int m_nNumAll;		//総数
};

#endif
