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
#include "object.h"
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
// プレイヤークラス
//=========================
class CEnemy : public CObject
{
public:

	//====================
	// モーションの種類
	//====================
	typedef enum
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
	}MOTIONTYPE;

	//====================
	// キャラのパラメータ
	//====================
	typedef struct
	{
		float fLife;		//体力
		float fLifeMax;		//最大体力
		float fSpeed;		//移動速度
		float fAttack;		//攻撃力
		float fColl;		//当たり判定の半径
	}Param;

	CEnemy(int nPriority = 3);		//コンストラクタ
	~CEnemy();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//初期化(オバロ)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//生成

	bool GetJump(void) { return m_bJump; }						//ジャンプ判定
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//前回位置取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }					//現在位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }					//向き取得
	D3DXVECTOR3 GetMove(void) { return m_move; }				//移動量取得
	D3DXVECTOR3 GetSize(void) { return m_size; }				//サイズ取得
	float GetHeartPos(void) { return m_fHeart; }				//心臓位置取得
	Param GetParam(void) { return m_param; }					//現在パラメータ取得
	void SetJump(const bool bJump) { m_bJump = bJump; }				//ジャンプ判定設定
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//移動量設定
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }				//現在位置設定
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }				//向き設定
	void SetHeartPos(const float fHeart) { m_fHeart = fHeart; }		//心臓位置設定
	void SetParam(const Param param) { m_param = param; }			//現在パラメータ設定
	void SetMember(const my_Identity::eMember member) { m_eMember = member; }		//所属設定
	my_Identity::eMember GetMember() { return m_eMember; }		//所属設定


	void ReadFile(void);	//ファイル読込
	void Damage(float fDamege = 1.0f);

	CMotion *GetMotion(void);					//現在パラメータ取得

protected:

private:
	void RotAdj(float fRotDiff);			//向き修正
	void RotLimit(float *pRot);				//向き修正

	void CollisionField(D3DXVECTOR3 pos);
	void CollisionBall(D3DXVECTOR3 pos);

	void Throw();

	CParts *m_apPart[MAX_PARTS];		//モデル(パーツ)へのポインタ
	int m_nNumModel;							//モデル(パーツ)の総数
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_posOld;		//前回位置
	D3DXVECTOR3 m_pos;			//現在位置
	D3DXVECTOR3 m_size;			//現在サイズ
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//目標の向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	float m_fHeart;			//プレイヤーの中心y軸(心臓座標)
	bool m_bJump;			//ジャンプ判定(falseはジャンプ可能状態)
	CMotion *m_pMotion;		//モーション情報
	Param m_param;			//パラメータ
	CBall *m_pBall;		//武器(ボール)情報
	CState_Life *m_pStateLife;		//状態
	my_Identity::eMember m_eMember;		//所属
	int m_nCatchCtr;		//キャッチカウンター
	int m_nThrowCtr;		//投げカウンター
};

#endif