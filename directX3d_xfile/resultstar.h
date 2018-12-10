//=============================================================================
//
// リザルト画面のスター表示処理 [resultstar.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _RESULTSTAR_H_
#define _RESULTSTAR_H_


// マクロ定義
#define STAR00	_T("data/TEXTURE/star000.png")				// サンプル用画像
#define STAR_SIZE_X	(300) // テクスチャサイズ
#define STAR_SIZE_Y	(300) // 同上

#define TEXTURE_PATTERN_DIVIDE_STAR_X	(1)					// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_STAR_Y	(1)					// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_STAR		(TEXTURE_PATTERN_DIVIDE_STAR_X*TEXTURE_PATTERN_DIVIDE_STAR_Y)	// アニメーションパターン数
#define TIME_ANIMATION_STAR				(4)					// アニメーションの切り替わるカウント

#define STAR_MAX			(1)								//スターの数

#define STAR_POS_X			(970)							// 表示位置
#define STAR_POS_Y			(400)							// 表示位置

//バレットの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	D3DXVECTOR3				rot;							// ポリゴンの回転量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// 使用中フラグ
	float					angle;							// 中心からの角度
	float					radius;							// 中心からの距離
}RESULTSTAR;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitResultstar(int type);
void UninitResultstar(void);
void UpdateResultstar(void);
void DrawResultstar(void);
RESULTSTAR *GetResultstar(int sno);

#endif
