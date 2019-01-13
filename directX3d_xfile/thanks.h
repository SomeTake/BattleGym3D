//=============================================================================
//
// 終了画面処理 [thanks.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _THANKS_H_
#define _THANKS_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_THANKS	_T("data/TEXTURE/thanks000.png")	// 読み込むテクスチャファイル名
#define	THANKS_SIZE_X	(640)								// 幅
#define	THANKS_SIZE_Y	(360)								// 高さ
#define	THANKS_POS_X	(SCREEN_WIDTH - THANKS_SIZE_X)		// 表示位置
#define	THANKS_POS_Y	(SCREEN_HEIGHT - THANKS_SIZE_Y)		// 表示位置

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitThanks(int type);
void UninitThanks(void);
void UpdateThanks(void);
void DrawThanks(void);
HRESULT MakeVertexThanks(void);
void SetReflectThanks(float per);	// 反射光の設定

#endif
