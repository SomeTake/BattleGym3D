//=============================================================================
//
// 2P用HPゲージ画面処理 [egauge.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "egauge.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEGauge = NULL;		// テクスチャへのポインタ

EGAUGE egauge[EGAUGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_EGAUGE,				// ファイルの名前
			&g_pD3DTextureEGauge);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	egauge->use = true;
	egauge->pos = D3DXVECTOR3(EGAUGE_POS_X, EGAUGE_POS_Y, 0.0f);

	// 頂点情報の作成
	MakeVertexEGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEGauge(void)
{
	if (g_pD3DTextureEGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureEGauge->Release();
		g_pD3DTextureEGauge = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEGauge(void)
{
	CHARA *enemy = GetEnemy();

	if (egauge->use == true)
	{
		//HP満タンの場合
		if (enemy->HPzan == enemy->HP)
		{
			egauge->PatternAnim = 0;
		}
		//100%未満の場合
		else if (enemy->HPzan < enemy->HP)
		{
			egauge->PatternAnim = 1;
		}

		//テクスチャ座標をセット
		SetTextureEGauge(egauge->PatternAnim);


		SetVertexEGauge();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (egauge->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureEGauge);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, egauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEGauge(void)
{

	// 頂点座標の設定
	SetVertexEGauge();

	// テクスチャのパースペクティブコレクト用
	egauge->vertexWk[0].rhw =
		egauge->vertexWk[1].rhw =
		egauge->vertexWk[2].rhw =
		egauge->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	egauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	egauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	egauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	egauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	egauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	egauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	egauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	egauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEGauge(int cntPattern)
{
	CHARA *enemy = GetEnemy();

	int x = cntPattern % EGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / EGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / EGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / EGAUGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	egauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	egauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)enemy->HPzan / (float)enemy->HP), (float)(y)* sizeY);
	egauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	egauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)enemy->HPzan / (float)enemy->HP), (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEGauge(void)
{
	CHARA *enemy = GetEnemy();

	// 頂点座標の設定
	egauge->vertexWk[0].vtx = D3DXVECTOR3(egauge->pos.x, egauge->pos.y, egauge->pos.z);
	egauge->vertexWk[1].vtx = D3DXVECTOR3(egauge->pos.x + EGAUGE_SIZE_X * ((float)enemy->HPzan / (float)enemy->HP), egauge->pos.y, egauge->pos.z);
	egauge->vertexWk[2].vtx = D3DXVECTOR3(egauge->pos.x, egauge->pos.y + EGAUGE_SIZE_Y, egauge->pos.z);
	egauge->vertexWk[3].vtx = D3DXVECTOR3(egauge->pos.x + EGAUGE_SIZE_X * ((float)enemy->HPzan / (float)enemy->HP), egauge->pos.y + EGAUGE_SIZE_Y, egauge->pos.z);
}
