//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : GP11B341 24�@�����a�P
//
//=============================================================================
#ifndef ___SOUND_H___
#define ___SOUND_H___


//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>


//*****************************************************************************
// �}�N�����񋓌^��`
//*****************************************************************************
enum SOUND_NAME//�T�E���h�e�[�u�� --------------------------------------------
{	// SE�i�T�E���h�G�t�F�N�g�j
	SE_DEFEND0,
	SE_HIT0,
	SE_HIT1,
	SE_SWING0,
	SE_SELECT0,
	SE_SELECT1,
	SE_WINNER0,
	SE_KO,
	SE_EFFECT0,
	SE_CUTIN0,
	SE_COUNTDOWN,
	// BGM�i���y�j
	BGM_TITLE,
	BGM_TUTORIAL,
	BGM_TRAINING,
	BGM_BATTLE,

//-----------------------------------------------------------------------------
	SOUND_MAX	// �T�E���h�̐�
};

// �g�����Ɩ������������O�̂��ߒ�` ��ME_01��BGM_01�̖��O���ς������v���f
#define SE_MAX	(ME_WIN)								// SE�̐�
#define ME_MAX	(BGM_TITLE - SE_MAX)					// ME�̐�
#define BGM_MAX	(SOUND_MAX - SE_MAX - ME_MAX)			// BGM�̐�

enum
{	// �Đ��p�t���O
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT					Init_Sound( HWND hWnd );			// �T�E���h�̏���������
void					Uninit_Sound();						// �T�E���h�̏I������
LPDIRECTSOUNDBUFFER8	Load_Sound( int no );				// �T�E���h�̃��[�h
// �����ƂɍĐ�����
void Play_Sound( int no, int type, int flag);

// �����~�߂�
void Stop_Sound(int no, int type);

// �Đ������ǂ������ׂ�
bool Is_Playing( LPDIRECTSOUNDBUFFER8 pBuffer );
// �T�E���h���擾����
LPDIRECTSOUNDBUFFER8 Get_Sound(SOUND_NAME sound_name);

void Reset_Sound(SOUND_NAME sound_name);
#endif