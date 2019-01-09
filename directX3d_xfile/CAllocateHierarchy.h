//=============================================================================
//
// メモリ確保処理 [CAllocateHierarchy.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================
#ifndef _CALLOCATEHIERARCHY_H_
#define _CALLOCATEHIERARCHY_H_

#include "main.h"
#include <d3dx9.h>

//--------------------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: Structure derived from D3DXFRAME so we can add some app-specific
//       info that will be stored with each frame
//--------------------------------------------------------------------------------------
struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;

	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}
};

//--------------------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//--------------------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	// array of textures, entries are NULL if no texture specified    
	LPDIRECT3DTEXTURE9*		ppTextures;				// テクスチャ情報

													// SkinMesh info
	LPD3DXMESH				pOrigMesh;				// オリジナルメッシュ
	DWORD					BoneNum;				// ボーンの数
	DWORD					BoneWeightNum;			// １つの頂点に影響を及ぼす重みの数
	LPD3DXBUFFER			pBoneCombinationBuf;	// ボーン情報のバッファ
	D3DXMATRIX**			ppBoneMatrix;			// 全ボーンのマトリックスのポインタの配列
	D3DXMATRIX*				pBoneOffsetMatrices;	// ボーンのオフセット行列
	DWORD					NumPaletteEntries;		// パレットサイズ
};

//--------------------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//--------------------------------------------------------------------------------------
struct AllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
	HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName);
	HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer);
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR						Name,
		CONST D3DXMESHDATA*			pMeshData,
		CONST D3DXMATERIAL*			pMaterials,
		CONST D3DXEFFECTINSTANCE*	pEffectInstances,
		DWORD						NumMaterials,
		CONST DWORD*				pAdjacency,
		LPD3DXSKININFO				pSkinInfo,
		LPD3DXMESHCONTAINER*		ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	AllocateHierarchy()
	{
	}

};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
