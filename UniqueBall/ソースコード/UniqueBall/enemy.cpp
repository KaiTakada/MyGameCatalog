//===============================================
//
// �G(enemy.cpp)
// Author ���c ����
//
//===============================================

//============================
// �C���N���[�h�t�@�C��
//============================
#include "enemy.h"
#include "manager.h"	//�قڊm�ŕK�v
#include "game.h"		//�قڊm�ŕK�v
#include "camera.h"			//�ړ�����
#include "sound.h"			//���o���ɕK�v
#include "parts.h"			//�e�q�֌W
#include "motion.h"			//���[�V����
#include "Field.h"			//�n�ʂƂ̓����蔻��
#include "wp_ball.h"		//�u�[�X�g
#include "Xmodel.h"			//���f��
#include "debugproc.h"		//�f�o�b�O
#include "state_life.h"		//��ԊǗ�
#include "ef_smoke.h"		//�����o
#include "wp_ball.h"		//�{�[���̓����蔻��
#include "player.h"		//�v���C���[

//�f�o�b�O�p(�����炭����)
#include "bullet.h"
#include "block.h"
#include "enemy.h"
#include "item.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"

//============================
// �}�N����`
//============================

namespace
{
	const float NUM_GETAREA(100.0f);		//�擾�̈�̔��a
	const float NUM_HYGETAREA(100.0f);		//�擾�����̈�̔��a
	const float NUM_GETAREA_SPEED(8.0f);	//�擾�̈�̈����񂹑��x
	const float NUM_COLL(30.0f);			//�����蔻�蔼�a
	const float NUM_HP(1.0f);				//HP��
	const float NUM_ATTACK(1.0f);			//�U����
	const float NUM_SPEED_STOP(0.0001f);	//�ړ��ʂ�0�ɂ�������l
	const float NUM_HEART(50.0f);			//�S���ʒu
	const float NUM_BOOST(40.0f);			//�u�[�X�g��
	const float NUM_GRAV(0.5f);				//�d��
	const float NUM_RESIST(0.05f);				//�ړ���R
	const int HANDR_IDX = 8;			//�E��Idx
	const int READ_PSIZE(256);			//�Ǎ��|�C���^�T�C�Y
	const int DAMAGE_CT(1);				//�_���[�W���G����
	const int WAIT_JC(10);				//�W���X�g�L���b�`�P�\����
	const int COOLTIME_THROW(100);				//�����N�[���^�C��
}

#define MOTION_FILE "data\\SET\\MOTION\\motion_player.txt"		//���[�V�����t�@�C���p�X

//============================
// �萔��`
//============================

//============================
// �ÓI�����o�ϐ��錾
//============================

//============================
// �R���X�g���N�^
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
// �f�X�g���N�^
//============================
CEnemy::~CEnemy()
{

}

//====================================
// �v���C���[�̏�����(�f�t�H)
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

	//�t�@�C���Ǎ����p�[�c�����E������
	ReadFile();

	//�^�C�v�ݒ�
	SetType(TYPE_ENEMY);

	return S_OK;
}

//====================================
// �v���C���[�̏�����(�ʒu�I�o��)
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

	//���[�V�����̐����A������
	if (m_pMotion != nullptr)
	{
		return E_FAIL;
	}

	m_pMotion = new CMotion;
	m_pMotion->Init();

	//�t�@�C���Ǎ����p�[�c�����E������
	ReadFile();

	m_pMotion->SetModel(m_apPart, m_nNumModel);

	if (m_pBall != nullptr)
	{
		m_pBall = nullptr;
	}

	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	//��Ԃ̐���
	if (m_pStateLife != nullptr)
	{
		m_pStateLife->Uninit();
		m_pStateLife = nullptr;
	}

	m_pStateLife = CState_Life::Create();

	//�^�C�v�ݒ�
	SetType(TYPE_ENEMY);

	return S_OK;
}

//============================
// �v���C���[�̏I��
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

	//�I��
	Release();
}

