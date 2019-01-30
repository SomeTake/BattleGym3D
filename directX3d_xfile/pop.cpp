//=============================================================================
//
// 1P2P�\������ [pop.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "main.h"
#include "pop.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPop(POP *PopWk, LPDIRECT3DDEVICE9 pDevice);
void SetVertexPop(POP *PopWk, int nIdxPop, float fWidth, float fHeight);
void SetColorPop(POP *PopWk, int nIdxPop, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPop(int type, POP *PopWk, int ControllerNum)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexPop(PopWk, pDevice);

	if (type == 0)
	{
		// �e�N�X�`���̏�����
		switch (ControllerNum)
		{
		case 0:
			D3DXCreateTextureFromFile(pDevice,							// �f�o�C�X�ւ̃|�C���^
				TEXTURE_POP1P,			// �t�@�C���̖��O
				&PopWk->D3DTexture);	// �ǂݍ��ރ������[
			break;
		case 1:
			D3DXCreateTextureFromFile(pDevice,							// �f�o�C�X�ւ̃|�C���^
				TEXTURE_POP2P,			// �t�@�C���̖��O
				&PopWk->D3DTexture);	// �ǂݍ��ރ������[
			break;
		default:
			break;
		}
	}

	PopWk->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PopWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	PopWk->width = POP_WIDTH;
	PopWk->height = POP_HEIGHT;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPop(POP *PopWk)
{
	if (PopWk->D3DTexture != NULL)
	{// �e�N�X�`���̊J��
		PopWk->D3DTexture->Release();
		PopWk->D3DTexture = NULL;
	}

	if (PopWk->D3DVtxBuff != NULL)
	{// ���_�o�b�t�@�̊J��
		PopWk->D3DVtxBuff->Release();
		PopWk->D3DVtxBuff = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePop(POP *PopWk, D3DXVECTOR3 pos)
{
	PopWk->pos = pos + POP_POS;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPop(POP *PopWk)
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
	D3DXMatrixIdentity(&PopWk->mtxWorld);

	// �r���[�}�g���b�N�X���擾
	mtxView = cameraWk->mtxView;

	// �|���S���𐳖ʂɌ�����
#if 1
	// �t�s������Ƃ߂�
	D3DXMatrixInverse(&PopWk->mtxWorld, NULL, &mtxView);
	PopWk->mtxWorld._41 = 0.0f;
	PopWk->mtxWorld._42 = 0.0f;
	PopWk->mtxWorld._43 = 0.0f;
#else
	PopWk->mtxWorld._11 = mtxView._11;
	PopWk->mtxWorld._12 = mtxView._21;
	PopWk->mtxWorld._13 = mtxView._31;
	PopWk->mtxWorld._21 = mtxView._12;
	PopWk->mtxWorld._22 = mtxView._22;
	PopWk->mtxWorld._23 = mtxView._32;
	PopWk->mtxWorld._31 = mtxView._13;
	PopWk->mtxWorld._32 = mtxView._23;
	PopWK.mtxWorld._33 = mtxView._33;
#endif

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScale, PopWk->scl.x,
		PopWk->scl.y,
		PopWk->scl.z);
	D3DXMatrixMultiply(&PopWk->mtxWorld, &PopWk->mtxWorld, &mtxScale);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, PopWk->pos.x,
		PopWk->pos.y,
		PopWk->pos.z);
	D3DXMatrixMultiply(&PopWk->mtxWorld, &PopWk->mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &PopWk->mtxWorld);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, PopWk->D3DVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, PopWk->D3DTexture);

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
HRESULT MakeVertexPop(POP *PopWk, LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,				// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&PopWk->D3DVtxBuff,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))									// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		PopWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-POP_WIDTH / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-POP_WIDTH / 2.0f, POP_HEIGHT, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(POP_WIDTH / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(POP_WIDTH / 2.0f, POP_HEIGHT, 0.0f);

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
		PopWk->D3DVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPop(POP *PopWk, int nIdxPop, float fWidth, float fHeight)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		PopWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// ���_�f�[�^���A�����b�N����
		PopWk->D3DVtxBuff->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorPop(POP *PopWk, int nIdxPop, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		PopWk->D3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		PopWk->D3DVtxBuff->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetPop(POP *PopWk, D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col)
{
	PopWk->pos = pos;
	PopWk->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	PopWk->width = fWidth;
	PopWk->height = fHeight;

	// ���_���W�̐ݒ�
	SetVertexPop(PopWk, 0, fWidth, fHeight);

	// ���_�J���[�̐ݒ�
	SetColorPop(PopWk, 0, col);

	return 0;
}
