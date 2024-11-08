//===============================================
//
// 2Dポリゴン(object2D.cpp)
// Author 髙田 佳依
//
//===============================================
#ifndef _OBJECT2D_H_		//このマクロ定義がされてなかったら
#define _OBJECT2D_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "object.h"

//=========================
// マクロ定義
//=========================
#define DEF_SIZE_STATIC D3DXVECTOR2(160.0f,90.0f)				//デフォルトサイズ

//=========================
// 前方宣言
//=========================
class CAnim;

//=========================
// オブジェクト2Dクラス
//=========================
class CObject2D : public CObject
{
public:

	//=========================
	// タイプ列挙型
	//=========================
	enum UPDATE_TYPE
	{
		UPDATE_TYPE_NONE = 0,
		UPDATE_TYPE_ANIM,
		UPDATE_TYPE_MAX
	};

	CObject2D(int nPriority = CObject::PRIORITY_UI);		//コンストラクタ
	~CObject2D();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	D3DXVECTOR3 GetPos(void) { return m_pos; }		//位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }		//向き取得
	D3DXVECTOR3 GetSize(void) { return m_size; }		//サイズ取得
	float GetLength(void) { return m_fLength; }		//対角線の長さ取得
	float GetAngle(void) { return m_fAngle; }		//対角線の角度取得
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }		//バッファ取得
	D3DXVECTOR2 GetTexPosleftUp(void);			//テクスチャ座標取得(左上)
	D3DXVECTOR2 GetTexPosrightDown(void);			//テクスチャ座標取得(右下)
	D3DXVECTOR2 GetTexPosDif(void);			//テクスチャ座標取得(右下 - 左下)
	D3DXCOLOR CObject2D::GetTexCol(void);		//頂点カラー取得

	void SetPos(const D3DXVECTOR3 pos);			//位置設定
	void SetRot(const D3DXVECTOR3 rot);			//向き設定
	void SetSize(const D3DXVECTOR3 size);			//サイズ設定
	void SetTexPos(const D3DXVECTOR2 rightDown, const D3DXVECTOR2 leftUp);			//テクスチャ座標設定()
	void SetTexCol(D3DXCOLOR col);			//頂点カラー設定
	void SetAbrend(const bool bAbrend) { m_bAbrend = bAbrend; }			//αブレンド設定
	static CObject2D *Create(void);		//生成
	static CObject2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const UPDATE_TYPE UpdType);		//生成
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }		//テクスチャ番号の割り当て
	void SetAppear(bool set) { m_bAppear = set; }		//表示設定
	bool GetAppear() { return m_bAppear; }		//表示取得

	//付き合いで...
	bool GetAbrend(void) { return m_bAbrend; }		//αブレンド取得
	bool GetJump(void) { return false; }		//ジャンプ判定
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//前回位置取得

	//void SetTexCol(D3DXCOLOR col);			//頂点カラー

protected:

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファ

	D3DXVECTOR3 m_pos;		//座標
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_size;		//サイズ
	float m_fLength;		//対角線の長さ
	float m_fAngle;		//対角線の角度
	int m_nIdxTexture;		//テクスチャのインデックス
	bool m_bAbrend;			//αブレンドの是非
	bool m_bAppear;			//描画するかどうか
};

//=========================
// オブジェクト2Dアニメーションクラス
//=========================
class CObject2DAnim : public CObject2D
{
public:
	CObject2DAnim(int nPriority = 3);		//コンストラクタ
	~CObject2DAnim();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	CAnim *GetAnim(void) { return m_anim; }		//アニメ情報取得

protected:

private:
	CAnim *m_anim;		//アニメーション要素
};

#endif