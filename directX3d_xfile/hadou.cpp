//=============================================================================
//
// 波動拳用バレット処理 [hadou.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "battle.h"
#include "player.h"
#include "enemy.h"
#include "hadou.h"
#include "D3DXAnimation.h"
#include "particle.h"
#include "battle.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitHadou(int type, HADOU *Hadou)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 場所、大きさの設定
	Hadou->firstpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Hadou->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Hadou->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Hadou->scl = D3DXVECTOR3(HADOU_SIZE, HADOU_SIZE, HADOU_SIZE);
	Hadou->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Hadou->frame = 0;

	if (type == 0)
	{
		// ポインタの初期化
		Hadou->D3DTexture = NULL;
		Hadou->Mesh = NULL;
		Hadou->D3DXMatBuff = NULL;

		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(BALL_XFILE,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&Hadou->D3DXMatBuff,
			NULL,
			&Hadou->NumMat,
			&Hadou->Mesh)))
		{
			return E_FAIL;
		}

	}
	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitHadou(HADOU *Hadou)
{
	// テクスチャの開放
	SAFE_RELEASE(Hadou->D3DTexture);

	// メッシュの開放
	SAFE_RELEASE(Hadou->Mesh);

	// マテリアルの開放
	SAFE_RELEASE(Hadou->D3DXMatBuff);

}

//=============================================================================
// 更新処理 引数：Hadou:攻撃側波動拳 DefendHadou:防御側波動拳 DefendAnimation:防御側アニメーション HitFrag:波動拳のヒットフラグ
//=============================================================================
void UpdateHadou(HADOU *Hadou)
{
	// 移動計算
	Hadou->move.x -= sinf(Hadou->rot.y) * HADOU_SPEED_A;
	Hadou->move.z -= cosf(Hadou->rot.y) * HADOU_SPEED_A;

	Hadou->pos += Hadou->move;
	Hadou->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 一定距離動いたら消えるように
	D3DXVECTOR3 unit = Hadou->firstpos - Hadou->pos;
	Hadou->dist = D3DXVec3Length(&unit);
	if (Hadou->dist >= MAX_DISTANCE)
	{
		Hadou->use = false;
	}

	// エフェクト
	if (Hadou->use == true)
	{
		D3DXVECTOR3 pos = Hadou->pos;
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.50f), Hadou->scl.x * 2.0f, Hadou->scl.y * 2.0f, PARTICLE_TIME_A);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.30f), Hadou->scl.x * 1.5f, Hadou->scl.y * 1.5f, PARTICLE_TIME_B);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.15f), Hadou->scl.x, Hadou->scl.y, PARTICLE_TIME_C);

		pos.x = Hadou->pos.x;
		pos.y = Hadou->pos.y - Hadou->scl.y * 2.0f;
		pos.z = Hadou->pos.z;
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.50f), Hadou->scl.x * 2.0f, Hadou->scl.y * 2.0f, PARTICLE_TIME_A);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.30f), Hadou->scl.x * 1.5f, Hadou->scl.y * 1.5f, PARTICLE_TIME_B);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.15f), Hadou->scl.x, Hadou->scl.y, PARTICLE_TIME_C);

		pos.x = Hadou->pos.x + Hadou->scl.x;
		pos.y = Hadou->pos.y - Hadou->scl.y;
		pos.z = Hadou->pos.z + Hadou->scl.z;
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.50f), Hadou->scl.x * 2.0f, Hadou->scl.y * 2.0f, PARTICLE_TIME_A);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.30f), Hadou->scl.x * 1.5f, Hadou->scl.y * 1.5f, PARTICLE_TIME_B);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.15f), Hadou->scl.x, Hadou->scl.y, PARTICLE_TIME_C);

		pos.x = Hadou->pos.x - Hadou->scl.x;
		pos.y = Hadou->pos.y - Hadou->scl.y;
		pos.z = Hadou->pos.z - Hadou->scl.z;
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.50f), Hadou->scl.x * 2.0f, Hadou->scl.y * 2.0f, PARTICLE_TIME_A);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.30f), Hadou->scl.x * 1.5f, Hadou->scl.y * 1.5f, PARTICLE_TIME_B);
		SetParticle(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			PURPLE(0.15f), Hadou->scl.x, Hadou->scl.y, PARTICLE_TIME_C);

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHadou(HADOU *Hadou)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&Hadou->mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, Hadou->scl.x, Hadou->scl.y, Hadou->scl.z);
	D3DXMatrixMultiply(&Hadou->mtxWorld, &Hadou->mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Hadou->rot.y, Hadou->rot.x, Hadou->rot.z);
	D3DXMatrixMultiply(&Hadou->mtxWorld, &Hadou->mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, Hadou->pos.x, Hadou->pos.y, Hadou->pos.z);
	D3DXMatrixMultiply(&Hadou->mtxWorld, &Hadou->mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &Hadou->mtxWorld);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)Hadou->D3DXMatBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)Hadou->NumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, Hadou->D3DTexture);

		// 描画
		Hadou->Mesh->DrawSubset(nCntMat);
	}

	{// マテリアルをデフォルトに戻す
		D3DXMATERIAL mat;

		mat.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		mat.MatD3D.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		mat.MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		pDevice->SetMaterial(&mat.MatD3D);
	}
}

//=============================================================================
// 発射処理 引数 Hadou:波動拳 HitBall:攻撃側の当たり判定用ボール CharaRot:攻撃側キャラの回転
//=============================================================================
void SetHadou(HADOU *Hadou, BALL *HitBall, D3DXVECTOR3 CharaRot)
{
	// 一発ごとにしか出せない
	if (Hadou->use == false)
	{
		Hadou->firstpos = (HitBall[RightHand].pos + HitBall[LeftHand].pos) * 0.5f;	// 発射位置を両手の間にする
		Hadou->pos = Hadou->firstpos;
		Hadou->rot = CharaRot;	// 向きをキャラクタに合わせる
		Hadou->dist = 0.0f;
		Hadou->use = true;
	}
}