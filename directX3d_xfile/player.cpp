//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitParts(void);
HRESULT LoadXFile(void);

// アニメーション関連
void InitAnim(void);
void UpdateAnim(void);

void DrawPlayer_RArm(void);
void DrawPlayer_LArm(void);
void DrawPlayer_RFoot(void);
void DrawPlayer_LFoot(void);
void DrawPlayer_Head(void);

D3DXMATRIX *GetPlayerWorldMatrix(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************

PLAYER playerWk[1];

// オブジェクトの作成
//D3DXAnimation* PlayerAnimation;
//AnimationModel* PlayerModel;
//UINT Idle;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(int type)
{
	PLAYER *playerWk = GetPlayer(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//オブジェクトの初期化
	//PlayerAnimation = new D3DXAnimation(pDevice);
	//PlayerModel = new AnimationModel();

	// 位置・回転・スケールの初期設定
	InitParts();

	// アニメーションデータの初期化
	InitAnim();

	playerWk->jump = false;
	playerWk->speed = JUMP_SPEED;

	//Xファイルの読み込み
	//PlayerAnimation->Load_xFile(PLAYER_XFILE);
	//PlayerModel->InitController(PlayerAnimation);

	//複数アニメーション
	//PlayerModel->ChangeAnimation(Idle);

	if (type == 0)
	{
		// Xファイルの読み込み
		LoadXFile();

		// メモリの動的確保
		playerWk->D3DTexture = (LPDIRECT3DTEXTURE9 *)malloc(playerWk->NumMat * sizeof(LPDIRECT3DTEXTURE9));
		playerWk->MeshMaterial = (D3DMATERIAL9 *)malloc(playerWk->NumMat * sizeof(D3DMATERIAL9));

#if 1		
		D3DXMATERIAL *pD3DXMat;

		pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat->GetBufferPointer();

		// テクスチャの読み込み
		for (int i = 0; i < ((signed)playerWk->NumMat); i++)
		{
			playerWk->D3DTexture[i] = NULL;

			D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
				pD3DXMat[i].pTextureFilename,					// ファイルの名前
				&playerWk->D3DTexture[i]);						// 読み込むメモリー
		}
#endif
	}

	playerWk->IdxShadow = CreateShadow(playerWk->pos, SHADOW_SIZE_X, SHADOW_SIZE_Z);
	playerWk->SizeShadow = 25.0f;
	playerWk->ColShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *playerWk = GetPlayer(0);

	for (int i = 0; i < (signed)playerWk->NumMat; i++)
	{
		if (playerWk->D3DTexture != NULL)
		{// テクスチャの開放
			playerWk->D3DTexture[i]->Release();
			playerWk->D3DTexture[i] = NULL;
		}
	}

	if(playerWk->D3DXMesh != NULL)
	{// メッシュの開放
		playerWk->D3DXMesh->Release();
		playerWk->D3DXMesh = NULL;
	}

	if(playerWk->D3DXBuffMat != NULL)
	{// マテリアルの開放
		playerWk->D3DXBuffMat->Release();
		playerWk->D3DXBuffMat = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	PLAYER *playerWk = GetPlayer(0);
	CAMERA *camera = GetCamera(0);
	float fDiffRotY;

	////モデルの更新
	//PlayerModel->AdvanceTime(1.0f / 60);

	//モデルの移動
	// D：右( → )
	if (GetKeyboardPress(DIK_D))
	{
		// D + S：右下( →・↓ )
		if (GetKeyboardPress(DIK_S))
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.75f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.75f;
		}
		// D + W：右上( →・↑ )
		else if (GetKeyboardPress(DIK_W))
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.25f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.25f;
		}
		// D：右( → )
		else
		{
			playerWk->move.x -= sinf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y + D3DX_PI * 0.50f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y + D3DX_PI * 0.50f;
		}
	}
	// A：左( ← )
	else if (GetKeyboardPress(DIK_A))
	{
		// A + S：左下( ←・↓ )
		if (GetKeyboardPress(DIK_S))
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.75f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.75f;
		}
		// A + W：左上( ←・↑ )
		else if (GetKeyboardPress(DIK_W))
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.25f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.25f;
		}
		// A：左( ← )
		else
		{
			playerWk->move.x -= sinf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;
			playerWk->move.z -= cosf(camera->rot.y - D3DX_PI * 0.50f) * VALUE_MOVE;

			playerWk->rotDest.y = camera->rot.y - D3DX_PI * 0.50f;
		}
	}
	// S：下( ↓ )
	else if (GetKeyboardPress(DIK_S))
	{
		playerWk->move.x -= sinf(D3DX_PI + camera->rot.y) * VALUE_MOVE;
		playerWk->move.z -= cosf(D3DX_PI + camera->rot.y) * VALUE_MOVE;

		playerWk->rotDest.y = D3DX_PI + camera->rot.y;
	}
	// W：上( ↑ )
	else if (GetKeyboardPress(DIK_W))
	{
		playerWk->move.x -= sinf(camera->rot.y) * VALUE_MOVE;
		playerWk->move.z -= cosf(camera->rot.y) * VALUE_MOVE;

		playerWk->rotDest.y = camera->rot.y;
	}


	// 目的の角度までの差分
	fDiffRotY = playerWk->rotDest.y - playerWk->rot.y;
	if (fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if (fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	playerWk->rot.y += fDiffRotY * RATE_ROTATE_MODEL;
	if (playerWk->rot.y > D3DX_PI)
	{
		playerWk->rot.y -= D3DX_PI * 2.0f;
	}
	if (playerWk->rot.y < -D3DX_PI)
	{
		playerWk->rot.y += D3DX_PI * 2.0f;
	}

	/// 位置移動
	playerWk->pos.x += playerWk->move.x;
	playerWk->pos.y += playerWk->move.y;
	playerWk->pos.z += playerWk->move.z;

	// 移動量に慣性をかける
	playerWk->move.x += (0.0f - playerWk->move.x) * RATE_MOVE_MODEL;
	playerWk->move.y += (0.0f - playerWk->move.y) * RATE_MOVE_MODEL;
	playerWk->move.z += (0.0f - playerWk->move.z) * RATE_MOVE_MODEL;

	//モデルの拡大縮小
	if (GetKeyboardPress(DIK_C))
	{
		playerWk->scl.x += VALUE_ENLARGEMENT;
		playerWk->scl.y += VALUE_ENLARGEMENT;
		playerWk->scl.z += VALUE_ENLARGEMENT;
	}
	else if (GetKeyboardPress(DIK_Z))
	{
		playerWk->scl.x -= VALUE_ENLARGEMENT;
		playerWk->scl.y -= VALUE_ENLARGEMENT;
		playerWk->scl.z -= VALUE_ENLARGEMENT;
	}

	//モデルのジャンプ
	if (GetKeyboardTrigger(DIK_SPACE) && playerWk->pos.y == 0.0f)
	{
		playerWk->jump = true;
	}
	if (playerWk->jump == true)
	{
		playerWk->pos.y += playerWk->speed;
		playerWk->speed -= 0.98f;
		if (playerWk->pos.y <= 0.0f)
		{
			playerWk->speed = JUMP_SPEED;
			playerWk->pos.y = 0.0f;
			playerWk->jump = false;
		}
	}

	// アニメーションの更新
	UpdateAnim();

	// 影の位置設定
	SetPositionShadow(playerWk->IdxShadow, D3DXVECTOR3(playerWk->pos.x, 0.1f, playerWk->pos.z));
	SetVertexShadow(playerWk->IdxShadow, playerWk->SizeShadow, playerWk->SizeShadow);
	SetColorShadow(playerWk->IdxShadow, playerWk->ColShadow);
}

//=============================================================================
// 描画処理
//=============================================================================
MATRIX g_matrix;

void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_matrix.world);

	// スケールを反映
	D3DXMatrixScaling(&g_matrix.scale, playerWk->scl.x, playerWk->scl.y, playerWk->scl.z);
	D3DXMatrixMultiply(&g_matrix.world, &g_matrix.world, &g_matrix.scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&g_matrix.rotation, playerWk->rotDest.y, playerWk->rotDest.x, playerWk->rotDest.z);
	D3DXMatrixMultiply(&g_matrix.world, &g_matrix.world, &g_matrix.rotation);

	// 移動を反映
	D3DXMatrixTranslation(&g_matrix.translation, playerWk->pos.x, playerWk->pos.y, playerWk->pos.z);
	D3DXMatrixMultiply(&g_matrix.world, &g_matrix.world, &g_matrix.translation);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_matrix.world);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// プレイヤーのレンダリング
	//PlayerModel->SetMatrix(&g_matrix.world);
	//PlayerModel->Render();

	//Draw_Shader();

