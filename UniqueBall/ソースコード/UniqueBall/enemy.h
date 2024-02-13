//===============================================
//
// G(enemy.h)
// Author ûüc ÀË
//
//===============================================
#ifndef _ENEMY_H_		//±Ì}Nè`ª³êÄÈ©Á½ç
#define _ENEMY_H_		//ñdCN[hh~Ì}Nðè`·é

//=========================
// CN[ht@C
//=========================
#include "object.h"
#include "identity.h"

//=========================
// }Nè`
//=========================
#define MAX_PARTS (32)		//Ååp[c
#define MAX_PLAYER_WEAPON (2)		//í
#define MAX_PLAYER_SIZE D3DXVECTOR3(30.0f,60.0f,30.0f)		//TCY

//=========================
// Oûé¾
//=========================
class CParts;
class CMotion;
class CBall;
class CGauge;
class CCursor;
class CState_Life;

//=========================
// vC[NX
//=========================
class CEnemy : public CObject
{
public:

	//====================
	// [VÌíÞ
	//====================
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	//Ò@
		MOTIONTYPE_WALK,	//à«
		MOTIONTYPE_DASH,	//_bV
		MOTIONTYPE_JUMP,	//Wv
		MOTIONTYPE_JUMP2,	//2iWv
		MOTIONTYPE_GROUND,	//n
		MOTIONTYPE_THROW_RDY,	//°p\¦é
		MOTIONTYPE_THROW,	//°é
		MOTIONTYPE_CATCH_RDY,	//Lb`p\¦é
		MOTIONTYPE_CATCH,	//Lb`
		MOTIONTYPE_TACKLE,	//^bN
		MOTIONTYPE_MAX,
	}MOTIONTYPE;

	//====================
	// LÌp[^
	//====================
	typedef struct
	{
		float fLife;		//ÌÍ
		float fLifeMax;		//ÅåÌÍ
		float fSpeed;		//Ú®¬x
		float fAttack;		//UÍ
		float fColl;		//½è»èÌ¼a
	}Param;

	CEnemy(int nPriority = 3);		//RXgN^
	~CEnemy();		//fXgN^

	HRESULT Init(void);							//ú»(ftH)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//ú»(Io)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//ú»(Io)
	void Uninit(void);							//I¹
	void Update(void);							//XV
	void Draw(void);							//`æ

	static CEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//¶¬

	bool GetJump(void) { return m_bJump; }						//Wv»è
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//OñÊuæ¾
	D3DXVECTOR3 GetPos(void) { return m_pos; }					//»ÝÊuæ¾
	D3DXVECTOR3 GetRot(void) { return m_rot; }					//ü«æ¾
	D3DXVECTOR3 GetMove(void) { return m_move; }				//Ú®Êæ¾
	D3DXVECTOR3 GetSize(void) { return m_size; }				//TCYæ¾
	float GetHeartPos(void) { return m_fHeart; }				//SÊuæ¾
	Param GetParam(void) { return m_param; }					//»Ýp[^æ¾
	void SetJump(const bool bJump) { m_bJump = bJump; }				//Wv»èÝè
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//Ú®ÊÝè
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }				//»ÝÊuÝè
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }				//ü«Ýè
	void SetHeartPos(const float fHeart) { m_fHeart = fHeart; }		//SÊuÝè
	void SetParam(const Param param) { m_param = param; }			//»Ýp[^Ýè
	void SetMember(const my_Identity::eMember member) { m_eMember = member; }		//®Ýè
	my_Identity::eMember GetMember() { return m_eMember; }		//®Ýè


	void ReadFile(void);	//t@CÇ
	void Damage(float fDamege = 1.0f);

	CMotion *GetMotion(void);					//»Ýp[^æ¾

protected:

private:
	void RotAdj(float fRotDiff);			//ü«C³
	void RotLimit(float *pRot);				//ü«C³

	void CollisionField(D3DXVECTOR3 pos);
	void CollisionBall(D3DXVECTOR3 pos);

	void Throw();

	CParts *m_apPart[MAX_PARTS];		//f(p[c)ÖÌ|C^
	int m_nNumModel;							//f(p[c)Ì
	D3DXVECTOR3 m_move;			//Ú®Ê
	D3DXVECTOR3 m_posOld;		//OñÊu
	D3DXVECTOR3 m_pos;			//»ÝÊu
	D3DXVECTOR3 m_size;			//»ÝTCY
	D3DXVECTOR3 m_rot;			//ü«
	D3DXVECTOR3 m_rotDest;		//ÚWÌü«
	D3DXMATRIX m_mtxWorld;		//[h}gbNX
	float m_fHeart;			//vC[ÌSy²(SÀW)
	bool m_bJump;			//Wv»è(falseÍWvÂ\óÔ)
	CMotion *m_pMotion;		//[Vîñ
	Param m_param;			//p[^
	CBall *m_pBall;		//í({[)îñ
	CState_Life *m_pStateLife;		//óÔ
	my_Identity::eMember m_eMember;		//®
	int m_nCatchCtr;		//Lb`JE^[
	int m_nThrowCtr;		//°JE^[
};

#endif