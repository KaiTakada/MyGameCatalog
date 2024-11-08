//===============================================
//
// |S(object.cpp)
// Author ûüc ÀË
//
//===============================================
#include "object.h"
#include "manager.h"
#include "debugproc.h"
#include "main.h"
#include "block.h"
#include "item.h"
#include "enemy.h"
#include "camera.h"

//============================
// ÃIoÏé¾
//============================
CObject *CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT];		//IuWFNgCX^X
int CObject::m_nNumAll;		//IuWFNg

////============================
//// RXgN^
////============================
//CObject::CObject()
//{
//	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
//	{
//		for (int nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
//		{
//			if (m_apObject[nCntPrt][nCntAll] == nullptr)
//			{
//				m_apObject[nCntPrt][nCntAll] = this;		//©gðãü
//				m_nID = nCntAll;				//©gÌIDðÛ¶
//				m_nNumAll++;					//ðJEgAbv
//				break;
//			}
//		}
//	}
//}

//============================
// RXgN^(`æIo)
//============================
CObject::CObject(int nPriority)
{
	int nCntAll = 0;

	for (nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
	{
		if (m_apObject[nPriority][nCntAll] == nullptr)
		{
			m_apObject[nPriority][nCntAll] = this;		//©gðãü
			m_nID = nCntAll;				//©gÌIDðÛ¶
			m_nPriority = nPriority;		//©gÌ`æDæxðÛ¶
			m_bDeath = false;				//StOðº°é
			m_nNumAll++;					//ðJEgAbv
			break;
		}
	}

	//if (nCntAll >= MAX_OBJECT)
	//{
	//	return nullptr;
	//}
}
//============================
// fXgN^
//============================
CObject::~CObject()
{

}

//============================
// IuWFNgÌjü
//============================
void CObject::Release(void)
{
	int nIdx = m_nID;
	int nPrity = m_nPriority;

	if (m_apObject[nPrity][nIdx] != nullptr)
	{
		//©gjü
		delete m_apObject[nPrity][nIdx];
		m_apObject[nPrity][nIdx] = nullptr;

		//_E
		m_nNumAll--;
	}
}

//============================
// ^CvÅjü
//============================
void CObject::Reset(void)
{
	//SÁ
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntType = 0; nCntType < MAX_OBJECT; nCntType++)
		{
			if (m_apObject[nCntPrt][nCntType] != nullptr)
			{
				switch (m_apObject[nCntPrt][nCntType]->m_type)
				{
				case TYPE_BLOCK:

					m_apObject[nCntPrt][nCntType]->Uninit();

					break;

				case TYPE_ITEM:

					m_apObject[nCntPrt][nCntType]->Uninit();

					break;

				case TYPE_ENEMY:

					m_apObject[nCntPrt][nCntType]->Uninit();

					break;

				default:

					break;
				}

			}
		}
	}

	//CManager::SetObjDflt();
}

//============================
// SÄÌjü
//============================
void CObject::ReleaseAll(void)
{
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
		{
			if (m_apObject[nCntPrt][nCntAll] != nullptr)
			{
				m_apObject[nCntPrt][nCntAll]->Uninit();
			}
		}
	}
}

//============================
// SÄÌXV
//============================
void CObject::UpdateAll(void)
{
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
		{
			if (m_apObject[nCntPrt][nCntAll] != nullptr)
			{
				m_apObject[nCntPrt][nCntAll]->Update();
			}
		}
	}
}

//============================
// SÄÌ`æ
//============================
void CObject::DrawAll(void)
{
	int nCnt[PRIORITY_MAX] = {};
	CDebugProc *pDebug = CManager::GetDebugProc();
	pDebug->Print("FPS:%d\n", GetFPS());

	CCamera *pCamera = CManager::GetCamera();
	pCamera->SetCamera();

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
		{
			if (m_apObject[nCntPrt][nCntAll] != nullptr)
			{
				m_apObject[nCntPrt][nCntAll]->Draw();
			
				nCnt[nCntPrt]++;
			}
		}
	}

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		pDebug->Print("»ÝÌIuWFNg:%d\n", nCnt[nCntPrt]);
	}

	pDebug->Draw();
}

//============================
// `æÄÝè
//============================
void CObject::SetPrity(int nPriority)
{
	//this->GetType();


	//m_nPriority = nPriority;
}

//============================
// wèIuWFNgæ¾
//============================
CObject *CObject::GetObject(const int nPriority, const int nIdx)
{
	return m_apObject[nPriority][nIdx];
}
