//=============================================================================
//
// Xファイル読み込み処理処理 [model.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_BLEND_COUNT 4		// 1つの頂点に適用できるボーンの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
#pragma once

#pragma warning(disable:4244 4786 4819 4996)
#include <d3dx9.h>
#include <vector>
using namespace std;

// フレームのクラス
class CModelFrame {
public:
	D3DXMATRIXA16 TransformMatrix;								// 変換行列
	D3DXMATRIXA16 OffsetMatrix;									// オフセット行列
	D3DXMATRIXA16 CombinedMatrix;								// 合成行列
	D3DXMATRIXA16 SkinningMatrix;								// スキニング行列
	D3DXMATRIXA16 SmootherMatrix;								// 平滑化行列
	char* Name;													// フレーム名
	vector<CModelFrame*> Child;									// 小フレームの配列
	size_t Index;												// フレーム番号

	CModelFrame(vector<CModelFrame*>* frame);					// コンストラクタ
	void Animate(D3DXMATRIXA16* parent);						// 合成行列の計算
};

// サブセットのクラス
class CModelSubset {
public:
	DWORD MaterialIndex;										// サブセットに適応するマテリアル番号
	DWORD FaceIndex;											// サブセットを開始する面番号
	DWORD FaceCount;											// サブセットに含まれる面数
};

// マテリアルのクラス
class CModelMaterial {
public:
	D3DXVECTOR4 Color;											// 色
	IDirect3DTexture9* Texture;									// テクスチャ
};

// アニメーションキーのクラス
class CModelAnimationKey {
public:
	vector<float> Time;											// 時間の配列
	vector<D3DXMATRIXA16*> Matrix;								// 行列の配列
	char* FrameName;											// フレーム名
	size_t FrameIndex;											// フレーム番号
};

// アニメーションのクラス
class CModelAnimation {
public:
	char* Name;													// 名前
	vector<CModelAnimationKey*> Key;							// キーの配列
	float Time;													// 現在の時間
	float Weight;												// ウェイト

	float GetMaxTime() { return Key.front()->Time.back(); }		// アニメーションの長さを返す
	bool IsOver() { return Time >= GetMaxTime(); }
	CModelAnimation() {}
	CModelAnimation(const CModelAnimation* anim);
};

// モデルのクラス
class CModel {
public:
	IDirect3DDevice9 * Device;
	IDirect3DVertexBuffer9* OriginalVertexBuffer;
	IDirect3DVertexBuffer9* AnimatedVertexBuffer;
	DWORD VertexCount;
	IDirect3DIndexBuffer9* IndexBuffer;
	DWORD FaceCount;
	vector<CModelFrame*> Frame;
	vector<CModelSubset*> Subset;
	vector<CModelMaterial*> Material;
	vector<CModelAnimation*> Animation;
	float SmootherWeight;

	CModel(IDirect3DDevice9* device, const char* file_name, bool anim_only);
	void AnimateFrame(D3DXMATRIXA16* world);
	void AnimateVertex();
	void Draw(ID3DXEffect* effect, D3DXMATRIXA16* view, D3DXMATRIXA16* proj);
	void Draw(D3DXMATRIXA16* view, D3DXMATRIXA16* proj);

	CModelFrame* FindFrame(const char* name);
	DWORD GetFrameCount() { return (DWORD)Frame.size(); }
	CModelFrame* GetFrame(DWORD index) { return Frame[index]; }

	CModelAnimation* FindAnimation(const char* name);
	void AddAnimation(CModelAnimation* anim);
	void ResetAnimationWeight();
	DWORD GetAnimationCount() { return (DWORD)Animation.size(); }
	CModelAnimation* GetAnimation(DWORD index) { return Animation[index]; }

	void SaveSmoother();

	//	void ReverseAnimationX(int anim_index);
};

extern IDirect3DTexture9* GetTexture(IDirect3DDevice9* device, const char* file_name);

// オリジナル頂点バッファ用の頂点形式を表す構造体
struct ORIGINAL_VERTEX {
	D3DXVECTOR3 Pos;
	float Weight[MODEL_BLEND_COUNT];
	unsigned char Index[MODEL_BLEND_COUNT];
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};

// オリジナル頂点バッファ用の頂点形式を表すFVF
#define ORIGINAL_FVF (D3DFVF_XYZB5|D3DFVF_LASTBETA_UBYTE4|D3DFVF_NORMAL|D3DFVF_TEX1)

// アニメーション頂点バッファ用の頂点形式を表す構造体
struct ANIMATED_VERTEX {
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};

// アニメーション頂点バッファ用の頂点形式を表すFVF
#define ANIMATED_FVF (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
