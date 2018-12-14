//=============================================================================
//
// SPゲージ画面処理 [spguage.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _SPGUAGE_H_
#define _SPGUAGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SPGUAGE			_T("data/TEXTURE/spguage000.png")			// 読み込むテクスチャファイル名
#define	SPGUAGE_POS_X			(100)										// ゲージの表示位置
#define	SPGUAGE_POS_Y			(675)										// ゲージの表示位置
#define	SPGUAGE_SIZE_X			(300)										// ゲージの幅
#define	SPGUAGE_SIZE_Y			(30)										// ゲージの高さ
#define SPGUAGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define SPGUAGE_PATTERN_DIVIDE_Y	(1)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_SPGUAGE	(SPGUAGE_PATTERN_DIVIDE_X*SPGUAGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_SPGUAGE	(4)										// アニメーションの切り替わるカウント
#define SPGUAGE_MAX				(1)										// ゲージの個数
#define FULL_SPGUAGE			(1000)									// ゲージの最大値

//SPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	int						sp;								// SPゲージの現在の値
	bool					use;							// プレイヤーの使用判定
}SPGUAGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSpGuage(int type);
void UninitSpGuage(void);
void UpdateSpGuage(void);
void DrawSpGuage(void);
HRESULT MakeVertexSpGuage(void);
void SetTextureSpGuage(int cntPattern);
void SetVertexSpGuage(void);
SPGUAGE *GetSpGuage(int gno);				// SPGAUGE構造体のゲッター
void AddSpGuage(int add);					// SPゲージを増やすときに使用

#endif
