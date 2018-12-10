//=============================================================================
// �T�E���h���� [sound.cpp]
//
// Author : GP11B341 24�@�����a�P
// �쐬�� : 2018/8/1
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "sound.h"

// ���܂��Ȃ�
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )


//*****************************************************************************
// �T�E���h�t�@�C���̃p�X
//*****************************************************************************
// �T�E���h�t�@�C���̃p�X�ih�̒ʂ��i���o�[�ƍ��킹�邱�Ɓj
const TCHAR* c_soundFilename[] = {
	// SE
	_T("data/SOUND/SE/Defend0.wav"),
	_T("data/SOUND/SE/Hit0.wav"),
	_T("data/SOUND/SE/Hit1.wav"),
	_T("data/SOUND/SE/Swing0.wav"),
	_T("data/SOUND/SE/Select0.wav"),
	_T("data/SOUND/SE/Select1.wav"),
	_T("data/SOUND/SE/yattaze0.wav"),
	_T("data/SOUND/SE/KO.wav"),
	_T("data/SOUND/SE/effect0.wav"),
	_T("data/SOUND/SE/cutin0.wav"),
	_T("data/SOUND/SE/countdown0.wav"),
	// BGM
	_T("data/SOUND/BGM/title.wav"),
	_T("data/SOUND/BGM/tutorial.wav"),
	_T("data/SOUND/BGM/training.wav"),
	_T("data/SOUND/BGM/battle.wav"),
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IDirectSound8 *g_pDirectSound = NULL;			// �T�E���h�C���^�[�t�F�[�X
LPDIRECTSOUNDBUFFER8 sound[SOUND_MAX];			// �T�E���h�p�o�b�t�@


//=============================================================================
// ����������
//=============================================================================
// hWnd:�E�B���h�E�n���h��
HRESULT Init_Sound( HWND hWnd )
{
	// DirectSound�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectSoundCreate8(NULL, &g_pDirectSound, NULL)))
		return E_FAIL;
	
	// �������x���ݒ� 
	if (FAILED(g_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;

	//���[�h
	int i;
	for (i = 0; i < SOUND_MAX; i++)
	{
		sound[i] = Load_Sound(i);
	}


	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void Uninit_Sound()
{
	// �T�E���h�p�o�b�t�@�̉��
	int i;
	for (i = 0; i < SOUND_MAX; i++)
	{
		SAFE_RELEASE(sound[i])
	}

	// ���T�E���h�p�o�b�t�@�����ɍs��
	if( g_pDirectSound != NULL )
	{
		g_pDirectSound->Release();
		g_pDirectSound = NULL;
	}
}



//=============================================================================
// �T�E���h�̃��[�h
//=============================================================================
// no:�T�E���h�i���o�[�i�w�b�_�ɒ�`���ꂽ�񋓌^�萔�j
LPDIRECTSOUNDBUFFER8 Load_Sound( int no )
{
	// MMIO = �}���`���f�B�A���o�́A�̗��B
	LPDIRECTSOUNDBUFFER  pBaseBuffer = NULL;	// �ȃf�[�^�̑����o�b�t�@
	LPDIRECTSOUNDBUFFER8 pBuffer = NULL;		// �ȃf�[�^�̃o�b�t�@
	DSBUFFERDESC buff;							// �o�b�t�@�ݒ�\����

	HMMIO hMmio = NULL;							// MMIO�n���h��
	MMIOINFO mmioInfo;							// �}���`���f�B�A�f�[�^�\����

	MMRESULT mmRes;								// MM�������ʊi�[�ϐ�
	MMCKINFO riffChunk, formatChunk, dataChunk;	// �`�����N�f�[�^�\����
	DWORD size;									// �f�[�^�T�C�Y�i�[�p
	WAVEFORMATEX pcm;							// �ȃf�[�^�t�H�[�}�b�g�\����

	LPVOID pBlock;								// �Z�J���_���o�b�t�@�̃f�[�^�������ݐ�A�h���X
	DWORD  dwSize;								// �Z�J���_���o�b�t�@�̃T�C�Y

	// 1.�n���h�������ƂɃt�@�C�����J��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen((LPSTR)c_soundFilename[no], &mmioInfo, MMIO_READ);
	if(!hMmio)
		return NULL;

	// 2.�t�@�C����͇@ RIFF�`�����N����
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);			// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 3.�t�@�C����͇A �t�H�[�}�b�g�`�����N����
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);	// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR)		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);				// �����������Ƃɓǂݍ���
	
	if(size != formatChunk.cksize) 		// �������ǂݍ��߂Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0); 

	// 4.�t�@�C����͇B �f�[�^�`�����N����
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');						// �����L�[���[�h
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);		// �L�[���[�h�����ƂɌ���
	
	if(mmRes != MMSYSERR_NOERROR) 		// ������Ȃ�������ُ�I��
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// 5.�f�[�^�ǂݍ���		
	char *pData = new char[dataChunk.cksize];								// �K�v�ȑ傫���̗̈���m�ۂ���
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);					// �f�[�^��ǂݍ���
	
	if(size != dataChunk.cksize)  		// �������ǂݍ��߂Ȃ�������ُ�I��
	{
		delete[] pData;
		return NULL;
	}

	// 6.�Ȃ�ǂݍ��ށu�Z�J���_���o�b�t�@�v��p��
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));								// �܂�������
	buff.dwSize = sizeof(DSBUFFERDESC);										// ��������e��ݒ�
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// �����o�b�t�@�����
	if (FAILED(g_pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
		return NULL;

	// �T�E���h�o�b�t�@�����o��
	if (FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&pBuffer)))
		return NULL;

	// ���o�����瑍���o�b�t�@���̂Ă�
	pBaseBuffer->Release();

	// 7.���ꂽ�Z�J���_���o�b�t�@�Ƀf�[�^��]��
	// �܂��͏������݂ł���悤�o�b�t�@�����b�N
	if (FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(pBlock, pData, dwSize);	// �f�[�^�]��
	delete[] pData;					// ���̓ǂݍ��ݗ̈������

	// �o�b�t�@���b�N����
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// �Z�J���_���o�b�t�@��Ԃ��Ă悤�₭����...
	return pBuffer;
}


