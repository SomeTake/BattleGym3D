//=============================================================================
//
// 当たり判定処理 [HitCheck.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool hitBC(D3DXVECTOR3 centerPosA, D3DXVECTOR3 centerPosB, float r1, float r2);
bool hitBB(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 size1, D3DXVECTOR3 size2);

#endif