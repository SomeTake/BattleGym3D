//=============================================================================
//
// 黒画面処理 [blackscreen.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "blackscreen.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBlackscreen(void);
void SetReflectBlackscreen(float per);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureBlackscreen = NULL;	// テクスチャへのポインタ

static VERTEX_2D				g_vertexWkBlackscreen[NUM_VERTEX];		// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBlackscreen(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_BLACKSCREEN,			// ファイルの名前
			&g_pD3DTextureBlackscreen);	// 読み込むメモリー
	}

	// 頂点情報の作成
	MakeVertexBlackscreen();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlackscreen(void)
{
	if (g_pD3DTextureBlackscreen != NULL)
	{// テクスチャの開放
		g_pD3DTextureBlackscreen->Release();
		g_pD3DTextureBlackscreen = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlackscreen(void)
{
	int phase = *GetPhase();

	//透明度の設定
	if (phase == PhasePause || phase == PhaseTrainingPause)
	{
		SetReflectBlackscreen(TRANSPARENCY);
	}
	if (phase == PhaseTitle || phase == PhaseCompanyLogo)
	{
		SetReflectBlackscreen(1.0f);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlackscreen(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureBlackscreen);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkBlackscreen, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexBlackscreen(void)
{
	// 頂点座標の設定
	g_vertexWkBlackscreen[0].vtx = D3DXVECTOR3(BLACKSCREEN_POS_X, BLACKSCREEN_POS_Y, 0.0f);
	g_vertexWkBlackscreen[1].vtx = D3DXVECTOR3(BLACKSCREEN_POS_X + BLACKSCREEN_SIZE_X, BLACKSCREEN_POS_Y, 0.0f);
	g_vertexWkBlackscreen[2].vtx = D3DXVECTOR3(BLACKSCREEN_POS_X, BLACKSCREEN_POS_Y + BLACKSCREEN_SIZE_Y, 0.0f);
	g_vertexWkBlackscreen[3].vtx = D3DXVECTOR3(BLACKSCREEN_POS_X + BLACKSCREEN_SIZE_X, BLACKSCREEN_POS_Y + BLACKSCREEN_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkBlackscreen[0].rhw =
		g_vertexWkBlackscreen[1].rhw =
		g_vertexWkBlackscreen[2].rhw =
		g_vertexWkBlackscreen[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkBlackscreen[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBlackscreen[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBlackscreen[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkBlackscreen[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkBlackscreen[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkBlackscreen[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkBlackscreen[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkBlackscreen[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 反射光の設定 引数:int per = 透明度の％
//=============================================================================
void SetReflectBlackscreen(float per)
{
	int clear = (int)(255 * per);

	g_vertexWkBlackscreen[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkBlackscreen[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkBlackscreen[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkBlackscreen[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}