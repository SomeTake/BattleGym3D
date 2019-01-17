//=============================================================================
//
// �����蔻��p�J�v�Z������ [CapusuleMesh.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "CapsuleMesh.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetVertex(VERTEX_3D*& verticle, const D3DXVECTOR3& o, const D3DXVECTOR3& v);
HRESULT MakeVertexCapsule(CAPSULE *Capsule);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ����������
//=============================================================================
HRESULT CreateCapsule(CAPSULE *Capsule, D3DXVECTOR3 Pos, D3DXVECTOR3 CreateVec, float Length, float Radius, bool DrawInCenter)
{
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	Capsule->pos = Pos;
	Capsule->CreateVec = CreateVec;
	Capsule->Length = Length;
	Capsule->Radius = Radius;
	Capsule->NumOfVtxBuffer = 0;
	Capsule->NumOfTriangle = 0;
	if (DrawInCenter == true)
	{
		// �J�v�Z�����S����P1�AP2��ݒu
		Capsule->P1 = Capsule->pos + Capsule->CreateVec * Capsule->Length / 2;
		Capsule->P2 = Capsule->pos - Capsule->CreateVec * Capsule->Length / 2;
	}
	else
	{
		// �J�v�Z���̂����[����P1�AP2��ݒu
		Capsule->P1 = Capsule->pos;
		Capsule->P2 = Capsule->pos - Capsule->CreateVec * Capsule->Length;
	}

	// ���_���̍쐬
	if (FAILED(MakeVertexCapsule(Capsule)))
	{
		return E_FAIL;
	}

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitCapsule(CAPSULE *Capsule)
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(Capsule->VtxBuffer);

	// �C���f�b�N�X�o�b�t�@�̊J��
	SAFE_RELEASE(Capsule->IdxBuffer);

	return;
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateCapsule(CAPSULE *Capsule)
{

	return;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawCapsule(CAPSULE *Capsule, const D3DXMATRIX *WorldMatrix)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	// �J�v�Z������]����p�x
	float RotDegree = 0.0f;
	// �J�v�Z���̉�]��
	D3DXVECTOR3 RotAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&Capsule->WorldMatrix);

	// �X�P�[���𔽉f
	//D3DXMatrixScaling(&mtxScl, Capsule->Scale.x, Capsule->Scale.y, Capsule->Scale.z);
	//D3DXMatrixMultiply(&Capsule->WorldMatrix, &Capsule->WorldMatrix, &mtxScl);

	// �{�[���}�g���N�X����A�{�[���̃x�N�g�����擾
	Capsule->vec.x = WorldMatrix->_31;
	Capsule->vec.y = WorldMatrix->_32;
	Capsule->vec.z = WorldMatrix->_33;
	// ���K��
	D3DXVec3Normalize(&Capsule->vec, &Capsule->vec);
	// �{�[���̃x�N�g����CreateVec�̊p�x(�Ȃ��p)���v�Z
	RotDegree = acosf(D3DXVec3Dot(&Capsule->vec, &Capsule->CreateVec));
	// ��]���̃x�N�g�����v�Z
	// RotAxis��Vec�ACreateVec�̐����x�N�g��
	D3DXVec3Cross(&RotAxis, &Capsule->vec, &Capsule->CreateVec);
	// ���K��
	D3DXVec3Normalize(&RotAxis, &RotAxis);
	// ��]���Ɋ�Â���-RotDegree�p�x��]
	D3DXMatrixRotationAxis(&Capsule->WorldMatrix, &RotAxis, -RotDegree);

	// �ړ��𔽉f
	// �{�[���}�g���N�X����{�[���̍��W���擾
	Capsule->pos.x = WorldMatrix->_41;
	Capsule->pos.y = WorldMatrix->_42;
	Capsule->pos.z = WorldMatrix->_43;
	D3DXMatrixTranslation(&mtxTranslate, Capsule->pos.x, Capsule->pos.y, Capsule->pos.z);
	D3DXMatrixMultiply(&Capsule->WorldMatrix, &Capsule->WorldMatrix, &mtxTranslate);

#ifdef _DEBUG
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// �I�u�W�F�N�g�̃}�g���N�X�𔽉f
	//D3DXMatrixMultiply(&Capsule->WorldMatrix, &Capsule->WorldMatrix, WorldMatrix);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &Capsule->WorldMatrix);

	// ���_�o�b�t�@�̃Z�b�g
	Device->SetStreamSource(0, Capsule->VtxBuffer, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�̃Z�b�g
	Device->SetIndices(Capsule->IdxBuffer);

	// FVF�̃Z�b�g
	Device->SetFVF(FVF_VERTEX_3D);

	// �`��
	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, Capsule->NumOfVtxBuffer, 0, Capsule->NumOfTriangle);

	// �I������
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

	return;
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexCapsule(CAPSULE *Capsule)
{
	//--  �����ݒ�E�͈̓`�F�b�N�Ȃ�  --//
	LPDIRECT3DDEVICE9 Device = GetDevice();
	UINT slices = SPHERE_SLICE;
	UINT stacks_1_2 = SPHERE_STACK;

	// �f�o�C�X
	if (Device == NULL)
	{
		return E_POINTER;
	}

	// ������
	if (slices <= 0)
	{
		return E_INVALIDARG;
	}
	FLOAT invSlices = 1.0f / slices;
	if (stacks_1_2 <= 0)
	{
		return E_INVALIDARG;
	}
	FLOAT invStacks = 1.0f / stacks_1_2;

	// ���a
	if (Capsule->Radius <= 0.0f)
	{
		return E_INVALIDARG;
	}

	// p1��p2�������� �� �~�����̒�����0 �� ����
	if (Capsule->P1 == Capsule->P2)
	{
		// ���̕`��ōς܂���
		LPD3DXMESH pMesh = NULL;
		D3DXCreateSphere(
			Device,
			Capsule->Radius,
			slices,
			stacks_1_2,
			&pMesh,
			NULL
		);
		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);
		D3DXMatrixTranslation(&m, Capsule->P1.x, Capsule->P1.y, Capsule->P1.z);
		Device->SetTransform(D3DTS_WORLD, &m);
		pMesh->DrawSubset(0);
		pMesh->Release();
		return S_OK;
	}

	//--  �J�v�Z�����W�̒P�ʃx�N�g�����쐬����  --//

	// vz : P2����P1�֌����������̃x�N�g��
	D3DXVECTOR3 uz = Capsule->P1 - Capsule->P2;
	if (D3DXVec3LengthSq(&uz) == 0.0f) return E_INVALIDARG;

	// vy : vz��v�ƒ��p�ȃx�N�g��
	D3DXVECTOR3 uy;
	D3DXVec3Cross(&uy, &uz, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	if (D3DXVec3LengthSq(&uy) == 0.0f)
	{
		MessageBox(0, "Create Capsule Failed�I\nCreateVec Can't Use (1.0f, 0.0f, 0.0f)", "Error", 0);
		return E_INVALIDARG;
	}

	D3DXVECTOR3 ux;			// vx : vz��vy�ƒ��p�ȃx�N�g��
	D3DXVec3Cross(&ux, &uz, &uy);
	// vz�Evy��0�x�N�g���łȂ����vx��0�x�N�g���ł͂Ȃ��̂Ń`�F�b�N�͕s�v
	//if (D3DXVec3LengthSq(&uy)==0.0f) return E_INVALIDARG;

	// �x�N�g���̑傫�������[���h���W�ł�radius�ɂ���
	D3DXVec3Normalize(&ux, &ux);
	D3DXVec3Scale(&ux, &ux, -Capsule->Radius);
	D3DXVec3Normalize(&uy, &uy);
	D3DXVec3Scale(&uy, &uy, Capsule->Radius);
	D3DXVec3Normalize(&uz, &uz);
	D3DXVec3Scale(&uz, &uz, Capsule->Radius);

	//--  ���_�v�Z  --//

	// �ϐ���`
	//LPDIRECT3DVERTEXBUFFER9 pVB;
	HRESULT hr;

	// ���_�o�b�t�@�̐�(�i�q�_+�[�_)
	//UINT numberOfVB = (1 + slices * stacks_1_2) * 2;
	Capsule->NumOfVtxBuffer = (1 + slices * stacks_1_2) * 2;

	// ���_�o�b�t�@�쐬
	hr = Device->CreateVertexBuffer(
		sizeof(VERTEX_3D) * Capsule->NumOfVtxBuffer,
		0,
		FVF_VERTEX_3D,
		D3DPOOL_DEFAULT,
		&Capsule->VtxBuffer,
		NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	// ���_�o�b�t�@�̃��b�N
	VERTEX_3D *pV;
	hr = Capsule->VtxBuffer->Lock(0, 0, (void**)&pV, 0);
	if (FAILED(hr))
	{
		Capsule->VtxBuffer->Release();
		return hr;
	}

	// P1���̌v�Z
	SetVertex(pV, Capsule->P1, uz);
	for (UINT i = 0; i < stacks_1_2; i++)
	{
		FLOAT t = (D3DX_PI*0.5f)*invStacks*(i + 1);
		FLOAT r = sinf(t);
		D3DXVECTOR3 vx = ux * r;
		D3DXVECTOR3 vy = uy * r;
		D3DXVECTOR3 vz = uz * cosf(t);
		for (UINT j = 0; j < slices; j++)
		{
			FLOAT u = (D3DX_PI*2.0f)*invSlices*j;
			SetVertex(pV, Capsule->P1, vx*cosf(u) + vy * sinf(u) + vz);
		}
	}

	// P2���̌v�Z
	for (UINT i = 0; i < stacks_1_2; i++)
	{
		FLOAT t = (D3DX_PI*0.5f)*invStacks*i;
		FLOAT r = cosf(t);
		D3DXVECTOR3 vx = ux * r;
		D3DXVECTOR3 vy = uy * r;
		D3DXVECTOR3 vz = uz * -sinf(t);
		for (UINT j = 0; j < slices; j++)
		{
			FLOAT u = (D3DX_PI*2.0f)*invSlices*j;
			SetVertex(pV, Capsule->P2, vx*cosf(u) + vy * sinf(u) + vz);
		}
	}
	SetVertex(pV, Capsule->P2, -uz);

	// ���_�o�b�t�@�̃A�����b�N
	hr = Capsule->VtxBuffer->Unlock();
	if (FAILED(hr))
	{
		Capsule->VtxBuffer->Release();
		return hr;
	}

	//--  �C���f�b�N�X  --//

	// �ϐ���`
	//LPDIRECT3DINDEXBUFFER9 pIB;

	// �C���f�b�N�X�o�b�t�@�̐�(�Жʂ�����̎l�p�`�|���S���̐�*2+�[�_����̎O�p�`�|���S���̐�*2+�����̃|���S����)
	//UINT numberOfTriangle = slices * stacks_1_2 * 2 * 2;
	Capsule->NumOfTriangle = slices * stacks_1_2 * 2 * 2;
	UINT numberOfIB = Capsule->NumOfTriangle * 3;

	//�C���f�b�N�X�o�b�t�@�쐬
	hr = Device->CreateIndexBuffer(sizeof(WORD)*numberOfIB, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &Capsule->IdxBuffer, NULL);
	if (FAILED(hr))
	{
		Capsule->VtxBuffer->Release();
		return hr;
	}

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	WORD* pI;
	hr = Capsule->IdxBuffer->Lock(0, 0, (VOID**)&pI, 0);
	if (FAILED(hr))
	{
		Capsule->IdxBuffer->Release();
		Capsule->VtxBuffer->Release();
		return hr;
	}

	// P1���[�_����̌v�Z
	for (UINT j = 0; j < slices; j++)
	{
		*(pI++) = 0;
		*(pI++) = 1 + j;
		*(pI++) = 1 + (j + 1) % slices;
	}

	// P1-P2�Ԃ̌v�Z
	UINT imax = stacks_1_2 * 2 - 1;
	for (UINT i = 0; i < imax; i++)
	{
		UINT ibase = 1 + slices * i;
		for (UINT j = 0; j < slices; j++)
		{
			UINT jnext = (j + 1) % slices;
			*(pI++) = ibase + j;
			*(pI++) = ibase + j + slices;
			*(pI++) = ibase + jnext;

			*(pI++) = ibase + jnext;
			*(pI++) = ibase + j + slices;
			*(pI++) = ibase + jnext + slices;
		}
	}

	// P2���[�_����̌v�Z
	UINT ibase = 1 + slices * imax;
	UINT ilast = Capsule->NumOfVtxBuffer - 1;
	for (UINT j = 0; j < slices; j++)
	{
		*(pI++) = ibase + j;
		*(pI++) = ilast;
		*(pI++) = ibase + (j + 1) % slices;
	}

	// �C���f�b�N�X�o�b�t�@�̃A�����b�N
	hr = Capsule->IdxBuffer->Unlock();
	if (FAILED(hr))
	{
		Capsule->IdxBuffer->Release();
		Capsule->VtxBuffer->Release();
		return hr;
	}

	return S_OK;
}

void SetVertex(VERTEX_3D*& verticle, const D3DXVECTOR3& o, const D3DXVECTOR3& v)
{
	verticle->vtx = o + v;
	verticle->nor = v;
	verticle->diffuse = D3DXCOLOR(255,255,255,255);
	verticle++;
}
