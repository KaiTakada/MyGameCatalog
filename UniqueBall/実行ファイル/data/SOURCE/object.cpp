//===============================================
//
// ポリゴン(object.cpp)
// Author 髙田 佳依
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
// 静的メンバ変数宣言
//============================
CObject *CObject::m_apTop[PRIORITY_MAX] = {};
CObject *CObject::m_apEnd[PRIORITY_MAX] = {};
int CObject::m_nNumAll;		//オブジェクト総数

//============================
// コンストラクタ(描画順オバロ)
//============================
CObject::CObject(int nPriority)
{
	if (m_apTop[nPriority] == nullptr)
	{//先頭がいないならthisを据える
		m_apTop[nPriority] = this;
	}

	m_pNext = nullptr;

	//現状の最後尾
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
// デストラクタ
//============================
CObject::~CObject()
{

}

//============================
// オブジェクトの破棄
//============================
void CObject::Release(void)
{
	int nPrity = m_nPriority;
	CObject *pNext = m_pNext;		//次の
	CObject *pPrev = m_pPrev;		//前の

	if (m_apTop[nPrity] != nullptr)
	{
		if (m_apTop[nPrity] == this)
		{//先頭とthisが一緒であれば

			m_apTop[nPrity] = pNext;
		}
	}

	if (m_apEnd[nPrity] != nullptr)
	{
		if (m_apEnd[nPrity] == this)
		{//最後尾とthisが一緒であれば

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
		{//自身の後を先代に引き継がせる
			pPrev->m_pNext = pNext;
		}
	}

	if (pNext != nullptr)
	{//自身の前を後続に引き継がせる
		pNext->m_pPrev = m_pPrev;
	}

	delete this;

	//総数ダウン
	m_nNumAll--;
}

//============================
// 全ての破棄
//============================
void CObject::ReleaseAll(void)
{
	CObject *pObject = m_apTop[0];

	//全てに死亡フラグを立てていく
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

	//死亡フラグONなら消す
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
// 全ての更新
//============================
void CObject::UpdateAll(void)
{
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("FPS:%d\n", GetFPS());
	pDebug->Print("オブジェクト総数:%d\n", m_nNumAll);

	CObject *pObject = m_apTop[0];

	//更新
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
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}

		pObject = m_apTop[nCnt];
	}

	if (CPlayerManager* pPlayerManager = CPlayerManager::GetInstance())
	{
		pPlayerManager->Update();
	}

	//死亡フラグに応じて消す
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
// 全ての描画
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
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}
}
