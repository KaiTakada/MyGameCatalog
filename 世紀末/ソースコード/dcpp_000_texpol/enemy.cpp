//===============================================
//
// G(enemy.cpp)
// Author ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "enemy.h"
#include "texture.h"
#include "game.h"
#include "player.h"
#include "experience.h"
#include "state_life.h"		//óÔÇ
#include "score.h"

//============================
// }Nè`
//============================
#define NUM_ATTACK (1.0f)	//UÍ
#define NUM_HP (10.0f)		//ÌÍ
#define NUM_SPEED (20.0f)	//Ú®Ê
#define ENEMY_FILE "data\\SET\\ENEMY\\parameter.txt"		//[Vt@CpX

//============================
// ÃIoÏé¾
//============================

//============================
// GÌíÞf
//============================
const char *c_apFilenameEnemy[] =
{
	"data\\MODEL\\ENEMY\\probe.x",		//noneT¸@
	"data\\MODEL\\ENEMY\\bike.x",		//speedoCN
	"data\\MODEL\\ENEMY\\waller.x",		//hpob^
	"data\\MODEL\\ENEMY\\tank.x",		//bossíÔ
};

//============================
// RXgN^
//============================
CEnemy::CEnemy(int nPriority) : CObjectX(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_param.fAttack = 0.0f;
	m_param.fLife = 0.0f;
	m_param.fLifeMax = 0.0f;
	m_param.fSpeed = 0.0f;
	m_pStateLife = nullptr;

	m_bMove = false;
}

//============================
// fXgN^
//============================
CEnemy::~CEnemy()
{

}

//====================================
// ú»(ftH)
//====================================
HRESULT CEnemy::Init(void)
{
	//ú»
	CObjectX::Init();

	//óÔÌ¶¬
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//^CvÝè
	SetType(TYPE_ENEMY);

	return S_OK;
}

//====================================
// ú»(ÊuIo)
//====================================
HRESULT CEnemy::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//ú»
	if(FAILED(CObjectX::Init(pos, rot)))
	{
		return E_FAIL;
	}

	//^CvÝè
	SetType(TYPE_ENEMY);

	//óÔÌ¶¬
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	return S_OK;
}

//============================
// I¹
//============================
void CEnemy::Uninit(void)
{
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	//I¹
	CObjectX::Uninit();
}

//============================
// XV
//============================
void CEnemy::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	pos += m_move;
	SetPos(pos);

	if (m_bMove == false)
	{//ªóÁÛÅË
		StalkPlayer();
	}
	else if(m_type == ET_BOSS)
	{
		MoveBoss();
	}

	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Update();
	}

	//CollisionCircle(pos);
	CollisionRect();

	CObjectX::Update();
}

//============================
// `æ
//============================
void CEnemy::Draw(void)
{
	D3DXMATERIAL m_DefMat;				//ftHg}eAf[^ÖÌ|C^

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->Get() == CState_Life::STATE_DAMAGE)
		{
			m_DefMat = m_pStateLife->GetMat();
			
			CObjectX::Draw(m_DefMat.MatD3D);
		}
		else
		{
			CObjectX::Draw();
		}
	}
}

//============================
// G¶¬
//============================
CEnemy * CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type)
{
	CEnemy *pEnemy = nullptr;
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//IuWFNg¶¬&ú»
	pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		pEnemy->Init(pos, rot);

		pEnemy->m_type = type;
		pEnemy->InitialState(type);
		pEnemy->InitialMove();

		CXModel *pXmodel = CManager::GetXModel();
		pEnemy->SetIdxModel(pXmodel->Regist(c_apFilenameEnemy[type], &vtxMin, &vtxMax));		//fÌèÄ

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
// _[W
//============================
void CEnemy::Damage(int nDamege)
{
	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->Get() != CState_Life::STATE_NORMAL)
		{//_[Wðó¯çêÈ¢óÔÈç
			return;
		}
	}

	m_param.fLife -= nDamege;

	m_pStateLife->Set(CState_Life::STATE_DAMAGE, 3);

	if (m_param.fLife <= 0)
	{
		CScore *pScore = CGame::GetScore();

		if (pScore == nullptr)
		{
			Uninit();
			return;
		}

		if (m_type == ET_BOSS)
		{
			pScore->CntValue(100000);

			CManager::SetResult(CManager::RT_WIN);
			Uninit();
		}
		else
		{
			pScore->CntValue(100);
			
			CExperience::Create(GetPos());
			Uninit();
		}
	}
}

