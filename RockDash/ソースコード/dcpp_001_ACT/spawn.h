//===============================================
//
// スポーン(spawn.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _SPAWN_H_		//このマクロ定義がされてなかったら
#define _SPAWN_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "object.h"
#include "enemy.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================

//=========================
// スポーンクラス
//=========================
class CSpawn : public CObject
{
public:
	struct Generate		//生成要素
	{
		int nFrequency;		//生成頻度
		int nNumMin;			//生成数最小
		int nNumMax;			//生成数最大
	};

	CSpawn();		//コンストラクタ
	~CSpawn();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CSpawn *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);		//生成

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }				//位置設定
	void SetSize(const D3DXVECTOR3 size) { m_size = size; }			//サイズ設定
	void SetRad(const float fRad) { m_fRad = fRad; }				//半径設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }			//位置取得
	D3DXVECTOR3 GetSize(void) { return m_size; }		//サイズ取得
	float GetRad(void) { return m_fRad; }				//半径取得
	void GenerateEnemy();									//生成関数
	void FileRead();									//ファイル読込

protected:

private:
	D3DXVECTOR3 m_pos;		//現在座標
	D3DXVECTOR3 m_size;		//スポーン範囲
	float m_fRad;			//スポーン半径
	float m_fLegScr;		//画面上の対角線の長さ
	int m_nCtr;				//カウンター
	int m_nEneGrowCtr;		//敵強化カウンター

	bool m_bDebSpawn;		//生成状態
	Generate m_aGenerate[CEnemy::ET_MAX];				//生成情報
};

#endif