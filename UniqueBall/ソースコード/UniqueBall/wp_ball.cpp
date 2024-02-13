//==================================================
//
// ブースター(wp_ball.cpp) 継承：CWeapon
// Author : Kai Takada
//
//==================================================
#include "wp_ball.h"
#include "weapon.h"
#include "Xmodel.h"
#include "manager.h"
#include "bullet.h"
#include "Field.h"

#include "sound.h"
#include "particle.h"

//=======================
// 定数定義
//=======================
namespace
{
	const float GRAVITY = 0.5f;		//重力
	const float NUM_RESIST(0.02f);		//移動抵抗
	const float STOP_SPEED = 0.4f;		//停止する低限速度
	const float COLLISION_RAD = 50.0f;		//当たり半径
	const int DEATHCTR = 300;		//消滅猶予時間
}

//=======================
// 静的メンバ変数宣言
//=======================
int CBall::s_nNumAll = 0;

//=======================
// コンストラクタ
//=======================
CBall::CBall(int nPriority) : CWeapon(nPriority)
{
	m_fRad = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SetMultiply(true);		//掛け合わせフラグ設定
	m_eMember = my_Identity::MEMBER_NONE;
	m_bLand = false;
	m_nDeathCtr = 0;

	s_nNumAll++;
}

//=======================
// デストラクタ
//=======================
CBall::~CBall()
{
	s_nNumAll--;
}

//=======================
// 初期化処理
//=======================
HRESULT CBall::Init(void)
{
	m_fRad = COLLISION_RAD;
	m_nDeathCtr = DEATHCTR;
	CWeapon::Init();
	
	SetType(TYPE_BALL);

	return S_OK;
}

//=======================
// 初期化処理
//=======================
HRESULT CBall::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_fRad = COLLISION_RAD;
	m_nDeathCtr = DEATHCTR;
	CWeapon::Init(pos, rot, CWeapon::WPNTYPE_BALL);

	SetType(TYPE_BALL);

	return S_OK;
}

//=======================
// 終了処理
//=======================
void CBall::Uninit(void)
{
	CWeapon::Uninit();
}

//=======================
// 更新処理
//=======================
void CBall::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	m_posOld = pos;

	CWeapon::Update();

	CParts *pParts = GetParent();

	if (pParts != nullptr)
	{//親がある＝所持＝位置固定
		return;
	}

	//@移動-------------

	//重力
	m_move.y -= GRAVITY;

	//高さを取得する
	CollisionField(pos);

	//移動量を適用
	SetPos(pos += m_move); 

	//移動量を更新(減衰させる)慣性
	m_move.x += (0 - m_move.x) * NUM_RESIST;
	m_move.z += (0 - m_move.z) * NUM_RESIST;

	if (m_move.x <= STOP_SPEED && m_move.x >= -STOP_SPEED)
	{//規定値以下で移動量を0に
		m_move.x = 0.0f;
	}
	if (m_move.z <= STOP_SPEED && m_move.z >= -STOP_SPEED)
	{//規定値以下で移動量を0に
		m_move.z = 0.0f;
	}

	//消滅カウントダウン
	if (m_eMember == my_Identity::MEMBER_NONE)
	{
		if (m_nDeathCtr <= 0)
		{
			Uninit();
			return;
		}

		m_nDeathCtr--;
	}
}

//=======================
// 描画処理
//=======================
void CBall::Draw(void)
{
	CWeapon::Draw();
}

//=======================
// 攻撃処理
//=======================
void CBall::Attack(const D3DXVECTOR3 rot, const int nDamage)
{
	//投げる方向を設定
	SetRot(rot);
}

//=======================
// 攻撃処理
//=======================
void CBall::Throw(const D3DXVECTOR3 vec, float fPower)
{
	//m_move.x += sinf(cameraRot.y + -0.25f * D3DX_PI) * m_param.fSpeed;		//x

	//投げる方向を設定
	m_move.x = sinf(vec.y + 1.0f * D3DX_PI) * fPower;
	m_move.z = cosf(vec.y + 1.0f * D3DX_PI) * fPower;
	m_move.y = sinf(vec.x + 1.0f * D3DX_PI) * fPower;

}

//=======================
//@brief プレイヤーとの当たり判定
// 引数：
// (pos)判定対象の座標
// (fRad)判定対象の半径
// 返り値：当たったかどうか(true:当たった,false:当たってない)
//=======================
bool CBall::CollisionCircle(const D3DXVECTOR3 pos, const float fRad)
{
	bool bCol = false;
	D3DXVECTOR3 myPos = GetPos();

	//3軸使った球の判定
	float fColl = m_fRad + fRad;		//当たり判定範囲

	float fLength0 = hypotf((myPos.x - pos.x), (myPos.z - pos.z));		//2点間の長さxz
	float fLength1 = hypotf((myPos.x - pos.x), (myPos.y - pos.y));		//2点間の長さxy
	float fLength2 = hypotf((myPos.z - pos.z), (myPos.y - pos.y));		//2点間の長さzy

	if (fLength0 <= fColl &&
		fLength1 <= fColl &&
		fLength2 <= fColl)
	{//当たっていたら
		bCol = true;
	}

	return bCol;
}

//=======================
//@brief 壁との当たり判定
// 引数：
// (pos)判定対象の座標
// (pVtx)判定対象の頂点ポインタ
// 返り値：当たったかどうか(true:当たった,false:当たってない)
//=======================
bool CBall::CollisionWall(const D3DXVECTOR3 pos, const D3DXVECTOR3* pVtx)
{
	//posと各Vtxを掛け合わせて、そこと当たっているか

	return false;
}

//=======================
// 所属設定
//=======================
void CBall::SetMember(const my_Identity::eMember member)
{
	if (m_eMember == my_Identity::MEMBER_NONE &&
		member != my_Identity::MEMBER_NONE)
	{
		m_nDeathCtr = DEATHCTR;
	}

	m_eMember = member;
}

//=======================
// 生成処理
//=======================
CBall *CBall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	CBall *pWeapon = nullptr;

	//オブジェクト生成&初期化
	pWeapon = new CBall;

	if (pWeapon != nullptr)
	{
		if (FAILED(pWeapon->Init(pos, rot)))
		{
			return nullptr;
		}
		else
		{
			//モデルの割り当ては親で一括で行う
		}
	}
	else
	{
		return nullptr;
	}

	return pWeapon;
}

//============================
//ブロックとの当たり判定(矩形)
//============================
void CBall::CollisionField(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;
	pos.y -= m_fRad;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_FIELD)
				{//アイテムだったら

					CField *pField = (CField*)pObject;
					fHeight = pField->GetHeight(pos);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	if (pos.y < fHeight && m_posOld.y >= fHeight)
	{//着地時

		pos.y = fHeight;
		m_move.y *= -1.0f;
		m_eMember = my_Identity::MEMBER_NONE;
	}

	//座標設定(更新)
	SetPos(pos);
}
