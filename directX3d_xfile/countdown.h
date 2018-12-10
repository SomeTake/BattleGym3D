//=============================================================================
//
// バトル前のカウントダウン処理 [countdown.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_


// マクロ定義
#define TEXTURE_GAME_COUNTDOWN00	_T("data/TEXTURE/countdown000.png")	// サンプル用画像
#define TEXTURE_COUNTDOWN00_SIZE_X	(1000)							// テクスチャサイズ
#define TEXTURE_COUNTDOWN00_SIZE_Y	(250)							// 同上

#define TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X	(1)					// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_COUNTDOWN_Y	(4)						// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_COUNTDOWN		(TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X*TEXTURE_PATTERN_DIVIDE_COUNTDOWN_Y)	// アニメーションパターン数
#define TIME_ANIMATION_COUNTDOWN		(1)							// アニメーションの切り替わるカウント

#define COUNTDOWN_MAX					(1)							//カウントダウンの桁数
#define FULL_COUNTDOWN					(4)							//カウントダウンの最大数

#define COUNTDOWN_POS_X					(SCREEN_WIDTH / 2 - TEXTURE_COUNTDOWN00_SIZE_X / 2)						//カウントダウンの表示位置
#define COUNTDOWN_POS_Y					(SCREEN_HEIGHT / 2 - TEXTURE_COUNTDOWN00_SIZE_Y / 2)					//カウントダウンの表示位置

//スコアの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];				// 頂点情報格納ワーク
	D3DXVECTOR3				pos;								// ポリゴンの移動量
	int						CountAnim;							// アニメーションカウント
	int						PatternAnim;						// アニメーションパターンナンバー
	bool					use;								// 使用しているかどうかのフラグ
}COUNTDOWN;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCountdown(int type);
void UninitCountdown(void);
void UpdateCountdown(void);
void DrawCountdown(void);
COUNTDOWN *GetCountdown(int cno);

#endif
