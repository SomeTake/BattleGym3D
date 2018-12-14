//=============================================================================
//
// 2P用赤ゲージ画面処理 [eredguage.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _EREDGUAGE_H_
#define _EREDGUAGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_EREDGUAGE			_T("data/TEXTURE/ehpguage000.png")		// 読み込むテクスチャファイル名
#define	EREDGUAGE_POS_X				(680)									// ゲージの表示位置
#define	EREDGUAGE_POS_Y				(50)									// ゲージの表示位置
#define	EREDGUAGE_SIZE_X			(500)									// ゲージの幅
#define	EREDGUAGE_SIZE_Y			(50)									// ゲージの高さ
#define EREDGUAGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define EREDGUAGE_PATTERN_DIVIDE_Y	(3)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_EREDGUAGE	(EREDGUAGE_PATTERN_DIVIDE_X*EREDGUAGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_EREDGUAGE	(4)										// アニメーションの切り替わるカウント
#define EREDGUAGE_MAX				(1)										// ゲージの個数

//HPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	bool					use;							// プレイヤーの使用判定
	int						value;							// 赤ゲージをHPゲージと合わせるときに増減させる
}EREDGUAGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitERedGuage(int type);
void UninitERedGuage(void);
void UpdateERedGuage(void);
void DrawERedGuage(void);
HRESULT MakeVertexERedGuage(void);
void SetTextureERedGuage(int cntPattern);
void SetVertexERedGuage(void);
EREDGUAGE *GetERedGuage(int eno);		// EREDGAUGE構造体のゲッター

#endif
