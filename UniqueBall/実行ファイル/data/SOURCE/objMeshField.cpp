//============================
//
// メッシュ地面(objMeshField.cpp)
// Author: Kai Takada
//
//============================
//==================================
// インクルードファイル
//==================================
#include "objMeshField.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"

//==================================
// 定数定義
//==================================
//一枚における最大頂点数(64000)

namespace
{
	const float MESHFIELD_SIZE(600.0f);	//半径の長さ
	const int CUT_U(2);		//分割数x
	const int CUT_V(2);		//分割数z
	const char* TEX_FIELD = "data\\TEXTURE\\BG\\field.jpg";
	const int DEF_NUM_DIF(8);
}

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// コンストラクタ
//==================================
CObjMeshField::CObjMeshField(int nPriority) : CObject(nPriority)
{
	m_pos = mylib_const::DEFVEC3;
	m_rot = mylib_const::DEFVEC3;
	m_size = mylib_const::DEFVEC2;
	m_mtxWorld = {};
	m_nNumCutV = 0;
	m_nNumCutU = 0;
	m_nNumVtxV = 0;
	m_nNumVtxU = 0;
	m_nNumVtxAll = 0;
	m_nNumIdx = 0;
	m_nNumPrim = 0;
	
	m_pIdxBuff= nullptr;
	m_pVtxBuff = nullptr;
	m_nIdxTexture = 0;

	m_bWireFrame = false;
	m_nCull = 0;

	m_bZtest = false;	
	m_bAtest = false;
	m_bAbrend = false;
	m_bLight = false;
}

//==================================
// デストラクタ
//==================================
CObjMeshField::~CObjMeshField()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CObjMeshField::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_nNumCutV = CUT_V;
	m_nNumCutU = CUT_U;
	m_nNumVtxV = m_nNumCutV + 1;
	m_nNumVtxU = m_nNumCutU + 1;
	m_nNumVtxAll = m_nNumVtxV * m_nNumVtxU;			//全頂点

	m_nNumIdx = m_nNumVtxV * m_nNumVtxU				//全頂点
		+ (m_nNumVtxV - 2) * m_nNumVtxU		//かぶり
		+ (m_nNumVtxV - 2) * 2;									//斜め	

	m_nNumPrim = (m_nNumCutV * m_nNumCutU) * 2	//描画ポリゴン
		+ (m_nNumVtxV - 2) * 4;								//縮退ポリゴン

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxAll,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//半径
	float fRadiusU = MESHFIELD_SIZE;
	float fRadiusV = MESHFIELD_SIZE;

	//スタート
	float fStartU = -fRadiusU;
	float fStartV = fRadiusV;

	//頂点間の距離
	float fLargeU = (fRadiusU * 2) / m_nNumCutU;	//横
	float fLargeV = -(fRadiusV * 2) / m_nNumCutV;	//縦

	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標・法線ベクトル・頂点カラーの設定
	for (int nCntPosV = 0; nCntPosV < m_nNumVtxV; nCntPosV++)
	{//縦
		for (int nCntPosU = 0; nCntPosU < m_nNumVtxU; nCntPosU++)
		{//横
			if (nCntPosU == 355 && nCntPosV == 2)
			{
				pVtx[0] = pVtx[0];
			}

			//頂点座標
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].pos = D3DXVECTOR3(fStartU + (nCntPosU * fLargeU),
				0.0f,
				fStartV + (nCntPosV * fLargeV));		//(x,y,z)

			//テクスチャ座標
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].tex = D3DXVECTOR2(nCntPosU * 1.0f, nCntPosV * 1.0f);

			//法線ベクトル
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	WORD* pIdx;		//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCtrV = 0;

	for (int nCntIdx = 0; nCntIdx < m_nNumIdx / 2; nCntIdx++)
	{//インデックス決定
		nCtrV = nCntIdx / (m_nNumVtxU + 1);	// 何段目か

		if (nCntIdx == (m_nNumVtxU + ((m_nNumVtxU * nCtrV) + nCtrV)) &&
			nCntIdx != 0)	//斜めの時だけこっち
		{
			pIdx[nCntIdx * 2 + 1] = (WORD)(nCntIdx - (nCtrV)+m_nNumVtxU);
			pIdx[nCntIdx * 2] = (WORD)(nCntIdx - (nCtrV)-1);
		}
		else
		{
			pIdx[nCntIdx * 2 + 1] = (WORD)(nCntIdx - (nCtrV));
			pIdx[nCntIdx * 2] = (WORD)(nCntIdx - (nCtrV)+m_nNumVtxU);
		}
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	//テクスチャ番号
	SetIdxTexture(0);

	SetType(TYPE_MESHFIELD);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CObjMeshField::Init(const int nNumCutU, const int nNumCutV)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_nNumCutV = nNumCutV;
	m_nNumCutU = nNumCutU;
	m_nNumVtxV = nNumCutV + 1;
	m_nNumVtxU = nNumCutU + 1;
	m_nNumVtxAll = m_nNumVtxV * m_nNumVtxU;			//全頂点

	m_nNumIdx = m_nNumVtxV * m_nNumVtxU				//全頂点
		+ (m_nNumVtxV - 2) * m_nNumVtxU		//かぶり
		+ (m_nNumVtxV - 2) * 2;									//斜め	

	m_nNumPrim = (m_nNumCutV * m_nNumCutU) * 2	//描画ポリゴン
		+ (m_nNumVtxV - 2) * 4;								//縮退ポリゴン

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxAll,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//半径
	float fRadiusU = MESHFIELD_SIZE;
	float fRadiusV = MESHFIELD_SIZE;

	//スタート
	float fStartU = -fRadiusU;
	float fStartV = fRadiusV;

	//頂点間の距離
	float fLargeU = (fRadiusU * 2) / m_nNumCutU;	//横
	float fLargeV = -(fRadiusV * 2) / m_nNumCutV;	//縦

	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標・法線ベクトル・頂点カラーの設定
	for (int nCntPosV = 0; nCntPosV < m_nNumVtxV; nCntPosV++)
	{//縦
		for (int nCntPosU = 0; nCntPosU < m_nNumVtxU; nCntPosU++)
		{//横
			if (nCntPosU == 355 && nCntPosV == 2)
			{
				pVtx[0] = pVtx[0];
			}

			//頂点座標
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].pos = D3DXVECTOR3(fStartU + (nCntPosU * fLargeU),
				0.0f,
				fStartV + (nCntPosV * fLargeV));		//(x,y,z)

			//テクスチャ座標
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].tex = D3DXVECTOR2(nCntPosU * 1.0f, nCntPosV * 1.0f);

			//法線ベクトル
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD* pIdx;		//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCtrV = 0;

	for (int nCntIdx = 0; nCntIdx < m_nNumIdx / 2; nCntIdx++)
	{//インデックス決定
		nCtrV = nCntIdx / (m_nNumVtxU + 1);	// 何段目か

		if (nCntIdx == (m_nNumVtxU + ((m_nNumVtxU * nCtrV) + nCtrV)) &&
			nCntIdx != 0)	//斜めの時だけこっち
		{
			pIdx[nCntIdx * 2 + 1] = (WORD)(nCntIdx - (nCtrV)+m_nNumVtxU);
			pIdx[nCntIdx * 2] = (WORD)(nCntIdx - (nCtrV)-1);
		}
		else
		{
			pIdx[nCntIdx * 2 + 1] = (WORD)(nCntIdx - (nCtrV));
			pIdx[nCntIdx * 2] = (WORD)(nCntIdx - (nCtrV)+m_nNumVtxU);
		}
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	//テクスチャ番号
	SetIdxTexture(0);

	SetType(TYPE_MESHFIELD);

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CObjMeshField::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;					//バッファの破棄
	}

	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;					//バッファの破棄
	}

	//死亡フラグを建てちゃう
	SetDeath(true);
	Release();
}