#if 1
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)playerWk->NumMat; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// 描画
		playerWk->D3DXMesh->DrawSubset(nCntMat);
	}

#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	DrawPlayer_RArm();
	DrawPlayer_LArm();
	DrawPlayer_RFoot();
	DrawPlayer_LFoot();
	DrawPlayer_Head();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer_RArm(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_rarm.x, playerWk->scl_rarm.y, playerWk->scl_rarm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_rarm.y, playerWk->rot_rarm.x, playerWk->rot_rarm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// 移動を反映
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_rarm.x, playerWk->pos_rarm.y, playerWk->pos_rarm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// 胴体のワールドマトリクスと連結する
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_rarm->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_rarm; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// 描画
		playerWk->D3DXMesh_rarm->DrawSubset(nCntMat);
	}

#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer_LArm(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_larm.x, playerWk->scl_larm.y, playerWk->scl_larm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_larm.y, playerWk->rot_larm.x, playerWk->rot_larm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// 移動を反映
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_larm.x, playerWk->pos_larm.y, playerWk->pos_larm.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// 胴体のワールドマトリクスと連結する
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_larm->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_larm; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// 描画
		playerWk->D3DXMesh_larm->DrawSubset(nCntMat);
	}

#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer_RFoot(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_rfoot.x, playerWk->scl_rfoot.y, playerWk->scl_rfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_rfoot.y, playerWk->rot_rfoot.x, playerWk->rot_rfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// 移動を反映
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_rfoot.x, playerWk->pos_rfoot.y, playerWk->pos_rfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// 胴体のワールドマトリクスと連結する
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_rfoot->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_rfoot; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// 描画
		playerWk->D3DXMesh_rfoot->DrawSubset(nCntMat);
	}

