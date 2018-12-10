//=============================================================================
//
// ノックアウト表示処理 [knockout.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "knockout.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexKnockout(void);
void SetTextureKnockout(int cntPattern);
void SetVertexKnockout(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureKnockout = NULL;		// テクスチャへのポリゴン

KNOCKOUT knockout[1];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitKnockout(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_KNOCKOUT00,				// ファイルの名前
			&g_pD3DTextureKnockout);				// 読み込むメモリのポインタ

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// フレームの初期化
	knockout->use = true;
	knockout->pos = D3DXVECTOR3(KNOCKOUT_POS_X, KNOCKOUT_POS_Y, 0.0f);
	knockout->CountAnim = 0;
	knockout->PatternAnim = 0;

	// 頂点情報の作成
	MakeVertexKnockout();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitKnockout(void)
{
	if (g_pD3DTextureKnockout != NULL)	//
	{	// テクスチャの開放
		g_pD3DTextureKnockout->Release();
		g_pD3DTextureKnockout = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateKnockout(void)
{
	PLAYER *player = GetPlayer(0);
	ENEMY *enemy = GetEnemy(0);

	Stop_Sound(BGM_BATTLE, 0);

	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter押したら、ステージを切り替える
		SetPhase(PhaseResult);
		Play_Sound(SE_WINNER0, 1, 0);
	}
	// ゲームパッドで移動処理
	else if (IsButtonTriggered(0, BUTTON_M))
	{
		SetPhase(PhaseResult);
		Play_Sound(SE_WINNER0, 1, 0);
	}

	if (knockout->use == true)
	{
		//どちらかがKOされたら
		if(player->HPzan == 0 || enemy->HPzan == 0)
		{ 
			knockout->PatternAnim = 0;
		}
		//タイムアップなら
		else
		{
			knockout->PatternAnim = 1;
		}

		//テクスチャ座標をセット
		SetTextureKnockout(knockout->PatternAnim);

	}
	SetVertexKnockout();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawKnockout(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (knockout->use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureKnockout);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, knockout->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexKnockout(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexKnockout();

	// rhwの設定
	knockout->vertexWk[0].rhw =
		knockout->vertexWk[1].rhw =
		knockout->vertexWk[2].rhw =
		knockout->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	knockout->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	knockout->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	knockout->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	knockout->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	knockout->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	knockout->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	knockout->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	knockout->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureKnockout(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_Y;

	// テクスチャ座標の設定
	knockout->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	knockout->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	knockout->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	knockout->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexKnockout(void)
{
	// 頂点座標の設定
	knockout->vertexWk[0].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y, knockout->pos.z);
	knockout->vertexWk[1].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_KNOCKOUT00_SIZE_X, knockout->pos.y, knockout->pos.z);
	knockout->vertexWk[2].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y + TEXTURE_KNOCKOUT00_SIZE_Y, knockout->pos.z);
	knockout->vertexWk[3].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_KNOCKOUT00_SIZE_X, knockout->pos.y + TEXTURE_KNOCKOUT00_SIZE_Y, knockout->pos.z);
}

