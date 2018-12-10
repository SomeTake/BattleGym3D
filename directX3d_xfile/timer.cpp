//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "timer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTimer(int tno);
void SetTextureTimer(int cntPattern, int tno);
void SetVertexTimer(int tno);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureTimer = NULL;		// テクスチャへのポリゴン

int second;														// 経過秒数
int TimerFrame;													// バトル中のフレーム経過

TIMER timer[TIMER_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimer(int type)
{
	int i;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	TIMER *timer = GetTimer(0);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_TIMER00,				// ファイルの名前
			&g_pD3DTextureTimer);				// 読み込むメモリのポインタ

	}

	//秒数とフレーム数のリセット
	second = 0;
	TimerFrame = 0;

	for (i = 0; i < TIMER_MAX; i++, timer++)
	{
		timer->use = true;
		timer->pos = D3DXVECTOR3(TIMER_POS_X, TIMER_POS_Y, 0.0f);
		timer->CountAnim = 0;
		timer->PatternAnim = 0;

		// 頂点情報の作成
		MakeVertexTimer(i);

	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	int i;
	TIMER *timer = GetTimer(0);

	for (i = 0; i < TIMER_MAX; i++, timer++)
	{
		if (g_pD3DTextureTimer != NULL)	//
		{	// テクスチャの開放
			g_pD3DTextureTimer->Release();
			g_pD3DTextureTimer = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{
	int i;
	TIMER *timer = GetTimer(0);

	int phase = *GetPhase();

	//60フレームで1秒経過させる
	if (phase == PhaseGame)
	{
		TimerFrame++;
	}
	second = TimerFrame / 60;

	int kariTimer[TIMER_MAX];
	int num = FULL_TIMER - second;
	
	// 秒数が0になったらバトル終了
	if (num < 0)
	{
		num = 0;
		SetPhase(PhaseFinish);
	}

	//タイムを一桁ずつバラす
	for (i = 0; i < TIMER_MAX; i++)
	{
		kariTimer[i] = num % 10;
		num = num / 10;
	}

	for (i = 0; i < TIMER_MAX; i++, timer++)
	{
		if (timer->use == true)
		{
			// アニメーション
			timer->CountAnim++;

			//アニメーションwaitチェック
			if ((timer->CountAnim % TIME_ANIMATION_TIMER) == 0)
			{
				//パターンの切り替え
				timer->PatternAnim = kariTimer[TIMER_MAX - i - 1];

				//テクスチャ座標をセット
				SetTextureTimer(timer->PatternAnim, i);
			}


		}
		SetVertexTimer(i);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int i;
	TIMER *timer = GetTimer(0);
	for (i = 0; i < TIMER_MAX; i++, timer++)
	{
		if (timer->use == true)
		{
			// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
			// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureTimer);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, timer->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTimer(int tno)
{
	TIMER *timer = GetTimer(tno);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexTimer(tno);

	// rhwの設定
	timer->vertexWk[0].rhw =
		timer->vertexWk[1].rhw =
		timer->vertexWk[2].rhw =
		timer->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	timer->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	timer->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	timer->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	timer->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	timer->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	timer->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureTimer(int cntPattern, int tno)
{
	TIMER *timer = GetTimer(tno);
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_TIMER_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_TIMER_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_TIMER_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_TIMER_Y;

	// テクスチャ座標の設定
	timer->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	timer->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	timer->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	timer->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexTimer(int tno)
{
	TIMER *timer = GetTimer(tno);
	// 頂点座標の設定
	timer->vertexWk[0].vtx = D3DXVECTOR3(timer->pos.x + (TEXTURE_TIMER00_SIZE_X * tno), timer->pos.y, timer->pos.z);
	timer->vertexWk[1].vtx = D3DXVECTOR3(timer->pos.x + (TEXTURE_TIMER00_SIZE_X * (tno + 1)), timer->pos.y, timer->pos.z);
	timer->vertexWk[2].vtx = D3DXVECTOR3(timer->pos.x + (TEXTURE_TIMER00_SIZE_X * tno), timer->pos.y + TEXTURE_TIMER00_SIZE_Y, timer->pos.z);
	timer->vertexWk[3].vtx = D3DXVECTOR3(timer->pos.x + (TEXTURE_TIMER00_SIZE_X * (tno + 1)), timer->pos.y + TEXTURE_TIMER00_SIZE_Y, timer->pos.z);
}

//=============================================================================
//タイマーの情報を取得する
//=============================================================================
TIMER *GetTimer(int tno)
{
	return &timer[tno];
}
