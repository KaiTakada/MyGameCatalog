//===============================================
//
// ポリゴン(object.cpp)
// Author 髙田 佳依
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
// 静的メンバ変数宣言
//============================
CObject *CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT];		//オブジェクトインスタンス
int CObject::m_nNumAll;		//オブジェクト総数

////============================
//// コンストラクタ
////============================
//CObject::CObject()
//{
//	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
//	{
//		for (int nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
//		{
//			if (m_apObject[nCntPrt][nCntAll] == nullptr)
//			{
//				m_apObject[nCntPrt][nCntAll] = this;		//自身を代入
//				m_nID = nCntAll;				//自身のIDを保存
//				m_nNumAll++;					//総数をカウントアップ
//				break;
//			}
//		}
//	}
//}

//============================
// コンストラクタ(描画順オバロ)
//============================
CObject::CObject(int nPriority)
{
	int nCntAll = 0;

	for (nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
	{
		if (m_apObject[nPriority][nCntAll] == nullptr)
		{
			m_apObject[nPriority][nCntAll] = this;		//自身を代入
			m_nID = nCntAll;				//自身のIDを保存
			m_nPriority = nPriority;		//自身の描画優先度を保存
			m_bDeath = false;				//死亡フラグを下げる
			m_nNumAll++;					//総数をカウントアップ
			break;
		}
	}

	//if (nCntAll >= MAX_OBJECT)
	//{
	//	return nullptr;
	//}
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
	int nIdx = m_nID;
	int nPrity = m_nPriority;

	if (m_apObject[nPrity][nIdx] != nullptr)
	{
		//自身破棄
		delete m_apObject[nPrity][nIdx];
		m_apObject[nPrity][nIdx] = nullptr;

		//総数ダウン
		m_nNumAll--;
	}
}

//============================
// タイプで破棄
//============================
void CObject::Reset(void)
{
	//全消去
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
// 全ての破棄
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
// 全ての更新
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
// 全ての描画
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
		pDebug->Print("現在のオブジェクト数:%d\n", nCnt[nCntPrt]);
	}

	pDebug->Draw();
}

//============================
// 描画順再設定
//============================
void CObject::SetPrity(int nPriority)
{
	//this->GetType();


	//m_nPriority = nPriority;
}

//============================
// 指定オブジェクト取得
//============================
CObject *CObject::GetObject(const int nPriority, const int nIdx)
{
	return m_apObject[nPriority][nIdx];
}
