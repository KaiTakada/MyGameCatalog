//===============================================
//
// モーション(motion.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "motion.h"
#include "parts.h"

//============================
// マクロ定義
//============================
namespace
{
	const char* PLAYER_FILE("data\\MOTION\\player.txt");		//モーションファイルパス
}

//============================
// コンストラクタ
//============================
CMotion::CMotion()
{
	for (int nCntInfo = 0; nCntInfo < NUM_MOTION; nCntInfo++)
	{
		m_aInfo[nCntInfo].bLoop = false;
		m_aInfo[nCntInfo].nNumKey = 0;
		
		for (int nCntKeyInfo = 0; nCntKeyInfo < NUM_KEY; nCntKeyInfo++)
		{
			m_aInfo[nCntInfo].aKeyInfo[nCntKeyInfo].nFrame = 0;
		
			for (int nCntKey = 0; nCntKey < MAX_PARTS; nCntKey++)
			{
				m_aInfo[nCntInfo].aKeyInfo[nCntKeyInfo].aKey[nCntKey] = CMotion::KEY();
			}
		}
	}
	
	for (int nCntKey = 0; nCntKey < MAX_PARTS; nCntKey++)
	{
		m_aKeyOrg[nCntKey] = CMotion::KEY();
		m_aKeyOld[nCntKey] = CMotion::KEY();
	}

	m_nNumAll = 0;
	m_nType = 0;
	m_nTypeOld = m_nType;
	m_bLoop = false;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nKeyOld = 0;
	m_nCounter = 0;
	m_bFinish = true;		//終了したかどうか
	m_nTotalCtr = 0;

	m_ppParts = nullptr;	//モデルへのポインタ
	m_nNumModel = 0;	//モデルの総数
}

//============================
// デストラクタ
//============================
CMotion::~CMotion()
{
}

//============================
// 初期化
//============================
HRESULT CMotion::Init(void)
{
	m_nNumAll = 0;
	m_nType = 0;
	m_bLoop = false;
	m_nNumKey = 1;
	m_nKey = 0;
	m_nCounter = 0;
	m_bFinish = true;		//終了したかどうか
	m_nTotalCtr = 0;

	m_ppParts = nullptr;	//モデルへのポインタ
	m_nNumModel = 0;	//モデルの総数

	for (int nCntInfo = 0; nCntInfo < NUM_MOTION; nCntInfo++)
	{
		m_aInfo[nCntInfo].bLoop = false;
		m_aInfo[nCntInfo].nNumKey = 1;
		
		for (int nCntKeyInfo = 0; nCntKeyInfo < NUM_KEY; nCntKeyInfo++)
		{
			m_aInfo[nCntInfo].aKeyInfo[nCntKeyInfo].nFrame = 0;
		
			for (int nCntKey = 0; nCntKey < MAX_PARTS; nCntKey++)
			{
				m_aInfo[nCntInfo].aKeyInfo[nCntKeyInfo].aKey[nCntKey] = CMotion::KEY();
			}
		}
	}

	for (int nCntKey = 0; nCntKey < MAX_PARTS; nCntKey++)
	{
		m_aKeyOrg[nCntKey] = CMotion::KEY();
	}

	return S_OK;
}

//============================
// 終了
//============================
void CMotion::Uninit(void)
{
	if (m_ppParts != nullptr)
	{
		delete[] m_ppParts;
		m_ppParts = nullptr;
	}
}

//============================
// モーション起動
//============================
void CMotion::Set(int nType)
{
	//各値の初期設定
	m_nTypeOld = m_nType;
	m_nType = nType;
	m_nKeyOld = m_nKey;
	m_nKey = 0;

	m_bLoop = m_aInfo[nType].bLoop;
	m_nNumKey = m_aInfo[nType].nNumKey;
	m_nTotalCtr = 0;
	m_bFinish = false;

	if (m_nBrendCtr == 0)
	{
		m_nCounter = 0;
	}
}

//============================
// 更新
//============================
void CMotion::Update(void)
{
	int nNextKey = m_nKey + 1;
	if (nNextKey >= m_nNumKey)
	{
		if (m_bLoop)
		{// 始めから
			nNextKey = 0;
			m_nTotalCtr = 0;
		}
		else
		{// 終了
			nNextKey = m_nKey;
			m_bFinish = true;
			m_nCounter = 0;
			m_nTotalCtr = 0;
		}
	}

	m_nKey %= m_nNumKey;

	UpdateParts(nNextKey);

	m_nCounter++;
	m_nTotalCtr++;
}

