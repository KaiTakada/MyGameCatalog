//===============================================
//
// enter£΅(press_enter.cpp)
// Author ϋόc ΐΛ
//
//===============================================
#ifndef _PRESS_ENTER_H_		//±Μ}Nθ`ͺ³κΔΘ©Α½η
#define _PRESS_ENTER_H_		//ρdCN[hh~Μ}Nπθ`·ι

//=========================
// CN[ht@C
//=========================
#include "manager.h"
#include "obj_polygon2D.h"

//=========================
// }Nθ`
//=========================
#define DEF_SIZE_ENTER D3DXVECTOR3(90.0f * 1.5f, 90.0f, 0.0f)				//ftHgTCY

//=========================
// OϋιΎ
//=========================

//=========================
// IuWFNg2DNX
//=========================
class CPress_Enter : public CObject2D
{
public:

	//=========================
	// ^Cvρ^
	//=========================
	enum UPDATE_TYPE
	{
		UPDATE_TYPE_NONE = 0,
		UPDATE_TYPE_ANIM,
		UPDATE_TYPE_MAX
	};

	CPress_Enter(int nPriority = CObject2D::PRIORITY_UI);		//RXgN^
	~CPress_Enter();		//fXgN^

	HRESULT Init(void);							//ϊ»(ftH)
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//ϊ»(Io)
	void Uninit(void);							//IΉ
	void Update(void);							//XV
	void Draw(void);							//`ζ

	static CPress_Enter *Create(void);		//Ά¬
	static CPress_Enter *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size = DEF_SIZE_ENTER, const D3DXVECTOR3 rot = DEF_VEC3);		//Ά¬

	void SetInterval(int nCtr) { m_nTimeInterval = nCtr; }
	int SetInterval() { return m_nTimeInterval; }

protected:

private:
	int m_nAlphaCnter;						//PRESS ENTERΜ_ΕΤuΜ»θ
	int m_nTimeInterval;					//PRESS ENTERΜ_ΕΤu
	float m_fAlphaColor;					//Έ_J[a
};

#endif