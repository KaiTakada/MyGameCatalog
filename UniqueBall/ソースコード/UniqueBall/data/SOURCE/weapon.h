//==================================================
//
// 武器処理(weapon.h)
// Author : Kai Takada
//
//==================================================
#ifndef _WEAPON_H_								//このマクロ定義がされてなかったら
#define _WEAPON_H_								//二重インクルード防止のマクロを定義する

#include "Parts.h"

//マクロ定義
#define MAX_WEAPON (256)						//ブロックの最大数
#define BLOCK_WIDTH (80.0f)						//ブロックの幅
#define BLOCK_HEIGHT (80.0f)					//ブロックの高さ

//============================================
// 前方宣言
//============================================

//============================================
// ブロッククラス
//============================================
class CWeapon : public CParts
{
public:
	//===================================
	// ブロック列挙型の定義
	//===================================
	typedef enum
	{
		WPNTYPE_NIFE = 0,		//ナイフ
		WPNTYPE_REVOLVER,		//リボルバー
		WPNTYPE_AK,				//ライフル
		WPNTYPE_BOOST,			//ブースト
		WPNTYPE_BALL,			//ボール
		WPNTYPE_MAX,
	}WPNTYPE;

	CWeapon(int nPriority = 3);
	~CWeapon();

	HRESULT Init(void);			//初期化
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const WPNTYPE type);		//初期化(オバロ)
	void Uninit(void);			//終了
	void Update(void);			//更新
	void Draw(void);			//描画
	virtual void Attack(const D3DXVECTOR3 rot, const int nDamage = 1);			//攻撃

	static CWeapon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), WPNTYPE type = WPNTYPE_NIFE);		//生成

	void SetWpnType(CWeapon::WPNTYPE type);
	CWeapon::WPNTYPE GetWpnType() { return m_type; }
	void GetCooltime(int nCooltime) { m_nCooltime = nCooltime; }
	int SetCooltime() { return m_nCooltime; }
	void SetCooltimeMin(int nCooltime) { m_nCooltimeMin = nCooltime; }
	int GetCooltimeMin() { return m_nCooltimeMin; }

protected:

private:
	void ReadMuzzleFile(WPNTYPE type = WPNTYPE_NIFE);
	void ReadCooltime(WPNTYPE type = WPNTYPE_NIFE);

	CWeapon::WPNTYPE m_type;				//種類
	D3DXVECTOR3 m_muzzlePos;				//銃口位置
	int m_nCooltime;						//クールタイム
	int m_nCooltimeMin;						//クールタイムマイナス分
};

#endif
