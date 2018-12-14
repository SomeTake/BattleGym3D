//=============================================================================
//
// 地面処理 [field.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#define	TEXTURE_FIELD	"data/TEXTURE/field000.jpg"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

#endif
