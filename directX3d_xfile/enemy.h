//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	ENEMY_XFILE		"data/MODEL/Rare.x"						// 読み込むモデル名(ファイルパス名)
#define ENEMY_XFILE		"data/MODEL/punching/punching.x"
#define TEXTURE_FILENAME	"data/TEXTURE/00tex_master.BMP"				// テクスチャの名前

#define ENEMY_JUMP_SPEED	(10.0f)	//ジャンプの初速

//*****************************************************************************
// グローバル変数
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9	*D3DTexture = NULL;	// テクスチャへのポインタ
	LPD3DXMESH			D3DXMesh;		// メッシュ情報へのポインタ
	LPD3DXBUFFER		D3DXBuffMat;	// マテリアル情報へのポインタ
	D3DMATERIAL9		*MeshMaterial = NULL;		// マテリアル色の指定
	DWORD				NumMat;			// マテリアル情報の数
	D3DXVECTOR3			pos;			// モデルの位置
	D3DXVECTOR3			move;			// モデルの移動量
	D3DXVECTOR3			rot;			// 現在の向き
	D3DXVECTOR3			rotDest;		// 目的の向き
	D3DXVECTOR3			scl;			// モデルの大きさ(スケール)
	int					IdxShadow;		// 影ID
	float				SizeShadow;		// 影のサイズ
	D3DXCOLOR			ColShadow;		// 影の色
	bool				jump;			// ジャンプフラグ
	float				speed;			// ジャンプのスピード
	int					HP;				// 体力
	int					HPzan;			// 残り体力
}ENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int pno);

#endif
