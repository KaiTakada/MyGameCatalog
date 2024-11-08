//===============================================
//
// Xモデルオブジェクト(objectX.cpp)
// Author: 髙田 佳依
//
//===============================================
#ifndef _OBJECTX_H_								//このマクロ定義がされてなかったら
#define _OBJECTX_H_								//二重インクルード防止のマクロを定義する

//================================
// インクルードファイル
//================================
#include "object.h"
#include "Xmodel.h"

//================================
// マクロ定義
//================================
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//デフォルトサイズ

//================================
// 前方宣言
//================================

//================================
// モデルクラス
//================================
class CObjectX : public CObject
{
public:

	CObjectX(int nPriority = 1);
	~CObjectX();

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Draw(const D3DMATERIAL9 mat);

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }			//位置設定
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }			//向き設定
	void SetSize(const D3DXVECTOR3 size);						//サイズ設定
	void SetVtx(const D3DXVECTOR3 vtxMin, const D3DXVECTOR3 vtxMax) { m_vtxMin = vtxMin; m_vtxMax = vtxMax; }			//向き設定
	void SetCol(const D3DXCOLOR col) { m_col = col; }		//色設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }		//現在位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }		//向き取得
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }		//前回位置取得
	D3DXVECTOR3 GetSize(void) { return m_size; }		//2Dサイズ取得
	D3DXVECTOR3 GetMinVtx(void) { return m_vtxMin; }			//最小座標取得
	D3DXVECTOR3 GetMaxVtx(void) { return m_vtxMax; }			//最大座標取得
	D3DXCOLOR GetCol(void) { return m_col; }			//色取得
	int GetIdxModel(void) { return m_nIdxModel; }				//モデル番号取得
	static CObjectX *Create(void);		//生成
	static CObjectX *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, float fSize = 1.0f);		//生成
	void SetIdxModel(int nIdx) { m_nIdxModel = nIdx; }		//モデル番号の割り当て

protected:

private:
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_posOld;				//位置
	D3DXVECTOR3 m_rot;					//移動量
	D3DXVECTOR3 m_size;					//サイズ
	D3DXVECTOR3 m_vtxMin;				//矩形の最小座標
	D3DXVECTOR3 m_vtxMax;				//矩形の最大座標
	D3DXCOLOR m_col;					//色
	int m_nIdxModel;					//モデル番号
};

#endif