#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer_LFoot(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_lfoot.x, playerWk->scl_lfoot.y, playerWk->scl_lfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_lfoot.y, playerWk->rot_lfoot.x, playerWk->rot_lfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// 移動を反映
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_lfoot.x, playerWk->pos_lfoot.y, playerWk->pos_lfoot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// 胴体のワールドマトリクスと連結する
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_lfoot->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_lfoot; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// 描画
		playerWk->D3DXMesh_lfoot->DrawSubset(nCntMat);
	}

#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer_Head(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	MATRIX *matrix = GetMatrix();

	D3DXMATRIX mtxWorld;

	PLAYER *playerWk = GetPlayer(0);
	CAMERA *cameraWk = GetCamera(0);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&matrix->scale, playerWk->scl_head.x, playerWk->scl_head.y, playerWk->scl_head.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->scale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&matrix->rotation, playerWk->rot_head.y, playerWk->rot_head.x, playerWk->rot_head.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->rotation);

	// 移動を反映
	D3DXMatrixTranslation(&matrix->translation, playerWk->pos_head.x, playerWk->pos_head.y, playerWk->pos_head.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &matrix->translation);

	// 胴体のワールドマトリクスと連結する
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerWorldMatrix());

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//player_model->SetMatrix(&mtxWorld);
	//player_model->Render();

#if 1
	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)playerWk->D3DXBuffMat_head->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)playerWk->NumMat_head; nCntMat++)
	{

		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, playerWk->D3DTexture[nCntMat]);

		// 描画
		playerWk->D3DXMesh_head->DrawSubset(nCntMat);
	}

#endif

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//プレイヤーの情報を取得する
//=============================================================================
PLAYER *GetPlayer(int pno)
{
	return &playerWk[pno];
}

//=============================================================================
//プレイヤーのワールドマトリクスを取得する
//=============================================================================
D3DXMATRIX *GetPlayerWorldMatrix(void)
{
	return &g_matrix.world;
}

