//===============================================
//
// CPU敵(enemy_cpu.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _ENEMY_CPU_H_		//このマクロ定義がされてなかったら
#define _ENEMY_CPU_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "enemy.h"
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
class CObjectCharacter;

//=========================
// CPU敵クラス
//=========================
class CEnemy_CPU : public CEnemy
{
public:

	//アクション列挙型
	enum EAction
	{
		ACTION_NONE = 0,
		ACTION_CHASE,	// プレイヤーを追いかける
		ACTION_THROW,	// 投げる
		ACTION_PICKUP,	// ボールを拾いに行く
		ACTION_CAUTION,	// プレイヤーを警戒する(キャッチしようとする)
		ACTION_MAX,
	};

	CEnemy_CPU(int nPriority = 3);		//コンストラクタ
	~CEnemy_CPU();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//初期化(オバロ)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CEnemy_CPU *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//生成

	void SetAction(const EAction eAction) { m_eAction = eAction; }		//アクション設定
	EAction GetAction() { return m_eAction; }							//アクション取得

	void ActionSelect();	//アクション選択

protected:

private:
	void SerchTarget();		//最寄りの敵を補足

	EAction m_eAction;	//アクション

	CObjectCharacter* m_pTarget;	//ターゲットのキャラクター
};

#endif