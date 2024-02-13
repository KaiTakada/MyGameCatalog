//===============================================
//
// プレイヤー(player.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
//#include "objectX.h"
#include "object.h"

//=========================
// マクロ定義
//=========================
#define MAX_PLAYER_PARTS (32)		//最大パーツ数
#define MAX_PLAYER_SIZE D3DXVECTOR3(30.0f,50.0f,30.0f)		//サイズ

//=========================
// 前方宣言
//=========================
class CParts;
class CMotion;
class CWeapon;
class CGauge;
class CCursor;
class CState_Life;

//=========================
// プレイヤークラス
//=========================
class CPlayer : public CObject
{
public:

	//====================
	// モーションの種類
	//====================
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	//待機
		MOTIONTYPE_WALK,	//ダッシュ
		MOTIONTYPE_DASH,	//拍手
		MOTIONTYPE_JUMP,	//ジャンプ
		MOTIONTYPE_GROUND,	//着地
		MOTIONTYPE_FIRE_REV,	//発射(REVOLVER)
		MOTIONTYPE_FIRE_AK,	//発射(AK)
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
		float fSpeedFire;	//連射速度
		float fAttack;		//攻撃力
		float fGetArea;		//引き寄せ判定の半径
		float fColl;		//当たり判定の半径
	}Param;

	CPlayer(int nPriority = 3);		//コンストラクタ
	~CPlayer();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//初期化(オバロ)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), int nNumParts = 1);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//生成
	
	bool GetJump(void) { return m_bJump; }						//ジャンプ判定
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//前回位置取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }					//現在位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }					//向き取得
	D3DXVECTOR3 GetMove(void) { return m_move; }				//移動量取得
	D3DXVECTOR3 GetSize(void) { return MAX_PLAYER_SIZE; }				//サイズ取得
	float GetHeartPos(void) { return m_fHeart; }				//心臓位置取得
	float GetExp(void) { return m_fExp; }						//現在経験値取得
	Param GetParam(void) { return m_param; }					//現在パラメータ取得
	void SetJump(const bool bJump) { m_bJump = bJump; }				//ジャンプ判定設定
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//移動量設定
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }				//現在位置設定
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }				//向き設定
	void SetHeartPos(const float fHeart) { m_fHeart = fHeart; }		//心臓位置設定
	void SetExp(const float fExp);				//現在経験値設定
	void CntExp(const float fExp);				//現在経験値加算
	void SetParam(const Param param) { m_param = param; }			//現在パラメータ設定
	void ReadFile(void);	//ファイル読込
	void Damage(float fDamege = 1.0f);

protected:

private:
	void MoveOperate(float *pfRotDest);		//移動操作
	void RotOperate(float *pfRotDest);		//向き操作
	void RotAdj(float fRotDiff);			//向き修正
	void PullItem(void);					//アイテム引き寄せ
	void RotLimit(float *pRot);				//向き修正

	void DebugKey(CInputKeyboard *pInputKeyboard);		//デバッグキー
	void CollisionField(D3DXVECTOR3 pos);

	CParts *m_apPart[MAX_PLAYER_PARTS];		//モデル(パーツ)へのポインタ
	int m_nNumModel;							//モデル(パーツ)の総数
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_posOld;		//前回位置
	D3DXVECTOR3 m_pos;			//現在位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	float m_fHeart;			//プレイヤーの中心y軸(心臓座標)
	bool m_bJump;			//ジャンプ判定(falseはジャンプ可能状態)
	CMotion *m_pMotion;		//モーション情報
	Param m_param;			//パラメータ
	CWeapon *m_pWeapon;		//武器情報
	CGauge *m_pGaugeHP;		//HP情報
	CGauge *m_pGaugeExp;	//exp情報
	CCursor *m_pCursor;		//Aim用カーソル
	float m_fExp;			//現在経験値
	float m_fExpMax;		//経験値ゲージMax値
	CState_Life *m_pStateLife;		//状態
};

#endif
