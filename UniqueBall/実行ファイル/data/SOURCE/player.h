//===============================================
//
// vC[(player.h)
// Author ûüc ÀË
//
//===============================================
#ifndef _PLAYER_H_		//±Ì}Nè`ª³êÄÈ©Á½ç
#define _PLAYER_H_		//ñdCN[hh~Ì}Nðè`·é

//=========================
// CN[ht@C
//=========================
#include "objectcharacter.h"
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
class CState_Life;
class CUI_Life;
class CTarget_Mark;

//=========================
// vC[NX
//=========================
class CPlayer : public CObjectCharacter
{
public:

	//====================
	// [VÌíÞ
	//====================
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		//Ò@
		MOTIONTYPE_WALK,			//à«
		MOTIONTYPE_DASH,			//_bV
		MOTIONTYPE_JUMP,			//Wv
		MOTIONTYPE_JUMP2,			//2iWv
		MOTIONTYPE_GROUND,			//n
		MOTIONTYPE_THROW_RDY,		//°p\¦é
		MOTIONTYPE_THROW,			//°é
		MOTIONTYPE_CATCH_RDY,		//Lb`p\¦é
		MOTIONTYPE_CATCH,			//Lb`
		MOTIONTYPE_TACKLE,			//^bN
		MOTIONTYPE_DAMAGE,			//_[W
		MOTIONTYPE_NEUTRAL_BALL,	//Ò@(¿)
		MOTIONTYPE_DASH_BALL,		//_bV(¿)
		MOTIONTYPE_JUMP_BALL,		//Wv(¿)
		MOTIONTYPE_MAX,
	};

	//====================
	// {^Ç
	//====================
	enum CONTROL_MANAGE
	{
		CONTROL_FRONT= 0,
		CONTROL_BACK,
		CONTROL_RIGHT,
		CONTROL_LEFT,
		CONTROL_JUMP,
		CONTROL_TACKLE,
		CONTROL_THROW,
		CONTROL_THROW_RELEASE,
		CONTROL_CATCH,
		CONTROL_TARGET,
		CONTROL_MAX,
	};

	CPlayer(int nPriority = 3);		//RXgN^
	~CPlayer();		//fXgN^

	HRESULT Init(void);							//ú»(ftH)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//ú»(Io)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//ú»(Io)
	void Uninit(void);							//I¹
	void Update(void);							//XV
	void Draw(void);							//`æ

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//¶¬
	
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//OñÊuæ¾
	D3DXVECTOR3 GetSize(void) { return m_size; }				//TCYæ¾
	CMotion* GetMotion(void);					//[Vîñæ¾
	
	static int GetControlPlayer() { return m_nControlIdx; }					//ìvC[

	void SetIdx(int nIdx) { m_nIdx = nIdx; }		//ÔÝè
	int GetIdx(void) { return m_nIdx; }				//Ôæ¾
	void SetUI_Life(CUI_Life* pLifeUI) { m_pLifeUI = pLifeUI; }		//ÌÍUIÝè
	CUI_Life *GetUI_Life(void) { return m_pLifeUI; }				//ÌÍUIæ¾
	
	void ReadFile(void);	//t@CÇ
	void InitUI();	//UIú»
	void InitModel();	//fú»
	void Damage(int nDamege = 1);
	void Knockback(float fRot);		//mbNobN

protected:
	void MotionSet(int type, int brend = 0) override;

private:
	void MoveOperate(float *pfRotDest);		//Ú®ì
	void RotOperate(float *pfRotDest);		//ü«ì

	void CollisionWall();
	void CollisionBall(D3DXVECTOR3 pos);
	void TackleCollision();
	void MotionEffect();
	void ReduceCounter();
	void ButtonAction();	//{^ANV
	void Debug();		//fobOL[
	void RankOK();
	bool ControllManage(CONTROL_MANAGE key);

	void Throw();	//{[°
	void Target();	//^[QeBO
	void SetNeutral();	//j[góÔÉ·é

	CBall *m_pBall;		//í({[)îñ
	CUI_Life *m_pLifeUI;		//ÌÍUI
	int m_nCatchCtr;		//Lb`JE^[
	int m_nTackleCtr;		//^bNJE^[
	float m_fThrowChargeCtr;		//­ß°JE^[
	int m_nJump2Ctr;		//2iWvJE^[
	int m_nIdx;		//©gÌÔ
	bool m_bTarget;		//^[QbgtO[t:ñ¾]
	CTarget_Mark *m_pTargetMark;		//^[Qbg}[N

	static int m_nControlIdx;		//L[{[hÅì·évC[
};

#endif
