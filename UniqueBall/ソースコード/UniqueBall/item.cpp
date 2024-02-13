//==================================================
//
// アイテム(item.cpp)
// Author : Kai Takada
//
//==================================================
#include "item.h"
//#include "object2D.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "game.h"

//=======================
// マクロ定義
//=======================
#define PLAYER_EXCESSX (0.0f)						//プレイヤーのテクスチャの分の余分な幅消し

//=======================
// バイナリファイル名
//=======================
static const char *c_apFileItem[CItem::TYPE_MAX] =
{
	"data\\TEXTURE\\ITEM\\exp.png",		//通常ブロック
};

//=======================
// 静的メンバ変数宣言
//=======================
LPDIRECT3DTEXTURE9 CItem::m_pTexture[TYPE_MAX] = {};			//テクスチャへのポインタ
int CItem::m_nNumAll = 0;											//現在使っているブロック数

//=======================
// コンストラクタ
//=======================
CItem::CItem(int nPriority) : CObjectBillboard(nPriority)
{
	m_type = TYPE_EXP;
}

//=======================
// デストラクタ
//=======================
CItem::~CItem()
{

}

//=======================
// 初期化処理
//=======================
HRESULT CItem::Init(void)
{
	m_type = TYPE_EXP;

	CObjectBillboard::Init();
		
	SetType(TYPE_ITEM);

	return S_OK;
}

//=======================
// 初期化処理
//=======================
HRESULT CItem::Init(const D3DXVECTOR3 pos)
{
	m_type = TYPE_EXP;

	CObjectBillboard::Init(pos);

	SetType(TYPE_ITEM);

	return S_OK;
}

//=======================
// 終了処理
//=======================
void CItem::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//=======================
// 更新処理
//=======================
void CItem::Update(void)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 pos = GetPos();

	pos += move;

	SetPos(pos);

	if (CollisionBall())
	{//入る=本体uninit済み
		return;
	}

	CObjectBillboard::Update();
}

//=======================
// 描画処理
//=======================
void CItem::Draw(void)
{
	CObjectBillboard::Draw();
}

//=======================
// 設定処理
//=======================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type)
{
	CItem *pItem = nullptr;

	//オブジェクト生成&初期化
	pItem = new CItem;

	if (pItem != nullptr)
	{
		if (FAILED(pItem->Init(pos)))
		{
			return nullptr;
		}
		else
		{
			pItem->SetSize(size);		//サイズ設定

			CTexture *pTexture = CManager::GetInstance()->GetTexture();
			pItem->SetIdxTexture(pTexture->Regist(c_apFileItem[type]));
		}
	}
	else
	{
		return nullptr;
	}

	return pItem;
}

//============================
// 当たり判定 (球と球)
//============================
bool CItem::CollisionBall(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		CPlayer *pPlayer = CScene::GetPlayer(i);

		if (pPlayer != nullptr)
		{//プレイヤー側を向く
			Objsize = pPlayer->GetSize();
			Objpos = pPlayer->GetPos();
			Objpos.y += pPlayer->GetHeartPos();

			float fSize0 = (size.x + size.z) * 0.5f;		//アイテムの半径xz
			float fSize1 = (size.x + size.y) * 0.5f;		//アイテムの半径xy
			float fSize2 = (size.z + size.y) * 0.5f;		//アイテムの半径zy

			float fObjsize0 = (Objsize.x + Objsize.z) * 0.5f;		//オブジェクトの半径xz
			float fObjsize1 = (Objsize.x + Objsize.y) * 0.5f;		//オブジェクトの半径xy
			float fObjsize2 = (Objsize.z + Objsize.y) * 0.5f;		//オブジェクトの半径zy

			float fColl0 = fSize0 + fObjsize0;		//当たり判定範囲
			float fColl1 = fSize1 + fObjsize1;		//当たり判定範囲
			float fColl2 = fSize2 + fObjsize2;		//当たり判定範囲

			float fLength0 = hypotf((pos.x - Objpos.x), (pos.z - Objpos.z));		//2点間の長さxz
			float fLength1 = hypotf((pos.x - Objpos.x), (pos.y - Objpos.y));		//2点間の長さxy
			float fLength2 = hypotf((pos.z - Objpos.z), (pos.y - Objpos.y));		//2点間の長さzy

			if (fLength0 <= fColl0 &&
				fLength1 <= fColl1 &&
				fLength2 <= fColl2)
			{
				//弾終了
				Uninit();

				return TRUE;
			}
		}
	}

	return FALSE;
}
