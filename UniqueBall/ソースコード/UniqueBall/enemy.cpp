//===============================================
//
// G(enemy.cpp)
// Author ûüc ÀË
//
//===============================================

//============================
// CN[ht@C
//============================
#include "enemy.h"
#include "manager.h"	//ÙÚmÅKv
#include "game.h"		//ÙÚmÅKv
#include "camera.h"			//Ú®ûü
#include "sound.h"			//oÉKv
#include "parts.h"			//eqÖW
#include "motion.h"			//[V
#include "Field.h"			//nÊÆÌ½è»è
#include "wp_ball.h"		//u[Xg
#include "Xmodel.h"			//f
#include "debugproc.h"		//fobO
#include "state_life.h"		//óÔÇ
#include "ef_smoke.h"		//o
#include "wp_ball.h"		//{[Ì½è»è
#include "player.h"		//vC[

//fobOp(¨»ç­Á·)
#include "bullet.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// }Nè`
//============================

namespace
{
	const float NUM_GETAREA(100.0f);		//æ¾ÌæÌ¼a
	const float NUM_HYGETAREA(100.0f);		//æ¾Á¬ÌæÌ¼a
	const float NUM_GETAREA_SPEED(8.0f);	//æ¾ÌæÌø«ñ¹¬x
	const float NUM_COLL(30.0f);			//½è»è¼a
	const float NUM_HP(1.0f);				//HPÊ
	const float NUM_ATTACK(1.0f);			//UÍ
	const float NUM_SPEED_STOP(0.0001f);	//Ú®Êð0É·éðl
	const float NUM_HEART(50.0f);			//SÊu
	const float NUM_BOOST(40.0f);			//u[XgÍ
	const float NUM_GRAV(0.5f);				//dÍ
	const float NUM_RESIST(0.05f);				//Ú®ïR
	const int HANDR_IDX = 8;			//EèIdx
	const int READ_PSIZE(256);			//Ç|C^TCY
	const int DAMAGE_CT(1);				//_[W³GÔ
	const int WAIT_JC(10);				//WXgLb`P\Ô
	const int COOLTIME_THROW(100);				//°N[^C
}

#define MOTION_FILE "data\\SET\\MOTION\\motion_player.txt"		//[Vt@CpX

//============================
// èè`
//============================

//============================
// ÃIoÏé¾
//============================

//============================
// RXgN^
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
// fXgN^
//============================
CEnemy::~CEnemy()
{

}

//====================================
// vC[Ìú»(ftH)
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

	//t@CÇp[c¶¬Eú»
	ReadFile();

	//^CvÝè
	SetType(TYPE_ENEMY);

	return S_OK;
}

//====================================
// vC[Ìú»(ÊuIo)
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

	//[VÌ¶¬Aú»
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//t@CÇp[c¶¬Eú»
	ReadFile();

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	//óÔÌ¶¬
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//^CvÝè
	SetType(TYPE_ENEMY);

	return S_OK;
}

//============================
// vC[ÌI¹
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

	//I¹
	Release();
}

