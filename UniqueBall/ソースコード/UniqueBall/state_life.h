//===============================================
//
// 生命状態管理(state_life.cpp)
// Author: 髙田 佳依
//
//===============================================
#ifndef _STATE_LIFE_H_								//このマクロ定義がされてなかったら
#define _STATE_LIFE_H_								//二重インクルード防止のマクロを定義する

//================================
// インクルードファイル
//================================

//================================
// マクロ定義
//================================

//================================
// モデルクラス
//================================
class CState_Life
{
public:
	enum STATE
	{
		STATE_NORMAL = 0,
		STATE_DAMAGE,
		STATE_GOD,
		STATE_DEATH,
		STATE_MAX,
	};

	CState_Life();
	~CState_Life();

	HRESULT Init();
	void Uninit();
	void Update();
	static CState_Life *Create();

	void SetState(const STATE state, const int nCT) { m_state = state; m_nCtrState = nCT; }		//設定
	STATE GetState(void) { return m_state; }		//現在位置取得
	int GetCnt(void) { return m_nCtrState; }		//残りカウント取得
	D3DXMATERIAL GetMat(void) { return m_MatDmg; }		//残りカウント取得

protected:

private:
	STATE m_state;						//状態
	int m_nCtrState;					//状態カウンター
	D3DXMATERIAL m_MatDmg;					//ダメージ色
	CState_Life *m_pAdress;				//受け皿
};

#endif
