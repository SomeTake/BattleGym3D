//=============================================================================
//
// リプレイ画面処理 [replay.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _REPLAY_H_
#define _REPLAY_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_REPLAY		_T("data/TEXTURE/replay000.png")		// 画像
#define	REPLAY_SIZE_X		(800)									// 幅
#define	REPLAY_SIZE_Y		(500)									// 高さ
#define	REPLAY_POS_X		(SCREEN_WIDTH / 2 - REPLAY_SIZE_X / 2)	// 表示位置
#define	REPLAY_POS_Y		(SCREEN_HEIGHT / 2 - REPLAY_SIZE_Y / 2)	// 表示位置

#define FLASHING_TIME		(20)									// 点滅するフレーム数
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitReplay(int type);
void UninitReplay(void);
void UpdateReplay(void);
void DrawReplay(void);
HRESULT MakeVertexReplay(void);
void SetReflectReplay(float per);

#endif
