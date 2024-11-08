//============================
//
// Xモデルの処理(model.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "Xmodel.h"

//===============================
// マクロ定義
//===============================

//============================
// 静的メンバ変数宣言
//============================
int CXModel::m_nNumAll = 0;				//共通テクスチャバッファ

//============================
// モデルパス
//============================
const char *c_apXFilepass[] =
{
	"data\\MODEL\\null.x"
};

//=================================
// コンストラクタ
//=================================
CXModel::CXModel()
{
	m_aModel.clear();
	m_apFilename.clear();
}

//=================================
// デストラクタ
//=================================
CXModel::~CXModel()
{
	m_aModel.clear();
	m_apFilename.clear();
}

//=================================
// 読み込み
//=================================
HRESULT CXModel::Load(void)
{
	for (int nCntFile = 0; nCntFile < sizeof(c_apXFilepass) / sizeof(c_apXFilepass[0]); nCntFile++)
	{
		Regist(c_apXFilepass[nCntFile]);
	}

	return S_OK;
}

//=================================
// 破棄
//=================================
void CXModel::Unload(void)
{
	// リストループ
	for (std::list<Model>::const_iterator it = m_aModel.cbegin(); it != m_aModel.cend(); it++)
	{
		Model model = *it;

		if (model.pMesh != nullptr)
		{
			model.pMesh->Release();
			model.pMesh = nullptr;
		}

		if (model.pBuffMat != nullptr)
		{
			model.pBuffMat->Release();
			model.pBuffMat = nullptr;
		}

		if (model.pTexture != nullptr)
		{
			delete model.pTexture;
			model.pTexture = nullptr;
		}
	}

	m_aModel.clear();
	m_apFilename.clear();
}

//============================
// 追加読込
//============================
int CXModel::Regist(const char *pFilename, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	int nNumVtx = 0;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ
	int nCntReg = 0;

	for (nCntReg = 0; nCntReg < MAX_XMODEL; nCntReg++)
	{
		Model* model = GetAddress(nCntReg);

		if (model == nullptr)
		{//情報が入っていない
			break;
		}

		std::string str = GetFilename(nCntReg);
		if (strcmp(str.c_str(), (pFilename)) == 0)
		{//情報が入っている
			
			if (vtxMin != nullptr) vtxMin = &model->vtxMin;
			if (vtxMax != nullptr) vtxMax = &model->vtxMax;

			return nCntReg;
		}
	}

	Model* pModel = new Model;

	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModel->pBuffMat,
		NULL,
		&pModel->dwNumMat,
		&pModel->pMesh)))
	{
		return 0;
	}

	//マテリアル情報に対するポインタを取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	if (pModel->pTexture != nullptr)
	{
		pModel->pTexture = nullptr;
	}

	//TODO: ここから再度ブラッシュアップ
	pModel->pTexture = new LPDIRECT3DTEXTURE9[(int)pModel->dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,				//テクスチャのファイル名
				&pModel->pTexture[nCntMat])))	//テクスチャパスが間違っていると描画時に壊れる(breakpoint)
			{
				pModel->pTexture[nCntMat] = nullptr;
			}
		}
		else
		{
			pModel->pTexture[nCntMat] = nullptr;
		}
	}

	//頂点数を取得
	nNumVtx = pModel->pMesh->GetNumVertices();

	//頂点フォーマットのサイズの取得
	dwSizeFVF = D3DXGetFVFVertexSize(pModel->pMesh->GetFVF());

	//頂点バッファをロック
	pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

		//すべての頂点を比較して敵の最小値・最大値を抜き出す
		if (pModel->vtxMax.x < vtx.x)
		{
			pModel->vtxMax.x = vtx.x;
		}
		if (pModel->vtxMax.y < vtx.y)
		{
			pModel->vtxMax.y = vtx.y;
		}
		if (pModel->vtxMax.z < vtx.z)
		{
			pModel->vtxMax.z = vtx.z;
		}

		if (pModel->vtxMin.x > vtx.x)
		{
			pModel->vtxMin.x = vtx.x;
		}
		if (pModel->vtxMin.y > vtx.y)
		{
			pModel->vtxMin.y = vtx.y;
		}
		if (pModel->vtxMin.z > vtx.z)
		{
			pModel->vtxMin.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;		//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	pModel->pMesh->UnlockVertexBuffer();

	vtxMin = &pModel->vtxMin;
	vtxMax = &pModel->vtxMax;

	//リストに追加
	m_aModel.push_back(*pModel);
	m_apFilename.push_back(pFilename);

	//ポインタ削除
	delete pModel;

	m_nNumAll++;
	return nCntReg;
}

//============================
// 情報取得
//============================
CXModel::Model* CXModel::GetAddress(int nIdx)
{
	if (nIdx < 0 || nIdx >= static_cast<int>(m_aModel.size()))
	{
		return nullptr;
	}
	return &(*std::next(m_aModel.begin(), nIdx));
}

//============================
// パス取得
//============================
std::string CXModel::GetFilename(int nIdx)
{
	if (nIdx < 0 || nIdx >= static_cast<int>(m_apFilename.size()))
	{
		return nullptr;
	}
	std::string str = (*std::next(m_apFilename.begin(), nIdx));
	return str;
}

//============================
// 最小矩形取得
//============================
D3DXVECTOR3 CXModel::GetVtxMin(int nIdx)
{
	if (nIdx < 0 || nIdx >= static_cast<int>(m_aModel.size()))
	{
		return mylib_const::DEFVEC3;
	}

	Model* model = &(*std::next(m_aModel.begin(), nIdx));
	
	return model->vtxMax;
}

//============================
// 最大矩形取得
//============================
D3DXVECTOR3 CXModel::GetVtxMax(int nIdx)
{
	if (nIdx < 0 || nIdx >= static_cast<int>(m_aModel.size()))
	{
		return mylib_const::DEFVEC3;
	}

	Model* model = &(*std::next(m_aModel.begin(), nIdx));

	return model->vtxMin;
}
