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
#define	PLAYER_XFILE		"data/MODEL/kicking/kicking.x"			// �ǂݍ��ރ��f����(�t�@�C���p�X��)
//#define TEXTURE_FILENAME	"data/TEXTURE/00tex_master.BMP"				// �e�N�X�`���̖��O

#define PLAYER_JUMP_SPEED	(10.0f)										// �W�����v�̏���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct {
	LPDIRECT3DTEXTURE9	*D3DTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			D3DXMesh;			// ���b�V�����ւ̃|�C���^
	D3DMATERIAL9		*MeshMaterial = NULL;		// �}�e���A���F�̎w��
	LPD3DXBUFFER		D3DXBuffMat;		// �}�e���A�����ւ̃|�C���^
	DWORD				NumMat;				// �}�e���A�����̐�
	D3DXVECTOR3			pos;				// ���f���̈ʒu
	D3DXVECTOR3			move;				// ���f���̈ړ���
	D3DXVECTOR3			rot;				// ���݂̌���
	D3DXVECTOR3			rotDest;			// �ړI�̌���
	D3DXVECTOR3			scl;				// ���f���̑傫��(�X�P�[��)
	int					IdxShadow;			// �eID
	float				SizeShadow;			// �e�̃T�C�Y
	D3DXCOLOR			ColShadow;			// �e�̐F
	bool				jump;				// �W�����v�t���O
	float				speed;				// �W�����v�̃X�s�[�h
	int					HP;					// �̗�
	int					HPzan;				// �c��̗�
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
