//===============================================
//
// ポリゴン(object.cpp)
// Author 髙田 佳依
//
//===============================================
#ifndef _OBJECT_H_								//このマクロ定義がされてなかったら
#define _OBJECT_H_								//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"

//=========================
// マクロ定義
//=========================
#define MAX_OBJECT (1024)		//最大数
#define DEF_VEC3 (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//3軸の初期値

//=========================
// オブジェクトクラス
//=========================
class CObject
{
public:

	//========================
	// オブジェクトタイプ
	//========================
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BULLET,
		TYPE_EXPLOSION,
		TYPE_BG,
		TYPE_MULTIBG,
		TYPE_EFFECT,
		TYPE_PARTICLE,
		TYPE_NUMBER,
		TYPE_SCORE,
		TYPE_BLOCK,
		TYPE_ITEM,
		TYPE_3DPOLYGON,
		TYPE_BILLBOARD,
		TYPE_MODEL,
		TYPE_FIELD,
		TYPE_WEAPON,
		TYPE_GAUGE,
		TYPE_PARTS,
		TYPE_EXP,
		TYPE_FADE,
		TYPE_TIMER,
		TYPE_EVO,
		TYPE_ITEM2D,
		TYPE_SPAWN,
		TYPE_BOSS,
		TYPE_ENTER,
		TYPE_BALL,
		TYPE_BALL_GENERATOR,
		TYPE_WALL,
		TYPE_UI,
		TYPE_MESHFIELD,
		TYPE_MAX
	}TYPE;

	//========================
	// 優先順位
	//========================
	typedef enum
	{
		PRIORITY_BG = 0,
		PRIORITY_MAP,
		PRIORITY_OBJ,
		PRIORITY_MOB,
		PRIORITY_MAIN,
		PRIORITY_UI,
		PRIORITY_PAUSE,
		PRIORITY_FADE,
		PRIORITY_MAX,
	}PRIORITY;

	//CObject();				//コンストラクタ(デフォ)
	CObject(int nPriority = 3);				//コンストラクタ(描画順オバロ)
	virtual ~CObject();		//デストラクタ

	virtual HRESULT Init(void) = 0;							//初期化(デフォ)
	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//初期化(オバロ)
	virtual void Uninit(void) = 0;							//終了
	virtual void Update(void) = 0;							//更新
	virtual void Draw(void) = 0;							//描画
	
	virtual void SetPos(const D3DXVECTOR3 pos) { pos; }			//位置設定
	virtual void SetRot(const D3DXVECTOR3 rot) { rot; }			//向き設定
	virtual void SetJump(const bool bJump) { bJump; }		//ジャンプ判定設定
	virtual void SetBoost(const bool bBoost) { bBoost; }		//ブースト判定設定
	virtual void SetMove(const D3DXVECTOR3 move) { move; }		//移動量設定
	virtual D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//現在位置取得
	virtual D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//向き取得
	virtual D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//前回位置取得
	virtual D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//2Dサイズ取得
	virtual bool GetJump(void) { return FALSE; }		//ジャンプ判定取得
	virtual bool GetBoost(void) { return FALSE; }		//ブースト判定取得
	virtual D3DXVECTOR3 GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//移動量取得
	virtual void GetPolyPos(D3DXVECTOR3 Vtx0, D3DXVECTOR3 Vtx1, D3DXVECTOR3 Vtx2, D3DXVECTOR3 Vtx3) { Vtx0, Vtx1, Vtx2, Vtx3; }		//移動量取得
	virtual D3DXVECTOR3 GetMinVtx(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//最小座標取得
	virtual D3DXVECTOR3 GetMaxVtx(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//最大座標取得

	static void ReleaseAll(void);		//全ての破棄
	static void UpdateAll(void);		//全ての更新
	static void DrawAll(void);			//全ての描画
	static int GetNumAll(void) { return m_nNumAll; }		//総数取得

	void SetType(CObject::TYPE type) { m_type = type; }		//タイプ取得
	CObject::TYPE GetType() { return m_type; }		//タイプ取得
	virtual int GetLocalType() { assert(false); return 0; }			//それぞれのクラスで作ったタイプを取得(使用時は継承宣言をしてください)

	virtual void SetDeath(bool bDeath = true) { m_bDeath = bDeath; }		//死亡フラグ取得
	bool GetDeath(void) { return m_bDeath; }				//死亡フラグ取得

	static CObject *GetTop(int nPrty) { return m_apTop[nPrty]; }		//オブジェクト
	static CObject *GetEnd(int nPrty) { return m_apEnd[nPrty]; }		//優先順でのオブジェクト
	void SetNext(CObject *pNext) { m_pNext = pNext; }		//前アドレス設定
	CObject *GetNext(void) { return m_pNext; }				//前アドレス取得
	void SetPrev(CObject *pPrev) { m_pPrev = pPrev; }		//後アドレス設定
	CObject *GetPrev(void) { return m_pPrev; }				//後アドレス取得

protected:
	void Release(void);		//自身の破棄

private:
	static int m_nNumAll;		//オブジェクト総数
	static CObject *m_apTop[PRIORITY_MAX];		//先頭オブジェクト
	static CObject *m_apEnd[PRIORITY_MAX];		//最後尾オブジェクト
	CObject *m_pNext;				//次オブジェクト
	CObject *m_pPrev;				//前オブジェクト
	CObject::TYPE m_type;		//タイプ
	int m_nPriority;	//描画順
	bool m_bDeath;		//死亡フラグ
};

#endif