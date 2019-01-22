//=============================================================================
//
// 空処理 [skybox.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "skybox.h"
#include "player.h"
#include "camera.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSkyBox(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 SkyBoxVertexBuffer = NULL;			// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		SkyBoxTexture = NULL;
D3DXMATRIX				WorldMatrix_SkyBox;					// ワールドマトリックス
SKYBOX					skyWk;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSkyBox(int type)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 位置・回転・スケールの初期設定
	skyWk.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
	skyWk.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	skyWk.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// 初めて初期化
	if (type == 0)
	{
		// 頂点情報の作成
		if (FAILED(MakeVertexSkyBox()))
		{
			return E_FAIL;
		}

		// テクスチャの読み込み
		if (FAILED(LoadTexture(TEXTURE_SKYBOX, &SkyBoxTexture, "SkyBox")))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSkyBox(void)
{
	// テクスチャの開放
	SAFE_RELEASE(SkyBoxTexture);

	// 頂点バッファの開放
	SAFE_RELEASE(SkyBoxVertexBuffer);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSkyBox(void)
{
	// ちょっとずつ回転
	skyWk.rot.y += 0.0001f;

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSkyBox(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX ScaleMatrix, RotMatrix, TransMatrix;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&WorldMatrix_SkyBox);

	// スケールを反映
	D3DXMatrixScaling(&ScaleMatrix, skyWk.scl.x, skyWk.scl.y, skyWk.scl.z);
	D3DXMatrixMultiply(&WorldMatrix_SkyBox, &WorldMatrix_SkyBox, &ScaleMatrix);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&RotMatrix, skyWk.rot.y, skyWk.rot.x, skyWk.rot.z);
	D3DXMatrixMultiply(&WorldMatrix_SkyBox, &WorldMatrix_SkyBox, &RotMatrix);

	// 移動を反映
	D3DXMatrixTranslation(&TransMatrix, skyWk.pos.x, skyWk.pos.y, skyWk.pos.z);
	D3DXMatrixMultiply(&WorldMatrix_SkyBox, &WorldMatrix_SkyBox, &TransMatrix);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &WorldMatrix_SkyBox);

	// 頂点バッファをデバイスのデータストリームにバインド
	Device->SetStreamSource(0, SkyBoxVertexBuffer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, SkyBoxTexture);

	// ポリゴンの描画
	for (int i = 0; i < SURFACE_NUM; i++)
	{
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, NUM_POLYGON);
	}

	return;
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSkyBox(void)
{
	int i = 0;
	int x = 0, y = 0;
	float sizeX = 1.0f / TEXTURE_SKYBOX_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_SKYBOX_DIVIDE_Y;
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * SURFACE_NUM,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&SkyBoxVertexBuffer,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	// 頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	SkyBoxVertexBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定

	// 前
	pVtx[0].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[1].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[2].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	pVtx[3].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2));
	// 後ろ
	pVtx[4].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[5].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), (SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[6].vtx = D3DXVECTOR3((SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	pVtx[7].vtx = D3DXVECTOR3(-(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2), -(SKYBOX_LENGTH / 2));
	// 上
	pVtx[8].vtx = pVtx[5].vtx;
	pVtx[9].vtx = pVtx[4].vtx;
	pVtx[10].vtx = pVtx[0].vtx;
	pVtx[11].vtx = pVtx[1].vtx;
	// 右
	pVtx[12].vtx = pVtx[1].vtx;
	pVtx[13].vtx = pVtx[4].vtx;
	pVtx[14].vtx = pVtx[3].vtx;
	pVtx[15].vtx = pVtx[6].vtx;
	// 底
	pVtx[16].vtx = pVtx[2].vtx;
	pVtx[17].vtx = pVtx[3].vtx;
	pVtx[18].vtx = pVtx[7].vtx;
	pVtx[19].vtx = pVtx[6].vtx;
	// 左
	pVtx[20].vtx = pVtx[5].vtx;
	pVtx[21].vtx = pVtx[0].vtx;
	pVtx[22].vtx = pVtx[7].vtx;
	pVtx[23].vtx = pVtx[2].vtx;


	// 法線ベクトルの設定
	for (i = 0; i < NUM_VERTEX * SURFACE_NUM; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// 反射光の設定
	for (i = 0; i < NUM_VERTEX * SURFACE_NUM; i++)
	{
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標の設定

	// 前
	x = 1;
	y = 1;
	pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 後ろ
	x = 3;
	y = 1;
	pVtx[4].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[5].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[6].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[7].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 上
	x = 1;
	y = 0;
	pVtx[8].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[9].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[10].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[11].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 右
	x = 2;
	y = 1;
	pVtx[12].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[13].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[14].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[15].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 底
	x = 1;
	y = 2;
	pVtx[16].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[17].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[18].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[19].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	// 左
	x = 0;
	y = 1;
	pVtx[20].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[21].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + 0.001f);
	pVtx[22].tex = D3DXVECTOR2((float)(x)* sizeX + 0.001f, (float)(y)* sizeY + sizeY - 0.001f);
	pVtx[23].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX - 0.001f, (float)(y)* sizeY + sizeY - 0.001f);

	// 頂点データをアンロックする
	SkyBoxVertexBuffer->Unlock();

	return S_OK;
}
