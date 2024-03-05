//==================================================
//
// ブースター(wp_ball.h)
// Author : Kai Takada
//
//==================================================
#ifndef _WP_BALL_H_								//このマクロ定義がされてなかったら
#define _WP_BALL_H_								//二重インクルード防止のマクロを定義する

#include "weapon.h"
#include "identity.h"

//============================================
// マクロ定義
//============================================

//============================================
// 前方宣言
//============================================
class CPlayer;	//Cキャラクターでまとめたい
class CEnemy;

//============================================
// ボールクラス
//============================================
class CBall : public CWeapon
{
public:
	CBall(int nPriority = 3);
	~CBall();

	HRESULT Init(void);			//初期化
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);			//終了
	void Update(void);			//更新
	void Draw(void) override;			//描画
	void Attack(const D3DXVECTOR3 rot, const int nDamage = 1) override;			//攻撃
	void Throw(const D3DXVECTOR3 vec, float fPower) ;			//攻撃
	void Target();
	bool CollisionCircle(const D3DXVECTOR3 pos, const float fRad);			//当たり判定(プレイヤー)
	bool CollisionWall(const D3DXVECTOR3 pos, const D3DXVECTOR3* pVtx);		//当たり判定(壁)

	void SetMove(const D3DXVECTOR3 move) { m_move = move; }		//移動量設定
	D3DXVECTOR3 GetMove(void) { return m_move; }				//移動量取得
	void SetMember(const my_Identity::eMember member);		//所属設定
	my_Identity::eMember GetMember() { return m_eMember; }		//所属設定
	void SetPosOld(const D3DXVECTOR3 pos) { m_posOld = pos; }		//前回位置設定
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }				//前回位置取得
	void SetDeathCtr(const int nCtr) { m_nDeathCtr = nCtr; }		//消滅カウンタ設定
	int GetDeathCtr(void) { return m_nDeathCtr; }					//消滅カウンタ取得
	void AddSpeedMag(const float fSpeedMag) { m_fSpeedMag += fSpeedMag; }		//倍率加算
	void SetSpeedMag(const float fSpeedMag) { m_fSpeedMag = fSpeedMag; }		//倍率設定
	float GetSpeedMag(void) { return m_fSpeedMag; }								//倍率取得
	void SetTargetPlayer(CPlayer *pTargetPlayer) { m_pTargetPlayer = pTargetPlayer; }		//ターゲット設定
	CPlayer *GetTargetPlayer(void) { return m_pTargetPlayer; }								//ターゲット取得
	void SetTargetEnemy(CEnemy *pTargetEnemy) { m_pTargetEnemy = pTargetEnemy; }		//ターゲット設定
	CEnemy *GetTargetEnemy(void) { return m_pTargetEnemy; }								//ターゲット取得

	D3DXVECTOR3 GetWorldPos(void) { return D3DXVECTOR3(GetMtxWorld()._41, GetMtxWorld()._42, GetMtxWorld()._43); }								//倍率取得

	static void SetNumAll(const int nCtr) { s_nNumAll = nCtr; }		//ボール総数設定
	static int GetNumAll(void) { return s_nNumAll; }					//ボール総数取得
	static CBall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));		//生成

protected:

private:
	void CollisionField(D3DXVECTOR3 pos);

	float m_fRad;	//半径
	D3DXVECTOR3 m_posOld;			//前回の位置
	D3DXVECTOR3 m_move;			//移動量
	my_Identity::eMember m_eMember;		//所属
	bool m_bLand;		//地面に落ちてるフラグ(false:落ちてない)
	int m_nDeathCtr;	//自然消滅カウンタ
	float m_fSpeedMag;		//速度倍率
	CPlayer *m_pTargetPlayer;		//ターゲット
	CEnemy *m_pTargetEnemy;		//ターゲット
	static int s_nNumAll;
};

#endif
