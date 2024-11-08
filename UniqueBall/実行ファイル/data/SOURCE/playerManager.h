//===============================================
//
// プレイヤーマネージャ(playerManager.h)
// Author: Kai Takada
//
//===============================================
#ifndef _PLAYER_MANAGER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_MANAGER_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================

//=========================
// 前方宣言
//=========================
class CPlayer;

//=========================
// マネージャクラス
//=========================
class CPlayerManager
{
public:

	CPlayerManager();			//コンストラクタ
	~CPlayerManager();		//デストラクタ

	HRESULT Init();		//初期化
	void Uninit();												//終了
	void Update();												//更新

	static CPlayerManager* GetInstance();						//インスタンス取得
	static void Release();						//リリース
	void Add(CPlayer* pPlayer);						//要素追加

	CPlayer* GetElem(int nIdx);

protected:

private:
	static CPlayerManager* m_pManager;		//インスタンス保持

	std::list<CPlayer*> m_pPlayer;		//プレイヤー
	//CPlayer* m_pPlayer;					//プレイヤー
};

#endif
