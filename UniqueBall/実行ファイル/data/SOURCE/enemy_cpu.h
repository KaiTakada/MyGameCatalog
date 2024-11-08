//===============================================
//
// CPUG(enemy_cpu.h)
// Author ûüc ÀË
//
//===============================================
#ifndef _ENEMY_CPU_H_		//±Ì}Nè`ª³êÄÈ©Á½ç
#define _ENEMY_CPU_H_		//ñdCN[hh~Ì}Nðè`·é

//=========================
// CN[ht@C
//=========================
#include "enemy.h"
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
class CObjectCharacter;

//=========================
// CPUGNX
//=========================
class CEnemy_CPU : public CEnemy
{
public:

	//ANVñ^
	enum EAction
	{
		ACTION_NONE = 0,
		ACTION_CHASE,	// vC[ðÇ¢©¯é
		ACTION_THROW,	// °é
		ACTION_PICKUP,	// {[ðE¢És­
		ACTION_CAUTION,	// vC[ðxú·é(Lb`µæ¤Æ·é)
		ACTION_MAX,
	};

	CEnemy_CPU(int nPriority = 3);		//RXgN^
	~CEnemy_CPU();		//fXgN^

	HRESULT Init(void);							//ú»(ftH)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//ú»(Io)
	HRESULT Init(const D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), const D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), int nNumParts = 1);		//ú»(Io)
	void Uninit(void);							//I¹
	void Update(void);							//XV
	void Draw(void);							//`æ

	static CEnemy_CPU *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//¶¬

	void SetAction(const EAction eAction) { m_eAction = eAction; }		//ANVÝè
	EAction GetAction() { return m_eAction; }							//ANVæ¾

	void ActionSelect();	//ANVIð

protected:

private:
	void SerchTarget();		//ÅñèÌGðâ«

	EAction m_eAction;	//ANV

	CObjectCharacter* m_pTarget;	//^[QbgÌLN^[
};

#endif