//=============================================================================
//パーツアニメーションを使う場合の初期化処理
//=============================================================================
void InitParts(void)
{
	// 胴体
	playerWk->pos = D3DXVECTOR3(0.0f, 80.0f, 0.0f);
	playerWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//右腕
	playerWk->pos_rarm = D3DXVECTOR3(-100.0f, 180.0f, 0.0f);
	playerWk->rot_rarm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_rarm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_rarm = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_rarm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//左腕
	playerWk->pos_larm = D3DXVECTOR3(100.0f, 180.0f, 0.0f);
	playerWk->rot_larm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_larm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_larm = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_larm = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//右足
	playerWk->pos_rfoot = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);
	playerWk->rot_rfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_rfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_rfoot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_rfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//左足
	playerWk->pos_lfoot = D3DXVECTOR3(30.0f, 0.0f, 0.0f);
	playerWk->rot_lfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_lfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_lfoot = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_lfoot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頭
	playerWk->pos_head = D3DXVECTOR3(0.0f, 150.0f, 0.0f);
	playerWk->rot_head = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->rotDest_head = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	playerWk->scl_head = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	playerWk->move_head = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//胴体
	playerWk->D3DXMesh = NULL;
	playerWk->D3DXBuffMat = NULL;
	playerWk->NumMat = 0;

	//右腕
	playerWk->D3DXMesh_rarm = NULL;
	playerWk->D3DXBuffMat_rarm = NULL;
	playerWk->NumMat_rarm = 0;

	//左腕
	playerWk->D3DXMesh_larm = NULL;
	playerWk->D3DXBuffMat_larm = NULL;
	playerWk->NumMat_larm = 0;

	//右足
	playerWk->D3DXMesh_rfoot = NULL;
	playerWk->D3DXBuffMat_rfoot = NULL;
	playerWk->NumMat_rfoot = 0;

	//左足
	playerWk->D3DXMesh_lfoot = NULL;
	playerWk->D3DXBuffMat_lfoot = NULL;
	playerWk->NumMat_lfoot = 0;

	//頭
	playerWk->D3DXMesh_head = NULL;
	playerWk->D3DXBuffMat_head = NULL;
	playerWk->NumMat_head = 0;
}

//=============================================================================
//パーツアニメーションを使う場合のXファイル読み込み処理
//=============================================================================
HRESULT LoadXFile(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//胴体
	if (FAILED(D3DXLoadMeshFromX("body.x",			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&playerWk->D3DXBuffMat,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&playerWk->NumMat,		// D3DXMATERIAL構造体の数
		&playerWk->D3DXMesh)))	// ID3DXMeshインターフェイスへのポインタのアドレス=頂点情報の結果が入る
	{
		return E_FAIL;
	}

	//右腕
	if (FAILED(D3DXLoadMeshFromX("rightarm.x",			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&playerWk->D3DXBuffMat_rarm,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&playerWk->NumMat_rarm,		// D3DXMATERIAL構造体の数
		&playerWk->D3DXMesh_rarm)))	// ID3DXMeshインターフェイスへのポインタのアドレス=頂点情報の結果が入る
	{
		return E_FAIL;
	}

	//左腕
	if (FAILED(D3DXLoadMeshFromX("leftarm.x",			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&playerWk->D3DXBuffMat_larm,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&playerWk->NumMat_larm,		// D3DXMATERIAL構造体の数
		&playerWk->D3DXMesh_larm)))	// ID3DXMeshインターフェイスへのポインタのアドレス=頂点情報の結果が入る
	{
		return E_FAIL;
	}

	//右足
	if (FAILED(D3DXLoadMeshFromX("rightfoot.x",			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&playerWk->D3DXBuffMat_rfoot,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&playerWk->NumMat_rfoot,		// D3DXMATERIAL構造体の数
		&playerWk->D3DXMesh_rfoot)))	// ID3DXMeshインターフェイスへのポインタのアドレス=頂点情報の結果が入る
	{
		return E_FAIL;
	}

	//左足
	if (FAILED(D3DXLoadMeshFromX("leftfoot.x",			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&playerWk->D3DXBuffMat_lfoot,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&playerWk->NumMat_lfoot,		// D3DXMATERIAL構造体の数
		&playerWk->D3DXMesh_lfoot)))	// ID3DXMeshインターフェイスへのポインタのアドレス=頂点情報の結果が入る
	{
		return E_FAIL;
	}

	//頭
	if (FAILED(D3DXLoadMeshFromX("head.x",			// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&playerWk->D3DXBuffMat_head,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&playerWk->NumMat_head,		// D3DXMATERIAL構造体の数
		&playerWk->D3DXMesh_head)))	// ID3DXMeshインターフェイスへのポインタのアドレス=頂点情報の結果が入る
	{
		return E_FAIL;
	}

	return S_OK;
}

