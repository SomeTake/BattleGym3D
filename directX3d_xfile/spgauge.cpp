//=============================================================================
//
// SPゲージ画面処理 [spgauge.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "spgauge.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureSpGauge = NULL;		// テクスチャへのポインタ

SPGAUGE spgauge[SPGAUGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSpGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_SPGAUGE,				// ファイルの名前
			&g_pD3DTextureSpGauge);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化

	SPGAUGE *spgauge = GetSpGauge(0);
	spgauge->use = true;
	spgauge->pos = D3DXVECTOR3(SPGAUGE_POS_X, SPGAUGE_POS_Y, 0.0f);
	spgauge->PatternAnim = 0;
	spgauge->CountAnim = 0;

	// 頂点情報の作成
	MakeVertexSpGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSpGauge(void)
{
	if (g_pD3DTextureSpGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureSpGauge->Release();
		g_pD3DTextureSpGauge = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSpGauge(void)
{
	CHARA *player = GetPlayer();
	SPGAUGE *spgauge = GetSpGauge(0);

	if (spgauge->use == true)
	{
		spgauge->PatternAnim = 1;

		//テクスチャ座標をセット
		SetTextureSpGauge(spgauge->PatternAnim);

		SetVertexSpGauge();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSpGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPGAUGE *spgauge = GetSpGauge(0);

	if (spgauge->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureSpGauge);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, spgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSpGauge(void)
{

	// 頂点座標の設定
	SetVertexSpGauge();

	// テクスチャのパースペクティブコレクト用
	spgauge->vertexWk[0].rhw =
		spgauge->vertexWk[1].rhw =
		spgauge->vertexWk[2].rhw =
		spgauge->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	spgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	spgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	spgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	spgauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	spgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	spgauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureSpGauge(int cntPattern)
{

	int x = cntPattern % SPGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / SPGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / SPGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / SPGAUGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	spgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	spgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	spgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	spgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexSpGauge(void)
{
	CHARA *playerWk = GetPlayer();

	// 頂点座標の設定
	spgauge->vertexWk[0].vtx = D3DXVECTOR3(spgauge->pos.x, spgauge->pos.y, spgauge->pos.z);
	spgauge->vertexWk[1].vtx = D3DXVECTOR3(spgauge->pos.x + SPGAUGE_SIZE_X * ((float)playerWk->SP / (float)FULL_SPGAUGE), spgauge->pos.y, spgauge->pos.z);
	spgauge->vertexWk[2].vtx = D3DXVECTOR3(spgauge->pos.x, spgauge->pos.y + SPGAUGE_SIZE_Y, spgauge->pos.z);
	spgauge->vertexWk[3].vtx = D3DXVECTOR3(spgauge->pos.x + SPGAUGE_SIZE_X * ((float)playerWk->SP / (float)FULL_SPGAUGE), spgauge->pos.y + SPGAUGE_SIZE_Y, spgauge->pos.z);
}

//=============================================================================
// プレイヤーSPゲージのゲッター
//=============================================================================
SPGAUGE *GetSpGauge(int gno)
{
	return &spgauge[gno];
}