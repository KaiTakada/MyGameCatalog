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
	
	CXModel *pXmodel = CManager::GetInstance()->GetXModel();

	D3DXVECTOR3 vtxMin = pXmodel->GetAddress(GetIdxModel())->vtxMin;
	D3DXVECTOR3 vtxMax = pXmodel->GetAddress(GetIdxModel())->vtxMax;
	SetVtx(vtxMin, vtxMax);

	SetType(TYPE_BLOCK);

	return S_OK;
}

//=======================
//ブロックの初期化処理
//=======================
HRESULT CBlock::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE_Blk type)
{
	CObjectX::Init(pos, rot);

	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = type;

	D3DXVECTOR3 vtxMin = DEF_VEC3;
	D3DXVECTOR3 vtxMax = DEF_VEC3;

	CXModel *pXmodel = CManager::GetInstance()->GetXModel();
	SetIdxModel(pXmodel->Regist(c_apFileBlock[type], &vtxMin, &vtxMax));		//モデル割り当て

	vtxMin = pXmodel->GetAddress(GetIdxModel())->vtxMin;
	vtxMax = pXmodel->GetAddress(GetIdxModel())->vtxMax;
	SetVtx(vtxMin, vtxMax);

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
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE_Blk type)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CBlock *pBlock = nullptr;

	//オブジェクト生成&初期化
	switch (type)
	{
	case TYPE_NORMAL:
	default:
		pBlock = new CBlock;
		break;
	}


	if (pBlock != nullptr)
	{
		if (FAILED(pBlock->Init(pos, rot, type)))
		{
			return nullptr;
		}
		else
		{
			pBlock->m_posOld = pos;
			pBlock->m_type = type;
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
	D3DXVECTOR3 sizeMin = GetMinVtx();		//最小値
	D3DXVECTOR3 sizeMax = GetMaxVtx();		//最大値
	D3DXVECTOR3 move = mylib_const::DEFVEC3;

	D3DXVECTOR3 posC = mylib_const::DEFVEC3;
	D3DXVECTOR3 posOldC = mylib_const::DEFVEC3;
	D3DXVECTOR3 sizeC = mylib_const::DEFVEC3;
	D3DXVECTOR3 moveC = mylib_const::DEFVEC3;
	bool bJumpC = false;

	bool bCollision = false;			//当たり判定があったかどうか

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject* pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_PLAYER)
				{//プレイヤ―だったら
					posC = pObject->GetPos();
					posOldC = pObject->GetPosOld();
					sizeC = pObject->GetSize();
					moveC = pObject->GetMove();
					bJumpC = pObject->GetJump();

					bCollision = mylib_useful::CollisionBlock(pos, pos, move, sizeMin,
						&posC, posOldC, &moveC, sizeC, &bJumpC);
				
					pObject->SetPos(posC);
					pObject->SetMove(moveC);
					pObject->SetJump(bJumpC);
				}
			}

		}
	}

	return bCollision;
}
