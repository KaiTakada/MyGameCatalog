//===============================================
//
// スポーン(spawn.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "spawn.h"
#include "manager.h"
//#include "enemy.h"
#include "player.h"
#include "debugproc.h"
#include "input.h"
#include "game.h"

//============================
// マクロ定義
//============================
#define SPAWN_FILE_OP "data\\SET\\SPAWN\\data.txt"			//敵スポーン頻度

#if _DEBUG
#define DIF_SPAWN (10)			//敵スポーン頻度
#define DIF_SPAWN_SPEED (180)			//速度敵スポーン頻度
#define DEF_SPAWN_RAD (500.0f)			//スポーン半径
#define ENE_GROW_COUNT (6000)			//敵強化フェーズカウント
#else
#define DIF_SPAWN (50)			//スポーン頻度
#define DIF_SPAWN_SPEED (180)			//速度敵スポーン頻度
#define DEF_SPAWN_RAD (1000.0f)			//スポーン半径
#define ENE_GROW_COUNT (1200)			//敵強化フェーズカウント
#endif

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ(デフォ)
//============================
CSpawn::CSpawn()
{
	m_fRad = 0.0f;
	m_fLegScr = 0.0f;
	m_nEneGrowCtr = 0;
	m_nCtr = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bDebSpawn = false;

	//0で埋める
	for (int nCnt = 0; nCnt < sizeof(m_aGenerate) / sizeof(Generate); nCnt++)
	{
		m_aGenerate[nCnt].nFrequency = 0;
		m_aGenerate[nCnt].nNumMax = 0;
		m_aGenerate[nCnt].nNumMin = 0;
	}
	//ZeroMemory(m_aGenerate, sizeof(m_aGenerate));
}

//============================
// デストラクタ
//============================
CSpawn::~CSpawn()
{

}
//====================================
// 初期化処理(デフォ)
//====================================
HRESULT CSpawn::Init(void)
{
	m_fRad = DEF_SPAWN_RAD;
	m_nCtr = 1;
	m_nEneGrowCtr = 1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//FileRead();

	for (int nCnt = 0; nCnt < sizeof(m_aGenerate) / sizeof(Generate); nCnt++)
	{
		m_aGenerate[nCnt].nFrequency = 1;
		m_aGenerate[nCnt].nNumMax = 0;
		m_aGenerate[nCnt].nNumMin = 0;
	}

	SetType(TYPE_SPAWN);

	return S_OK;
}

//====================================
// 初期化処理(デフォ)
//====================================
HRESULT CSpawn::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_nCtr = 1;
	m_nEneGrowCtr = 1;
	m_pos = pos;
	m_size = size;

	//対角線の長さを算出する
	m_fLegScr = sqrtf((SCREEN_WIDTH * SCREEN_WIDTH) + (SCREEN_HEIGHT * SCREEN_HEIGHT)) * 0.5f;
	m_fRad = DEF_SPAWN_RAD;

	for (int nCnt = 0; nCnt < sizeof(m_aGenerate) / sizeof(Generate); nCnt++)
	{
		m_aGenerate[nCnt].nFrequency = 1;
		m_aGenerate[nCnt].nNumMax = 0;
		m_aGenerate[nCnt].nNumMin = 0;
	}

	//FileRead();

	SetType(TYPE_SPAWN);

	return S_OK;
}

//============================
// 終了処理
//============================
void CSpawn::Uninit(void)
{
	Release();
}

//============================
// 更新処理
//============================
void CSpawn::Update(void)
{
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();

	if (pInputKey->GetTrigger(DIK_O))
	{
		m_bDebSpawn = m_bDebSpawn ? false : true;
	}

	if (m_bDebSpawn == false)
	{
		GenerateEnemy();
	}

	m_nCtr++;

	if ((m_nCtr % ENE_GROW_COUNT) == 0)
	{
		m_nEneGrowCtr++;
	}
}

//============================
// 描画処理
//============================
void CSpawn::Draw(void)
{
	//周期で生成するだけなので描画内容は無し
}

