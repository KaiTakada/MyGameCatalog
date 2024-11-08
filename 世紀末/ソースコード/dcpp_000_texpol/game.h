//===============================================
//
// Q[(game.h)
// Author: ûüc ÀË
//
//===============================================
#ifndef _GAME_H_		//±Ì}Nè`ª³êÄÈ©Á½ç
#define _GAME_H_		//ñdCN[hh~Ì}Nðè`·é

//=========================
// CN[ht@C
//=========================
#include "main.h"
#include "manager.h"

//=========================
// Oûé¾
//=========================
//class CRenderer;
class CPause;
class CGrowSelecter;
class CTimer;
class CEnemy_Boss;
class CResult;

//=========================
// æÊ(V[)NX
//=========================
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	virtual HRESULT Init();		//ú»
	virtual void Uninit();		//I¹
	virtual void Update();		//XV
	virtual void Draw();		//`æ
	static CGame *Create();		//¶¬

	static HRESULT SetGrow(CGrowSelecter *pGrow);					//¬·V[Ýè

	static CScore *GetScore() { return m_pScore; }								//XRAÌæ¾
	static CPlayer *GetPlayer() { return m_pPlayer; }							//vC[Ìæ¾
	static CTimer *GetTimer() { return m_pTimer; }								//^C}[Ìæ¾
	static CGrowSelecter *GetGrow() { return m_pGrow; }							//¬·V[æ¾
	static CEnemy_Boss *GetBoss() { return m_pBoss; }								//{Xæ¾

	static void NullPause() { m_pPause = nullptr; }								//|[Y|C^ðnullÉ
	static void NullGrow() { m_pGrow = nullptr; }									//¬·V[|C^ðnullÉ

protected:

private:
	static CPlayer *m_pPlayer;					//vC[
	static CPause *m_pPause;					//|[Y
	static CGrowSelecter *m_pGrow;				//¬·i»V[
	static CScore *m_pScore;					//XRA
	static CTimer *m_pTimer;					//^C}[
	static CEnemy_Boss *m_pBoss;				//{X
	CField *m_pField;							//nÊ
	CSpawn *m_pSpawn;							//GX|[
	CResult *m_pResult;							//Ug
	bool m_bResult;								//Ug
};

#endif