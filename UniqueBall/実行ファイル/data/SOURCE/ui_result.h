//===============================================
//
// UgUI(ui_result.cpp)
// Author : ûüc ÀË
//
//===============================================
#ifndef _UI_RESULT_H_		//±Ì}Nè`ª³êÄÈ©Á½ç
#define _UI_RESULT_H_		//ñdCN[hh~Ì}Nðè`·é

//=========================
// CN[ht@C
//=========================
#include "manager.h"
#include "object.h"

//=========================
// }Nè`
//=========================

//=========================
// Oûé¾
//=========================
class CScore;
class CObject2D;

//=========================
// IuWFNg2DwiNX
//=========================
class CUI_Result : public CObject
{
public:
	CUI_Result();		//RXgN^
	~CUI_Result();		//fXgN^

	HRESULT Init(void);							//ú»(ftH)
	void Uninit(void);							//I¹
	void Update(void);							//XV
	void Draw(void);							//`æ

	static CUI_Result *Create(void);		//¶¬

	void SetMark(CObject2D* pMark, int nIdx) { m_apFrame[nIdx] = pMark; }			//ÊuÝè
	CObject2D **GetMark() { return m_apFrame; }		//Êuæ¾
	CObject2D *GetMark(int nIdx) { return m_apFrame[nIdx]; }		//Êuæ¾

	bool GetTrans() { return m_bTrans; }	//JÚtOæ¾

protected:

private:
	CObject2D *m_apFrame[mylib_const::MAX_PLAYER];		//XRAîñ
	bool m_bOK[mylib_const::MAX_PLAYER];	//JÚ¯ÓtO
	bool m_bTrans;	//JÚtO
};

#endif