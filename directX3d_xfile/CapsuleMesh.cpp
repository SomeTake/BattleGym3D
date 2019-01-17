//=============================================================================
//
// 当たり判定用カプセル処理 [CapusuleMesh.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "CapsuleMesh.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetVertex(VERTEX_3D*& verticle, const D3DXVECTOR3& o, const D3DXVECTOR3& v);
HRESULT MakeVertexCapsule(CAPSULE *Capsule);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CreateCapsule(CAPSULE *Capsule, D3DXVECTOR3 Pos, D3DXVECTOR3 CreateVec, float Length, float Radius, bool DrawInCenter)
{
	// 位置・回転・スケールの初期設定
	Capsule->pos = Pos;
	Capsule->CreateVec = CreateVec;
	Capsule->Length = Length;
	Capsule->Radius = Radius;
	Capsule->NumOfVtxBuffer = 0;
	Capsule->NumOfTriangle = 0;
	if (DrawInCenter == true)
	{
		// カプセル中心からP1、P2を設置
		Capsule->P1 = Capsule->pos + Capsule->CreateVec * Capsule->Length / 2;
		Capsule->P2 = Capsule->pos - Capsule->CreateVec * Capsule->Length / 2;
	}
	else
	{
		// カプセルのある一端からP1、P2を設置
		Capsule->P1 = Capsule->pos;
		Capsule->P2 = Capsule->pos - Capsule->CreateVec * Capsule->Length;
	}

	// 頂点情報の作成
	if (FAILED(MakeVertexCapsule(Capsule)))
	{
		return E_FAIL;
	}

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitCapsule(CAPSULE *Capsule)
{
	// 頂点バッファの開放
	SAFE_RELEASE(Capsule->VtxBuffer);

	// インデックスバッファの開放
	SAFE_RELEASE(Capsule->IdxBuffer);

	return;
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateCapsule(CAPSULE *Capsule)
{

	return;
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawCapsule(CAPSULE *Capsule, const D3DXMATRIX *WorldMatrix)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	// カプセルを回転する角度
	float RotDegree = 0.0f;
	// カプセルの回転軸
	D3DXVECTOR3 RotAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&Capsule->WorldMatrix);

	// スケールを反映
	//D3DXMatrixScaling(&mtxScl, Capsule->Scale.x, Capsule->Scale.y, Capsule->Scale.z);
	//D3DXMatrixMultiply(&Capsule->WorldMatrix, &Capsule->WorldMatrix, &mtxScl);

	// ボーンマトリクスから、ボーンのベクトルを取得
	Capsule->vec.x = WorldMatrix->_31;
	Capsule->vec.y = WorldMatrix->_32;
	Capsule->vec.z = WorldMatrix->_33;
	// 正規化
	D3DXVec3Normalize(&Capsule->vec, &Capsule->vec);
	// ボーンのベクトルとCreateVecの角度(なす角)を計算
	RotDegree = acosf(D3DXVec3Dot(&Capsule->vec, &Capsule->CreateVec));
	// 回転軸のベクトルを計算
	// RotAxisはVec、CreateVecの垂直ベクトル
	D3DXVec3Cross(&RotAxis, &Capsule->vec, &Capsule->CreateVec);
	// 正規化
	D3DXVec3Normalize(&RotAxis, &RotAxis);
	// 回転軸に基づいて-RotDegree角度回転
	D3DXMatrixRotationAxis(&Capsule->WorldMatrix, &RotAxis, -RotDegree);

	// 移動を反映
	// ボーンマトリクスからボーンの座標を取得
	Capsule->pos.x = WorldMatrix->_41;
	Capsule->pos.y = WorldMatrix->_42;
	Capsule->pos.z = WorldMatrix->_43;
	D3DXMatrixTranslation(&mtxTranslate, Capsule->pos.x, Capsule->pos.y, Capsule->pos.z);
	D3DXMatrixMultiply(&Capsule->WorldMatrix, &Capsule->WorldMatrix, &mtxTranslate);

#ifdef _DEBUG
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// オブジェクトのマトリクスを反映
	//D3DXMatrixMultiply(&Capsule->WorldMatrix, &Capsule->WorldMatrix, WorldMatrix);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &Capsule->WorldMatrix);

	// 頂点バッファのセット
	Device->SetStreamSource(0, Capsule->VtxBuffer, 0, sizeof(VERTEX_3D));

	// インデックスバッファのセット
	Device->SetIndices(Capsule->IdxBuffer);

	// FVFのセット
	Device->SetFVF(FVF_VERTEX_3D);

	// 描画
	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, Capsule->NumOfVtxBuffer, 0, Capsule->NumOfTriangle);

	// 終了処理
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

	return;
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexCapsule(CAPSULE *Capsule)
{
	//--  初期設定・範囲チェックなど  --//
	LPDIRECT3DDEVICE9 Device = GetDevice();
	UINT slices = SPHERE_SLICE;
	UINT stacks_1_2 = SPHERE_STACK;

	// デバイス
	if (Device == NULL)
	{
		return E_POINTER;
	}

	// 分割数
	if (slices <= 0)
	{
		return E_INVALIDARG;
	}
	FLOAT invSlices = 1.0f / slices;
	if (stacks_1_2 <= 0)
	{
		return E_INVALIDARG;
	}
	FLOAT invStacks = 1.0f / stacks_1_2;

	// 半径
	if (Capsule->Radius <= 0.0f)
	{
		return E_INVALIDARG;
	}

	// p1とp2が等しい ⇒ 円筒部の長さが0 ⇒ 球体
	if (Capsule->P1 == Capsule->P2)
	{
		// 球体描画で済ませる
		LPD3DXMESH pMesh = NULL;
		D3DXCreateSphere(
			Device,
			Capsule->Radius,
			slices,
			stacks_1_2,
			&pMesh,
			NULL
		);
		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);
		D3DXMatrixTranslation(&m, Capsule->P1.x, Capsule->P1.y, Capsule->P1.z);
		Device->SetTransform(D3DTS_WORLD, &m);
		pMesh->DrawSubset(0);
		pMesh->Release();
		return S_OK;
	}

	//--  カプセル座標の単位ベクトルを作成する  --//

	// vz : P2からP1へ向かう向きのベクトル
	D3DXVECTOR3 uz = Capsule->P1 - Capsule->P2;
	if (D3DXVec3LengthSq(&uz) == 0.0f) return E_INVALIDARG;

	// vy : vzとvと直角なベクトル
	D3DXVECTOR3 uy;
	D3DXVec3Cross(&uy, &uz, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	if (D3DXVec3LengthSq(&uy) == 0.0f)
	{
		MessageBox(0, "Create Capsule Failed！\nCreateVec Can't Use (1.0f, 0.0f, 0.0f)", "Error", 0);
		return E_INVALIDARG;
	}

	D3DXVECTOR3 ux;			// vx : vzとvyと直角なベクトル
	D3DXVec3Cross(&ux, &uz, &uy);
	// vz・vyが0ベクトルでなければvxも0ベクトルではないのでチェックは不要
	//if (D3DXVec3LengthSq(&uy)==0.0f) return E_INVALIDARG;

	// ベクトルの大きさをワールド座標でのradiusにする
	D3DXVec3Normalize(&ux, &ux);
	D3DXVec3Scale(&ux, &ux, -Capsule->Radius);
	D3DXVec3Normalize(&uy, &uy);
	D3DXVec3Scale(&uy, &uy, Capsule->Radius);
	D3DXVec3Normalize(&uz, &uz);
	D3DXVec3Scale(&uz, &uz, Capsule->Radius);

	//--  頂点計算  --//

	// 変数定義
	//LPDIRECT3DVERTEXBUFFER9 pVB;
	HRESULT hr;

	// 頂点バッファの数(格子点+端点)
	//UINT numberOfVB = (1 + slices * stacks_1_2) * 2;
	Capsule->NumOfVtxBuffer = (1 + slices * stacks_1_2) * 2;

	// 頂点バッファ作成
	hr = Device->CreateVertexBuffer(
		sizeof(VERTEX_3D) * Capsule->NumOfVtxBuffer,
		0,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&Capsule->VtxBuffer,
		NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	// 頂点バッファのロック
	VERTEX_3D *pV;
	hr = Capsule->VtxBuffer->Lock(0, 0, (void**)&pV, 0);
	if (FAILED(hr))
	{
		Capsule->VtxBuffer->Release();
		return hr;
	}

	// P1側の計算
	SetVertex(pV, Capsule->P1, uz);
	for (UINT i = 0; i < stacks_1_2; i++)
	{
		FLOAT t = (D3DX_PI*0.5f)*invStacks*(i + 1);
		FLOAT r = sinf(t);
		D3DXVECTOR3 vx = ux * r;
		D3DXVECTOR3 vy = uy * r;
		D3DXVECTOR3 vz = uz * cosf(t);
		for (UINT j = 0; j < slices; j++)
		{
			FLOAT u = (D3DX_PI*2.0f)*invSlices*j;
			SetVertex(pV, Capsule->P1, vx*cosf(u) + vy * sinf(u) + vz);
		}
	}

	// P2側の計算
	for (UINT i = 0; i < stacks_1_2; i++)
	{
		FLOAT t = (D3DX_PI*0.5f)*invStacks*i;
		FLOAT r = cosf(t);
		D3DXVECTOR3 vx = ux * r;
		D3DXVECTOR3 vy = uy * r;
		D3DXVECTOR3 vz = uz * -sinf(t);
		for (UINT j = 0; j < slices; j++)
		{
			FLOAT u = (D3DX_PI*2.0f)*invSlices*j;
			SetVertex(pV, Capsule->P2, vx*cosf(u) + vy * sinf(u) + vz);
		}
	}
	SetVertex(pV, Capsule->P2, -uz);

	// 頂点バッファのアンロック
	hr = Capsule->VtxBuffer->Unlock();
	if (FAILED(hr))
	{
		Capsule->VtxBuffer->Release();
		return hr;
	}

	//--  インデックス  --//

	// 変数定義
	//LPDIRECT3DINDEXBUFFER9 pIB;

	// インデックスバッファの数(片面あたりの四角形ポリゴンの数*2+端点周りの三角形ポリゴンの数*2+筒部のポリゴン数)
	//UINT numberOfTriangle = slices * stacks_1_2 * 2 * 2;
	Capsule->NumOfTriangle = slices * stacks_1_2 * 2 * 2;
	UINT numberOfIB = Capsule->NumOfTriangle * 3;

	//インデックスバッファ作成
	hr = Device->CreateIndexBuffer(sizeof(WORD)*numberOfIB, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &Capsule->IdxBuffer, NULL);
	if (FAILED(hr))
	{
		Capsule->VtxBuffer->Release();
		return hr;
	}

	// インデックスバッファのロック
	WORD* pI;
	hr = Capsule->IdxBuffer->Lock(0, 0, (VOID**)&pI, 0);
	if (FAILED(hr))
	{
		Capsule->IdxBuffer->Release();
		Capsule->VtxBuffer->Release();
		return hr;
	}

	// P1側端点周りの計算
	for (UINT j = 0; j < slices; j++)
	{
		*(pI++) = 0;
		*(pI++) = 1 + j;
		*(pI++) = 1 + (j + 1) % slices;
	}

	// P1-P2間の計算
	UINT imax = stacks_1_2 * 2 - 1;
	for (UINT i = 0; i < imax; i++)
	{
		UINT ibase = 1 + slices * i;
		for (UINT j = 0; j < slices; j++)
		{
			UINT jnext = (j + 1) % slices;
			*(pI++) = ibase + j;
			*(pI++) = ibase + j + slices;
			*(pI++) = ibase + jnext;

			*(pI++) = ibase + jnext;
			*(pI++) = ibase + j + slices;
			*(pI++) = ibase + jnext + slices;
		}
	}

	// P2側端点周りの計算
	UINT ibase = 1 + slices * imax;
	UINT ilast = Capsule->NumOfVtxBuffer - 1;
	for (UINT j = 0; j < slices; j++)
	{
		*(pI++) = ibase + j;
		*(pI++) = ilast;
		*(pI++) = ibase + (j + 1) % slices;
	}

	// インデックスバッファのアンロック
	hr = Capsule->IdxBuffer->Unlock();
	if (FAILED(hr))
	{
		Capsule->IdxBuffer->Release();
		Capsule->VtxBuffer->Release();
		return hr;
	}

	return S_OK;
}

void SetVertex(VERTEX_3D*& verticle, const D3DXVECTOR3& o, const D3DXVECTOR3& v)
{
	verticle->vtx = o + v;
	verticle->nor = v;
	verticle->diffuse = D3DXCOLOR(255,255,255,255);
	verticle++;
}
