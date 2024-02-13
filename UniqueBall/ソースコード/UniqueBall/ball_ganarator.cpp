//==================================================
//
// ブロック処理(block.cpp)
// Author : Kai Takada
//
//==================================================
#include "block.h"
#include "manager.h"

#include "ball_ganarator.h"
#include "wp_ball.h"

//=======================
// 定数定義
//=======================
namespace
{
	const char *FILEPASS = "data\\MODEL\\BLOCK\\ball_generator.x";
	const int GENERATECNT = 300;
}

//=======================
// コンストラクタ
//=======================
CBallGenerator::CBallGenerator(int nPriority) : CObjectX(nPriority)
{
	m_nGenerateCtr = 0;
	m_pBall = nullptr;
}

//=======================
// デストラクタ
//=======================
CBallGenerator::~CBallGenerator()
{

}

//=======================
//ブロックの初期化処理
//=======================
HRESULT CBallGenerator::Init(void)
{
	CObjectX::Init();
	m_pBall = nullptr;

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
HRESULT CBallGenerator::Init(const D3DXVECTOR3 pos)
{
	CObjectX::Init(pos, mylib_const::DEFVEC3);
	m_pBall = nullptr;

	D3DXVECTOR3 vtxMin = mylib_const::DEFVEC3;
	D3DXVECTOR3 vtxMax = mylib_const::DEFVEC3;

	CXModel *pXmodel = CManager::GetInstance()->GetXModel();
	SetIdxModel(pXmodel->Regist(FILEPASS, &vtxMin, &vtxMax));		//モデル割り当て

	vtxMin = pXmodel->GetAddress(GetIdxModel())->vtxMin;
	vtxMax = pXmodel->GetAddress(GetIdxModel())->vtxMax;
	SetVtx(vtxMin, vtxMax);

	SetType(TYPE_BALL_GENERATOR);

	return S_OK;
}

//=======================
//ブロックの終了処理
//=======================
void CBallGenerator::Uninit(void)
{
	CObjectX::Uninit();
}

//=======================
//ブロックの更新処理
//=======================
void CBallGenerator::Update(void)
{
	if (mylib_const::MAX_PLAYER * 2 > CBall::GetNumAll() &&
		m_pBall == nullptr)
	{
		if (m_nGenerateCtr >= GENERATECNT)
		{
			//生成
			CBall *pBall = CBall::Create(GetPos());
			m_pBall = pBall;
		}
		else
		{
			m_nGenerateCtr++;
		}
	}
	else
	{
		m_nGenerateCtr = 0;
	}

	if (m_pBall != nullptr)
	{
		if (m_pBall->GetMember() != my_Identity::MEMBER_NONE)
		{//他の所属になったら手放す
			m_pBall = nullptr;
		}
		else
		{//
			D3DXVECTOR3 pos = GetPos();
			pos.y += m_pBall->GetMaxVtx().y;
			m_pBall->SetPos(pos);
			m_pBall->SetDeathCtr(100);
		}
	}

	//CollisionRect();

	CObjectX::Update();
}

//=======================
//ブロックの描画処理
//=======================
void CBallGenerator::Draw(void)
{
	CObjectX::Draw();
}

//=======================
//ブロックの設定処理
//=======================
CBallGenerator *CBallGenerator::Create(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CBallGenerator *pBlock = nullptr;

	pBlock = new CBallGenerator;

	if (pBlock != nullptr)
	{
		if (FAILED(pBlock->Init(pos)))
		{
			return nullptr;
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
bool CBallGenerator::CollisionRect(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 sizeMin = GetMinVtx();		//最小値
	D3DXVECTOR3 sizeMax = GetMaxVtx();		//最大値

	D3DXVECTOR3 posC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 posOldC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 sizeC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 moveC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//押し戻す分

	bool bLand = false;			//着地したかどうか
	bool bCollision = false;			//当たり判定があったかどうか

	//x
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

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

					D3DXVECTOR3 sizeOldMinC = D3DXVECTOR3(posOldC.x - sizeC.x, posOldC.y, posOldC.z - sizeC.z);		//キャラ最小値
					D3DXVECTOR3 sizeOldMaxC = D3DXVECTOR3(posOldC.x + sizeC.x, posOldC.y + (sizeC.y * 2), posOldC.z + sizeC.z);		//キャラ最大値

					D3DXVECTOR3 sizeMinC = D3DXVECTOR3(posC.x - sizeC.x, posC.y, posC.z - sizeC.z);		//キャラ最小値
					D3DXVECTOR3 sizeMaxC = D3DXVECTOR3(posC.x + sizeC.x, posC.y + (sizeC.y * 2), posC.z + sizeC.z);		//キャラ最大値

					if (pObject->GetJump() == false)
					{
						if (sizeOldMaxC.x <= pos.x + sizeMin.x
							&& sizeMaxC.x > pos.x + sizeMin.x
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//ブロック西
							move.x = (pos.x + sizeMin.x) - (sizeMaxC.x) - 0.1f;
							bCollision = true;
						}
						else if (sizeOldMinC.x >= pos.x + sizeMax.x
							&& sizeMinC.x <= pos.x + sizeMax.x
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//ブロック左
							move.x = (pos.x + sizeMax.x) - (sizeMinC.x) + 0.1f;
							bCollision = true;
						}
					}
					else
					{
						if (sizeOldMaxC.x <= pos.x + sizeMin.x
							&& sizeMaxC.x > pos.x + sizeMin.x
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//ブロック西
							move.x = (pos.x + sizeMin.x) - (sizeMaxC.x) - 0.1f;
							bCollision = true;
						}
						else if (sizeOldMinC.x >= pos.x + sizeMax.x
							&& sizeMinC.x <= pos.x + sizeMax.x
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//ブロック左
							move.x = (pos.x + sizeMax.x) - (sizeMinC.x) + 0.1f;
							bCollision = true;
						}
					}
				}

				if (bCollision)
				{
					pObject->SetPos(posC + move);
					bCollision = false;
					break;
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	//y
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

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

					D3DXVECTOR3 sizeOldMinC = D3DXVECTOR3(posOldC.x - sizeC.x, posOldC.y, posOldC.z - sizeC.z);		//キャラ最小値
					D3DXVECTOR3 sizeOldMaxC = D3DXVECTOR3(posOldC.x + sizeC.x, posOldC.y + (sizeC.y * 2), posOldC.z + sizeC.z);		//キャラ最大値

					D3DXVECTOR3 sizeMinC = D3DXVECTOR3(posC.x - sizeC.x, posC.y, posC.z - sizeC.z);		//キャラ最小値
					D3DXVECTOR3 sizeMaxC = D3DXVECTOR3(posC.x + sizeC.x, posC.y + (sizeC.y * 2), posC.z + sizeC.z);		//キャラ最大値

					if (pObject->GetJump() == false)
					{
						if (sizeOldMinC.y >= pos.y + sizeMax.y
							&& sizeMinC.y <= pos.y + sizeMax.y
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f)
						{//ブロック上
							D3DXVECTOR3 Objmove = pObject->GetMove();
							pObject->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

							move.y = (pos.y + sizeMax.y) - (sizeMinC.y) + 0.1f;
							pObject->SetJump(false);
							pObject->SetBoost(false);
							bCollision = true;
							bLand = true;
						}
						else if (sizeOldMaxC.y <= pos.y + sizeMin.y
							&& sizeMaxC.y >= pos.y + sizeMin.y
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f)
						{//ブロック下
							D3DXVECTOR3 Objmove = pObject->GetMove();
							pObject->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

							move.y = (pos.y + sizeMin.y) - (sizeMaxC.y) - 0.1f;
							bCollision = true;
						}
					}
					else
					{
						if (sizeOldMinC.y >= pos.y + sizeMax.y
							&& sizeMinC.y <= pos.y + sizeMax.y
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f)
						{//ブロック上
							D3DXVECTOR3 Objmove = pObject->GetMove();
							pObject->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

							move.y = (pos.y + sizeMax.y) - (sizeMinC.y) + 0.1f;
							pObject->SetJump(false);
							pObject->SetBoost(false);
							bCollision = true;
							bLand = true;
						}
						else if (sizeOldMaxC.y <= pos.y + sizeMin.y
							&& sizeMaxC.y >= pos.y + sizeMin.y
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& sizeMaxC.z > pos.z + sizeMin.z + 0.1f
							&& sizeMinC.z < pos.z + sizeMax.z + 0.1f)
						{//ブロック下
							D3DXVECTOR3 Objmove = pObject->GetMove();
							pObject->SetMove(D3DXVECTOR3(Objmove.x, 0.0f, Objmove.z));

							move.y = (pos.y + sizeMin.y) - (sizeMaxC.y) - 0.1f;
							bCollision = true;
						}
					}
				}

				if (bCollision)
				{
					pObject->SetPos(posC + move);
					bCollision = false;
					break;
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	//z
	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

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

					D3DXVECTOR3 sizeOldMinC = D3DXVECTOR3(posOldC.x - sizeC.x, posOldC.y, posOldC.z - sizeC.z);		//キャラ最小値
					D3DXVECTOR3 sizeOldMaxC = D3DXVECTOR3(posOldC.x + sizeC.x, posOldC.y + (sizeC.y * 2), posOldC.z + sizeC.z);		//キャラ最大値

					D3DXVECTOR3 sizeMinC = D3DXVECTOR3(posC.x - sizeC.x, posC.y, posC.z - sizeC.z);		//キャラ最小値
					D3DXVECTOR3 sizeMaxC = D3DXVECTOR3(posC.x + sizeC.x, posC.y + (sizeC.y * 2), posC.z + sizeC.z);		//キャラ最大値

					if (pObject->GetJump() == false)
					{
						if (sizeOldMaxC.z <= pos.z + sizeMin.z
							&& sizeMaxC.z > pos.z + sizeMin.z
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//ブロック北
							move.z = (pos.z + sizeMin.z) - (sizeMaxC.z) - 0.1f;
							bCollision = true;
						}
						else if (sizeOldMinC.z >= pos.z + sizeMax.z
							&& sizeMinC.z <= pos.z + sizeMax.z
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//ブロック南
							move.z = (pos.z + sizeMax.z) - (sizeMinC.z) + 0.1f;
							bCollision = true;
						}
					}
					else
					{
						if (sizeOldMaxC.z <= pos.z + sizeMin.z
							&& sizeMaxC.z > pos.z + sizeMin.z
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//ブロック北
							move.z = (pos.z + sizeMin.z) - (sizeMaxC.z) - 0.1f;
							bCollision = true;
						}
						else if (sizeOldMinC.z >= pos.z + sizeMax.z
							&& sizeMinC.z <= pos.z + sizeMax.z
							&& sizeMaxC.x > pos.x + sizeMin.x + 0.1f
							&& sizeMinC.x < pos.x + sizeMax.x + 0.1f
							&& ((sizeMaxC.y >= pos.y + sizeMin.y + 0.1f
								&& sizeMaxC.y <= pos.y + sizeMax.y - 0.1f)
								|| (sizeMinC.y >= pos.y + sizeMin.y + 0.1f
									&& sizeMinC.y <= pos.y + sizeMax.y - 0.1f)))
						{//ブロック南
							move.z = (pos.z + sizeMax.z) - (sizeMinC.z) + 0.1f;
							bCollision = true;
						}
					}
				}

				if (bCollision)
				{
					pObject->SetPos(posC + move);
					bCollision = false;
					break;
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	return bLand;
}
