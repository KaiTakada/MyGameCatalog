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
//#include "objectX.h"
#include "object.h"

//=========================
// }Nè`
//=========================
#define MAX_PLAYER_PARTS (32)		//Ååp[c
#define MAX_PLAYER_SIZE D3DXVECTOR3(30.0f,50.0f,30.0f)		//TCY

//=========================
// Oûé¾
//=========================
class CParts;
class CMotion;
class CWeapon;
class CGauge;
class CCursor;
class CState_Life;

//=========================
// vC[NX
//=========================
class CPlayer : public CObject
{
public:

	//====================
	// [VÌíÞ
	//====================
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	//Ò@
		MOTIONTYPE_WALK,	//_bV
		MOTIONTYPE_DASH,	//è
		MOTIONTYPE_JUMP,	//Wv
		MOTIONTYPE_GROUND,	//n
		MOTIONTYPE_FIRE_REV,	//­Ë(REVOLVER)
		MOTIONTYPE_FIRE_AK,	//­Ë(AK)
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
		float fSpeedFire;	//AË¬x
		float fAttack;		//UÍ
		float fGetArea;		//ø«ñ¹»èÌ¼a
		float fColl;		//½è»èÌ¼a
	}Param;

	CPlayer(int nPriority = 3);		//RXgN^
	~CPlayer();		//fXgN^

	HRESULT Init(void);							//ú»(ftH)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//ú»(Io)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), int nNumParts = 1);		//ú»(Io)
	void Uninit(void);							//I¹
	void Update(void);							//XV
	void Draw(void);							//`æ

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//¶¬
	
	bool GetJump(void) { return m_bJump; }						//Wv»è
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }			//OñÊuæ¾
	D3DXVECTOR3 GetPos(void) { return m_pos; }					//»ÝÊuæ¾
	D3DXVECTOR3 GetRot(void) { return m_rot; }					//ü«æ¾
	D3DXVECTOR3 GetMove(void) { return m_move; }				//Ú®Êæ¾
	D3DXVECTOR3 GetSize(void) { return MAX_PLAYER_SIZE; }				//TCYæ¾
	float GetHeartPos(void) { return m_fHeart; }				//SÊuæ¾
	float GetExp(void) { return m_fExp; }						//»Ýo±læ¾
	Param GetParam(void) { return m_param; }					//»Ýp[^æ¾
	void SetJump(const bool bJump) { m_bJump = bJump; }				//Wv»èÝè
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }			//Ú®ÊÝè
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }				//»ÝÊuÝè
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }				//ü«Ýè
	void SetHeartPos(const float fHeart) { m_fHeart = fHeart; }		//SÊuÝè
	void SetExp(const float fExp);				//»Ýo±lÝè
	void CntExp(const float fExp);				//»Ýo±lÁZ
	void SetParam(const Param param) { m_param = param; }			//»Ýp[^Ýè
	void ReadFile(void);	//t@CÇ
	void Damage(float fDamege = 1.0f);

protected:

private:
	void MoveOperate(float *pfRotDest);		//Ú®ì
	void RotOperate(float *pfRotDest);		//ü«ì
	void RotAdj(float fRotDiff);			//ü«C³
	void PullItem(void);					//ACeø«ñ¹
	void RotLimit(float *pRot);				//ü«C³

	void DebugKey(CInputKeyboard *pInputKeyboard);		//fobOL[
	void CollisionField(D3DXVECTOR3 pos);

	CParts *m_apPart[MAX_PLAYER_PARTS];		//f(p[c)ÖÌ|C^
	int m_nNumModel;							//f(p[c)Ì
	D3DXVECTOR3 m_move;			//Ú®Ê
	D3DXVECTOR3 m_posOld;		//OñÊu
	D3DXVECTOR3 m_pos;			//»ÝÊu
	D3DXVECTOR3 m_rot;			//ü«
	D3DXMATRIX m_mtxWorld;		//[h}gbNX
	float m_fHeart;			//vC[ÌSy²(SÀW)
	bool m_bJump;			//Wv»è(falseÍWvÂ\óÔ)
	CMotion *m_pMotion;		//[Vîñ
	Param m_param;			//p[^
	CWeapon *m_pWeapon;		//íîñ
	CGauge *m_pGaugeHP;		//HPîñ
	CGauge *m_pGaugeExp;	//expîñ
	CCursor *m_pCursor;		//AimpJ[\
	float m_fExp;			//»Ýo±l
	float m_fExpMax;		//o±lQ[WMaxl
	CState_Life *m_pStateLife;		//óÔ
};

#endif
