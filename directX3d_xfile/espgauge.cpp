//=============================================================================
//
// 2P用SPゲージ画面処理 [espgauge.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "espgauge.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureESpGauge = NULL;		// テクスチャへのポインタ

ESPGAUGE espgauge[ESPGAUGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitESpGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_ESPGAUGE,				// ファイルの名前
			&g_pD3DTextureESpGauge);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	ESPGAUGE *espgauge = GetESpGauge(0);
	espgauge->use = true;
	espgauge->pos = D3DXVECTOR3(ESPGAUGE_POS_X, ESPGAUGE_POS_Y, 0.0f);
	espgauge->PatternAnim = 0;
	espgauge->CountAnim = 0;

	// 頂点情報の作成
	MakeVertexESpGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitESpGauge(void)
{
	if (g_pD3DTextureESpGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureESpGauge->Release();
		g_pD3DTextureESpGauge = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateESpGauge(void)
{
	ESPGAUGE *espgauge = GetESpGauge(0);

	if (espgauge->use == true)
	{
		espgauge->PatternAnim = 1;

		//テクスチャ座標をセット
		SetTextureESpGauge(espgauge->PatternAnim);

		SetVertexESpGauge();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawESpGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ESPGAUGE *espgauge = GetESpGauge(0);

	if (espgauge->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureESpGauge);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, espgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexESpGauge(void)
{
	// 頂点座標の設定
	SetVertexESpGauge();

	// テクスチャのパースペクティブコレクト用
	espgauge->vertexWk[0].rhw =
		espgauge->vertexWk[1].rhw =
		espgauge->vertexWk[2].rhw =
		espgauge->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	espgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	espgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	espgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	espgauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	espgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	espgauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureESpGauge(int cntPattern)
{
	int x = cntPattern % ESPGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / ESPGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / ESPGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / ESPGAUGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	espgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	espgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	espgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	espgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexESpGauge(void)
{
	CHARA *enemyWk = GetEnemy();

	// 頂点座標の設定
	espgauge->vertexWk[0].vtx = D3DXVECTOR3(espgauge->pos.x + ESPGAUGE_SIZE_X - ESPGAUGE_SIZE_X * ((float)enemyWk->SP / (float)FULL_SPGAUGE), espgauge->pos.y, espgauge->pos.z);
	espgauge->vertexWk[1].vtx = D3DXVECTOR3(espgauge->pos.x + ESPGAUGE_SIZE_X, espgauge->pos.y, espgauge->pos.z);
	espgauge->vertexWk[2].vtx = D3DXVECTOR3(espgauge->pos.x + ESPGAUGE_SIZE_X - ESPGAUGE_SIZE_X * ((float)enemyWk->SP / (float)FULL_SPGAUGE), espgauge->pos.y + ESPGAUGE_SIZE_Y, espgauge->pos.z);
	espgauge->vertexWk[3].vtx = D3DXVECTOR3(espgauge->pos.x + ESPGAUGE_SIZE_X, espgauge->pos.y + ESPGAUGE_SIZE_Y, espgauge->pos.z);
}

//=============================================================================
// エネミーSPゲージのゲッター
//=============================================================================
ESPGAUGE *GetESpGauge(int gno)
{
	return &espgauge[gno];
}
