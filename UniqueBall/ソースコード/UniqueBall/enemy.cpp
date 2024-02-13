//===============================================
//
// 敵(enemy.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "enemy.h"
#include "manager.h"	//ほぼ確で必要
#include "game.h"		//ほぼ確で必要
#include "camera.h"			//移動方向
#include "sound.h"			//演出等に必要
#include "parts.h"			//親子関係
#include "motion.h"			//モーション
#include "Field.h"			//地面との当たり判定
#include "wp_ball.h"		//ブースト
#include "Xmodel.h"			//モデル
#include "debugproc.h"		//デバッグ
#include "state_life.h"		//状態管理
#include "ef_smoke.h"		//煙演出
#include "wp_ball.h"		//ボールの当たり判定
#include "player.h"		//プレイヤー

//デバッグ用(おそらく消す)
#include "bullet.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// マクロ定義
//============================

namespace
{
	const float NUM_GETAREA(100.0f);		//取得領域の半径
	const float NUM_HYGETAREA(100.0f);		//取得加速領域の半径
	const float NUM_GETAREA_SPEED(8.0f);	//取得領域の引き寄せ速度
	const float NUM_COLL(30.0f);			//当たり判定半径
	const float NUM_HP(1.0f);				//HP量
	const float NUM_ATTACK(1.0f);			//攻撃力
	const float NUM_SPEED_STOP(0.0001f);	//移動量を0にする条件値
	const float NUM_HEART(50.0f);			//心臓位置
	const float NUM_BOOST(40.0f);			//ブースト力
	const float NUM_GRAV(0.5f);				//重力
	const float NUM_RESIST(0.05f);				//移動抵抗
	const int HANDR_IDX = 8;			//右手Idx
	const int READ_PSIZE(256);			//読込ポインタサイズ
	const int DAMAGE_CT(1);				//ダメージ無敵時間
	const int WAIT_JC(10);				//ジャストキャッチ猶予時間
	const int COOLTIME_THROW(100);				//投げクールタイム
}

#define MOTION_FILE "data\\SET\\MOTION\\motion_player.txt"		//モーションファイルパス

//============================
// 定数定義
//============================

//============================
// 静的メンバ変数宣言
//============================

//============================
// コンストラクタ
//============================
CEnemy::CEnemy(int nPriority) : CObject(nPriority)
{
	m_nNumModel = 0;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = MAX_PLAYER_SIZE;
	m_posOld = m_pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = m_rot;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_fHeart = 0.0f;
	m_bJump = false;
	m_pMotion = nullptr;
	ZeroMemory(&m_param, sizeof(m_param));
	m_pStateLife = nullptr;

	m_pBall = nullptr;

	for (int nCntPart = 0; nCntPart < MAX_PARTS; nCntPart++)
	{
		m_apPart[nCntPart] = nullptr;
	}

	m_eMember = my_Identity::MEMBER_NONE;
	m_nCatchCtr = -1;
	m_nThrowCtr = 0;
}

//============================
// デストラクタ
//============================
CEnemy::~CEnemy()
{

}

//====================================
// プレイヤーの初期化(デフォ)
//====================================
HRESULT CEnemy::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_size = MAX_PLAYER_SIZE;
	m_rotDest = m_rot;
	m_fHeart = NUM_HEART;
	m_param.fLife = NUM_HP;
	m_param.fLifeMax = NUM_HP;
	m_param.fColl = NUM_COLL;
	m_eMember = my_Identity::MEMBER_PLAYER0;

	//ファイル読込＆パーツ生成・初期化
	ReadFile();

	//タイプ設定
	SetType(TYPE_ENEMY);

	return S_OK;
}

//====================================
// プレイヤーの初期化(位置オバロ)
//====================================
HRESULT CEnemy::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nNumParts)
{
	m_pos = pos;
	m_posOld = pos;
	m_size = MAX_PLAYER_SIZE;
	//m_rot = rot;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
	m_rotDest = rot;
	m_nNumModel = nNumParts;
	m_fHeart = NUM_HEART;
	m_param.fLife = NUM_HP;
	m_param.fLifeMax = NUM_HP;
	m_param.fAttack = NUM_ATTACK;
	m_param.fColl = NUM_COLL;
	m_eMember = my_Identity::MEMBER_ENEMY;

	//モーションの生成、初期化
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//ファイル読込＆パーツ生成・初期化
	ReadFile();

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	//状態の生成
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//タイプ設定
	SetType(TYPE_ENEMY);

	return S_OK;
}