//============================
// パーツの更新
//============================
void CMotion::UpdateParts(int nNextKey)
{
	// ブレンド時に必要
	//今回設定されたもの
	//前回再生中のもの

	KEY key = KEY();	// 差分
	int nTypeNow = m_nType;
	int nKeyNow = m_nKey;
	int keyNext = nNextKey;
	int nFrame = m_aInfo[nTypeNow].aKeyInfo[nKeyNow].nFrame;	// 今回フレーム

#if 1
	if (m_nBrendCtr > 0)
	{
		nTypeNow = m_nTypeOld;
		keyNext = m_nKey;
		nKeyNow = m_nKeyOld;
		nFrame = m_aInfo[nTypeNow].aKeyInfo[nKeyNow].nFrame;	// 今回フレーム
		//nFrame = m_nBrendCtr;
	}
#endif

	//全モデル(パーツ)の更新
	for (int i = 0; i < m_nNumModel; i++)
	{
		CMotion::KEY KeyNext = m_aInfo[m_nType].aKeyInfo[keyNext].aKey[i];		// 次回キー
		CMotion::KEY KeyNow = m_aInfo[nTypeNow].aKeyInfo[nKeyNow].aKey[i];		// 今回キー

		// キーの情報から位置・向きを算出
		//==============================
		// 差分 = 次のキー - 現在のキー

		key.fPosX = KeyNext.fPosX - KeyNow.fPosX;
		key.fPosY = KeyNext.fPosY - KeyNow.fPosY;
		key.fPosZ = KeyNext.fPosZ - KeyNow.fPosZ;
		key.fRotX = KeyNext.fRotX - KeyNow.fRotX;
		key.fRotY = KeyNext.fRotY - KeyNow.fRotY;
		key.fRotZ = KeyNext.fRotZ - KeyNow.fRotZ;

		// 求める値 = 現在のキー + 差分 * (モーションカウンタ / 再生フレーム)
		key.fPosX = m_aKeyOrg[i].fPosX + KeyNow.fPosX + key.fPosX * (float(m_nCounter) / float(nFrame));
		key.fPosY = m_aKeyOrg[i].fPosY + KeyNow.fPosY + key.fPosY * (float(m_nCounter) / float(nFrame));
		key.fPosZ = m_aKeyOrg[i].fPosZ + KeyNow.fPosZ + key.fPosZ * (float(m_nCounter) / float(nFrame));
		key.fRotX = m_aKeyOrg[i].fRotX + KeyNow.fRotX + key.fRotX * (float(m_nCounter) / float(nFrame));
		key.fRotY = m_aKeyOrg[i].fRotY + KeyNow.fRotY + key.fRotY * (float(m_nCounter) / float(nFrame));
		key.fRotZ = m_aKeyOrg[i].fRotZ + KeyNow.fRotZ + key.fRotZ * (float(m_nCounter) / float(nFrame));

		// パーツの位置・向きを設定
		m_ppParts[i]->SetPos(D3DXVECTOR3(key.fPosX, key.fPosY, key.fPosZ));
		m_ppParts[i]->SetRot(D3DXVECTOR3(key.fRotX, key.fRotY, key.fRotZ));
	}

	if (m_nBrendCtr > 0)
	{
		m_nBrendCtr--;
		mylib_useful::Limit(m_nBrendCtr, 0);
	}

	// フレーム
	if (m_nCounter >= nFrame)
	{
		if (m_nBrendCtr > 0) m_nBrendCtr = 0;
		m_nCounter = 0;
		m_nKey++;
	}
}

//============================
// キー設定(ブレンド)
//============================
void CMotion::KeySetBrend()
{
	for (int i = 0; i < m_nNumModel; i++)
	{
		if (m_ppParts[i] == nullptr) continue;
		D3DXVECTOR3 rot = m_ppParts[i]->GetRot();
		D3DXVECTOR3 pos = m_ppParts[i]->GetPos();
		m_aKeyOld[i].fRotX = -rot.x;
		m_aKeyOld[i].fRotY = -rot.y;
		m_aKeyOld[i].fRotZ = -rot.z;
		m_aKeyOld[i].fPosX = -pos.x;
		m_aKeyOld[i].fPosY = -pos.y;
		m_aKeyOld[i].fPosZ = -pos.z;
	}
}

//============================
// キー設定(キー更新)
//============================
void CMotion::KeySetNormal()
{
	for (int i = 0; i < m_nNumModel; i++)
	{
		m_aKeyOld[i].fRotX = -m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[i].fRotX;
		m_aKeyOld[i].fRotY = -m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[i].fRotY;
		m_aKeyOld[i].fRotZ = -m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[i].fRotZ;
		m_aKeyOld[i].fPosX = -m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[i].fPosX;
		m_aKeyOld[i].fPosY = -m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[i].fPosY;
		m_aKeyOld[i].fPosZ = -m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[i].fPosZ;
	}
}

