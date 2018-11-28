//=============================================================================
//
// スコア処理 [score.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "score.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexScore(int sno);
void SetTextureScore(int cntPattern, int sno);
void SetVertexScore(int sno);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureScore = NULL;		// テクスチャへのポリゴン

SCORE score[SCORE_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitScore(int type)
{
	int i;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCORE *score = GetScore(0);

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_SCORE00,				// ファイルの名前
			&g_pD3DTextureScore);				// 読み込むメモリのポインタ

	}

	for (i = 0; i < SCORE_MAX; i++, score++)
	{
		score->use = true;
		score->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		score->CountAnim = 0;
		score->PatternAnim = 0;

		score->pScore = 0;

		// 頂点情報の作成
		MakeVertexScore(i);

	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	int i;
	SCORE *score = GetScore(0);

	for (i = 0; i < SCORE_MAX; i++, score++)
	{
		if (g_pD3DTextureScore != NULL)	//
		{	// テクスチャの開放
			g_pD3DTextureScore->Release();
			g_pD3DTextureScore = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	int i;
	SCORE *score = GetScore(0);

	int kariScore[SCORE_MAX];
	int num = score->pScore;
	
	//スコアを一桁ずつバラす
	for (i = 0; i < SCORE_MAX; i++)
	{
		kariScore[i] = num % 10;
		num = num / 10;
	}

	for (i = 0; i < SCORE_MAX; i++, score++)
	{
		if (score->use == true)
		{
			// アニメーション
			score->CountAnim++;

			//アニメーションwaitチェック
			if ((score->CountAnim % TIME_ANIMATION_SCORE) == 0)
			{
				//パターンの切り替え
				score->PatternAnim = kariScore[SCORE_MAX - i - 1];

				//テクスチャ座標をセット
				SetTextureScore(score->PatternAnim, i);
			}


		}
		SetVertexScore(i);
	}

	if (GetKeyboardPress(DIK_SPACE))
	{
		AddScore(1);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int i;
	SCORE *score = GetScore(0);
	for (i = 0; i < SCORE_MAX; i++, score++)
	{
		if (score->use == true)
		{
			// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
			// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureScore);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, score->vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexScore(int sno)
{
	SCORE *score = GetScore(sno);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexScore(sno);

	// rhwの設定
	score->vertexWk[0].rhw =
		score->vertexWk[1].rhw =
		score->vertexWk[2].rhw =
		score->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	score->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	score->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	score->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	score->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	score->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	score->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureScore(int cntPattern, int sno)
{
	SCORE *score = GetScore(sno);
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_SCORE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_SCORE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_SCORE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_SCORE_Y;

	// テクスチャ座標の設定
	score->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	score->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	score->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	score->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexScore(int sno)
{
	SCORE *score = GetScore(sno);
	// 頂点座標の設定

	score->vertexWk[0].vtx = D3DXVECTOR3((float)SCORE_POS_X + (TEXTURE_SCORE00_SIZE_X * sno), (float)SCORE_POS_Y, score->pos.z);
	score->vertexWk[1].vtx = D3DXVECTOR3((float)SCORE_POS_X + (TEXTURE_SCORE00_SIZE_X * (sno + 1)), (float)SCORE_POS_Y, score->pos.z);
	score->vertexWk[2].vtx = D3DXVECTOR3((float)SCORE_POS_X + (TEXTURE_SCORE00_SIZE_X * sno), (float)SCORE_POS_Y + TEXTURE_SCORE00_SIZE_Y, score->pos.z);
	score->vertexWk[3].vtx = D3DXVECTOR3((float)SCORE_POS_X + (TEXTURE_SCORE00_SIZE_X * (sno + 1)), (float)SCORE_POS_Y + TEXTURE_SCORE00_SIZE_Y, score->pos.z);
}

//=============================================================================
//スコアの情報を取得する
//=============================================================================
SCORE *GetScore(int sno)
{
	return &score[sno];
}

//=============================================================================
// スコアの追加
// 引数:add :追加する点数。マイナスも可能、初期化などに。
//=============================================================================
void AddScore(int add)
{
	SCORE *score = GetScore(0);

	score->pScore += add;

	//カンスト処理
	if (score->pScore > FULL_SCORE)
	{
		score->pScore = FULL_SCORE;
	}
	else if (score->pScore < 0)
	{
		score->pScore = 0;
	}

}
