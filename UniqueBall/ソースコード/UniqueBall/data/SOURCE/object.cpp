//===============================================
//
// |S(object.cpp)
// Author ϋόc ΐΛ
//
//===============================================
#include "object.h"
#include "debugproc.h"
#include "main.h"
#include "block.h"
#include "item.h"
#include "enemy.h"
#include "camera.h"

#include "obj_polygon3D.h"

#include "playerManager.h"

//============================
// ΓIoΟιΎ
//============================
CObject *CObject::m_apTop[PRIORITY_MAX] = {};
CObject *CObject::m_apEnd[PRIORITY_MAX] = {};
int CObject::m_nNumAll;		//IuWFNg

//============================
// RXgN^(`ζIo)
//============================
CObject::CObject(int nPriority)
{
	if (m_apTop[nPriority] == nullptr)
	{//ζͺͺ’Θ’Θηthisπ¦ι
		m_apTop[nPriority] = this;
	}

	m_pNext = nullptr;

	//»σΜΕγφ
	m_pPrev = m_apEnd[nPriority];
	m_nPriority = nPriority;
	m_bDeath = false;

	if (m_apEnd[nPriority] != nullptr)
	{
		m_apEnd[nPriority]->m_pNext = this;
	}

	m_apEnd[nPriority] = this;
	m_nNumAll++;
}
//============================
// fXgN^
//============================
CObject::~CObject()
{

}

//============================
// IuWFNgΜjό
//============================
void CObject::Release(void)
{
	int nPrity = m_nPriority;
	CObject *pNext = m_pNext;		//Μ
	CObject *pPrev = m_pPrev;		//OΜ

	if (m_apTop[nPrity] != nullptr)
	{
		if (m_apTop[nPrity] == this)
		{//ζͺΖthisͺκΕ κΞ

			m_apTop[nPrity] = pNext;
		}
	}

	if (m_apEnd[nPrity] != nullptr)
	{
		if (m_apEnd[nPrity] == this)
		{//ΕγφΖthisͺκΕ κΞ

			m_apEnd[nPrity] = m_pPrev;
		}
	}

	if (pPrev != nullptr)
	{
		if (pNext == nullptr)
		{
			pPrev->m_pNext = nullptr;
		}
		else
		{//©gΜγπζγΙψ«pͺΉι
			pPrev->m_pNext = pNext;
		}
	}

	if (pNext != nullptr)
	{//©gΜOπγ±Ιψ«pͺΉι
		pNext->m_pPrev = m_pPrev;
	}

	delete this;

	//_E
	m_nNumAll--;
}

//============================
// SΔΜjό
//============================
void CObject::ReleaseAll(void)
{
	CObject *pObject = m_apTop[0];

	//SΔΙStOπ§ΔΔ’­
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->m_pNext;

			pObject->SetDeath();

			pObject = pObjectNext;
		}

		pObject = m_apTop[nCnt];
	}

	if (CPlayerManager* pPlayerManager = CPlayerManager::GetInstance())
	{
		pPlayerManager->Uninit();
	}

	//StOONΘηΑ·
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				pObject->Uninit();
			}
		
			pObject = pObjectNext;
		}

		pObject = m_apTop[nCnt];
	}
}

//============================
// SΔΜXV
//============================
void CObject::UpdateAll(void)
{
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("FPS:%d\n", GetFPS());
	pDebug->Print("IuWFNg:%d\n", m_nNumAll);

	CObject *pObject = m_apTop[0];

	//XV
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				if (pObject->m_type == TYPE_FADE)
				{
					pObject->Update();
				}

				CObject *pObjectNext = pObject->m_pNext;

				pObject->Update();

				pObject = pObjectNext;
			}
			else
			{// (pObject == NULL) == EndάΕsΑ½ΑΔ±ΖΕ±ΜDζxΝIΉ
				break;
			}
		}

		pObject = m_apTop[nCnt];
	}

	if (CPlayerManager* pPlayerManager = CPlayerManager::GetInstance())
	{
		pPlayerManager->Update();
	}

	//StOΙΆΔΑ·
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				pObject->Uninit();
			}

			pObject = pObjectNext;
		}

		pObject = m_apTop[nCnt];
	}
}

//============================
// SΔΜ`ζ
//============================
void CObject::DrawAll(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	if (pCamera != nullptr)
	{
		//pCamera->SetCamera();
	}

	CObject *pObject = nullptr;

	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		pObject = m_apTop[nCnt];

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				pObject->Draw();

				pObject = pObject->m_pNext;
			}
			else
			{// (pObject == NULL) == EndάΕsΑ½ΑΔ±ΖΕ±ΜDζxΝIΉ
				break;
			}
		}
	}
}
