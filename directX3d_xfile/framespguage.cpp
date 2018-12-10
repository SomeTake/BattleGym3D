//=============================================================================
//
// SPゲージ用フレーム表示処理 [frame.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "framespguage.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFrameSpguage(void);
void SetTextureFrameSpguage(int cntPattern);
void SetVertexFrameSpguage(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureFrameSpguage = NULL;		// テクスチャへのポリゴン

FRAMESPGUAGE framespguage[FRAMESPGUAGE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFrameSpguage(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_FRAMESPGUAGE00,				// ファイルの名前
			&g_pD3DTextureFrameSpguage);				// 読み込むメモリのポインタ

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// フレームの初期化
	framespguage->use = true;
	framespguage->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	framespguage->CountAnim = 0;
	framespguage->PatternAnim = 0;

	// 頂点情報の作成
	MakeVertexFrameSpguage();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFrameSpguage(void)
{
	if (g_pD3DTextureFrameSpguage != NULL)	//
	{	// テクスチャの開放
		g_pD3DTextureFrameSpguage->Release();
		g_pD3DTextureFrameSpguage = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFrameSpguage(void)
{
	if (framespguage->use == true)
	{
		// アニメーション
		framespguage->CountAnim++;

		//アニメーションwaitチェック
		if ((framespguage->CountAnim % TIME_ANIMATION_FRAMESPGUAGE) == 0)
		{
			//パターンの切り替え
			framespguage->PatternAnim = (framespguage->PatternAnim + 1) % ANIM_PATTERN_NUM_FRAMESPGUAGE;

			//テクスチャ座標をセット
			SetTextureFrameSpguage(framespguage->PatternAnim);
		}


	}
	SetVertexFrameSpguage();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFrameSpguage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (framespguage->use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureFrameSpguage);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, framespguage->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFrameSpguage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexFrameSpguage();

	// rhwの設定
	framespguage->vertexWk[0].rhw =
		framespguage->vertexWk[1].rhw =
		framespguage->vertexWk[2].rhw =
		framespguage->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	framespguage->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespguage->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespguage->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	framespguage->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	framespguage->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	framespguage->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	framespguage->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	framespguage->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureFrameSpguage(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAMESPGUAGE_Y;

	// テクスチャ座標の設定
	framespguage->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	framespguage->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	framespguage->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	framespguage->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexFrameSpguage(void)
{
	// 頂点座標の設定
	framespguage->vertexWk[0].vtx = D3DXVECTOR3(framespguage->pos.x, framespguage->pos.y, framespguage->pos.z);
	framespguage->vertexWk[1].vtx = D3DXVECTOR3(framespguage->pos.x + TEXTURE_FRAMESPGUAGE00_SIZE_X, framespguage->pos.y, framespguage->pos.z);
	framespguage->vertexWk[2].vtx = D3DXVECTOR3(framespguage->pos.x, framespguage->pos.y + TEXTURE_FRAMESPGUAGE00_SIZE_Y, framespguage->pos.z);
	framespguage->vertexWk[3].vtx = D3DXVECTOR3(framespguage->pos.x + TEXTURE_FRAMESPGUAGE00_SIZE_X, framespguage->pos.y + TEXTURE_FRAMESPGUAGE00_SIZE_Y, framespguage->pos.z);
}

