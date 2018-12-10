//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	ENEMY_XFILE		"data/MODEL/Rare.x"						// �ǂݍ��ރ��f����(�t�@�C���p�X��)
#define ENEMY_XFILE		"data/MODEL/punching/punching.x"
#define TEXTURE_FILENAME	"data/TEXTURE/00tex_master.BMP"				// �e�N�X�`���̖��O

#define ENEMY_JUMP_SPEED	(10.0f)	//�W�����v�̏���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9	*D3DTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			D3DXMesh;		// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		D3DXBuffMat;	// �}�e���A�����ւ̃|�C���^
	D3DMATERIAL9		*MeshMaterial = NULL;		// �}�e���A���F�̎w��
	DWORD				NumMat;			// �}�e���A�����̐�
	D3DXVECTOR3			pos;			// ���f���̈ʒu
	D3DXVECTOR3			move;			// ���f���̈ړ���
	D3DXVECTOR3			rot;			// ���݂̌���
	D3DXVECTOR3			rotDest;		// �ړI�̌���
	D3DXVECTOR3			scl;			// ���f���̑傫��(�X�P�[��)
	int					IdxShadow;		// �eID
	float				SizeShadow;		// �e�̃T�C�Y
	D3DXCOLOR			ColShadow;		// �e�̐F
	bool				jump;			// �W�����v�t���O
	float				speed;			// �W�����v�̃X�s�[�h
	int					HP;				// �̗�
	int					HPzan;			// �c��̗�
}ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(int type);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(int pno);

#endif
