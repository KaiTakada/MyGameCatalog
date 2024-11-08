//===============================================
//
// |S(object.cpp)
// Author ûüc ÀË
//
//===============================================
#ifndef _OBJECT_H_								//±Ì}Nè`ª³êÄÈ©Á½ç
#define _OBJECT_H_								//ñdCN[hh~Ì}Nðè`·é

//=========================
// CN[ht@C
//=========================
#include "manager.h"

//=========================
// }Nè`
//=========================
#define MAX_OBJECT (1024)		//Åå
#define DEF_VEC3 (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//3²Ìúl

//=========================
// IuWFNgNX
//=========================
class CObject
{
public:

	//========================
	// IuWFNg^Cv
	//========================
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BULLET,
		TYPE_EXPLOSION,
		TYPE_BG,
		TYPE_MULTIBG,
		TYPE_EFFECT,
		TYPE_PARTICLE,
		TYPE_NUMBER,
		TYPE_SCORE,
		TYPE_BLOCK,
		TYPE_ITEM,
		TYPE_3DPOLYGON,
		TYPE_BILLBOARD,
		TYPE_MODEL,
		TYPE_FIELD,
		TYPE_WEAPON,
		TYPE_GAUGE,
		TYPE_PARTS,
		TYPE_EXP,
		TYPE_FADE,
		TYPE_TIMER,
		TYPE_EVO,
		TYPE_ITEM2D,
		TYPE_SPAWN,
		TYPE_BOSS,
		TYPE_MAX
	}TYPE;

	//========================
	// DæÊ
	//========================
	typedef enum
	{
		PRIORITY_BG = 0,
		PRIORITY_MAP,
		PRIORITY_OBJ,
		PRIORITY_MOB,
		PRIORITY_MAIN,
		PRIORITY_UI,
		PRIORITY_PAUSE,
		PRIORITY_FADE,
		PRIORITY_MAX,
	}PRIORITY;

	//CObject();				//RXgN^(ftH)
	CObject(int nPriority = 3);				//RXgN^(`æIo)
	virtual ~CObject();		//fXgN^

	virtual HRESULT Init(void) = 0;							//ú»(ftH)
	virtual HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot) { pos; rot; return E_FAIL; }		//ú»(Io)
	virtual void Uninit(void) = 0;							//I¹
	virtual void Update(void) = 0;							//XV
	virtual void Draw(void) = 0;							//`æ
	
	virtual void SetPos(const D3DXVECTOR3 pos) { pos; }			//ÊuÝè
	virtual void SetRot(const D3DXVECTOR3 rot) { rot; }			//ü«Ýè
	virtual void SetJump(const bool bJump) { bJump; }		//Wv»èÝè
	virtual void SetMove(const D3DXVECTOR3 move) { move; }		//Ú®ÊÝè
	virtual D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//»ÝÊuæ¾
	virtual D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//ü«æ¾
	virtual D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//OñÊuæ¾
	virtual D3DXVECTOR3 GetSize(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//2DTCYæ¾
	virtual bool GetJump(void) { return FALSE; }		//Wv»èæ¾
	virtual D3DXVECTOR3 GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//Ú®Êæ¾
	virtual void GetPolyPos(D3DXVECTOR3 Vtx0, D3DXVECTOR3 Vtx1, D3DXVECTOR3 Vtx2, D3DXVECTOR3 Vtx3) { Vtx0, Vtx1, Vtx2, Vtx3; }		//Ú®Êæ¾
	virtual D3DXVECTOR3 GetMinVtx(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//Å¬ÀWæ¾
	virtual D3DXVECTOR3 GetMaxVtx(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			//ÅåÀWæ¾

	static void Reset(void);		//^CvÅjü
	static void ReleaseAll(void);		//SÄÌjü
	static void UpdateAll(void);		//SÄÌXV
	static void DrawAll(void);			//SÄÌ`æ
	static int GetNumAll(void) { return m_nNumAll; }		//æ¾
	void SetPrity(int nPriority);		//`æÝè
	int GetID(void) { return m_nID; }		//æ¾

	void SetType(CObject::TYPE type) { m_type = type; }		//^Cvæ¾
	CObject::TYPE GetType(void) { return m_type; }		//^Cvæ¾
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }		//StOæ¾
	bool GetDeath(void) { return m_bDeath; }				//StOæ¾

	static CObject *GetObject(const int nPriority, const int nIdx);		//wèIuWFNgæ¾

protected:
	void Release(void);		//©gÌjü

private:
	static CObject *m_apObject[PRIORITY_MAX][MAX_OBJECT];		//IuWFNgCX^X
	static int m_nNumAll;		//IuWFNg
	int m_nID;					//©ª©gÌID
	CObject::TYPE m_type;					//^Cv
	int m_nPriority;	//`æ
	bool m_bDeath;		//StO
};

#endif