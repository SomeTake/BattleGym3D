//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_XFILE		"data/MODEL/KizunaAi.x"						// 読み込むモデル名(ファイルパス名)
//#define	PLAYER_XFILE		"walking.x"								// 読み込むモデル名(ファイルパス名)
#define TEXTURE_FILENAME	"data/TEXTURE/00tex_master.BMP"				// テクスチャの名前

#define JUMP_SPEED	(10.0f)												// ジャンプの初速
#define PARTS_MAX	(6)													// Xファイルの分割数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9	*D3DTexture = NULL;		// テクスチャへのポインタ
	LPD3DXMESH			D3DXMesh;			// メッシュ情報へのポインタ

	LPD3DXMESH			D3DXMesh_rarm;		// メッシュ情報へのポインタ
	LPD3DXMESH			D3DXMesh_larm;		// メッシュ情報へのポインタ
	LPD3DXMESH			D3DXMesh_rfoot;		// メッシュ情報へのポインタ
	LPD3DXMESH			D3DXMesh_lfoot;		// メッシュ情報へのポインタ
	LPD3DXMESH			D3DXMesh_head;		// メッシュ情報へのポインタ

	D3DMATERIAL9		*MeshMaterial = NULL;		// マテリアル色の指定
	LPD3DXBUFFER		D3DXBuffMat;		// マテリアル情報へのポインタ

	LPD3DXBUFFER		D3DXBuffMat_rarm;	// マテリアル情報へのポインタ
	LPD3DXBUFFER		D3DXBuffMat_larm;	// マテリアル情報へのポインタ
	LPD3DXBUFFER		D3DXBuffMat_rfoot;	// マテリアル情報へのポインタ
	LPD3DXBUFFER		D3DXBuffMat_lfoot;	// マテリアル情報へのポインタ
	LPD3DXBUFFER		D3DXBuffMat_head;	// マテリアル情報へのポインタ

	DWORD				NumMat;				// マテリアル情報の数

	DWORD				NumMat_rarm;		// マテリアル情報の数
	DWORD				NumMat_larm;		// マテリアル情報の数
	DWORD				NumMat_rfoot;		// マテリアル情報の数
	DWORD				NumMat_lfoot;		// マテリアル情報の数
	DWORD				NumMat_head;		// マテリアル情報の数

	D3DXVECTOR3			pos;				// モデルの位置

	D3DXVECTOR3			pos_rarm;			// モデルの位置
	D3DXVECTOR3			pos_larm;			// モデルの位置
	D3DXVECTOR3			pos_rfoot;			// モデルの位置
	D3DXVECTOR3			pos_lfoot;			// モデルの位置
	D3DXVECTOR3			pos_head;			// モデルの位置

	D3DXVECTOR3			move;				// モデルの移動量

	D3DXVECTOR3			move_rarm;			// モデルの移動量
	D3DXVECTOR3			move_larm;			// モデルの移動量
	D3DXVECTOR3			move_rfoot;			// モデルの移動量
	D3DXVECTOR3			move_lfoot;			// モデルの移動量
	D3DXVECTOR3			move_head;			// モデルの移動量

	D3DXVECTOR3			rot;				// 現在の向き

	D3DXVECTOR3			rot_rarm;			// 現在の向き
	D3DXVECTOR3			rot_larm;			// 現在の向き
	D3DXVECTOR3			rot_rfoot;			// 現在の向き
	D3DXVECTOR3			rot_lfoot;			// 現在の向き
	D3DXVECTOR3			rot_head;			// 現在の向き

	D3DXVECTOR3			rotDest;			// 目的の向き

	D3DXVECTOR3			rotDest_rarm;		// 目的の向き
	D3DXVECTOR3			rotDest_larm;		// 目的の向き
	D3DXVECTOR3			rotDest_rfoot;		// 目的の向き
	D3DXVECTOR3			rotDest_lfoot;		// 目的の向き
	D3DXVECTOR3			rotDest_head;		// 目的の向き

	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)

	D3DXVECTOR3			scl_rarm;			// モデルの大きさ(スケール)
	D3DXVECTOR3			scl_larm;			// モデルの大きさ(スケール)
	D3DXVECTOR3			scl_rfoot;			// モデルの大きさ(スケール)
	D3DXVECTOR3			scl_lfoot;			// モデルの大きさ(スケール)
	D3DXVECTOR3			scl_head;			// モデルの大きさ(スケール)

	int					IdxShadow;			// 影ID
	float				SizeShadow;			// 影のサイズ
	D3DXCOLOR			ColShadow;			// 影の色
	bool				jump;				// ジャンプフラグ
	float				speed;				// ジャンプのスピード
}PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(int pno);

#endif