//============================
// プレイヤーの終了
//============================
void CEnemy::Uninit(void)
{
	for (int nCntPrt = 0; nCntPrt < MAX_PARTS; nCntPrt++)
	{
		if (m_apPart[nCntPrt] != nullptr)
		{
			m_apPart[nCntPrt]->Uninit();
			//delete m_apPart[nCntPrt];
			m_apPart[nCntPrt] = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pBall != nullptr)
	{
		m_pBall->Uninit();
		m_pBall = nullptr;
	}

	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	//終了
	Release();
}

//============================
// プレイヤーの更新
//============================
void CEnemy::Update(void)
{
	//座標取得
	float fRotMove = m_rot.y;		//現在の向き
	float fRotDest = m_rot.y;		//目標の向き
	float fRotDiff = 0.0f;			//差分

	float fLength = 0.0f;			//プレイヤーとの直線距離

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		CPlayer *pPlayer = CScene::GetPlayer(i);

		if (pPlayer != nullptr)
		{//プレイヤー側を向く
			D3DXVECTOR3 posP = pPlayer->GetPos();
			D3DXVECTOR3 vecDiff = D3DXVECTOR3(posP.x - m_pos.x,
				0.0f,
				posP.z - m_pos.z);

			float fLen = mylib_useful::VecLength(vecDiff.x, vecDiff.z);
			//距離
			if (fLength > fLen || i == 0)
			{
				fLength = fLen;
				fRotDest = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;		//目標の向き
			}
		}
	}

	D3DXVECTOR3 pos = GetPos();
	m_posOld = pos;

	fRotDiff = fRotDest - fRotMove;
	RotAdj(fRotDiff);

	m_rotDest.y = fRotDiff;

	if (!m_pMotion->IsFinish())
	{
		m_pMotion->Update();
	}

	//投げ
	if (m_nThrowCtr == 0 &&
		(m_pMotion->GetType() != MOTIONTYPE_THROW_RDY && m_pMotion->GetType() != MOTIONTYPE_THROW))
	{//カウンター0＆投げていない

		if (m_pBall != nullptr)
		{//投げ構え
			m_pMotion->Set(MOTIONTYPE_THROW_RDY);
		}
	}

	if (m_pBall != nullptr &&
		m_pMotion->GetType() == MOTIONTYPE_THROW_RDY &&
		m_pMotion->IsFinish())
	{//投げ
		Throw();
	}

	//キャッチ前提
	m_nCatchCtr = 0;

	CollisionBall(pos);

	CEnemy::MOTIONTYPE Mtype = MOTIONTYPE(m_pMotion->GetType());

	//重力
	m_move.y -= NUM_GRAV;

	//移動量を適用
	pos += m_move;

	//高さを取得する
	CollisionField(pos);

	//落下死救済
	if (pos.y <= -50.0f)
	{
		pos.y = -50.0f;
		m_move.y = 0.0f;
		m_bJump = false;
		//static_assert(true, "沼");
	}

	if (m_nThrowCtr > 0)
	{
		m_nThrowCtr--;
	}

	//武器の更新
	if (m_pBall != nullptr)
	{
		m_pBall->Update();
	}

	//キャッチ状態カウント
	if (m_nCatchCtr >= WAIT_JC &&m_pMotion->GetType() == MOTIONTYPE_CATCH)
	{//カウントオーバーでキャッチ解除
	 //m_pMotion->Set(MOTIONTYPE_NEUTRAL);
		m_nCatchCtr++;
	}

	if (m_nCatchCtr != -1)
	{
		m_nCatchCtr++;
	}

	if (m_pStateLife != nullptr)
	{
		CState_Life::STATE state = m_pStateLife->GetState();

		m_pStateLife->Update();

		if (state == CState_Life::STATE_DAMAGE &&
			m_pStateLife->GetState() == CState_Life::STATE_NORMAL)
		{//DAMAGEからNORMALに戻ったら

			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(D3DXMATERIAL));

			//パーツ一つずつ塗装する
			for (int nCntPrt = 0; nCntPrt < m_nNumModel; nCntPrt++)
			{
				if (m_apPart[nCntPrt] != nullptr)
				{
					m_apPart[nCntPrt]->SetMat(mat);
					m_apPart[nCntPrt]->Update();
				}
			}
		}
	}

	//移動量を更新(減衰させる)慣性
	m_move.x += (0 - m_move.x) * NUM_RESIST;
	m_move.z += (0 - m_move.z) * NUM_RESIST;

	if (m_move.x <= NUM_SPEED_STOP && m_move.x >= -NUM_SPEED_STOP)
	{//規定値以下で移動量を0に
		m_move.x = 0.0f;
	}
	if (m_move.z <= NUM_SPEED_STOP && m_move.z >= -NUM_SPEED_STOP)
	{//規定値以下で移動量を0に
		m_move.z = 0.0f;
	}

	if (m_pMotion->IsFinish())
	{
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
	}
}

