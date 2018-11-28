//=============================================================================
//
// �v���C���[���� [player.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_XFILE		"data/MODEL/KizunaAi.x"						// �ǂݍ��ރ��f����(�t�@�C���p�X��)
//#define	PLAYER_XFILE		"walking.x"								// �ǂݍ��ރ��f����(�t�@�C���p�X��)
#define TEXTURE_FILENAME	"data/TEXTURE/00tex_master.BMP"				// �e�N�X�`���̖��O

#define JUMP_SPEED	(10.0f)												// �W�����v�̏���
#define PARTS_MAX	(6)													// X�t�@�C���̕�����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9	*D3DTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			D3DXMesh;			// ���b�V�����ւ̃|�C���^

	LPD3DXMESH			D3DXMesh_rarm;		// ���b�V�����ւ̃|�C���^
	LPD3DXMESH			D3DXMesh_larm;		// ���b�V�����ւ̃|�C���^
	LPD3DXMESH			D3DXMesh_rfoot;		// ���b�V�����ւ̃|�C���^
	LPD3DXMESH			D3DXMesh_lfoot;		// ���b�V�����ւ̃|�C���^
	LPD3DXMESH			D3DXMesh_head;		// ���b�V�����ւ̃|�C���^

	D3DMATERIAL9		*MeshMaterial = NULL;		// �}�e���A���F�̎w��
	LPD3DXBUFFER		D3DXBuffMat;		// �}�e���A�����ւ̃|�C���^

	LPD3DXBUFFER		D3DXBuffMat_rarm;	// �}�e���A�����ւ̃|�C���^
	LPD3DXBUFFER		D3DXBuffMat_larm;	// �}�e���A�����ւ̃|�C���^
	LPD3DXBUFFER		D3DXBuffMat_rfoot;	// �}�e���A�����ւ̃|�C���^
	LPD3DXBUFFER		D3DXBuffMat_lfoot;	// �}�e���A�����ւ̃|�C���^
	LPD3DXBUFFER		D3DXBuffMat_head;	// �}�e���A�����ւ̃|�C���^

	DWORD				NumMat;				// �}�e���A�����̐�

	DWORD				NumMat_rarm;		// �}�e���A�����̐�
	DWORD				NumMat_larm;		// �}�e���A�����̐�
	DWORD				NumMat_rfoot;		// �}�e���A�����̐�
	DWORD				NumMat_lfoot;		// �}�e���A�����̐�
	DWORD				NumMat_head;		// �}�e���A�����̐�

	D3DXVECTOR3			pos;				// ���f���̈ʒu

	D3DXVECTOR3			pos_rarm;			// ���f���̈ʒu
	D3DXVECTOR3			pos_larm;			// ���f���̈ʒu
	D3DXVECTOR3			pos_rfoot;			// ���f���̈ʒu
	D3DXVECTOR3			pos_lfoot;			// ���f���̈ʒu
	D3DXVECTOR3			pos_head;			// ���f���̈ʒu

	D3DXVECTOR3			move;				// ���f���̈ړ���

	D3DXVECTOR3			move_rarm;			// ���f���̈ړ���
	D3DXVECTOR3			move_larm;			// ���f���̈ړ���
	D3DXVECTOR3			move_rfoot;			// ���f���̈ړ���
	D3DXVECTOR3			move_lfoot;			// ���f���̈ړ���
	D3DXVECTOR3			move_head;			// ���f���̈ړ���

	D3DXVECTOR3			rot;				// ���݂̌���

	D3DXVECTOR3			rot_rarm;			// ���݂̌���
	D3DXVECTOR3			rot_larm;			// ���݂̌���
	D3DXVECTOR3			rot_rfoot;			// ���݂̌���
	D3DXVECTOR3			rot_lfoot;			// ���݂̌���
	D3DXVECTOR3			rot_head;			// ���݂̌���

	D3DXVECTOR3			rotDest;			// �ړI�̌���

	D3DXVECTOR3			rotDest_rarm;		// �ړI�̌���
	D3DXVECTOR3			rotDest_larm;		// �ړI�̌���
	D3DXVECTOR3			rotDest_rfoot;		// �ړI�̌���
	D3DXVECTOR3			rotDest_lfoot;		// �ړI�̌���
	D3DXVECTOR3			rotDest_head;		// �ړI�̌���

	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)

	D3DXVECTOR3			scl_rarm;			// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			scl_larm;			// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			scl_rfoot;			// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			scl_lfoot;			// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			scl_head;			// ���f���̑傫��(�X�P�[��)

	int					IdxShadow;			// �eID
	float				SizeShadow;			// �e�̃T�C�Y
	D3DXCOLOR			ColShadow;			// �e�̐F
	bool				jump;				// �W�����v�t���O
	float				speed;				// �W�����v�̃X�s�[�h
}PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(int type);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(int pno);

#endif
