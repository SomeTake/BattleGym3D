//=============================================================================
//
// 空処理 [skybox.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SKYBOX					_T("data/TEXTURE/skybox000.png")	// 読み込むテクスチャファイル名
#define TEXTURE_SKYBOX_DIVIDE_X			(4)		// テクスチャ内分割数（横)
#define TEXTURE_SKYBOX_DIVIDE_Y			(3)		// テクスチャ内分割数（縦)
#define SURFACE_NUM						(6)
#define SKYBOX_LENGTH					(5000.0f)
#define SKYBLUE(Alpha)		D3DCOLOR_RGBA(135, 206, 235, Alpha)

typedef struct
{
	D3DXVECTOR3				pos;			// 空の位置
	D3DXVECTOR3				rot;			// 空の向き(回転)
	D3DXVECTOR3				scl;			// 空の大きさ(スケール)
}SKYBOX;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSkyBox(int type);
void UninitSkyBox(void);
void UpdateSkyBox(void);
void DrawSkyBox(void);

#endif
