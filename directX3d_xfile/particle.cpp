//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#include "particle.h"
#include "camera.h"
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
	CHARA *playerWk = GetPlayer();


	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
	{
		// �g�p���Ă�����̂̂ݍX�V
		if (particle->bUse == true)
		{
			// �p�[�e�B�N���̓���
			// �^���ʂɍ��킹�ē���
			particle->pos.x += particle->move.x;
			particle->pos.y += particle->move.y;
			particle->pos.z += particle->move.z;

			// �����x�������l�ɍ��킹�Ēǉ�
			particle->col.a -= particle->DecAlpha;
			if (particle->col.a <= 0.0f)
			{
				// �S�ē����ɂȂ�����g�p����߂�
				particle->col.a = 0.0f;
				particle->bUse = false;
				particle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			// �F�̐ݒ�
			SetColorParticle(nCntParticle,
				D3DXCOLOR(Particle->col.r, particle->col.g,
					particle->col.b, particle->col.a));

			// �\�����Ԃ̍X�V
			particle->time--;
			if (particle->time <= 0)
			{
				particle->bUse = false;
				particle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

		}
	}

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

	// ���Z���� �����_�����O�X�e�[�g�̕ύX�������ۂ��Ȃ�i���Z�����͔����ۂ��Ȃ�i255�ɋ߂Â��Ă����Ɓj�j
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// ���� = �]����(DEST) - �]����(SRC)
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// Z�e�X�g
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
			particle->mtxWorld._11 = matrix.view._11;
			particle->mtxWorld._12 = matrix.view._21;
			particle->mtxWorld._13 = matrix.view._31;
			particle->mtxWorld._21 = matrix.view._12;
			particle->mtxWorld._22 = matrix.view._22;
			particle->mtxWorld._23 = matrix.view._32;
			particle->mtxWorld._31 = matrix.view._13;
			particle->mtxWorld._32 = matrix.view._23;
			particle->mtxWorld._33 = matrix.view._33;

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

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, particle++)
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
			SetColorParticle(nCntParticle,particle->col);

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

//=============================================================================
// �g�p���Ă���p�[�e�B�N���̐��𐔂���
//=============================================================================
int NumParticle(void)
{
	PARTICLE *particle = GetParticle(0);

	int Num = 0;

	for (int i = 0; i < MAX_PARTICLE; i++, particle++)
	{
		if (particle->bUse == true)
		{
			Num++;
		}
	}

	return Num;
}

//=============================================================================
// �U���q�b�g���Ɏg�p����p�[�e�B�N���G�t�F�N�g
//=============================================================================
void SetHitParticle(D3DXVECTOR3 AttackPos)
{
	D3DXVECTOR3 move;

	for (int i = 0; i < HIT_PARTICLE_NUM; i++)
	{
		move.x = (float)(rand() % 10 - 5);
		move.y = (float)(rand() % 10 - 5);
		move.z = (float)(rand() % 10 - 5);

		SetParticle(AttackPos, D3DXVECTOR3(move.x, move.y, move.z),
			ORANGE(0.5f), ATTACK_SIZE_A.x, ATTACK_SIZE_A.y, ATTACK_TIME_C);
	}
}

//=============================================================================
// �ړ����Ɏg�p����p�[�e�B�N���G�t�F�N�g
//=============================================================================
void SetWalkParticle(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 NewPos = pos;
	for (int i = 0; i < MOVE_PARTICLE_NUM; i++)
	{
		int size = rand() % 10;
		NewPos.x += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.y += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.z += (float)(rand() % 20 - 10) * 0.1f;

		SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			SAND(0.15f), (float)size, (float)size, PARTICLE_TIME_A);
		SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			SAND(0.15f), (float)size * 0.5f, (float)size * 0.5f, PARTICLE_TIME_B);
		SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			SAND(0.15f), (float)size * 0.1f, (float)size * 0.1f, PARTICLE_TIME_C);
	}
}

//=============================================================================
// SP�U�����Ɏg�p����p�[�e�B�N���G�t�F�N�g
//=============================================================================
void SetSPattackParticle(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 NewPos = pos;
	for (int i = 0; i < SP_PARTICLE_NUM; i++)
	{
		NewPos.x += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.y += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.z += (float)(rand() % 20 - 10) * 0.1f;

		SetParticle(NewPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			RED(0.50f), ATTACK_SIZE_A.x, ATTACK_SIZE_A.y, ATTACK_TIME_A);
		SetParticle(NewPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			VERMILION(0.30f), ATTACK_SIZE_B.x, ATTACK_SIZE_B.y, ATTACK_TIME_B);
		SetParticle(NewPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			ORANGE(0.15f), ATTACK_SIZE_C.x, ATTACK_SIZE_C.y, ATTACK_TIME_C);
	}
}