//============================
// プレイヤーの描画
//============================
void CEnemy::Draw(void)
{
	//描画
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

										//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//============================
// プレイヤーの生成
//============================
CEnemy *CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = nullptr;

	//オブジェクト生成&初期化
	pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		pEnemy->Init(pos, rot, 1);
	}
	else
	{
		return nullptr;
	}

	return pEnemy;
}

//============================
// 向きの修正
//============================
void CEnemy::RotAdj(float fRotDiff)
{
	//角度の値を修正する
	RotLimit(&fRotDiff);

	m_rot.y += fRotDiff * 0.1f;		//移動方向(角度)の補正

	//角度の値を修正する
	RotLimit(&m_rotDest.x);
	RotLimit(&m_rotDest.y);
	RotLimit(&m_rotDest.z);
	RotLimit(&m_rot.x);
	RotLimit(&m_rot.y);
	RotLimit(&m_rot.z);
}

//============================
// ファイル読み込み(motion)
//============================
void CEnemy::ReadFile(void)
{
	char *pComp = new char[READ_PSIZE];				//ゴミ
	char *pFilepass[MAX_PARTS] = {};		//ファイルパス
	D3DXVECTOR3 pos[MAX_PARTS];			//プリセット位置
	D3DXVECTOR3 rot[MAX_PARTS];			//プリセット向き
	int aParent[MAX_PARTS];				//親モデルの有無
	int nNumParts = 0;					//パーツ総数
	D3DXVECTOR3 **ppPos = nullptr;		//位置
	D3DXVECTOR3 **ppRot = nullptr;		//向き
	int nNumKey = 0;					//キー数
	int nFrame = 0;						//フレーム数
	int nLoop = 0;						//ループ [ 0:しない / 1:する ]

	int nKeyCtr = 0;			//モーション読込時のキーカウンター
	int nKeySetCtr = 0;			//モーション読込時のキーセットカウンター
	int nMotionCtr = 0;			//モーション数
	CMotion::INFO *info = new CMotion::INFO[16];		//モーション情報

	for (int nCntNull = 0; nCntNull < MAX_PARTS; nCntNull++)
	{
		pos[nCntNull] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot[nCntNull] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		aParent[nCntNull] = -1;
	}

	FILE *pFile;

	pFile = fopen(MOTION_FILE, "r");

	if (pFile != nullptr)
	{
		do
		{
			fscanf(pFile, "%s", pComp);

			if (strncmp(pComp, "#", 1) == 0)
			{// これのあとコメント
				fgets(pComp, READ_PSIZE, pFile);
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
								//aParent[nCntSet] = -1;
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
			else if (strcmp(pComp, "MOTIONSET") == 0)
			{
				do
				{//モーション情報を読む

					fscanf(pFile, "%s", pComp);

					if (strncmp(pComp, "#", 1) == 0)
					{// これのあとコメント
						fgets(pComp, READ_PSIZE, pFile);
						continue;
					}
					else if (strcmp(pComp, "END_MOTIONSET") == 0)
					{
						nMotionCtr++;
						nKeyCtr = 0;
						nKeySetCtr = 0;

						if (ppPos != nullptr)
						{
							for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
							{
								delete[] ppPos[nCntMotKey];
							}

							delete[] ppPos;
						}

						if (ppRot != nullptr)
						{
							for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
							{
								delete[] ppRot[nCntMotKey];
							}

							delete[] ppRot;
						}

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

						//モーション時に必要な数だけ位置情報を生成
						//生成内容 : ppPos[キー数][パーツ数]
						//生成内容 : ppRot[キー数][パーツ数]
						ppPos = new D3DXVECTOR3*[nNumKey];
						ppRot = new D3DXVECTOR3*[nNumKey];

						for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
						{
							ppPos[nCntMotKey] = new D3DXVECTOR3[nNumParts];
							ppRot[nCntMotKey] = new D3DXVECTOR3[nNumParts];
						}
					}
					else if (strcmp(pComp, "KEYSET") == 0)
					{//キーセット情報取得開始
						do
						{
							fscanf(pFile, "%s", pComp);

							if (strncmp(pComp, "#", 1) == 0)
							{// これのあとコメント
								fgets(pComp, READ_PSIZE, pFile);
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
										fgets(pComp, READ_PSIZE, pFile);
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
										fscanf(pFile, "%s %f %f %f", pComp, &ppPos[nKeySetCtr][nKeyCtr].x, &ppPos[nKeySetCtr][nKeyCtr].y, &ppPos[nKeySetCtr][nKeyCtr].z);

										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosX = ppPos[nKeySetCtr][nKeyCtr].x;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosY = ppPos[nKeySetCtr][nKeyCtr].y;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fPosZ = ppPos[nKeySetCtr][nKeyCtr].z;
									}
									else if (strcmp(pComp, "ROT") == 0)
									{
										fscanf(pFile, "%s %f %f %f", pComp, &ppRot[nKeySetCtr][nKeyCtr].x, &ppRot[nKeySetCtr][nKeyCtr].y, &ppRot[nKeySetCtr][nKeyCtr].z);

										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotX = ppRot[nKeySetCtr][nKeyCtr].x;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotY = ppRot[nKeySetCtr][nKeyCtr].y;
										info[nMotionCtr].aKeyInfo[nKeySetCtr].aKey[nKeyCtr].fRotZ = ppRot[nKeySetCtr][nKeyCtr].z;
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

	//モデルの生成(全パーツ分)
	for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
	{
		m_apPart[nCntCrt] = CParts::Create(pFilepass[nCntCrt], pos[nCntCrt], rot[nCntCrt]);
	}

	//親モデルの設定(全パーツ分)
	for (int nCntPrt = 0; nCntPrt < nNumParts; nCntPrt++)
	{
		if (aParent[nCntPrt] <= -1)
		{
			m_apPart[nCntPrt]->SetParent(nullptr);
		}
		else
		{
			if (m_apPart[aParent[nCntPrt]] != nullptr)
			{
				m_apPart[nCntPrt]->SetParent(m_apPart[aParent[nCntPrt]]);
			}
		}
	}

	//モーション情報設定
	m_pMotion->SetInfo(info, nMotionCtr);

	delete[] pComp;		//ゴミ
	delete[] info;

	for (int nCntPass = 0; nCntPass < 32; nCntPass++)
	{
		if (pFilepass[nCntPass] != nullptr)
		{
			delete pFilepass[nCntPass];
		}
	}
}

//============================
// ダメージ処理
//============================
void CEnemy::Damage(float fDamege)
{
	D3DXMATERIAL mat;
	ZeroMemory(&mat, sizeof(D3DXMATERIAL));

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->GetState() != CState_Life::STATE_NORMAL)
		{//ダメージを受けられない状態なら
			return;
		}
	}

	m_param.fLife -= fDamege;

	if (m_param.fLife > m_param.fLifeMax)
	{
		m_param.fLife = m_param.fLifeMax;
	}

	if (m_pStateLife != nullptr)
	{
		mat = m_pStateLife->GetMat();
	}

	//パーツ一つずつ塗装する
	for (int nCntPrt = 0; nCntPrt < MAX_PARTS; nCntPrt++)
	{
		if (m_apPart[nCntPrt] != nullptr)
		{
			m_apPart[nCntPrt]->SetMat(mat);
		}
	}

	float fRatio = m_param.fLife / m_param.fLifeMax;

	if (m_param.fLife <= 0.0f)
	{
		//死んだらどうしようか
		CManager::GetInstance()->SetResult(CManager::RT_LOSE);
	}

	m_pStateLife->SetState(CState_Life::STATE_DAMAGE, DAMAGE_CT);
}

//============================
// モーション情報取得 (.hでは前方宣言のみのため、中身は.cppに書く)
//============================
CMotion *CEnemy::GetMotion(void)
{
	return m_pMotion;
}

//============================
// 角度の値を修正する
//============================
void CEnemy::RotLimit(float *pRot)
{
	if (*pRot > D3DX_PI)
	{
		*pRot -= D3DX_PI * 2;
	}
	else if (*pRot < -D3DX_PI)
	{
		*pRot += D3DX_PI * 2;
	}
}

//============================
//ブロックとの当たり判定(矩形)
//============================
void CEnemy::CollisionField(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;

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

		if (m_bJump)
		{
			D3DXVECTOR3 move = { 3.0f, 0.0f, 0.0f };

			//煙演出
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				CEf_Smoke * pSmoke = CEf_Smoke::Create(m_pos);
				pSmoke->SetMove(move);
				pSmoke->SetLife(15);
				pSmoke->SetLifeMax(15);

				move.x *= -1.0f;
			}

			m_pMotion->Set(MOTIONTYPE_GROUND);
		}

		pos.y = fHeight;
		m_move.y = 0.0f;
		m_bJump = false;
	}

	//座標設定(更新)
	SetPos(pos);
}

//============================
//ボールとの当たり判定(球)
//============================
void CEnemy::CollisionBall(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;

	for (int nCntPrt = 0; nCntPrt < PRIORITY_MAX; nCntPrt++)
	{
		CObject *pObject = CObject::GetTop(nCntPrt);

		while ((pObject != nullptr))
		{
			if (pObject != nullptr)
			{
				CObject::TYPE type = pObject->GetType();	//今回のオブジェクトのタイプ

				if (type == CObject::TYPE_BALL)
				{//ボールだったら

				 //キャスト
					CBall *pBall = static_cast<CBall*>(pObject);

					if (pBall != nullptr)
					{
						if (pBall->CollisionCircle(pos, NUM_GETAREA))
						{//ボールに当たっていたら

							my_Identity::eMember member = pBall->GetMember();

							if (member == m_eMember)		//自軍
							{
							}
							else if (member == my_Identity::MEMBER_NONE)	//無所属
							{
								if (m_pBall == nullptr)
								{
									m_pBall = pBall;
									m_pBall->SetParent(m_apPart[HANDR_IDX]);
									m_pBall->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
									m_pBall->SetMember(m_eMember);
									m_nThrowCtr = COOLTIME_THROW;
								}
							}
							else	//敵軍
							{
								//キャッチ
								if (m_nCatchCtr != -1)
								{
									if (m_pBall != nullptr)
									{//持っていたら放って
										D3DXMATRIX mtx = m_pBall->GetMtxWorld();
										m_pBall->Throw(D3DXVECTOR3(5.0f, 20.0f, 5.0f), 10.0f);
										m_pBall->SetParent(nullptr);
										m_pBall->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
										m_pBall->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
										m_pBall = nullptr;
									}

									m_pMotion->Set(MOTIONTYPE_CATCH);

									m_pBall = pBall;
									m_pBall->SetParent(m_apPart[HANDR_IDX]);
									m_pBall->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
									m_pBall->SetMember(m_eMember);
									m_nThrowCtr = COOLTIME_THROW;
								}
							}
						}
					}
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == Endまで行ったってことでこの優先度は終了
				break;
			}
		}
	}

	if (m_pBall != nullptr)
	{//ボールを入手した場合

		m_pBall->SetPosOld(mylib_const::DEFVEC3);
		m_pBall->SetPos(mylib_const::DEFVEC3);
		m_pBall->SetMove(mylib_const::DEFVEC3);
		m_pBall->SetParent(m_apPart[HANDR_IDX]);
		m_pBall->SetMember(m_eMember);
	}
}

//============================
// ボール投げ
//============================
void CEnemy::Throw()
{
	if (m_pBall == nullptr)
	{
		return;
	}

	D3DXMATRIX mtx = m_pBall->GetMtxWorld();

	//少し上向きにする
	D3DXVECTOR3 rot = m_rot;
	rot.x += my_Identity::THROW_PARABOLA;

	m_pBall->Throw(rot, my_Identity::THROW_SPEED);
	m_pBall->SetParent(nullptr);
	m_pBall->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	m_pBall->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pBall = nullptr;
	m_nThrowCtr = COOLTIME_THROW;

	m_pMotion->Set(MOTIONTYPE_THROW);
}
