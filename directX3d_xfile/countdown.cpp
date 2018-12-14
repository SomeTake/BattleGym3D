//=============================================================================
//
// カウントダウン処理 [countdown.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "countdown.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureCountdown = NULL;		// テクスチャへのポリゴン

int CountdownSecond;												// 経過秒数
int CountdownFrame;													// カウントダウン中のフレーム経過

COUNTDOWN countdown[COUNTDOWN_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCountdown(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,			// デバイスのポインタ
			TEXTURE_GAME_COUNTDOWN00,				// ファイルの名前
			&g_pD3DTextureCountdown);				// 読み込むメモリのポインタ

	}

	//秒数とフレーム数のリセット
	CountdownSecond = 0;
	CountdownFrame = 0;

	countdown->use = true;
	countdown->pos = D3DXVECTOR3(COUNTDOWN_POS_X, COUNTDOWN_POS_Y, 0.0f);
	countdown->CountAnim = 0;
	countdown->PatternAnim = 0;

	// 頂点情報の作成
	MakeVertexCountdown();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCountdown(void)
{
	if (g_pD3DTextureCountdown != NULL)	//
	{	// テクスチャの開放
		g_pD3DTextureCountdown->Release();
		g_pD3DTextureCountdown = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCountdown(void)
{
	static bool flag = false;

	//カウントダウン音
	if (flag == false)
	{
		PlaySound(SE_COUNTDOWN, 0, 0);
		flag = true;
	}

	//60フレームで1秒経過させる
	CountdownFrame++;
	CountdownSecond = CountdownFrame / 60;

	if (countdown->use == true)
	{
		// アニメーション
		countdown->CountAnim++;

		//アニメーションwaitチェック
		if ((countdown->CountAnim % TIME_ANIMATION_COUNTDOWN) == 0)
		{
			//パターンの切り替え
			countdown->PatternAnim = CountdownSecond;

			//テクスチャ座標をセット
			SetTextureCountdown(countdown->PatternAnim);
		}

	}
	SetVertexCountdown();

	// 秒数が0になったらバトル開始
	if (CountdownSecond > FULL_COUNTDOWN - 1)
	{
		PlaySound(BGM_BATTLE, 1, 1);
		SetPhase(PhaseGame);
		flag = false;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (countdown->use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureCountdown);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, countdown->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCountdown(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexCountdown();

	// rhwの設定
	countdown->vertexWk[0].rhw =
		countdown->vertexWk[1].rhw =
		countdown->vertexWk[2].rhw =
		countdown->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	countdown->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	countdown->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	countdown->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	countdown->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	countdown->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	countdown->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	countdown->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	countdown->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureCountdown(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_COUNTDOWN_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_COUNTDOWN_Y;

	// テクスチャ座標の設定
	countdown->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	countdown->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	countdown->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	countdown->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexCountdown(void)
{
	// 頂点座標の設定
	countdown->vertexWk[0].vtx = D3DXVECTOR3(countdown->pos.x + (TEXTURE_COUNTDOWN00_SIZE_X), countdown->pos.y, countdown->pos.z);
	countdown->vertexWk[1].vtx = D3DXVECTOR3(countdown->pos.x + (TEXTURE_COUNTDOWN00_SIZE_X), countdown->pos.y, countdown->pos.z);
	countdown->vertexWk[2].vtx = D3DXVECTOR3(countdown->pos.x + (TEXTURE_COUNTDOWN00_SIZE_X), countdown->pos.y + TEXTURE_COUNTDOWN00_SIZE_Y, countdown->pos.z);
	countdown->vertexWk[3].vtx = D3DXVECTOR3(countdown->pos.x + (TEXTURE_COUNTDOWN00_SIZE_X), countdown->pos.y + TEXTURE_COUNTDOWN00_SIZE_Y, countdown->pos.z);
}