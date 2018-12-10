//=============================================================================
//
// ポーズ画面処理 [pause.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "pause.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPause(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTexturePause = NULL;	// テクスチャへのポインタ

VERTEX_2D				g_vertexWkPause[NUM_VERTEX];		// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_PAUSE,			// ファイルの名前
			&g_pD3DTexturePause);	// 読み込むメモリー
	}

	// 頂点情報の作成
	MakeVertexPause();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	if (g_pD3DTexturePause != NULL)
	{// テクスチャの開放
		g_pD3DTexturePause->Release();
		g_pD3DTexturePause = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTexturePause);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkPause, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexPause(void)
{
	// 頂点座標の設定
	g_vertexWkPause[0].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
	g_vertexWkPause[1].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y, 0.0f);
	g_vertexWkPause[2].vtx = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);
	g_vertexWkPause[3].vtx = D3DXVECTOR3(PAUSE_POS_X + PAUSE_SIZE_X, PAUSE_POS_Y + PAUSE_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkPause[0].rhw =
		g_vertexWkPause[1].rhw =
		g_vertexWkPause[2].rhw =
		g_vertexWkPause[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkPause[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkPause[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkPause[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkPause[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkPause[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkPause[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkPause[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkPause[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

