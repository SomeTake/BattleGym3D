//=============================================================================
//
// チュートリアル表示処理 [tutorial.h]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// マクロ定義
#define TEXTURE_GAME_TUTORIAL		_T("data/TEXTURE/tutorial000.png")	// 画像
#define TEXTURE_TUTORIAL_SIZE_X	(1280)			// テクスチャサイズ
#define TEXTURE_TUTORIAL_SIZE_Y	(720)			// 同上

#define TEXTURE_PATTERN_DIVIDE_X_TUTORIAL	(1)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL	(2)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_TUTORIAL			(TEXTURE_PATTERN_DIVIDE_X_TUTORIAL*TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL)	// アニメーションパターン数

#define TUTORIAL_MAX						(1)		// チュートリアルの数

#define TUTORIAL_POS_X						(0)		// 表示位置
#define TUTORIAL_POS_Y						(0)		// 表示位置

//*****************************************************************************
// 構造体宣言
//*****************************************************************************

typedef struct	// エネミー構造体
{
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

} TUTORIAL;



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTutorial(int type);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
HRESULT MakeVertexTutorial(void);
void SetTextureTutorial(int cntPattern);
void SetVertexTutorial(void);

#endif
