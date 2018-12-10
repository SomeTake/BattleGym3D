//=============================================================================
//
// ウィナー表示処理 [winner.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _WINNER_H_
#define _WINNER_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_WINNER		_T("data/TEXTURE/win000.png")			// 読み込むテクスチャファイル名
#define	WINNER_POS_X		(750)									// ウィナーの表示位置
#define WINNER_POS_Y		(70)									// ウィナーの表示位置
#define	WINNER_SIZE_X		(200)									// ウィナーの幅
#define	WINNER_SIZE_Y		(300)									// ウィナーの高さ

#define TEXTURE_PATTERN_WINNER_X	(1)							// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_WINNER_Y	(2)							// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_WINNER		(TEXTURE_PATTERN_WINNER_X*TEXTURE_PATTERN_WINNER_Y)	// アニメーションパターン数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitWinner(int type);
void UninitWinner(void);
void UpdateWinner(void);
void DrawWinner(void);

#endif
