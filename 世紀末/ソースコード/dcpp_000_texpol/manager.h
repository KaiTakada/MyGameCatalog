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

//=========================
// 前方宣言
//=========================
class CRenderer;
class CTexture;
class CXModel;
class CCamera;
class CLight;
class CSound;
class CSpawn;
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
// 画面(シーン)クラス
//=========================
class CScene
{
public:
	enum MODE
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RANKING,
		MODE_MAX
	};

	CScene();
	~CScene();

	virtual HRESULT Init();		//初期化
	virtual void Uninit();		//終了
	virtual void Update();		//更新
	virtual void Draw();		//描画
	static CScene *Create(MODE mode);		//生成
	CScene::MODE GetMode() { return m_mode; }		//現在のモード取得
	static CFade *GetFade(void) { return m_pFade; }									//フェードの得

	static CBg *GetBg(void) { return m_pBg; }											//背景の取得
	static CMultiBg *GetMultiBg(void) { return m_pMultiBg; }							//多重背景の取得
	static CEffect *GetEffect(void) { return m_pEffect; }								//エフェクトの取得
	static CParticle *GetParticle(void) { return m_pParticle; }							//パーティクルの取得
	static CNumber *GetNumber(void) { return m_pNumber; }								//数字の取得
	static CScore *GetScore(void) { return m_pScore; }									//スコアの取得

	static CObject3D *GetObject3D(void) { return m_pObject3D; }							//3Dポリゴンの取得
	static CField *GetField(void) { return m_pField; }									//地面の取得

	static CObjectBillboard *GetBillBoard(void) { return m_pBillBoard; }				//ビルボードの取得
	static CItem *GetItem(void) { return m_pItem; }										//アイテムの取得
	static CBullet *GetBullet(void) { return m_pBullet; }								//弾の取得
	static CExplosion *GetExplosion(void) { return m_pExplosion; }						//爆発の取得

	static CObjectX *GetObjectX(void) { return m_pObjectX; }							//モデルの取得
	static CEnemy *GetEnemy(void) { return m_pEnemy; }									//敵の取得
	static CBlock *GetBlock(void) { return m_pBlock; }									//ブロックの取得

	static int GetNowScore(void) { return m_nScore; }									//今回プレイ結果の取得
	static void SetNowScore(int nScore) { m_nScore = nScore; }							//今回プレイ結果の取得

protected:

