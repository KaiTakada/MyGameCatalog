//===============================================
//
// 2Dポリゴン(object2D.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "obj_polygon2D.h"
#include "texture.h"
#include "Anim.h"

//============================
// マクロ定義
//============================

//============================
// 前方宣言
//============================
//class CPlayer;

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ(デフォ)
//============================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_nIdxTexture = 0;
	m_bAbrend = false;
	m_bAppear = true;
}

//============================
// デストラクタ
//============================
CObject2D::~CObject2D()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CObject2D::Init(void)
{
	//サイズ設定
	m_size = D3DXVECTOR3(DEF_SIZE_STATIC.x, DEF_SIZE_STATIC.y, 0.0f);

	//対角線の長さを算出する
	m_fLength = sqrtf((m_size.x * m_size.x * 4) + (m_size.y * m_size.y * 4)) * 0.5f;

	//対角線の角度を算出する
	m_fAngle = atan2f(m_size.x * 2, m_size.y * 2);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{//失敗した場合
		return E_FAIL;
	}

	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(x)
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(y)
	pVtx[0].pos.z = 0.0f;	//(z)

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CObject2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//位置設定
	m_pos = pos;

	//向き設定
	m_rot = rot;

	//サイズ設定
	m_size = D3DXVECTOR3(DEF_SIZE_STATIC.x, DEF_SIZE_STATIC.y, 0.0f);

	//対角線の長さを算出する
	m_fLength = sqrtf((m_size.x * m_size.x * 4) + (m_size.y * m_size.y * 4)) * 0.5f;

	//対角線の角度を算出する
	m_fAngle = atan2f(m_size.x * 2, m_size.y * 2);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{//失敗した場合
		return E_FAIL;
	}

	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(x)
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(y)
	pVtx[0].pos.z = 0.0f;	//(z)

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CObject2D::Uninit(void)
{
	//頂点バッファ破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//死亡フラグを建てちゃう
	SetDeath(true);
	//自身の破棄
	Release();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CObject2D::Update(void)
{
	SetSize(GetSize());
}

//============================
// 2Dポリゴンの描画処理
//============================
void CObject2D::Draw(void)
{
	if (!m_bAppear)
	{
		return;
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//テクスチャ取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//aブレンディングを加算合成に設定
	if (m_bAbrend == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//2Dポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//プリミティブの種類
		0,															//先頭プリミティブ番号
		2);															//描画するプリミティブ数

	//aブレンディングを元に戻す
	if (m_bAbrend == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//============================
// 2Dポリゴンの位置設定処理
//============================
void CObject2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(x)
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(y)
	pVtx[0].pos.z = 0.0f;	//(z)

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================
// 2Dポリゴンの向き設定処理
//============================
void CObject2D::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;

	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(x)
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(y)
	pVtx[0].pos.z = 0.0f;	//(z)
					
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;
					
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;
					
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================
// 2Dポリゴンのサイズ設定処理
//============================
void CObject2D::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;

	//対角線の長さを算出する
	m_fLength = sqrtf((size.x * size.x * 4) + (size.y * size.y * 4)) * 0.5f;

	//対角線の角度を算出する
	m_fAngle = atan2f(size.x * 2, size.y * 2);

	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(x)
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - D3DX_PI + m_fAngle) * m_fLength;	//(y)
	pVtx[0].pos.z = 0.0f;	//(z)

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI - m_fAngle) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================
// テクスチャ座標設定
//============================
void CObject2D::SetTexPos(const D3DXVECTOR2 rightDown, const D3DXVECTOR2 leftUp)
{
	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(leftUp.x, leftUp.y);
	pVtx[1].tex = D3DXVECTOR2(rightDown.x, leftUp.y);
	pVtx[2].tex = D3DXVECTOR2(leftUp.x, rightDown.y);
	pVtx[3].tex = D3DXVECTOR2(rightDown.x, rightDown.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================
// 2Dポリゴンの生成処理
//============================
CObject2D * CObject2D::Create(void)
{
	CObject2D *pObject2D;

	//オブジェクト生成
	pObject2D = new CObject2D;

	//初期化
	pObject2D->Init();

	return pObject2D;
}

//============================
// 2Dポリゴンの生成処理
//============================
CObject2D * CObject2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const UPDATE_TYPE UpdType)
{
	CObject2D *pObject2D = nullptr;

	//オブジェクト生成&初期化
	switch (UpdType)
	{
	case UPDATE_TYPE_ANIM:
	
		pObject2D = new CObject2DAnim;

		break;

	default:
		
		pObject2D = new CObject2D;

		break;
	}

	if (pObject2D != nullptr)
	{
		pObject2D->Init(pos, rot);
		
		pObject2D->SetSize(size);

		pObject2D->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pObject2D;
}

//============================
// テクスチャ座標設定(左上)
//============================
D3DXVECTOR2 CObject2D::GetTexPosleftUp(void)
{
	D3DXVECTOR2 texPosleftUp;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	texPosleftUp = pVtx[0].tex;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return texPosleftUp;
}

//============================
// テクスチャ座標設定(右下)
//============================
D3DXVECTOR2 CObject2D::GetTexPosrightDown(void)
{
	D3DXVECTOR2 texPosrightDown;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	texPosrightDown = pVtx[3].tex;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return texPosrightDown;
}

//============================
// テクスチャ座標設定(右下 - 左上)
//============================
D3DXVECTOR2 CObject2D::GetTexPosDif(void)
{
	D3DXVECTOR2 texPosDif;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	texPosDif = pVtx[0].tex - pVtx[3].tex;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return texPosDif;
}

//============================
// 頂点カラー取得
//============================
D3DXCOLOR CObject2D::GetTexCol(void)
{
	D3DXCOLOR col;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	col = pVtx[0].col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return col;
}

//============================
// 頂点カラー設定
//============================
void CObject2D::SetTexCol(D3DXCOLOR col)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================================
//============= アニメーション =============
//============================
// コンストラクタ
//============================
CObject2DAnim::CObject2DAnim(int nPriority) : CObject2D(nPriority)
{
	m_anim = nullptr;
}

//============================
// デストラクタ
//============================
CObject2DAnim::~CObject2DAnim()
{

}

//====================================
// 2Dポリゴンの初期化処理(デフォ)
//====================================
HRESULT CObject2DAnim::Init(void)
{
	//初期化
	CObject2D::Init();
	
	m_anim = new CAnim;

	if (m_anim != nullptr)
	{
		m_anim->Init();
	
		m_anim->SetNumLoop(0);
		m_anim->SetNumPtnX(1);
		m_anim->SetNumPtnY(1);
	}

	CObject2D::SetTexPos(D3DXVECTOR2(0.2f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//====================================
// 2Dポリゴンの初期化(位置オバロ)
//====================================
HRESULT CObject2DAnim::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject2D::Init();

	CObject2D::SetPos(pos);
	CObject2D::SetRot(rot);
	CObject2D::SetSize(D3DXVECTOR3(DEF_SIZE_ANIM.x, DEF_SIZE_ANIM.y, 0.0f));

	m_anim = new CAnim;

	if (m_anim != nullptr)
	{
		m_anim->Init();

		m_anim->SetNumLoop(0);
		m_anim->SetNumPtnX(1);
		m_anim->SetNumPtnY(1);
	}
	
	CObject2D::SetTexPos(D3DXVECTOR2(1.0f / m_anim->GetNumPtnX(), 1.0f / m_anim->GetNumPtnY()), D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//============================
// 2Dポリゴンの終了処理
//============================
void CObject2DAnim::Uninit(void)
{
	if (m_anim != nullptr)
	{
		m_anim->Uninit();
		delete m_anim;
	}

	//終了
	CObject2D::Uninit();
}

//============================
// 2Dポリゴンの更新処理
//============================
void CObject2DAnim::Update(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CObject2D::GetVtxBuff();
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
void CObject2DAnim::Draw(void)
{
	//描画
	CObject2D::Draw();
}
