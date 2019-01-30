//=============================================================================
//
// 2P用SPゲージ画面処理 [espgauge.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _ESPGAUGE_H_
#define _ESPGAUGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_ESPGAUGE		_T("data/TEXTURE/spgauge000.png")			// エネミー用SPゲージの画像
#define	ESPGAUGE_POS_X			(880)										// ゲージの表示位置
#define	ESPGAUGE_POS_Y			(675)										// ゲージの表示位置
#define	ESPGAUGE_SIZE_X			(300)										// ゲージの幅
#define	ESPGAUGE_SIZE_Y			(30)										// ゲージの高さ
#define ESPGAUGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define ESPGAUGE_PATTERN_DIVIDE_Y	(1)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_ESPGAUGE	(ESPGAUGE_PATTERN_DIVIDE_X*ESPGAUGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_ESPGAUGE		(4)										// アニメーションの切り替わるカウント
#define ESPGAUGE_MAX				(1)										// ゲージの個数

//SPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
}ESPGAUGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitESpGauge(int type);
void UninitESpGauge(void);
void UpdateESpGauge(void);
void DrawESpGauge(void);
HRESULT MakeVertexESpGauge(void);
void SetTextureESpGauge(int cntPattern);
void SetVertexESpGauge(void);
ESPGAUGE *GetESpGauge(int gno);		// ESPGAUGE構造体のゲッター

#endif
