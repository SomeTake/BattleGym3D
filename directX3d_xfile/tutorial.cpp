//=============================================================================
//
// チュートリアル処理 [evaluation.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureTutorial = NULL;		// テクスチャへのポリゴン

TUTORIAL tutorialWk[TUTORIAL_MAX];						// 構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_TUTORIAL,				// ファイルの名前
			&g_pD3DTextureTutorial);			// 読み込むメモリのポインタ
	}

	// 初期化処理
	tutorialWk->pos = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y, 0.0f);	// 座標データを初期化
	tutorialWk->PatternAnim = 0;									// アニメパターン番号をランダムで初期化
	tutorialWk->CountAnim = 0;									// アニメカウントを初期化

	tutorialWk->Texture = g_pD3DTextureTutorial;				// テクスチャ情報
	MakeVertexTutorial();										// 頂点情報の作成


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{
	if (g_pD3DTextureTutorial != NULL)
	{	// テクスチャの開放
		g_pD3DTextureTutorial->Release();
		g_pD3DTextureTutorial = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{
	if (GetKeyboardPress(DIK_SPACE) || IsButtonPressed(0, BUTTON_Z))
	{
		SetTextureTutorial(1);
	}
	else
	{
		SetTextureTutorial(0);
	}

	SetVertexTutorial();	// 移動後の座標で頂点を設定

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, tutorialWk->Texture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, tutorialWk->vertexWk, sizeof(VERTEX_2D));
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexTutorial();

	// rhwの設定
	tutorialWk->vertexWk[0].rhw =
		tutorialWk->vertexWk[1].rhw =
		tutorialWk->vertexWk[2].rhw =
		tutorialWk->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	tutorialWk->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	tutorialWk->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	tutorialWk->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);
	tutorialWk->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 200);

	// テクスチャ座標の設定
	tutorialWk->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	tutorialWk->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_TUTORIAL, 0.0f);
	tutorialWk->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL);
	tutorialWk->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_TUTORIAL, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTutorial(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_TUTORIAL;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_TUTORIAL;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_TUTORIAL;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_TUTORIAL;

	tutorialWk->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	tutorialWk->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	tutorialWk->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	tutorialWk->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexTutorial(void)
{
	// 頂点座標の設定
	tutorialWk->vertexWk[0].vtx = D3DXVECTOR3(tutorialWk->pos.x, tutorialWk->pos.y, tutorialWk->pos.z);
	tutorialWk->vertexWk[1].vtx = D3DXVECTOR3(tutorialWk->pos.x + TEXTURE_TUTORIAL_SIZE_X, tutorialWk->pos.y, tutorialWk->pos.z);
	tutorialWk->vertexWk[2].vtx = D3DXVECTOR3(tutorialWk->pos.x, tutorialWk->pos.y + TEXTURE_TUTORIAL_SIZE_Y, tutorialWk->pos.z);
	tutorialWk->vertexWk[3].vtx = D3DXVECTOR3(tutorialWk->pos.x + TEXTURE_TUTORIAL_SIZE_X, tutorialWk->pos.y + TEXTURE_TUTORIAL_SIZE_Y, tutorialWk->pos.z);
}
