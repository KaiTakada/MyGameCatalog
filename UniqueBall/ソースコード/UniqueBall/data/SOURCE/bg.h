//===============================================
//
// 背景(bg.cpp)
// Author 髙田 佳依
//
//===============================================
#ifndef _BG_H_		//このマクロ定義がされてなかったら
#define _BG_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "obj_polygon2D.h"

//=========================
// マクロ定義
//=========================

//=========================
// オブジェクト2D背景クラス
//=========================
class CBg : public CObject2D
{
public:
	enum TEX
	{
		TEX_000 = 0,
		TEX_001,
		TEX_002,
		TEX_MAX,
	};

	CBg();		//コンストラクタ
	~CBg();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CBg *Create(void);		//生成
	static CBg *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR2 move, const D3DXVECTOR3 size, const TEX TexType);		//生成
	void SetMove(const D3DXVECTOR2 move) { m_texMove = move; }		//移動量設定
	D3DXVECTOR3 GetMove(void) { return D3DXVECTOR3(m_texMove.x, m_texMove.y, 0.0f); }		//移動量取得

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_MAX];				//テクスチャバッファ
	static int m_nIdx[TEX_MAX];				//テクスチャバッファ
	D3DXVECTOR2 m_texMove;				//移動量
};

#endif