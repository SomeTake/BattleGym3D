//=============================================================================
//
// ノックアウト表示処理 [knockout.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _KNOCKOUT_H_
#define _KNOCKOUT_H_


// マクロ定義
#define TEXTURE_GAME_KNOCKOUT00	_T("data/TEXTURE/knockout000.png")	// ノックアウト画面よう画像
#define TEXTURE_KNOCKOUT00_SIZE_X	(1000)							// テクスチャサイズ
#define TEXTURE_KNOCKOUT00_SIZE_Y	(500)							// 同上

#define TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X	(1)						// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_KNOCKOUT_Y	(2)						// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_KNOCKOUT		(TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X*TEXTURE_PATTERN_DIVIDE_KNOCKOUT_Y)	// アニメーションパターン数
#define TIME_ANIMATION_KNOCKOUT				(1)						// アニメーションの切り替わるカウント

#define KNOCKOUT_POS_X					(SCREEN_WIDTH / 2 - TEXTURE_KNOCKOUT00_SIZE_X / 2)						//カウントダウンの表示位置
#define KNOCKOUT_POS_Y					(SCREEN_HEIGHT / 2 - TEXTURE_KNOCKOUT00_SIZE_Y / 2)						//カウントダウンの表示位置


//フレームの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
	D3DXVECTOR3				pos;								// ポリゴンの移動量
	int						CountAnim;							// アニメーションカウント
	int						PatternAnim;						// アニメーションパターンナンバー
	bool					use;								// フレームを表示するかどうかのフラグ
}KNOCKOUT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKnockout(int type);
void UninitKnockout(void);
void UpdateKnockout(void);
void DrawKnockout(void);
HRESULT MakeVertexKnockout(void);
void SetTextureKnockout(int cntPattern);
void SetVertexKnockout(void);

#endif