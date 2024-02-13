//=================================================
//
// ビルボード(billboard.cpp)
// Author: Kai Takada
//
//=================================================
#include "manager.h"
#include "objectBillboard.h"
#include "debugproc.h"
#include "input.h"
#include "texture.h"
#include "Anim.h"

//==============================
// マクロ定義
//==============================
#define BILLBOARD_SIZE (100.0f)
#define TEX_PASS "data\\TEXTURE\\chara.png"

//==============================
// 静的メンバ変数宣言
//==============================

//==============================
// コンストラクタ
//==============================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject(nPriority)
{
	m_mtxWorld = {};								//ワールドマトリックス
	m_pTexture = nullptr;							//テクスチャへのポインタ
	m_pVtxBuff = nullptr;							//頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//大きさ
	m_nIdxTexture = -1;
	m_bZtest = false;
	m_bAtest = false;
	m_bAbrend = false;
	m_bLight = false;
}

//==============================
// デストラクタ
//==============================
CObjectBillboard::~CObjectBillboard()
{

}

//==============================
// ビルボードの初期化処理
//==============================
HRESULT CObjectBillboard::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_size = D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE, BILLBOARD_SIZE);			//大きさ
	m_bZtest = false;
	m_bAtest = true;
	m_bAbrend = false;
	m_bLight = false;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	SetType(TYPE_BILLBOARD);

	return S_OK;
}

//==============================
// ビルボードの初期化処理
//==============================
HRESULT CObjectBillboard::Init(const D3DXVECTOR3 pos)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	m_pos = pos;			//位置
	m_size = D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE, BILLBOARD_SIZE);			//大きさ
	m_bZtest = false;
	m_bAtest = true;
	m_bAbrend = false;
	m_bLight = false;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}


	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	SetType(TYPE_BILLBOARD);

	return S_OK;
}

//==============================
// ビルボードの終了処理
//==============================
void CObjectBillboard::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;					//テクスチャ破棄
	}

	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;					//バッファの破棄
	}

	//死亡フラグを建てちゃう
	SetDeath(true);
	Release();
}

//==============================
// ビルボードの更新処理
//==============================
void CObjectBillboard::Update(void)
{

}

//==============================
// ビルボードの描画処理
//==============================
void CObjectBillboard::Draw(void)
{
	//テクスチャ取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;									//ビューマトリックス取得用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	//m_mtxWorld._21 = 0.0f;		//Y軸回転の打ち消し
	//m_mtxWorld._22 = 1.0f;		//rotのxyz?
	//m_mtxWorld._23 = 0.0f;

	m_mtxWorld._41 = 0.0f;		//全部引っ付く
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));
	//pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//描画する最初のインデックス
		2);												//描画するプリミティブ数

	//ライティングをTRUEに設定
	if (m_bLight == true)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	//Zテストを有効にする
	if (m_bZtest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	//アルファテストを無効にする
	if (m_bAtest == true)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
	}

	//aブレンディングを元に戻す
	if (m_bAbrend == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//============================
// 2Dポリゴンの生成処理
//============================
CObjectBillboard * CObjectBillboard::Create(void)
{
	CObjectBillboard *pObject3D;

	//オブジェクト生成
	pObject3D = new CObjectBillboard;

	//初期化
	pObject3D->Init();

	return pObject3D;
}

//============================
// 3Dポリゴンの生成処理
//============================
CObjectBillboard * CObjectBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, bool bZtest)
{
	CObjectBillboard *pBillBoard = nullptr;

	//オブジェクト生成&初期化
	pBillBoard = new CObjectBillboard;

	if (pBillBoard != nullptr)
	{
		pBillBoard->Init(pos);

		pBillBoard->m_bZtest = bZtest;

		pBillBoard->SetSize(size);

		pBillBoard->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pBillBoard;
}

//============================
// 位置設定
//============================
void CObjectBillboard::SetPos(const D3DXVECTOR3 pos)
{ 
	m_pos = pos; 
}

//============================
// 大きさ設定
//============================
void CObjectBillboard::SetSize(const D3DXVECTOR3 size)
{
	m_size = size; 

	VERTEX_3D *pVtx;		//頂点情報のポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, 0.0f);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================
// テクスチャ座標設定
//============================
void CObjectBillboard::SetTexPos(const D3DXVECTOR2 rightDown, const D3DXVECTOR2 leftUp)
{
	VERTEX_3D *pVtx;

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

//============================
// 色設定
//============================
void CObjectBillboard::SetColor(const D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報から色の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================
// 色取得
//============================
D3DXCOLOR CObjectBillboard::GetColor()
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報から色の取得
	D3DXCOLOR col = pVtx[0].col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return col;
}

//==========================================
//============= アニメーション =============
//============================
// コンストラクタ
//============================
CObjectBillboardAnim::CObjectBillboardAnim(int nPriority) : CObjectBillboard(nPriority)
{
	m_anim = nullptr;
}

//============================
// デストラクタ
//============================
CObjectBillboardAnim::~CObjectBillboardAnim()
{

}

//====================================
// 初期化処理(デフォ)
//====================================
HRESULT CObjectBillboardAnim::Init(void)
{
	//初期化
	CObjectBillboard::Init();

	m_anim = new CAnim;

	if (m_anim != nullptr)
	{
		m_anim->Init();
	}

	CObjectBillboard::SetTexPos(D3DXVECTOR2(1.0f / m_anim->GetNumPtnX(), 1.0f / m_anim->GetNumPtnY()), D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CObjectBillboardAnim::Init(const D3DXVECTOR3 pos)
{
	if(FAILED(CObjectBillboard::Init(pos)))
	{
		return E_FAIL;
	}

	m_anim = new CAnim;

	if (m_anim != nullptr)
	{
		m_anim->Init();
	}

	CObjectBillboard::SetTexPos(D3DXVECTOR2(1.0f / m_anim->GetNumPtnX(), 1.0f / m_anim->GetNumPtnY()), D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//============================
// 終了処理
//============================
void CObjectBillboardAnim::Uninit(void)
{
	if (m_anim != nullptr)
	{
		m_anim->Uninit();
		delete m_anim;
		m_anim = nullptr;
	}

	//終了
	CObjectBillboard::Uninit();
}

//============================
// 更新処理
//============================
void CObjectBillboardAnim::Update(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CObjectBillboard::GetVtxBuff();
	VERTEX_3D *pVtx;

	CObjectBillboard::Update();

	//アニメーション更新
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
// 描画処理
//============================
void CObjectBillboardAnim::Draw(void)
{
	//描画
	CObjectBillboard::Draw();
}

//============================
// アニメBBの生成処理
//============================
CObjectBillboardAnim * CObjectBillboardAnim::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CObjectBillboardAnim *pBillBoardAnim = nullptr;

	//オブジェクト生成&初期化
	pBillBoardAnim = new CObjectBillboardAnim;

	if (pBillBoardAnim != nullptr)
	{
		pBillBoardAnim->Init(pos);

		pBillBoardAnim->SetSize(size);

		pBillBoardAnim->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pBillBoardAnim;
}
