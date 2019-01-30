//=============================================================================
//
// 2P用HPゲージ画面処理 [egauge.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _EGAUGE_H_
#define _EGAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EGAUGE			_T("data/TEXTURE/ehpgauge000.png")			// 読み込むテクスチャファイル名
#define	EGAUGE_POS_X			(680)										// ゲージの表示位置
#define	EGAUGE_POS_Y			(50)										// ゲージの表示位置
#define	EGAUGE_SIZE_X			(500)										// ゲージの幅
#define	EGAUGE_SIZE_Y			(50)										// ゲージの高さ
#define EGAUGE_PATTERN_DIVIDE_X	(1)											// アニメパターンのテクスチャ内分割数（X)
#define EGAUGE_PATTERN_DIVIDE_Y	(3)											// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_EGAUGE	(EGAUGE_PATTERN_DIVIDE_X*EGAUGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_EGAUGE	(4)											// アニメーションの切り替わるカウント
#define EGAUGE_MAX				(1)											// ゲージの個数

//HPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
}EGAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEGauge(int type);
void UninitEGauge(void);
void UpdateEGauge(void);
void DrawEGauge(void);
HRESULT MakeVertexEGauge(void);
void SetTextureEGauge(int cntPattern);
void SetVertexEGauge(void);

#endif
