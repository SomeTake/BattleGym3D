//=============================================================================
//
// 当たり判定用ボール処理 [ball.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "ball.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBall(int type, BALL *Ball, D3DXMATRIX Matrix, float Radius)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ポインタの初期化
	Ball->D3DTexture = NULL;
	Ball->Mesh = NULL;
	Ball->D3DXMatBuff = NULL;

	// 場所、大きさの設定
	Ball->pos.x = Matrix._41;
	Ball->pos.y = Matrix._42;
	Ball->pos.z = Matrix._43;
	Ball->rot = Ball->rot;
	Ball->scl = D3DXVECTOR3(Radius, Radius, Radius);

	if (type == 0)
	{
		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(BALL_XFILE,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&Ball->D3DXMatBuff,
			NULL,
			&Ball->NumMat,
			&Ball->Mesh)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitBall(BALL *Ball)
{
	// テクスチャの開放
	SAFE_RELEASE(Ball->D3DTexture);

	// メッシュの開放
	SAFE_RELEASE(Ball->Mesh);

	// マテリアルの開放
	SAFE_RELEASE(Ball->D3DXMatBuff);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBall(BALL *Ball, D3DXMATRIX Matrix)
{
	Ball->pos.x = Matrix._41;
	Ball->pos.y = Matrix._42;
	Ball->pos.z = Matrix._43;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBall(BALL *Ball)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&Ball->mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, Ball->scl.x, Ball->scl.y, Ball->scl.z);
	D3DXMatrixMultiply(&Ball->mtxWorld, &Ball->mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Ball->rot.y, Ball->rot.x, Ball->rot.z);
	D3DXMatrixMultiply(&Ball->mtxWorld, &Ball->mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, Ball->pos.x, Ball->pos.y, Ball->pos.z);
	D3DXMatrixMultiply(&Ball->mtxWorld, &Ball->mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &Ball->mtxWorld);

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)Ball->D3DXMatBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)Ball->NumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, Ball->D3DTexture);

		// 描画
		Ball->Mesh->DrawSubset(nCntMat);
	}

	{// マテリアルをデフォルトに戻す
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}