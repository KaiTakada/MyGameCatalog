//===============================================
//
// ���X�g�}�l�[�W��(listManager.h)
// Author: Kai Takada
//
//===============================================
#ifndef _LISTMANAGER_H_		//���̃}�N����`������ĂȂ�������
#define _LISTMANAGER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=========================
// �C���N���[�h�t�@�C��
//=========================

//=========================
// �O���錾
//=========================

//=========================
// �}�l�[�W���N���X
//=========================
template <class T> class CListManager
{
public:

	using Iterator = typename std::list<T*>::iterator;

	CListManager();			// �R���X�g���N�^
	~CListManager();		// �f�X�g���N�^

	typename CListManager<T>::Iterator GetBegin();
	typename CListManager<T>::Iterator GetEnd();

	void Add(T* pList);						// �v�f�ǉ�
	void Remove(T* pList);					// �v�f����
	void Clear() { m_pList.clear(); }		// �v�f�S����
	bool ListLoop(Iterator& itr);			// ���X�g���[�v����
	int GetNumAll() { m_pList->size(); }	// �����擾

	T* GetElem(int nIdx);					// �v�f�擾
	std::list<T*> GetList() { return m_pList; }

protected:

private:
	std::list<T*> m_pList;		//���X�g
};

#endif

//==========================================================================
// �R���X�g���N�^
//==========================================================================
template<class T>
CListManager<T>::CListManager()
{
	m_pList.clear();	// �A�C�e���̃��X�g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
template<class T>
CListManager<T>::~CListManager()
{
	m_pList.clear();
}

//============================
// �C�e���[�^�[�擾
//============================
template<class T>
typename CListManager<T>::Iterator CListManager<T>::GetBegin()
{
	return m_pList.begin();
}

template<class T>
typename CListManager<T>::Iterator CListManager<T>::GetEnd()
{
	return m_pList.end();
}

//============================
// ��������
//============================
template <class T>
void CListManager<T>::Remove(T* pList)
{
	int i = 0;

	// �e�v�f�ɑ΂��đ�����s��
	for (auto itr = m_pList.begin(); itr != m_pList.end();)
	{
		T* player = *std::next(m_pList.begin(), i);

		if (player == pList)
		{
			itr = m_pList.erase(std::next(m_pList.begin(), i));
			m_nNumAll--;
			return;
		}
		else
		{
			itr++;
			i++;
		}
	}
}

//======================================
// �v�f�ǉ�
//======================================
template <class T>
void CListManager<T>::Add(T* pList)
{
	m_pList.push_back(pList);              // �����ɒǉ�

	m_nNumAll++;
}


//======================================
// �v�f�擾
//======================================
template <class T>
T* CListManager<T>::GetElem(int nIdx)
{
	T* player = nullptr;

	if (nIdx < static_cast<int>(m_pList.size()))
	{
		player = *std::next(m_pList.begin(), nIdx);
	}

	return player;
}

//==========================================================================
// ���X�g���[�v����
//==========================================================================
template<class T>
bool CListManager<T>::ListLoop(Iterator& itr)
{
	if (m_pList.empty())
	{// ��̏ꍇ���I��
		return false;
	}

#if 0
	if (*itr == nullptr)
	{// �擪

		auto begin = m_pList.begin();
		(*itr) = &begin;
	}
	else
	{
		// �I�[�̃C�e���[�^�[���擾
		Iterator enditr = m_pList.end();
		enditr = std::prev(enditr);

		if (*(*itr) == enditr)
		{// �I�[��������I��
			return false;
		}

		*(*itr)++;
	}

	return (itr != nullptr);	// nullptr�ŏI��

#else

	if (itr == m_pList.end())
	{
		itr = m_pList.begin();
	}
	else
	{
		itr++;
	}

	return (itr != m_pList.end());

#endif
}
