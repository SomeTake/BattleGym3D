//=============================================================================
//
// 1P2P表示処理 [pop.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _POP_H_
#define _POP_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_POP1P		"data/TEXTURE/pointer001.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_POP2P		"data/TEXTURE/pointer002.png"	// 読み込むテクスチャファイル名
#define	POP_WIDTH			(20.0f)							// 半径高さ
#define	POP_HEIGHT			(20.0f)							// 半径幅
#define POP_POS				D3DXVECTOR3(0.0f, 75.0f, 0.0f)	// 表示場所

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9		D3DTexture = {};		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;		// 頂点バッファインターフェースへのポインタ
	D3DXMATRIX				mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3				pos;					// 位置
	D3DXVECTOR3				scl;					// スケール
	float					width;					// 幅
	float					height;					// 高さ
} POP;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPop(int type, POP *PopWk, int ControllerNum);
void UninitPop(POP *PopWk);
void UpdatePop(POP *PopWk, D3DXVECTOR3 pos);
void DrawPop(POP *PopWk);
int SetPop(POP *PopWk, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col);

#endif
