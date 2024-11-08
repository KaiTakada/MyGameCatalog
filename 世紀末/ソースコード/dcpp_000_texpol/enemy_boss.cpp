//===============================================
//
// {XG(enemy_boss.cpp)
// Author ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "enemy_boss.h"
#include "bullet.h"

//============================
// }Nè`
//============================
#define NUM_ATTACK (1.0f)	//UÍ
#define NUM_HP (10.0f)		//ÌÍ
#define NUM_SPEED (20.0f)	//Ú®Ê
#define BOSS_FILE "data\\MODEL\\ENEMY\\tank.x"		//bossíÔ
#define NUM_COOL (120)	//N[^C
#define NUM_BURST (60)	//burst­Ë
#define NUM_TACKLE (30)	//tacklet[
#define NUM_TACKLE_ADD (10.0f)	//tackleXs[hÁ¬l
#define NUM_BURST_READY (60)	//burstõt[

//============================
// ÃIoÏé¾
//============================

//============================
// RXgN^
//============================
CEnemy_Boss::CEnemy_Boss(int nPriority) : CEnemy(nPriority)
{
	m_AttackState = AS_NONE;
	m_nCoolTime = 0;
	m_bClear = true;
	m_nMoveCnt = 0;
}

//============================
// fXgN^
//============================
CEnemy_Boss::~CEnemy_Boss()
{

}

//====================================
// ú»(ftH)
//====================================
HRESULT CEnemy_Boss::Init(void)
{
	//ú»
	CEnemy::Init();

	//^CvÝè
	SetType(TYPE_BOSS);

	return S_OK;
}

//====================================
// ú»(ÊuIo)
//====================================
HRESULT CEnemy_Boss::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//ú»
	if(FAILED(CEnemy::Init(pos, rot)))
	{
		return E_FAIL;
	}

	//^CvÝè
	SetType(TYPE_BOSS);

	return S_OK;
}

//============================
// I¹
//============================
void CEnemy_Boss::Uninit(void)
{
	//I¹
	CEnemy::Uninit();
}

//============================
// XV
//============================
void CEnemy_Boss::Update(void)
{
	Param param = GetParam();
	float fRatio = param.fLife / param.fLifeMax;

	if (!GetMoveFrag())
	{//®­»è¶áÈ¯êÎ

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
	{//s®®¹&N[_E®¹

		//_s®Ýè
		int nSet = 0;

		fRatio = param.fLife / param.fLifeMax;

		if (fRatio <= 0.5)
		{//ÌÍª¼ªÉÈéÆ^bNÇÁ
			nSet = rand() % AS_MAX;
		}
		else if (fRatio <= 0.9)
		{//ÌÍª¼ªÉÈéÆ^bNÇÁ
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
	{//s®®¹&N[_E
		m_nCoolTime--;
	}

	CEnemy::Update();
}

//============================
// `æ
//============================
void CEnemy_Boss::Draw(void)
{
	//`æ
	CEnemy::Draw();
}

//============================
// G¶¬
//============================
CEnemy_Boss * CEnemy_Boss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy_Boss *pEnemy = nullptr;
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//IuWFNg¶¬&ú»
	pEnemy = new CEnemy_Boss;

	if (pEnemy != nullptr)
	{
		pEnemy->Init(pos, rot);

		pEnemy->SetTypeE(ET_BOSS);
		pEnemy->InitialState(ET_BOSS);
		pEnemy->InitialMove();
		
		CXModel *pXmodel = CManager::GetXModel();
		pEnemy->SetIdxModel(pXmodel->Regist(BOSS_FILE, &vtxMin, &vtxMax));		//fÌèÄ

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
// UÝè
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
// UÄà
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
// e­Ë
//============================
bool CEnemy_Boss::Burst()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	if (m_nMoveCnt >= NUM_BURST_READY)
	{//õ60f

		//vC\ÖÌpx©çNUM_BURSTÌ¼ª¸çµÄ1t[Éê­¸Â¸çµÄÅÂ
		CBullet::Create(pos, rot, CBullet::SIDE_ENEMY, 2);
		rot.y += 0.01f;

		if (m_nMoveCnt >= NUM_BURST_READY + NUM_BURST)
		{
			//®¹µ½ç
			m_AttackState = AS_NONE;

			return true;
		}
	}
	else if (m_nMoveCnt == NUM_BURST_READY - 1)
	{//­ËO

		rot.y -= 0.01f * NUM_BURST;
		SetRot(rot);
	}

	//»¤ÅÈ¢Èç
	return false;
}

//============================
// Ëi
//============================
bool CEnemy_Boss::Tackle()
{
	Param param = GetParam();

	if (m_nMoveCnt >= NUM_BURST_READY)
	{//õ60f

		//¬Ú®úÔ

		//Á¬
		param.fSpeed *= NUM_TACKLE_ADD;
		SetParam(param);

		StalkPlayer();

		//³Éß·
		param.fSpeed *= (1 / NUM_TACKLE_ADD);
		SetParam(param);

		if (m_nMoveCnt >= NUM_BURST_READY + NUM_TACKLE)
		{
			//®¹µ½ç
			m_AttackState = AS_NONE;

			return true;
		}
	}
	else if (m_nMoveCnt == NUM_BURST_READY - 1)
	{//­ËO

	}

	//»¤ÅÈ¢Èç
	return false;
}
