//===============================================
//
// Xモデルオブジェクト(Xmodel.cpp)
// Author: 髙田 佳依
//
//===============================================
#ifndef _XMODEL_H_								//このマクロ定義がされてなかったら
#define _XMODEL_H_								//二重インクルード防止のマクロを定義する

//================================
// インクルードファイル
//================================
#include "object.h"

//================================
// マクロ定義
//================================
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//デフォルトサイズ
#define MAX_XMODEL (256)				//モデルの受け皿容量
#define XMODEL_SIZE_META (0.1f)				//モデルの出力倍率

//================================
// モデルクラス
//================================
class CXModel
{
public:
	struct Model
	{
		D3DXMATRIX mtxWorld;				//ワールドマトリックス
		LPD3DXMESH pMesh;					//メッシュ(頂点情報)へのポインタ
		LPD3DXBUFFER pBuffMat;				//マテリアルへのポインタ
		LPDIRECT3DTEXTURE9 *pTexture;		//テクスチャへのポインタ
		DWORD dwNumMat;						//マテリアルの数
		D3DXVECTOR3 vtxMin;					//矩形の最小座標
		D3DXVECTOR3 vtxMax;					//矩形の最大座標

		Model() :mtxWorld(D3DXMATRIX()), pMesh(nullptr), pBuffMat(nullptr), pTexture(nullptr), dwNumMat(0), vtxMin(mylib_const::DEFVEC3), vtxMax(mylib_const::DEFVEC3) {};
	};

	CXModel();
	~CXModel();

	HRESULT Load(void);		//テクスチャ読み込み
	void Unload(void);		//テクスチャ破棄
	int Regist(const char *pFilename, D3DXVECTOR3 *vtxMin = nullptr, D3DXVECTOR3 *vtxMax = nullptr);		//モデル追加読込
	Model *GetAddress(int nIdx);			//モデル情報取得
	std::string GetFilename(int nIdx);		//モデルパス取得
	D3DXVECTOR3 GetVtxMin(int nIdx);		//最小矩形取得
	D3DXVECTOR3 GetVtxMax(int nIdx);		//最大矩形取得

protected:

private:
	std::list<Model> m_aModel;				//モデル情報格納
	std::list<std::string> m_apFilename;		//ファイル文字列
	static int m_nNumAll;		//総モデル数
};

#endif
