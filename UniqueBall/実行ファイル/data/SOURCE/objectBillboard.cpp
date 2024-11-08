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
CObjectBillboard::CObjectBillboard(int nPriority) : CObject3D(nPriority)
{
	m_pTexture = nullptr;							//テクスチャへのポインタ
	m_move = mylib_const::DEFVEC3;			//移動
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
	CObject3D::Init();

	SetSize(D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE, BILLBOARD_SIZE));			//大きさ
	SetNormal(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	SetAtest(true);

	SetType(TYPE_BILLBOARD);

	return S_OK;
}

//==============================
// ビルボードの初期化処理
//==============================
HRESULT CObjectBillboard::Init(const D3DXVECTOR3 pos)
{
	CObject3D::Init();

	SetPos(pos);			//大きさ
	SetSize(D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE, BILLBOARD_SIZE));			//大きさ
	SetNormal(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	SetAtest(true);

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

	CObject3D::Uninit();
}

//==============================
// ビルボードの更新処理
//==============================
void CObjectBillboard::Update(void)
{
	CObject3D::Update();
}

//==============================
// ビルボードの描画処理
//==============================
void CObjectBillboard::Draw(void)
{
	//テクスチャ取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = GetPos();
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = GetVtxBuff();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;									//ビューマトリックス取得用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&mtxWorld, NULL, &mtxView);

	//mtxWorld._21 = 0.0f;		//Y軸回転の打ち消し
	//mtxWorld._22 = 1.0f;		//rotのxyz?
	//mtxWorld._23 = 0.0f;

	mtxWorld._41 = 0.0f;		//全部引っ付く
	mtxWorld._42 = 0.0f;
	mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	SetMtxWorld(mtxWorld);

	//---------------------------------------------------
	//レンダラー
	DrawRenderSet(pDevice);
	//---------------------------------------------------

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(GetIdxTexture()));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//描画する最初のインデックス
		2);												//描画するプリミティブ数

	//---------------------------------------------------
	//レンダラー
	DrawRenderReset(pDevice);
	//---------------------------------------------------
}

//============================
// ビルボードの生成処理
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
// ビルボードの生成処理
//============================
CObjectBillboard * CObjectBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, bool bZtest)
{
	CObjectBillboard *pBillBoard = nullptr;

	//オブジェクト生成&初期化
	pBillBoard = new CObjectBillboard;

	if (pBillBoard != nullptr)
	{
		pBillBoard->Init(pos);

		pBillBoard->SetZtest(bZtest);

		pBillBoard->SetSize(size);

		pBillBoard->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pBillBoard;
}

//====================================
// 位置の設定
//====================================
void CObjectBillboard::SetSize(const D3DXVECTOR3 size)
{
	D3DXVECTOR3 Mysize = GetSize();
	Mysize = size;
	Mysize.z = 0.0f;

	SetSizeValue(size);

	VERTEX_3D* pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, 0.0f);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, 0.0f);

	//頂点バッファをアンロックする
	pVtxBuff->Unlock();
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

	if (m_anim->IsEnd())
	{
		SetDeath(true);
	}
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
