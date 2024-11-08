//=================================================
//
// 3Dポリゴン(object3D.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _OBJECT3D_H_								//このマクロ定義がされてなかったら
#define _OBJECT3D_H_								//二重インクルード防止のマクロを定義する

//============================
// インクルードファイル
//============================
#include "manager.h"
#include "object.h"

//============================
// マクロ定義
//============================
#define DEF_SIZE D3DXVECTOR3(100.0f,0.0f,100.0f)				//デフォルトサイズ
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//デフォルトサイズ

//============================
// 前方宣言
//============================
class CAnim;

//============================
// 3Dポリゴンクラス
//============================
class CObject3D : public CObject
{
public:
	CObject3D(int nPriority = 1);
	~CObject3D();

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject3D *Create(void);		//生成
	static CObject3D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_SIZE);		//生成

	void SetPos(const D3DXVECTOR3 pos);				//位置設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }		//現在位置取得
	void SetRot(const D3DXVECTOR3 rot);				//向き設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }		//向き取得
	virtual void SetSize(const D3DXVECTOR3 size);		//ポリゴンサイズ設定
	D3DXVECTOR3 GetSize(void) { return m_size; }		//ポリゴンサイズ取得
	void SetColor(const D3DXCOLOR col);				//テクスチャ色設定
	D3DXCOLOR GetColor();							//テクスチャ色取得
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }		//テクスチャ番号の割り当て
	int GetIdxTexture() { return m_nIdxTexture; }				//テクスチャ番号の取得
	void SetMtxWorld(D3DXMATRIX mtx) { m_mtxWorld = mtx; }		//ワールドマトリックス設定
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }			//ワールドマトリックス取得

	void SetSizeValue(const D3DXVECTOR3 size) { m_size = size; }			//サイズ値設定
	void SetNormal(const D3DXVECTOR3 nor);			//法線設定
	void SetDivTex(int nDiv0, int nDiv1);											//テクスチャの分割数設定
	void SetTexPos(const D3DXVECTOR2 rightDown, const D3DXVECTOR2 leftUp);			//テクスチャ座標設定
	void GetPolyPos(D3DXVECTOR3* Vtx0, D3DXVECTOR3* Vtx1, D3DXVECTOR3* Vtx2, D3DXVECTOR3* Vtx3);	//4点取得
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }		//バッファ取得

	float GetHeight(D3DXVECTOR3 posObj);				//乗っているオブジェクトの位置y取得

	void SetZtest(const bool bZtest) { m_bZtest = bZtest; }			//Zテスト設定
	void SetAtest(const bool bAtest) { m_bAtest = bAtest; }			//αテスト設定
	void SetAbrend(const bool bAbrend) { m_bAbrend = bAbrend; }		//αブレンド設定
	void SetLight(const bool bLight) { m_bLight = bLight; }			//ライティング設定
	bool GetZtest(void) { return m_bZtest; }		//Zテスト取得
	bool GetAtest(void) { return m_bAtest; }		//αテスト取得
	bool GetAbrend(void) { return m_bAbrend; }		//αブレンド取得
	bool GetLight(void) { return m_bLight; }		//ライティング取得

protected:
	void DrawRenderSet(LPDIRECT3DDEVICE9 pDevice);
	void DrawRenderReset(LPDIRECT3DDEVICE9 pDevice);

private:
	D3DXVECTOR3 vec(D3DXVECTOR3 vtxpos0, D3DXVECTOR3 vtxpos1, D3DXVECTOR3 vtxpos2);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_size;					//サイズ
	D3DXVECTOR3 m_rot;					//移動量
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス
	int m_nIdxTexture;		//テクスチャのインデックス
	bool m_bZtest;			//Zテスト[t:on,f:off]
	bool m_bAtest;			//αテスト[t:on,f:off]
	bool m_bAbrend;			//αブレンド[t:on,f:off]
	bool m_bLight;			//ライティング[t:on,f:off]
};

//=========================
// オブジェクト2Dアニメーションクラス
//=========================
class CObject3DAnim : public CObject3D
{
public:
	CObject3DAnim(int nPriority = 3);		//コンストラクタ
	~CObject3DAnim();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	CAnim *GetAnim(void) { return m_anim; }		//アニメ情報取得
	static CObject3DAnim* Create(void);		//生成
	static CObject3DAnim* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_SIZE);		//生成
	static int GetNumAll() { return s_nNumAll; }		//生成

protected:

private:
	CAnim *m_anim;		//アニメーション要素
	static int s_nNumAll;
};

#endif
