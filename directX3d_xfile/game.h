//=============================================================================
//
// �Q�[������ [game.h]
// Author : HAL���� GP11B341-17 80277 ���J���u
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#define HITSTOP_TIME	(7)		// �q�b�g�X�g�b�v�����鎞��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
bool GetHitStop(void);
void SetHitStop(void);

#endif