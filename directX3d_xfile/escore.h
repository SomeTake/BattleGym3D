//=============================================================================
//
// エネミースコア処理 [escore.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _ESCORE_H_
#define _ESCORE_H_


// マクロ定義
#define TEXTURE_GAME_ESCORE00	_T("data/TEXTURE/score000.png")	// エネミースコア用画像

//PhaseGame,PhaseCountdown,PhaseFinishの時
#define TEXTURE_ESCORE00_SIZE_X		(20)	// テクスチャサイズ
#define TEXTURE_ESCORE00_SIZE_Y		(30)	// 同上

//PhaseResultの時
#define RESULT_ESCORE00_SIZE_X		(100)	// テクスチャサイズ
#define RESULT_ESCORE00_SIZE_Y		(150)	// 同上

#define TEXTURE_PATTERN_DIVIDE_ESCORE_X	(10)// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_ESCORE_Y	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_ESCORE		(TEXTURE_PATTERN_DIVIDE_ESCORE_X*TEXTURE_PATTERN_DIVIDE_ESCORE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_ESCORE		(1)		// アニメーションの切り替わるカウント

#define ESCORE_MAX					(5)		//スコアの桁数

//PhaseGame,PhaseCountdown,PhaseFinishの時
#define ESCORE_POS_X				(683)	//スコアの表示位置
#define ESCORE_POS_Y				(18)	//スコアの表示位置

//PhaseResultの時
#define RESULT_ESCORE_POS_X			(600)	//スコアの表示位置
#define RESULT_ESCORE_POS_Y			(400)	//スコアの表示位置

//スコアの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// 使用中フラグ
}ESCORE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEScore(int type);
void UninitEScore(void);
void UpdateEScore(void);
void DrawEScore(void);
HRESULT MakeVertexEScore(int sno);
void SetTextureEScore(int cntPattern, int sno);
void SetVertexEScore(int sno);
ESCORE *GetEScore(int bno);		// ESCORE構造体のゲッター

#endif
