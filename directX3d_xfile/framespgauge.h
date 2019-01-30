//=============================================================================
//
// SPゲージ用フレーム表示処理 [framespgauge.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _FRAMESPGAUGE_H_
#define _FRAMESPGAUGE_H_


// マクロ定義
#define TEXTURE_GAME_FRAMESPGAUGE00	_T("data/TEXTURE/framespgauge000.png")	// SPゲージのフレーム用画像
#define TEXTURE_FRAMESPGAUGE00_SIZE_X	(1280)								// テクスチャサイズ
#define TEXTURE_FRAMESPGAUGE00_SIZE_Y	(720)								// 同上

#define TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X	(1)							// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_Y	(1)							// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_FRAMESPGAUGE		(TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X*TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_FRAMESPGAUGE				(1)							// アニメーションの切り替わるカウント

#define FRAMESPGAUGE_MAX						(1)							//フレームの数

//フレームの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];							// 頂点情報格納ワーク
	D3DXVECTOR3				pos;											// ポリゴンの移動量
	int						CountAnim;										// アニメーションカウント
	int						PatternAnim;									// アニメーションパターンナンバー
	bool					use;											// フレームを表示するかどうかのフラグ
}FRAMESPGAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFrameSpgauge(int type);
void UninitFrameSpgauge(void);
void UpdateFrameSpgauge(void);
void DrawFrameSpgauge(void);
HRESULT MakeVertexFrameSpgauge(void);
void SetTextureFrameSpgauge(int cntPattern);
void SetVertexFrameSpgauge(void);

#endif