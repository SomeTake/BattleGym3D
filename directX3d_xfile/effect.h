//=============================================================================
//
// 1P2P表示処理 [effect.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_HIT00		"data/EFFECT/hit000.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_HIT01		"data/EFFECT/hit001.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_GUARD00		"data/EFFECT/guard000.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_CHARGE00	"data/EFFECT/charge000.png"	// 読み込むテクスチャファイル名
#define	EFFECT_WIDTH			(20.0f)						// 半径高さ
#define	EFFECT_HEIGHT			(20.0f)						// 半径幅

#define HIT00_PATTERN_DIVIDE_X	(2)										// アニメパターンのテクスチャ内分割数（X)
#define HIT00_PATTERN_DIVIDE_Y	(5)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_HIT00	(HIT00_PATTERN_DIVIDE_X*HIT00_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define HIT01_PATTERN_DIVIDE_X	(3)										// アニメパターンのテクスチャ内分割数（X)
#define HIT01_PATTERN_DIVIDE_Y	(5)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_HIT01	(HIT01_PATTERN_DIVIDE_X*HIT01_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define GUARD_PATTERN_DIVIDE_X	(2)										// アニメパターンのテクスチャ内分割数（X)
#define GUARD_PATTERN_DIVIDE_Y	(5)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_GUARD	(GUARD_PATTERN_DIVIDE_X*GUARD_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define CHARGE_PATTERN_DIVIDE_X	(3)										// アニメパターンのテクスチャ内分割数（X)
#define CHARGE_PATTERN_DIVIDE_Y	(5)										// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_CHARGE	(CHARGE_PATTERN_DIVIDE_X*CHARGE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

#define EFFECT_MAX				(4)	// エフェクトの種類

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9		D3DTexture[EFFECT_MAX] = {};		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff = NULL;		// 頂点バッファインターフェースへのポインタ
	D3DXMATRIX				mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3				pos;					// 位置
	D3DXVECTOR3				scl;					// スケール
	float					width;					// 幅
	float					height;					// 高さ
	int						cntPattern;				// アニメーション番号
	int						num;					// 使用するテクスチャの番号
	bool					use;					// 使用しているかどうか
} EFFECT;

// エフェクト番号（EFFECT構造体のnumに入れて使用）
enum EffecuNum
{
	hit00,
	hit01,
	guard00,
	charge00
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffect(int type, EFFECT *EffectWk);
void UninitEffect(EFFECT *EffectWk);
void UpdateEffect(EFFECT *EffectWk);
void DrawEffect(EFFECT *EffectWk);
int SetEffect(EFFECT *EffectWk, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int num);
void SetTextureEffect(EFFECT *EffectWk, int cntPattern);

#endif
