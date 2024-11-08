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
#include "effect.h"

//=======================
// 定数定義
//=======================
namespace
{
	const char *FILEPASS = "data\\MODEL\\BLOCK\\ball_generator.x";
	const int GENERATE_COUNT = 300;
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
		if (m_nGenerateCtr >= GENERATE_COUNT)
		{
			//生成
			CBall *pBall = CBall::Create(GetPos());
			pBall->SetMember(my_Identity::MEMBER_MAP);
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
		if (m_pBall->GetMember() != my_Identity::MEMBER_MAP)
		{//他の所属になったら手放す
			m_pBall = nullptr;
		}
		else
		{//
			D3DXVECTOR3 pos = GetPos();
			pos.y += m_pBall->GetMaxVtx().y;
			m_pBall->SetPos(pos);
			m_pBall->SetDeathCtr(100);

			//ある
			CEffect* pEffect = CEffect::Create(pos);
			if (pEffect == nullptr) return;
			pEffect->SetZtest(false);
			pEffect->SetSize(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
			pEffect->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f));
			pEffect->SetLight(true);
		}
	}

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
