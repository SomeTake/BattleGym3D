//=============================================================================
//
// プレイヤー用赤ゲージ画面処理 [redgauge.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _REDGAUGE_H_
#define _REDGAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_REDGAUGE			_T("data/TEXTURE/hpgauge000.png")		// 赤ゲージ用画像
#define	REDGAUGE_POS_X				(100)									// 赤ゲージの表示位置
#define	REDGAUGE_POS_Y				(50)									// 赤ゲージの表示位置
#define	REDGAUGE_SIZE_X				(500)									// 赤ゲージの幅
#define	REDGAUGE_SIZE_Y				(50)									// 赤ゲージの高さ
#define REDGAUGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define REDGAUGE_PATTERN_DIVIDE_Y	(3)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_REDGAUGE	(REDGAUGE_PATTERN_DIVIDE_X*REDGAUGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_REDGAUGE		(4)										// アニメーションの切り替わるカウント
#define REDGAUGE_MAX				(1)										// 赤ゲージの個数

//HPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
	int						value;							// 赤ゲージをHPゲージと合わせるときに増減させる
}REDGAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRedGauge(int type);
void UninitRedGauge(void);
void UpdateRedGauge(void);
void DrawRedGauge(void);
HRESULT MakeVertexRedGauge(void);
void SetTextureRedGauge(int cntPattern);
void SetVertexRedGauge(void);
REDGAUGE *GetRedGauge(void);

#endif
