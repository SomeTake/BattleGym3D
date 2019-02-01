//=============================================================================
//
// 1P2P�\������ [effect.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "effect.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEffect(EFFECT *EffectWk, LPDIRECT3DDEVICE9 pDevice);
void SetVertexEffect(EFFECT *EffectWk, int nIdxEffect, float fWidth, float fHeight);
void SetColorEffect(EFFECT *EffectWk, int nIdxEffect, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(int type, EFFECT *EffectWk)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexEffect(EffectWk, pDevice);

	if (type == 0)
	{
		// �e�N�X�`���̏�����
		D3DXCreateTextureFromFile(pDevice,							// �f�o�C�X�ւ̃|�C���^
			TEXTURE_HIT00,			// �t�@�C���̖��O
			&EffectWk->D3DTexture[hit00]);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,							// �f�o�C�X�ւ̃|�C���^
			TEXTURE_HIT01,			// �t�@�C���̖��O
			&EffectWk->D3DTexture[hit01]);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,							// �f�o�C�X�ւ̃|�C���^
			TEXTURE_GUARD00,			// �t�@�C���̖��O
			&EffectWk->D3DTexture[guard00]);	// �ǂݍ��ރ������[

		D3DXCreateTextureFromFile(pDevice,							// �f�o�C�X�ւ̃|�C���^
			TEXTURE_CHARGE00,			// �t�@�C���̖��O
			&EffectWk->D3DTexture[charge00]);	// �ǂݍ��ރ������[

	}

	EffectWk->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	EffectWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	EffectWk->width = EFFECT_WIDTH;
	EffectWk->height = EFFECT_HEIGHT;
	EffectWk->use = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(EFFECT *EffectWk)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (EffectWk->D3DTexture[i] != NULL)
		{// �e�N�X�`���̊J��
			EffectWk->D3DTexture[i]->Release();
			EffectWk->D3DTexture[i] = NULL;
		}
	}

	if (EffectWk->D3DVtxBuff != NULL)
	{// ���_�o�b�t�@�̊J��
		EffectWk->D3DVtxBuff->Release();
		EffectWk->D3DVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(EFFECT *EffectWk)
{
	
	EffectWk->cntPattern++;
	switch (EffectWk->num)
	{
	case hit00:
		if (EffectWk->cntPattern == ANIM_PATTERN_NUM_HIT00)
		{
			EffectWk->use = false;
		}
		break;
	case hit01:
		if (EffectWk->cntPattern == ANIM_PATTERN_NUM_HIT01)
		{
			EffectWk->use = false;
		}
		break;
	case guard00:
		if (EffectWk->cntPattern == ANIM_PATTERN_NUM_GUARD)
		{
			EffectWk->use = false;
		}
		break;
	case charge00:
		if (EffectWk->cntPattern == ANIM_PATTERN_NUM_CHARGE)
		{
			EffectWk->use = false;
		}
		break;
	default:
		break;
	}

	SetTextureEffect(EffectWk, EffectWk->cntPattern);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(EFFECT *EffectWk)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	CAMERA *cameraWk = GetCamera(0);

	//// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//// Z�e�X�g
	////pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&EffectWk->mtxWorld);

	// �r���[�}�g���b�N�X���擾
	mtxView = cameraWk->mtxView;

	// �|���S���𐳖ʂɌ�����
#if 1
	// �t�s������Ƃ߂�
	D3DXMatrixInverse(&EffectWk->mtxWorld, NULL, &mtxView);
	EffectWk->mtxWorld._41 = 0.0f;
	EffectWk->mtxWorld._42 = 0.0f;
	EffectWk->mtxWorld._43 = 0.0f;
#else
	EffectWk->mtxWorld._11 = mtxView._11;
	EffectWk->mtxWorld._12 = mtxView._21;
	EffectWk->mtxWorld._13 = mtxView._31;
	EffectWk->mtxWorld._21 = mtxView._12;
	EffectWk->mtxWorld._22 = mtxView._22;
	EffectWk->mtxWorld._23 = mtxView._32;
	EffectWk->mtxWorld._31 = mtxView._13;
	EffectWk->mtxWorld._32 = mtxView._23;
	EffectWK.mtxWorld._33 = mtxView._33;
#endif

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, EffectWk->scl.x,
		EffectWk->scl.y,
		EffectWk->scl.z);
	D3DXMatrixMultiply(&EffectWk->mtxWorld, &EffectWk->mtxWorld, &mtxScale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, EffectWk->pos.x,
		EffectWk->pos.y,
		EffectWk->pos.z);
	D3DXMatrixMultiply(&EffectWk->mtxWorld, &EffectWk->mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &EffectWk->mtxWorld);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, EffectWk->D3DVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	switch (EffectWk->num)
	{
	case hit00:
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, EffectWk->D3DTexture[hit00]);
		break;
	case hit01:
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, EffectWk->D3DTexture[hit01]);
		break;
	case guard00:
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, EffectWk->D3DTexture[guard00]);
		break;
	case charge00:
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, EffectWk->D3DTexture[charge00]);
		break;
	default:
		break;
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexEffect(EFFECT *EffectWk, LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,				// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&EffectWk->D3DVtxBuff,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))									// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		EffectWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-EFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-EFFECT_WIDTH / 2.0f, EFFECT_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(EFFECT_WIDTH / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(EFFECT_WIDTH / 2.0f, EFFECT_HEIGHT, 0.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���_�f�[�^���A�����b�N����
		EffectWk->D3DVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEffect(EFFECT *EffectWk, int nIdxEffect, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		EffectWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// ���_�f�[�^���A�����b�N����
		EffectWk->D3DVtxBuff->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorEffect(EFFECT *EffectWk, int nIdxEffect, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		EffectWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		EffectWk->D3DVtxBuff->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetEffect(EFFECT *EffectWk, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int num)
{
	EffectWk->pos = pos;
	EffectWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	EffectWk->width = fWidth;
	EffectWk->height = fHeight;

	// ���_���W�̐ݒ�
	SetVertexEffect(EffectWk, 0, fWidth, fHeight);

	// ���_�J���[�̐ݒ�
	SetColorEffect(EffectWk, 0, col);

	// �g�p����G�t�F�N�g�̎��
	EffectWk->cntPattern = 0;
	EffectWk->num = num;
	EffectWk->use = true;

	return 0;
}

//=============================================================================
// �e�N�X�`�����W�̍쐬
//=============================================================================
void SetTextureEffect(EFFECT *EffectWk, int cntPattern)
{
	int x,y;
	float sizeX, sizeY;

	switch (EffectWk->num)
	{
	case hit00:
		x = cntPattern % HIT00_PATTERN_DIVIDE_X;
		y = cntPattern / HIT00_PATTERN_DIVIDE_X;
		sizeX = 1.0f / HIT00_PATTERN_DIVIDE_X;
		sizeY = 1.0f / HIT00_PATTERN_DIVIDE_Y;
		break;
	case hit01:
		x = cntPattern % HIT01_PATTERN_DIVIDE_X;
		y = cntPattern / HIT01_PATTERN_DIVIDE_X;
		sizeX = 1.0f / HIT01_PATTERN_DIVIDE_X;
		sizeY = 1.0f / HIT01_PATTERN_DIVIDE_Y;
		break;
	case guard00:
		x = cntPattern % GUARD_PATTERN_DIVIDE_X;
		y = cntPattern / GUARD_PATTERN_DIVIDE_X;
		sizeX = 1.0f / GUARD_PATTERN_DIVIDE_X;
		sizeY = 1.0f / GUARD_PATTERN_DIVIDE_Y;
		break;
	case charge00:
		x = cntPattern % CHARGE_PATTERN_DIVIDE_X;
		y = cntPattern / CHARGE_PATTERN_DIVIDE_X;
		sizeX = 1.0f / CHARGE_PATTERN_DIVIDE_X;
		sizeY = 1.0f / CHARGE_PATTERN_DIVIDE_Y;
		break;
	default:
		break;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		EffectWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);

		// ���_�f�[�^���A�����b�N����
		EffectWk->D3DVtxBuff->Unlock();
	}
}