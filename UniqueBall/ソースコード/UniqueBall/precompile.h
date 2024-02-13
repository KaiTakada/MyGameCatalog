//===============================================
//
// プリコンパイル(precompile.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _PRECOMPILE_H_		//このマクロ定義がされてなかったら
#define _PRECOMPILE_H_		//二重インクルード防止のマクロを定義する

#include <windows.h>
#include <stdio.h>
#include <assert.h>

//.メモリリーク出力用マクロ
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		//メモリリーク検出に必要

#include "useful.h"		//自作関数群

////=================================
//// マクロ定義
////=================================
//#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#else
//#define DBG_NEW new
//#endif

#endif
