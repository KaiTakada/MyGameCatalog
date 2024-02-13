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
#include "objectX.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================
class CState_Life;

//=========================
// プレイヤークラス
//=========================
class CEnemy : public CObjectX
{
public:

	//=========================
	// 敵の種類
	//=========================
	enum TYPE
	{
		ET_NONE = 0,
		ET_SPEED,
		ET_HP,
		ET_BOSS,
		ET_MAX
	};

	//====================
	// キャラのパラメータ
	//====================
	struct Param
	{
		float fLife;		//体力
		float fLifeMax;		//最大体力
		float fSpeed;		//移動速度
		float fAttack;		//攻撃力
	};

	CEnemy(int nPriority = 2);		//コンストラクタ
	~CEnemy();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type = ET_NONE);		//生成
	bool CollisionRect(void);
	void Damage(int nDamege = 1);
	void StalkPlayer();
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }		//移動量設定
	bool CollisionCircle(D3DXVECTOR3 pos);
	void InitialState(CEnemy::TYPE type);
	void InitialMove();
	void MoveSpeed();
	void MoveBoss();

	void SetTypeE(CEnemy::TYPE type) { m_type = type; }
	CEnemy::TYPE GetTypeE() { return m_type; }

	void SetParam(Param param) { m_param = param; }
	Param GetParam() { return m_param; }

	void SetMoveFrag(bool bMove) { m_bMove = bMove; }
	bool GetMoveFrag() { return m_bMove; }
	void MoveCtrl() { m_bMove = !m_bMove; }

protected:

private:
	D3DXVECTOR3 m_move;		//移動量
	CEnemy::TYPE m_type;	//敵種
	Param m_param;			//パラメータ
	bool m_bMove;			//動きが決まっている奴はtrue、ただ追いかける奴はfalse
	CState_Life *m_pStateLife;		//状態
};

#endif
