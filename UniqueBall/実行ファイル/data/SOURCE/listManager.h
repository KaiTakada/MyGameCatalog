//===============================================
//
// リストマネージャ(listManager.h)
// Author: Kai Takada
//
//===============================================
#ifndef _LISTMANAGER_H_		//このマクロ定義がされてなかったら
#define _LISTMANAGER_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================

//=========================
// 前方宣言
//=========================

//=========================
// マネージャクラス
//=========================
template <class T> class CListManager
{
public:

	using Iterator = typename std::list<T*>::iterator;

	CListManager();			// コンストラクタ
	~CListManager();		// デストラクタ

	typename CListManager<T>::Iterator GetBegin();
	typename CListManager<T>::Iterator GetEnd();

	void Add(T* pList);						// 要素追加
	void Remove(T* pList);					// 要素消去
	void Clear() { m_pList.clear(); }		// 要素全消去
	bool ListLoop(Iterator& itr);			// リストループ処理
	int GetNumAll() { m_pList->size(); }	// 総数取得

	T* GetElem(int nIdx);					// 要素取得
	std::list<T*> GetList() { return m_pList; }

protected:

private:
	std::list<T*> m_pList;		//リスト
};

#endif

//==========================================================================
// コンストラクタ
//==========================================================================
template<class T>
CListManager<T>::CListManager()
{
	m_pList.clear();	// アイテムのリスト
}

//==========================================================================
// デストラクタ
//==========================================================================
template<class T>
CListManager<T>::~CListManager()
{
	m_pList.clear();
}

//============================
// イテレーター取得
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
// 消去処理
//============================
template <class T>
void CListManager<T>::Remove(T* pList)
{
	int i = 0;

	// 各要素に対して操作を行う
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
// 要素追加
//======================================
template <class T>
void CListManager<T>::Add(T* pList)
{
	m_pList.push_back(pList);              // 末尾に追加

	m_nNumAll++;
}


//======================================
// 要素取得
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
// リストループ処理
//==========================================================================
template<class T>
bool CListManager<T>::ListLoop(Iterator& itr)
{
	if (m_pList.empty())
	{// 空の場合即終了
		return false;
	}

#if 0
	if (*itr == nullptr)
	{// 先頭

		auto begin = m_pList.begin();
		(*itr) = &begin;
	}
	else
	{
		// 終端のイテレーターを取得
		Iterator enditr = m_pList.end();
		enditr = std::prev(enditr);

		if (*(*itr) == enditr)
		{// 終端だったら終了
			return false;
		}

		*(*itr)++;
	}

	return (itr != nullptr);	// nullptrで終了

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
