//===============================================
//
// 移動2D(move2D.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _MOVE2D_H_		//このマクロ定義がされてなかったら
#define _MOVE2D_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "obj_polygon2D.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================

//=========================
// オブジェクト2Dクラス
//=========================
class CMove2D : public CObject2D
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

	CMove2D(int nPriority = CObject::PRIORITY_UI);		//コンストラクタ
	~CMove2D();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CMove2D *Create(void);		//生成
	static CMove2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posDest,const float fSpeed);		//生成

	void SetPosDest(const D3DXVECTOR3 posDest) { m_posDest = posDest; }			//目標座標設定
	D3DXVECTOR3 GetPosDest(void) { return m_posDest; }							//目標座標取得
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }			//移動量設定
	float GetSpeed(void) { return m_fSpeed; }					//移動量取得

protected:

private:
	D3DXVECTOR3 m_posDest;		//目標座標
	float m_fSpeed;		//移動スピード
};

#endif