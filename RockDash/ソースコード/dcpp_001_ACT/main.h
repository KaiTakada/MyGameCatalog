//===============================================
//
// ポリゴン描画(main.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _MAIN_H_								//このマクロ定義がされてなかったら
#define _MAIN_H_								//二重インクルード防止のマクロを定義する

#include "d3dx9.h"								//描画処理に必要
#include "xaudio2.h"							//サウンド処理に必要

//=========================
// ライブラリのリンク
//=========================
#pragma comment(lib,"d3d9.lib")					//描画処理に必要
#pragma comment(lib,"d3dx9.lib")				//描画処理の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")				//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")				//システム時刻取得に必要

//=========================
// マクロ定義
//=========================
#define SCREEN_WIDTH (1280)						//ウィンドウの幅
#define SCREEN_HEIGHT (720)						//ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define WINDOW_NAME "RockDash"					//ウィンドウの名前(キャプションに表示)
#define TIMER_FPS (60)							//平均FPS

//=================================
// マクロ定義
//=================================
#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )		//ポインタの解放忘れ検知
#else
#define DBG_NEW new
#endif

//====================================
// 頂点情報[2D]の構造体を定義
//====================================
typedef struct
{
	D3DXVECTOR3 pos;							//座標
	float rhw;									//座標変換用係数
	D3DCOLOR col;								//色
	D3DXVECTOR2 tex;							//テクスチャ座標
}VERTEX_2D;

//====================================
// 頂点情報[3D]の構造体を定義
//====================================
typedef struct
{
	D3DXVECTOR3 pos;							//座標
	D3DXVECTOR3 nor;							//法線ベクトル
	D3DCOLOR col;								//色
	D3DXVECTOR2 tex;							//テクスチャ座標
}VERTEX_3D;

//=========================
// 前方宣言
//=========================

//=========================
// プロトタイプ宣言
//=========================
int GetFPS(void);		//FPS取得

#endif