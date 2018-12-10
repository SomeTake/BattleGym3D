//=============================================================================
//
// モデル処理 [D3DXAnimation.h]
// Author : HAL東京 GP11B341-17 80277 染谷武志
//
//=============================================================================

#ifndef _D3DXANIMATION_H_
#define _D3DXANIMATION_H_

#include "CAllocateHierarchy.h"


class D3DXAnimation
{
private:
	IDirect3DDevice9*			m_pDevice;
	CAllocateHierarchy*			m_pAllocateHier;
	LPD3DXFRAME					m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	//D3DXMATRIX*					m_pBoneMatrix;

	//D3DXAnimation*				m_pAnimMesh;
	//LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	D3DXMATRIX					m_Matrix;
	float						m_fSpeed;
	bool						m_bIsLoop;
private:
	
	void DrawMeshContainer(IDirect3DDevice9* pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	void DrawFrame(IDirect3DDevice9* pd3dDevice, LPD3DXFRAME pFrame);
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot);
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

public:
	D3DXAnimation(IDirect3DDevice9* device);
	~D3DXAnimation(void);

	bool Load_xFile(LPCTSTR filename);

	LPD3DXANIMATIONCONTROLLER CloneAnimCtrl(void);

	void Render(const LPD3DXMATRIX matrix);
};

#endif
