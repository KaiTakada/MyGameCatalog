//=================================================
//
// 3D背景(Bg3D.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _BG3D_H_								//このマクロ定義がされてなかったら
#define _BG3D_H_								//二重インクルード防止のマクロを定義する

//============================
// インクルードファイル
//============================
#include "manager.h"
#include "obj_polygon3D.h"

//============================
// マクロ定義
//============================
#define DEF_BG3D_SIZE D3DXVECTOR3(2048.0f,0.0f,2048.0f)				//デフォルトサイズ
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//デフォルト向き

//============================
// 3Dポリゴンクラス
//============================
class CBg3D : public CObject3D
{
public:
	CBg3D(int nPriority = 1);
	~CBg3D();

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBg3D *Create(void);		//生成
	static CBg3D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_BG3D_SIZE);		//生成

private:

};
#endif
