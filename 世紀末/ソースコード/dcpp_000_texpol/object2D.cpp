//===============================================
//
// 2D|S(object2D.cpp)
// Author ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "object2D.h"
#include "texture.h"
#include "Anim.h"

//============================
// }Nè`
//============================

//============================
// Oûé¾
//============================
//class CPlayer;

//============================
// ÃIoÏé¾
//============================

//============================
// RXgN^(ftH)
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
}

//============================
// fXgN^
//============================
CObject2D::~CObject2D()
{

}

//====================================
// 2D|SÌú»(ftH)
//====================================
HRESULT CObject2D::Init(void)
{
	//TCYÝè
	m_size = D3DXVECTOR3(DEF_SIZE_STATIC.x, DEF_SIZE_STATIC.y, 0.0f);

	//ÎpüÌ·³ðZo·é
	m_fLength = sqrtf((m_size.x * m_size.x * 4) + (m_size.y * m_size.y * 4)) * 0.5f;

	//ÎpüÌpxðZo·é
	m_fAngle = atan2f(m_size.x * 2, m_size.y * 2);

	//foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//¸_obt@Ì¶¬
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{//¸sµ½ê
		return E_FAIL;
	}

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//¸_ÀWÌÝè
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

	//rhwÌÝè
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//¸_J[ÌÝè
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//eNX`ÀWÌÝè
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();

	return S_OK;
}

//====================================
// 2D|SÌú»(ÊuIo)
//====================================
HRESULT CObject2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	//ÊuÝè
	m_pos = pos;

	//ü«Ýè
	m_rot = rot;

	//TCYÝè
	m_size = D3DXVECTOR3(DEF_SIZE_STATIC.x, DEF_SIZE_STATIC.y, 0.0f);

	//ÎpüÌ·³ðZo·é
	m_fLength = sqrtf((m_size.x * m_size.x * 4) + (m_size.y * m_size.y * 4)) * 0.5f;

	//ÎpüÌpxðZo·é
	m_fAngle = atan2f(m_size.x * 2, m_size.y * 2);

	//foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//¸_obt@Ì¶¬
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{//¸sµ½ê
		return E_FAIL;
	}

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//¸_ÀWÌÝè
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

	//rhwÌÝè
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//¸_J[ÌÝè
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//eNX`ÀWÌÝè
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================
// 2D|SÌI¹
//============================
void CObject2D::Uninit(void)
{
	//¸_obt@jü
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//StOðÄ¿á¤
	SetDeath(true);
	//©gÌjü
	Release();
}

//============================
// 2D|SÌXV
//============================
void CObject2D::Update(void)
{

}

//============================
// 2D|SÌ`æ
//============================
void CObject2D::Draw(void)
{
	//foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//eNX`æ¾
	CTexture *pTexture = CManager::GetTexture();

	//¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);

	//aufBOðÁZ¬ÉÝè
	if (m_bAbrend == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//eNX`ÌÝè
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//2D|SÌ`æ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				//v~eBuÌíÞ
		0,															//æªv~eBuÔ
		2);															//`æ·év~eBu

	//aufBOð³Éß·
	if (m_bAbrend == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//============================
// 2D|SÌÊuÝè
//============================
void CObject2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//¸_ÀWÌÝè
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

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();
}

//============================
// 2D|SÌü«Ýè
//============================
void CObject2D::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//¸_ÀWÌÝè
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

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();
}

//============================
// 2D|SÌTCYÝè
//============================
void CObject2D::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;

	//ÎpüÌ·³ðZo·é
	m_fLength = sqrtf((size.x * size.x * 4) + (size.y * size.y * 4)) * 0.5f;

	//ÎpüÌpxðZo·é
	m_fAngle = atan2f(size.x * 2, size.y * 2);

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//¸_ÀWÌÝè
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

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();
}

