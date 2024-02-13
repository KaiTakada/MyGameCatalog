//============================
//
// Xモデルの処理(model.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "objectX.h"

//===============================
// マクロ定義
//===============================

//=================================
// コンストラクタ
//=================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_nIdxModel = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = m_pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//=================================
// デストラクタ
//=================================
CObjectX::~CObjectX()
{

}

//=================================
// プレイヤーの初期化処理
//=================================
HRESULT CObjectX::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);		//初期化する
	m_posOld = m_pos;		//初期化する
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//初期化する

	return S_OK;
}

//=================================
// プレイヤーの初期化処理
//=================================
HRESULT CObjectX::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)		//初期化(オバロ)
{
	m_pos = pos;		//初期化する
	m_posOld = m_pos;		//初期化する
	m_rot = rot;		//初期化する

	return S_OK;
}

//=================================
// プレイヤーの終了処理
//=================================
void CObjectX::Uninit(void)
{
	//死亡フラグを建てちゃう
	SetDeath(true);

	Release();
}

//=================================
// プレイヤーの更新処理
//=================================
void CObjectX::Update(void)
{
	m_posOld = m_pos;
}

//=================================
// プレイヤーの描画処理
//=================================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;				//現在のマテリアル保存用
	D3DXMATERIAL *pMat;					//マテリアルデータへのポインタ

	CXModel *pXModel = CManager::GetXModel();
	CXModel::Model *pModel = pXModel->GetAddress(m_nIdxModel);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&pModel->mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pModel->mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pModel->pTexture[nCntMat]);

		//モデル(パーツ)の描画
		pModel->pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=================================
// プレイヤーの描画処理(色指定)
//=================================
void CObjectX::Draw(const D3DMATERIAL9 mat)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;				//現在のマテリアル保存用
	D3DXMATERIAL *pMat;					//マテリアルデータへのポインタ

	CXModel *pXModel = CManager::GetXModel();
	CXModel::Model *pModel = pXModel->GetAddress(m_nIdxModel);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&pModel->mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pModel->mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&mat);

		//テクスチャの設定
		pDevice->SetTexture(0, pModel->pTexture[nCntMat]);

		//モデル(パーツ)の描画
		pModel->pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=================================
// 生成
//=================================
CObjectX *CObjectX::Create(void)
{
	CObjectX *pModel;

	//オブジェクト生成
	pModel = new CObjectX;

	//初期化
	pModel->Init();

	return pModel;
}

//=================================
// 生成
//=================================
CObjectX *CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fSize)
{
	CObjectX *pObjectX = nullptr;
	
	fSize;

	//オブジェクト生成&初期化
	pObjectX = new CObjectX;

	if (pObjectX != nullptr)
	{
		pObjectX->Init(pos, rot);
	}
	else
	{
		return nullptr;
	}

	return pObjectX;
}
