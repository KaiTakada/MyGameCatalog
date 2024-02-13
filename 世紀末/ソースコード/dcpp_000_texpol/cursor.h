//===============================================
//
// 背景(bg.cpp)
// Author 髙田 佳依
//
//===============================================
#ifndef _CURSOR_H_		//このマクロ定義がされてなかったら
#define _CURSOR_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "object2D.h"

//=========================
// マクロ定義
//=========================
#define DEF_SIZE_CURSOR D3DXVECTOR3(40.0f, 40.0f, 0.0f)		//デフォルトサイズ

//=========================
// オブジェクト2D背景クラス
//=========================
class CCursor : public CObject2D
{
public:
	enum TEX
	{
		TEX_AIM = 0,
		TEX_001,
		TEX_002,
		TEX_MAX,
	};

	CCursor();		//コンストラクタ
	~CCursor();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CCursor *Create(void);		//生成
	static CCursor *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size = DEF_SIZE_CURSOR, const TEX TexType = TEX_AIM);		//生成
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }		//移動量設定
	D3DXVECTOR3 GetMove() { return m_move; }		//移動量取得
	D3DXVECTOR3 GetVec() { return m_vec; }		//移動量取得
	D3DXVECTOR3 GetRot();		//方向取得

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TEX_MAX];				//テクスチャバッファ
	static int m_nIdx[TEX_MAX];				//テクスチャバッファ
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_Defpos;				//初期位置
	D3DXVECTOR3 m_vec;				//初期位置 - 現在位置のベクトル
	bool m_bMouse;		//カーソルのマウス操作フラグ
};

#endif