//=============================================================================
//
// HPゲージ画面処理 [guage.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _GUAGE_H_
#define _GUAGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_GUAGE			_T("data/TEXTURE/hpguage000.png")			// 読み込むテクスチャファイル名
#define	GUAGE_POS_X				(100)										// ゲージの表示位置
#define	GUAGE_POS_Y				(50)										// ゲージの表示位置
#define	GUAGE_SIZE_X			(500)									// ゲージの幅
#define	GUAGE_SIZE_Y			(50)									// ゲージの高さ
#define GUAGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define GUAGE_PATTERN_DIVIDE_Y	(3)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_GUAGE	(GUAGE_PATTERN_DIVIDE_X*GUAGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_GUAGE	(4)										// アニメーションの切り替わるカウント
#define GUAGE_MAX				(1)										// ゲージの個数

//HPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
}GUAGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGuage(int type);
void UninitGuage(void);
void UpdateGuage(void);
void DrawGuage(void);

#endif
