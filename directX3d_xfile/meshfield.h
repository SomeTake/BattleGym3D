//=============================================================================
//
// メッシュ地面の処理 [meshfield.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

#define	TEXTURE_MESHFIELD	"data/TEXTURE/mokume.jpg"		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockZ, float nBlockSizeX, float nBlockSizeZ, int type);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif
