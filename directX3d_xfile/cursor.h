//=============================================================================
//
// カーソル処理 [cursor.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _CURSOR_H_
#define _CURSOR_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_CURSOR1P	_T("data/TEXTURE/cursor001.png")			// 読み込むテクスチャファイル名
#define TEXTURE_CURSOR2P	_T("data/TEXTURE/cursor002.png")			// 読み込むテクスチャファイル名
#define	CURSOR_SIZE_X		(320)										// 幅
#define	CURSOR_SIZE_Y		(360)										// 高さ
#define	CURSOR_POS_X_A		(SCREEN_WIDTH / 2 - CURSOR_SIZE_X + 17)		// 表示位置
#define CURSOR_POS_X_B		(SCREEN_WIDTH / 2 - 11)
#define	CURSOR_POS_Y		(SCREEN_HEIGHT / 2 - 10)					// 表示位置

#define CURSOR_MAX			(2)											// カーソルの数=プレイヤー数

typedef struct
{
	D3DXVECTOR3				pos;					// 表示位置
	LPDIRECT3DTEXTURE9		D3DTexture = NULL;		// テクスチャへのポインタ
	VERTEX_2D				vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	bool					modeinput;				// 入力モードの選択がOKかどうか
}CURSOR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCursor(int type);
void UninitCursor(void);
void UpdateCursor(void);
void DrawCursor(void);
HRESULT MakeVertexCursor(int num);
void SetVertexCursor(int num);
CURSOR *GetCursor(int num);

#endif
