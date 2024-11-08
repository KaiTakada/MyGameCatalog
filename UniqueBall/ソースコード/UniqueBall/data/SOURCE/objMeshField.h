//=================================================
//
// メッシュ地面(objMeshField.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _OBJMESHFIELD_H_								//このマクロ定義がされてなかったら
#define _OBJMESHFIELD_H_								//二重インクルード防止のマクロを定義する

//============================
// インクルードファイル
//============================
#include "manager.h"
#include "object.h"

//============================
// 3Dポリゴンクラス
//============================
class CObjMeshField : public CObject
{
public:

	//===================================================
	// カリングの列挙型宣言
	//===================================================
	enum
	{
		CULLMODE_NONE = 0,	//カリング無し
		CULLMODE_CW,		//時計回り
		CULLMODE_CCW,		//反時計回り
		CULLMODE_MAX,
	}CULLMODE;

	CObjMeshField(int nPriority = 1);
	~CObjMeshField();

	HRESULT Init(void);
	HRESULT Init(const int nNumCutU, const int nNumCutV);		//初期化(オバロ)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObjMeshField*Create(void);		//生成
	static CObjMeshField*Create(const int nNumCutU, const int nNumCutV);		//生成

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }		//位置設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }				//位置取得
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }		//向き設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }				//向き取得
	void SetSize2D(const D3DXVECTOR2 size) { m_size = size; }		//ポリゴンサイズ設定
	D3DXVECTOR2 GetSize2D(void) { return m_size; }					//ポリゴンサイズ取得
	void SetColor(const D3DXCOLOR col) { m_col = col; }		//テクスチャ色設定
	D3DXCOLOR GetColor() { return m_col; }					//テクスチャ色取得
	void SetMtxWorld(D3DXMATRIX mtx) { m_mtxWorld = mtx; }		//ワールドマトリックス設定
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }			//ワールドマトリックス取得

	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }		//テクスチャ番号の割り当て
	int GetIdxTexture() { return m_nIdxTexture; }				//テクスチャ番号の取得
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void) { return m_pIdxBuff; }		//インデックスバッファ取得
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }		//頂点バッファ取得

	void GetPolyPos(D3DXVECTOR3* Vtx0, D3DXVECTOR3* Vtx1, D3DXVECTOR3* Vtx2, D3DXVECTOR3* Vtx3);	//4点取得
	float GetHeight(D3DXVECTOR3 posObj);				//乗っているオブジェクトの位置y取得

	void SetZtest(const bool bZtest) { m_bZtest = bZtest; }			//Zテスト設定
	void SetAtest(const bool bAtest) { m_bAtest = bAtest; }			//αテスト設定
	void SetAbrend(const bool bAbrend) { m_bAbrend = bAbrend; }		//αブレンド設定
	void SetLight(const bool bLight) { m_bLight = bLight; }			//ライティング設定
	bool GetZtest(void) { return m_bZtest; }		//Zテスト取得
	bool GetAtest(void) { return m_bAtest; }		//αテスト取得
	bool GetAbrend(void) { return m_bAbrend; }		//αブレンド取得
	bool GetLight(void) { return m_bLight; }		//ライティング取得
	void DrawRenderSet(LPDIRECT3DDEVICE9 pDevice);
	void DrawRenderReset(LPDIRECT3DDEVICE9 pDevice);

private:
	void UpdateVtx();	//頂点情報の更新

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//移動量
	D3DXVECTOR2 m_size;		//サイズ・長さ
	D3DXCOLOR m_col;		//色
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	int m_nNumCutV;		//分割数z
	int m_nNumCutU;		//分割数x
	int m_nNumVtxV;		//頂点数z
	int m_nNumVtxU;		//頂点数x
	int m_nNumVtxAll;		//全ての頂点数
	int m_nNumIdx;		//インデックス数
	int m_nNumPrim;		//プリミティブ数
	bool m_bWireFrame;	//ワイヤーフレーム
	int m_nCull;	//カリングモード
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//インデックスバッファへのポインタ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	int m_nIdxTexture;		//テクスチャのインデックス
	bool m_bZtest;			//Zテスト[t:on,f:off]
	bool m_bAtest;			//αテスト[t:on,f:off]
	bool m_bAbrend;			//αブレンド[t:on,f:off]
	bool m_bLight;			//ライティング[t:on,f:off]
};
#endif
