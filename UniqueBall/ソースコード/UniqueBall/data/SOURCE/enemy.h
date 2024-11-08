//===============================================
//
// 敵(enemy.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _ENEMY_H_		//このマクロ定義がされてなかったら
#define _ENEMY_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "objectcharacter.h"
#include "identity.h"

//=========================
// マクロ定義
//=========================
#define MAX_PARTS (32)		//最大パーツ数
#define MAX_PLAYER_WEAPON (2)		//武器数
#define MAX_PLAYER_SIZE D3DXVECTOR3(30.0f,60.0f,30.0f)		//サイズ

//=========================
// 前方宣言
//=========================
class CParts;
class CMotion;
class CBall;
class CGauge;
class CCursor;
class CState_Life;

//=========================
// 敵クラス
//=========================
class CEnemy : public CObjectCharacter
{
public:

	//====================
	// モーションの種類
	//====================
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,	//待機
		MOTIONTYPE_WALK,	//歩き
		MOTIONTYPE_DASH,	//ダッシュ
		MOTIONTYPE_JUMP,	//ジャンプ
		MOTIONTYPE_JUMP2,	//2段ジャンプ
		MOTIONTYPE_GROUND,	//着地
		MOTIONTYPE_THROW_RDY,	//投げ用構える
		MOTIONTYPE_THROW,	//投げる
		MOTIONTYPE_CATCH_RDY,	//キャッチ用構える
		MOTIONTYPE_CATCH,	//キャッチ
		MOTIONTYPE_TACKLE,	//タックル
		MOTIONTYPE_MAX,
	};

	CEnemy(int nPriority = 3);		//コンストラクタ
	~CEnemy();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//初期化(オバロ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//生成

	void SetDelete(const bool bDelete) { m_bDelete = bDelete; }		//org死亡フラグ設定
	bool GetDelete(void) { return m_bDelete; }						//org死亡フラグ取得

	void SetBall(CBall *pBall) { m_pBall = pBall; }		//ボール設定
	CBall *GetBall(void) { return m_pBall; }			//ボール取得

	void ReadFile(void);	//ファイル読込
	void InitModel();	//モデル初期設定

protected:

private:
	void CollisionBall(D3DXVECTOR3 pos);

	void Throw();

	CBall *m_pBall;		//武器(ボール)情報
	int m_nCatchCtr;		//キャッチカウンター
	int m_nThrowCtr;		//投げカウンター
	bool m_bDelete;		//死んだらSceneで殺す用フラグ[t:死んだ]
};

#endif