//============================
// タイプ取得
//============================
int CMotion::GetType(void)
{
	return m_nType;
}

//============================
// 終了判定取得
//============================
bool CMotion::IsFinish(void)
{
	return m_bFinish;
}

//============================
// モーション情報設定
//============================
void CMotion::SetInfo(INFO *info,int nNum)
{
	for (int nCntInfo = 0; nCntInfo < nNum; nCntInfo++)
	{
		m_aInfo[nCntInfo] = info[nCntInfo];
	}
}

//============================
// モデル情報設定
//============================
void CMotion::SetModel(CParts ** ppParts, int nNumModel)
{
	m_nNumModel = nNumModel;

	m_ppParts = new CParts*[m_nNumModel];

	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		m_ppParts[nCntParts] = ppParts[nCntParts];
		m_aKeyOrg[nCntParts].fPosX = m_ppParts[nCntParts]->GetPos().x;
		m_aKeyOrg[nCntParts].fPosY = m_ppParts[nCntParts]->GetPos().y;
		m_aKeyOrg[nCntParts].fPosZ = m_ppParts[nCntParts]->GetPos().z;
		m_aKeyOrg[nCntParts].fRotX = m_ppParts[nCntParts]->GetRot().x;
		m_aKeyOrg[nCntParts].fRotY = m_ppParts[nCntParts]->GetRot().y;
		m_aKeyOrg[nCntParts].fRotZ = m_ppParts[nCntParts]->GetRot().z;
	}
}

//============================
// パーツファイル読込
//============================
char **CMotion::ReadParts(const char * pReadFile, int *pnNumFile)
{
	char *pComp = new char[128];				//ゴミ
	char *pFilepass[MAX_PARTS] = {};		//ファイルパス
	int nNumParts = 0;

	FILE *pFile;

	pFile = fopen(pReadFile, "r");

	if (pFile != nullptr)
	{
		do
		{
			fscanf(pFile, "%s", pComp);

			if (strncmp(pComp, "#", 1) == 0)
			{// これのあとコメント
				fgets(pComp, 128, pFile);
				continue;
			}

			if (strcmp(pComp, "NUM_MODEL") == 0)
			{//総数取得
				fscanf(pFile, "%s %d", pComp, &nNumParts);
				m_nNumModel = nNumParts;
			}
			else if (strcmp(pComp, "MODEL_FILENAME") == 0)
			{//ファイル読込
				for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
				{
					if (pFilepass[nCntCrt] == nullptr)
					{
						pFilepass[nCntCrt] = new char[128];
						fscanf(pFile, "%s %s", pComp, pFilepass[nCntCrt]);
						break;
					}
				}
			}
		} while (strcmp(pComp, "END_SCRIPT") != 0);
	}

	*pnNumFile = nNumParts;

	return pFilepass;
}

//============================
// セットアップ読込
//============================
void CMotion::ReadSetUp(const char * pReadFile, CParts **ppParts)
{
	char *pComp = new char[128];				//ゴミ
	D3DXVECTOR3 pos[MAX_PARTS];			//プリセット位置
	D3DXVECTOR3 rot[MAX_PARTS];			//プリセット向き
	int aParent[MAX_PARTS];				//親モデルの有無
	int nNumParts = 0;					//パーツ総数

	for (int nCntNull = 0; nCntNull < MAX_PARTS; nCntNull++)
	{
		pos[nCntNull] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot[nCntNull] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	FILE *pFile;

	pFile = fopen(pReadFile, "r");

	if (pFile != nullptr)
	{
		do
		{
			fscanf(pFile, "%s", pComp);

			if (strncmp(pComp, "#", 1) == 0)
			{// これのあとコメント
				fgets(pComp, 128, pFile);
				continue;
			}

			if (strcmp(pComp, "NUM_MODEL") == 0)
			{//総数取得
				fscanf(pFile, "%s %d", pComp, &nNumParts);
				m_nNumModel = nNumParts;
			}
			else if (strcmp(pComp, "CHARACTERSET") == 0)
			{//オフセット情報取得開始
				int nCntSet = 0;

				do
				{
					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "PARTSSET") == 0)
					{//パーツ情報取得開始
						while (TRUE)
						{
							fscanf(pFile, "%s", pComp);

							if (strcmp(pComp, "END_PARTSSET") == 0)
							{
								break;
							}
							else if (strcmp(pComp, "PARENT") == 0)
							{
								fscanf(pFile, "%s %d", pComp, &aParent[nCntSet]);
							}
							else if (strcmp(pComp, "POS") == 0)
							{
								fscanf(pFile, "%s %f %f %f", pComp, &pos[nCntSet].x, &pos[nCntSet].y, &pos[nCntSet].z);
							}
							else if (strcmp(pComp, "ROT") == 0)
							{
								fscanf(pFile, "%s %f %f %f", pComp, &rot[nCntSet].x, &rot[nCntSet].y, &rot[nCntSet].z);
							}
						}

						//取得終了で加算
						nCntSet++;
					}

				} while (strcmp(pComp, "END_CHARACTERSET") != 0);
			}

		} while (strcmp(pComp, "END_SCRIPT") != 0);

		fclose(pFile);
	}
	else
	{//ファイル読込に失敗
		return;
	}

	//親モデルの設定(全パーツ分)
	for (int nCntPrt = 0; nCntPrt < nNumParts; nCntPrt++)
	{
		if (aParent[nCntPrt] <= -1)
		{
			ppParts[nCntPrt]->SetParent(nullptr);
		}
		else
		{
			ppParts[nCntPrt]->SetParent(ppParts[aParent[nCntPrt]]);
		}
	}

	delete[] pComp;		//ゴミ
}

