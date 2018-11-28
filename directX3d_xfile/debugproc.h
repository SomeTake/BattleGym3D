//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);

void PrintDebugProc(const char *fmt,...);

#endif