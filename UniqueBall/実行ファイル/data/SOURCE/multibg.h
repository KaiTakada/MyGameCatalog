//===============================================
//
// マルチ背景(multibg.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _MULTIBG_H_		//このマクロ定義がされてなかったら
#define _MULTIBG_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "manager.h"
#include "object.h"

//=========================
// マクロ定義
//=========================
#define MAX_MULTIBG (16)		//総数
//#define MAX_MULTIBG (128)		//総数

//=========================
// 前方宣言
//=========================
class CObject2D;

//=========================
// オブジェクト2Dクラス
//=========================
class CMultiBg : public CObject
{
public:

	CMultiBg();		//コンストラクタ
	~CMultiBg();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR2 move, const D3DXVECTOR3 size, int nNum);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CMultiBg *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR2 move, const D3DXVECTOR3 size,int nNum);		//生成
	static void Release(int nID);							//破棄

	void SetPos(const D3DXVECTOR3 pos);			//位置設定
	void SetRot(const D3DXVECTOR3 rot);			//向き設定
	D3DXVECTOR3 GetPos(void);		//位置取得
	D3DXVECTOR3 GetSize(void);		//2Dサイズ取得
	bool GetJump(void) { return false; }		//ジャンプ判定
	D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//前回位置取得

protected:

private:
	CObject2D *m_apObject[MAX_MULTIBG];			//アドレス＆情報取得用
};

#endif