//============================
// �v���C���[�̍X�V
//============================
void CEnemy::Update(void)
{
	//���W�擾
	float fRotMove = m_rot.y;		//���݂̌���
	float fRotDest = m_rot.y;		//�ڕW�̌���
	float fRotDiff = 0.0f;			//����

	float fLength = 0.0f;			//�v���C���[�Ƃ̒�������

	for (int i = 0; i < mylib_const::MAX_PLAYER; i++)
	{
		CPlayer *pPlayer = CScene::GetPlayer(i);

		if (pPlayer != nullptr)
		{//�v���C���[��������
			D3DXVECTOR3 posP = pPlayer->GetPos();
			D3DXVECTOR3 vecDiff = D3DXVECTOR3(posP.x - m_pos.x,
				0.0f,
				posP.z - m_pos.z);

			float fLen = mylib_useful::VecLength(vecDiff.x, vecDiff.z);
			//����
			if (fLength > fLen || i == 0)
			{
				fLength = fLen;
				fRotDest = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;		//�ڕW�̌���
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

	//����
	if (m_nThrowCtr == 0 &&
		(m_pMotion->GetType() != MOTIONTYPE_THROW_RDY && m_pMotion->GetType() != MOTIONTYPE_THROW))
	{//�J�E���^�[0�������Ă��Ȃ�

		if (m_pBall != nullptr)
		{//�����\��
			m_pMotion->Set(MOTIONTYPE_THROW_RDY);
		}
	}

	if (m_pBall != nullptr &&
		m_pMotion->GetType() == MOTIONTYPE_THROW_RDY &&
		m_pMotion->IsFinish())
	{//����
		Throw();
	}

	//�L���b�`�O��
	m_nCatchCtr = 0;

	CollisionBall(pos);

	CEnemy::MOTIONTYPE Mtype = MOTIONTYPE(m_pMotion->GetType());

	//�d��
	m_move.y -= NUM_GRAV;

	//�ړ��ʂ�K�p
	pos += m_move;

	//�������擾����
	CollisionField(pos);

	//�������~��
	if (pos.y <= -50.0f)
	{
		pos.y = -50.0f;
		m_move.y = 0.0f;
		m_bJump = false;
		//static_assert(true, "��");
	}

	if (m_nThrowCtr > 0)
	{
		m_nThrowCtr--;
	}

	//����̍X�V
	if (m_pBall != nullptr)
	{
		m_pBall->Update();
	}

	//�L���b�`��ԃJ�E���g
	if (m_nCatchCtr >= WAIT_JC &&m_pMotion->GetType() == MOTIONTYPE_CATCH)
	{//�J�E���g�I�[�o�[�ŃL���b�`����
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
		{//DAMAGE����NORMAL�ɖ߂�����

			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(D3DXMATERIAL));

			//�p�[�c����h������
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

	//�ړ��ʂ��X�V(����������)����
	m_move.x += (0 - m_move.x) * NUM_RESIST;
	m_move.z += (0 - m_move.z) * NUM_RESIST;

	if (m_move.x <= NUM_SPEED_STOP && m_move.x >= -NUM_SPEED_STOP)
	{//�K��l�ȉ��ňړ��ʂ�0��
		m_move.x = 0.0f;
	}
	if (m_move.z <= NUM_SPEED_STOP && m_move.z >= -NUM_SPEED_STOP)
	{//�K��l�ȉ��ňړ��ʂ�0��
		m_move.z = 0.0f;
	}

	if (m_pMotion->IsFinish())
	{
		m_pMotion->Set(MOTIONTYPE_NEUTRAL);
	}
}

//============================
// �v���C���[�̕`��
//============================
void CEnemy::Draw(void)
{
	//�`��
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

										//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//============================
// �v���C���[�̐���
//============================
CEnemy *CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = nullptr;

	//�I�u�W�F�N�g����&������
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
// �����̏C��
//============================
void CEnemy::RotAdj(float fRotDiff)
{
	//�p�x�̒l���C������
	RotLimit(&fRotDiff);

	m_rot.y += fRotDiff * 0.1f;		//�ړ�����(�p�x)�̕␳

	//�p�x�̒l���C������
	RotLimit(&m_rotDest.x);
	RotLimit(&m_rotDest.y);
	RotLimit(&m_rotDest.z);
	RotLimit(&m_rot.x);
	RotLimit(&m_rot.y);
	RotLimit(&m_rot.z);
}

//============================
// �t�@�C���ǂݍ���(motion)
//============================
void CEnemy::ReadFile(void)
{
	char *pComp = new char[READ_PSIZE];				//�S�~
	char *pFilepass[MAX_PARTS] = {};		//�t�@�C���p�X
	D3DXVECTOR3 pos[MAX_PARTS];			//�v���Z�b�g�ʒu
	D3DXVECTOR3 rot[MAX_PARTS];			//�v���Z�b�g����
	int aParent[MAX_PARTS];				//�e���f���̗L��
	int nNumParts = 0;					//�p�[�c����
	D3DXVECTOR3 **ppPos = nullptr;		//�ʒu
	D3DXVECTOR3 **ppRot = nullptr;		//����
	int nNumKey = 0;					//�L�[��
	int nFrame = 0;						//�t���[����
	int nLoop = 0;						//���[�v [ 0:���Ȃ� / 1:���� ]

	int nKeyCtr = 0;			//���[�V�����Ǎ����̃L�[�J�E���^�[
	int nKeySetCtr = 0;			//���[�V�����Ǎ����̃L�[�Z�b�g�J�E���^�[
	int nMotionCtr = 0;			//���[�V������
	CMotion::INFO *info = new CMotion::INFO[16];		//���[�V�������

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
			{// ����̂��ƃR�����g
				fgets(pComp, READ_PSIZE, pFile);
				continue;
			}

			if (strcmp(pComp, "NUM_MODEL") == 0)
			{//�����擾
				fscanf(pFile, "%s %d", pComp, &nNumParts);
				m_nNumModel = nNumParts;
			}
			else if (strcmp(pComp, "MODEL_FILENAME") == 0)
			{//�t�@�C���Ǎ�
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
			{//�I�t�Z�b�g���擾�J�n
				int nCntSet = 0;

				do
				{
					fscanf(pFile, "%s", pComp);

					if (strcmp(pComp, "PARTSSET") == 0)
					{//�p�[�c���擾�J�n
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

						//�擾�I���ŉ��Z
						nCntSet++;
					}

				} while (strcmp(pComp, "END_CHARACTERSET") != 0);
			}
			else if (strcmp(pComp, "MOTIONSET") == 0)
			{
				do
				{//���[�V��������ǂ�

					fscanf(pFile, "%s", pComp);

					if (strncmp(pComp, "#", 1) == 0)
					{// ����̂��ƃR�����g
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
					{//���[�v�ݒ�擾
						fscanf(pFile, "%s %d", pComp, &nLoop);

						info[nMotionCtr].bLoop = (nLoop == 0 ? false : true);
					}
					else if (strcmp(pComp, "NUM_KEY") == 0)
					{//�L�[���擾
						fscanf(pFile, "%s %d", pComp, &nNumKey);

						info[nMotionCtr].nNumKey = nNumKey;

						//���[�V�������ɕK�v�Ȑ������ʒu���𐶐�
						//�������e : ppPos[�L�[��][�p�[�c��]
						//�������e : ppRot[�L�[��][�p�[�c��]
						ppPos = new D3DXVECTOR3*[nNumKey];
						ppRot = new D3DXVECTOR3*[nNumKey];

						for (int nCntMotKey = 0; nCntMotKey < nNumKey; nCntMotKey++)
						{
							ppPos[nCntMotKey] = new D3DXVECTOR3[nNumParts];
							ppRot[nCntMotKey] = new D3DXVECTOR3[nNumParts];
						}
					}
					else if (strcmp(pComp, "KEYSET") == 0)
					{//�L�[�Z�b�g���擾�J�n
						do
						{
							fscanf(pFile, "%s", pComp);

							if (strncmp(pComp, "#", 1) == 0)
							{// ����̂��ƃR�����g
								fgets(pComp, READ_PSIZE, pFile);
								continue;
							}
							else if (strcmp(pComp, "END_KEYSET") == 0)
							{
								//�擾�I���ŉ��Z
								nKeySetCtr++;
								nKeyCtr = 0;

								break;
							}
							else if (strcmp(pComp, "FRAME") == 0)
							{//�t���[�����擾
								fscanf(pFile, "%s %d", pComp, &nFrame);

								info[nMotionCtr].aKeyInfo[nKeySetCtr].nFrame = nFrame;
							}
							else if (strcmp(pComp, "KEY") == 0)
							{//�p�[�c���擾�J�n
								while (TRUE)
								{
									fscanf(pFile, "%s", pComp);

									if (strncmp(pComp, "#", 1) == 0)
									{// ����̂��ƃR�����g
										fgets(pComp, READ_PSIZE, pFile);
										continue;
									}
									else if (strcmp(pComp, "END_KEY") == 0)
									{
										//�擾�I���ŉ��Z
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
	{//�t�@�C���Ǎ��Ɏ��s
		return;
	}

	//���f���̐���(�S�p�[�c��)
	for (int nCntCrt = 0; nCntCrt < nNumParts; nCntCrt++)
	{
		m_apPart[nCntCrt] = CParts::Create(pFilepass[nCntCrt], pos[nCntCrt], rot[nCntCrt]);
	}

	//�e���f���̐ݒ�(�S�p�[�c��)
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

	//���[�V�������ݒ�
	m_pMotion->SetInfo(info, nMotionCtr);

	delete[] pComp;		//�S�~
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
// �_���[�W����
//============================
void CEnemy::Damage(float fDamege)
{
	D3DXMATERIAL mat;
	ZeroMemory(&mat, sizeof(D3DXMATERIAL));

	if (m_pStateLife != nullptr)
	{
		if (m_pStateLife->GetState() != CState_Life::STATE_NORMAL)
		{//�_���[�W���󂯂��Ȃ���ԂȂ�
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

	//�p�[�c����h������
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
		//���񂾂�ǂ����悤��
		CManager::GetInstance()->SetResult(CManager::RT_LOSE);
	}

	m_pStateLife->SetState(CState_Life::STATE_DAMAGE, DAMAGE_CT);
}

//============================
// ���[�V�������擾 (.h�ł͑O���錾�݂̂̂��߁A���g��.cpp�ɏ���)
//============================
CMotion *CEnemy::GetMotion(void)
{
	return m_pMotion;
}

//============================
// �p�x�̒l���C������
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
//�u���b�N�Ƃ̓����蔻��(��`)
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
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_FIELD)
				{//�A�C�e����������

					CField *pField = (CField*)pObject;
					fHeight = pField->GetHeight(pos);
				}
				pObject = pObject->GetNext();
			}
			else
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	if (pos.y < fHeight && m_posOld.y >= fHeight)
	{//���n��

		if (m_bJump)
		{
			D3DXVECTOR3 move = { 3.0f, 0.0f, 0.0f };

			//�����o
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

	//���W�ݒ�(�X�V)
	SetPos(pos);
}

//============================
//�{�[���Ƃ̓����蔻��(��)
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
				CObject::TYPE type = pObject->GetType();	//����̃I�u�W�F�N�g�̃^�C�v

				if (type == CObject::TYPE_BALL)
				{//�{�[����������

				 //�L���X�g
					CBall *pBall = static_cast<CBall*>(pObject);

					if (pBall != nullptr)
					{
						if (pBall->CollisionCircle(pos, NUM_GETAREA))
						{//�{�[���ɓ������Ă�����

							my_Identity::eMember member = pBall->GetMember();

							if (member == m_eMember)		//���R
							{
							}
							else if (member == my_Identity::MEMBER_NONE)	//������
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
							else	//�G�R
							{
								//�L���b�`
								if (m_nCatchCtr != -1)
								{
									if (m_pBall != nullptr)
									{//�����Ă����������
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
			{// (pObject == NULL) == End�܂ōs�������Ă��Ƃł��̗D��x�͏I��
				break;
			}
		}
	}

	if (m_pBall != nullptr)
	{//�{�[������肵���ꍇ

		m_pBall->SetPosOld(mylib_const::DEFVEC3);
		m_pBall->SetPos(mylib_const::DEFVEC3);
		m_pBall->SetMove(mylib_const::DEFVEC3);
		m_pBall->SetParent(m_apPart[HANDR_IDX]);
		m_pBall->SetMember(m_eMember);
	}
}

//============================
// �{�[������
//============================
void CEnemy::Throw()
{
	if (m_pBall == nullptr)
	{
		return;
	}

	D3DXMATRIX mtx = m_pBall->GetMtxWorld();

	//����������ɂ���
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
