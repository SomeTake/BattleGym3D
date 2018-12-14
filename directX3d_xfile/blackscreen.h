//=============================================================================
//
// 黒画面処理 [blackscreen.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _BLACKSCREEN_H_
#define _BLACKSCREEN_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_BLACKSCREEN	_T("data/TEXTURE/blackscreen000.png")		// 読み込むテクスチャファイル名
#define	BLACKSCREEN_POS_X	(0)								// タイトルロゴの表示位置
#define	BLACKSCREEN_POS_Y	(0)								// タイトルロゴの表示位置
#define	BLACKSCREEN_SIZE_X	(1280)							// タイトルロゴの幅
#define	BLACKSCREEN_SIZE_Y	(720)							// タイトルロゴの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBlackscreen(int type);
void UninitBlackscreen(void);
void UpdateBlackscreen(void);
void DrawBlackscreen(void);
HRESULT MakeVertexBlackscreen(void);
void SetReflectBlackscreen(float per);		// 透明度変更用

#endif