struct PART {
	PART *parent;
};

PART g_Part[PARTS_MAX];	//パーツの数分オブジェクトを作る

struct KEY {
	D3DXVECTOR3	pos;
	D3DXVECTOR3	rot;
};

// アニメーションデータの定義
struct KEY_ANIME
{
	int			frame;
	KEY			key[PARTS_MAX];
};

// アニメーションデータの宣言
KEY_ANIME g_anim[] = {
	//frame,pos,rot
	{
		//フレーム数
		60, 
	// 胴体
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// 右腕
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// 左腕
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// 右足
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// 左足
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	// 頭
	D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	},
		{
			//フレーム数
			60,
			// 胴体
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			// 右腕
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
			// 左腕
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
			// 右足
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
			// 左足
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
			// 頭
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		},
				{
					//フレーム数
					60,
					// 胴体
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					// 右腕
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
					// 左腕
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					// 右足
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
					// 左足
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
					// 頭
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				},
						{
							//フレーム数
							30,
							// 胴体
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							// 右腕
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
							// 左腕
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
							// 右足
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
							// 左足
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f),
							// 頭
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						},
};

int g_keyMax;					// 位置データの数
float g_MotionTime = 0.0f;		// アニメーション全体時間

//=============================================================================
//アニメーションの初期化処理
//=============================================================================
void InitAnim(void)
{
	//g_Part[0]番=親
	//自分が親ならNULL、それ以外なら親のアドレス
	g_Part[0].parent = NULL;

	//g_Part[1]番=子供
	//自分が親ならNULL、それ以外なら親のアドレス
	g_Part[1].parent = &g_Part[0];
	//g_Part[1]番=子供
	//自分が親ならNULL、それ以外なら親のアドレス
	g_Part[2].parent = &g_Part[0];
	//g_Part[1]番=子供
	//自分が親ならNULL、それ以外なら親のアドレス
	g_Part[3].parent = &g_Part[0];
	//g_Part[1]番=子供
	//自分が親ならNULL、それ以外なら親のアドレス
	g_Part[4].parent = &g_Part[0];
	//g_Part[1]番=子供
	//自分が親ならNULL、それ以外なら親のアドレス
	g_Part[5].parent = &g_Part[0];

	// キーデータの数
	g_keyMax = sizeof(g_anim) / sizeof(KEY_ANIME);	// データのサイズ/データの要素数=個数
}

//=============================================================================
//アニメーションのアップデート処理
//=============================================================================
void UpdateAnim(void)
{
	int i = 0;
	i += (int)g_MotionTime;				// i に全体アニメ時間の整数部分を代入

	// アニメ更新
	g_MotionTime += 1.0f / g_anim[i].frame;

	if (GetKeyState(VK_BACK) & 0x80)
	{
		g_MotionTime = 0.0f;
	}

	if (g_MotionTime > g_keyMax - 1.0f)
	{
		g_MotionTime = g_keyMax - 1.0f;
	}

	if (i > g_keyMax - 2)
	{
		i = g_keyMax - 2;
	}

	for (int j = 0; j < PARTS_MAX; j++)
	{
		// 移動
		// playerWk->pos = g_anim[i].key[j].pos + (g_anim[i + 1].key[j].pos - g_anim[i].key[j].pos) * (g_MotionTime - i);	// 0の位置
																														// 1と0の差分
																														// にアニメーション時間の比率をかけたもの

		// 回転
		// 胴体
		if (j == 0)
		{
			// 一度座標を中心に戻してから回転させる
			playerWk->rot = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
			// また元の位置に戻す
		}
		// 右腕
		if (j == 1)
		{
			playerWk->rot_rarm = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
		// 左腕
		if (j == 2)
		{
			playerWk->rot_larm = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
		// 右足
		if (j == 3)
		{
			playerWk->rot_rfoot = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
		// 左足
		if (j == 4)
		{
			playerWk->rot_lfoot = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
		// 頭
		if (j == 5)
		{
			playerWk->rot_head = g_anim[i].key[j].rot + (g_anim[i + 1].key[j].rot - g_anim[i].key[j].rot) * (g_MotionTime - i);
		}
	}

}