//=============================================================================
//
// HPゲージ画面処理 [gauge.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_GAUGE			_T("data/TEXTURE/hpgauge000.png")		// 読み込むテクスチャファイル名
#define	GAUGE_POS_X				(100)									// ゲージの表示位置
#define	GAUGE_POS_Y				(50)									// ゲージの表示位置
#define	GAUGE_SIZE_X			(500)									// ゲージの幅
#define	GAUGE_SIZE_Y			(50)									// ゲージの高さ
#define GAUGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define GAUGE_PATTERN_DIVIDE_Y	(3)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_GAUGE	(GAUGE_PATTERN_DIVIDE_X*GAUGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_GAUGE	(4)										// アニメーションの切り替わるカウント
#define GAUGE_MAX				(1)										// ゲージの個数

//HPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
}GAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGauge(int type);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);
HRESULT MakeVertexGauge(void);
void SetTextureGauge(int cntPattern);
void SetVertexGauge(void);

#endif
