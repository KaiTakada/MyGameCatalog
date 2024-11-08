//===============================================
//
// マネージャ(manager.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _MANAGER_H_		//このマクロ定義がされてなかったら
#define _MANAGER_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"
#include "renderer.h"
#include "scene.h"

//=========================
// 前方宣言
//=========================
class CRenderer;
class CTexture;
class CXModel;
class CCamera;
class CLight;
class CSound;
class CFade;

class CInputKeyboard;
class CInputGamepad;
class CInputMouse;

class CBg;
class CMultiBg;
class CNumber;
class CScore;

class CObject3D;
class CField;

class CObjectBillboard;
class CItem;
class CBullet;
class CExplosion;
class CEffect;
class CParticle;

class CObjectX;
class CBlock;
class CEnemy;
class CPlayer;

class CDebugProc;

//=========================
// マネージャクラス
//=========================
class CManager
{
public:

	//========================
	// リザルト種
	//========================
	enum RESULT_TYPE
	{
		RT_NONE = 0,
		RT_WIN,
		RT_LOSE,
		RT_MAX,
	};

	CManager();			//コンストラクタ
	~CManager();		//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		//初期化
	void Uninit(void);												//終了
	void Update(void);												//更新
	void Draw(void);												//描画
	void SetMode(CScene::MODE mode);							//シーン設定
	
	static CManager *GetInstance();						//インスタンス取得
	static void Release();						//リリース

	CRenderer *GetRenderer(void) { return m_pRenderer; }							//レンダラーの取得
	CTexture *GetTexture(void) { return m_pTeture; }								//テクスチャの取得
	CXModel *GetXModel(void) { return m_pXModel; }								//Xファイルモデルの取得
	CCamera *GetCamera(void) { return m_pCamera; }								//カメラの取得
	CLight *GetLight(void) { return m_pLight; }									//ライトの取得
	CSound *GetSound(void) { return m_pSound; }									//サウンドの取得

	CInputKeyboard *GetInputKeyboard(void) { return m_pCInputKeyboard; }			//入力情報の取得(キーボード)
	CInputGamepad *GetInputGamepad(void) { return m_pCInputGamepad; }			//入力情報の取得(Pad [X])
	CInputMouse *GetInputMouse(void) { return m_pCInputMouse; }					//入力情報の取得(マウス)

	CDebugProc *GetDebugProc(void) { return m_pDebugProc; }				//デバッグの取得
	CScene *GetScene(void) { return m_pScene; }							//シーンの取得

	void SetPause(bool bPause) { m_bPause = bPause; }					//ポーズ状態設定
	bool GetPause(void) { return m_bPause; }								//ポーズ状態取得
	void InvPause(void) { m_bPause = m_bPause ? false : true; }			//ポーズ切り替え

	void SetGrow(bool bGrow) { m_bGrow = bGrow; }						//進化シーン状態設定
	bool GetGrow(void) { return m_bGrow; }								//進化シーン状態取得
	void InvGrow(void) { m_bGrow = m_bGrow ? false : true; }			//進化シーン切り替え

	void SetEdit(bool bEdit) { m_bEdit = bEdit; }						//進化シーン状態設定
	bool GetEdit(void) { return m_bEdit; }								//進化シーン状態取得
	void InvEdit(void) { m_bEdit = m_bEdit ? false : true; }			//進化シーン切り替え

	void SetResult(RESULT_TYPE result) { m_result = result; }			//勝敗状態設定
	RESULT_TYPE GetResult(void) { return m_result; }					//勝敗状態取得
	
	void SetRank(int nRank,int nIdx) { m_anRank[nIdx] = nRank; }			//順位設定
	int GetRank(int nIdx) { return m_anRank[nIdx]; }						//順位取得(1つ)
	int *GetRank() { return m_anRank; }									//順位取得
	void ZeroRank();									//順位取得

protected:

private:
	CRenderer *m_pRenderer;						//レンダラー
	CTexture *m_pTeture;						//テクスチャ
	CXModel *m_pXModel;							//Xファイルモデル
	CCamera *m_pCamera;							//カメラ
	CLight *m_pLight;							//ライト
	CSound *m_pSound;							//サウンド

	CInputKeyboard *m_pCInputKeyboard;			//入力(キーボード)
	CInputGamepad *m_pCInputGamepad;			//入力(Pad[X])
	CInputMouse *m_pCInputMouse;				//入力(マウス)

	CDebugProc *m_pDebugProc;					//デバッグ
	CScene* m_pScene;							//シーン

	bool m_bPause;				//ポーズフラグ(※ゲームシーンで使用※Renderer干渉)
	bool m_bGrow;				//進化シーンフラグ(※ゲームシーンで使用※Renderer干渉)
	bool m_bEdit;				//エディットフラグ(※ゲームシーンで使用※Renderer干渉)
	RESULT_TYPE m_result;		//勝敗フラグ(※ゲーム→リザルトシーンで使用)
	int m_anRank[mylib_const::MAX_PLAYER];	//順位保存(値はプレイヤー番号、順位は箱番号

	static CManager *m_pManager;		//インスタンス保持
};

#endif
