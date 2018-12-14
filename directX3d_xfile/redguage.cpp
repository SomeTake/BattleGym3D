//=============================================================================
//
// プレイヤー用赤ゲージ画面処理 [redguage.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "redguage.h"
#include "player.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureRedGuage = NULL;		// テクスチャへのポインタ

REDGUAGE redguage[REDGUAGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRedGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_REDGUAGE,				// ファイルの名前
			&g_pD3DTextureRedGuage);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	redguage->use = true;
	redguage->pos = D3DXVECTOR3(REDGUAGE_POS_X, REDGUAGE_POS_Y, 0.0f);
	redguage->value = player->HPzan;

	// 頂点情報の作成
	MakeVertexRedGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRedGuage(void)
{
	if (g_pD3DTextureRedGuage != NULL)
	{// テクスチャの開放
		g_pD3DTextureRedGuage->Release();
		g_pD3DTextureRedGuage = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRedGuage(void)
{
	if (redguage->use == true)
	{
		//赤ゲージ
		redguage->PatternAnim = 2;

		//テクスチャ座標をセット
		SetTextureRedGuage(redguage->PatternAnim);


		SetVertexRedGuage();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRedGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (redguage->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureRedGuage);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, redguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexRedGuage(void)
{

	// 頂点座標の設定
	SetVertexRedGuage();

	// テクスチャのパースペクティブコレクト用
	redguage->vertexWk[0].rhw =
		redguage->vertexWk[1].rhw =
		redguage->vertexWk[2].rhw =
		redguage->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	redguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	redguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	redguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	redguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	redguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	redguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureRedGuage(int cntPattern)
{
	PLAYER *player = GetPlayer(0);

	if (redguage->value > player->HPzan)
	{
		redguage->value -= 5;
	}

	//トレーニングモードなどで回復した時用
	if (player->HPzan > redguage->value)
	{
		redguage->value = player->HPzan;
	}

	int x = cntPattern % REDGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / REDGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / REDGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / REDGUAGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	redguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - redguage->value) / (float)player->HP), (float)(y)* sizeY);
	redguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	redguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)(player->HP - redguage->value) / (float)player->HP), (float)(y)* sizeY + sizeY);
	redguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexRedGuage(void)
{
	PLAYER *player = GetPlayer(0);

	// 頂点座標の設定
	redguage->vertexWk[0].vtx = D3DXVECTOR3(redguage->pos.x + REDGUAGE_SIZE_X * ((float)(player->HP - redguage->value) / player->HP), redguage->pos.y, redguage->pos.z);
	redguage->vertexWk[1].vtx = D3DXVECTOR3(redguage->pos.x + REDGUAGE_SIZE_X, redguage->pos.y, redguage->pos.z);
	redguage->vertexWk[2].vtx = D3DXVECTOR3(redguage->pos.x + REDGUAGE_SIZE_X * ((float)(player->HP - redguage->value) / player->HP), redguage->pos.y + REDGUAGE_SIZE_Y, redguage->pos.z);
	redguage->vertexWk[3].vtx = D3DXVECTOR3(redguage->pos.x + REDGUAGE_SIZE_X, redguage->pos.y + REDGUAGE_SIZE_Y, redguage->pos.z);
}
