//=============================================================================
//
// 2P用赤ゲージ画面処理 [eredgauge.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "redgauge.h"
#include "eredgauge.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureERedGauge = NULL;		// テクスチャへのポインタ

REDGAUGE eredgauge[EREDGAUGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitERedGauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CHARA *enemy = GetEnemy();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
			TEXTURE_EREDGAUGE,				// ファイルの名前
			&g_pD3DTextureERedGauge);		// 読み込むメモリー

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// ゲージの初期化
	eredgauge->use = true;
	eredgauge->pos = D3DXVECTOR3(EREDGAUGE_POS_X, EREDGAUGE_POS_Y, 0.0f);
	eredgauge->value = enemy->HPzan;

	// 頂点情報の作成
	MakeVertexERedGauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitERedGauge(void)
{
	if (g_pD3DTextureERedGauge != NULL)
	{// テクスチャの開放
		g_pD3DTextureERedGauge->Release();
		g_pD3DTextureERedGauge = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateERedGauge(void)
{
	if (eredgauge->use == true)
	{
		//赤ゲージ
		eredgauge->PatternAnim = 2;
		
		//テクスチャ座標をセット
		SetTextureERedGauge(eredgauge->PatternAnim);


		SetVertexERedGauge();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawERedGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (eredgauge->use == true)
	{

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureERedGauge);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, eredgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexERedGauge(void)
{

	// 頂点座標の設定
	SetVertexERedGauge();

	// テクスチャのパースペクティブコレクト用
	eredgauge->vertexWk[0].rhw =
		eredgauge->vertexWk[1].rhw =
		eredgauge->vertexWk[2].rhw =
		eredgauge->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	eredgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	eredgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	eredgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	eredgauge->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	eredgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	eredgauge->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureERedGauge(int cntPattern)
{
	CHARA *enemy = GetEnemy();

	if (eredgauge->value > enemy->HPzan)
	{
		eredgauge->value -= 5;
	}

	//トレーニングモードなどで回復した時用
	if (enemy->HPzan > eredgauge->value)
	{
		eredgauge->value = enemy->HPzan;
	}

	int x = cntPattern % EREDGAUGE_PATTERN_DIVIDE_X;
	int y = cntPattern / EREDGAUGE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / EREDGAUGE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / EREDGAUGE_PATTERN_DIVIDE_Y;

	// テクスチャ座標の設定
	eredgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	eredgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)eredgauge->value / (float)enemy->HP), (float)(y)* sizeY);
	eredgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	eredgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX * ((float)eredgauge->value / (float)enemy->HP), (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexERedGauge(void)
{
	CHARA *enemy = GetEnemy();

	// 頂点座標の設定
	eredgauge->vertexWk[0].vtx = D3DXVECTOR3(eredgauge->pos.x, eredgauge->pos.y, eredgauge->pos.z);
	eredgauge->vertexWk[1].vtx = D3DXVECTOR3(eredgauge->pos.x + EREDGAUGE_SIZE_X * ((float)eredgauge->value / (float)enemy->HP), eredgauge->pos.y, eredgauge->pos.z);
	eredgauge->vertexWk[2].vtx = D3DXVECTOR3(eredgauge->pos.x, eredgauge->pos.y + EREDGAUGE_SIZE_Y, eredgauge->pos.z);
	eredgauge->vertexWk[3].vtx = D3DXVECTOR3(eredgauge->pos.x + EREDGAUGE_SIZE_X * ((float)eredgauge->value / (float)enemy->HP), eredgauge->pos.y + EREDGAUGE_SIZE_Y, eredgauge->pos.z);
}

//=============================================================================
// 赤ゲージのエネミー側ゲッター
//=============================================================================
REDGAUGE *GetERedGauge(void)
{
	return &eredgauge[0];
}