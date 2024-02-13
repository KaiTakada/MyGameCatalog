//===============================================
//
// モーション(motion.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _MOTION_H_		//このマクロ定義がされてなかったら
#define _MOTION_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
//#include "objectX.h"

//=========================
// マクロ定義
//=========================
#define MAX_PLAYER_PARTS (32)		//最大パーツ数
#define NUM_MOTION (8)				//モーション数
#define NUM_KEY (8)					//キー数

//=========================
// 前方宣言
//=========================
class CParts;

//=========================
// プレイヤークラス
//=========================
class CMotion
{
public:
	//====================
	// キーの構造体
	//====================
	typedef struct
	{
		float fPosX;		//位置X
		float fPosY;		//位置Y
		float fPosZ;		//位置Z
		float fRotX;		//位置X
		float fRotY;		//位置Y
		float fRotZ;		//位置Z
	}KEY;

	//====================
	// キー情報の構造体
	//====================
	typedef struct
	{
		int nFrame;			//再生フレーム
		KEY aKey[MAX_PLAYER_PARTS];		//各モデルのキー要素
	}KEY_INFO;

	//====================
	// モーション情報の構造体
	//====================
	typedef struct
	{
		bool bLoop;				//ループするかどうか
		int nNumKey;			//キーの総数
		KEY_INFO aKeyInfo[NUM_KEY];	//キー情報
	}INFO;

	CMotion();		//コンストラクタ
	~CMotion();		//デストラクタ

	HRESULT Init(void);						//初期化(デフォ)
	void Uninit(void);						//終了
	void Set(int nType);					//モーションの起動
	void Update(void);						//終了

	int GetType(void);						//現在のモーションの種類取得
	bool IsFinish(void);					//終了判定取得
	void SetInfo(INFO *info, int nNum);				//モーション情報設定

	void SetModel(CParts **ppParts, int nNumModel);		//モデル情報設定
	//void SetOrg(CParts **ppParts, int nNumModel);		//オフセット情報設定

	int GetKey(void) { return m_nKey; }		//現在のキー取得

protected:

private:
	void ReadFile(char *pFilePass);	//ファイル読込

	KEY m_aKeyOrg[MAX_PLAYER_PARTS];		//各モデルのプリセット情報
	INFO m_aInfo[NUM_MOTION];	//モーション情報
	int m_nNumAll;		//モーションの総数
	int m_nType;		//モーションの種類
	int m_nCounter;		//モーションカウンター
	int m_nNumKey;		//キーの総数
	int m_nKey;			//現在のキーNo.
	bool m_bLoop;		//ループするかどうか
	bool m_bFinish;		//終了したかどうか

	CParts **m_ppParts;	//モデルへのポインタ
	int m_nNumModel;	//モデルの総数
};

#endif
