//=============================================================================
//
// 入力モードセレクト画面処理 [inputselect.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _INPUTSELECT_H_
#define _INPUTSELECT_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_INPUTSELECT	_T("data/TEXTURE/input000.png")					// 読み込むテクスチャファイル名
#define	INPUTSELECT_SIZE_X	(640)											// 幅
#define	INPUTSELECT_SIZE_Y	(360)											// 高さ
#define	INPUTSELECT_POS_X	(SCREEN_WIDTH / 2 - INPUTSELECT_SIZE_X / 2)		// 表示位置
#define	INPUTSELECT_POS_Y	(SCREEN_HEIGHT / 2)								// 表示位置

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInputselect(int type);
void UninitInputselect(void);
void UpdateInputselect(void);
void DrawInputselect(void);
HRESULT MakeVertexInputselect(void);

#endif
