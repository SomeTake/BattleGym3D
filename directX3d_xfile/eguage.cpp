//=============================================================================
//
// 2P用HPゲージ画面処理 [eguage.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "eguage.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEGuage = NULL;		// テクスチャへのポインタ

EGUAGE eguage[EGUAGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEGuage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_EGUAGE,				// ファイルの名前
			&g_pD3DTextureEGuage);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	eguage->use = true;
	eguage->pos = D3DXVECTOR3(EGUAGE_POS_X, EGUAGE_POS_Y, 0.0f);

	// 頂点情報の作成
	MakeVertexEGuage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEGuage(void)
{
	if (g_pD3DTextureEGuage != NULL)
	{// テクスチャの開放
		g_pD3DTextureEGuage->Release();
		g_pD3DTextureEGuage = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEGuage(void)
{
	ENEMY *enemy = GetEnemy(0);

	if (eguage->use == true)
	{
		//HP満タンの場合
		if (enemy->HPzan == enemy->HP)
		{
			eguage->PatternAnim = 0;
		}
		//100%未満の場合
		else if (enemy->HPzan < enemy->HP)
		{
			eguage->PatternAnim = 1;
		}

		//テクスチャ座標をセット
		SetTextureEGuage(eguage->PatternAnim);


		SetVertexEGuage();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEGuage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (eguage->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureEGuage);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, eguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEGuage(void)
{

	// 頂点座標の設定
	SetVertexEGuage();

	// テクスチャのパースペクティブコレクト用
	eguage->vertexWk[0].rhw =
		eguage->vertexWk[1].rhw =
		eguage->vertexWk[2].rhw =
		eguage->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	eguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	eguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	eguage->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	eguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	eguage->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEGuage(int cntPattern)
{
	ENEMY *enemy = GetEnemy(0);

	int x = cntPattern % EGUAGE_PATTERN_DIVIDE_X;
	int y = cntPattern / EGUAGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / EGUAGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / EGUAGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	eguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	eguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)enemy->HPzan / (float)enemy->HP), (float)(y)* sizeY);
	eguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	eguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)enemy->HPzan / (float)enemy->HP), (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEGuage(void)
{
	ENEMY *enemy = GetEnemy(0);

	// 頂点座標の設定
	eguage->vertexWk[0].vtx = D3DXVECTOR3(eguage->pos.x, eguage->pos.y, eguage->pos.z);
	eguage->vertexWk[1].vtx = D3DXVECTOR3(eguage->pos.x + EGUAGE_SIZE_X * ((float)enemy->HPzan / (float)enemy->HP), eguage->pos.y, eguage->pos.z);
	eguage->vertexWk[2].vtx = D3DXVECTOR3(eguage->pos.x, eguage->pos.y + EGUAGE_SIZE_Y, eguage->pos.z);
	eguage->vertexWk[3].vtx = D3DXVECTOR3(eguage->pos.x + EGUAGE_SIZE_X * ((float)enemy->HPzan / (float)enemy->HP), eguage->pos.y + EGUAGE_SIZE_Y, eguage->pos.z);
}
