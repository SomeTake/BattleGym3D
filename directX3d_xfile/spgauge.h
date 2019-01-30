//=============================================================================
//
// SPゲージ画面処理 [spgauge.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _SPGAUGE_H_
#define _SPGAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SPGAUGE			_T("data/TEXTURE/spgauge000.png")			// 読み込むテクスチャファイル名
#define	SPGAUGE_POS_X			(100)										// ゲージの表示位置
#define	SPGAUGE_POS_Y			(675)										// ゲージの表示位置
#define	SPGAUGE_SIZE_X			(300)										// ゲージの幅
#define	SPGAUGE_SIZE_Y			(30)										// ゲージの高さ
#define SPGAUGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define SPGAUGE_PATTERN_DIVIDE_Y	(1)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_SPGAUGE	(SPGAUGE_PATTERN_DIVIDE_X*SPGAUGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_SPGAUGE	(4)										// アニメーションの切り替わるカウント
#define SPGAUGE_MAX				(1)										// ゲージの個数

//SPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
}SPGAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSpGauge(int type);
void UninitSpGauge(void);
void UpdateSpGauge(void);
void DrawSpGauge(void);
HRESULT MakeVertexSpGauge(void);
void SetTextureSpGauge(int cntPattern);
void SetVertexSpGauge(void);
SPGAUGE *GetSpGauge(int gno);				// SPGAUGE構造体のゲッター

#endif
