//=============================================================================
//
// X�t�@�C���ǂݍ��ݏ������� [model.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_BLEND_COUNT 4		// 1�̒��_�ɓK�p�ł���{�[���̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
#pragma once

#pragma warning(disable:4244 4786 4819 4996)
#include <d3dx9.h>
#include <vector>
using namespace std;

// �t���[���̃N���X
class CModelFrame {
public:
	D3DXMATRIXA16 TransformMatrix;								// �ϊ��s��
	D3DXMATRIXA16 OffsetMatrix;									// �I�t�Z�b�g�s��
	D3DXMATRIXA16 CombinedMatrix;								// �����s��
	D3DXMATRIXA16 SkinningMatrix;								// �X�L�j���O�s��
	D3DXMATRIXA16 SmootherMatrix;								// �������s��
	char* Name;													// �t���[����
	vector<CModelFrame*> Child;									// ���t���[���̔z��
	size_t Index;												// �t���[���ԍ�

	CModelFrame(vector<CModelFrame*>* frame);					// �R���X�g���N�^
	void Animate(D3DXMATRIXA16* parent);						// �����s��̌v�Z
};

// �T�u�Z�b�g�̃N���X
class CModelSubset {
public:
	DWORD MaterialIndex;										// �T�u�Z�b�g�ɓK������}�e���A���ԍ�
	DWORD FaceIndex;											// �T�u�Z�b�g���J�n����ʔԍ�
	DWORD FaceCount;											// �T�u�Z�b�g�Ɋ܂܂��ʐ�
};

// �}�e���A���̃N���X
class CModelMaterial {
public:
	D3DXVECTOR4 Color;											// �F
	IDirect3DTexture9* Texture;									// �e�N�X�`��
};

// �A�j���[�V�����L�[�̃N���X
class CModelAnimationKey {
public:
	vector<float> Time;											// ���Ԃ̔z��
	vector<D3DXMATRIXA16*> Matrix;								// �s��̔z��
	char* FrameName;											// �t���[����
	size_t FrameIndex;											// �t���[���ԍ�
};

// �A�j���[�V�����̃N���X
class CModelAnimation {
public:
	char* Name;													// ���O
	vector<CModelAnimationKey*> Key;							// �L�[�̔z��
	float Time;													// ���݂̎���
	float Weight;												// �E�F�C�g

	float GetMaxTime() { return Key.front()->Time.back(); }		// �A�j���[�V�����̒�����Ԃ�
	bool IsOver() { return Time >= GetMaxTime(); }
	CModelAnimation() {}
	CModelAnimation(const CModelAnimation* anim);
};

// ���f���̃N���X
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

// �I���W�i�����_�o�b�t�@�p�̒��_�`����\���\����
struct ORIGINAL_VERTEX {
	D3DXVECTOR3 Pos;
	float Weight[MODEL_BLEND_COUNT];
	unsigned char Index[MODEL_BLEND_COUNT];
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};

// �I���W�i�����_�o�b�t�@�p�̒��_�`����\��FVF
#define ORIGINAL_FVF (D3DFVF_XYZB5|D3DFVF_LASTBETA_UBYTE4|D3DFVF_NORMAL|D3DFVF_TEX1)

// �A�j���[�V�������_�o�b�t�@�p�̒��_�`����\���\����
struct ANIMATED_VERTEX {
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};

// �A�j���[�V�������_�o�b�t�@�p�̒��_�`����\��FVF
#define ANIMATED_FVF (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
