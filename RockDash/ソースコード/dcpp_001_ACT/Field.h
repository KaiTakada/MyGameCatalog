//=================================================
//
// 3D地面(Field.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _FIELD_H_								//このマクロ定義がされてなかったら
#define _FIELD_H_								//二重インクルード防止のマクロを定義する

//============================
// インクルードファイル
//============================
#include "manager.h"
#include "object3D.h"

//============================
// マクロ定義
//============================
#define DEF_FIELD_SIZE D3DXVECTOR3(16384.0f,0.0f,512.0f)				//デフォルトサイズ
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//デフォルト向き

//============================
// 3Dポリゴンクラス
//============================
class CField : public CObject3D
{
public:
	CField(int nPriority = 1);
	~CField();

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CField *Create(void);		//生成
	static CField *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, const D3DXVECTOR3 size = DEF_FIELD_SIZE);		//生成

private:

};
#endif
