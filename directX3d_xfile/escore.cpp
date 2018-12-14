//=============================================================================
//
// エネミーのスコア処理 [escore.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "escore.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureEScore = NULL;		// テクスチャへのポリゴン

ESCORE EScore[ESCORE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEScore(int type)
{
	int i;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ESCORE *escore = GetEScore(0);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_ESCORE00,				// ファイルの名前
			&g_pD3DTextureEScore);				// 読み込むメモリのポインタ

	}

	for (i = 0; i < ESCORE_MAX; i++, escore++)
	{
		escore->use = true;
		escore->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		escore->CountAnim = 0;
		escore->PatternAnim = 0;

		escore->eScore = 0;

		// 頂点情報の作成
		MakeVertexEScore(i);

	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEScore(void)
{
	int i;
	ESCORE *escore = GetEScore(0);

	for (i = 0; i < ESCORE_MAX; i++, escore++)
	{
		if (g_pD3DTextureEScore != NULL)	//
		{	// テクスチャの開放
			g_pD3DTextureEScore->Release();
			g_pD3DTextureEScore = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEScore(void)
{
	int i;
	ESCORE *escore = GetEScore(0);

	int kariScore[ESCORE_MAX];
	int num = escore->eScore;

	//スコアを一桁ずつバラす
	for (i = 0; i < ESCORE_MAX; i++)
	{
		kariScore[i] = num % 10;
		num = num / 10;
	}

	for (i = 0; i < ESCORE_MAX; i++, escore++)
	{
		if (escore->use == true)
		{
			// アニメーション
			escore->CountAnim++;

			//アニメーションwaitチェック
			if ((escore->CountAnim % TIME_ANIMATION_ESCORE) == 0)
			{
				//パターンの切り替え
				escore->PatternAnim = kariScore[ESCORE_MAX - i - 1];

				//テクスチャ座標をセット
				SetTextureEScore(escore->PatternAnim, i);
			}


		}
		SetVertexEScore(i);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int i;
	ESCORE *escore = GetEScore(0);
	for (i = 0; i < ESCORE_MAX; i++, escore++)
	{
		if (escore->use == true)
		{
			// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
			// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureEScore);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, escore->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEScore(int sno)
{
	ESCORE *escore = GetEScore(sno);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexEScore(sno);

	// rhwの設定
	escore->vertexWk[0].rhw =
		escore->vertexWk[1].rhw =
		escore->vertexWk[2].rhw =
		escore->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	escore->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	escore->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	escore->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	escore->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	escore->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	escore->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	escore->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	escore->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEScore(int cntPattern, int sno)
{
	ESCORE *escore = GetEScore(sno);
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_ESCORE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_ESCORE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_ESCORE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_ESCORE_Y;

	// テクスチャ座標の設定
	escore->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	escore->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	escore->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	escore->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEScore(int sno)
{
	ESCORE *escore = GetEScore(sno);

	int phase = *GetPhase();

	//頂点座標の設定
	if (phase == PhaseCountdown || phase == PhaseGame || phase == PhaseFinish)
	{
		escore->vertexWk[0].vtx = D3DXVECTOR3((float)ESCORE_POS_X + (TEXTURE_ESCORE00_SIZE_X * sno), (float)ESCORE_POS_Y, escore->pos.z);
		escore->vertexWk[1].vtx = D3DXVECTOR3((float)ESCORE_POS_X + (TEXTURE_ESCORE00_SIZE_X * (sno + 1)), (float)ESCORE_POS_Y, escore->pos.z);
		escore->vertexWk[2].vtx = D3DXVECTOR3((float)ESCORE_POS_X + (TEXTURE_ESCORE00_SIZE_X * sno), (float)ESCORE_POS_Y + TEXTURE_ESCORE00_SIZE_Y, escore->pos.z);
		escore->vertexWk[3].vtx = D3DXVECTOR3((float)ESCORE_POS_X + (TEXTURE_ESCORE00_SIZE_X * (sno + 1)), (float)ESCORE_POS_Y + TEXTURE_ESCORE00_SIZE_Y, escore->pos.z);
	}
	else if (phase == PhaseResult)
	{
		escore->vertexWk[0].vtx = D3DXVECTOR3((float)RESULT_ESCORE_POS_X + (RESULT_ESCORE00_SIZE_X * sno), (float)RESULT_ESCORE_POS_Y, escore->pos.z);
		escore->vertexWk[1].vtx = D3DXVECTOR3((float)RESULT_ESCORE_POS_X + (RESULT_ESCORE00_SIZE_X * (sno + 1)), (float)RESULT_ESCORE_POS_Y, escore->pos.z);
		escore->vertexWk[2].vtx = D3DXVECTOR3((float)RESULT_ESCORE_POS_X + (RESULT_ESCORE00_SIZE_X * sno), (float)RESULT_ESCORE_POS_Y + RESULT_ESCORE00_SIZE_Y, escore->pos.z);
		escore->vertexWk[3].vtx = D3DXVECTOR3((float)RESULT_ESCORE_POS_X + (RESULT_ESCORE00_SIZE_X * (sno + 1)), (float)RESULT_ESCORE_POS_Y + RESULT_ESCORE00_SIZE_Y, escore->pos.z);
	}
}

//=============================================================================
//スコアの情報を取得する
//=============================================================================
ESCORE *GetEScore(int sno)
{
	return &EScore[sno];
}

//=============================================================================
// スコアの追加
// 引数:add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddEScore(int add)
{
	ESCORE *escore = GetEScore(0);
	escore->eScore += add;

	//カンスト処理
	if (escore->eScore > FULL_ESCORE)
	{
		escore->eScore = FULL_ESCORE;
	}
	else if (escore->eScore < 0)
	{
		escore->eScore = 0;
	}

}
