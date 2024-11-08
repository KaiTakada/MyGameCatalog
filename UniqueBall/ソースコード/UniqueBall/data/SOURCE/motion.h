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
#define MAX_PARTS (32)		//最大パーツ数
#define NUM_MOTION (32)				//モーション数
#define NUM_KEY (32)					//キー数

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
	struct KEY
	{
		float fPosX;		//位置X
		float fPosY;		//位置Y
		float fPosZ;		//位置Z
		float fRotX;		//位置X
		float fRotY;		//位置Y
		float fRotZ;		//位置Z

		KEY(): fPosX(0.0f), fPosY(0.0f) , fPosZ(0.0f) , fRotX(0.0f) , fRotY(0.0f) , fRotZ(0.0f) {};
	};

	//====================
	// キー情報の構造体
	//====================
	struct KEY_INFO
	{
		int nFrame;			//再生フレーム
		KEY aKey[MAX_PARTS];		//各モデルのキー要素
	};

	//====================
	// モーション情報の構造体
	//====================
	struct INFO
	{
		bool bLoop;				//ループするかどうか
		int nNumKey;			//キーの総数
		KEY_INFO aKeyInfo[NUM_KEY];	//キー情報
	};

	CMotion();		//コンストラクタ
	~CMotion();		//デストラクタ

	HRESULT Init(void);						// 初期化(デフォ)
	void Uninit(void);						// 終了
	void Update(void);						// 更新
	void Set(int nType);															// モーションの起動
	void Set(int nType, int nBrendCtr) { m_nBrendCtr = nBrendCtr; Set(nType); }		// モーションの起動(ブレンド)

	// 更新
	void UpdateParts(int nNextKey);		// パーツの更新

	int GetType(void);						// 現在のモーションの種類取得
	bool IsFinish(void);					// 終了判定取得
	void SetInfo(INFO *info, int nNum);				// モーション情報設定

	void SetModel(CParts **ppParts, int nNumModel);		// モデル情報設定
	//void SetOrg(CParts **ppParts, int nNumModel);		// オフセット情報設定

	char **ReadParts(const char *pReadFile, int *pnNumFile);
	void ReadSetUp(const char *pReadFile, CParts **ppParts);
	void ReadMotions(const char *pReadFile, CMotion::INFO *pInfo);

	int GetKey(void) { return m_nKey; }				// 現在のキー取得
	int GetKeyOld(void) { return m_nKeyOld; }		// 前回のキー取得
	INFO *GetInfo(void) { return m_aInfo; }			// 現在のキー情報取得

	int GetCounter() { return m_nCounter; }			// 現在カウント取得
	int GetTotalCtr() { return m_nTotalCtr; }		// トータルカウント取得
	
protected:

private:
	void ReadFile(char* pFilePass);	// ファイル読込
	void KeySetBrend();			// キー設定(ブレンド)
	void KeySetNormal();		// キー設定(キー更新)

	KEY m_aKeyOrg[MAX_PARTS];	// 各モデルのプリセット情報
	KEY m_aKeyOld[MAX_PARTS];	// 各モデルのプリセット情報(過去)
	INFO m_aInfo[NUM_MOTION];	// モーション情報
	int m_nNumAll;				// モーションの総数
	int m_nType;				// モーションの種類
	int m_nTypeOld;				// モーションの種類(前回)
	int m_nCounter;				// モーションカウンター
	int m_nNumKey;				// キーの総数
	int m_nKey;					// 現在のキーNo.
	int m_nKeyOld;				// 前回のキーNo.
	bool m_bLoop;				// ループするかどうか
	bool m_bFinish;				// 終了したかどうか
	int m_nTotalCtr;			// 1つのトータルカウンター
	int m_nBrendCtr;			// ブレンドカウンター

	CParts **m_ppParts;	// モデルへのポインタ
	int m_nNumModel;	// モデルの総数
};

#endif