//============================
// モーション内容読込
//============================
void CMotion::ReadMotions(const char * pReadFile, CMotion::INFO *pInfo)
{
	char *pComp = new char[128];				//ゴミ
	char *pFilepass[MAX_PARTS] = {};		//ファイルパス
	D3DXVECTOR3 pos[MAX_PARTS];			//プリセット位置
	D3DXVECTOR3 rot[MAX_PARTS];			//プリセット向き
	int aParent[MAX_PARTS];				//親モデルの有無
	int nNumParts = 0;					//パーツ総数
	int nNumKey = 0;					//キー数
	int nFrame = 0;						//フレーム数
	int nLoop = 0;						//ループ [ 0:しない / 1:する ]
	int nKeyCtr = 0;			//モーション読込時のキーカウンター
	int nKeySetCtr = 0;			//モーション読込時のキーセットカウンター
	int nMotionCtr = 0;			//モーション数
	CMotion::INFO *info = new CMotion::INFO[16];		//モーション情報

	for (int nCntNull = 0; nCntNull < MAX_PARTS; nCntNull++)
	{
		aParent[nCntNull] = -1;
	}

	FILE *pFile;

	pFile = fopen(pReadFile, "r");

	if (pFile != nullptr)
	{
		do
		{
			fscanf(pFile, "%s", pComp);

			if (strncmp(pComp, "#", 1) == 0)
			{// これのあとコメント
				fgets(pComp, 128, pFile);
				continue;
			}

			if (strcmp(pComp, "NUM_MODEL") == 0)
			{//総数取得
				fscanf(pFile, "%s %d", pComp, &nNumParts);
				m_nNumModel = nNumParts;
			}
			else if (strcmp(pComp, "MOTIONSET") == 0)
			{
				do
				{//モーション情報を読む

					fscanf(pFile, "%s", pComp);

					if (strncmp(pComp, "#", 1) == 0)
					{// これのあとコメント
						fgets(pComp, 128, pFile);
						continue;
					}
					else if (strcmp(pComp, "END_MOTIONSET") == 0)
					{
						nMotionCtr++;
						nKeyCtr = 0;
						nKeySetCtr = 0;
						break;
					}
					else if (strcmp(pComp, "LOOP") == 0)
					{//ループ設定取得
						fscanf(pFile, "%s %d", pComp, &nLoop);

						info[nMotionCtr].bLoop = (nLoop == 0 ? false : true);
					}
					else if (strcmp(pComp, "NUM_KEY") == 0)
					{//キー数取得
						fscanf(pFile, "%s %d", pComp, &nNumKey);

						info[nMotionCtr].nNumKey = nNumKey;
					}
					else if (strcmp(pComp, "KEYSET") == 0)
					{//キーセット情報取得開始
						do
						{
							fscanf(pFile, "%s", pComp);

							if (strncmp(pComp, "#", 1) == 0)
							{// これのあとコメント
								fgets(pComp, 128, pFile);
								continue;
							}
							else if (strcmp(pComp, "END_KEYSET") == 0)
							{
								//取得終了で加算
								nKeySetCtr++;
								nKeyCtr = 0;

								break;
							}
							else if (strcmp(pComp, "FRAME") == 0)
							{//フレーム数取得
								fscanf(pFile, "%s %d", pComp, &nFrame);

								info[nMotionCtr].aKeyInfo[nKeySetCtr].nFrame = nFrame;
							}
							else if (strcmp(pComp, "KEY") == 0)
							{//パーツ情報取得開始
								while (TRUE)
								{
									fscanf(pFile, "%s", pComp);

									if (strncmp(pComp, "#", 1) == 0)
									{// これのあとコメント
										fgets(pComp, 128, pFile);
										continue;
									}
									else if (strcmp(pComp, "END_KEY") == 0)
									{
										//取得終了で加算
										nKeyCtr++;
										break;
									}
									else if (strcmp(pComp, "POS") == 0)
									{
										float fX, fY, fZ;

										fscanf(pFile, "%s %f %f %f", pComp, &fX, &fY, &fZ);

										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosX = fX;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosY = fY;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosZ = fZ;
									}
									else if (strcmp(pComp, "ROT") == 0)
									{
										float fX, fY, fZ;

										fscanf(pFile, "%s %f %f %f", pComp, &fX, &fY, &fZ);

										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotX = fX;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotY = fY;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotZ = fZ;
									}
								}

							}

						} while (strcmp(pComp, "END_KEYSET") != 0);
					}
				} while (strcmp(pComp, "END_MOTIONSET") != 0);
			}

		} while (strcmp(pComp, "END_SCRIPT") != 0);

		fclose(pFile);
	}
	else
	{//ファイル読込に失敗
		return;
	}

	////モーション情報設定
	//m_pMotion->SetInfo(info, nMotionCtr);

	delete[] pComp;		//ゴミ
	delete[] info;
}

