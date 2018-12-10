//=============================================================================
//
// プレイヤー用赤ゲージ画面処理 [redguage.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _REDGUAGE_H_
#define _REDGUAGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_REDGUAGE			_T("data/TEXTURE/hpguage000.png")		// 読み込むテクスチャファイル名
#define	REDGUAGE_POS_X				(100)									// ゲージの表示位置
#define	REDGUAGE_POS_Y				(50)									// ゲージの表示位置
#define	REDGUAGE_SIZE_X				(500)									// ゲージの幅
#define	REDGUAGE_SIZE_Y				(50)									// ゲージの高さ
#define REDGUAGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define REDGUAGE_PATTERN_DIVIDE_Y	(3)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_REDGUAGE	(REDGUAGE_PATTERN_DIVIDE_X*REDGUAGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_REDGUAGE		(4)										// アニメーションの切り替わるカウント
#define REDGUAGE_MAX				(1)										// ゲージの個数

//HPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
	int						red;							// 赤ゲージ
}REDGUAGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRedGuage(int type);
void UninitRedGuage(void);
void UpdateRedGuage(void);
void DrawRedGuage(void);
REDGUAGE *GetRedGuage(int eno);

#endif
