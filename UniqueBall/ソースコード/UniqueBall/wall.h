//=================================================
//
// 3D壁(wall.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _WALL_H_								//このマクロ定義がされてなかったら
#define _WALL_H_								//二重インクルード防止のマクロを定義する

//============================
// インクルードファイル
//============================
#include "manager.h"
#include "object3D.h"
#include "game.h"

//============================
// マクロ定義
//============================
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//デフォルト向き

//============================
// 3Dポリゴンクラス
//============================
class CWall : public CObject3D
{
public:
	CWall(int nPriority = 1);
	~CWall();

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CWall *Create(void);		//生成
	static CWall *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, const D3DXVECTOR3 size = mylib_const::DEF_FIELD_SIZE);		//生成

	bool CollisionChara(D3DXVECTOR3 posObj, D3DXVECTOR3 posOldObj, D3DXVECTOR3 *posCross);		//キャラクターが壁にめり込まないよう補正する

private:

};
#endif
