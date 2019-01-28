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
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureKnockout = NULL;		// テクスチャへのポリゴン
static LPDIRECT3DTEXTURE9		g_pD3DTexturePushbutton = NULL;

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

		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_PUSHBUTTON00,
			&g_pD3DTexturePushbutton);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// フレームの初期化
	knockout->use = true;
	knockout->pos = D3DXVECTOR3(KNOCKOUT_POS_X, KNOCKOUT_POS_Y, 0.0f);
	knockout->CountAnim = 0;
	knockout->PatternAnim = 0;
	knockout->pushok = false;

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

	if (g_pD3DTexturePushbutton != NULL)
	{
		g_pD3DTexturePushbutton->Release();
		g_pD3DTexturePushbutton = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateKnockout(void)
{
	CHARA *player = GetPlayer();
	CHARA *enemy = GetEnemy();

	StopSound(BGM_BATTLE, 0);

	static int sceneframe = 0;
	static int pushframe = 0;

	// シーン遷移有効になるまでの時間
	if (sceneframe < POSSIBLE_FRAME)
	{
		sceneframe++;
	}
	
	if (sceneframe == POSSIBLE_FRAME)
	{
		knockout->pushok = true;
		knockout->pos = D3DXVECTOR3(PUSHBUTTON_POS_X, PUSHBUTTON_POS_Y, 0.0f);
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			SetPhase(PhaseResult);
			PlaySound(SE_WINNER0, 1, 0);
			sceneframe = 0;
			pushframe = 0;
		}
		// ゲームパッドで移動処理
		else if (IsButtonTriggered(0, BUTTON_M) || IsButtonTriggered(0, BUTTON_C) || IsButtonTriggered(1, BUTTON_M) || IsButtonTriggered(1, BUTTON_C))
		{
			SetPhase(PhaseResult);
			PlaySound(SE_WINNER0, 1, 0);
			sceneframe = 0;
			pushframe = 0;
		}
	}

	if (knockout->use == true)
	{
		if (knockout->pushok == true)
		{
			knockout->PatternAnim = 0;
			// 点滅させる
			pushframe == 60 ? pushframe = 0 : pushframe++;
			if (pushframe >= 30)
			{
				SetReflectKnockout(0.00f);
			}
			else
			{
				SetReflectKnockout(1.00f);
			}
		}
		else
		{
			//どちらかがKOされたら
			if (player->HPzan == 0 || enemy->HPzan == 0)
			{
				knockout->PatternAnim = 0;
			}
			//タイムアップなら
			else
			{
				knockout->PatternAnim = 1;
			}
			SetReflectKnockout(1.00f);
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
		if (knockout->pushok == false)
		{
			// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
			// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pD3DTextureKnockout);
		}
		else
		{
			pDevice->SetTexture(0, g_pD3DTexturePushbutton);
		}
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
	knockout->vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	knockout->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	knockout->vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureKnockout(int cntPattern)
{
	int x, y;
	float sizeX, sizeY;

	if (knockout->pushok == false)
	{
		x = cntPattern % TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
		y = cntPattern / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
		sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_X;
		sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_KNOCKOUT_Y;
	}
	else
	{
		x = cntPattern % TEXTURE_PATTERN_DIVIDE_PUSHBUTTON_X;
		y = cntPattern / TEXTURE_PATTERN_DIVIDE_PUSHBUTTON_X;
		sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_PUSHBUTTON_X;
		sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_PUSHBUTTON_Y;
	}

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
	if (knockout->pushok == false)
	{
		// 頂点座標の設定
		knockout->vertexWk[0].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y, knockout->pos.z);
		knockout->vertexWk[1].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_KNOCKOUT00_SIZE_X, knockout->pos.y, knockout->pos.z);
		knockout->vertexWk[2].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y + TEXTURE_KNOCKOUT00_SIZE_Y, knockout->pos.z);
		knockout->vertexWk[3].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_KNOCKOUT00_SIZE_X, knockout->pos.y + TEXTURE_KNOCKOUT00_SIZE_Y, knockout->pos.z);
	}
	else
	{
		knockout->vertexWk[0].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y, knockout->pos.z);
		knockout->vertexWk[1].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_PUSHBUTTON00_SIZE_X, knockout->pos.y, knockout->pos.z);
		knockout->vertexWk[2].vtx = D3DXVECTOR3(knockout->pos.x, knockout->pos.y + TEXTURE_PUSHBUTTON00_SIZE_Y, knockout->pos.z);
		knockout->vertexWk[3].vtx = D3DXVECTOR3(knockout->pos.x + TEXTURE_PUSHBUTTON00_SIZE_X, knockout->pos.y + TEXTURE_PUSHBUTTON00_SIZE_Y, knockout->pos.z);
	}
}

//=============================================================================
// 反射光の設定 引数:float per = 透明度の％
//=============================================================================
void SetReflectKnockout(float per)
{
	int clear = (int)(255 * per);

	// 反射光の設定
	knockout->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	knockout->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	knockout->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
	knockout->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, clear);
}

//=============================================================================
// ノックアウト構造体のゲッター
//=============================================================================
KNOCKOUT *GetKnockout(void)
{
	return &knockout[0];
}