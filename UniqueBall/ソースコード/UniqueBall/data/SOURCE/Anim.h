//===============================================
//
// 2D|S(object2D.cpp)
// Author ûüc ÀË
//
//===============================================
#ifndef _ANIM_H_		//±Ì}Nè`ª³êÄÈ©Á½ç
#define _ANIM_H_		//ñdCN[hh~Ì}Nðè`·é

//=========================
// CN[ht@C
//=========================
#include "manager.h"

//=========================
// }Nè`
//=========================
#define MAX_VTX (4)		//¸_
#define DEF_SIZE_ANIM D3DXVECTOR2(125.0f,200.0f)				//ftHgTCY

//=========================
// IuWFNg2DNX
//=========================
class CAnim
{
public:
	CAnim();		//RXgN^
	~CAnim();		//fXgN^

	HRESULT Init(void);							//ú»(ftH)
	void Uninit(void);							//I¹
	void Update(void);							//XV

	void SetNumPtnXY(const int nNumPtnX, const int nNumPtnY) { SetNumPtnX(nNumPtnX),SetNumPtnY(nNumPtnY); }			//p^[Ýèxy

	void SetNumPtnX(const int nNumPtn) { m_nNumPtnAnimX = nNumPtn; }			//p^[Ýèx
	void SetNumPtnY(const int nNumPtn) { m_nNumPtnAnimY = nNumPtn; }			//p^[Ýèy
	void SetNumLoop(const int nNumLoop) { m_nLoop = nNumLoop; }			//p^[Ýè
	void SetNumFrame(const int nFrame) { m_nFrame = nFrame; }			//t[Ýè
	void SetTexpos(const D3DXVECTOR2 *pTexpos);					//eNX`ÀWÝè
	int GetNumPtnX(void) { return m_nNumPtnAnimX; }			//p^[æ¾x
	int GetNumPtnY(void) { return m_nNumPtnAnimY; }			//p^[æ¾y
	int GetNumLoop(void) { return m_nLoop; }			//p^[æ¾
	int GetNumFrame(void) { return m_nFrame; }			//t[æ¾
	D3DXVECTOR2* GetTexpos(void) { return m_aTexpos; }		//eNX`ÀWæ¾
	D3DXVECTOR2 GetTexpos(int nIdx) { return m_aTexpos[nIdx]; }		//eNX`ÀWæ¾
	bool IsEnd() { return m_bEnd; }			//I¹»èæ¾
	
protected:

private:
	int m_nCtrAnim;			//Aj[VJE^[
	int m_nPtnAnimX;		//Aj[Vp^[x »ÝNo.
	int m_nPtnAnimY;		//Aj[Vp^[y »ÝNo.
	int m_nNumPtnAnimX;		//Aj[Vp^[x
	int m_nNumPtnAnimY;		//Aj[Vp^[y
	int m_nFrame;			//XVpxt[
	int m_nLoop;			//[v(-1Å³À)
	bool m_bEnd;			//I¹tO

	D3DXVECTOR2 m_aTexpos[MAX_VTX];
};

#endif