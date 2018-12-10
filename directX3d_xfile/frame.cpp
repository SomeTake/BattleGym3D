//=============================================================================
//
// 枠表示処理 [frame.cpp]
// Author : HAL東京 GP11B341 17 染谷武志
//
//=============================================================================
#include "main.h"
#include "frame.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexFrame(void);
void SetTextureFrame(int cntPattern );
void SetVertexFrame(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTextureFrame = NULL;		// テクスチャへのポリゴン

FRAME frame[FRAME_MAX];

//ゲージを揺らす
int fcount = 0;
int fcounttable[SWAY_MAX] = {
	0, 2, -2, 4, -4, 8, -8, 16, -16, 32, -32, 16, -16, 8, -8, 4, -4, 2, -2, 0,
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitFrame(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (type == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,		// デバイスのポインタ
			TEXTURE_GAME_FRAME00,				// ファイルの名前
			&g_pD3DTextureFrame);				// 読み込むメモリのポインタ

	}

	///////////////////////////////////////////////////////////////////////////////////////
	// フレームの初期化
	frame->use = true;
	frame->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	frame->CountAnim = 0;
	frame->PatternAnim = 0;
	frame->sway = false;
	
	// 頂点情報の作成
	MakeVertexFrame();
	///////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFrame(void)
{
	if (g_pD3DTextureFrame != NULL)	//
	{	// テクスチャの開放
		g_pD3DTextureFrame->Release();
		g_pD3DTextureFrame = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateFrame(void)
{
	if (frame->use == true)
	{
		// アニメーション
		frame->CountAnim++;

		//アニメーションwaitチェック
		if ((frame->CountAnim % TIME_ANIMATION_FRAME) == 0)
		{
			//パターンの切り替え
			frame->PatternAnim = (frame->PatternAnim + 1) % ANIM_PATTERN_NUM_FRAME;

			//テクスチャ座標をセット
			SetTextureFrame(frame->PatternAnim);
		}

		//揺らすとき
		if (frame->sway == true)
		{
			fcount++;
			if (fcount == SWAY_MAX)
			{
				fcount = 0;
				frame->sway = false;
			}
		}
		else
		{
			fcount = 0;
		}
	}
	SetVertexFrame();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (frame->use == true)
	{
		// テクスチャの設定(ポリゴンの描画前に読み込んだテクスチャのセットを行う)
		// テクスチャのセットをしないと前にセットされたテクスチャが貼られる→何もはらないことを指定するpDevide->SetTexture(0, NULL);
		pDevice->SetTexture(0, g_pD3DTextureFrame);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, frame->vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点座標の設定
	SetVertexFrame();

	// rhwの設定
	frame->vertexWk[0].rhw =
		frame->vertexWk[1].rhw =
		frame->vertexWk[2].rhw =
		frame->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	frame->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	frame->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	frame->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	frame->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	frame->vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	frame->vertexWk[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	frame->vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	frame->vertexWk[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureFrame(int cntPattern)
{
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_FRAME_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_FRAME_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAME_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_FRAME_Y;

	// テクスチャ座標の設定
	frame->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	frame->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	frame->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	frame->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexFrame(void)
{
	int fno = fcount % 18;

	// 頂点座標の設定
	frame->vertexWk[0].vtx = D3DXVECTOR3(frame->pos.x, fcounttable[fno] + frame->pos.y, frame->pos.z);
	frame->vertexWk[1].vtx = D3DXVECTOR3(frame->pos.x + TEXTURE_FRAME00_SIZE_X, fcounttable[fno] + frame->pos.y, frame->pos.z);
	frame->vertexWk[2].vtx = D3DXVECTOR3(frame->pos.x, fcounttable[fno] + frame->pos.y + TEXTURE_FRAME00_SIZE_Y, frame->pos.z);
	frame->vertexWk[3].vtx = D3DXVECTOR3(frame->pos.x + TEXTURE_FRAME00_SIZE_X, fcounttable[fno] + frame->pos.y + TEXTURE_FRAME00_SIZE_Y, frame->pos.z);
}

//=============================================================================
// フレームを揺らすためのフラグ管理
//=============================================================================
void FrameSway(void)
{
	frame->sway = true;
}