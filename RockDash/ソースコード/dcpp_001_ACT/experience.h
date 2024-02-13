//===============================================
//
// 経験値オーブ(experience.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _EXPERIENCE_H_		//このマクロ定義がされてなかったら
#define _EXPERIENCE_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "effect.h"

//=========================
// マクロ定義
//=========================
#define EXP_SIZE D3DXVECTOR3(10.0f,10.0f,10.0f)	//経験値オーブのデフォルトサイズ
#define EXP_VALUE (1.0f)	//経験値オーブのデフォルト値

//=========================
// 前方宣言
//=========================

//=========================
// プレイヤークラス
//=========================
class CExperience : public CEffect
{
public:

	CExperience(int nPriority = 5);		//コンストラクタ
	~CExperience();		//デストラクタ

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos);		//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	static CExperience *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size = EXP_SIZE, const float fRat = EXP_VALUE);		//生成
	bool CollisionBall(void);		//当たり判定
	void SetValue(float fValue) { m_fValue = fValue; }		//経験値設定
	float GetValue() { return m_fValue; }					//経験値取得

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;				//共通テクスチャバッファ
	float m_fValue;			//経験値
};

#endif
