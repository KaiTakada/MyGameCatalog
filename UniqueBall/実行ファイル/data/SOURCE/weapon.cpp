//==================================================
//
// 武器処理(weapon.cpp) 継承 : Parts
// Author : Kai Takada
//
//==================================================
#include "weapon.h"
#include "Xmodel.h"
#include "manager.h"
#include "sound.h"

//=======================
// マクロ定義
//=======================
#define MUZZLE_FILE "data\\MODEL\\WEAPON\\muzzle.txt"		//銃口位置情報
#define READ_SIZE (128)		//読込ポインタサイズ

//=======================
// バイナリファイル名
//=======================
const char *c_apFileWeapon[CWeapon::TYPE_MAX] =
{
	"data\\MODEL\\WEAPON\\nife.x",			//ナイフ
	"data\\MODEL\\WEAPON\\revolver.x",		//リボルバー
	"data\\MODEL\\WEAPON\\ak.x",			//ライフル
	"data\\MODEL\\WEAPON\\boost.x",			//ブースト
	"data\\MODEL\\WEAPON\\ball.x",			//ボール
};

//=======================
// 静的メンバ変数宣言
//=======================

//=======================
// コンストラクタ
//=======================
CWeapon::CWeapon(int nPriority) : CParts(nPriority)
{
	m_type = WPNTYPE_REVOLVER;
	m_muzzlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCooltime = 0;
	m_nCooltimeMin = 0;
}

//=======================
// デストラクタ
//=======================
CWeapon::~CWeapon()
{

}

//=======================
// 初期化処理
//=======================
HRESULT CWeapon::Init(void)
{
	m_nCooltimeMin = 0;
	m_type = WPNTYPE_NIFE;

	CParts::Init();
	
	SetType(TYPE_WEAPON);

	return S_OK;
}

//=======================
// 初期化処理
//=======================
HRESULT CWeapon::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const WPNTYPE type)
{
	m_nCooltimeMin = 0;
	m_type = type;

	CParts::Init(pos, rot);

	CXModel *pXmodel = CManager::GetInstance()->GetXModel();
	SetIdxModel(pXmodel->Regist(c_apFileWeapon[type]));		//モデルの割り当て

	SetType(TYPE_WEAPON);

	return S_OK;
}

//=======================
// 終了処理
//=======================
void CWeapon::Uninit(void)
{
	CParts::Uninit();
}

//=======================
// 更新処理
//=======================
void CWeapon::Update(void)
{
	if (m_nCooltime > 0)
	{
		m_nCooltime--;
	}

	CParts::Update();
}

//=======================
// 描画処理
//=======================
void CWeapon::Draw(void)
{
	CParts::Draw();
}

//=======================
// 攻撃処理
//=======================
void CWeapon::Attack(const D3DXVECTOR3 rot, const int nDamage)
{
	D3DXMATRIX mtxworld = GetMtxWorld();
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 mtxrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pos.x = mtxworld._41;
	pos.y = mtxworld._42;
	pos.z = mtxworld._43;

	mtxrot.x = mtxworld._11 * mtxworld._21 * mtxworld._31;
	mtxrot.y = mtxworld._12 * mtxworld._22 * mtxworld._32;
	mtxrot.z = mtxworld._13 * mtxworld._23 * mtxworld._33;

	D3DXVECTOR3 muzzlePos;
	muzzlePos.x = mtxrot.x * m_muzzlePos.x;
	muzzlePos.y = mtxrot.y * m_muzzlePos.y;
	muzzlePos.z = mtxrot.z * m_muzzlePos.z;

	//武器種に応じた銃口の位置をfileから取得し、そこから発射する
	if (m_nCooltime > 0)
	{
		return;
	}

	m_nCooltime = 5 - m_nCooltimeMin;
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ONFIRE);

	if (m_nCooltime < 0)
	{//0未満にはならない
		m_nCooltime = 0;
	}
}

//=======================
// 生成処理
//=======================
CWeapon *CWeapon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, WPNTYPE type)
{
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	CWeapon *pWeapon = nullptr;

	//オブジェクト生成&初期化
	pWeapon = new CWeapon;

	if (pWeapon != nullptr)
	{
		if (FAILED(pWeapon->Init(pos, rot, type)))
		{
			return nullptr;
		}
		else
		{
			pWeapon->ReadMuzzleFile(type);

			CXModel *pXmodel = CManager::GetInstance()->GetXModel();
			pWeapon->SetIdxModel(pXmodel->Regist(c_apFileWeapon[type], &vtxMin, &vtxMax));		//モデルの割り当て
		}
	}
	else
	{
		return nullptr;
	}

	return pWeapon;
}

//=======================
// タイプ設定
//=======================
void CWeapon::SetWpnType(CWeapon::WPNTYPE type)
{
	CXModel *pXmodel = CManager::GetInstance()->GetXModel();
	SetIdxModel(pXmodel->Regist(c_apFileWeapon[type]));		//モデルの割り当て
	m_type = type;

	ReadMuzzleFile(type);
}

//============================
// ファイル読み込み
//============================
void CWeapon::ReadMuzzleFile(WPNTYPE type)
{
	char *pComp = new char[128];				//読み込む箱
	char *pNum = new char[2];

	//今回のタイプの数値を文字に変換
	snprintf(pNum, 2, "%d", type);

	FILE *pFile;

	pFile = fopen(MUZZLE_FILE, "r");

	if (pFile != nullptr)
	{//ファイル読込に成功した場合
		do
		{
			fscanf(pFile, "%s", pComp);

			if (strncmp(pComp, "#", 1) == 0)
			{// これのあとコメント
				fgets(pComp, READ_SIZE, pFile);
				continue;
			}
			else if (strncmp(pComp, pNum, 2) == 0)
			{
				fscanf(pFile, "%f %f %f", &m_muzzlePos.x, &m_muzzlePos.y, &m_muzzlePos.z);

				break;
			}

		} while (strcmp(pComp, "END_SCRIPT") != 0);

		fclose(pFile);
	}

	delete[] pComp;
	delete[] pNum;
}


//============================
//クールタイムファイル読込
//============================
void CWeapon::ReadCooltime(WPNTYPE type)
{
	char *pComp = new char[128];				//読み込む箱
	char *pNum = new char[2];

	//今回のタイプの数値を文字に変換
	snprintf(pNum, 2, "%d", type);

	FILE *pFile;

	pFile = fopen(MUZZLE_FILE, "r");

	if (pFile != nullptr)
	{//ファイル読込に成功した場合
		do
		{
			fscanf(pFile, "%s", pComp);

			if (strncmp(pComp, "#", 1) == 0)
			{// これのあとコメント
				fgets(pComp, READ_SIZE, pFile);
				continue;
			}
			else if (strncmp(pComp, pNum, 2) == 0)
			{
				fscanf(pFile, "%d", &m_nCooltime);

				break;
			}

		} while (strcmp(pComp, "END_SCRIPT") != 0);

		fclose(pFile);
	}

	delete[] pComp;
	delete[] pNum;
}
