//===============================================
//
// CPUG(enemy_cpu.cpp)
// Author ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "enemy_cpu.h"
#include "manager.h"	//ÙÚmÅKv
#include "game.h"		//ÙÚmÅKv
#include "camera.h"			//Ú®ûü
#include "sound.h"			//oÉKv
#include "parts.h"			//eqÖW
#include "motion.h"			//[V
#include "Field.h"			//nÊÆÌ½è»è
#include "wp_ball.h"		//u[Xg
#include "Xmodel.h"			//f
#include "debugproc.h"		//fobO
#include "state_life.h"		//óÔÇ
#include "ef_smoke.h"		//o
#include "wp_ball.h"		//{[Ì½è»è
#include "player.h"		//vC[
#include "objectcharacter.h"		//®S

//fobOp(¨»ç­Á·)
#include "block.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// èè`
//============================
namespace
{
	//// s®IÌ\¢Ì
	//struct sProbability
	//{
	//	CEnemy_CPU::EAction action;	// s®
	//	float fProbability;			// m¦
	//};

	const float THROW_LENGTH(200.0f);		//°Ì»è£
	const float CAUTION_LENGTH(300.0f);		//xúÌ»è£
	//std::vector<sProbability> ACT_PROBABILITY_HAVE =	// s®ÌIm¦({[ðÁÄ¢éÆ«)
	//{
	//	{ CEnemy_CPU::ACTION_CHASE, 0.7f },		// U
	//	{ CEnemy_CPU::ACTION_THROW, 0.3f },		// r[U
	//};
	//std::vector<sProbability> ACT_PROBABILITY_NOT =	// s®ÌIm¦({[ðÁÄ¢È¢Æ«)
	//{
	//	{ CEnemy_CPU::ACTION_PICKUP, 0.7f },
	//	{ CEnemy_CPU::ACTION_CAUTION, 0.3f },
	//};

}

//============================
// ÃIoÏé¾
//============================

//============================
// RXgN^
//============================
CEnemy_CPU::CEnemy_CPU(int nPriority) : CEnemy(nPriority)
{
	m_eAction = EAction::ACTION_NONE;
	m_pTarget = nullptr;
}

//============================
// fXgN^
//============================
CEnemy_CPU::~CEnemy_CPU()
{

}

//====================================
// vC[Ìú»(ftH)
//====================================
HRESULT CEnemy_CPU::Init(void)
{
	CEnemy::Init();

	m_eAction = EAction::ACTION_NONE;

	return S_OK;
}

//====================================
// vC[Ìú»(ÊuIo)
//====================================
HRESULT CEnemy_CPU::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nNumParts)
{
	CEnemy::Init(pos, rot, nNumParts);

	return S_OK;
}

//============================
// vC[ÌI¹
//============================
void CEnemy_CPU::Uninit(void)
{
	CEnemy::Uninit();
}

//============================
// vC[ÌXV
//============================
void CEnemy_CPU::Update(void)
{
	CEnemy::Update();
}

//============================
// vC[Ì`æ
//============================
void CEnemy_CPU::Draw(void)
{
	CEnemy::Draw();
}

//============================
// vC[Ì¶¬
//============================
CEnemy_CPU *CEnemy_CPU::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy_CPU *pEnemy = nullptr;

	//IuWFNg¶¬&ú»
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
// ANVIð
//====================================
void CEnemy_CPU::ActionSelect()
{
	if (m_eAction == EAction::ACTION_NONE)
	{
		CBall *pBall = CEnemy::GetBall();

		//GðT·
		SerchTarget();
		D3DXVECTOR3 pos = GetPos();
		float fLength = mylib_useful::Point2Angle(pos, m_pTarget->GetPos());

		//{[ðÁÄ¢é©
		if (pBall != nullptr)
		{
			if (fLength < THROW_LENGTH)
			{//Gªß©Á½ç°é
			
			}
			else
			{//Ç¢©¯é

			}
		}
		else
		{
			
			if (fLength > CAUTION_LENGTH)
			{//{[ðE¢És­

			}
			else
			{//Lb`µæ¤ÆãÞµÈªçxú·é

			}
			

		}
	}

	m_eAction = EAction::ACTION_NONE;

}

//============================
// GðT·(xz£)
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
				CObject::TYPE type = pObject->GetType();	//¡ñÌIuWFNgÌ^Cv

				if (type == CObject::TYPE_PLAYER || type == CObject::TYPE_ENEMY)
				{

				 //LXg
					CPlayer * pChara = static_cast<CPlayer*>(pObject);

					if (pChara != nullptr)
					{
						my_Identity::eMember member = pChara->GetMember();

						if (member != myMember)	// ¼R
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
			{// (pObject == nullptr) == EndÜÅsÁ½ÁÄ±ÆÅ±ÌDæxÍI¹
				break;
			}
		}
	}
}