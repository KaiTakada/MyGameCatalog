//===============================================
//
// 体力UI(ui_life.cpp)
// Author 髙田 佳依
//
//===============================================
#ifndef _UI_LIFE_H_		//このマクロ定義がされてなかったら
#define _UI_LIFE_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "object.h"

//=========================
// マクロ定義
//=========================
namespace myUI_life
{
	const int MAX_NUM_LIFE = 10;	//配列最大数
}

//=========================
// 前方宣言
//=========================
class CScore;
class CObject2D;

//=========================
// オブジェクト2D背景クラス
//=========================
class CUI_Life : public CObject
{
public:
	CUI_Life();		//コンストラクタ
	~CUI_Life();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CUI_Life *Create(void);		//生成
	static CUI_Life *Create(const int nNumMark, const int nIdx);		//生成

	void SetPos(const D3DXVECTOR3 pos) override;			//位置設定
	D3DXVECTOR3 GetPos() { return m_pos; }		//位置取得

	void SetNumMark(const int nNumMark) { m_nNumMark = nNumMark; }			//位置設定
	int GetNumMark() { return m_nNumMark; }		//位置取得

	void SetIdx(const int nIdx) { m_nIdx = nIdx; }			//番号設定
	int GetIdx() { return m_nIdx; }		//番号取得

	void SetMark(CObject2D* pMark, int nIdx) { m_apMark[nIdx] = pMark; }			//位置設定
	CObject2D **GetMark() { return m_apMark; }		//位置取得
	CObject2D* GetMark(int nIdx) { return m_apMark[nIdx]; }		//位置取得

	void SetColAll(D3DXCOLOR col);		//全ての色変え

	void AddLife(const int nIdx);			//増加
	void SubLife(const int nIdx);			//減少

protected:

private:
	CObject2D *m_apMark[myUI_life::MAX_NUM_LIFE];		//スコア情報
	D3DXVECTOR3 m_pos;	//位置
	int m_nNumMark;	//マーク数
	int m_nIdx;	//映るカメラ番号
};

#endif