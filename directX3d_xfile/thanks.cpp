//=============================================================================
//
// 終了画面処理 [thanks.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "thanks.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureThanks = NULL;		// テクスチャへのポインタ

VERTEX_2D				g_vertexWkThanks[NUM_VERTEX];	// 頂点情報格納ワーク

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitThanks(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_THANKS,			// ファイルの名前
			&g_pD3DTextureThanks);	// 読み込むメモリー
	}

	// 頂点情報の作成
	MakeVertexThanks();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitThanks(void)
{
	if (g_pD3DTextureThanks != NULL)
	{// テクスチャの開放
		g_pD3DTextureThanks->Release();
		g_pD3DTextureThanks = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateThanks(void)
{
	static float per = 0.0f;
	static int frame = 0;

	frame++;

	//透明度の設定
	//徐々に表示
	if (frame < 100)
	{
		per += 0.01f;
	}
	//徐々に消していく
	else if (frame >= 100 && frame < 200)
	{
		per -= 0.01f;
	}

	//完全に消えたらタイトル画面へ
	if (frame == 200)
	{
		SetPhase(PhaseExit);
	}
	SetReflectThanks(per);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawThanks(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureThanks);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_vertexWkThanks, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexThanks(void)
{
	// 頂点座標の設定
	g_vertexWkThanks[0].vtx = D3DXVECTOR3(THANKS_POS_X, THANKS_POS_Y, 0.0f);
	g_vertexWkThanks[1].vtx = D3DXVECTOR3(THANKS_POS_X + THANKS_SIZE_X, THANKS_POS_Y, 0.0f);
	g_vertexWkThanks[2].vtx = D3DXVECTOR3(THANKS_POS_X, THANKS_POS_Y + THANKS_SIZE_Y, 0.0f);
	g_vertexWkThanks[3].vtx = D3DXVECTOR3(THANKS_POS_X + THANKS_SIZE_X, THANKS_POS_Y + THANKS_SIZE_Y, 0.0f);

	// テクスチャのパースペクティブコレクト用
	g_vertexWkThanks[0].rhw =
		g_vertexWkThanks[1].rhw =
		g_vertexWkThanks[2].rhw =
		g_vertexWkThanks[3].rhw = 1.0f;

	// 反射光の設定
	g_vertexWkThanks[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkThanks[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkThanks[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_vertexWkThanks[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	g_vertexWkThanks[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	g_vertexWkThanks[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	g_vertexWkThanks[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	g_vertexWkThanks[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// 反射光の設定 引数:float per = 透明度の％
//=============================================================================
void SetReflectThanks(float per)
{
	int clear = (int)(255 * per);

	g_vertexWkThanks[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkThanks[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkThanks[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	g_vertexWkThanks[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}