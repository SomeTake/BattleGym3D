//=============================================================================
//
// 2P用HPゲージ画面処理 [eguage.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _EGUAGE_H_
#define _EGUAGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EGUAGE			_T("data/TEXTURE/ehpguage000.png")			// 読み込むテクスチャファイル名
#define	EGUAGE_POS_X			(680)										// ゲージの表示位置
#define	EGUAGE_POS_Y			(50)										// ゲージの表示位置
#define	EGUAGE_SIZE_X			(500)										// ゲージの幅
#define	EGUAGE_SIZE_Y			(50)										// ゲージの高さ
#define EGUAGE_PATTERN_DIVIDE_X	(1)											// アニメパターンのテクスチャ内分割数（X)
#define EGUAGE_PATTERN_DIVIDE_Y	(3)											// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_EGUAGE	(EGUAGE_PATTERN_DIVIDE_X*EGUAGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_EGUAGE	(4)											// アニメーションの切り替わるカウント
#define EGUAGE_MAX				(1)											// ゲージの個数

//HPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
}EGUAGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEGuage(int type);
void UninitEGuage(void);
void UpdateEGuage(void);
void DrawEGuage(void);
HRESULT MakeVertexEGuage(void);
void SetTextureEGuage(int cntPattern);
void SetVertexEGuage(void);

#endif
