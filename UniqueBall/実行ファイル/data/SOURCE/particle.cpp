//===============================================
//
// パーティクル(particle.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "particle.h"
#include "effect.h"
#include "debugproc.h"

//============================
// マクロ定義
//============================

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ(デフォ)
//============================
CParticle::CParticle(int nPriority) : CObject(nPriority)
{
	for (int nCntCrt = 0; nCntCrt < MAX_PARTICLE; nCntCrt++)
	{
		if (m_apEffect[nCntCrt] != nullptr)
		{
			m_apEffect[nCntCrt] = nullptr;
		}
	}

	m_nLife = 0;
}

//============================
// デストラクタ
//============================
CParticle::~CParticle()
{

}
//====================================
// 初期化処理(デフォ)
//====================================
HRESULT CParticle::Init(void)
{
	for (int nCntCrt = 0; nCntCrt < MAX_PARTICLE; nCntCrt++)
	{
		if (m_apEffect[nCntCrt] == nullptr)
		{
			m_apEffect[nCntCrt] = CEffect::Create();		//自身を代入

			if (m_apEffect[nCntCrt] != nullptr)
			{
				m_apEffect[nCntCrt]->Init();
			}

			break;
		}
	}

	return S_OK;
}

//====================================
// 初期化処理(デフォ)
//====================================
HRESULT CParticle::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR3 move, const D3DXVECTOR3 size, TYPE type)
{
	int nNum = 0;
	int nLife = 0;
	D3DXVECTOR3 vec = rot * 100.0f;
	D3DXVECTOR3 moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//タイプで
	switch (type)
	{
	case TYPE_EXPLODE:

		nNum = 30;
		nLife = 20;

		break;

	case TYPE_SMOKE:

		nNum = 30;
		nLife = 40;

		if (vec.x < 1.0f && vec.x > -1.0f)
		{
			vec.x = 1.0f;
		}
		if (vec.y < 1.0f && vec.y > -1.0f)
		{
			vec.y = 1.0f;
		}
		if (vec.z < 1.0f && vec.z > -1.0f)
		{
			vec.z = 1.0f;
		}

		vec *= 100.0f;

		break;

	default:

		nNum = 10;
		nLife = 10;

		break;
	}

	for (int nCntCrt = 0; nCntCrt < nNum; nCntCrt++)
	{
		if (nCntCrt >= MAX_PARTICLE)
		{
			break;
		}

		//タイプで
		switch (type)
		{
		case TYPE_EXPLODE:
			moveR.x = sinf((float)(rand() % 629 - 314) / 100.0f) * move.x;
			moveR.y = cosf((float)(rand() % 629 - 314) / 100.0f) * move.y;
			moveR.z = sinf((float)(rand() % 629 - 314) / 100.0f) * move.z;
			break;

		case TYPE_SMOKE:
			moveR.x = sinf((float)(rand() % int(vec.x) - 79) / 100.0f) * move.x;
			moveR.y = cosf((float)(rand() % int(vec.y) - 79) / 100.0f) * move.y;
			moveR.z = sinf((float)(rand() % int(vec.z) - 79) / 100.0f) * move.z;
			break;

		default:
			moveR.x = sinf((float)(rand() % 629 - 314) / 100.0f) * move.x;
			moveR.y = cosf((float)(rand() % 629 - 314) / 100.0f) * move.y;
			moveR.z = sinf((float)(rand() % 629 - 314) / 100.0f) * move.z;
			break;
		}

		//CEffect::Create(pos, rot, moveR, size, nLife, CEffect::TEX_000);		//自身を代入
		m_apEffect[nCntCrt] = CEffect::Create(pos, rot, moveR, size, nLife, CEffect::TEX_000);		//自身を代入
		m_apEffect[nCntCrt]->SetLight(true);

		if (type == TYPE_SMOKE)
		{
			m_apEffect[nCntCrt]->SetColor(D3DXCOLOR(1.0f, 0.1f, 0.5f, 1.0f));
		}
	}

	SetType(TYPE_PARTICLE);

	return S_OK;
}

//====================================
// 初期化処理(オバロ)
//====================================
HRESULT CParticle::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	pos;
	rot;

	return S_OK;
}

//============================
// 終了処理
//============================
void CParticle::Uninit(void)
{
	for (int nCntCrt = 0; nCntCrt < MAX_PARTICLE; nCntCrt++)
	{
		if (m_apEffect[nCntCrt] != nullptr)
		{
			m_apEffect[nCntCrt] = nullptr;
		}
	}

	if (m_apEffect != nullptr)
	{
	}

	CObject::Release();
}

//============================
// 更新処理
//============================
void CParticle::Update(void)
{
	if (m_nLife <= 0)
	{
		Uninit();
	}
	else
	{
		m_nLife--;
	}
}

//============================
// 描画処理
//============================
void CParticle::Draw(void)
{

}

//==========================
// 生成処
//==========================
CParticle *CParticle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const D3DXVECTOR3 size, TYPE m_type)
{
	CParticle *pParticle = nullptr;

	pParticle = new CParticle;

	if (pParticle != nullptr)
	{
		pParticle->Init(pos,rot,move,size,m_type);
		pParticle->m_nLife = 1;
	}
	else
	{
		return nullptr;
	}

	return pParticle;
}

//============================
// 破棄
//============================
void CParticle::Release(int nID)
{
	nID;
}

//============================
// 位置設定
//============================
void CParticle::SetPos(const D3DXVECTOR3 pos)
{
	pos;
}

//============================
// 向き設定
//============================
void CParticle::SetRot(const D3DXVECTOR3 rot)
{
	rot;
}

//============================
// 位置取得
//============================
D3DXVECTOR3 CParticle::GetPos(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
// 2Dサイズ取得
//============================
D3DXVECTOR3 CParticle::GetSize(void)
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//============================
// 色設定
//============================
void CParticle::SetColor(D3DXCOLOR col)
{
	int nSize = sizeof(m_apEffect) / sizeof(CParticle*);

	for (int nCntCrt = 0; nCntCrt < nSize; nCntCrt++)
	{
		if (m_apEffect[nCntCrt] != nullptr)
		{
			m_apEffect[nCntCrt]->SetColor(col);
		}
	}
}

//============================
// サイズ設定
//============================
void CParticle::SetSize(D3DXVECTOR3 size)
{
	int nSize = sizeof(m_apEffect) / sizeof(CParticle*);

	for (int nCntCrt = 0; nCntCrt < nSize; nCntCrt++)
	{
		if (m_apEffect[nCntCrt] != nullptr)
		{
			m_apEffect[nCntCrt]->SetSize(size);
		}
	}
}
