//===============================================
//
// プリコンパイル(precompile.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _PRECOMPILE_H_		//このマクロ定義がされてなかったら
#define _PRECOMPILE_H_		//二重インクルード防止のマクロを定義する

#include <windows.h>
#include <string>
#include <stdio.h>
#include <time.h>
#include <string.h>

//input
#include <Xinput.h>
#define DIRECTINPUT_VERSION (0x0800)			//ビルド時の警告対処マクロ用
#include <dinput.h>

#include <assert.h>

//.メモリリーク出力用マクロ
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		//メモリリーク検出に必要

//inputライブラリのリンク
#pragma comment(lib,"xinput.lib")				//Gamepad入力処理に必要
#pragma comment(lib,"dinput8.lib")				//入力処理に必要

//自作
#include "useful.h"		//自作関数群

//名前空間指定(省略)
//using namespace std;	//名前空間ごと
//using std::begin;		//名前空間内の関数のみ

////=================================
//// マクロ定義
////=================================
//#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#else
//#define DBG_NEW new
//#endif

#endif