//=============================================================================
// �g�����Ɏg�p����p�[�e�B�N���G�t�F�N�g
//=============================================================================
void SetHadouParticle(D3DXVECTOR3 pos, float radius)
{
	D3DXVECTOR3 NewPos;

	for (int angle1 = 0; angle1 < 360; angle1 += 45)
	{
		for (int angle2 = 0; angle2 < 360; angle2 += 45)
		{
			NewPos = pos + GetSpherePosition((float)angle1, (float)angle2, radius);
			SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				VERMILION(0.50f), PARTICLE_SIZE_A.x, PARTICLE_SIZE_A.y, ATTACK_TIME_A);
			SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				VERMILION(0.30f), PARTICLE_SIZE_B.x, PARTICLE_SIZE_B.y, ATTACK_TIME_B);
			SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				VERMILION(0.15f), PARTICLE_SIZE_C.x, PARTICLE_SIZE_C.y, ATTACK_TIME_C);
		}
	}
}

//=============================================================================
// ���̕\�ʏ�̔C�Ӎ��W���擾����
//=============================================================================
D3DXVECTOR3 GetSpherePosition(float Angle1, float Angle2, float Radius)
{
	D3DXVECTOR3 Pos;
	
	Pos.x = Radius * sinf(D3DXToRadian(Angle1)) * cosf(D3DXToRadian(Angle2));
	Pos.y = Radius * sinf(D3DXToRadian(Angle1)) * sinf(D3DXToRadian(Angle2));
	Pos.z = Radius * cosf(D3DXToRadian(Angle1));
	
	return Pos;
}

//=============================================================================
// �K�[�h���Ɏg�p����p�[�e�B�N���G�t�F�N�g
//=============================================================================
void SetGuardParticle(D3DXVECTOR3 DefendPos)
{
	D3DXVECTOR3 move;

	for (int i = 0; i < ATTACK_PARTICLE_NUM; i++)
	{
		move.x = (float)(rand() % 5 - 2);
		move.y = (float)(rand() % 5 - 2);
		move.z = (float)(rand() % 5 - 2);

		SetParticle(DefendPos, D3DXVECTOR3(move.x, move.y, move.z),
			WHITE(0.5f), ATTACK_SIZE_A.x, ATTACK_SIZE_A.y, ATTACK_TIME_C);
	}
}

//=============================================================================
// �p���`�A�L�b�N���Ɏg�p����p�[�e�B�N���G�t�F�N�g
//=============================================================================
void SetAttackParticle(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 NewPos = pos;
	for (int i = 0; i < ATTACK_PARTICLE_NUM; i++)
	{
		NewPos.x += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.y += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.z += (float)(rand() % 20 - 10) * 0.1f;

		SetParticle(NewPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			RED(0.50f), ATTACK_SIZE_A.x, ATTACK_SIZE_A.y, ATTACK_TIME_A);
		SetParticle(NewPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			VERMILION(0.30f), ATTACK_SIZE_B.x, ATTACK_SIZE_B.y, ATTACK_TIME_B);
		SetParticle(NewPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			ORANGE(0.15f), ATTACK_SIZE_C.x, ATTACK_SIZE_C.y, ATTACK_TIME_C);
	}
}

//=============================================================================
// �������Ɏg�p����p�[�e�B�N���G�t�F�N�g
//=============================================================================
void SetShoryuParticle(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 NewPos = pos;
	for (int i = 0; i < ATTACK_PARTICLE_NUM; i++)
	{
		NewPos.x += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.y += (float)(rand() % 20 - 10) * 0.1f;
		NewPos.z += (float)(rand() % 20 - 10) * 0.1f;

		SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			RED(0.50f), ATTACK_SIZE_A.x, ATTACK_SIZE_A.y, ATTACK_TIME_A);
		SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			VERMILION(0.30f), ATTACK_SIZE_B.x, ATTACK_SIZE_B.y, ATTACK_TIME_B);
		SetParticle(NewPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			ORANGE(0.15f), ATTACK_SIZE_C.x, ATTACK_SIZE_C.y, ATTACK_TIME_C);
	}
}