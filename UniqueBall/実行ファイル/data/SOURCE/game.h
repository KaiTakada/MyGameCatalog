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
#include "scene.h"

//=========================
// èè`
//=========================
namespace mygame
{
	const int NUMPLAYER = 2;	//vC[ÌÅèl
	const int NUM_WALL = 4;		//ÇÌÅè
}

//=========================
// Oûé¾
//=========================
//class CRenderer;
class CPause;
class CGrowSelecter;
class CTimer;
class CMap;
class CCameraPlayer;
class CWall;
class CGameManager;
class CObjMeshField;

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

	static CPause *GetPause() { return m_pPause; }								//|[YÌæ¾
	static CTimer *GetTimer() { return m_pTimer; }								//^C}[Ìæ¾
	static CMap *GetMap() { return m_pMap; }								//}bvÌæ¾

	static void NullPause() { m_pPause = nullptr; }								//|[Y|C^ðnullÉ
	static bool MapFlag() { return m_pMap != nullptr; }								//|[Y|C^ðnullÉ
	CCameraPlayer *GetCamera(int nIdx) { return m_ppCamera[nIdx]; }					//ªJÌ1Âðæ¾
	CCameraPlayer **GetCamera() { return m_ppCamera; }								//ªJÌæ¾
	CObjMeshField* GetField() { return m_pField; }						//°ðæ¾
	CWall *GetWall(int nIdx) { return m_ppWall[nIdx]; }					//ÇÌ1Âðæ¾
	CWall **GetWall() { return m_ppWall; }								//ÇÌæ¾

protected:

private:
	static CPause *m_pPause;					//|[Y
	static CTimer *m_pTimer;					//^C}[
	static CMap *m_pMap;						//}bv
	CObjMeshField *m_pField;							//nÊ
	CWall *m_ppWall[mygame::NUM_WALL];							//nÊ
	CEnemy *m_pBot;								//°{bg
	CCameraPlayer *m_ppCamera[mylib_const::MAX_PLAYER];		//vC[Ç]J
	CGameManager* m_pGameManager;	//Q[}l[W[
};

#endif