//============================
//
// Xモデルオブジェクトパーツ(Parts.cpp)
// Author: Kai Takada
//
//============================
#include "manager.h"
#include "Parts.h"
#include "Xmodel.h"

//===============================
// マクロ定義
//===============================

//============================
// 静的メンバ変数宣言
//============================

//=================================
// コンストラクタ
//=================================
CParts::CParts(int nPriority) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pParent = nullptr;		//親モデルへのポインタ

	ZeroMemory(&m_mat.MatD3D, sizeof(D3DMATERIAL9));
}

//=================================
// デストラクタ
//=================================
CParts::~CParts()
{

}

//=================================
// 初期化
//=================================
HRESULT CParts::Init(void)
{
	SetType(TYPE_PARTS);

	return E_FAIL;
}

//=================================
// 初期化
//=================================
HRESULT CParts::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;

	SetType(TYPE_PARTS);

	return S_OK;
}

//=================================
// 終了
//=================================
void CParts::Uninit(void)
{
	m_pParent = nullptr;

	Release();
}

//=================================
// 更新
//=================================
void CParts::Update(void)
{
}

//=================================
// 描画
//=================================
void CParts::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス
	D3DMATERIAL9 matDef;				//現在のマテリアル保存用
	D3DXMATERIAL *pMat;					//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;						//親のマトリックス

	CXModel *pXModel = CManager::GetXModel();
	CXModel::Model *pModel = pXModel->GetAddress(m_nIdxModel);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&pModel->mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxTrans);

	//パーツの「親のマトリックス」を設定
	if (m_pParent != nullptr)
	{//親モデルがある場合
		mtxParent = m_pParent->GetMtxWorld();		//親モデルのマトリックスを指定
	}
	else
	{//親モデルがない場合
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);		//大元ののマトリックスを設定
	}

	//算出した「パーツのマトリックス」と「親のマトリックス」を掛け合わせる
	D3DXMatrixMultiply(&pModel->mtxWorld,
		&pModel->mtxWorld,
		&mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &pModel->mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		if (m_mat.MatD3D.Ambient.a == 0.0f)
		{
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		}
		else
		{
			pDevice->SetMaterial(&m_mat.MatD3D);
		}
		
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
CParts * CParts::Create(const char *pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CParts *pParts = nullptr;

	//オブジェクト生成
	pParts = new CParts;

	if (pParts != nullptr)
	{
		//初期化
		if (FAILED(pParts->Init(pos, rot)))
		{
			pParts = nullptr;
			return nullptr;
		}

		CXModel *pXmodel = CManager::GetXModel();
		pParts->SetIdxModel(pXmodel->Regist(pFilename));		//テクスチャの割り当て

	}
	else
	{
		return nullptr;
	}

	return pParts;
}

//=================================
// ワールドマトリックス取得
//=================================
D3DXMATRIX CParts::GetMtxWorld(void)
{
	CXModel *pXModel = CManager::GetXModel();
	CXModel::Model *pModel = pXModel->GetAddress(m_nIdxModel);

	return pModel->mtxWorld;
}

//=================================
// 合計座標取得
//=================================
D3DXVECTOR3 CParts::GetOutcomePos(CParts * pParts)
{
	D3DXVECTOR3 answer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CParts * pNow = pParts;
	
	while (true)
	{
		if (pNow == nullptr)
		{
			break;
		}

		answer += pNow->GetPos();

		pNow = pNow->GetParent();
	}

	return answer;
}

//=================================
// 親モデルの設定
//=================================
void CParts::SetParent(CParts * pParts)
{
	m_pParent = pParts;
}