//============================
// vC[ðÇ¢©¯é
//============================
void CEnemy::StalkPlayer()
{
	CPlayer *pPlayer;

	if ((pPlayer = CGame::GetPlayer()) == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posP = pPlayer->GetPos();

	D3DXVECTOR3 vecDiff = D3DXVECTOR3(posP.x - GetPos().x,
		0.0f,
		posP.z - GetPos().z);

	//ü«C³
	D3DXVECTOR3 rot = GetRot();

	rot.y = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;

	SetRot(rot);

	D3DXVec3Normalize(&vecDiff, &vecDiff);		//xNgð³K»·é(1.0fÉ·é)

	//ÅLÌXs[hðæ¹é
	vecDiff.x *= m_param.fSpeed;
	vecDiff.z *= m_param.fSpeed;

	SetMove(vecDiff);
}

//============================
// vC[ÆÌ½è»è(é`)
//============================
bool CEnemy::CollisionRect(void)
{
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//µß·ª
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetMaxVtx();
	D3DXVECTOR3 sizeMax = GetMaxVtx();
	D3DXVECTOR3 sizeMin = GetMinVtx();
	D3DXVECTOR3 posOld = pos - m_move;

	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjposOld = Objpos;
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	bool bCollision = false;			//½è»èª Á½©Ç¤©

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			bCollision = false;

			CObject *pObj;

			//IuWFNgðæ¾
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULL`FbN
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_PLAYER)
				{//vC\¾Á½ç
					Objpos = pObj->GetPos();
					ObjposOld = pObj->GetPosOld();
					Objsize = pObj->GetSize();

					if (ObjposOld.z + Objsize.z <= pos.z + sizeMin.z
						&& Objpos.z + Objsize.z >= pos.z + sizeMin.z
						&& Objpos.x + Objsize.x >= pos.x + sizeMin.x + 0.1f
						&& Objpos.x - Objsize.x <= pos.x + sizeMax.x - 0.1f
						&& ((Objpos.y + Objsize.y >= pos.y + sizeMin.y + 0.1f
						&& Objpos.y + Objsize.y <= pos.y + sizeMax.y - 0.1f)
						|| (Objpos.y - Objsize.y >= pos.y + sizeMin.y + 0.1f
						&& Objpos.y - Objsize.y <= pos.y + sizeMax.y - 0.1f)))
					{//ubNk
						move.z = (pos.z + sizeMin.z) - (Objpos.z + Objsize.z) - 0.1f;
						bCollision = true;
					}
					else if (ObjposOld.z - Objsize.z >= pos.z + sizeMax.z
						&& Objpos.z - Objsize.z <= pos.z + sizeMax.z
						&& Objpos.x + Objsize.x >= pos.x + sizeMin.x + 0.1f
						&& Objpos.x - Objsize.x <= pos.x + sizeMax.x - 0.1f
						&& ((Objpos.y + Objsize.y >= pos.y + sizeMin.y + 0.1f
						&& Objpos.y + Objsize.y <= pos.y + sizeMax.y - 0.1f)
						|| (Objpos.y - Objsize.y >= pos.y + sizeMin.y + 0.1f
						&& Objpos.y - Objsize.y <= pos.y + sizeMax.y - 0.1f)))
					{//ubNì
						move.z = (pos.z + sizeMax.z) - (Objpos.z - Objsize.z) + 0.1f;
						bCollision = true;
					}

					if (ObjposOld.x - Objsize.x >= pos.x + sizeMax.x
						&& Objpos.x - Objsize.x <= pos.x + sizeMax.x
						&& Objpos.z + Objsize.z > pos.z + sizeMin.z + 0.1f
						&& Objpos.z - Objsize.z < pos.z + sizeMax.z + 0.1f
						&& ((Objpos.y + Objsize.y >= pos.y + sizeMin.y + 0.1f
						&& Objpos.y + Objsize.y <= pos.y + sizeMax.y - 0.1f)
						|| (Objpos.y - Objsize.y >= pos.y + sizeMin.y + 0.1f
						&& Objpos.y - Objsize.y <= pos.y + sizeMax.y - 0.1f)))
					{//ubN
						move.x = (pos.x + sizeMax.x) - (Objpos.x - Objsize.x) + 0.1f;
						bCollision = true;
					}
					else if (ObjposOld.x + Objsize.x <= pos.x + sizeMin.x
						&& Objpos.x + Objsize.x > pos.x + sizeMin.x
						&& Objpos.z + Objsize.z > pos.z + sizeMin.z + 0.1f
						&& Objpos.z - Objsize.z < pos.z + sizeMax.z + 0.1f
						&& ((Objpos.y + Objsize.y >= pos.y + sizeMin.y + 0.1f
						&& Objpos.y + Objsize.y <= pos.y + sizeMax.y - 0.1f)
						|| (Objpos.y - Objsize.y >= pos.y + sizeMin.y + 0.1f
						&& Objpos.y - Objsize.y <= pos.y + sizeMax.y - 0.1f)))
					{//ubN¼
						move.x = (pos.x + sizeMin.x) - (Objpos.x + Objsize.x) - 0.1f;
						bCollision = true;
					}

					if (ObjposOld.y - Objsize.y >= pos.y + sizeMax.y
						&& Objpos.y - Objsize.y <= pos.y + sizeMax.y
						&& Objpos.x + Objsize.x > pos.x + sizeMin.x + 0.1f
						&& Objpos.x - Objsize.x < pos.x + sizeMax.x + 0.1f
						&& Objpos.z + Objsize.z > pos.z + sizeMin.z + 0.1f
						&& Objpos.z - Objsize.z < pos.z + sizeMax.z + 0.1f)
					{//ubNã
						D3DXVECTOR3 Objmove = pObj->GetMove();
						pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

						move.y = (pos.y + sizeMax.y) - (Objpos.y - Objsize.y) + 0.1f;
						pObj->SetJump(false);

						bCollision = true;
					}
					else if (ObjposOld.y + Objsize.y <= pos.y + sizeMin.y
						&& Objpos.y + Objsize.y >= pos.y + sizeMin.y
						&& Objpos.x + Objsize.x > pos.x + sizeMin.x + 0.1f
						&& Objpos.x - Objsize.x < pos.x + sizeMax.x + 0.1f
						&& Objpos.z + Objsize.z > pos.z + sizeMin.z + 0.1f
						&& Objpos.z - Objsize.z < pos.z + sizeMax.z + 0.1f)
					{//ubNº
						D3DXVECTOR3 Objmove = pObj->GetMove();
						pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

						move.y = (pos.y + sizeMin.y) - (Objpos.y + Objsize.y) - 0.1f;
						bCollision = true;
					}

					if (bCollision)
					{
						pObj->SetPos(Objpos + move);
						return bCollision;
					}
				}
			}
		}
	}

	return bCollision;
}

//============================
// GÆÌ½è»è(~)
//============================
bool CEnemy::CollisionCircle(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 size = GetMaxVtx();

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			CObject *pObj;

			//IuWFNgðæ¾
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULL`FbN
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_PLAYER)
				{//G¾Á½ç

					D3DXVECTOR3 Objpos = pObj->GetPos();
					D3DXVECTOR3 Objsize = pObj->GetMaxVtx();

					float fSize0 = (size.x + size.z) * 0.5f;		//ACeÌ¼axz
					float fSize1 = (size.x + size.y) * 0.5f;		//ACeÌ¼axy
					float fSize2 = (size.z + size.y) * 0.5f;		//ACeÌ¼azy

					float fObjsize0 = (Objsize.x + Objsize.z) * 0.5f;		//IuWFNgÌ¼axz
					float fObjsize1 = (Objsize.x + Objsize.y) * 0.5f;		//IuWFNgÌ¼axy
					float fObjsize2 = (Objsize.z + Objsize.y) * 0.5f;		//IuWFNgÌ¼azy

					float fColl0 = fSize0 + fObjsize0;		//½è»èÍÍ
					float fColl1 = fSize1 + fObjsize1;		//½è»èÍÍ
					float fColl2 = fSize2 + fObjsize2;		//½è»èÍÍ

					float fLength0 = hypotf((pos.x - Objpos.x), (pos.z - Objpos.z));		//2_ÔÌ·³xz
					float fLength1 = hypotf((pos.x - Objpos.x), (pos.y - Objpos.y));		//2_ÔÌ·³xy
					float fLength2 = hypotf((pos.z - Objpos.z), (pos.y - Objpos.y));		//2_ÔÌ·³zy

					if (fLength0 <= fColl0 &&
						fLength1 <= fColl1 &&
						fLength2 <= fColl2)
					{
						//_Ci~bNLXg
						//CPlayer *pPlayer = dynamic_cast<CPlayer*>(pObj);
						CPlayer *pPlayer = CGame::GetPlayer();

						//GI¹
						if (pPlayer != nullptr)
						{
							pPlayer->Damage(m_param.fAttack);
						}

						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

//============================
// úXeUè
//============================
void CEnemy::InitialState(CEnemy::TYPE type)
{
	m_param.fAttack = NUM_ATTACK;
	m_param.fLife = NUM_HP;
	m_param.fLifeMax = NUM_HP;
	m_param.fSpeed = NUM_SPEED;

	char *pComp = new char[256];
	FILE *pFile;

	pFile = fopen(ENEMY_FILE, "r");

	if (pFile != nullptr)
	{
		int nCnt = 0;

		while (true)
		{
			fscanf(pFile, "%s", pComp);

			if (nCnt >= CEnemy::ET_MAX)
			{//zñI[o[
				break;
			}

			if (strncmp(pComp, "#", 1) == 0)
			{// ±êÌ ÆRg
				fgets(pComp, 256, pFile);
				continue;
			}
			else if (strcmp(pComp, "SET_PARAM") == 0)
			{
				if (nCnt != type)
				{//
					nCnt++;
					continue;
				}
				
				nCnt++;

				while (true)
				{
					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "END_PARAM") == 0)
					{
						break;
					}
					else if (strcmp(pComp, "LIFE") == 0)
					{
						fscanf(pFile, "%s", pComp);
						fscanf(pFile, "%f", &m_param.fLife);
						m_param.fLifeMax = m_param.fLife;
						continue;
					}
					else if (strcmp(pComp, "SPEED") == 0)
					{
						fscanf(pFile, "%s", pComp);
						fscanf(pFile, "%f", &m_param.fSpeed);
						continue;
					}
					else if (strcmp(pComp, "ATTACK") == 0)
					{
						fscanf(pFile, "%s", pComp);
						fscanf(pFile, "%f", &m_param.fAttack);
						continue;
					}
				}
			}
		}

		fclose(pFile);
	}

	delete[] pComp;

	//switch (type)
	//{
	//case CEnemy::ET_NONE:
	//	break;
	//
	//case CEnemy::ET_SPEED:
	//	m_param.fSpeed *= 2;
	//	break;
	//
	//case CEnemy::ET_HP:
	//	m_param.fLife *= 2;
	//	m_param.fLifeMax *= 2;
	//	break;
	//
	//case CEnemy::ET_BOSS:
	//	m_param.fLife *= 5;
	//	m_param.fLifeMax *= 5;
	//	m_param.fSpeed *= 0.1f;
	//	break;
	//
	//case CEnemy::ET_MAX:
	//	break;
	//
	//default:
	//	break;
	//}
}

//============================
// ^CvÉ¶½s®ÖÄà
//============================
void CEnemy::InitialMove()
{
	switch (m_type)
	{
	case CEnemy::ET_NONE:
		break;

	case CEnemy::ET_SPEED:
		MoveSpeed();
		break;

	case CEnemy::ET_HP:
		break;

	case CEnemy::ET_BOSS:
		MoveBoss();
		break;

	case CEnemy::ET_MAX:
		break;

	default:
		break;
	}
}

//============================
// ^CvÉ¶½s®ÖÄà
//============================
void CEnemy::MoveSpeed()
{
	CPlayer *pPlayer;

	if ((pPlayer = CGame::GetPlayer()) == nullptr)
	{
		return;
	}

	D3DXVECTOR3 posP = pPlayer->GetPos();

	D3DXVECTOR3 vecDiff = D3DXVECTOR3(posP.x - GetPos().x,
		0.0f,
		posP.z - GetPos().z);

	//ü«C³
	D3DXVECTOR3 rot = GetRot();

	rot.y = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;

	SetRot(rot);

	D3DXVec3Normalize(&vecDiff, &vecDiff);		//xNgð³K»·é(1.0fÉ·é)

	//ÅLÌXs[hðæ¹é
	vecDiff.x *= m_param.fSpeed;
	vecDiff.z *= m_param.fSpeed;

	SetMove(vecDiff);

	m_bMove = true;
}

//============================
// ^CvÉ¶½s®ÖÄà
//============================
void CEnemy::MoveBoss()
{
	//N®µ½ç

	//N[^CÅU

	//tF[YÅUàeØèÖ¦ÍÇÁ

	if (m_bMove == false)
	{
		return;
	}

	StalkPlayer();
}
