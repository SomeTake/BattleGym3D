//=============================================================================
//
// スコア評価処理 [evaluation.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "evaluation.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "escore.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEvaluation(int no);
void SetTextureEvaluation(int no, int cntPattern);
void SetVertexEvaluation(int no);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureEvaluation = NULL;		// テクスチャへのポリゴン

EVALUATION evaluationWk[EVALUATION_MAX];				// 構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEvaluation(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EVALUATION *evaluation = evaluationWk;

	// テクスチャーの初期化を行う？
	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
			TEXTURE_GAME_EVALUATION,				// ファイルの名前
			&g_pD3DTextureEvaluation);			// 読み込むメモリのポインタ
	}

	// 初期化処理
	for (int i = 0; i < EVALUATION_MAX; i++, evaluation++)
	{
		evaluationWk->pos = D3DXVECTOR3(EVALUATION_POS_X, EVALUATION_POS_Y, 0.0f);	// 座標データを初期化
		evaluationWk->PatternAnim = 0;									// アニメパターン番号をランダムで初期化
		evaluationWk->CountAnim = 0;									// アニメカウントを初期化

		evaluationWk->Texture = g_pD3DTextureEvaluation;				// テクスチャ情報
		MakeVertexEvaluation(i);										// 頂点情報の作成
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEvaluation(void)
{
	if (g_pD3DTextureEvaluation != NULL)
	{	// テクスチャの開放
		g_pD3DTextureEvaluation->Release();
		g_pD3DTextureEvaluation = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEvaluation(void)
{
	EVALUATION *evaluation = evaluationWk;
	PLAYER *player = GetPlayer(0);
	ENEMY *enemy = GetEnemy(0);
	ESCORE *escore = GetEScore(0);
	SCORE *score = GetScore(0);

	for (int i = 0; i < EVALUATION_MAX; i++, evaluation++)
	{
		if (player->HPzan > enemy->HPzan)
		{
			//A評価
			if (score->pScore >= SCORE_A)
			{
				SetTextureEvaluation(i, 0);
			}
			//B評価
			else if (score->pScore < SCORE_A || score->pScore >= SCORE_B)
			{
				SetTextureEvaluation(i, 1);
			}
			//C評価
			else
			{
				SetTextureEvaluation(i, 2);
			}
		}
		else if (player->HPzan < enemy->HPzan)
		{
			//A評価
			if (escore->eScore >= SCORE_A)
			{
				SetTextureEvaluation(i, 0);
			}
			//B評価
			else if (escore->eScore < SCORE_A || escore->eScore >= SCORE_B)
			{
				SetTextureEvaluation(i, 1);
			}
			//C評価
			else
			{
				SetTextureEvaluation(i, 2);
			}

		}

		SetVertexEvaluation(i);	// 移動後の座標で頂点を設定
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEvaluation(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EVALUATION *evaluation = evaluationWk;

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < EVALUATION_MAX; i++, evaluation++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, evaluationWk->Texture);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, evaluationWk->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEvaluation(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EVALUATION *evaluation = &evaluationWk[no];

	// 頂点座標の設定
	SetVertexEvaluation(no);

	// rhwの設定
	evaluationWk->vertexWk[0].rhw =
		evaluationWk->vertexWk[1].rhw =
		evaluationWk->vertexWk[2].rhw =
		evaluationWk->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	evaluationWk->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	evaluationWk->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	evaluationWk->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	evaluationWk->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	evaluationWk->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	evaluationWk->vertexWk[1].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_EVALUATION, 0.0f);
	evaluationWk->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EVALUATION);
	evaluationWk->vertexWk[3].tex = D3DXVECTOR2(1.0f / TEXTURE_PATTERN_DIVIDE_X_EVALUATION, 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EVALUATION);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureEvaluation(int no, int cntPattern)
{
	EVALUATION *evaluation = &evaluationWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_EVALUATION;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_EVALUATION;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EVALUATION;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EVALUATION;

	evaluationWk->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	evaluationWk->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	evaluationWk->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	evaluationWk->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexEvaluation(int no)
{
	EVALUATION *evaluation = &evaluationWk[no];

	// 頂点座標の設定
	evaluationWk->vertexWk[0].vtx = D3DXVECTOR3(evaluationWk->pos.x, evaluationWk->pos.y, evaluationWk->pos.z);
	evaluationWk->vertexWk[1].vtx = D3DXVECTOR3(evaluationWk->pos.x + TEXTURE_EVALUATION_SIZE_X, evaluationWk->pos.y, evaluationWk->pos.z);
	evaluationWk->vertexWk[2].vtx = D3DXVECTOR3(evaluationWk->pos.x, evaluationWk->pos.y + TEXTURE_EVALUATION_SIZE_Y, evaluationWk->pos.z);
	evaluationWk->vertexWk[3].vtx = D3DXVECTOR3(evaluationWk->pos.x + TEXTURE_EVALUATION_SIZE_X, evaluationWk->pos.y + TEXTURE_EVALUATION_SIZE_Y, evaluationWk->pos.z);
}

//=============================================================================
// エネミー取得関数
//=============================================================================
EVALUATION *GetEvaluation(int no)
{
	return(&evaluationWk[no]);
}