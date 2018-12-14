//=============================================================================
//
// SPゲージ用フレーム表示処理 [framespguage.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _FRAMESPGUAGE_H_
#define _FRAMESPGUAGE_H_


// マクロ定義
#define TEXTURE_GAME_FRAMESPGUAGE00	_T("data/TEXTURE/framespguage000.png")	// SPゲージのフレーム用画像
#define TEXTURE_FRAMESPGUAGE00_SIZE_X	(1280)								// テクスチャサイズ
#define TEXTURE_FRAMESPGUAGE00_SIZE_Y	(720)								// 同上

#define TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X	(1)							// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_Y	(1)							// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_FRAMESPGUAGE		(TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X*TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_FRAMESPGUAGE				(1)							// アニメーションの切り替わるカウント

#define FRAMESPGUAGE_MAX						(1)							//フレームの数

//フレームの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];							// 頂点情報格納ワーク
	D3DXVECTOR3				pos;											// ポリゴンの移動量
	int						CountAnim;										// アニメーションカウント
	int						PatternAnim;									// アニメーションパターンナンバー
	bool					use;											// フレームを表示するかどうかのフラグ
}FRAMESPGUAGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFrameSpguage(int type);
void UninitFrameSpguage(void);
void UpdateFrameSpguage(void);
void DrawFrameSpguage(void);
HRESULT MakeVertexFrameSpguage(void);
void SetTextureFrameSpguage(int cntPattern);
void SetVertexFrameSpguage(void);

#endif