//============================
// eNX`ÀWÝè
//============================
void CObject2D::SetTexPos(const D3DXVECTOR2 rightDown, const D3DXVECTOR2 leftUp)
{
	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//eNX`ÀWÌÝè
	pVtx[0].tex = D3DXVECTOR2(leftUp.x, leftUp.y);
	pVtx[1].tex = D3DXVECTOR2(rightDown.x, leftUp.y);
	pVtx[2].tex = D3DXVECTOR2(leftUp.x, rightDown.y);
	pVtx[3].tex = D3DXVECTOR2(rightDown.x, rightDown.y);

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();
}

//============================
// 2D|SÌ¶¬
//============================
CObject2D * CObject2D::Create(void)
{
	CObject2D *pObject2D;

	//IuWFNg¶¬
	pObject2D = new CObject2D;

	//ú»
	pObject2D->Init();

	return pObject2D;
}

//============================
// 2D|SÌ¶¬
//============================
CObject2D * CObject2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size, const UPDATE_TYPE UpdType)
{
	CObject2D *pObject2D = nullptr;

	//IuWFNg¶¬&ú»
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

//==========================================
//============= Aj[V =============
//============================
// RXgN^
//============================
CObject2DAnim::CObject2DAnim(int nPriority) : CObject2D(nPriority)
{
	m_anim = nullptr;
}

//============================
// fXgN^
//============================
CObject2DAnim::~CObject2DAnim()
{

}

//====================================
// 2D|SÌú»(ftH)
//====================================
HRESULT CObject2DAnim::Init(void)
{
	//ú»
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
// 2D|SÌú»(ÊuIo)
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
// 2D|SÌI¹
//============================
void CObject2DAnim::Uninit(void)
{
	if (m_anim != nullptr)
	{
		m_anim->Uninit();
		delete m_anim;
	}

	//I¹
	CObject2D::Uninit();
}

//============================
// 2D|SÌXV
//============================
void CObject2DAnim::Update(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CObject2D::GetVtxBuff();
	VERTEX_2D*pVtx;

	m_anim->Update();
	D3DXVECTOR2 *pTexpos = m_anim->GetTexpos();

	//eNX`ÌXV
	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//eNX`ÀWÌÝè
	for (int nCntTex = 0; nCntTex < MAX_VTX; nCntTex++)
	{
		pVtx[nCntTex].tex = pTexpos[nCntTex];		//(x,y)
	}

	//¸_obt@ðAbN·é
	pVtxBuff->Unlock();
}

//============================
// 2D|SÌ`æ
//============================
void CObject2DAnim::Draw(void)
{
	//`æ
	CObject2D::Draw();
}

//============================
// eNX`ÀWÝè(¶ã)
//============================
D3DXVECTOR2 CObject2D::GetTexPosleftUp(void)
{
	D3DXVECTOR2 texPosleftUp;

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//eNX`ÀWÌÝè
	texPosleftUp = pVtx[0].tex;

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();

	return texPosleftUp;
}

//============================
// eNX`ÀWÝè(Eº)
//============================
D3DXVECTOR2 CObject2D::GetTexPosrightDown(void)
{
	D3DXVECTOR2 texPosrightDown;

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//eNX`ÀWÌÝè
	texPosrightDown = pVtx[3].tex;

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();

	return texPosrightDown;
}

//============================
// eNX`ÀWÝè(Eº - ¶ã)
//============================
D3DXVECTOR2 CObject2D::GetTexPosDif(void)
{
	D3DXVECTOR2 texPosDif;

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//eNX`ÀWÌÝè
	texPosDif = pVtx[0].tex - pVtx[3].tex;

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();

	return texPosDif;
}

//============================
// ¸_J[æ¾
//============================
D3DXCOLOR CObject2D::GetTexCol(void)
{
	D3DXCOLOR col;

	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//eNX`ÀWÌÝè
	col = pVtx[0].col;

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();

	return col;
}

//============================
// ¸_J[Ýè
//============================
void CObject2D::SetTexCol(D3DXCOLOR col)
{
	VERTEX_2D*pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//eNX`ÀWÌÝè
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//¸_obt@ðAbN·é
	m_pVtxBuff->Unlock();
}