//=============================================================================
// ����炷
//=============================================================================
// pBuffer:����炵�����T�E���h�f�[�^�̃Z�J���_���o�b�t�@
// flag   :1(E_DS8_FLAG_LOOP)�Ȃ烋�[�v�Đ�
void Play_Sound(  int no, int type, int flag/*=0*/ )
{	// ��������炷�̂ŁA�ŏ�����炵�����ꍇ��SetCurrentPosition(0)�����邱��

	if (type == 0)
	{
		// �Đ��ʒu��擪�ɖ߂�
		// �����T�E���h�𖳉����ԂȂ��A���Đ����邽��
		sound[no]->SetCurrentPosition(0);
	}
	/*	�Z�b�e�B���O0�ɂ���Ɗ֐����ĂԂ��ƂɍĐ��ʒu���擪�ɂ��ǂ�̂�
	BGM��A���Ŕ���������ʉ��̓Z�b�e�B���O��1�ɂ���				 */


	sound[no]->Play(0, 0, flag);
}


//=============================================================================
// �����~�߂�
//=============================================================================
void Stop_Sound( int no, int type)
{

	DWORD status;

	sound[no]->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )	// ���Ă�����
	{
		sound[no]->Stop();	// �Ӗ��I�ɂ�Pause�ɂȂ�B
	}

	// ���̌�ɂ�������s���Ȃ��ƍĐ��ʒu������������Ȃ�
	if (type == 0)
	{
		// �Đ��ʒu��擪�ɖ߂�
		// �����T�E���h�𖳉����ԂȂ��A���Đ����邽��
		sound[no]->SetCurrentPosition(0);
	}
	/*	�Z�b�e�B���O0�ɂ���Ɗ֐����ĂԂ��ƂɍĐ��ʒu���擪�ɂ��ǂ�̂�
	BGM��A���Ŕ���������ʉ��̓Z�b�e�B���O��1�ɂ���				 */

}


//=============================================================================
// �Đ������ǂ������ׂ�
//=============================================================================
// pBuffer:����炵�����T�E���h�f�[�^�̃Z�J���_���o�b�t�@
bool Is_Playing( LPDIRECTSOUNDBUFFER8 pBuffer)
{
	DWORD status;

	pBuffer->GetStatus(&status);
	if( status & DSBSTATUS_PLAYING )
	{
		return true;
	}
	return false;
}


//=============================================================================
// �T�E���h�擾�֐�
// [����]	: �T�E���h���擾����
// [�߂�l]	: LPDIRECTSOUNDBUFFER8�^�@�T�E���h�p�o�b�t�@�ւ̃|�C���^
// [����]	: SOUND_NAME�^ �T�E���h�l�[���̔ԍ�, int�^ �T�E���h�̃Z�b�e�B���O
//=============================================================================
LPDIRECTSOUNDBUFFER8 Get_Sound(SOUND_NAME sound_name)
{
	return	sound[sound_name];
}


void Reset_Sound(SOUND_NAME sound_name)
{

	LPDIRECTSOUNDBUFFER8 buffer = Get_Sound(sound_name);
	// �Đ��ʒu��擪�ɖ߂�
	// �����T�E���h�𖳉����ԂȂ��A���Đ����邽��
	buffer->SetCurrentPosition(0);
}
