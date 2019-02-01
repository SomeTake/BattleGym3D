//=============================================================================
//
// プレイヤー用赤ゲージ画面処理 [redgauge.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "redgauge.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureRedGauge = NULL;		// テクスチャへのポインタ

REDGAUGE redgauge[REDGAUGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRedGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHARA *player = GetPlayer();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_REDGAUGE,				// ファイルの名前
			&g_pD3DTextureRedGauge);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	redgauge->use = true;
	redgauge->pos = D3DXVECTOR3(REDGAUGE_POS_X, REDGAUGE_POS_Y, 0.0f);
	redgauge->value = player->HPzan;

	// 頂点情報の作成
	MakeVertexRedGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRedGauge(void)
{
	if (g_pD3DTextureRedGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureRedGauge->Release();
		g_pD3DTextureRedGauge = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRedGauge(void)
{
	if (redgauge->use == true)
	{
		//赤ゲージ
		redgauge->PatternAnim = 2;

		//テクスチャ座標をセット
		SetTextureRedGauge(redgauge->PatternAnim);


		SetVertexRedGauge();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRedGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (redgauge->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureRedGauge);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, redgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexRedGauge(void)
{

	// 頂点座標の設定
	SetVertexRedGauge();

	// テクスチャのパースペクティブコレクト用
	redgauge->vertexWk[0].rhw =
		redgauge->vertexWk[1].rhw =
		redgauge->vertexWk[2].rhw =
		redgauge->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	redgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	redgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	redgauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	redgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	redgauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureRedGauge(int cntPattern)
{
	CHARA *player = GetPlayer();

	if (redgauge->value > player->HPzan)
	{
		redgauge->value -= 5;
	}

	//トレーニングモードなどで回復した時用
	if (player->HPzan > redgauge->value)
	{
		redgauge->value = player->HPzan;
	}

	int x = cntPattern % REDGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / REDGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / REDGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / REDGAUGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	redgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - redgauge->value) / (float)player->HP), (float)(y)* sizeY);
	redgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	redgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - redgauge->value) / (float)player->HP), (float)(y)* sizeY + sizeY);
	redgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexRedGauge(void)
{
	CHARA *player = GetPlayer();

	// 頂点座標の設定
	redgauge->vertexWk[0].vtx = D3DXVECTOR3(redgauge->pos.x + REDGAUGE_SIZE_X * ((float)(player->HP - redgauge->value) / player->HP), redgauge->pos.y, redgauge->pos.z);
	redgauge->vertexWk[1].vtx = D3DXVECTOR3(redgauge->pos.x + REDGAUGE_SIZE_X, redgauge->pos.y, redgauge->pos.z);
	redgauge->vertexWk[2].vtx = D3DXVECTOR3(redgauge->pos.x + REDGAUGE_SIZE_X * ((float)(player->HP - redgauge->value) / player->HP), redgauge->pos.y + REDGAUGE_SIZE_Y, redgauge->pos.z);
	redgauge->vertexWk[3].vtx = D3DXVECTOR3(redgauge->pos.x + REDGAUGE_SIZE_X, redgauge->pos.y + REDGAUGE_SIZE_Y, redgauge->pos.z);
}

//=============================================================================
// 赤ゲージのプレイヤー側ゲッター
//=============================================================================
REDGAUGE *GetRedGauge(void)
{
	return &redgauge[0];
}