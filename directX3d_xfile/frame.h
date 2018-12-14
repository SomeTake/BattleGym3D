//=============================================================================
//
// バトル画面フレーム表示処理 [frame.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _FRAME_H_
#define _FRAME_H_

// マクロ定義
#define TEXTURE_GAME_FRAME00	_T("data/TEXTURE/frame000.png")	// フレーム用画像
#define TEXTURE_FRAME00_SIZE_X	(1280)							// テクスチャサイズ
#define TEXTURE_FRAME00_SIZE_Y	(720)							// 同上

#define TEXTURE_PATTERN_DIVIDE_FRAME_X	(1)						// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_FRAME_Y	(1)						// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_FRAME		(TEXTURE_PATTERN_DIVIDE_FRAME_X*TEXTURE_PATTERN_DIVIDE_FRAME_Y)	// アニメーションパターン数
#define TIME_ANIMATION_FRAME		(1)							// アニメーションの切り替わるカウント

#define FRAME_MAX					(1)							//フレームの数

#define SWAY_MAX					(20)						//揺らすフレーム数

//フレームの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
	D3DXVECTOR3				pos;								// ポリゴンの移動量
	int						CountAnim;							// アニメーションカウント
	int						PatternAnim;						// アニメーションパターンナンバー
	bool					use;								// フレームを表示するかどうかのフラグ
	bool					sway;								// フレームを揺らすためのフラグ
}FRAME;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFrame(int type);
void UninitFrame(void);
void UpdateFrame(void);
void DrawFrame(void);
void FrameSway(void);					// フレームを揺らすためのフラグをONにする関数
HRESULT MakeVertexFrame(void);
void SetTextureFrame(int cntPattern);
void SetVertexFrame(void);

#endif