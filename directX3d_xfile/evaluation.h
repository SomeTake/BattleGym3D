//=============================================================================
//
// スコア評価処理 [evaluation.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _EVALUATION_H_
#define _EVALUATION_H_

// マクロ定義
#define TEXTURE_GAME_EVALUATION		_T("data/TEXTURE/evaluation000.png")	// 画像
#define TEXTURE_EVALUATION_SIZE_X	(150)			// テクスチャサイズ
#define TEXTURE_EVALUATION_SIZE_Y	(150)			// 同上

#define TEXTURE_PATTERN_DIVIDE_X_EVALUATION	(1)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_EVALUATION	(3)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_EVALUATION			(TEXTURE_PATTERN_DIVIDE_X_EVALUATION*TEXTURE_PATTERN_DIVIDE_Y_EVALUATION)	// アニメーションパターン数

#define EVALUATION_MAX						(1)		// 評価の数

#define EVALUATION_POS_X					(1040)	//表示位置
#define EVALUATION_POS_Y					(490)	//表示位置

//スコア評価
#define SCORE_A								(1500)
#define SCORE_B								(1000)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// エネミー構造体
{
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} EVALUATION;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEvaluation(int type);
void UninitEvaluation(void);
void UpdateEvaluation(void);
void DrawEvaluation(void);
EVALUATION *GetEvaluation(int no);


#endif
