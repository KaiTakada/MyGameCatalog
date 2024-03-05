//=================================================
//
// ビルボード(ObjectBillboard.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _OBJECTBILLBOARD_H_								//このマクロ定義がされてなかったら
#define _OBJECTBILLBOARD_H_								//二重インクルード防止のマクロを定義する

//===============================
// インクルードファイル
//===============================
#include "object.h"

//============================
// マクロ定義
//============================
#define DEF_BBSIZE D3DXVECTOR3(20.0f,0.0f,20.0f)				//デフォルトサイズ
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//デフォルトサイズ

//===============================
// 前方宣言
//===============================
class CAnim;

//===============================
// ビルボードクラス
//===============================
class CObjectBillboard : public CObject
{
public:
	CObjectBillboard(int nPriority = 3);
	~CObjectBillboard();
	
	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CObjectBillboard *Create(void);		//生成
	static CObjectBillboard *Create(const D3DXVECTOR3 pos = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_BBSIZE, bool bZtest = false);		//生成
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }		//テクスチャ番号の割り当て

	void SetPos(const D3DXVECTOR3 pos);			//位置設定
	void SetRot(const D3DXVECTOR3 rot);			//位置設定
	void SetSize(const D3DXVECTOR3 size);			//向き設定
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//移動量設定
	void SetTexPos(const D3DXVECTOR2 rightDown, const D3DXVECTOR2 leftUp);			//テクスチャ座標設定()
	void SetColor(const D3DXCOLOR col);			//テクスチャ色設定()
	void SetZtest(const bool bZtest) { m_bZtest = bZtest; }			//Zテスト設定
	void SetAtest(const bool bAtest) { m_bAtest = bAtest; }			//αテスト設定
	void SetAbrend(const bool bAbrend) { m_bAbrend = bAbrend; }			//αブレンド設定
	void SetLight(const bool bLight) { m_bLight = bLight; }			//ライティング設定
	
	D3DXVECTOR3 GetPos(void) { return m_pos; }		//現在位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }		//現在位置取得
	D3DXVECTOR3 GetSize(void) { return m_size; }		//2Dサイズ取得
	D3DXVECTOR3 GetMove(void) { return m_move; }		//2Dサイズ取得
	D3DXCOLOR GetColor();			//テクスチャ色設定()
	bool GetZtest(void) { return m_bZtest; }		//Zテスト取得
	bool GetAtest(void) { return m_bAtest; }		//αテスト取得
	bool GetAbrend(void) { return m_bAbrend; }		//αブレンド取得
	bool GetLight(void) { return m_bLight; }		//ライティング取得
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }		//バッファ取得

protected:

private:
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファへのポインタ
	D3DXVECTOR3 m_move;							//移動量
	D3DXVECTOR3 m_pos;							//位置
	D3DXVECTOR3 m_rot;							//向き
	D3DXVECTOR3 m_size;							//大きさ
	int m_nIdxTexture;		//テクスチャのインデックス
	bool m_bZtest;			//Zテスト[t:on,f:off]
	bool m_bAtest;			//αテスト[t:on,f:off]
	bool m_bAbrend;			//αブレンド[t:on,f:off]
	bool m_bLight;			//ライティング[t:on,f:off]
};

//=========================
// オブジェクト2Dアニメーションクラス
//=========================
class CObjectBillboardAnim : public CObjectBillboard
{
public:
	CObjectBillboardAnim(int nPriority = 3);		//コンストラクタ
	~CObjectBillboardAnim();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CObjectBillboardAnim *Create(const D3DXVECTOR3 pos = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_BBSIZE);		//生成
	CAnim *GetAnim(void) { return m_anim; }		//アニメ情報取得
	void SetAnim(CAnim *anim) { m_anim = anim; }		//アニメ情報設定

protected:

private:
	CAnim *m_anim;		//アニメーション要素
};

#endif