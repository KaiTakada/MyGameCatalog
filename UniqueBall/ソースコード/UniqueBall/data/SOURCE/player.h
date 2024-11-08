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
class CState_Life;
class CUI_Life;
class CTarget_Mark;

//=========================
// プレイヤークラス
//=========================
class CPlayer : public CObjectCharacter
{
public:

	//====================
	// モーションの種類
	//====================
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		//待機
		MOTIONTYPE_WALK,			//歩き
		MOTIONTYPE_DASH,			//ダッシュ
		MOTIONTYPE_JUMP,			//ジャンプ
		MOTIONTYPE_JUMP2,			//2段ジャンプ
		MOTIONTYPE_GROUND,			//着地
		MOTIONTYPE_THROW_RDY,		//投げ用構える
		MOTIONTYPE_THROW,			//投げる
		MOTIONTYPE_CATCH_RDY,		//キャッチ用構える
		MOTIONTYPE_CATCH,			//キャッチ
		MOTIONTYPE_TACKLE,			//タックル
		MOTIONTYPE_DAMAGE,			//ダメージ
		MOTIONTYPE_NEUTRAL_BALL,	//待機(球持ち)
		MOTIONTYPE_DASH_BALL,		//ダッシュ(球持ち)
		MOTIONTYPE_JUMP_BALL,		//ジャンプ(球持ち)
		MOTIONTYPE_MAX,
	};

	//====================
	// ボタン管理
	//====================
	enum CONTROL_MANAGE
	{
		CONTROL_FRONT= 0,
		CONTROL_BACK,
		CONTROL_RIGHT,
		CONTROL_LEFT,
		CONTROL_JUMP,
		CONTROL_TACKLE,
		CONTROL_THROW,
		CONTROL_THROW_RELEASE,
		CONTROL_CATCH,
		CONTROL_TARGET,
		CONTROL_MAX,
	};

	CPlayer(int nPriority = 3);		//コンストラクタ
	~CPlayer();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//初期化(オバロ)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//生成
	
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//前回位置取得
	D3DXVECTOR3 GetSize(void) { return m_size; }				//サイズ取得
	CMotion* GetMotion(void);					//モーション情報取得
	
	static int GetControlPlayer() { return m_nControlIdx; }					//操作プレイヤー

	void SetIdx(int nIdx) { m_nIdx = nIdx; }		//番号設定
	int GetIdx(void) { return m_nIdx; }				//番号取得
	void SetUI_Life(CUI_Life* pLifeUI) { m_pLifeUI = pLifeUI; }		//体力UI設定
	CUI_Life *GetUI_Life(void) { return m_pLifeUI; }				//体力UI取得
	
	void ReadFile(void);	//ファイル読込
	void InitUI();	//UI初期化
	void InitModel();	//モデル初期化
	void Damage(int nDamege = 1);
	void Knockback(float fRot);		//ノックバック

protected:
	void MotionSet(int type, int brend = 0) override;

private:
	void MoveOperate(float *pfRotDest);		//移動操作
	void RotOperate(float *pfRotDest);		//向き操作

	void CollisionWall();
	void CollisionBall(D3DXVECTOR3 pos);
	void TackleCollision();
	void MotionEffect();
	void ReduceCounter();
	void ButtonAction();	//ボタンアクション
	void Debug();		//デバッグキー
	void RankOK();
	bool ControllManage(CONTROL_MANAGE key);

	void Throw();	//ボール投げ
	void Target();	//ターゲティング
	void SetNeutral();	//ニュートラル状態にする

	CBall *m_pBall;		//武器(ボール)情報
	CUI_Life *m_pLifeUI;		//体力UI
	int m_nCatchCtr;		//キャッチカウンター
	int m_nTackleCtr;		//タックルカウンター
	float m_fThrowChargeCtr;		//溜め投げカウンター
	int m_nJump2Ctr;		//2段ジャンプカウンター
	int m_nIdx;		//自身の番号
	bool m_bTarget;		//ターゲットフラグ[t:死んだ]
	CTarget_Mark *m_pTargetMark;		//ターゲットマーク

	static int m_nControlIdx;		//キーボードで操作するプレイヤー
};

#endif