//==================================
// 更新処理
//==================================
void CObjMeshField::Update(void)
{
	UpdateVtx();
}

//==================================
// 描画処理
//==================================
void CObjMeshField::Draw(void)
{
	//テクスチャ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//レンダラー個体設定
	DrawRenderSet(pDevice);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,
		0,													//最小インデックス
		m_nNumVtxAll,										//用意した頂点の数
		0,													//何番スタートか
		m_nNumPrim);										//描画するプリミティブの数

	//レンダラーデフォルト設定
	DrawRenderReset(pDevice);
}

//============================
// 生成処理
//============================
CObjMeshField* CObjMeshField::Create()
{
	CObjMeshField* pField = nullptr;

	//オブジェクト生成
	pField = new CObjMeshField;

	//初期化
	pField->Init();

	return pField;
}

//============================
// 生成処理
//============================
CObjMeshField* CObjMeshField::Create(const int nNumCutU, const int nNumCutV)
{
	CObjMeshField* pField = nullptr;

	//オブジェクト生成&初期化
	pField = new CObjMeshField;

	if (pField != nullptr)
	{
		pField->Init(nNumCutU, nNumCutV);

		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		pField->SetIdxTexture(pTexture->Regist(TEX_FIELD));
	}
	else
	{
		return nullptr;
	}

	return pField;
}

