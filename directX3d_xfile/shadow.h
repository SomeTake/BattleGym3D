//=============================================================================
//
// 影処理 [shadow.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define	SHADOW_SIZE_X		(50.0f)							// 影の幅
#define	SHADOW_SIZE_Z		(50.0f)							// 影の高さ

#define	MAX_SHADOW			(128)							// 影最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	bool bUse;				// 使用しているかどうか
} SHADOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(int type);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);		// 影の生成処理
void ReleaseShadow(int nIdxShadow);									// 影の消去処理
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);			// 影を作る場所を指定
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);	// 影のサイズを指定
void SetColorShadow(int nIdxShadow, D3DXCOLOR col);					// 影の色を指定

#endif
