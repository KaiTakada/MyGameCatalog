//===============================================
//
// ボス敵(enemy_boss.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "enemy_boss.h"
#include "bullet.h"

//============================
// マクロ定義
//============================
#define NUM_ATTACK (1.0f)	//攻撃力
#define NUM_HP (10.0f)		//体力
#define NUM_SPEED (20.0f)	//移動量
#define BOSS_FILE "data\\MODEL\\ENEMY\\tank.x"		//boss戦車
#define NUM_COOL (120)	//クールタイム
#define NUM_BURST (60)	//burst時発射数
#define NUM_TACKLE (30)	//tackle時フレーム数
#define NUM_TACKLE_ADD (10.0f)	//tackle時スピード加速値
#define NUM_BURST_READY (60)	//burst準備フレーム

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
//============================
CEnemy_Boss::CEnemy_Boss(int nPriority) : CEnemy(nPriority)
{
	m_AttackState = AS_NONE;
	m_nCoolTime = 0;
	m_bClear = true;
	m_nMoveCnt = 0;
}

//============================
// デストラクタ
//============================
CEnemy_Boss::~CEnemy_Boss()
{

}

//====================================
// 初期化(デフォ)
//====================================
HRESULT CEnemy_Boss::Init(void)
{
	//初期化
	CEnemy::Init();

	//タイプ設定
	SetType(TYPE_BOSS);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CEnemy_Boss::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//初期化
	if(FAILED(CEnemy::Init(pos, rot)))
	{
		return E_FAIL;
	}

	//タイプ設定
	SetType(TYPE_BOSS);

	return S_OK;
}

//============================
// 終了
//============================
void CEnemy_Boss::Uninit(void)
{
	//終了
	CEnemy::Uninit();
}

//============================
// 更新
//============================
void CEnemy_Boss::Update(void)
{
	Param param = GetParam();
	float fRatio = param.fLife / param.fLifeMax;

	if (!GetMoveFrag())
	{//動く判定じゃなければ

		if (fRatio <= 0.9)
		{
			SetMoveFrag(true);
			return;
		}
		else
		{
			return;
		}
	}

	Attack();

	if (m_bClear == true && m_nCoolTime <= 0)
	{//行動完了&クールダウン完了

		//ランダム行動設定
		int nSet = 0;

		fRatio = param.fLife / param.fLifeMax;

		if (fRatio <= 0.5)
		{//体力が半分になるとタックル追加
			nSet = rand() % AS_MAX;
		}
		else if (fRatio <= 0.9)
		{//体力が半分になるとタックル追加
			nSet = rand() % AS_MAX;
			
		}
		else
		{
			nSet = rand() % AS_TACKLE;
		}

#if _DEBUG
		SetAttack(CEnemy_Boss::ATACKSTATE(nSet));
#else
		SetAttack(CEnemy_Boss::ATACKSTATE(nSet));
#endif
	}
	else if (m_bClear == true && m_nCoolTime > 0)
	{//行動完了&クールダウン中
		m_nCoolTime--;
	}

	CEnemy::Update();
}

//============================
// 描画
//============================
void CEnemy_Boss::Draw(void)
{
	//描画
	CEnemy::Draw();
}

//============================
// 敵生成
//============================
CEnemy_Boss * CEnemy_Boss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy_Boss *pEnemy = nullptr;
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//オブジェクト生成&初期化
	pEnemy = new CEnemy_Boss;

	if (pEnemy != nullptr)
	{
		pEnemy->Init(pos, rot);

		pEnemy->SetTypeE(ET_BOSS);
		pEnemy->InitialState(ET_BOSS);
		pEnemy->InitialMove();
		
		CXModel *pXmodel = CManager::GetXModel();
		pEnemy->SetIdxModel(pXmodel->Regist(BOSS_FILE, &vtxMin, &vtxMax));		//モデルの割り当て

		vtxMin = pXmodel->GetAddress(pEnemy->GetIdxModel())->vtxMin;
		vtxMax = pXmodel->GetAddress(pEnemy->GetIdxModel())->vtxMax;
		pEnemy->SetVtx(vtxMin, vtxMax);
	}
	else
	{
		return nullptr;
	}

	return pEnemy;
}

//============================
// 攻撃設定
//============================
void CEnemy_Boss::SetAttack(CEnemy_Boss::ATACKSTATE state)
{
	m_AttackState = state;
	m_nMoveCnt = 0;
	m_bClear = false;

	switch (m_AttackState)
	{

	case CEnemy_Boss::AS_BURST:
	case CEnemy_Boss::AS_TACKLE:
		m_nCoolTime = NUM_COOL;
		break;

	default:
		m_nCoolTime = 0;
		m_bClear = true;
		break;
	}
}

//============================
// 攻撃案内
//============================
void CEnemy_Boss::Attack()
{
	switch (m_AttackState)
	{
	case CEnemy_Boss::AS_STALK:

		StalkPlayer();
		m_bClear = true;

		break;

	case CEnemy_Boss::AS_BURST:

		m_bClear = Burst();

		break;

	case CEnemy_Boss::AS_TACKLE:

		m_bClear = Tackle();

		break;

	default:

		m_AttackState = AS_NONE;
		m_nCoolTime = 0;

		break;
	}

	m_nMoveCnt++;
}

//============================
// 弾発射
//============================
bool CEnemy_Boss::Burst()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	if (m_nMoveCnt >= NUM_BURST_READY)
	{//準備60f

		//プレイヤ―への角度からNUM_BURSTの半数分ずらして1フレームに一発ずつずらして打つ
		CBullet::Create(pos, rot, CBullet::SIDE_ENEMY, 2);
		rot.y += 0.01f;

		if (m_nMoveCnt >= NUM_BURST_READY + NUM_BURST)
		{
			//完了したら
			m_AttackState = AS_NONE;

			return true;
		}
	}
	else if (m_nMoveCnt == NUM_BURST_READY - 1)
	{//発射前

		rot.y -= 0.01f * NUM_BURST;
		SetRot(rot);
	}

	//そうでないなら
	return false;
}

//============================
// 突進
//============================
bool CEnemy_Boss::Tackle()
{
	Param param = GetParam();

	if (m_nMoveCnt >= NUM_BURST_READY)
	{//準備60f

		//高速移動期間

		//加速
		param.fSpeed *= NUM_TACKLE_ADD;
		SetParam(param);

		StalkPlayer();

		//元に戻す
		param.fSpeed *= (1 / NUM_TACKLE_ADD);
		SetParam(param);

		if (m_nMoveCnt >= NUM_BURST_READY + NUM_TACKLE)
		{
			//完了したら
			m_AttackState = AS_NONE;

			return true;
		}
	}
	else if (m_nMoveCnt == NUM_BURST_READY - 1)
	{//発射前

	}

	//そうでないなら
	return false;
}
