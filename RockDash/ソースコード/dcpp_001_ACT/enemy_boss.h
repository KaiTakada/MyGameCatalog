//===============================================
//
// ボス敵(enemy_boss.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _ENEMY_BOSS_H_		//このマクロ定義がされてなかったら
#define _ENEMY_BOSS_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "enemy.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================

//=========================
// プレイヤークラス
//=========================
class CEnemy_Boss : public CEnemy
{
public:

	//=========================
	// 敵の種類
	//=========================
	enum ATACKSTATE
	{
		AS_NONE = 0,
		AS_STALK,
		AS_BURST,
		AS_TACKLE,
		AS_MAX
	};

	CEnemy_Boss(int nPriority = 2);		//コンストラクタ
	~CEnemy_Boss();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CEnemy_Boss *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//生成
	void SetAttack(CEnemy_Boss::ATACKSTATE state);		//行動設定
	void Attack();		//攻撃内容案内
	bool Burst();		//攻撃内容
	bool Tackle();		//攻撃内容

protected:

private:
	CEnemy_Boss::ATACKSTATE m_AttackState;		//行動種
	int m_nCoolTime;		//行動パターン毎にCT設定
	bool m_bClear;			//行動完了フラグ
	int m_nMoveCnt;			//行動カウンター
};

#endif
