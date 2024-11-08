//===============================================
//
// |S`ζ(main.h)
// Author ϋόc ΐΛ
//
//===============================================
#ifndef _MAIN_H_								//±Μ}Nθ`ͺ³κΔΘ©Α½η
#define _MAIN_H_								//ρdCN[hh~Μ}Nπθ`·ι

#include "d3dx9.h"								//`ζΙKv
#include "xaudio2.h"							//TEhΙKv

//=========================
// CuΜN
//=========================
#pragma comment(lib,"d3d9.lib")					//`ζΙKv
#pragma comment(lib,"d3dx9.lib")				//`ζΜg£Cu
#pragma comment(lib,"dxguid.lib")				//DirectXR|[lg(i)gpΙKv
#pragma comment(lib,"winmm.lib")				//VXeζΎΙKv

//=========================
// }Nθ`
//=========================
#define SCREEN_WIDTH (1280)						//EBhEΜ
#define SCREEN_HEIGHT (720)						//EBhEΜ³
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define WINDOW_NAME "’I"					//EBhEΜΌO(LvVΙ\¦)

//=================================
// }Nθ`
//=================================
#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )		//|C^ΜπϊYκm
#else
#define DBG_NEW new
#endif

//====================================
// Έ_ξρ[2D]Μ\’Μπθ`
//====================================
typedef struct
{
	D3DXVECTOR3 pos;							//ΐW
	float rhw;									//ΐWΟ·pW
	D3DCOLOR col;								//F
	D3DXVECTOR2 tex;							//eNX`ΐW
}VERTEX_2D;

//====================================
// Έ_ξρ[3D]Μ\’Μπθ`
//====================================
typedef struct
{
	D3DXVECTOR3 pos;							//ΐW
	D3DXVECTOR3 nor;							//@όxNg
	D3DCOLOR col;								//F
	D3DXVECTOR2 tex;							//eNX`ΐW
}VERTEX_3D;

//=========================
// OϋιΎ
//=========================

//=========================
// vg^CvιΎ
//=========================
int GetFPS(void);		//FPSζΎ

#endif