//=============================================================================
//
// 2P用SPゲージ画面処理 [espguage.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _ESPGUAGE_H_
#define _ESPGUAGE_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_ESPGUAGE		_T("data/TEXTURE/spguage000.png")			// エネミー用SPゲージの画像
#define	ESPGUAGE_POS_X			(880)										// ゲージの表示位置
#define	ESPGUAGE_POS_Y			(675)										// ゲージの表示位置
#define	ESPGUAGE_SIZE_X			(300)										// ゲージの幅
#define	ESPGUAGE_SIZE_Y			(30)										// ゲージの高さ
#define ESPGUAGE_PATTERN_DIVIDE_X	(1)										// アニメパターンのテクスチャ内分割数（X)
#define ESPGUAGE_PATTERN_DIVIDE_Y	(1)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_ESPGUAGE	(ESPGUAGE_PATTERN_DIVIDE_X*ESPGUAGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION_ESPGUAGE		(4)										// アニメーションの切り替わるカウント
#define ESPGUAGE_MAX				(1)										// ゲージの個数
#define FULL_ESPGUAGE				(1000)									// ゲージの最大値

//SPゲージの構造体
typedef struct {
	VERTEX_2D				vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				pos;							// ポリゴンの移動量
	int						CountAnim;						// アニメーションカウント
	int						PatternAnim;					// アニメーションパターンナンバー
	int						sp;								// SPゲージの現在の値
	bool					use;							// プレイヤーの使用判定
}ESPGUAGE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitESpGuage(int type);
void UninitESpGuage(void);
void UpdateESpGuage(void);
void DrawESpGuage(void);
HRESULT MakeVertexESpGuage(void);
void SetTextureESpGuage(int cntPattern);
void SetVertexESpGuage(void);
ESPGUAGE *GetESpGuage(int gno);		// ESPGAUGE構造体のゲッター
void AddESpGuage(int add);			// エネミーSPゲージを増やすときに使用

#endif