//============================
// ファイル読み込み
//============================
void CMotion::ReadFile(char *pFilePass)
{
	char *pComp = new char[128];		//ゴミ
	D3DXVECTOR3 pos[MAX_PARTS];	//位置
	D3DXVECTOR3 rot[MAX_PARTS];	//向き
	int nNumKey = 0;		//キー数
	int nFrame = 0;		//フレーム数
	int nLoop = 0;		//ループ [ 0:しない / 1:する ]

	for (int nCntNull = 0; nCntNull < MAX_PARTS; nCntNull++)
	{
		pos[nCntNull] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot[nCntNull] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	FILE *pFile;

	pFile = fopen(pFilePass, "r");

	if (pFile != nullptr)
	{
		do
		{//とりあえず読む
			fscanf(pFile, "%s", pComp);

			if (strcmp(pComp, "MOTIONSET") == 0)
			{
				do
				{//モーション情報を読む

					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "LOOP") == 0)
					{//ループ設定取得
						fscanf(pFile, "%s %d", pComp, &nLoop);
					}
					else if (strcmp(pComp, "NUM_KEY") == 0)
					{//キー数取得
						fscanf(pFile, "%s %d", pComp, &nNumKey);
					}
					else if (strcmp(pComp, "KEYSET") == 0)
					{//プリセット情報取得開始
						int nCntSet = 0;

						do
						{
							fscanf(pFile, "%s", pComp);

							if (strcmp(pComp, "FRAME") == 0)
							{//キー数取得
								fscanf(pFile, "%s %d", pComp, &nFrame);
							}
							else if (strcmp(pComp, "KEY") == 0)
							{//パーツ情報取得開始
								while (TRUE)
								{
									fscanf(pFile, "%s", pComp);

									if (strcmp(pComp, "END_KEY") == 0)
									{
										break;
									}
									else if (strcmp(pComp, "POS") == 0)
									{
										fscanf(pFile, "%s %f %f %f", pComp, &pos[nCntSet].x, &pos[nCntSet].y, &pos[nCntSet].z);
									}
									else if (strcmp(pComp, "ROT") == 0)
									{
										fscanf(pFile, "%s %f %f %f", pComp, &rot[nCntSet].x, &rot[nCntSet].y, &rot[nCntSet].z);
									}
								}

								//取得終了で加算
								nCntSet++;
							}

						} while (strcmp(pComp, "END_KEYSET") != 0);
					}
				} while (strcmp(pComp, "END_MOTIONSET") != 0);
			}
		} while (strcmp(pComp, "END_MOTIONSET") != 0);

		fclose(pFile);
	}
	else
	{//ファイル読込に失敗
		return;
	}

	//読み込んだ値を代入
	for (int nCntNull = 0; nCntNull < MAX_PARTS; nCntNull++)
	{
		m_ppParts[nCntNull]->SetPos(pos[nCntNull]);
		m_ppParts[nCntNull]->SetRot(rot[nCntNull]);
	}

	delete[] pComp;		//ゴミ
}
