//===============================================
//
// マップ(map.cpp)
// Author:Kai Takada
//
//===============================================

//============================
// インクルードファイル
//============================
#include "map.h"
#include "manager.h"	//大元
#include "game.h"
#include "input.h"
#include "block.h"
#include "debugproc.h"

//=======================
//マクロ定義
//=======================

//=======================
// ファイルパス
//=======================
const char *c_apFilenameMap[CMap::MAP_MAX] =
{
	"data\\SET\\MAP\\load.txt",
	"data\\SET\\MAP\\save.txt",
};

//=======================
// 静的メンバ変数
//=======================

//============================
// コンストラクタ
//============================
CMap::CMap()
{
	m_pBlock = nullptr;
}

//============================
// デストラクタ
//============================
CMap::~CMap()
{

}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CMap::Init()
{
	if (m_pBlock != nullptr)
	{
		m_pBlock->Uninit();
		m_pBlock = nullptr;
	}

	m_pBlock->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pBlock->SetDeath(true);

	return S_OK;
}

//====================================
// 初期化(位置オバロ)
//====================================
HRESULT CMap::Init(const D3DXVECTOR3 pos)
{
	if (m_pBlock != nullptr)
	{
		m_pBlock->Uninit();
		m_pBlock = nullptr;
	}

	m_pBlock = CBlock::Create(pos);
	m_pBlock->SetDeath(true);
	//m_pBlock->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	return S_OK;
}

//============================
// 終了
//============================
void CMap::Uninit(void)
{
	if (m_pBlock != nullptr)
	{
		m_pBlock->SetDeath(true);
		m_pBlock = nullptr;
	}
}

//============================
// 更新
//============================
void CMap::Update(void)
{
	//キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	
	if (m_pBlock == nullptr)
	{
		return;
	}
	
	D3DXVECTOR3 pos = m_pBlock->GetPos();
	D3DXVECTOR3 size = m_pBlock->GetSize();

	if (pInputKeyboard->GetTrigger(DIK_B))
	{
		//壊して
		int nType = m_pBlock->GetType_Blk();
		m_pBlock->SetDeath(true);

		//1個ずらし
		nType++;
		nType %= CBlock::TYPE_MAX;

		//作り直す
		m_pBlock = CBlock::Create(pos,DEF_VEC3,(CBlock::TYPE_Blk)nType);
		m_pBlock->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		m_pBlock->SetDeath(true);
	}

	//ブロックサイズ変更
	if (pInputKeyboard->GetTrigger(DIK_UP))
	{
		size.y += 1.0f;
		m_pBlock->SetSize(size);
	}
	if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		size.y -= 1.0f;
		m_pBlock->SetSize(size);
	}
	if (pInputKeyboard->GetTrigger(DIK_RIGHT))
	{
		size.x += 1.0f;
		m_pBlock->SetSize(size);
	}
	if (pInputKeyboard->GetTrigger(DIK_LEFT))
	{
		size.x -= 1.0f;
		m_pBlock->SetSize(size);
	}


	//カーソルの移動
	if (pInputKeyboard->GetPress(DIK_SPACE))
	{
		if (pInputKeyboard->GetPress(DIK_W))
		{//上
			pos.y += 10.0f;
		}
		if (pInputKeyboard->GetPress(DIK_S))
		{//下
			pos.y -= 10.0f;
		}
		if (pInputKeyboard->GetPress(DIK_A))
		{//西
			pos.x -= 10.0f;
		}
		if (pInputKeyboard->GetPress(DIK_D))
		{//東
			pos.x += 10.0f;
		}
	}
	else
	{
		if (pInputKeyboard->GetPress(DIK_W))
		{//上
			pos.y += 10.0f;
		}
		if (pInputKeyboard->GetPress(DIK_S))
		{//下
			pos.y -= 10.0f;
		}
		if (pInputKeyboard->GetPress(DIK_A))
		{//西
			pos.x -= 10.0f;
		}
		if (pInputKeyboard->GetPress(DIK_D))
		{//東
			pos.x += 10.0f;
		}
	}

	m_pBlock->SetPos(pos);


	if (pInputKeyboard->GetTrigger(DIK_RETURN))
	{//ブロック設置
		CBlock *pBlock = CBlock::Create(pos, DEF_VEC3, m_pBlock->GetType_Blk());
		pBlock->SetSize(m_pBlock->GetSize());
	}

	if (pInputKeyboard->GetTrigger(DIK_F8))
	{//ブロック設置
		Load(c_apFilenameMap[MAP_LOAD]);
	}
	if (pInputKeyboard->GetTrigger(DIK_F9))
	{//ブロック設置
		Save(c_apFilenameMap[MAP_SAVE]);
	}
}

