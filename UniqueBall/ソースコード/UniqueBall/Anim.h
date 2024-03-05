//===============================================
//
// 2Dポリゴン(object2D.cpp)
// Author 髙田 佳依
//
//===============================================
#ifndef _ANIM_H_		//このマクロ定義がされてなかったら
#define _ANIM_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"

//=========================
// マクロ定義
//=========================
#define MAX_VTX (4)		//頂点数
#define DEF_SIZE_ANIM D3DXVECTOR2(125.0f,200.0f)				//デフォルトサイズ

//=========================
// オブジェクト2Dクラス
//=========================
class CAnim
{
public:
	CAnim();		//コンストラクタ
	~CAnim();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	void Uninit(void);							//終了
	void Update(void);							//更新

	void SetNumPtnX(const int nNumPtn) { m_nNumPtnAnimX = nNumPtn; }			//パターン数設定x
	void SetNumPtnY(const int nNumPtn) { m_nNumPtnAnimY = nNumPtn; }			//パターン数設定y
	void SetNumLoop(const int nNumLoop) { m_nLoop = nNumLoop; }			//パターン数設定
	void SetNumFrame(const int nFrame) { m_nFrame = nFrame; }			//フレーム数設定
	void SetTexpos(const D3DXVECTOR2 *pTexpos);					//テクスチャ座標設定
	int GetNumPtnX(void) { return m_nNumPtnAnimX; }			//パターン数取得x
	int GetNumPtnY(void) { return m_nNumPtnAnimY; }			//パターン数取得y
	int GetNumLoop(void) { return m_nLoop; }			//パターン数取得
	int GetNumFrame(void) { return m_nFrame; }			//フレーム数取得
	D3DXVECTOR2 *GetTexpos(void) { return &m_aTexpos[0]; }		//テクスチャ座標取得
	bool IsEnd() { return m_bEnd; }			//終了判定取得
	
protected:

private:
	int m_nCtrAnim;							//アニメーションカウンター
	int m_nPtnAnimX;						//アニメーションパターンx No.
	int m_nPtnAnimY;						//アニメーションパターンy No.
	int m_nNumPtnAnimX;						//アニメーションパターン数x
	int m_nNumPtnAnimY;						//アニメーションパターン数y
	int m_nFrame;		//更新頻度フレーム
	int m_nLoop;		//ループ数(-1で無限)
	bool m_bEnd;			//終了フラグ

	D3DXVECTOR2 m_aTexpos[MAX_VTX];
};

#endif