//=============================================================================
//
// スコア処理 [score.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_


// マクロ定義
#define TEXTURE_GAME_SCORE00	("data/TEXTURE/score000.png")	// サンプル用画像

//PhaseGame,PhaseCountdown,PhaseFinishの時
#define TEXTURE_SCORE00_SIZE_X	(20) // テクスチャサイズ
#define TEXTURE_SCORE00_SIZE_Y	(30) // 同上

//PhaseResultの時
#define RESULT_SCORE00_SIZE_X	(100) // テクスチャサイズ
#define RESULT_SCORE00_SIZE_Y	(150) // 同上

#define TEXTURE_PATTERN_DIVIDE_SCORE_X	(10)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_SCORE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_SCORE		(TEXTURE_PATTERN_DIVIDE_SCORE_X*TEXTURE_PATTERN_DIVIDE_SCORE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_SCORE		(1)	// アニメーションの切り替わるカウント

#define SCORE_MAX					(5)		//スコアの桁数
#define FULL_SCORE					(99999)	//スコアの最大数

//PhaseGame,PhaseCountdown,PhaseFinishの時
#define SCORE_POS_X					(500)	//スコアの表示位置
#define SCORE_POS_Y					(18)	//スコアの表示位置

//PhaseResultの時
#define RESULT_SCORE_POS_X			(600)	//スコアの表示位置
#define RESULT_SCORE_POS_Y			(400)	//スコアの表示位置

//スコアの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							//弾を発射したかどうかのフラグ
	int						pScore;							//バトルのスコア
}SCORE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(int type);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
SCORE *GetScore(int bno);
void AddScore(int add);

#endif
