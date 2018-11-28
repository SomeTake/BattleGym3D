//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************

CAMERA cameraWk[MAX_SEPARATE];

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	PLAYER *playerWk = GetPlayer(0);
	ENEMY *enemyWk = GetEnemy(0);
	CAMERA *cameraWk = GetCamera(0);

	for (int i = 0; i < MAX_SEPARATE; i++, cameraWk++)
	{
		cameraWk->pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		if (i == 0)
		{
			cameraWk->at = playerWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);
		}
		else if (i == 1)
		{
			cameraWk->at = enemyWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);
		}
		else
		{
			cameraWk->at = playerWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);
		}
	
		cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		cameraWk->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		float vx, vz;
		vx = cameraWk->pos.x - cameraWk->at.x;
		vz = cameraWk->pos.z - cameraWk->at.z;
		cameraWk->distance = sqrtf(vx * vx + vz * vz);
	}
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *playerWk = GetPlayer(0);
	ENEMY *enemyWk = GetEnemy(0);
	CAMERA *cameraWk = GetCamera(0);

	static float x = POS_X_CAM;
	static float y = POS_Y_CAM;
	static float z = POS_Z_CAM;

	for (int i = 0; i < MAX_SEPARATE; i++, cameraWk++)
	{
		// カメラのY軸回転
		if (GetKeyboardPress(DIK_RIGHT))
		{
			cameraWk->rot.y -= VALUE_ROTATE;
			if (cameraWk->rot.y < -D3DX_PI)
			{
				cameraWk->rot.y = D3DX_PI;
			}
			x = cameraWk->distance * sinf(cameraWk->rot.y);
			z = cameraWk->distance * cosf(cameraWk->rot.y);
		}
		else if (GetKeyboardPress(DIK_LEFT))
		{
			cameraWk->rot.y += VALUE_ROTATE;
			if (cameraWk->rot.y > D3DX_PI)
			{
				cameraWk->rot.y = -D3DX_PI;
			}
			x = cameraWk->distance * sinf(cameraWk->rot.y);
			z = cameraWk->distance * cosf(cameraWk->rot.y);
		}

		// カメラの注視点と視点の距離変更
		if (GetKeyboardPress(DIK_UP))
		{
			if (cameraWk->distance > 5.0f)
			{
				cameraWk->distance -= 5.0f;
			}
			else if (cameraWk->distance == 5.0f)
			{
				cameraWk->distance = 1.0f;
			}
			else if (cameraWk->distance < 5.0f)
			{
				cameraWk->distance = 1.0f;
			}
			x = cameraWk->distance * sinf(cameraWk->rot.y);
			z = cameraWk->distance * cosf(cameraWk->rot.y);
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			cameraWk->distance += 5.0f;
			x = cameraWk->distance * sinf(cameraWk->rot.y);
			z = cameraWk->distance * cosf(cameraWk->rot.y);
		}

		cameraWk->pos = cameraWk->at + D3DXVECTOR3(x, y, z);				// カメラの位置（視点）
		if (i == 0)
		{
			cameraWk->at = playerWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);	// カメラの注視点＝モデルの中心点
		}
		else if(i == 1)
		{ 
			cameraWk->at = enemyWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);	// カメラの注視点＝モデルの中心点
		}
		else
		{
			cameraWk->at = playerWk->pos + D3DXVECTOR3(0.0f, POS_Y_CAM, 0.0f);	// カメラの注視点＝モデルの中心点
		}
		cameraWk->up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);						// 3D空間の上方向はどちら？＝Yが＋方向が上
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(int no)
{
	CAMERA *cameraWk = GetCamera(no);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float asp = GetAspect();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&cameraWk->mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&cameraWk->mtxView,
		&cameraWk->pos,		// カメラの視点
		&cameraWk->at,		// カメラの注視点
		&cameraWk->up);		// カメラの上方向ベクトル

							// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &cameraWk->mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&cameraWk->mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&cameraWk->mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		asp,				// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

							// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &cameraWk->mtxProjection);

}

//=============================================================================
//カメラの情報を取得する
//=============================================================================
CAMERA *GetCamera(int cno)
{
	return &cameraWk[cno];
}
