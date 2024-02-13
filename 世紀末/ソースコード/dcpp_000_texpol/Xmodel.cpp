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
// テクスチャパス
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
	for (int nCntUnload = 0; nCntUnload < MAX_XMODEL; nCntUnload++)
	{
		//データ破棄
		m_aModel[nCntUnload].pBuffMat = nullptr;
		m_aModel[nCntUnload].pTexture = nullptr;
		m_aModel[nCntUnload].pMesh = nullptr;

		m_aModel[nCntUnload].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aModel[nCntUnload].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aModel[nCntUnload].mtxWorld = {};
		m_aModel[nCntUnload].dwNumMat = 0;

		//データ破棄
		m_apFilename[nCntUnload] = nullptr;
	}
}

//=================================
// デストラクタ
//=================================
CXModel::~CXModel()
{
	m_nNumAll--;
}

//=================================
// テクスチャ読み込み
//=================================
HRESULT CXModel::Load(void)
{
	for (int nCntFile = 0; nCntFile < sizeof(c_apXFilepass) / sizeof(c_apXFilepass[0]); nCntFile++)
	{
		//m_apFilename[nCntFile] = new char;

		Regist(c_apXFilepass[nCntFile]);
	}

	return S_OK;
}

//=================================
// テクスチャ破棄
//=================================
void CXModel::Unload(void)
{
	for (int nCntFile = 0; nCntFile < MAX_XMODEL; nCntFile++)
	{
		if (m_apFilename[nCntFile] != nullptr)
		{
			//deleteで破壊されている
			delete m_apFilename[nCntFile];
			m_apFilename[nCntFile] = nullptr;
		}
	}

	for (int nCntUnload = 0; nCntUnload < MAX_XMODEL; nCntUnload++)
	{
		//テクスチャ破棄
		if (m_aModel[nCntUnload].pMesh != nullptr)
		{
			m_aModel[nCntUnload].pMesh->Release();
			m_aModel[nCntUnload].pMesh = nullptr;
		}

		if (m_aModel[nCntUnload].pBuffMat != nullptr)
		{
			m_aModel[nCntUnload].pBuffMat->Release();
			m_aModel[nCntUnload].pBuffMat = nullptr;
		}

		if (m_aModel[nCntUnload].pTexture != nullptr)
		{
			for (int nCntTex = 0; nCntTex < int(m_aModel[nCntUnload].dwNumMat); nCntTex++)
			{
				if (m_aModel[nCntUnload].pTexture[nCntTex] != nullptr)
				{
					m_aModel[nCntUnload].pTexture[nCntTex]->Release();
					m_aModel[nCntUnload].pTexture[nCntTex] = nullptr;
				}
			}

			delete m_aModel[nCntUnload].pTexture;
			m_aModel[nCntUnload].pTexture = nullptr;
		}
	}
}

//============================
// モデル追加読込
//============================
int CXModel::Regist(const char *pFilename, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	int nNumVtx = 0;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ
	int nCntReg = 0;

	for (nCntReg = 0; nCntReg < MAX_XMODEL; nCntReg++)
	{
		if (m_aModel[nCntReg].pMesh != nullptr &&
			strcmp(m_apFilename[nCntReg],(char*)(pFilename)) == 0)
		{//情報が入っている
			
			vtxMin = &m_aModel[nCntReg].vtxMin;
			vtxMax = &m_aModel[nCntReg].vtxMax;

			return nCntReg;
		}
		if (m_aModel[nCntReg].pMesh == nullptr)
		{//情報が入っていない
			break;
		}
	}

	if (nCntReg >= MAX_XMODEL)
	{//情報を入れる箱がなくて
	
		if (m_aModel[0].pMesh != nullptr)
		{//0番が正常に読み込めていたら
			return 0;
		}
		else
		{
			return -1;
		}
	}

	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_aModel[nCntReg].pBuffMat,
		NULL,
		&m_aModel[nCntReg].dwNumMat,
		&m_aModel[nCntReg].pMesh)))
	{
		return 0;
	}

	//マテリアル情報に対するポインタを取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_aModel[nCntReg].pBuffMat->GetBufferPointer();

	if (m_aModel[nCntReg].pTexture != nullptr)
	{
		m_aModel[nCntReg].pTexture = nullptr;
	}

	m_aModel[nCntReg].pTexture = new LPDIRECT3DTEXTURE9[(int)m_aModel[nCntReg].dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_aModel[nCntReg].dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,				//テクスチャのファイル名
				&m_aModel[nCntReg].pTexture[nCntMat])))	//テクスチャパスが間違っていると描画時に壊れる(breakpoint)
			{
				m_aModel[nCntReg].pTexture[nCntMat] = nullptr;
			}
		}
		else
		{
			m_aModel[nCntReg].pTexture[nCntMat] = nullptr;
		}
	}

	//頂点数を取得
	nNumVtx = m_aModel[nCntReg].pMesh->GetNumVertices();

	//頂点フォーマットのサイズの取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_aModel[nCntReg].pMesh->GetFVF());

	//頂点バッファをロック
	m_aModel[nCntReg].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

		//すべての頂点を比較して敵の最小値・最大値を抜き出す
		if (m_aModel[nCntReg].vtxMax.x < vtx.x)
		{
			m_aModel[nCntReg].vtxMax.x = vtx.x;
		}
		if (m_aModel[nCntReg].vtxMax.y < vtx.y)
		{
			m_aModel[nCntReg].vtxMax.y = vtx.y;
		}
		if (m_aModel[nCntReg].vtxMax.z < vtx.z)
		{
			m_aModel[nCntReg].vtxMax.z = vtx.z;
		}

		if (m_aModel[nCntReg].vtxMin.x > vtx.x)
		{
			m_aModel[nCntReg].vtxMin.x = vtx.x;
		}
		if (m_aModel[nCntReg].vtxMin.y > vtx.y)
		{
			m_aModel[nCntReg].vtxMin.y = vtx.y;
		}
		if (m_aModel[nCntReg].vtxMin.z > vtx.z)
		{
			m_aModel[nCntReg].vtxMin.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;		//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	m_aModel[nCntReg].pMesh->UnlockVertexBuffer();

	vtxMin = &m_aModel[nCntReg].vtxMin;
	vtxMax = &m_aModel[nCntReg].vtxMax;

	m_apFilename[nCntReg] = new char[int(strlen(pFilename)) + 1];
	strcpy(m_apFilename[nCntReg], pFilename);

	m_nNumAll++;
	return nCntReg;
}

//============================
//テクスチャ情報取得
//============================
CXModel::Model *CXModel::GetAddress(int nIdx)
{
	return &m_aModel[nIdx]; 
}
