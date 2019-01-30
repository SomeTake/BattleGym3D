//=============================================================================
//
// SPゲージ用フレーム表示処理 [frame.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "framespgauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureFrameSpgauge = NULL;		// テクスチャへのポリゴン

FRAMESPGAUGE framespgauge[FRAMESPGAUGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFrameSpgauge(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_FRAMESPGAUGE00,				// ファイルの名前
			&g_pD3DTextureFrameSpgauge);				// 読み込むメモリのポインタ

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// フレームの初期化
	framespgauge->use = true;
	framespgauge->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	framespgauge->CountAnim = 0;
	framespgauge->PatternAnim = 0;

	// 頂点情報の作成
	MakeVertexFrameSpgauge();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFrameSpgauge(void)
{
	if (g_pD3DTextureFrameSpgauge != NULL)	//
	{	// テクスチャの開放
		g_pD3DTextureFrameSpgauge->Release();
		g_pD3DTextureFrameSpgauge = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFrameSpgauge(void)
{
	if (framespgauge->use == true)
	{
		// アニメーション
		framespgauge->CountAnim++;

		//アニメーションwaitチェック
		if ((framespgauge->CountAnim % TIME_ANIMATION_FRAMESPGAUGE) == 0)
		{
			//パターンの切り替え
			framespgauge->PatternAnim = (framespgauge->PatternAnim + 1) % ANIM_PATTERN_NUM_FRAMESPGAUGE;

			//テクスチャ座標をセット
			SetTextureFrameSpgauge(framespgauge->PatternAnim);
		}


	}
	SetVertexFrameSpgauge();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFrameSpgauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (framespgauge->use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureFrameSpgauge);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, framespgauge->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFrameSpgauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexFrameSpgauge();

	// rhwの設定
	framespgauge->vertexWk[0].rhw =
		framespgauge->vertexWk[1].rhw =
		framespgauge->vertexWk[2].rhw =
		framespgauge->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	framespgauge->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespgauge->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespgauge->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespgauge->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	framespgauge->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	framespgauge->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	framespgauge->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	framespgauge->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureFrameSpgauge(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAMESPGAUGE_Y;

	// テクスチャ座標の設定
	framespgauge->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	framespgauge->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	framespgauge->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	framespgauge->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexFrameSpgauge(void)
{
	// 頂点座標の設定
	framespgauge->vertexWk[0].vtx = D3DXVECTOR3(framespgauge->pos.x, framespgauge->pos.y, framespgauge->pos.z);
	framespgauge->vertexWk[1].vtx = D3DXVECTOR3(framespgauge->pos.x + TEXTURE_FRAMESPGAUGE00_SIZE_X, framespgauge->pos.y, framespgauge->pos.z);
	framespgauge->vertexWk[2].vtx = D3DXVECTOR3(framespgauge->pos.x, framespgauge->pos.y + TEXTURE_FRAMESPGAUGE00_SIZE_Y, framespgauge->pos.z);
	framespgauge->vertexWk[3].vtx = D3DXVECTOR3(framespgauge->pos.x + TEXTURE_FRAMESPGAUGE00_SIZE_X, framespgauge->pos.y + TEXTURE_FRAMESPGAUGE00_SIZE_Y, framespgauge->pos.z);
}

