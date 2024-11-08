//===============================================
//
// Xモデルオブジェクトパーツ(Parts.cpp)	//親モデルを持つモデル
// Author: 髙田 佳依
//
//===============================================
#ifndef _PARTS_H_								//このマクロ定義がされてなかったら
#define _PARTS_H_								//二重インクルード防止のマクロを定義する

//================================
// インクルードファイル
//================================
#include "manager.h"
#include "object.h"

//================================
// マクロ定義
//================================
namespace my_parts
{
	const int NUM_TEX_PASS = 4;		//差し替えテクスチャ数
}

//================================
// モデルクラス
//================================
class CParts : public CObject
{
public:
	//=============================
	// パーツ情報構造体の定義
	//=============================
	typedef struct
	{
		char *pFilepass;		//ファイルパス
		int nIdxParent;			//親のインデックス
	}PassParent;

	CParts(int nPriority = 3);
	~CParts();

	HRESULT Init(void);							//初期化(デフォ)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);			//初期化(オバロ)
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画

	static CParts *Create(const char *pFilename, const D3DXVECTOR3 pos = mylib_const::DEFVEC3, const D3DXVECTOR3 rot = mylib_const::DEFVEC3);		//生成
	void SetParent(CParts *pParts);
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }						//現在位置設定
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }						//現在向き設定
	void SetScale(const float fScale) { m_scale = D3DXVECTOR3(fScale, fScale, fScale); }						//サイズ設定
	D3DXVECTOR3 GetScale() { return m_scale; }						//サイズ取得
	void SetMultiply(const bool bMultipy) { m_bMultiply = bMultipy; }		//掛け合わせフラグ設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }				//現在位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }				//現在向き取得
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }		//ワールドマトリックス取得
	bool GetMultiply(void) { return m_bMultiply; }			//掛け合わせフラグ取得
	D3DXVECTOR3 GetWorldPos(void) { return D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43); }		//ワールド座標取得

	void SetIdxModel(int nIdx) { m_nIdxModel = nIdx; }		//モデル番号の割り当て
	int GetIdxModel() { return m_nIdxModel; }				//モデル番号の取得
	D3DXVECTOR3 GetOutcomePos(CParts *pParts);				//合計座標
	CParts *GetParent() { return m_pParent; }				//親モデルポインタ取得
	void SetMat(D3DXMATERIAL mat) { m_mat = mat; }			//色設定
	std::string *GetTexPass() { return m_sTexPass; }				//差し替えテクスチャ取得
	void SetTexPass(std::string sTexPass, int nIdx);			//差し替えテクスチャ設定
	int GetIdxParent() { return m_nIdxParent; }				//差し替え番号取得
	void SetIdxParent(int nIdx) { m_nIdxParent = nIdx; }			//差し替え番号設定

protected:

private:
	D3DXMATRIX m_mtxWorld;			//現在位置
	D3DXVECTOR3 m_pos;			//現在位置
	D3DXVECTOR3 m_rot;			//現在向き
	D3DXVECTOR3 m_scale;		//現在スケール(デフォルト1.0f)
	CParts *m_pParent;			//親モデルへのポインタ
	int m_nIdxModel;			//モデル番号
	D3DXMATERIAL m_mat;			//指定色
	bool m_bMultiply;		//親or最新と掛け合わせるフラグ(false:合わせる)
	int m_nIdxParent;		//親が番号持ちだったら
	std::string m_sTexPass[my_parts::NUM_TEX_PASS];		//差し替えテクスチャ
	LPDIRECT3DTEXTURE9 m_pTexture[my_parts::NUM_TEX_PASS];		//差し替えテクスチャ
};

#endif
