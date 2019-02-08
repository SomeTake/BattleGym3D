//=============================================================================
//
// 入力モードセレクト画面処理 [inputselect.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "inputselect.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureInputselect = NULL;		// テクスチャへのポインタ

VERTEX_2D				g_vertexWkInputselect[NUM_VERTEX];	// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitInputselect(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_INPUTSELECT,			// ファイルの名前
			&g_pD3DTextureInputselect);	// 読み込むメモリー
	}

	// 頂点情報の作成
	MakeVertexInputselect();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitInputselect(void)
{
	if (g_pD3DTextureInputselect != NULL)
	{// テクスチャの開放
		g_pD3DTextureInputselect->Release();
		g_pD3DTextureInputselect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateInputselect(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawInputselect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureInputselect);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkInputselect, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexInputselect(void)
{
	// 頂点座標の設定
	g_vertexWkInputselect[0].vtx = D3DXVECTOR3(INPUTSELECT_POS_X, INPUTSELECT_POS_Y, 0.0f);
	g_vertexWkInputselect[1].vtx = D3DXVECTOR3(INPUTSELECT_POS_X + INPUTSELECT_SIZE_X, INPUTSELECT_POS_Y, 0.0f);
	g_vertexWkInputselect[2].vtx = D3DXVECTOR3(INPUTSELECT_POS_X, INPUTSELECT_POS_Y + INPUTSELECT_SIZE_Y, 0.0f);
	g_vertexWkInputselect[3].vtx = D3DXVECTOR3(INPUTSELECT_POS_X + INPUTSELECT_SIZE_X, INPUTSELECT_POS_Y + INPUTSELECT_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkInputselect[0].rhw =
		g_vertexWkInputselect[1].rhw =
		g_vertexWkInputselect[2].rhw =
		g_vertexWkInputselect[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkInputselect[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkInputselect[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkInputselect[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkInputselect[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkInputselect[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkInputselect[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkInputselect[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkInputselect[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