//==========================
// 生成処
//==========================
CSpawn *CSpawn::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CSpawn *pSpawn = nullptr;

	pSpawn = new CSpawn;

	if (pSpawn != nullptr)
	{
		if (SUCCEEDED(pSpawn->Init(pos, size)))
		{
			pSpawn->FileRead();
		}
	}
	else
	{
		return nullptr;
	}

	return pSpawn;
}

//==========================
// 敵生成
//==========================
void CSpawn::GenerateEnemy()
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 posR = pos;

	if (CGame::GetPlayer() != nullptr)
	{
		pos = CGame::GetPlayer()->GetPos();
	}

	float fRand, fRand2;
	
	//nCnt = ENEMYTYPE
	for (int nCnt = 0; nCnt < CEnemy::ET_MAX; nCnt++)
	{
		if ((m_nCtr % m_aGenerate[nCnt].nFrequency) == 0)
		{
			if (m_aGenerate[nCnt].nNumMax == 0 && m_aGenerate[nCnt].nNumMin == 0)
			{
				continue;
			}

			fRand2 = (float)(rand() % 629 - 314) / 100.0f;
			fRand = sinf(fRand2);
			fRand2 = cosf(fRand2);

			posR.x = fRand * m_fRad + pos.x;
			posR.z = fRand2 * m_fRad + pos.z;

			int nNum = m_aGenerate[nCnt].nNumMin;

			if (m_aGenerate[nCnt].nNumMax != 0)
			{
				nNum += rand() % m_aGenerate[nCnt].nNumMax;
			}

			for (int nCntCrt = 0; nCntCrt < nNum; nCntCrt++)
			{
				CEnemy *pEnemy = CEnemy::Create(posR, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::TYPE(nCnt));

				if (pEnemy != nullptr)
				{
					CEnemy::Param param = pEnemy->GetParam();
					param.fAttack *= m_nEneGrowCtr;
					param.fLife *= m_nEneGrowCtr;
					param.fLifeMax *= m_nEneGrowCtr;
				
					pEnemy->SetParam(param);
				}

				m_nEneGrowCtr;
				//ランダムで配置位置ずらし
				if (rand() % 2 == 0)
				{
					posR.x += 50.0f;
				}
				else
				{
					posR.z += 50.0f;
				}
			}
		}
	}
}

//==========================
// 敵生成情報ファイル読込
//==========================
void CSpawn::FileRead()
{
	char *pComp = new char[256];
	FILE *pFile;

	pFile = fopen(SPAWN_FILE_OP, "r");

	if (pFile != nullptr)
	{
		int nCnt = 0;

		while (true)
		{
			fscanf(pFile, "%s", pComp);

			if (nCnt >= CEnemy::ET_MAX)
			{//配列オーバー
				break;
			}

			if (strncmp(pComp, "#", 1) == 0)
			{// これのあとコメント
				fgets(pComp, 256, pFile);
				continue;
			}
			else if (strcmp(pComp, "SET_SPAWN") == 0)
			{
				while (true)
				{
					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "END_SPAWN") == 0)
					{
						break;
					}
					else if (strcmp(pComp, "FQC") == 0)
					{
						fscanf(pFile, "%s", pComp);
						fscanf(pFile, "%d", &m_aGenerate[nCnt].nFrequency);
						continue;
					}
					else if (strcmp(pComp, "GROUP_MIN") == 0)
					{
						fscanf(pFile, "%s", pComp);
						fscanf(pFile, "%d", &m_aGenerate[nCnt].nNumMin);
						continue;
					}
					else if (strcmp(pComp, "GROUP_MAX") == 0)
					{
						fscanf(pFile, "%s", pComp);
						fscanf(pFile, "%d", &m_aGenerate[nCnt].nNumMax);
						continue;
					}
				}
			}
		
			nCnt++;
		}
	
		fclose(pFile);
	}

	delete[] pComp;
}
