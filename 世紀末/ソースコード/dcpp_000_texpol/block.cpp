//==================================================
//
// ブロック処理(block.cpp)
// Author : Kai Takada
//
//==================================================
#include "block.h"
#include "manager.h"

//=======================
// マクロ定義
//=======================
#define PLAYER_EXCESSX (0.0f)						//プレイヤーのテクスチャの分の余分な幅消し

//=======================
// バイナリファイル名
//=======================
const char *c_apFileBlock[CBlock::TYPE_MAX] =
{
	"data\\MODEL\\BLOCK\\brick.x",		//通常ブロック
};

//=======================
// 静的メンバ変数宣言
//=======================
int CBlock::m_nNumAll = 0;											//現在使っているブロック数

//=======================
// コンストラクタ
//=======================
CBlock::CBlock(int nPriority) : CObjectX(nPriority)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_NORMAL;
}

//=======================
// デストラクタ
//=======================
CBlock::~CBlock()
{

}

//=======================
//ブロックの初期化処理
//=======================
HRESULT CBlock::Init(void)
{
	m_type = TYPE_NORMAL;

	CObjectX::Init();
	
	m_posOld = GetPos();
	
	SetType(TYPE_BLOCK);

	return S_OK;
}

//=======================
//ブロックの初期化処理
//=======================
HRESULT CBlock::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_NORMAL;

	CObjectX::Init(pos, rot);

	SetType(TYPE_BLOCK);

	return S_OK;
}

//=======================
//ブロックの終了処理
//=======================
void CBlock::Uninit(void)
{
	CObjectX::Uninit();
}

//=======================
//ブロックの更新処理
//=======================
void CBlock::Update(void)
{
	m_posOld = GetPos();

	CollisionRect();

	CObjectX::Update();
}

//=======================
//ブロックの描画処理
//=======================
void CBlock::Draw(void)
{
	CObjectX::Draw();
}

//=======================
//ブロックの設定処理
//=======================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CBlock *pBlock = nullptr;

	//オブジェクト生成&初期化
	pBlock = new CBlock;

	if (pBlock != nullptr)
	{
		if (FAILED(pBlock->Init(pos, rot)))
		{
			return nullptr;
		}
		else
		{
			pBlock->m_posOld = pos;

			CXModel *pXmodel = CManager::GetXModel();
			pBlock->SetIdxModel(pXmodel->Regist(c_apFileBlock[type], &vtxMin, &vtxMax));		//モデル割り当て
		
			pBlock->SetVtx(vtxMin, vtxMax);
		}
	}
	else
	{
		return nullptr;
	}

	return pBlock;
}

