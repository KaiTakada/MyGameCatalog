//===============================================
//
// 数字(number.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _ITEM2D_H_		//このマクロ定義がされてなかったら
#define _ITEM2D_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "object2D.h"

//=========================
// マクロ定義
//=========================
#define ITEM2D_SIZE (D3DXVECTOR3(100.0f, 100.0f, 0.0f))		//デフォルトサイズ

//=========================
// 前方宣言
//=========================
//class CObject2D;

//=========================
// プレイヤークラス
//=========================
class CItem2D : public CObject2D
{
public:

	enum TYPE
	{
		TYPE_000_HEAL = 0,			//回復(固定値)
		TYPE_001_ATTACK,			//攻撃力up(近距離)
		TYPE_002_SPEED,				//移動速度up(近距離)
		TYPE_003_COOLTIME,			//連射力up(銃)
		TYPE_MAX
	};

	CItem2D();		//コンストラクタ
	~CItem2D();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CItem2D *Create(const D3DXVECTOR3 pos, const TYPE type, const D3DXVECTOR3 size = ITEM2D_SIZE);		//生成
	void SetType(TYPE type);						//種類設定
	CItem2D::TYPE GetType() { return m_type; }		//種類取得
	void PlayerAdd();

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;				//共通テクスチャバッファ
	CItem2D::TYPE m_type;		//表示する数字値
};

#endif
