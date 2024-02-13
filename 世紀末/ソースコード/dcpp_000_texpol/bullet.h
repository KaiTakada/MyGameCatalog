//===============================================
//
// 弾(bullet.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _BULLET_H_		//このマクロ定義がされてなかったら
#define _BULLET_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "objectBillboard.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================
//class CObject2D;

//=========================
// プレイヤークラス
//=========================
class CBullet : public CObjectBillboard
{
public:

	//所属側
	enum SIDE
	{
		SIDE_NONE = 0,
		SIDE_ENEMY,
		SIDE_PLAYER,
		SIDE_MAX
	};

	CBullet(int nPriority = 2);		//コンストラクタ
	~CBullet();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CBullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const SIDE side, const int nDamage,
		const D3DXVECTOR3 size = D3DXVECTOR3(9.0f, 9.0f, 9.0f), const float fSpeed = 15.0f,const int nLife = 100);		//生成
	static int GetNumAll(void) { return m_nNumAll; }		//総数取得
	bool CollisionCircle(D3DXVECTOR3 pos);		//敵との当たり判定
	bool CollisionRect(void);

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;				//共通テクスチャバッファ
	static int m_nNumAll;		//総数
	int m_nLife;		//体力
	int m_nEffCtr;		//エフェクトカウンター
	int m_nDamage;		//攻撃力
	SIDE m_side;		//所属
};

#endif