private:
	MODE m_mode;		//現在のモード

	static CBg *m_pBg;									//背景
	static CMultiBg *m_pMultiBg;						//多重背景
	static CNumber *m_pNumber;							//数字
	static CScore *m_pScore;							//スコア
	static CFade *m_pFade;								//フェード

	static CObject3D *m_pObject3D;						//3Dポリゴン
	static CField *m_pField;							//地面

	static CObjectBillboard *m_pBillBoard;				//ビルボード
	static CItem *m_pItem;								//アイテム
	static CEffect *m_pEffect;							//エフェクト
	static CParticle *m_pParticle;						//パーティクル
	static CBullet *m_pBullet;							//弾
	static CExplosion *m_pExplosion;					//爆発

	static CObjectX *m_pObjectX;						//モデル
	static CEnemy *m_pEnemy;							//敵
	static CBlock *m_pBlock;							//ブロック
	static int m_nScore;								//今回プレイ結果
};

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
	static void SetMode(CScene::MODE mode);							//シーン設定

	static CRenderer *GetRenderer(void) { return m_pRenderer; }							//レンダラーの取得
	static CTexture *GetTexture(void) { return m_pTeture; }								//テクスチャの取得
	static CXModel *GetXModel(void) { return m_pXModel; }								//Xファイルモデルの取得
	static CCamera *GetCamera(void) { return m_pCamera; }								//カメラの取得
	static CLight *GetLight(void) { return m_pLight; }									//ライトの取得
	static CSound *GetSound(void) { return m_pSound; }									//サウンドの取得
	static CSpawn *GetSpawn(void) { return m_pSpawn; }									//敵スポーンの取得

	static CInputKeyboard *GetInputKeyboard(void) { return m_pCInputKeyboard; }			//入力情報の取得(キーボード)
	static CInputGamepad *GetInputGamepad(void) { return m_pCInputGamepad; }			//入力情報の取得(Pad [X])
	static CInputMouse *GetInputMouse(void) { return m_pCInputMouse; }					//入力情報の取得(マウス)

	//static CBg *GetBg(void) { return m_pBg; }											//背景の取得
	//static CMultiBg *GetMultiBg(void) { return m_pMultiBg; }							//多重背景の取得
	//static CEffect *GetEffect(void) { return m_pEffect; }								//エフェクトの取得
	//static CParticle *GetParticle(void) { return m_pParticle; }							//パーティクルの取得
	//static CNumber *GetNumber(void) { return m_pNumber; }								//数字の取得
	//static CScore *GetScore(void) { return m_pScore; }									//スコアの取得

	//static CObject3D *GetObject3D(void) { return m_pObject3D; }							//3Dポリゴンの取得
	//static CField *GetField(void) { return m_pField; }									//地面の取得

	//static CObjectBillboard *GetBillBoard(void) { return m_pBillBoard; }				//ビルボードの取得
	//static CItem *GetItem(void) { return m_pItem; }										//アイテムの取得
	//static CBullet *GetBullet(void) { return m_pBullet; }								//弾の取得
	//static CExplosion *GetExplosion(void) { return m_pExplosion; }						//爆発の取得

	//static CObjectX *GetObjectX(void) { return m_pObjectX; }								//モデルの取得
	//static CPlayer *GetPlayer(void) { return m_pPlayer; }								//プレイヤーの取得
	//static CEnemy *GetEnemy(void) { return m_pEnemy; }									//敵の取得
	//static CBlock *GetBlock(void) { return m_pBlock; }									//ブロックの取得

	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; }				//デバッグの取得
	static CScene *GetScene(void) { return m_pScene; }							//シーンの取得

	static void SetPause(bool bPause) { m_bPause = bPause; }					//ポーズ状態設定
	static bool GetPause(void) { return m_bPause; }								//ポーズ状態取得
	static void InvPause(void) { m_bPause = m_bPause ? false : true; }			//ポーズ切り替え

	static void SetGrow(bool bGrow) { m_bGrow = bGrow; }						//進化シーン状態設定
	static bool GetGrow(void) { return m_bGrow; }								//進化シーン状態取得
	static void InvGrow(void) { m_bGrow = m_bGrow ? false : true; }				//進化シーン切り替え

	static void SetResult(RESULT_TYPE result) { m_result = result; }			//勝敗状態設定
	static RESULT_TYPE GetResult(void) { return m_result; }						//勝敗状態取得

protected:

private:
	static CRenderer *m_pRenderer;						//レンダラー
	static CTexture *m_pTeture;							//テクスチャ
	static CXModel *m_pXModel;							//Xファイルモデル
	static CCamera *m_pCamera;							//カメラ
	static CLight *m_pLight;							//ライト
	static CSound *m_pSound;							//サウンド
	static CSpawn *m_pSpawn;							//敵スポーン

	static CInputKeyboard *m_pCInputKeyboard;			//入力(キーボード)
	static CInputGamepad *m_pCInputGamepad;				//入力(Pad[X])
	static CInputMouse *m_pCInputMouse;					//入力(マウス)

	static CDebugProc *m_pDebugProc;					//デバッグ
	static CScene *m_pScene;							//シーン

	static bool m_bPause;				//ポーズフラグ(※ゲームシーンで使用※Renderer干渉)
	static bool m_bGrow;				//進化シーンフラグ(※ゲームシーンで使用※Renderer干渉)
	static RESULT_TYPE m_result;		//勝敗フラグ(※ゲーム→リザルトシーンで使用)
};

#endif
