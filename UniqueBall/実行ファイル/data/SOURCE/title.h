//===============================================
//
// ^Cg(title.h)
// Author: ϋόc ΐΛ
//
//===============================================
#ifndef _TITLE_H_		//±Μ}Nθ`ͺ³κΔΘ©Α½η
#define _TITLE_H_		//ρdCN[hh~Μ}Nπθ`·ι

//=========================
// CN[ht@C
//=========================
#include "main.h"
#include "scene.h"

//=========================
// OϋιΎ
//=========================
class CPress_Enter;

//=========================
// ζΚ(V[)NX
//=========================
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	virtual HRESULT Init();		//ϊ»
	virtual void Uninit();		//IΉ
	virtual void Update();		//XV
	virtual void Draw();		//`ζ
	static CTitle *Create();		//Ά¬

protected:

private:
	CField *m_pField;							//nΚ

	CPress_Enter *m_pEnter;	//press enter
	int m_nAlphaCnter;						//PRESS ENTERΜ_ΕΤuΜ»θ
	int m_nTimeInterval;						//PRESS ENTERΜ_ΕΤu
	float m_fAlphaColor;					//Έ_J[
	int m_nCntRanking;						//LOJΪ
};

#endif