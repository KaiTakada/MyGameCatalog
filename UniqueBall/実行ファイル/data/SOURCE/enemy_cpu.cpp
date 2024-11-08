//===============================================
//
// CPU敵(enemy_cpu.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "enemy_cpu.h"
#include "manager.h"	//ほぼ確で必要
#include "game.h"		//ほぼ確で必要
#include "camera.h"			//移動方向
#include "sound.h"			//演出等に必要
#include "parts.h"			//親子関係
#include "motion.h"			//モーション
#include "Field.h"			//地面との当たり判定
#include "wp_ball.h"		//ブースト
#include "Xmodel.h"			//モデル
#include "debugproc.h"		//デバッグ
#include "state_life.h"		//状態管理
#include "ef_smoke.h"		//煙演出
#include "wp_ball.h"		//ボールの当たり判定
#include "player.h"		//プレイヤー
#include "objectcharacter.h"		//完全

//デバッグ用(おそらく消す)
#include "block.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// 定数定義
//============================
namespace
{
	//// 行動抽選の構造体
	//struct sProbability
	//{
	//	CEnemy_CPU::EAction action;	// 行動
	//	float fProbability;			// 確率
	//};

	const float THROW_LENGTH(200.0f);		//投げの判定距離
	const float CAUTION_LENGTH(300.0f);		//警戒の判定距離
	//std::vector<sProbability> ACT_PROBABILITY_HAVE =	// 行動の抽選確率(ボールを持っているとき)
	//{
	//	{ CEnemy_CPU::ACTION_CHASE, 0.7f },		// 迫撃攻撃
	//	{ CEnemy_CPU::ACTION_THROW, 0.3f },		// ビーム攻撃
	//};
	//std::vector<sProbability> ACT_PROBABILITY_NOT =	// 行動の抽選確率(ボールを持っていないとき)
	//{
	//	{ CEnemy_CPU::ACTION_PICKUP, 0.7f },
	//	{ CEnemy_CPU::ACTION_CAUTION, 0.3f },
	//};

}

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
//============================
CEnemy_CPU::CEnemy_CPU(int nPriority) : CEnemy(nPriority)
{
	m_eAction = EAction::ACTION_NONE;
	m_pTarget = nullptr;
}

//============================
// デストラクタ
//============================
CEnemy_CPU::~CEnemy_CPU()
{

}

//====================================
// プレイヤーの初期化(デフォ)
//====================================
HRESULT CEnemy_CPU::Init(void)
{
	CEnemy::Init();

	m_eAction = EAction::ACTION_NONE;

	return S_OK;
}

//====================================
// プレイヤーの初期化(位置オバロ)
//====================================
HRESULT CEnemy_CPU::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nNumParts)
{
	CEnemy::Init(pos, rot, nNumParts);

	return S_OK;
}

//============================
// プレイヤーの終了
//============================
void CEnemy_CPU::Uninit(void)
{
	CEnemy::Uninit();
}

//============================
// プレイヤーの更新
//============================
void CEnemy_CPU::Update(void)
{
	CEnemy::Update();
}

//============================
// プレイヤーの描画
//============================
void CEnemy_CPU::Draw(void)
{
	CEnemy::Draw();
}

//============================
// プレイヤーの生成
//============================
CEnemy_CPU *CEnemy_CPU::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy_CPU *pEnemy = nullptr;

	//オブジェクト生成&初期化
	pEnemy = new CEnemy_CPU;

	if (pEnemy != nullptr)
	{
		pEnemy->Init(pos, rot, 1);
	}
	else
	{
		return nullptr;
	}

	return pEnemy;
}

//====================================
// アクション選択
//====================================
void CEnemy_CPU::ActionSelect()
{
	if (m_eAction == EAction::ACTION_NONE)
	{
		CBall *pBall = CEnemy::GetBall();

		//敵を探す
		SerchTarget();
		D3DXVECTOR3 pos = GetPos();
		float fLength = mylib_useful::Point2Angle(pos, m_pTarget->GetPos());

		//ボールを持っているか
		if (pBall != nullptr)
		{
			if (fLength < THROW_LENGTH)
			{//敵が近かったら投げる
			
			}
			else
			{//追いかける

			}
		}
		else
		{
			
			if (fLength > CAUTION_LENGTH)
			{//ボールを拾いに行く

			}
			else
			{//キャッチしようと後退しながら警戒する

			}
			

		}
	}

	m_eAction = EAction::ACTION_NONE;

}

//============================
// 敵を探す処理(xz距離)
//============================
void CEnemy_CPU::SerchTarget()
{
	my_Identity::eMember myMember = GetMember();
	D3DXVECTOR3 myPos = GetPos();
	float fLength = 0.0f;
	bool bFirst = false;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY)
				{

				 //キャスト
					CPlayer * pChara = static_cast<CPlayer*>(pObject);

					if (pChara != nullptr)
					{
						my_Identity::eMember member = pChara->GetMember();

						if (member != myMember)	// 他軍
						{
							D3DXVECTOR3 pos = pChara->GetPos();

							D3DXVECTOR3 vec = mylib_useful::Point2Rot(myPos, pos);
							float fLengthNow = mylib_useful::Vec2Length(vec.x, vec.z);
							if (!bFirst)
							{
								bFirst = !bFirst;
								fLength = fLengthNow;
							}
							else if (fLength > fLengthNow)
							{
								fLength = fLengthNow;
								m_pTarget = static_cast<CObjectCharacter*>(pChara);
							}
						}
					}
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == nullptr) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}
}