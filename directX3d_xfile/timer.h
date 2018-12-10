//=============================================================================
//
// タイマー処理 [timer.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_


// マクロ定義
#define TEXTURE_GAME_TIMER00	_T("data/TEXTURE/timer000.png")	// サンプル用画像
#define TEXTURE_TIMER00_SIZE_X	(35)							// テクスチャサイズ
#define TEXTURE_TIMER00_SIZE_Y	(70)							// 同上

#define TEXTURE_PATTERN_DIVIDE_TIMER_X	(10)					// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_TIMER_Y	(1)						// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_TIMER		(TEXTURE_PATTERN_DIVIDE_TIMER_X*TEXTURE_PATTERN_DIVIDE_TIMER_Y)	// アニメーションパターン数
#define TIME_ANIMATION_TIMER		(1)							// アニメーションの切り替わるカウント

#define TIMER_MAX					(2)							//タイマーの桁数
#define FULL_TIMER					(99)						//タイマーの最大数

#define TIMER_POS_X					(600)						//タイマーの表示位置
#define TIMER_POS_Y					(35)						//タイマーの表示位置

//スコアの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
	D3DXVECTOR3				pos;								// ポリゴンの移動量
	int						CountAnim;							// アニメーションカウント
	int						PatternAnim;						// アニメーションパターンナンバー
	bool					use;								// 使用しているかどうかのフラグ
}TIMER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimer(int type);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
TIMER *GetTimer(int tno);

#endif
