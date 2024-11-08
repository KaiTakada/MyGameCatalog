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
class CCursor;
class CState_Life;

//=========================
// GNX
//=========================
class CEnemy : public CObjectCharacter
{
public:

	//====================
	// [VÌíÞ
	//====================
	enum MOTIONTYPE
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
	};

	CEnemy(int nPriority = 3);		//RXgN^
	~CEnemy();		//fXgN^

	HRESULT Init(void);							//ú»(ftH)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//ú»(Io)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//ú»(Io)
	void Uninit(void);							//I¹
	void Update(void);							//XV
	void Draw(void);							//`æ

	static CEnemy *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//¶¬

	void SetDelete(const bool bDelete) { m_bDelete = bDelete; }		//orgStOÝè
	bool GetDelete(void) { return m_bDelete; }						//orgStOæ¾

	void SetBall(CBall *pBall) { m_pBall = pBall; }		//{[Ýè
	CBall *GetBall(void) { return m_pBall; }			//{[æ¾

	void ReadFile(void);	//t@CÇ
	void InitModel();	//fúÝè

protected:

private:
	void CollisionBall(D3DXVECTOR3 pos);

	void Throw();

	CBall *m_pBall;		//í({[)îñ
	int m_nCatchCtr;		//Lb`JE^[
	int m_nThrowCtr;		//°JE^[
	bool m_bDelete;		//ñ¾çSceneÅE·ptO[t:ñ¾]
};

#endif