//============================
//ブロックの当たり判定(判定で押し戻す)
//============================
bool CBlock::CollisionRect(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 sizeMin, sizeMax;
	sizeMin = GetMinVtx();
	sizeMax = GetMaxVtx();

	D3DXVECTOR3 Objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjposOld = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 Objsize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Objmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	bool bLand = false;			//着地したかどうか
	bool Collision = false;			//当たり判定があったかどうか

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			bLand = false;
			Collision = false;

			CObject *pObj;

			//オブジェクトを取得
			pObj = CObject::GetObject(nCntPrt, nCntObj);

			if (pObj != nullptr)
			{//NULLチェック
				CObject::TYPE type = pObj->GetType();

				if (type == CObject::TYPE_PLAYER)
				{//プレイヤ―だったら
					Objpos = pObj->GetPos();
					ObjposOld = pObj->GetPosOld();
					//Objsize = pObj->GetSize();
					Objsize = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
					Objmove = pObj->GetMove();

					if (pObj->GetJump() == false)
					{
						if (ObjposOld.z + Objsize.z <= m_posOld.z - sizeMin.z
							&& Objpos.z + Objsize.z >= pos.z - sizeMin.z
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x + sizeMin.x + 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x - sizeMax.x - 0.1f)
						{//ブロック上
							//bLand = true;
							Objpos.z = pos.z - sizeMin.z - Objsize.z - 0.1f;		//ブロックの上に立たせる
							pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));			//移動量を0に
							Collision = true;
						}
						else if (ObjposOld.z - Objsize.z >= m_posOld.z + sizeMax.z
							&& Objpos.z - Objsize.z <= pos.z + sizeMax.z
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f)
						{//ブロック下
							Objpos.z = pos.z + sizeMax.z + Objsize.z;		//ブロックの下に立たせる
							pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));			//移動量を0に
							Collision = true;
						}

						if (ObjposOld.x + Objsize.x - PLAYER_EXCESSX <= m_posOld.x - sizeMin.x + 0.1f
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.z + Objsize.z > pos.z - sizeMin.z
							&& Objpos.z - Objsize.z < pos.z + sizeMax.z)
						{//ブロック左
							Objpos.x = pos.x - sizeMin.z - Objsize.x + PLAYER_EXCESSX;			//ブロックの左に立たせる
							pObj->SetMove(D3DXVECTOR3(0.0f, Objmove.z, Objmove.z));			//移動量を0に
							Collision = true;
						}
						else if (ObjposOld.x - Objsize.x + PLAYER_EXCESSX >= m_posOld.x + sizeMax.x - 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f
							&& Objpos.z + Objsize.z > pos.z - sizeMin.z
							&& Objpos.z - Objsize.z < pos.z + sizeMax.z)
						{//ブロック右
							Objpos.x = pos.x + sizeMax.x + Objsize.x - PLAYER_EXCESSX;		//ブロックの右に立たせる
							pObj->SetMove(D3DXVECTOR3(0.0f, Objmove.z, Objmove.z));			//移動量を0に
							Collision = true;
						}
					}
					else
					{
						if (ObjposOld.x + Objsize.x - PLAYER_EXCESSX <= m_posOld.x - sizeMin.x + 0.1f
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.z + Objsize.z > pos.z - sizeMin.z
							&& Objpos.z - Objsize.z < pos.z + sizeMax.z)
						{//ブロック左
							Objpos.x = pos.x - sizeMin.z - Objsize.x + PLAYER_EXCESSX;			//ブロックの左に立たせる
							pObj->SetMove(D3DXVECTOR3(0.0f, Objmove.z, Objmove.z));			//移動量を0に
							Collision = true;
						}
						else if (ObjposOld.x - Objsize.x + PLAYER_EXCESSX >= m_posOld.x + sizeMax.x - 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f
							&& Objpos.z + Objsize.z > pos.z - sizeMin.z
							&& Objpos.z - Objsize.z < pos.z + sizeMax.z)
						{//ブロック右
							Objpos.x = pos.x + sizeMax.x + Objsize.x - PLAYER_EXCESSX;		//ブロックの右に立たせる
							pObj->SetMove(D3DXVECTOR3(0.0f, Objmove.z, Objmove.z));			//移動量を0に
							Collision = true;
						}

						if (ObjposOld.z + Objsize.z <= m_posOld.z - sizeMin.z
							&& Objpos.z + Objsize.z >= pos.z - sizeMin.z
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f)
						{//ブロック上
							//bLand = true;
							Objpos.z = pos.z - sizeMin.z - Objsize.z - 0.1f;		//ブロックの上に立たせる
							pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));			//移動量を0に
							Collision = true;
						}
						else if (ObjposOld.z - Objsize.z >= m_posOld.z + sizeMax.z
							&& Objpos.z - Objsize.z <= pos.z + sizeMax.z
							&& Objpos.x + Objsize.x - PLAYER_EXCESSX >= pos.x - sizeMin.x + 0.1f
							&& Objpos.x - Objsize.x + PLAYER_EXCESSX <= pos.x + sizeMax.x - 0.1f)
						{//ブロック下
							Objpos.z = pos.z + sizeMax.z + Objsize.z;		//ブロックの下に立たせる
							pObj->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));			//移動量を0に
							Collision = true;
						}
					}
				}

				if (Collision)
				{
					pObj->SetPos(Objpos);
				}

				if (bLand)
				{
					pObj->SetJump(!bLand);
				}
			}
		}
	}

	return bLand;
}