//============================
// 頂点位置取得
//============================
void CObjMeshField::GetPolyPos(D3DXVECTOR3* Vtx0, D3DXVECTOR3* Vtx1, D3DXVECTOR3* Vtx2, D3DXVECTOR3* Vtx3)
{
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	if (Vtx0 != nullptr)*Vtx0 = pVtx[0].pos;		//(x,y,z)
	if (Vtx1 != nullptr)*Vtx1 = pVtx[m_nNumCutU].pos;
	if (Vtx2 != nullptr)*Vtx2 = pVtx[m_nNumVtxAll - m_nNumCutU].pos;
	if (Vtx3 != nullptr)*Vtx3 = pVtx[m_nNumVtxAll - 1].pos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================
// 乗っているオブジェクトの位置y取得
//====================================
float CObjMeshField::GetHeight(D3DXVECTOR3 posObj)
{
	D3DXVECTOR3 vecToPos0, vecToPos1, vecToPos2, vecToPos3;			//pos0とpos1の差分（位置関係）
	D3DXVECTOR3 vecLine0, vecLine1, vecLine2, vecLine3;		//壁の長さ
	D3DXVECTOR3 vecLineMid, vecToPosMid;	//真ん中の長さ・差分

	D3DXVECTOR3 Vtxpos0, Vtxpos1, Vtxpos2, Vtxpos3;

	GetPolyPos(&Vtxpos0, &Vtxpos1, &Vtxpos2, &Vtxpos3);

	vecLine0 = Vtxpos1 - Vtxpos0;
	vecToPos0 = posObj - Vtxpos0;

	vecLine1 = Vtxpos0 - Vtxpos2;
	vecToPos1 = posObj - Vtxpos2;

	vecLine2 = Vtxpos2 - Vtxpos3;
	vecToPos2 = posObj - Vtxpos3;

	vecLine3 = Vtxpos3 - Vtxpos1;
	vecToPos3 = posObj - Vtxpos1;

	//真ん中
	vecLineMid = Vtxpos1 - Vtxpos2;
	vecToPosMid = posObj - Vtxpos2;

	//プレイヤーがポリゴンの内側にいる
	if ((vecLine0.z * vecToPos0.x) - (vecLine0.x * vecToPos0.z) >= 0.0f &&
		(vecLine1.z * vecToPos1.x) - (vecLine1.x * vecToPos1.z) >= 0.0f &&
		(vecLine2.z * vecToPos2.x) - (vecLine2.x * vecToPos2.z) >= 0.0f &&
		(vecLine3.z * vecToPos3.x) - (vecLine3.x * vecToPos3.z) >= 0.0f)
	{
		D3DXVECTOR3 nor;	//法線
		float fHeight;		//求める高さ

		//対象オブジェクトへのベクトル
		D3DXVECTOR3 vec0;
		D3DXVECTOR3 vec1;
		D3DXVECTOR3 VtxposOrg;
		D3DXVECTOR3 vecP;

		if ((vecLineMid.z * vecToPosMid.x) - (vecLineMid.x * vecToPosMid.z) <= 0.0f)
		{
			VtxposOrg = Vtxpos0;
			vecP = posObj - Vtxpos0;
			vec0 = Vtxpos1 - Vtxpos0;
			vec1 = Vtxpos2 - Vtxpos0;
		}
		else
		{
			VtxposOrg = Vtxpos3;
			vecP = posObj - Vtxpos3;
			vec0 = Vtxpos2 - Vtxpos3;
			vec1 = Vtxpos1 - Vtxpos3;
		}

		//2つのベクトルから法線を求める
		D3DXVec3Cross(&nor, &vec0, &vec1);

		//法線を正規化
		D3DXVec3Normalize(&nor, &nor);

		if (nor.y != 0.0f)
		{//起伏があった場合?

			fHeight = (-((vecP.x * nor.x) + (vecP.z * nor.z)) + (VtxposOrg.y * nor.y)) / nor.y;

			return fHeight;
		}
	}

	return posObj.y;
}

//==========================================
// 描画時レンダラー設定
//==========================================
void CObjMeshField::DrawRenderSet(LPDIRECT3DDEVICE9 pDevice)
{
	//Zテストを無効にする
	if (m_bZtest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	//アルファテストを有効にする
	if (m_bAtest == true)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	}

	//aブレンディングを加算合成に設定
	if (m_bAbrend == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//ライティングをTRUEに設定
	if (m_bLight == true)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
}

//==========================================
// 描画時レンダラー設定解除
//==========================================
void CObjMeshField::DrawRenderReset(LPDIRECT3DDEVICE9 pDevice)
{
	//ライティングをTRUEに設定
	if (m_bLight)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	//Zテストを無効にする
	if (m_bZtest)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//アルファテストを有効にする
	if (m_bAtest)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
	}

	//aブレンディングを加算合成に設定
	if (m_bAbrend)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//==========================================
// 頂点情報更新
//==========================================
void CObjMeshField::UpdateVtx()
{
	//スタート
	float fStartU = -m_size.x;
	float fStartV = m_size.y;
	
	//頂点間の距離
	float fLargeU = (m_size.x * 2) / m_nNumCutU;	//横
	float fLargeV = -(m_size.y * 2) / m_nNumCutV;	//縦

	//頂点情報のポインタ
	VERTEX_3D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標・法線ベクトル・頂点カラーの設定
	for (int nCntPosV = 0; nCntPosV < m_nNumVtxV; nCntPosV++)
	{//縦
		for (int nCntPosU = 0; nCntPosU < m_nNumVtxU; nCntPosU++)
		{//横
			if (nCntPosU == 355 && nCntPosV == 2)
			{
				pVtx[0] = pVtx[0];
			}

			//頂点座標
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].pos = D3DXVECTOR3(fStartU + (nCntPosU * fLargeU),
				0.0f,
				fStartV + (nCntPosV * fLargeV));		//(x,y,z)

			//テクスチャ座標
			pVtx[nCntPosV * m_nNumVtxU + nCntPosU].tex = D3DXVECTOR2(nCntPosU * 1.0f, nCntPosV * 1.0f);
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
