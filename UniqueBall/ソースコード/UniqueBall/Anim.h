//===============================================
//
// 2D|S(object2D.cpp)
// Author ûüc ĀË
//
//===============================================
#ifndef _ANIM_H_		//ąĖ}Nč`ĒŗęÄČŠÁŊį
#define _ANIM_H_		//ņdCN[hh~Ė}Nđč`ˇé

//=========================
// CN[ht@C
//=========================
#include "manager.h"

//=========================
// }Nč`
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

	HRESULT Init(void);							//úģ(ftH)
	void Uninit(void);							//Iš
	void Update(void);							//XV

	void SetNumPtnX(const int nNumPtn) { m_nNumPtnAnimX = nNumPtn; }			//p^[Ũčx
	void SetNumPtnY(const int nNumPtn) { m_nNumPtnAnimY = nNumPtn; }			//p^[Ũčy
	void SetNumLoop(const int nNumLoop) { m_nLoop = nNumLoop; }			//p^[Ũč
	void SetNumFrame(const int nFrame) { m_nFrame = nFrame; }			//t[Ũč
	void SetTexpos(const D3DXVECTOR2 *pTexpos);					//eNX`ĀWŨč
	int GetNumPtnX(void) { return m_nNumPtnAnimX; }			//p^[æžx
	int GetNumPtnY(void) { return m_nNumPtnAnimY; }			//p^[æžy
	int GetNumLoop(void) { return m_nLoop; }			//p^[æž
	int GetNumFrame(void) { return m_nFrame; }			//t[æž
	D3DXVECTOR2 *GetTexpos(void) { return &m_aTexpos[0]; }		//eNX`ĀWæž
	bool IsEnd() { return m_bEnd; }			//Išģčæž
	
protected:

private:
	int m_nCtrAnim;							//Aj[VJE^[
	int m_nPtnAnimX;						//Aj[Vp^[x No.
	int m_nPtnAnimY;						//Aj[Vp^[y No.
	int m_nNumPtnAnimX;						//Aj[Vp^[x
	int m_nNumPtnAnimY;						//Aj[Vp^[y
	int m_nFrame;		//XVpxt[
	int m_nLoop;		//[v(-1ÅŗĀ)
	bool m_bEnd;			//IštO

	D3DXVECTOR2 m_aTexpos[MAX_VTX];
};

#endif