//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "particle.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		pD3DTextureParticle = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffParticle = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

PARTICLE				Particle[MAX_PARTICLE];			// �p�[�e�B�N�����[�N
static bool				g_bAlpaTest;					// �A���t�@�e�X�gON/OFF
static int				g_nAlpha;						// �A���t�@�e�X�g��臒l
D3DXVECTOR3 centerpos;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(int type)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PARTICLE *particle = GetParticle(0);

	// ���_���̍쐬
	MakeVertexParticle(pDevice);

	// �e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFileEx(pDevice,	// �f�o�C�X�ւ̃|�C���^
	//	TEXTURE_PARTICLE,					// �t�@�C���̖��O
	//	D3DX_DEFAULT,						// ����
	//	D3DX_DEFAULT,						// �c��
	//	1,									// �~�b�v�}�b�v���x��
	//	D3DUSAGE_DEPTHSTENCIL,				// �e�N�X�`���̐�����m�ۂ��郁�����̏ꏊ���w��
	//	D3DFMT_UNKNOWN,						// �e�N�X�`���̃s�N�Z���t�H�[�}�b�g���w��
	//	D3DPOOL_DEFAULT,					// �e�N�X�`�����i�[���郁�����̈ʒu�ƊǗ��Ɋւ���t���O
	//	DWORD	Filter,
	//	DWORD	MipFilter,
	//	D3DCOLOR	ColorKey,
	//	D3DXIMAGE_INFO*	pSrcInfo,
	//	PALETTEENTRY*	pPalette,
	//	&pD3DTextureParticle,				// �ǂݍ��ރ������[
	//);

	if (type == 0)
	{
		D3DXCreateTextureFromFile(pDevice,							// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PARTICLE,			// �t�@�C���̖��O
			&pD3DTextureParticle);	// �ǂݍ��ރ������[
	}

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	{
		particle->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		particle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		particle->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		particle->fWidth = PARTICLE_WIDTH;
		particle->fHeight = PARTICLE_HEIGHT;
		particle->bUse = false;
		particle->time = 0;
		particle->theta = 0.0f;
	}

	g_bAlpaTest = false;
	g_nAlpha = 0x0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	PARTICLE *particle = GetParticle(0);

	for(int i = 0; i < MAX_PARTICLE; i++, particle++)
	{
		if (pD3DTextureParticle != NULL)
		{// �e�N�X�`���̊J��
			pD3DTextureParticle->Release();
			pD3DTextureParticle = NULL;
		}

		if (pD3DVtxBuffParticle != NULL)
		{// ���_�o�b�t�@�̊J��
			pD3DVtxBuffParticle->Release();
			pD3DVtxBuffParticle = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle(void)
{
	PARTICLE *particle = GetParticle(0);
	PLAYER *playerWk = GetPlayer();


	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	{
		if (particle->bUse == true)
		{
			// �p�[�e�B�N���̓���
			particle->pos.y -= 5.0f;

			if (particle->pos.y <= 0.0f - PARTICLE_HEIGHT)
			{
				particle->bUse =false;
				ReleaseShadow(particle->nIdxShadow);
			}

			particle->pos.x += particle->move.x;
			particle->pos.z += particle->move.z;

			particle->col.a -= particle->DecAlpha;
			if (particle->col.a <= 0.0f)
			{
				particle->col.a = 0.0f;
			}
			SetColorParticle(nCntParticle,
				D3DXCOLOR(Particle->col.r, particle->col.b,
					particle->col.b, particle->col.a));

			particle->time;
			if (particle->time <= 0)
			{
				particle->bUse = false;
			}

		}
	}

	//============================================================================
	// ���@�w���ۂ���
	//float radius = CIRCLE_RADIUS;
	//if (GetKeyboardPress(DIK_SPACE))
	//{
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//	SetParticle(D3DXVECTOR3(0.0f, 1.0f, 0.0f), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(0.337f, 0.329f, 0.635f, 0.7f));
	//}

	//for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	//{
	//	//1��
	//	if (nCntParticle % 10 == 0)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(CIRCLE_RADIUS,0.0f,0.0f);
	//	}
	//	//2��
	//	else if (nCntParticle % 10 == 1)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(CIRCLE_RADIUS / 2 * sqrtf(2), 0.0f, CIRCLE_RADIUS / 2 * sqrtf(2));
	//	}
	//	//3��
	//	else if (nCntParticle % 10 == 2)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(0.0f, 0.0f, CIRCLE_RADIUS);
	//	}
	//	//4��
	//	else if (nCntParticle % 10 == 3)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(-CIRCLE_RADIUS / 2 * sqrtf(2), 0.0f, CIRCLE_RADIUS / 2 * sqrtf(2));
	//	}
	//	//5��
	//	else if (nCntParticle % 10 == 4)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(-CIRCLE_RADIUS, 0.0f, 0.0f);
	//	}
	//	//6��
	//	else if (nCntParticle % 10 == 5)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(-CIRCLE_RADIUS / 2 * sqrtf(2), 0.0f, -CIRCLE_RADIUS / 2 * sqrtf(2));
	//	}
	//	//7��
	//	else if (nCntParticle % 10 == 6)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(0.0f, 0.0f, -CIRCLE_RADIUS);
	//	}
	//	//8��
	//	else if (nCntParticle % 10 == 7)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(CIRCLE_RADIUS / 2 * sqrtf(2), 0.0f, -CIRCLE_RADIUS / 2 * sqrtf(2));
	//	}
	//	//9��&10����
	//	else if(nCntParticle % 10 == 8 || nCntParticle % 10 == 9)
	//	{
	//		centerpos = playerWk->pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	}

	//	if (nCntParticle % 10 == 8)
	//	{
	//		particle->theta += (1.0f + (rand() % 101 / 100.0f));
	//		particle->pos.x = centerpos.x + (radius * 2 + 10.0f) * cosf(particle->theta);
	//		particle->pos.z = centerpos.z + (radius * 2 + 10.0f) * sinf(particle->theta);
	//	}
	//	else if (nCntParticle % 10 == 9)
	//	{
	//		particle->theta += (1.0f + (rand() % 101 / 100.0f));
	//		particle->pos.x = centerpos.x + (radius * 2 + 20.0f) * cosf(particle->theta);
	//		particle->pos.z = centerpos.z + (radius * 2 + 20.0f) * sinf(particle->theta);
	//	}
	//	else
	//	{
	//		particle->theta += (1.0f + (rand() % 101 / 100.0f));
	//		particle->pos.x = centerpos.x + radius * cosf(particle->theta);
	//		particle->pos.z = centerpos.z + radius * sinf(particle->theta);

	//	}

	//	if (particle->theta == 2.0f)
	//	{
	//		particle->bUse = false;
	//		particle->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	}
	//}
	//============================================================================


	////=====================================================================================================================
	//// ��A�J���~�点��
	//// �p�[�e�B�N���̔���
	//int x = rand() % (RAND_RANGE + 1) - rand() % (RAND_RANGE + 1);
	//int z = rand() % (RAND_RANGE + 1) - rand() % (RAND_RANGE + 1);

	//SetParticle(D3DXVECTOR3((float)x,200.0f,(float)z), PARTICLE_WIDTH, PARTICLE_HEIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	//{
	//	if (particle->bUse == true)
	//	{
	//		// �p�[�e�B�N���̓���
	//		particle->pos.y -= 5.0f;

	//		if (particle->pos.y <= 0.0f - PARTICLE_HEIGHT)
	//		{
	//			particle->bUse =false;
	//			ReleaseShadow(particle->nIdxShadow);
	//		}

	//		//// �F�ύX
	//		//particle->col.a -= 0.01f;

	//		//if (particle->col.a <= 0.0f)
	//		//{
	//		//	particle->bUse = false;
	//		//	ReleaseShadow(particle->nIdxShadow);
	//		//	particle->col.a = 1.0f;
	//		//}

	//		// �e�̈ʒu�ݒ�
	//		SetPositionShadow(particle->nIdxShadow, D3DXVECTOR3(particle->pos.x, 0.1f, particle->pos.z));

	//		// �p�[�e�B�N���̈ʒu�A�F�ύX
	//		SetColorParticle(nCntParticle, particle->col);
	//	}
	//}
	////=====================================================================================================================

	//// �A���t�@�e�X�gON/OFF
	//if (GetKeyboardTrigger(DIK_1))
	//{
	//	g_bAlpaTest = g_bAlpaTest ? false : true;
	//}

	//// �A���t�@�e�X�g��臒l�ύX
	//if (GetKeyboardPress(DIK_I))
	//{
	//	g_nAlpha--;
	//	if (g_nAlpha < 0)
	//	{
	//		g_nAlpha = 0;
	//	}
	//}
	//if (GetKeyboardPress(DIK_K))
	//{
	//	g_nAlpha++;
	//	if (g_nAlpha > 255)
	//	{
	//		g_nAlpha = 255;
	//	}
	//}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA *cameraWk = GetCamera(0);
	MATRIX matrix;
	PARTICLE *particle = GetParticle(0);

	// ���C���e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//// ���Z���� �����_�����O�X�e�[�g�̕ύX�������ۂ��Ȃ�i���Z�����͔����ۂ��Ȃ�i255�ɋ߂Â��Ă����Ɓj�j
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// ���� = �]����(DEST) - �]����(SRC)
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//// Z�e�X�g
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�e�X�g
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	// ���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	{
		if (particle->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&particle->mtxWorld);

			// �r���[�}�g���b�N�X���擾
			matrix.view = cameraWk->mtxView;

			// �|���S���𐳖ʂɌ�����


#if 1
			// �t�s������Ƃ߂�
			D3DXMatrixInverse(&particle->mtxWorld, NULL, &matrix.view);
			particle->mtxWorld._41 = 0.0f;
			particle->mtxWorld._42 = 0.0f;
			particle->mtxWorld._43 = 0.0f;
#else
			particle->mtxWorld._11 = mtxView._11;
			particle->mtxWorld._12 = mtxView._21;
			particle->mtxWorld._13 = mtxView._31;
			particle->mtxWorld._21 = mtxView._12;
			particle->mtxWorld._22 = mtxView._22;
			particle->mtxWorld._23 = mtxView._32;
			particle->mtxWorld._31 = mtxView._13;
			particle->mtxWorld._32 = mtxView._23;
			particle->mtxWorld._33 = mtxView._33;
#endif

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&matrix.scale, particle->scl.x,
				particle->scl.y,
				particle->scl.z);
			D3DXMatrixMultiply(&particle->mtxWorld, &particle->mtxWorld, &matrix.scale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&matrix.translation, particle->pos.x,
				particle->pos.y,
				particle->pos.z);
			D3DXMatrixMultiply(&particle->mtxWorld, &particle->mtxWorld, &matrix.translation);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &particle->mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, pD3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pD3DTextureParticle);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle * 4), NUM_POLYGON);
		}
	}

	// ���C���e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// �ʏ�u�����h �����_�����O�X�e�[�g�����Ƃɖ߂��i�߂��Ȃ��ƌ��Z�����̂܂܂ɂȂ�j
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z��r����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	PARTICLE *particle = GetParticle(0);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuffParticle,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))									// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-PARTICLE_WIDTH / 2.0f, PARTICLE_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(PARTICLE_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_WIDTH / 2.0f, PARTICLE_HEIGHT, 0.0f);

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
		}

		// ���_�f�[�^���A�����b�N����
		pD3DVtxBuffParticle->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexParticle(int nIdxParticle, float fWidth, float fHeight)
{
	PARTICLE *particle = GetParticle(nIdxParticle);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// ���_�f�[�^���A�����b�N����
		pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ� ����(nIdxParticle = �ԍ�, col = �F)
//=============================================================================
void SetColorParticle(int nIdxParticle, D3DXCOLOR col)
{
	PARTICLE *particle = GetParticle(nIdxParticle);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pD3DVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		pD3DVtxBuffParticle->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fWidth, float fHeight, int Time)
{
	PARTICLE *particle = GetParticle(0);
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!particle->bUse)
		{
			particle->pos = pos;
			particle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			particle->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			particle->move = move;
			particle->col = col;
			particle->fWidth = fWidth;
			particle->fHeight = fHeight;
			particle->time = Time;
			particle->DecAlpha = col.a / Time;
			particle->bUse = true;

			// ���_���W�̐ݒ�
			SetVertexParticle(nCntParticle, fWidth, fHeight);

			// ���_�J���[�̐ݒ�
			SetColorParticle(nCntParticle,
				D3DXCOLOR(particle->col.r, particle->col.b,
					particle->col.b, particle->col.a));

			nIdxParticle = nCntParticle;

			break;
		}
	}

	return nIdxParticle;
}

//=============================================================================
// �p�[�e�B�N�����擾
//=============================================================================
PARTICLE *GetParticle(int no)
{
	return &Particle[no];
}

