//============================
//
// 3Dポリゴン処理(object3D.cpp)
// Author: Kai Takada
//
//============================
#include "object3D.h"
#include "texture.h"
#include "Anim.h"
#include "manager.h"
#include "debugproc.h"

//==================================
// マクロ定義
//==================================

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// コンストラクタ
//==================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_mtxWorld = {};
	m_pos = DEF_VERTEX3D;
	m_rot = DEF_VERTEX3D;
	m_size = DEF_SIZE;
}

//==================================
// デンストラクタ
//==================================
CObject3D::~CObject3D()
{

}

//==================================
// 3Dポリゴンの初期化処理
//==================================
HRESULT CObject3D::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_pos = DEF_VERTEX3D;	//初期化する
	m_rot = DEF_VERTEX3D;	//初期化する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-DEF_SIZE.x, 0.0f, DEF_SIZE.z);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(DEF_SIZE.x, 0.0f, DEF_SIZE.z);
	pVtx[2].pos = D3DXVECTOR3(-DEF_SIZE.x, 0.0f, -DEF_SIZE.z);
	pVtx[3].pos = D3DXVECTOR3(DEF_SIZE.x, 0.0f, -DEF_SIZE.z);

	//法線ベクトルの設定
	D3DXVECTOR3 nor0 = vec(pVtx[0].pos, pVtx[1].pos, pVtx[2].pos);
	D3DXVECTOR3 nor3 = vec(pVtx[3].pos, pVtx[2].pos, pVtx[1].pos);

	pVtx[0].nor = D3DXVECTOR3(nor0);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3((nor0 + nor3) * 0.5f);
	pVtx[2].nor = D3DXVECTOR3((nor0 + nor3) * 0.5f);
	pVtx[3].nor = D3DXVECTOR3(nor3);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	SetType(TYPE_3DPOLYGON);

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CObject3D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//位置設定
	m_pos = pos;

	//向き設定
	m_rot = rot;

	//サイズ設定
	m_size = D3DXVECTOR3(DEF_SIZE.x, 0.0f, DEF_SIZE.z);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{//失敗した場合
		return E_FAIL;
	}

	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.z);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.z);

	//法線ベクトルの設定
	D3DXVECTOR3 nor0 = vec(pVtx[0].pos, pVtx[1].pos, pVtx[2].pos);
	D3DXVECTOR3 nor3 = vec(pVtx[3].pos, pVtx[2].pos, pVtx[1].pos);

	pVtx[0].nor = D3DXVECTOR3(nor0);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3((nor0 + nor3) * 0.5f);
	pVtx[2].nor = D3DXVECTOR3((nor0 + nor3) * 0.5f);
	pVtx[3].nor = D3DXVECTOR3(nor3);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	SetType(TYPE_3DPOLYGON);

	return S_OK;
}

//==================================
//3Dポリゴンの終了処理
//==================================
void CObject3D::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;					//バッファの破棄
	}

	//死亡フラグを建てちゃう
	SetDeath(true);
	Release();
}

//==================================
//3Dポリゴンの更新処理
//==================================
void CObject3D::Update(void)
{

}

//==================================
//3Dポリゴンの描画処理
//==================================
void CObject3D::Draw(void)
{
	//テクスチャ取得
	CTexture *pTexture = CManager::GetTexture();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//描画する最初のインデックス
		2);												//描画するプリミティブ数
}

//============================
// 2Dポリゴンの生成処理
//============================
CObject3D * CObject3D::Create(void)
{
	CObject3D *pObject3D;

	//オブジェクト生成
	pObject3D = new CObject3D;

	//初期化
	pObject3D->Init();

	return pObject3D;
}

//============================
// 3Dポリゴンの生成処理
//============================
CObject3D * CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	CObject3D *pObject3D = nullptr;

	//オブジェクト生成&初期化
	pObject3D = new CObject3D;

	if (pObject3D != nullptr)
	{
		pObject3D->Init(pos, rot);

		pObject3D->SetSize(size);

		pObject3D->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pObject3D;
}

//====================================
// 位置の設定
//====================================
void CObject3D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//====================================
// 位置の設定
//====================================
void CObject3D::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;

	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//法線ベクトルの設定
	D3DXVECTOR3 nor0 = vec(pVtx[0].pos, pVtx[1].pos, pVtx[2].pos);
	D3DXVECTOR3 nor3 = vec(pVtx[3].pos, pVtx[2].pos, pVtx[1].pos);

	pVtx[0].nor = D3DXVECTOR3(nor0);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3((nor0 + nor3) * 0.5f);
	pVtx[2].nor = D3DXVECTOR3((nor0 + nor3) * 0.5f);
	pVtx[3].nor = D3DXVECTOR3(nor3);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================
// 位置の設定
//====================================
void CObject3D::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;

	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, 0.0f, size.z);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(size.x, 0.0f, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, 0.0f, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, 0.0f, -size.z);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================
