//=============================================================================
//
// HPゲージ画面処理 [gauge.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "gauge.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureGauge = NULL;		// テクスチャへのポインタ

GAUGE gauge[GAUGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_GAUGE,				// ファイルの名前
			&g_pD3DTextureGauge);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	gauge->use = true;
	gauge->pos = D3DXVECTOR3(GAUGE_POS_X, GAUGE_POS_Y, 0.0f);

	// 頂点情報の作成
	MakeVertexGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGauge(void)
{
	if (g_pD3DTextureGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureGauge->Release();
		g_pD3DTextureGauge = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGauge(void)
{
	CHARA *player = GetPlayer();

	if (gauge->use == true)
	{
		//HP満タンの場合
		if (player->HPzan == player->HP)
		{
			gauge->PatternAnim = 0;
		}
		//100%未満の場合
		else if (player->HPzan < player->HP)
		{
			gauge->PatternAnim = 1;
		}

		//テクスチャ座標をセット
		SetTextureGauge(gauge->PatternAnim);

		SetVertexGauge();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (gauge->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureGauge);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, gauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGauge(void)
{

	// 頂点座標の設定
	SetVertexGauge();

	// テクスチャのパースペクティブコレクト用
	gauge->vertexWk[0].rhw =
		gauge->vertexWk[1].rhw =
		gauge->vertexWk[2].rhw =
		gauge->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	gauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	gauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	gauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	gauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	gauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	gauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureGauge(int cntPattern)
{
	CHARA *player = GetPlayer();

	int x = cntPattern % GAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / GAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / GAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / GAUGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	gauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - player->HPzan) / (float)player->HP), (float)(y)* sizeY);
	gauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	gauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - player->HPzan) / (float)player->HP), (float)(y)* sizeY + sizeY);
	gauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexGauge(void)
{
	CHARA *player = GetPlayer();

	// 頂点座標の設定
	gauge->vertexWk[0].vtx = D3DXVECTOR3(gauge->pos.x + GAUGE_SIZE_X * ((float)(player->HP - player->HPzan) / player->HP), gauge->pos.y, gauge->pos.z);
	gauge->vertexWk[1].vtx = D3DXVECTOR3(gauge->pos.x + GAUGE_SIZE_X, gauge->pos.y, gauge->pos.z);
	gauge->vertexWk[2].vtx = D3DXVECTOR3(gauge->pos.x + GAUGE_SIZE_X * ((float)(player->HP - player->HPzan) / player->HP), gauge->pos.y + GAUGE_SIZE_Y, gauge->pos.z);
	gauge->vertexWk[3].vtx = D3DXVECTOR3(gauge->pos.x + GAUGE_SIZE_X, gauge->pos.y + GAUGE_SIZE_Y, gauge->pos.z);
}
