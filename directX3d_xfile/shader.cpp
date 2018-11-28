//====================================================================================================================================================================================
//
// �V�F�[�_���� [shader.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//====================================================================================================================================================================================

//====================================================================================================================================================================================
// �C���N���[�h
//====================================================================================================================================================================================
#include "main.h"	//	���C���w�b�_�[
#include "camera.h"	//	�J�����w�b�_�[
#include "light.h"	//	���C�g�w�b�_�[
#include "player.h"	//	�v���C���[�w�b�_�[
#include "shader.h"	//	�V�F�[�_�w�b�_�[
//====================================================================================================================================================================================
// �O���[�o���ϐ�
//====================================================================================================================================================================================
SHADER ShaderState;	//	�V�F�[�_
//====================================================================================================================================================================================
// �V�F�[�_�̏���������
// �֐���	�FHRESULT Initialization_Shader
// �߂�l	�FHRESULT
//====================================================================================================================================================================================
HRESULT Initialization_Shader(void)
{
	// �ϐ��錾
	LPD3DXBUFFER Error;	//	�G���[

	// �|�C���^�ϐ��錾 / ������
	LPDIRECT3DDEVICE9 Device = GetDevice();	//	�f�o�C�X�|�C���^

	// �e������
	ShaderState.Effect = NULL;
	ShaderState.ToonTexture = NULL;
	ShaderState.WhiteTexture = NULL;

	// �G�t�F�N�g�̍쐬
	if (FAILED(D3DXCreateEffectFromFile			//	D3DXCreateEffectFromFile�֐��F�G�t�F�N�g��ASCII�L�q�A�܂��̓o�C�i���L�q����G�t�F�N�g���쐬����
	(
		Device,					//	�G�t�F�N�g���쐬����f�o�C�X�ւ̃|�C���^
		SHADER_FILE,			//	�t�@�C����
		NULL,					//	�v���v���Z�b�T�}�N����`��"NULL"�ŏI���I�v�V�����̔z��
		NULL,					//	�I�v�V�����̃C���^�[�t�F�C�X�|�C���^"ID3DXInclude"�F#include�A�[�����߂̏����Ɏg��
		EFFECT_COMPILE_OPTION,	//	�R���p�C���I�v�V����
		NULL,					//	���L�����Ŏg��"ID3DXEffectPool"�I�u�W�F�N�g�ւ̃|�C���^�F���̒l��"NULL"���w�肷��ƁA�����͋��L����Ȃ�
		&ShaderState.Effect,	//	�R���p�C�����ꂽ�G�t�F�N�g���i�[���ꂽ�o�b�t�@�ւ̃|�C���^
		&Error					//	�R���p�C���G���[�̈ꗗ���i�[���ꂽ�o�b�t�@�ւ̃|�C���^
	)))
	{
		MessageBox								//	MessageBox�֐��F���b�Z�[�W�{�b�N�X�̍쐬�A�\���A������s��
		(
			NULL,								//	�쐬���������b�Z�[�W�{�b�N�X�̃I�[�i�[�E�B���h�E�̃n���h�����w�肷��
			(LPCSTR)Error->GetBufferPointer(),	//	�\�����������b�Z�[�W��ێ�����A"NULL"�ŏI��镶����ւ̃|�C���^���w�肷��
			"Error",							//	�_�C�A���O�{�b�N�X�̃^�C�g����ێ�����A"NULL"�ŏI��镶����ւ̃|�C���^���w�肷��
			MB_OK								//	���b�Z�[�W�{�b�N�X�̓��e�Ɠ�����w�肷��
		);
		return E_FAIL;							//	�������s
	}

	// ���\�[�X�̍Ď擾
	ShaderState.Effect->OnResetDevice();		//	OnResetDevice�֐��F���\�[�X���Ď擾���āA������Ԃ�ۑ�����

	// �V�F�[�_�e�N�X�`���̓ǂݍ���
#if 1
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		Device,						//	�f�o�C�X�ւ̃|�C���^
		SHADER_TEXTURE_TOON,		//	�e�N�X�`���t�@�C��
		&ShaderState.ToonTexture	//	�e�N�X�`��������
	);
#endif

	// ����I��
	return S_OK;
}

//====================================================================================================================================================================================
// �V�F�[�_�̉������
// �֐���	�Fvoid Release_Shader
// �߂�l	�Fvoid
//====================================================================================================================================================================================
void Release_Shader(void)
{
	SAFE_RELEASE(ShaderState.Effect);
	SAFE_RELEASE(ShaderState.ToonTexture);
	SAFE_RELEASE(ShaderState.WhiteTexture);
}

