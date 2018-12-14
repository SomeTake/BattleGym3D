//=============================================================================
//
// SPゲージMAX表示処理 [spmax.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _SPMAX_H_
#define _SPMAX_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SPMAX		_T("data/TEXTURE/max000.png")			// 読み込むテクスチャファイル名
#define	SPMAX_POS_X			(180)									// プレイヤーのSPゲージMAX表示位置
#define	SPMAX_POS_Y			(660)									// プレイヤーのSPゲージMAX表示位置
#define	SPMAX_SIZE_X		(150)									// SPゲージMAXの幅
#define	SPMAX_SIZE_Y		(50)									// SPゲージMAXの高さ
#define	ESPMAX_POS_X		(950)									// プレイヤーのSPゲージMAX表示位置
#define	ESPMAX_POS_Y		(660)									// プレイヤーのSPゲージMAX表示位置

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSpmax(int type);
void UninitSpmax(void);
void UpdateSpmax(void);
void DrawSpmax(void);
HRESULT MakeVertexSpmax(void);
void SetTextureSpmax(void);
void SetVertexSpmax(void);

#endif