//============================
// 描画
//============================
void CMap::Draw(void)
{
	D3DXVECTOR3 pos = m_pBlock->GetPos();
	D3DXVECTOR3 size = m_pBlock->GetSize();

	//デバッグ
	CDebugProc *pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- エディット情報 ---\n");
	pDebug->Print("現在の座標:%f %f %f\n", pos.x, pos.y, pos.z);
	pDebug->Print("現在のサイズ:%f %f %f\n", size.x, size.y, size.z);
	pDebug->Print("セーブ：F9 ・ ロード：F8\n");
	pDebug->Print("設置：Enter ・ タイプ変更：B\n");
	pDebug->Print("----------------------\n");
}

//============================
// ポーズの生成
//============================
CMap * CMap::Create(D3DXVECTOR3 pos)
{
	CMap *pMap = nullptr;

	//オブジェクト生成&初期化
	pMap = new CMap;

	if (pMap == nullptr)
	{
		return nullptr;
	}
	
	if (FAILED(pMap->Init(pos)))
	{//失敗したら
		return nullptr;
	}

	return pMap;
}

//============================
// ロード
//============================
void CMap::Load(const char * pFilepass)
{
	if (pFilepass == nullptr)
	{
		return;
	}

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nType = CBlock::TYPE_NORMAL;
	CBlock *pBlock = nullptr;		//生成内容

	char *pComp = new char[256];
	FILE *pFile = nullptr;

	pFile = fopen(pFilepass, "r");

	if (pFile == nullptr)
	{
		assert(false);
	}

	while (fscanf(pFile, "%s", pComp) != EOF)
	{//EOF(ラスト文)を読み込んだら終了

		if (strncmp(pComp, "#", 1) == 0)
		{// これのあとコメント
			fgets(pComp, 256, pFile);
			continue;
		}
		else if (strcmp(pComp, "SET_PARAM") == 0)
		{//パラメータ設定
			while (true)
			{
				fscanf(pFile, "%s", pComp);

				if (strncmp(pComp, "#", 1) == 0)
				{// これのあとコメント
					fgets(pComp, 256, pFile);
					continue;
				}

				if (strcmp(pComp, "END_PARAM") == 0)
				{
					break;
				}
				else if (strcmp(pComp, "POS") == 0)
				{
					fscanf(pFile, "%s", pComp);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
					continue;
				}
				else if (strcmp(pComp, "ROT") == 0)
				{
					fscanf(pFile, "%s", pComp);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					continue;
				}
				else if (strcmp(pComp, "SIZE") == 0)
				{
					fscanf(pFile, "%s", pComp);
					fscanf(pFile, "%f %f %f", &size.x, &size.y, &size.z);
					continue;
				}
				else if (strcmp(pComp, "TYPE") == 0)
				{
					fscanf(pFile, "%s", pComp);
					fscanf(pFile, "%d", &nType);
					continue;
				}
			}

			//読み込んだパラメータを元に生成
			pBlock = CBlock::Create(pos, rot,(CBlock::TYPE_Blk)nType);
			pBlock->SetSize(size);
		}
	}

	fclose(pFile);

	delete[] pComp;
}

//============================
// セーブ
//============================
void CMap::Save(const char * pFilepass)
{
	if (pFilepass == nullptr)
	{//ファイルが無い
		return;
	}

	//文の短縮化用
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nType = CBlock::TYPE_NORMAL;

	FILE *pFile = nullptr;

	pFile = fopen(pFilepass, "w");

	if (pFile == nullptr)
	{//ファイルが無い
		assert(false);
	}

	fprintf(pFile, "#===============================================================\n");
	fprintf(pFile, "#\n");
	fprintf(pFile, "# Map(Block情報)\n");
	fprintf(pFile, "# Author : Kai Takada\n");
	fprintf(pFile, "#\n");
	fprintf(pFile, "#===============================================================\n");

	for (int nCntPrt = 0; nCntPrt < CObject::PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ
				bool bDeath = pObject->GetDeath();

				if (type == CObject::TYPE_BLOCK && !bDeath)
				{//死んでないブロックだったら
					pos = pObject->GetPos();
					rot = pObject->GetRot();
					size = pObject->GetSize();

					nType = pObject->GetLocalType();

					fprintf(pFile, "SET_PARAM\n");
					fprintf(pFile, "POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
					fprintf(pFile, "ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);
					fprintf(pFile, "SIZE = %.2f %.2f %.2f\n", size.x, size.y, size.z);
					fprintf(pFile, "TYPE = %d\n", nType);
					fprintf(pFile, "END_PARAM\n\n");
				}

				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	fclose(pFile);
}

//============================
// カーソル情報
//============================
CBlock *CMap::GetCursor()
{
	return m_pBlock;
}
