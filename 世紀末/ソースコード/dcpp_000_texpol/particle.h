//===============================================
//
// パーティクル(particle.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _PARTICLE_H_		//このマクロ定義がされてなかったら
#define _PARTICLE_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "object.h"

//=========================
// マクロ定義
//=========================
#define MAX_PARTICLE (256)		//総数

//=========================
// 前方宣言
//=========================
class CEffect;

//=========================
// オブジェクト2Dクラス
//=========================
class CParticle : public CObject
{
public:
	enum TYPE
	{
		TYPE_NONE = 0,	//無し
		TYPE_EXPLODE,	//爆発
		TYPE_MAX,
	};

	CParticle(int nPriority = 5);		//コンストラクタ
	~CParticle();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR3 move, const D3DXVECTOR3 size, TYPE type);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CParticle *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const D3DXVECTOR3 size, TYPE type);		//生成
	static void Release(int nID);							//破棄

	void SetPos(const D3DXVECTOR3 pos);			//位置設定
	void SetRot(const D3DXVECTOR3 rot);			//向き設定
	D3DXVECTOR3 GetPos(void);		//位置取得
	D3DXVECTOR3 GetSize(void);		//2Dサイズ取得
	bool GetJump(void) { return false; }		//ジャンプ判定
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//前回位置取得

protected:

private:
	CEffect *m_apEffect[MAX_PARTICLE];			//アドレス＆情報取得用
	int m_nLife;	//寿命

};

#endif