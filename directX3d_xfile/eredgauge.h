//=============================================================================
//
// 2P用赤ゲージ画面処理 [eredgauge.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _EREDGAUGE_H_
#define _EREDGAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EREDGAUGE			_T("data/TEXTURE/ehpgauge000.png")		// 読み込むテクスチャファイル名
#define	EREDGAUGE_POS_X				(680)									// ゲージの表示位置
#define	EREDGAUGE_POS_Y				(50)									// ゲージの表示位置
#define	EREDGAUGE_SIZE_X			(500)									// ゲージの幅
#define	EREDGAUGE_SIZE_Y			(50)									// ゲージの高さ
#define EREDGAUGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define EREDGAUGE_PATTERN_DIVIDE_Y	(3)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_EREDGAUGE	(EREDGAUGE_PATTERN_DIVIDE_X*EREDGAUGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_EREDGAUGE	(4)										// アニメーションの切り替わるカウント
#define EREDGAUGE_MAX				(1)										// ゲージの個数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitERedGauge(int type);
void UninitERedGauge(void);
void UpdateERedGauge(void);
void DrawERedGauge(void);
HRESULT MakeVertexERedGauge(void);
void SetTextureERedGauge(int cntPattern);
void SetVertexERedGauge(void);
REDGAUGE *GetERedGauge(void);		// EREDGAUGE構造体のゲッター

#endif
