//===============================================
//
// �|���S��(object.cpp)
// Author ���c ����
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
// �ÓI�����o�ϐ��錾
//============================
CObject *CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT];		//�I�u�W�F�N�g�C���X�^���X
int CObject::m_nNumAll;		//�I�u�W�F�N�g����

////============================
//// �R���X�g���N�^
////============================
//CObject::CObject()
//{
//	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
//	{
//		for (int nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
//		{
//			if (m_apObject[nCntPrt][nCntAll] == nullptr)
//			{
//				m_apObject[nCntPrt][nCntAll] = this;		//���g����
//				m_nID = nCntAll;				//���g��ID��ۑ�
//				m_nNumAll++;					//�������J�E���g�A�b�v
//				break;
//			}
//		}
//	}
//}

//============================
// �R���X�g���N�^(�`�揇�I�o��)
//============================
CObject::CObject(int nPriority)
{
	int nCntAll = 0;

	for (nCntAll = 0; nCntAll < MAX_OBJECT; nCntAll++)
	{
		if (m_apObject[nPriority][nCntAll] == nullptr)
		{
			m_apObject[nPriority][nCntAll] = this;		//���g����
			m_nID = nCntAll;				//���g��ID��ۑ�
			m_nPriority = nPriority;		//���g�̕`��D��x��ۑ�
			m_bDeath = false;				//���S�t���O��������
			m_nNumAll++;					//�������J�E���g�A�b�v
			break;
		}
	}

	//if (nCntAll >= MAX_OBJECT)
	//{
	//	return nullptr;
	//}
}
//============================
// �f�X�g���N�^
//============================
CObject::~CObject()
{

}

//============================
// �I�u�W�F�N�g�̔j��
//============================
void CObject::Release(void)
{
	int nIdx = m_nID;
	int nPrity = m_nPriority;

	if (m_apObject[nPrity][nIdx] != nullptr)
	{
		//���g�j��
		delete m_apObject[nPrity][nIdx];
		m_apObject[nPrity][nIdx] = nullptr;

		//�����_�E��
		m_nNumAll--;
	}
}

//============================
// �^�C�v�Ŕj��
//============================
void CObject::Reset(void)
{
	//�S����
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
// �S�Ă̔j��
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
// �S�Ă̍X�V
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
// �S�Ă̕`��
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
		pDebug->Print("���݂̃I�u�W�F�N�g��:%d\n", nCnt[nCntPrt]);
	}

	pDebug->Draw();
}

//============================
// �`�揇�Đݒ�
//============================
void CObject::SetPrity(int nPriority)
{
	//this->GetType();


	//m_nPriority = nPriority;
}

//============================
// �w��I�u�W�F�N�g�擾
//============================
CObject *CObject::GetObject(const int nPriority, const int nIdx)
{
	return m_apObject[nPriority][nIdx];
}
