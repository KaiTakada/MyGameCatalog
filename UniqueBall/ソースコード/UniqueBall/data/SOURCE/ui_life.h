//===============================================
//
// ÌÍUI(ui_life.cpp)
// Author ûüc ÀË
//
//===============================================
#ifndef _UI_LIFE_H_		//±Ì}Nè`ª³êÄÈ©Á½ç
#define _UI_LIFE_H_		//ñdCN[hh~Ì}Nðè`·é

//=========================
// CN[ht@C
//=========================
#include "manager.h"
#include "object.h"

//=========================
// }Nè`
//=========================
namespace myUI_life
{
	const int MAX_NUM_LIFE = 10;	//zñÅå
}

//=========================
// Oûé¾
//=========================
class CScore;
class CObject2D;

//=========================
// IuWFNg2DwiNX
//=========================
class CUI_Life : public CObject
{
public:
	CUI_Life();		//RXgN^
	~CUI_Life();		//fXgN^

	HRESULT Init(void);							//ú»(ftH)
	void Uninit(void);							//I¹
	void Update(void);							//XV
	void Draw(void);							//`æ

	static CUI_Life *Create(void);		//¶¬
	static CUI_Life *Create(const int nNumMark, const int nIdx);		//¶¬

	void SetPos(const D3DXVECTOR3 pos) override;			//ÊuÝè
	D3DXVECTOR3 GetPos() { return m_pos; }		//Êuæ¾

	void SetNumMark(const int nNumMark) { m_nNumMark = nNumMark; }			//ÊuÝè
	int GetNumMark() { return m_nNumMark; }		//Êuæ¾

	void SetIdx(const int nIdx) { m_nIdx = nIdx; }			//ÔÝè
	int GetIdx() { return m_nIdx; }		//Ôæ¾

	void SetMark(CObject2D* pMark, int nIdx) { m_apMark[nIdx] = pMark; }			//ÊuÝè
	CObject2D **GetMark() { return m_apMark; }		//Êuæ¾
	CObject2D* GetMark(int nIdx) { return m_apMark[nIdx]; }		//Êuæ¾

	void SetColAll(D3DXCOLOR col);		//SÄÌFÏ¦

	void AddLife(const int nIdx);			//Á
	void SubLife(const int nIdx);			//¸­

protected:

private:
	CObject2D *m_apMark[myUI_life::MAX_NUM_LIFE];		//XRAîñ
	D3DXVECTOR3 m_pos;	//Êu
	int m_nNumMark;	//}[N
	int m_nIdx;	//féJÔ
};

#endif