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
#include "obj_polygon3D.h"

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
class CObjectBillboard : public CObject3D
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

	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//移動量設定
	D3DXVECTOR3 GetMove(void) { return m_move; }		//2Dサイズ取得
	void SetSize(const D3DXVECTOR3 size) override;			//サイズ設定

protected:

private:
	LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャへのポインタ
	D3DXVECTOR3 m_move;							//移動量
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