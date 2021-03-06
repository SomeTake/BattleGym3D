//=============================================================================
//
// 引き分け画面処理 [drawgame.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _DRAWGAME_H_
#define _DRAWGAME_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_DRAWGAME		_T("data/TEXTURE/drawgame000.png")		// 引き分け画面用の画像
#define	DRAWGAME_POS_X			(0)										// 引き分け画面の表示位置
#define	DRAWGAME_POS_Y			(0)										// 引き分け画面の表示位置
#define	DRAWGAME_SIZE_X		(1280)										// 引き分け画面の幅
#define	DRAWGAME_SIZE_Y		(720)										// 引き分け画面の高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDrawgame(int type);
void UninitDrawgame(void);
void UpdateDrawgame(void);
void DrawDrawgame(void);
HRESULT MakeVertexDrawgame(void);

#endif
