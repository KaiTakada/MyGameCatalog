//===============================================
//
// シーン(scene.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _SCENE_H_		//このマクロ定義がされてなかったら
#define _SCENE_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"
#include "renderer.h"
#include "constant.h"

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
		MODE_RESULT,
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

	static CNumber *GetNumber(void) { return m_pNumber; }								//数字の取得
	static CScore *GetScore(void) { return m_pScore; }									//スコアの取得

	static CObject3D *GetObject3D(void) { return m_pObject3D; }							//3Dポリゴンの取得
	static CField *GetField(void) { return m_pField; }									//地面の取得

	static CObjectBillboard *GetBillBoard(void) { return m_pBillBoard; }				//ビルボードの取得
	static CItem *GetItem(void) { return m_pItem; }										//アイテムの取得
	static CPlayer **GetElem(void) { return &m_pPlayer[0]; }						//プレイヤー全体の取得
	static CPlayer *GetElem(int nNum) { return m_pPlayer[nNum]; }						//特定のプレイヤーの取得
	static void SetPlayer(CPlayer *pPlayer, int nIdx) { m_pPlayer[nIdx] = pPlayer; }						//特定のプレイヤーの設定

	static CBlock *GetBlock(void) { return m_pBlock; }									//ブロックの取得

	static int GetNowTime(void) { return m_nTime; }									//今回プレイ結果の取得
	static void SetNowTime(int nScore) { m_nTime = nScore; }							//今回プレイ結果の取得
	static int GetNumPlayer(void) { return m_nNumPlayer; }									//プレイヤー人数の取得
	static void SetNumPlayer(int nNumPlayer) { m_nNumPlayer = nNumPlayer; }					//プレイヤー人数の取得

protected:

private:
	MODE m_mode;		//現在のモード

	static CNumber *m_pNumber;							//数字
	static CScore *m_pScore;							//スコア
	static CFade *m_pFade;								//フェード

	static CObject3D *m_pObject3D;						//3Dポリゴン
	static CField *m_pField;							//地面

	static CObjectBillboard *m_pBillBoard;				//ビルボード
	static CItem *m_pItem;								//アイテム
	static CPlayer *m_pPlayer[mylib_const::MAX_PLAYER];					//プレイヤー

	static CBlock *m_pBlock;							//ブロック
	static int m_nTime;								//今回プレイ結果
	static int m_nNumPlayer;						//プレイヤー人数
};

#endif
