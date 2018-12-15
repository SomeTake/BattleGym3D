//=============================================================================
//
// メッシュ壁の処理 [meshwall.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

#define	MAX_MESHWALL		(10)							// 壁の総数
#define	TEXTURE_MESHWALL	"data/TEXTURE/wall000.jpg"		// 読み込むテクスチャファイル名
#define	VALUE_MOVE_MESHWALL		(5.0f)							// 移動速度
#define	VALUE_ROTATE_MESHWALL	(D3DX_PI * 0.001f)				// 回転速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