//============================
// vC[ÌXV
//============================
void CEnemy::Update(void)
{
	//ÀWæ¾
	float fRotMove = m_rot.y;		//»ÝÌü«
	float fRotDest = m_rot.y;		//ÚWÌü«
	float fRotDiff = 0.0f;			//·ª

	float fLength = 0.0f;			//vC[ÆÌ¼ü£

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		CPlayer *pPlayer = CScene::GetPlayer(i);

		if (pPlayer != nullptr)
		{//vC[¤ðü­
			D3DXVECTOR3 posP = pPlayer->GetPos();
			D3DXVECTOR3 vecDiff = D3DXVECTOR3(posP.x - m_pos.x,
				0.0f,
				posP.z - m_pos.z);

			float fLen = mylib_useful::VecLength(vecDiff.x, vecDiff.z);
			//£
			if (fLength > fLen || i == 0)
			{
				fLength = fLen;
				fRotDest = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;		//ÚWÌü«
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

	//°
	if (m_nThrowCtr == 0 &&
		(m_pMotion->GetType() != MOTIONTYPE_THROW_RDY && m_pMotion->GetType() != MOTIONTYPE_THROW))
	{//JE^[0°Ä¢È¢

		if (m_pBall != nullptr)
		{//°\¦
			m_pMotion->Set(MOTIONTYPE_THROW_RDY);
		}
	}

	if (m_pBall != nullptr &&
		m_pMotion->GetType() == MOTIONTYPE_THROW_RDY &&
		m_pMotion->IsFinish())
	{//°
		Throw();
	}

	//Lb`Oñ
	m_nCatchCtr = 0;

	CollisionBall(pos);

	CEnemy::MOTIONTYPE Mtype = MOTIONTYPE(m_pMotion->GetType());

	//dÍ
	m_move.y -= NUM_GRAV;

	//Ú®ÊðKp
	pos += m_move;

	//³ðæ¾·é
	CollisionField(pos);

	//º~Ï
	if (pos.y <= -50.0f)
	{
		pos.y = -50.0f;
		m_move.y = 0.0f;
		m_bJump = false;
		//static_assert(true, "À");
	}

	if (m_nThrowCtr > 0)
	{
		m_nThrowCtr--;
	}

	//íÌXV
	if (m_pBall != nullptr)
	{
		m_pBall->Update();
	}

	//Lb`óÔJEg
	if (m_nCatchCtr >= WAIT_JC &&m_pMotion->GetType() == MOTIONTYPE_CATCH)
	{//JEgI[o[ÅLb`ð
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
		{//DAMAGE©çNORMALÉßÁ½ç

			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(D3DXMATERIAL));

			//p[cêÂ¸Âh·é
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

	//Ú®ÊðXV(¸³¹é)µ«
	m_move.x += (0 - m_move.x) * NUM_RESIST;
	m_move.z += (0 - m_move.z) * NUM_RESIST;

	if (m_move.x <= NUM_SPEED_STOP && m_move.x >= -NUM_SPEED_STOP)
	{//KèlÈºÅÚ®Êð0É
		m_move.x = 0.0f;
	}
	if (m_move.z <= NUM_SPEED_STOP && m_move.z >= -NUM_SPEED_STOP)
	{//KèlÈºÅÚ®Êð0É
		m_move.z = 0.0f;
	}

	if (m_pMotion->IsFinish())
	{
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
	}
}

//============================
// vC[Ì`æ
//============================
void CEnemy::Draw(void)
{
	//`æ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//foCXÌæ¾
	D3DXMATRIX mtxRot, mtxTrans;		//vZp}gbNX

										//[h}gbNXÌú»
	D3DXMatrixIdentity(&m_mtxWorld);

	//ü«ð½f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//Êuð½f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//[h}gbNXÌÝè
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//============================
// vC[Ì¶¬
//============================
CEnemy *CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = nullptr;

	//IuWFNg¶¬&ú»
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
// ü«ÌC³
//============================
void CEnemy::RotAdj(float fRotDiff)
{
	//pxÌlðC³·é
	RotLimit(&fRotDiff);

	m_rot.y += fRotDiff * 0.1f;		//Ú®ûü(px)Ìâ³

	//pxÌlðC³·é
	RotLimit(&m_rotDest.x);
	RotLimit(&m_rotDest.y);
	RotLimit(&m_rotDest.z);
	RotLimit(&m_rot.x);
	RotLimit(&m_rot.y);
	RotLimit(&m_rot.z);
}

//============================
// t@CÇÝÝ(motion)
//============================
void CEnemy::ReadFile(void)
{
	char *pComp = new char[READ_PSIZE];				//S~
	char *pFilepass[MAX_PARTS] = {};		//t@CpX
	D3DXVECTOR3 pos[MAX_PARTS];			//vZbgÊu
	D3DXVECTOR3 rot[MAX_PARTS];			//vZbgü«
	int aParent[MAX_PARTS];				//efÌL³
	int nNumParts = 0;					//p[c
	D3DXVECTOR3 **ppPos = nullptr;		//Êu
	D3DXVECTOR3 **ppRot = nullptr;		//ü«
	int nNumKey = 0;					//L[
	int nFrame = 0;						//t[
	int nLoop = 0;						//[v [ 0:µÈ¢ / 1:·é ]

	int nKeyCtr = 0;			//[VÇÌL[JE^[
	int nKeySetCtr = 0;			//[VÇÌL[ZbgJE^[
	int nMotionCtr = 0;			//[V
	CMotion::INFO *info = new CMotion::INFO[16];		//[Vîñ

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
			{// ±êÌ ÆRg
				fgets(pComp, READ_PSIZE, pFile);
				continue;
			}

			if (strcmp(pComp, "NUM_MODEL") == 0)
			{//æ¾
				fscanf(pFile, "%s %d", pComp, &nNumParts);
				m_nNumModel = nNumParts;
			}
			else if (strcmp(pComp, "MODEL_FILENAME") == 0)
			{//t@CÇ
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
			{//ItZbgîñæ¾Jn
				int nCntSet = 0;

				do
				{
					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "PARTSSET") == 0)
					{//p[cîñæ¾Jn
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

						//æ¾I¹ÅÁZ
						nCntSet++;
					}

				} while (strcmp(pComp, "END_CHARACTERSET") != 0);
			}
			else if (strcmp(pComp, "MOTIONSET") == 0)
			{
				do
				{//[VîñðÇÞ

					fscanf(pFile, "%s", pComp);

					if (strncmp(pComp, "#", 1) == 0)
					{// ±êÌ ÆRg
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
					{//[vÝèæ¾
						fscanf(pFile, "%s %d", pComp, &nLoop);

						info[nMotionCtr].bLoop = (nLoop == 0 ? false : true);
					}
					else if (strcmp(pComp, "NUM_KEY") == 0)
					{//L[æ¾
						fscanf(pFile, "%s %d", pComp, &nNumKey);

						info[nMotionCtr].nNumKey = nNumKey;

						//[VÉKvÈ¾¯Êuîñð¶¬
						//¶¬àe : ppPos[L[][p[c]
						//¶¬àe : ppRot[L[][p[c]
						ppPos = new D3DXVECTOR3*[nNumKey];
						ppRot = new D3DXVECTOR3*[nNumKey];

						for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
						{
							ppPos[nCntMotKey] = new D3DXVECTOR3[nNumParts];
							ppRot[nCntMotKey] = new D3DXVECTOR3[nNumParts];
						}
					}
					else if (strcmp(pComp, "KEYSET") == 0)
					{//L[Zbgîñæ¾Jn
						do
						{
							fscanf(pFile, "%s", pComp);

							if (strncmp(pComp, "#", 1) == 0)
							{// ±êÌ ÆRg
								fgets(pComp, READ_PSIZE, pFile);
								continue;
							}
							else if (strcmp(pComp, "END_KEYSET") == 0)
							{
								//æ¾I¹ÅÁZ
								nKeySetCtr++;
								nKeyCtr = 0;

								break;
							}
							else if (strcmp(pComp, "FRAME") == 0)
							{//t[æ¾
								fscanf(pFile, "%s %d", pComp, &nFrame);

								info[nMotionCtr].aKeyInfo[nKeySetCtr].nFrame = nFrame;
							}
							else if (strcmp(pComp, "KEY") == 0)
							{//p[cîñæ¾Jn
								while (TRUE)
								{
									fscanf(pFile, "%s", pComp);

									if (strncmp(pComp, "#", 1) == 0)
									{// ±êÌ ÆRg
										fgets(pComp, READ_PSIZE, pFile);
										continue;
									}
									else if (strcmp(pComp, "END_KEY") == 0)
									{
										//æ¾I¹ÅÁZ
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
	{//t@CÇÉ¸s
		return;
	}

	//fÌ¶¬(Sp[cª)
	for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
	{
		m_apPart[nCntCrt] = CParts::Create(pFilepass[nCntCrt], pos[nCntCrt], rot[nCntCrt]);
	}

	//efÌÝè(Sp[cª)
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

	//[VîñÝè
	m_pMotion->SetInfo(info, nMotionCtr);

	delete[] pComp;		//S~
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
// _[W
//============================
void CEnemy::Damage(float fDamege)
{
	D3DXMATERIAL mat;
	ZeroMemory(&mat, sizeof(D3DXMATERIAL));

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->GetState() != CState_Life::STATE_NORMAL)
		{//_[Wðó¯çêÈ¢óÔÈç
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

	//p[cêÂ¸Âh·é
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
		//ñ¾çÇ¤µæ¤©
		CManager::GetInstance()->SetResult(CManager::RT_LOSE);
	}

	m_pStateLife->SetState(CState_Life::STATE_DAMAGE, DAMAGE_CT);
}

//============================
// [Vîñæ¾ (.hÅÍOûé¾ÌÝÌ½ßAgÍ.cppÉ­)
//============================
CMotion *CEnemy::GetMotion(void)
{
	return m_pMotion;
}

//============================
// pxÌlðC³·é
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
//ubNÆÌ½è»è(é`)
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
				CObject::TYPE type = pObject->GetType();	//¡ñÌIuWFNgÌ^Cv

				if (type == CObject::TYPE_FIELD)
				{//ACe¾Á½ç

					CField *pField = (CField*)pObject;
					fHeight = pField->GetHeight(pos);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == EndÜÅsÁ½ÁÄ±ÆÅ±ÌDæxÍI¹
				break;
			}
		}
	}

	if (pos.y < fHeight && m_posOld.y >= fHeight)
	{//n

		if (m_bJump)
		{
			D3DXVECTOR3 move = { 3.0f, 0.0f, 0.0f };

			//o
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

	//ÀWÝè(XV)
	SetPos(pos);
}

//============================
//{[ÆÌ½è»è()
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
				CObject::TYPE type = pObject->GetType();	//¡ñÌIuWFNgÌ^Cv

				if (type == CObject::TYPE_BALL)
				{//{[¾Á½ç

				 //LXg
					CBall *pBall = static_cast<CBall*>(pObject);

					if (pBall != nullptr)
					{
						if (pBall->CollisionCircle(pos, NUM_GETAREA))
						{//{[É½ÁÄ¢½ç

							my_Identity::eMember member = pBall->GetMember();

							if (member == m_eMember)		//©R
							{
							}
							else if (member == my_Identity::MEMBER_NONE)	//³®
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
							else	//GR
							{
								//Lb`
								if (m_nCatchCtr != -1)
								{
									if (m_pBall != nullptr)
									{//ÁÄ¢½çúÁÄ
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
			{// (pObject == NULL) == EndÜÅsÁ½ÁÄ±ÆÅ±ÌDæxÍI¹
				break;
			}
		}
	}

	if (m_pBall != nullptr)
	{//{[ðüèµ½ê

		m_pBall->SetPosOld(mylib_const::DEFVEC3);
		m_pBall->SetPos(mylib_const::DEFVEC3);
		m_pBall->SetMove(mylib_const::DEFVEC3);
		m_pBall->SetParent(m_apPart[HANDR_IDX]);
		m_pBall->SetMember(m_eMember);
	}
}

//============================
// {[°
//============================
void CEnemy::Throw()
{
	if (m_pBall == nullptr)
	{
		return;
	}

	D3DXMATRIX mtx = m_pBall->GetMtxWorld();

	//­µãü«É·é
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
