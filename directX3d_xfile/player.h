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
#define	PLAYER_XFILE		"data/MODEL/kicking/kicking.x"			// 読み込むモデル名(ファイルパス名)
#define TEXTURE_FILENAME	"data/TEXTURE/00tex_master.BMP"				// テクスチャの名前

#define PLAYER_JUMP_SPEED	(10.0f)										// ジャンプの初速

//*****************************************************************************
// グローバル変数
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9	*D3DTexture = NULL;		// テクスチャへのポインタ
	LPD3DXMESH			D3DXMesh;			// メッシュ情報へのポインタ
	D3DMATERIAL9		*MeshMaterial = NULL;		// マテリアル色の指定
	LPD3DXBUFFER		D3DXBuffMat;		// マテリアル情報へのポインタ
	DWORD				NumMat;				// マテリアル情報の数
	D3DXVECTOR3			pos;				// モデルの位置
	D3DXVECTOR3			move;				// モデルの移動量
	D3DXVECTOR3			rot;				// 現在の向き
	D3DXVECTOR3			rotDest;			// 目的の向き
	D3DXVECTOR3			scl;				// モデルの大きさ(スケール)
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
