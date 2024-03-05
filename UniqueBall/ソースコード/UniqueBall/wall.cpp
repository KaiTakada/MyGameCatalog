//============================
//
// 地面処理(Field.cpp)
// Author: Kai Takada
//
//============================
//==================================
// インクルードファイル
//==================================
#include "wall.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"

//==================================
// マクロ定義
//==================================
namespace
{
	const char* TEX_FIELD = "data\\TEXTURE\\BG\\wall.jpg";
	const int DEF_NUM_DIF = 8;
}

//==================================
// 静的メンバ変数宣言
//==================================

//==================================
// コンストラクタ
//==================================
CWall::CWall(int nPriority) : CObject3D(nPriority)
{

}

//==================================
// デストラクタ
//==================================
CWall::~CWall()
{

}

//==================================
// 初期化処理
//==================================
HRESULT CWall::Init(void)
{
	CObject3D::Init();

	SetPos(DEF_VERTEX3D);	//初期化する
	SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));	//初期化する
	SetSize(D3DXVECTOR3(DEF_SIZE.x, DEF_SIZE.y, 0.0f));

	SetType(TYPE_WALL);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CWall::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CObject3D::Init(pos,rot);

	//サイズ設定
	SetSize(D3DXVECTOR3(DEF_SIZE.x, DEF_SIZE.z, 0.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));	//初期化する

	SetType(TYPE_WALL);

	return S_OK;
}

//==================================
// 終了処理
//==================================
void CWall::Uninit(void)
{
	CObject3D::Uninit();
}

//==================================
// 更新処理
//==================================
void CWall::Update(void)
{
	CObject3D::Update();
}

//==================================
// 描画処理
//==================================
void CWall::Draw(void)
{
	CObject3D::Draw();
}

//============================
// 生成処理
//============================
CWall * CWall::Create(void)
{
	CWall *pWall;

	//オブジェクト生成
	pWall = new CWall;

	//初期化
	pWall->Init();

	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	pWall->SetIdxTexture(pTexture->Regist(TEX_FIELD));

	return pWall;
}

//============================
// 生成処理
//============================
CWall * CWall::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	CWall *pWall = nullptr;

	//オブジェクト生成&初期化
	pWall = new CWall;

	if (pWall != nullptr)
	{
		pWall->Init(pos, rot);

		pWall->SetSize(size);

		int tex0 = int(size.x / 256.0f);
		int tex1 = int(size.z / 256.0f);
		pWall->SetDivTex(tex0, tex1);

		CTexture *pTexture = CManager::GetInstance()->GetTexture();
		pWall->SetIdxTexture(pTexture->Regist(TEX_FIELD));
	}
	else
	{
		return nullptr;
	}

	return pWall;
}

//====================================
// 乗っているオブジェクトの位置y取得
// 引数：
// [In](posObj)判定対象の座標
// [In](posOldObj)判定対象の過去座標
// [Out](posCross)交点
// 返り値：当たったかどうか(true:当たった,false:当たってない)
//====================================
bool CWall::CollisionChara(D3DXVECTOR3 posObj, D3DXVECTOR3 posOldObj, D3DXVECTOR3 *posCross)
{
	bool bCol = false;

	D3DXVECTOR3 posPoly0;		//pos0
	D3DXVECTOR3 posPoly1;		//pos1
	D3DXVECTOR3 posPoly2;		//pos2
	D3DXVECTOR3 posPoly3;		//pos3

	GetPolyPos(&posPoly0, &posPoly1, &posPoly2, &posPoly3);

	D3DXVECTOR3 vecToPos;		//pos0とpos1の差分（位置関係）
	D3DXVECTOR3 vecLine;		//壁の長さ
	D3DXVECTOR3 vecMove;		//弾のベクトル
	float fRate;		//交点の割合

	vecLine = posPoly1 - posPoly0;
	vecToPos = posObj - posPoly0;
	vecMove = posObj - posOldObj;

	fRate = (((vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z))	//A
		/ ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z)));	//B

	if ((vecLine.z * vecToPos.x)
		- (vecLine.x * vecToPos.z) <= 0.0f
		&& fRate >= 0.0f && fRate <= 1.0f)
	{//当たった
		if (posCross != nullptr)
		{
			*posCross = vecLine * fRate;	//交点
		}

		bCol = true;
	}

	return bCol;
}