// テクスチャの分割数
//====================================
void CObject3D::SetDivTex(int nDivU, int nDivV)
{
	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(float(nDivU), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, float(nDivV));
	pVtx[3].tex = D3DXVECTOR2(float(nDivU), float(nDivV));

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================
// テクスチャ座標設定
//============================
void CObject3D::SetTexPos(const D3DXVECTOR2 rightDown, const D3DXVECTOR2 leftUp)
{
	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(leftUp.x, leftUp.y);
	pVtx[1].tex = D3DXVECTOR2(rightDown.x, leftUp.y);
	pVtx[2].tex = D3DXVECTOR2(leftUp.x, rightDown.y);
	pVtx[3].tex = D3DXVECTOR2(rightDown.x, rightDown.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================
// 乗っているオブジェクトの位置y取得
//====================================
float CObject3D::GetHeight(D3DXVECTOR3 posObj)
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
		D3DXVECTOR3 nor;
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
		{//起伏がなかった場合

			fHeight = (-((vecP.x * nor.x) + (vecP.z * nor.z)) + (VtxposOrg.y * nor.y)) / nor.y;

			return fHeight;
		}
	}

	return posObj.y;
}

//============================
// 頂点位置取得
//============================
void CObject3D::GetPolyPos(D3DXVECTOR3 *Vtx0, D3DXVECTOR3 *Vtx1, D3DXVECTOR3 *Vtx2, D3DXVECTOR3 *Vtx3)
{
	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	*Vtx0 = pVtx[0].pos;		//(x,y,z)
	*Vtx1 = pVtx[1].pos;
	*Vtx2 = pVtx[2].pos;
	*Vtx3 = pVtx[3].pos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================
// 法線ベクトル取得
//============================
D3DXVECTOR3 CObject3D::vec(D3DXVECTOR3 vtxpos0, D3DXVECTOR3 vtxpos1, D3DXVECTOR3 vtxpos2)
{
	D3DXVECTOR3 vec1;		//弾のベクトル
	D3DXVECTOR3 vec2;		//弾のベクトル
	D3DXVECTOR3 nor;		//弾のベクトル

	vec1 = vtxpos0 - vtxpos1;
	vec2 = vtxpos0 - vtxpos2;

	nor.x = (vec1.y * vec2.z - vec1.z * vec2.y);
	nor.y = (vec1.z * vec2.x - vec1.x * vec2.z);
	nor.z = (vec1.x * vec2.y - vec1.y * vec2.x);

	//ベクトルを正規化する(1.0fにする)
	D3DXVec3Normalize(&nor, &nor);

	//外積
	D3DXVec3Cross(&nor, &vec1, &vec2);
	
	//内積
	D3DXVec3Dot(&vec1, &vec2);

	return nor;
}

//==========================================
//============= アニメーション =============
//============================
// コンストラクタ
//============================
CObject3DAnim::CObject3DAnim(int nPriority) : CObject3D(nPriority)
{
	m_anim = nullptr;
}

//============================
// デストラクタ
//============================
CObject3DAnim::~CObject3DAnim()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CObject3DAnim::Init(void)
{
	//初期化
	CObject3D::Init();

	m_anim = new CAnim;

	if (m_anim != nullptr)
	{
		m_anim->Init();

		m_anim->SetNumLoop(0);
		m_anim->SetNumPtnX(1);
		m_anim->SetNumPtnY(1);
	}

	CObject3D::SetTexPos(D3DXVECTOR2(0.2f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CObject3DAnim::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject3D::Init();

	CObject3D::SetPos(pos);
	CObject3D::SetRot(rot);
	CObject3D::SetSize(D3DXVECTOR3(DEF_SIZE_ANIM.x, DEF_SIZE_ANIM.y, 0.0f));

	m_anim = new CAnim;

	if (m_anim != nullptr)
	{
		m_anim->Init();

		m_anim->SetNumLoop(0);
		m_anim->SetNumPtnX(1);
		m_anim->SetNumPtnY(1);
	}

	CObject3D::SetTexPos(D3DXVECTOR2(1.0f / m_anim->GetNumPtnX(), 1.0f / m_anim->GetNumPtnY()), D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CObject3DAnim::Uninit(void)
{
	if (m_anim != nullptr)
	{
		m_anim->Uninit();
		delete m_anim;
	}

	//終了
	CObject3D::Uninit();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CObject3DAnim::Update(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CObject3D::GetVtxBuff();
	VERTEX_2D*pVtx;

	m_anim->Update();
	D3DXVECTOR2 *pTexpos = m_anim->GetTexpos();

	//テクスチャの更新
	//頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	for (int nCntTex = 0; nCntTex < MAX_VTX; nCntTex++)
	{
		pVtx[nCntTex].tex = pTexpos[nCntTex];		//(x,y)
	}

	//頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//============================
// 2Dポリゴンの描画処理
//============================
void CObject3DAnim::Draw(void)
{
	//描画
	CObject3D::Draw();
}
