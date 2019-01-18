//=============================================================================
//
// カメラ処理 [camera.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_X_CAM		(0.0f)											// カメラの初期位置(X座標)
#define	POS_Y_CAM		(10.0f)											// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(150.0f)										// カメラの初期位置(Z座標)

#define AT_X_CAM		(0.0f)											// カメラの注視点(X座標)
#define AT_Y_CAM		(50.0f)											// カメラの注視点(Y座標)
#define AT_Z_CAM		(0.0f)											// カメラの注視点(Z座標)

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// カメラの回転量

#define	INTERVAL_CAMERA_R	(12.5f)										// モデルの視線の先までの距離
#define	RATE_CHASE_CAMERA_P	(0.35f)										// カメラの視点への補正係数
#define	RATE_CHASE_CAMERA_R	(0.20f)										// カメラの注視点への補正係数

#define	CHASE_HEIGHT_P		(100.0f)									// 追跡時の視点の高さ
#define	CHASE_HEIGHT_R		(10.0f)										// 追跡時の注視点の高さ

#define CAMERA_SPD		(0.05f)											// 古いカメラ位置と新しいカメラ位置に差がある場合にカメラ位置を更新するスピード
																		// ２つのカメラ位置のベクトル＊CAMERA_SPDずつ毎フレーム新しいカメラ位置が更新される

//*****************************************************************************
// グローバル変数
//*****************************************************************************

typedef struct {
	D3DXVECTOR3			pos;		// カメラの視点
	D3DXVECTOR3			at;			// カメラの注視点
	D3DXVECTOR3			up;			// カメラの上方向ベクトル
	D3DXVECTOR3			rot;		// カメラの回転
	float				distance;	// カメラの注視点と視点との距離
	D3DXMATRIX			mtxView;			// ビューマトリックス
	D3DXMATRIX			mtxProjection;		// プロジェクションマトリックス
}CAMERA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UpdateCamera(void);
CAMERA *GetCamera(int cno);
void SetCamera(int no);
D3DXVECTOR3 GetCenterPos(void);		// PとEの間の位置を取得する
float GetPEdistance(void);			// PとEの距離を取得する

#endif