//====================================================================================================================================================================================
// �V�F�[�_�̕`�揈��
// �֐���	�Fvoid Draw_Shader
// �߂�l	�Fvoid
//====================================================================================================================================================================================
void Draw_Shader(void)
{
	// �ϐ��錾 / ������
	D3DXMATRIX Calc_Matrix;
	D3DCOLORVALUE Light_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DCOLORVALUE Light_Ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	UINT Pass_No;		//	�p�X�i���o�[
	UINT Pass_Index;	//	�p�X�C���f�b�N�X

	// �|�C���^�ϐ��錾 / ������
	LPDIRECT3DDEVICE9 Device = GetDevice();	//	�f�o�C�X�|�C���^
	MATRIX *Matrix = GetMatrix();				//	�}�g���N�X�|�C���^
	PLAYER *Player = GetPlayer(0);				//	�v���C���[�|�C���^
	D3DLIGHT9 *Light = GetLight(0);			//	���C�g�|�C���^
	
	// �g�D�[���V�F�[�_

	// �e�N�j�b�N�̐ݒ�
	ShaderState.Effect->SetTechnique(ShaderState.Effect->GetTechniqueByName("Toon_Shader"));	//	SetTechnique�֐��F�A�N�e�B�u�ȃe�N�j�b�N��ݒ肷��
	// �}�g���N�X�̐ݒ�
	ShaderState.Effect->SetMatrix("Matrix_World", &Matrix->world);		//	SetMatrix�֐��F��]�u�s���ݒ肷��
	// �}�g���N�X�̌v�Z
	Calc_Matrix = Matrix->world * Matrix->view * Matrix->projection;
	// �}�g���N�X�̐ݒ�
	ShaderState.Effect->SetMatrix("Matrix_Crucible", &Calc_Matrix);	//	SetMatrix�֐��F��]�u�s���ݒ肷��

	// �X�e�[�g�̐ݒ�
	// �s�N�Z���V�F�[�_���L����( �Œ蒸�_���g�p���� )�ꍇ
#if ENABLE_PIXEL_SHADER == 0
	// �e�N�X�`���X�e�[�W�O�̐ݒ�
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DSAMP_MIPFILTER);
	Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	// �e�N�X�`���X�e�[�W�P�̐ݒ�
	Device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DSAMP_MIPFILTER);
	Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
#endif
	Device->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	Device->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);

	// �e�N�j�b�N�̓K�p���J�n
	ShaderState.Effect->Begin(&Pass_No, 0);

	// ���C�g�̐ݒ�
	ShaderState.Effect->SetValue("Light_Color_Diffuse", &(Light + 0)->Diffuse, sizeof(D3DCOLORVALUE));	//	SetValue�֐��F�C�ӂ̃p�����[�^�A�܂��̓A�m�e�[�V�����̒l���擾����
	ShaderState.Effect->SetValue("Light_Color_Ambient", &Light_Ambient, sizeof(D3DCOLORVALUE));
	ShaderState.Effect->SetValue("Light_Direction", &(Light + 0)->Direction, sizeof(D3DXVECTOR3));

	// �`��
	for (Pass_Index = 0; Pass_Index < Pass_No; Pass_Index++)
	{
		// �e�N�j�b�N���Ńp�X���J�n����
		ShaderState.Effect->BeginPass(Pass_Index);	//	BeginPass�֐��F�A�N�e�B�u�ȃe�N�j�b�N���ŁA�p�X���J�n����

		// ���b�V���̓T�u�Z�b�g��A�e�ޗ��̂��߂̂P�ɕ�������Ă���̂ŁA���������[�v�����Ń����_�����O����
		for (DWORD i = 0; i < Player->NumMat; i++)
		{
			// ���̃T�u�Z�b�g�̃}�e���A���ƁA�e�N�X�`����ݒ肷��
			// �s�N�Z���V�F�[�_���L����( �Œ蒸�_���g�p���� )�ꍇ
#if ENABLE_PIXEL_SHADER == 0
			// �Œ蒸�_���g�p����ꍇ
			Device->SetTexture(0, Player->D3DTexture[i]);
			Device->SetTexture(1, ShaderState.ToonTexture);

			if (Player->D3DTexture[i])
			{
				Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			}
			else
			{
				Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			}
			// �s�N�Z���V�F�[�_���g�p����ꍇ
#else
			// �e�N�X�`���̐ݒ�
			Shader_State.Effect->SetTexture("Shade_Texture", Shader_State.Toon_Texture);

			if (Player->Direct3D_Texture[i])
			{
				Shader_State.Effect->SetTexture("Mesh_Texture", Player->Direct3D_Texture[i]);
			}
			else
			{
				Shader_State.Effect->SetTexture("Mesh_Texture", Shader_State.White_Texture);
			}
#endif
			// �}�e���A���J���[�̐ݒ�
			ShaderState.Effect->SetValue("Material_Color_Ambient", &Player->MeshMaterial[i].Ambient, sizeof(D3DCOLORVALUE));
			ShaderState.Effect->SetValue("Material_Color_Diffuse", &Player->MeshMaterial[i].Diffuse, sizeof(D3DCOLORVALUE));

			// �p�X���̏������f�o�C�X�ɐݒ�
			ShaderState.Effect->CommitChanges();	//	CommitChanges�֐��F�����_�����O����O�ɁA�A�N�e�B�u�ȃp�X�̓����Ŕ��������Ԃ̕ω����A�f�o�C�X�ɓ`�d����

			// ���b�V���̃T�u�Z�b�g��`��
			Player->D3DXMesh->DrawSubset(i);
		}
		// �A�N�e�B�u�p�X�̏I��
		ShaderState.Effect->EndPass();	//	EndPass�֐��F�A�N�e�B�u�p�X���I������
	}
	// �A�N�e�B�u�ȃe�N�j�b�N�̏I��
	ShaderState.Effect->End();	//	End�֐��F�A�N�e�B�u�ȃe�N�j�b�N���I������

	// �����_�����O�X�e�[�g�p�����[�^�̍Đݒ�
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	Device->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}